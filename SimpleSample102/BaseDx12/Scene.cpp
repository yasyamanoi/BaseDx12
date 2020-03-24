#include "stdafx.h"
#include "Scene.h"

namespace basedx12 {

	IMPLEMENT_DX12SHADER(VSPCSprite, App::GetShadersPath() + L"vshader.cso")
	IMPLEMENT_DX12SHADER(PSPCSprite, App::GetShadersPath() + L"pshader.cso")

	IMPLEMENT_DX12SHADER(VSPCConstSprite, App::GetShadersPath() + L"VsPCConst.cso")
	IMPLEMENT_DX12SHADER(PSPCConstSprite, App::GetShadersPath() + L"PsPCConst.cso")

	IMPLEMENT_DX12SHADER(VSPTSprite, App::GetShadersPath() + L"VSPTSprite.cso")
	IMPLEMENT_DX12SHADER(PSPTSprite, App::GetShadersPath() + L"PSPTSprite.cso")

	void FixedTriangle::OnInit() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		auto aspectRatio = baseDevice->GetAspectRatio();

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeLineDesc;
		//�R���X�^���g�o�b�t�@����
		m_pcPipelineState
		= PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(baseDevice->GetRootSignature(), pipeLineDesc);
		vector<VertexPositionColor> vertex =
		{
			{ Float3(0.0f, 0.25f * aspectRatio, 0.0f), Float4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Float3(0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Float3(-0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 0.0f, 1.0f, 1.0f) }
		};
		//�O�p�`���b�V���쐬
		m_pcTriangleMesh = BaseMesh::CreateBaseMesh<VertexPositionColor>(vertex);
	}

	void FixedTriangle::OnDraw() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//GraphicsRootDescriptorTable�̐ݒ�͂��Ȃ��Ă��悢�iSRV��Sampler��CSV���V�F�[�_�ɓn���Ȃ��̂Łj
		commandList->SetPipelineState(m_pcPipelineState.Get());
		commandList->IASetVertexBuffers(0, 1, &m_pcTriangleMesh->GetVertexBufferView());
		commandList->DrawInstanced(3, 1, 0, 0);
	}


	void MoveTriangle::OnInit() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		auto aspectRatio = baseDevice->GetAspectRatio();
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeLineDesc;
		m_pcConstPipelineState
		= PipelineState::CreateDefault2D<VertexPositionColor, VSPCConstSprite, PSPCConstSprite>(baseDevice->GetRootSignature(), pipeLineDesc);
		vector<VertexPositionColor> vertex =
		{
			{ Float3(0.0f, 0.25f * aspectRatio, 0.0f), Float4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Float3(0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ Float3(-0.25f, -0.25f * aspectRatio, 0.0f), Float4(1.0f, 0.0f, 0.0f, 1.0f) }
		};
		//�O�p�`���b�V���쐬
		m_pcTriangleMesh = BaseMesh::CreateBaseMesh<VertexPositionColor>(vertex);
		//�R���X�^���g�o�b�t�@�n���h�����쐬
		m_constBuffIndex = baseDevice->GetCbvSrvUavNextIndex();
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		m_ConstantBuffer = ConstantBuffer::CreateDirect(Handle, m_constantBufferData);
	}
	void MoveTriangle::OnUpdate() {
		const float translationSpeed = 0.005f;
		const float offsetBounds = 1.25f;
		m_constantBufferData.offset.x += translationSpeed;
		if (m_constantBufferData.offset.x > offsetBounds)
		{
			m_constantBufferData.offset.x = -offsetBounds;
		}
		m_ConstantBuffer->Copy(m_constantBufferData);
	}
	void MoveTriangle::OnDraw() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//GraphicsRootDescriptorTable�փZ�b�g�B
		//Cbv
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		//csv�BRootSignature���ID��2��
		commandList->SetGraphicsRootDescriptorTable(2, CbvHandle);
		commandList->SetPipelineState(m_pcConstPipelineState.Get());
		commandList->IASetVertexBuffers(0, 1, &m_pcTriangleMesh->GetVertexBufferView());
		commandList->DrawInstanced(3, 1, 0, 0);
	}

	void MoveSquare::OnInit() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		auto aspectRatio = baseDevice->GetAspectRatio();
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
		m_ptConstPipelineState
		= PipelineState::CreateDefault2D<VertexPositionTexture, VSPTSprite, PSPTSprite>(baseDevice->GetRootSignature(), PipeLineDesc);
		//���b�V��
		{
			float HelfSize = 0.2f;
			//���_�z��
			vector<VertexPositionTexture> vertices = {
				{ VertexPositionTexture(Float3(-HelfSize, HelfSize, 0),	Float2(0.0f, 0.0f)) },
				{ VertexPositionTexture(Float3(HelfSize, HelfSize, 0),	Float2(1.0f, 0.0f)) },
				{ VertexPositionTexture(Float3(-HelfSize, -HelfSize, 0),	Float2(0.0f, 1.0f)) },
				{ VertexPositionTexture(Float3(HelfSize, -HelfSize, 0),	Float2(1.0f, 1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint32_t> indices = { 0, 1, 2, 1, 3, 2 };
			//�l�p�`���b�V���̍쐬
			m_ptSquareMesh = BaseMesh::CreateBaseMesh<VertexPositionTexture>(vertices, indices);
		}
		//�e�N�X�`��
		{
			auto TexFile = App::GetRelativeAssetsPath() + L"sky.jpg";
			//�e�N�X�`���̍쐬
			//�V�F�[�_���\�[�X�n���h�����쐬
			m_srvIndex = baseDevice->GetCbvSrvUavNextIndex();
			CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
			//�摜�t�@�C�������ƂɃe�N�X�`�����쐬
			m_SkyTexture = BaseTexture::CreateBaseTexture(TexFile, srvHandle);
		}
		//�T���v���[
		{
			auto samplerDescriptorHandle = baseDevice->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
			Sampler::CreateSampler(SamplerState::LinearClamp, samplerDescriptorHandle);
		}
		//�R���X�^���g�o�b�t�@�n���h�����쐬
		m_constBuffIndex = baseDevice->GetCbvSrvUavNextIndex();
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		m_ConstantBuffer = ConstantBuffer::CreateDirect(Handle, m_constantBufferData);

	}
	void MoveSquare::OnUpdate() {
		const float translationSpeed = 0.01f;
		const float offsetBounds = 1.25f;
		m_constantBufferData.offset.x += translationSpeed;
		if (m_constantBufferData.offset.x > offsetBounds)
		{
			m_constantBufferData.offset.x = -offsetBounds;
		}
		m_ConstantBuffer->Copy(m_constantBufferData);
	}
	void MoveSquare::OnDraw() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();

		//GraphicsRootDescriptorTable�փZ�b�g�B
		//Srv
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		//srv�BRootSignature���ID��0��
		commandList->SetGraphicsRootDescriptorTable(0, SrvHandle);
		//Sampler
		CD3DX12_GPU_DESCRIPTOR_HANDLE samplerHandle(
			baseDevice->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//sampler�BRootSignature���ID��1��
		commandList->SetGraphicsRootDescriptorTable(1, samplerHandle);
		//Cbv
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		//csv�BRootSignature���ID��2��
		commandList->SetGraphicsRootDescriptorTable(2, CbvHandle);
		commandList->SetPipelineState(m_ptConstPipelineState.Get());
		m_SkyTexture->UpdateSRAndCreateSRV(commandList);
		commandList->IASetVertexBuffers(0, 1, &m_ptSquareMesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_ptSquareMesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_ptSquareMesh->GetNumIndices(), 1, 0, 0, 0);
	}

	void Scene::OnInit() {
		//�t���[������3
		ResetActiveBaseDevice<GameDivece>(3);
	}
	void Scene::OnInitAssets() {
		// ���ꂼ��̃I�u�W�F�N�g�̏�����
		{
			m_FixedTriangle.OnInit();
			m_MoveTriangle.OnInit();
			m_MoveSquare.OnInit();
		}
	}

	void Scene::OnUpdate() {
		m_MoveTriangle.OnUpdate();
		m_MoveSquare.OnUpdate();
	}
	void Scene::OnDraw() {
		//m_ConstantBuffer->Copy(m_constantBufferData);
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_FixedTriangle.OnDraw();
		m_MoveTriangle.OnDraw();
		m_MoveSquare.OnDraw();
	}
	void Scene::OnDestroy() {
	}

}
//end badedx12
