#include "stdafx.h"
#include "Scene.h"

namespace basedx12 {

	IMPLEMENT_DX12SHADER(VSPNT, App::GetShadersPath() + L"VSPNT.cso")
	IMPLEMENT_DX12SHADER(PSPNT, App::GetShadersPath() + L"PSPNT.cso")

	IMPLEMENT_DX12SHADER(VSPNTShadow, App::GetShadersPath() + L"VSPNTShadow.cso")
	IMPLEMENT_DX12SHADER(PSPNTShadow, App::GetShadersPath() + L"PSPNTShadow.cso")


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
		//コンスタントバッファ
		m_constBuffIndex = baseDevice->GetCbvSrvUavNextIndex();
		//コンスタントバッファハンドルを作成
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		m_constantBuffer = ConstantBuffer::CreateDirect(Handle, m_shadowSceneConstantsData);
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
			m_SkyTexture = BaseTexture::CreateBaseTexture(TexFile, srvHandle);
		}
	}

	void FixedBox::SetShadowSceneConstants() {
		auto scene = App::GetTypedSceneBase<Scene>();
		Mat4x4 worldMatrix;
		worldMatrix.affineTransformation(
			m_scale,
			Float3(0.0f),
			m_qt,
			m_pos
		);
		auto view = scene.GetCamera()->GetViewMatrix();
		auto proj = scene.GetCamera()->GetProjectionMatrix();
		//初期化
		m_shadowSceneConstantsData = {};
		m_shadowSceneConstantsData.World = bsm::transpose(worldMatrix);
		m_shadowSceneConstantsData.View = bsm::transpose(view);
		m_shadowSceneConstantsData.Projection = bsm::transpose(proj);
		//ライトの位置
		Float3 lightPos = scene.GetLightPos();
		Float3 lightAt = scene.GetCamera()->GetAt();
		Float3 lightUp(0, 0, 1);
		float w = (float)App::GetGameWidth();
		float h = (float)App::GetGameHeight();
		float aspectRatio = w / h;
		m_shadowSceneConstantsData.LightPos = lightPos;
		m_shadowSceneConstantsData.LightPos.w = 1.0f;
		m_shadowSceneConstantsData.EyePos = scene.GetCamera()->GetEye();
		m_shadowSceneConstantsData.EyePos.w = 1.0f;
		m_shadowSceneConstantsData.LightView.lookatLH(lightPos, lightAt, lightUp);
		m_shadowSceneConstantsData.LightView.transpose();
		m_shadowSceneConstantsData.LightProjection.perspectiveFovLH(XM_PIDIV4, aspectRatio, 1.0f, 100.0f);
		m_shadowSceneConstantsData.LightProjection.transpose();
	}


	void FixedBox::OnDraw() {
		auto scene = App::GetTypedSceneBase<Scene>();
		SetShadowSceneConstants();
		m_constantBuffer->Copy(m_shadowSceneConstantsData);
		auto Device = App::GetBaseDevice();
		auto commandList = Device->GetCommandList();
		//Srv
		//1つ目のシェーダリソース（0）
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			Device->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		commandList->SetGraphicsRootDescriptorTable(0, SrvHandle);
		//コンスタントバッファ（4）
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			Device->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		commandList->SetGraphicsRootDescriptorTable(4, CbvHandle);
		//パイプライステート
		commandList->SetPipelineState(scene.GetSceneShadowPipelineState().Get());
		m_SkyTexture->UpdateSRAndCreateSRV(commandList);
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
		//コンスタントバッファ
		m_constBuffIndex = baseDevice->GetCbvSrvUavNextIndex();
		//コンスタントバッファハンドルを作成
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		m_constantBuffer = ConstantBuffer::CreateDirect(Handle, m_simpleConstantsData);
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
		Float3 lightPos = scene.GetLightPos();
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


	void MoveBox::SetSimpleConstants() {
		auto scene = App::GetTypedSceneBase<Scene>();
		Mat4x4 worldMatrix;
		worldMatrix.affineTransformation(
			m_scale,
			Float3(0.0f),
			m_qt,
			m_pos
		);
		auto view = scene.GetCamera()->GetViewMatrix();
		auto proj = scene.GetCamera()->GetProjectionMatrix();
		//初期化
		m_simpleConstantsData = {};
		m_simpleConstantsData.worldViewProj = worldMatrix;
		m_simpleConstantsData.worldViewProj *= view;
		m_simpleConstantsData.worldViewProj *= proj;
		m_simpleConstantsData.worldViewProj.transpose();

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
		auto Device = App::GetBaseDevice();
		auto commandList = Device->GetCommandList();
		//Cbv（4）
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_shadowmapConstBuffIndex,
			Device->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		//コンスタントバッファ（4）
		commandList->SetGraphicsRootDescriptorTable(4, CbvHandle);
		commandList->SetPipelineState(scene.GetShadowmapPipelineState().Get());
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_mesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_mesh->GetNumIndices(), 1, 0, 0, 0);

	}


	void MoveBox::OnDraw() {
		auto scene = App::GetTypedSceneBase<Scene>();
		SetSimpleConstants();
		m_constantBuffer->Copy(m_simpleConstantsData);
		auto Device = App::GetBaseDevice();
		auto commandList = Device->GetCommandList();
		//1つ目のSrv（0）
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			Device->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
		commandList->SetGraphicsRootDescriptorTable(0, SrvHandle);
		//Cbv（4）
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			Device->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
		commandList->SetGraphicsRootDescriptorTable(4, CbvHandle);
		commandList->SetPipelineState(scene.GetScenePipelineState().Get());
		m_wallTexture->UpdateSRAndCreateSRV(commandList);
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_mesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_mesh->GetNumIndices(), 1, 0, 0, 0);
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
			m_scenePipelineState
				= PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNT, PSPNT>(baseDevice->GetRootSignature(), PipeLineDesc);
			//パイプライステート
			m_sceneShadowPipelineState
				= PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTShadow, PSPNTShadow>(baseDevice->GetRootSignature(), PipeLineDesc);
		}
		//カメラとライトは共有
		{
			m_camera = Camera::CreateCamera<Camera>(Float3(0, 3.0f, -5.0f), Float3(0, 0, 0));
			m_lightPos = Float3(0.0f, 10.0f, 0.0);
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
			//1つ目のSampler（2）
			CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
				baseDevice->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			commandList->SetGraphicsRootDescriptorTable(2, SamplerHandle);
			//２つ目のサンプラー(3)
			CD3DX12_GPU_DESCRIPTOR_HANDLE ShadowSamplerHandle(
				baseDevice->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
				1,
				baseDevice->GetSamplerDescriptorHandleIncrementSize()
			);
			commandList->SetGraphicsRootDescriptorTable(3, ShadowSamplerHandle);
			//2つ目のシェーダリソース（1）
			CD3DX12_GPU_DESCRIPTOR_HANDLE ShadowSrvHandle(
				baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
				baseDevice->GetShadowSRVIndex(),
				baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
			commandList->SetGraphicsRootDescriptorTable(1, ShadowSrvHandle);
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
