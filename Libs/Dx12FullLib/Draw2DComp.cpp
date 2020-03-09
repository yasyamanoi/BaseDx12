#include "stdafx.h"

namespace basedx12 {

	IMPLEMENT_DX12SHADER(VSPCSprite, App::GetShadersPath() + L"VSPCSprite.cso")
	IMPLEMENT_DX12SHADER(PSPCSprite, App::GetShadersPath() + L"PSPCSprite.cso")


	IMPLEMENT_DX12SHADER(VSPTSprite, App::GetShadersPath() + L"VSPTSprite.cso")
	IMPLEMENT_DX12SHADER(PSPTSprite, App::GetShadersPath() + L"PSPTSprite.cso")

	//--------------------------------------------------------------------------------------
	///	PC頂点Sprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& gameObjectPtr) :
		Draw2D(gameObjectPtr)
	{}
	PCSpriteDraw::~PCSpriteDraw() {}

	void PCSpriteDraw::OnInit() {
		auto Device = App::GetDx12Device();
		auto commandList = Device->GetCommandList();
		auto aspectRatio = Device->GetAspectRatio();
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
		//パイプライステート
		if (Device->Is3DDevice()) {
			m_pipelineState
				= PipelineState::CreateDefault3D<VertexPositionColor, VSPCSprite, PSPCSprite>(Device->GetRootSignature(), PipeLineDesc);
		}
		else {
			m_pipelineState
				= PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(Device->GetRootSignature(), PipeLineDesc);
		}
		//コンスタントバッファ
		//コンスタントバッファハンドルを作成
		m_constBuffIndex = Device->GetCbvSrvUavNextIndex();
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			Device->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			Device->GetCbvSrvDescriptorHandleIncrementSize()
		);
		m_constantBuffer = ConstantBuffer::CreateDirect(Handle, m_constantBufferData);
	}


	void PCSpriteDraw::OnUpdate() {
	}

	void PCSpriteDraw::OnDraw() {
		auto world = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
		float w = static_cast<float>(App::GetGameWidth());
		float h = static_cast<float>(App::GetGameHeight());
		Mat4x4 proj;
		proj.orthographicLH(w, h, -1.0, 1.0f);
		//行列の合成
		world *= proj;
		m_constantBufferData.World = world;
		m_constantBuffer->Copy(m_constantBufferData);

		auto Device = App::GetDx12Device();
		auto commandList = Device->GetCommandList();
		//Srv
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//srv
		commandList->SetGraphicsRootDescriptorTable(0, SrvHandle);
		//Sampler
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			Device->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//sampler
		commandList->SetGraphicsRootDescriptorTable(1, SamplerHandle);
		//Cbv
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			Device->GetCbvSrvDescriptorHandleIncrementSize()
		);
		//csv
		commandList->SetGraphicsRootDescriptorTable(2, CbvHandle);

		commandList->SetPipelineState(m_pipelineState.Get());
		commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
		commandList->DrawInstanced(3, 1, 0, 0);

	}

	//--------------------------------------------------------------------------------------
	///	PT頂点Sprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& gameObjectPtr) :
		Draw2D(gameObjectPtr)
	{}
	PTSpriteDraw::~PTSpriteDraw() {}

	void PTSpriteDraw::OnInit() {
		auto Device = App::GetDx12Device();
		auto commandList = Device->GetCommandList();
		auto aspectRatio = Device->GetAspectRatio();
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
		//パイプライステート
		if (Device->Is3DDevice()) {
			m_pipelineState
				= PipelineState::CreateDefault3D<VertexPositionTexture, VSPTSprite, PSPTSprite>(Device->GetRootSignature(), PipeLineDesc);
		}
		else {
			m_pipelineState
				= PipelineState::CreateDefault2D<VertexPositionTexture, VSPTSprite, PSPTSprite>(Device->GetRootSignature(), PipeLineDesc);
		}
		//コンスタントバッファ
		//コンスタントバッファハンドルを作成
		m_constBuffIndex = Device->GetCbvSrvUavNextIndex();
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			Device->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			Device->GetCbvSrvDescriptorHandleIncrementSize()
		);
		m_constantBuffer = ConstantBuffer::CreateDirect(Handle, m_constantBufferData);

		auto SamplerDescriptorHandle = Device->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
		Sampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);

	}

	void PTSpriteDraw::SetTextureFile(const wstring& textureName) {
		m_textureFileName = textureName;
		auto Device = App::GetDx12Device();
		//テクスチャの作成
		//シェーダリソースハンドルを作成
		m_srvIndex = Device->GetCbvSrvUavNextIndex();
		CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			Device->GetCbvSrvDescriptorHandleIncrementSize()
		);
		//画像ファイルをもとにテクスチャを作成
		m_texture = Dx12Texture::CreateDx12Texture(m_textureFileName, srvHandle);
	}

	void PTSpriteDraw::OnDraw() {
		auto world = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
		float w = static_cast<float>(App::GetGameWidth());
		float h = static_cast<float>(App::GetGameHeight());
		Mat4x4 proj;
		proj.orthographicLH(w, h, -1.0, 1.0f);
		//行列の合成
		world *= proj;
		m_constantBufferData.World = world;
		m_constantBuffer->Copy(m_constantBufferData);

		auto Device = App::GetDx12Device();
		auto commandList = Device->GetCommandList();
		//Srv
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			Device->GetCbvSrvDescriptorHandleIncrementSize()
		);
		//srv
		commandList->SetGraphicsRootDescriptorTable(0, SrvHandle);
		//Sampler
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			Device->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//sampler
		commandList->SetGraphicsRootDescriptorTable(1, SamplerHandle);
		//Cbv
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			Device->GetCbvSrvDescriptorHandleIncrementSize()
		);
		//csv
		commandList->SetGraphicsRootDescriptorTable(2, CbvHandle);
		commandList->SetPipelineState(m_pipelineState.Get());

		m_texture->UpdateSRAndCreateSRV(commandList);
		commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_mesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_mesh->GetNumIndices(), 1, 0, 0, 0);

	}

}
//end basedx12
