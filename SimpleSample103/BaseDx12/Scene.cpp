#include "stdafx.h"
#include "Scene.h"

namespace basedx12 {


	IMPLEMENT_DX12SHADER(VSPNT, App::GetShadersPath() + L"VSPNT.cso")
	IMPLEMENT_DX12SHADER(PSPNT, App::GetShadersPath() + L"PSPNT.cso")


	void FixedBox::OnInit() {
		m_camera = Camera::CreateCamera<Camera>(Float3(0, 3.0f, -5.0f), Float3(0, 0, 0));
		m_lightSet = LightSet::CreateDefaultLights();
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		auto aspectRatio = baseDevice->GetAspectRatio();
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
		//パイプライステート
		m_pipelineState
			= PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNT, PSPNT>(baseDevice->GetRootSignature(), PipeLineDesc);
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

	void FixedBox::SetSimpleConstants() {
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


	void FixedBox::OnDraw() {
		SetSimpleConstants();
		auto baseDevice = App::GetBaseDevice();
		m_constantBuffer->Copy(m_simpleConstantsData);
		auto Device = App::GetBaseDevice();
		auto commandList = Device->GetCommandList();
		//Srv
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			Device->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"t0"), SrvHandle);
		//Sampler
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			Device->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"s0"), SamplerHandle);
		//Cbv
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			Device->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"b0"), CbvHandle);
		commandList->SetPipelineState(m_pipelineState.Get());

		m_SkyTexture->UpdateSRAndCreateSRV(commandList);
		commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_mesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_mesh->GetNumIndices(), 1, 0, 0, 0);

	}


	void MoveBox::OnInit() {
		m_camera = Camera::CreateCamera<Camera>(Float3(0, 3.0f, -5.0f), Float3(0, 0, 0));
		m_lightSet = LightSet::CreateDefaultLights();
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		auto aspectRatio = baseDevice->GetAspectRatio();
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
		//パイプライステート
		m_pipelineState
			= PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNT, PSPNT>(baseDevice->GetRootSignature(), PipeLineDesc);
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
		m_constantBuffer = ConstantBuffer::CreateDirect(Handle, m_simpleConstantsData);
		auto SamplerDescriptorHandle = baseDevice->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
		Sampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
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

	void MoveBox::OnDraw() {
		SetSimpleConstants();
		m_constantBuffer->Copy(m_simpleConstantsData);
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//Srv
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"t0"), SrvHandle);
		//Sampler
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			baseDevice->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
			);
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"s0"), SamplerHandle);
		//Cbv
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"b0"), CbvHandle);
		commandList->SetPipelineState(m_pipelineState.Get());

		m_wallTexture->UpdateSRAndCreateSRV(commandList);
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
		//m_ConstantBuffer->Copy(m_constantBufferData);
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_FixedBox.OnDraw();
		m_MoveBox.OnDraw();
	}
	void Scene::OnDestroy() {
	}




}
//end badedx12
