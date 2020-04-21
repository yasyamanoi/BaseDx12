#include "stdafx.h"
#include "Scene.h"

namespace basedx12 {


	IMPLEMENT_DX12SHADER(VSPNT, App::GetShadersPath() + L"VSPNT.cso")
	IMPLEMENT_DX12SHADER(PSPNT, App::GetShadersPath() + L"PSPNT.cso")

	IMPLEMENT_DX12SHADER(VSPNTShadow, App::GetShadersPath() + L"VSPNTShadow.cso")
	IMPLEMENT_DX12SHADER(PSPNTShadow, App::GetShadersPath() + L"PSPNTShadow.cso")


	IMPLEMENT_DX12SHADER(VSShadowmap, App::GetShadersPath() + L"VSShadowmap.cso")

	void FixedBox::OnInit() {
		m_camera = Camera::CreateCamera<Camera>(Float3(0, 10.0f, -10.0f), Float3(0, 0, 0));
		m_lightSet = LightSet::CreateDefaultLights();
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		auto aspectRatio = baseDevice->GetAspectRatio();
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
		//パイプライステート
		m_pipelineState
			= PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTShadow, PSPNTShadow>(baseDevice->GetRootSignature(), PipeLineDesc);
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
		Mat4x4 worldMatrix;
		worldMatrix.affineTransformation(
			m_scale,
			Float3(0.0f),
			m_qt,
			m_pos
		);
		auto view = m_camera->GetViewMatrix();
		auto proj = m_camera->GetProjectionMatrix();
		//初期化
		m_shadowSceneConstantsData = {};

		m_shadowSceneConstantsData.World = bsm::transpose(worldMatrix);
		m_shadowSceneConstantsData.View = bsm::transpose(view);
		m_shadowSceneConstantsData.Projection = bsm::transpose(proj);

		//ライトの位置
		Float3 lightPos(0.0f, 10.0f, 0.0f);
		Float3 lightAt = m_camera->GetAt();
		Float3 lightUp(0, 0, 1);
		float w = (float)App::GetGameWidth();
		float h = (float)App::GetGameHeight();
		float aspectRatio = w / h;

		m_shadowSceneConstantsData.LightPos = lightPos;
		m_shadowSceneConstantsData.LightPos.w = 1.0f;
		m_shadowSceneConstantsData.EyePos = m_camera->GetEye();
		m_shadowSceneConstantsData.EyePos.w = 1.0f;
		m_shadowSceneConstantsData.LightView.lookatLH(lightPos, lightAt, lightUp);
		m_shadowSceneConstantsData.LightView.transpose();
		m_shadowSceneConstantsData.LightProjection.perspectiveFovLH(XM_PIDIV4, aspectRatio, 1.0f, 100.0f);
		m_shadowSceneConstantsData.LightProjection.transpose();

	}


	void FixedBox::OnDraw() {
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
		//2つ目のシェーダリソース（1）
		CD3DX12_GPU_DESCRIPTOR_HANDLE ShadowSrvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			Device->GetShadowSRVIndex(),
			Device->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		commandList->SetGraphicsRootDescriptorTable(1, ShadowSrvHandle);
		//Sampler
		//１つ目のサンプラー(2)
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			Device->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			0,
			Device->GetSamplerDescriptorHandleIncrementSize()
		);
		commandList->SetGraphicsRootDescriptorTable(2, SamplerHandle);
		//２つ目のサンプラー(3)
		CD3DX12_GPU_DESCRIPTOR_HANDLE ShadowSamplerHandle(
			Device->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			1,
			Device->GetSamplerDescriptorHandleIncrementSize()
		);
		commandList->SetGraphicsRootDescriptorTable(3, ShadowSamplerHandle);
		//コンスタントバッファ（4）
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			Device->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		commandList->SetGraphicsRootDescriptorTable(4, CbvHandle);
		//パイプライステート
		commandList->SetPipelineState(m_pipelineState.Get());

		m_SkyTexture->UpdateSRAndCreateSRV(commandList);
		commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_mesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_mesh->GetNumIndices(), 1, 0, 0, 0);

	}


	void MoveBox::OnInit() {
		m_camera = Camera::CreateCamera<Camera>(Float3(0, 10.0f, -10.0f), Float3(0, 0, 0));
		m_lightSet = LightSet::CreateDefaultLights();
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		auto aspectRatio = baseDevice->GetAspectRatio();
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
		//パイプライステート
		m_shadowmapPipelineState
			= PipelineState::CreateShadowmap3D<VertexPositionNormalTexture, VSShadowmap>(baseDevice->GetRootSignature(), PipeLineDesc);
		m_pipelineState
			= PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNT, PSPNT>(baseDevice->GetRootSignature(), PipeLineDesc);
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
		auto SamplerDescriptorHandle = baseDevice->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
		Sampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
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

	void MoveBox::SetShadowmapConstants() {
		Mat4x4 worldMatrix,viewMatrix,projMatrix;
		worldMatrix.affineTransformation(
			m_scale,
			Float3(0.0f),
			m_qt,
			m_pos
		);
		//ライトの位置
		Float3 lightPos(0.0f, 10.0f, 0.0);
		Float3 lightAt = m_camera->GetAt();
		//		Float3 lightAt(0.0f, -4.0f, 2.0f);
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
		Mat4x4 worldMatrix;
		worldMatrix.affineTransformation(
			m_scale,
			Float3(0.0f),
			m_qt,
			m_pos
		);
		auto view = m_camera->GetViewMatrix();
		auto proj = m_camera->GetProjectionMatrix();
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
		commandList->SetPipelineState(m_shadowmapPipelineState.Get());
		commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_mesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_mesh->GetNumIndices(), 1, 0, 0, 0);

	}


	void MoveBox::OnDraw() {
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
		//1つ目のSampler（2）
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			Device->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
			);
		commandList->SetGraphicsRootDescriptorTable(2, SamplerHandle);
		//Cbv（4）
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			Device->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
		commandList->SetGraphicsRootDescriptorTable(4, CbvHandle);
		commandList->SetPipelineState(m_pipelineState.Get());

		m_SkyTexture->UpdateSRAndCreateSRV(commandList);
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
			auto samplerDescriptorHandle = baseDevice->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
			Sampler::CreateSampler(SamplerState::LinearClamp, samplerDescriptorHandle);
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
	void Scene::OnDraw() {
		OnDrawPath(1);
	}

	void Scene::OnDrawPath(UINT index) {
		if (index == 1) {
			//m_ConstantBuffer->Copy(m_constantBufferData);
			auto baseDevice = App::GetBaseDevice();
			auto commandList = baseDevice->GetCommandList();
			commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_FixedBox.OnDraw();
			m_MoveBox.OnDraw();
		}
		else {
			//シャドウマップ
			auto baseDevice = App::GetBaseDevice();
			auto commandList = baseDevice->GetCommandList();
			commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_MoveBox.OnDrawShadowmap();
		}
	}


	void Scene::OnDestroy() {
	}




}
//end badedx12
