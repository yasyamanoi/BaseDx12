#include "stdafx.h"
#include "Scene.h"

namespace basedx12 {


	IMPLEMENT_DX12SHADER(BcVSPNTStaticPLShadow, App::GetShadersPath() + L"BcVSPNTStaticPLShadow.cso")
	IMPLEMENT_DX12SHADER(BcPSPNTPLShadow, App::GetShadersPath() + L"BcPSPNTPLShadow.cso")

	IMPLEMENT_DX12SHADER(VSShadowmap, App::GetShadersPath() + L"VSShadowmap.cso")

	void FixedBox::OnInit() {
		auto scene = App::GetTypedSceneBase<Scene>();
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//メッシュ
		{
			vector<VertexPositionNormalTexture> vertices;
			vector<uint32_t> indices;
			MeshUtill::CreateCube(1.0f, vertices, indices);
			m_mesh = BaseMesh::CreateBaseMesh<VertexPositionNormalTexture>(vertices, indices);
		}
		//Bcコンスタントバッファ
		m_bcConstBuffIndex = baseDevice->GetCbvSrvUavNextIndex();
		//コンスタントバッファハンドルを作成
		CD3DX12_CPU_DESCRIPTOR_HANDLE BcHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_bcConstBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		m_bcConstantBuffer = ConstantBuffer::CreateDirect(BcHandle, m_bcShadowSceneConstantsData);
		//テクスチャ
		{
			auto TexFile = App::GetRelativeAssetsPath() + L"sky.jpg";
			//テクスチャの作成
			//シェーダリソースハンドルを作成
			m_srvIndex = baseDevice->GetCbvSrvUavNextIndex();
			CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
				baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
				m_srvIndex,
				baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
			//画像ファイルをもとにテクスチャを作成
			m_skyTexture = BaseTexture::CreateBaseTexture(TexFile, srvHandle);
		}
	}

	void FixedBox::OnDraw() {
		auto scene = App::GetTypedSceneBase<Scene>();
		scene.SetBcConstants(
			m_scale,
			m_qt,
			m_pos,
			m_bcShadowSceneConstantsData);
		m_bcConstantBuffer->Copy(m_bcShadowSceneConstantsData);
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//Srv(t1)
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"t1"), SrvHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_bcConstBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"b0"), CbvHandle);
		//パイプライステート
		commandList->SetPipelineState(scene.GetBcPipelineState().Get());
		m_skyTexture->UpdateSRAndCreateSRV(commandList);
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_mesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_mesh->GetNumIndices(), 1, 0, 0, 0);

	}


	void MoveBox::OnInit() {
		auto scene = App::GetTypedSceneBase<Scene>();
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//メッシュ
		{
			vector<VertexPositionNormalTexture> vertices;
			vector<uint32_t> indices;
			MeshUtill::CreateCube(1.0f, vertices, indices);
			m_mesh = BaseMesh::CreateBaseMesh<VertexPositionNormalTexture>(vertices, indices);
		}
		//シャドウマップコンスタントバッファ
		m_shadowmapConstBuffIndex = baseDevice->GetCbvSrvUavNextIndex();
		//シャドウマップコンスタントバッファハンドルを作成
		CD3DX12_CPU_DESCRIPTOR_HANDLE shadowmapHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_shadowmapConstBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		m_shadowmapConstantBuffer = ConstantBuffer::CreateDirect(shadowmapHandle, m_shadowmapConstantsData);
		//Bcコンスタントバッファ
		m_bcConstBuffIndex = baseDevice->GetCbvSrvUavNextIndex();
		//コンスタントバッファハンドルを作成
		CD3DX12_CPU_DESCRIPTOR_HANDLE BcHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_bcConstBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		m_bcConstantBuffer = ConstantBuffer::CreateDirect(BcHandle, m_bcShadowSceneConstantsData);
		//テクスチャ
		{
			auto TexFile = App::GetRelativeAssetsPath() + L"wall.jpg";
			//テクスチャの作成
			//シェーダリソースハンドルを作成
			m_srvIndex = baseDevice->GetCbvSrvUavNextIndex();
			CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
				baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
				m_srvIndex,
				baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
			//画像ファイルをもとにテクスチャを作成
			m_wallTexture = BaseTexture::CreateBaseTexture(TexFile, srvHandle);
		}

	}

	void MoveBox::SetShadowmapConstants() {
		auto scene = App::GetTypedSceneBase<Scene>();
		Mat4x4 worldMatrix,viewMatrix,projMatrix;
		worldMatrix.affineTransformation(
			m_scale,
			Float3(0.0f),
			m_qt,
			m_pos
		);
		//ライトの位置
		Float3 lightPos = scene.GetLightPos(0);
		Float3 lightAt = scene.GetCamera()->GetAt();
		Float3 lightUp(0, 0, 1);
		viewMatrix.lookatLH(lightPos, lightAt, lightUp);
		float w = (float)App::GetGameWidth();
		float h = (float)App::GetGameHeight();
		float aspectRatio = w / h;
		projMatrix.perspectiveFovLH(XM_PIDIV4, aspectRatio, 1.0f, 100.0f);
		m_shadowmapConstantsData.world = worldMatrix.transpose();
		m_shadowmapConstantsData.view = viewMatrix.transpose();
		m_shadowmapConstantsData.proj = projMatrix.transpose();
	}

	void MoveBox::OnUpdate() {
		Quat qtspan(Float3(0, 1, 1), -0.02f);
		m_qt *= qtspan;
		m_qt.normalize();
		m_pos.x += m_posSpan;
		if (abs(m_pos.x) >= 5.0f) {
			m_posSpan *= -1.0f;
		}
	}

	void MoveBox::OnDrawShadowmap() {
		auto scene = App::GetTypedSceneBase<Scene>();
		SetShadowmapConstants();
		m_shadowmapConstantBuffer->Copy(m_shadowmapConstantsData);
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//Cbv（b0）
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_shadowmapConstBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"b0"), CbvHandle);
		commandList->SetPipelineState(scene.GetShadowmapPipelineState().Get());
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_mesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_mesh->GetNumIndices(), 1, 0, 0, 0);

	}


	void MoveBox::OnDraw() {
		auto scene = App::GetTypedSceneBase<Scene>();
		scene.SetBcConstants(
			m_scale,
			m_qt,
			m_pos,
			m_bcShadowSceneConstantsData);
		m_bcConstantBuffer->Copy(m_bcShadowSceneConstantsData);

		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//Srv（t1）
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
		//t1==1
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"t1"), SrvHandle);
		//Cbv（b0）
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_bcConstBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"b0"), CbvHandle);
		commandList->SetPipelineState(scene.GetBcPipelineState().Get());
		m_wallTexture->UpdateSRAndCreateSRV(commandList);
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_mesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_mesh->GetNumIndices(), 1, 0, 0, 0);
	}

	void Scene::SetBcConstants(
		const Float3& scale,
		const Quat& qt,
		const Float3& pos,
		BasicConstants& ret
	) {
		//ライトを取得
		Float3 lightPos = m_lightPos[0];
		//行列の定義
		Mat4x4 world;
		world.affineTransformation(
			scale,
			Float3(0.0f),
			qt,
			pos
		);
		//初期化
		ret = {};
		auto view = m_camera->GetViewMatrix();
		auto proj = m_camera->GetProjectionMatrix();
		auto worldView = world * view;
		ret.worldViewProj = XMMatrixTranspose(XMMatrixMultiply(worldView, proj));
		if (m_fogEnabled) {
			auto start = -5.0f;
			auto end = -20.0f;
			if (start == end)
			{
				// Degenerate case: force everything to 100% fogged if start and end are the same.
				static const XMVECTORF32 fullyFogged = { 0, 0, 0, 1 };
				ret.fogVector = fullyFogged;
			}
			else
			{
				XMMATRIX worldViewTrans = worldView;
				// _13, _23, _33, _43
				XMVECTOR worldViewZ = XMVectorMergeXY(XMVectorMergeZW(worldViewTrans.r[0], worldViewTrans.r[2]),
					XMVectorMergeZW(worldViewTrans.r[1], worldViewTrans.r[3]));
				XMVECTOR wOffset = XMVectorSwizzle<1, 2, 3, 0>(XMLoadFloat(&start));
				ret.fogVector = (worldViewZ + wOffset) / (start - end);
			}
			ret.fogColor = Float4(0.8f, 0.8f, 0.8f, 1.0f);

		}
		else {
			ret.fogVector = g_XMZero;
			ret.fogColor = g_XMZero;
		}

		//ライトの決定
		for (UINT i = 0; i < 3; i++) {
			ret.lightDirection[i] = Float3(0.0) - m_lightPos[i];
			ret.lightDirection[i].normalize();
			ret.lightDiffuseColor[i] = Float4(0.7,0.7,0.7,1.0);
			ret.lightSpecularColor[i] = Float4(0.0);
		}
		//ワールド行列
		ret.world = XMMatrixTranspose(world);

		XMMATRIX worldInverse = XMMatrixInverse(nullptr, world);
		ret.worldInverseTranspose[0] = worldInverse.r[0];
		ret.worldInverseTranspose[1] = worldInverse.r[1];
		ret.worldInverseTranspose[2] = worldInverse.r[2];

		XMMATRIX viewInverse = XMMatrixInverse(nullptr, view);
		ret.eyePosition = viewInverse.r[3];

		XMVECTOR diffuse = Float4(0.8f);
		XMVECTOR alphaVector = XMVectorReplicate(1.0f);
		XMVECTOR emissiveColor = Float4(0.0f);
		XMVECTOR ambientLightColor = Float4(0, 0, 0, 1.0);

		ret.emissiveColor = (emissiveColor + ambientLightColor * diffuse) * alphaVector;
		ret.specularColorAndPower = Float4(0, 0, 0, 1);

		// xyz = diffuse * alpha, w = alpha.
		ret.diffuseColor = XMVectorSelect(alphaVector, diffuse * alphaVector, g_XMSelect1110);

		Float3 CalcLightDir = Float3(0.0) - m_lightPos[0];
		CalcLightDir.normalize();
		Float3 LightAt(m_camera->GetAt());
		Float3 LightEye(m_lightPos[0]);
		LightEye = LightAt + LightEye;
		Float4 LightEye4(LightEye, 1.0f);
		LightEye4.w = 1.0f;
		ret.lightPos = LightEye4;
		Float4 eyePos4(m_camera->GetEye(), 1.0f);
		eyePos4.w = 1.0f;
		ret.eyePos = eyePos4;
		bsm::Mat4x4 LightView, LightProj;
		//ライトのビューと射影を計算
		LightView = XMMatrixLookAtLH(LightEye, LightAt, Float3(0, 0.0, 1.0));
		float w = (float)App::GetGameWidth();
		float h = (float)App::GetGameHeight();
		float aspectRatio = w / h;
		LightProj = XMMatrixPerspectiveFovLH(XM_PIDIV4, aspectRatio, 1.0f, 100.0f);
		ret.lightView = bsm::transpose(LightView);
		ret.lightProjection = bsm::transpose(LightProj);

	}


	void Scene::OnInit() {
		//フレーム数は3
		ResetActiveBaseDevice<GameDevice>(3);
	}
	void Scene::OnInitAssets() {
		auto baseDevice = App::GetBaseDevice();
		//サンプラーは共有
		{
			//テスクチャ描画用サンプラー
			CD3DX12_CPU_DESCRIPTOR_HANDLE SamplerDescriptorHandle(
				baseDevice->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
				0,
				baseDevice->GetSamplerDescriptorHandleIncrementSize()
			);
			Sampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
			//シャドウ描画用サンプラー
			CD3DX12_CPU_DESCRIPTOR_HANDLE ShadowSamplerDescriptorHandle(
				baseDevice->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
				1,
				baseDevice->GetSamplerDescriptorHandleIncrementSize()
			);
			Sampler::CreateSampler(SamplerState::ComparisonLinear, ShadowSamplerDescriptorHandle);
		}
		//パイプラインは共有
		{
			D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
			//パイプライステート
			m_shadowmapPipelineState
				= PipelineState::CreateShadowmap3D<VertexPositionNormalTexture, VSShadowmap>(baseDevice->GetRootSignature(), PipeLineDesc);
			//パイプライステート
			m_bcSceneShadowPipelineState
				= PipelineState::CreateDefault3D<VertexPositionNormalTexture, BcVSPNTStaticPLShadow, BcPSPNTPLShadow>(baseDevice->GetRootSignature(), PipeLineDesc);

		}
		//カメラとライトは共有
		{
			m_camera = Camera::CreateCamera<Camera>(Float3(0, 3.0f, -5.0f), Float3(0, 0, 0));
			m_lightPos[0] = Float3(0.0f, 10.0f, 5.0);
			m_lightPos[1] = Float3(10.0f, 10.0f, -10.0);
			m_lightPos[2] = Float3(-10.0f, 10.0f, -10.0);
		}
		// それぞれのオブジェクトの初期化
		{
			m_FixedBox.OnInit();
			m_MoveBox.OnInit();
		}
	}

	void Scene::OnUpdate() {
		m_FixedBox.OnUpdate();
		m_MoveBox.OnUpdate();
	}

	void Scene::OnDrawPath(UINT index) {
		if (index == 1) {
			auto baseDevice = App::GetBaseDevice();
			auto commandList = baseDevice->GetCommandList();
			//Sampler（s1）
			CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
				baseDevice->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"s1"), SamplerHandle);
			//Sampler(s0)
			CD3DX12_GPU_DESCRIPTOR_HANDLE ShadowSamplerHandle(
				baseDevice->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
				1,
				baseDevice->GetSamplerDescriptorHandleIncrementSize()
			);
			commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"s0"), ShadowSamplerHandle);
			//Srv (t0)
			CD3DX12_GPU_DESCRIPTOR_HANDLE ShadowSrvHandle(
				baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
				baseDevice->GetShadowSRVIndex(),
				baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
			commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"t0"), ShadowSrvHandle);
			m_FixedBox.OnDraw();
			m_MoveBox.OnDraw();
		}
		else {
			//シャドウマップ
			auto baseDevice = App::GetBaseDevice();
			auto commandList = baseDevice->GetCommandList();
			m_MoveBox.OnDrawShadowmap();
		}
	}


	void Scene::OnDestroy() {
	}




}
//end badedx12
