#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void ItemSquare::OnInit() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//���b�V��
		{
			m_ptSquareMesh = CreateSquareMesh(m_initData.m_scale.x, m_initData.m_scale.y);
		}
		//�e�N�X�`��
		{
			auto texFile = App::GetRelativeAssetsPath() + L"Brown.png";
			m_texture = CreateTextureFromFile(texFile, m_srvIndex);
		}
		//�R���X�^���g�o�b�t�@
		{
			m_constantBuffer = CreateConstantBuffer(m_spriteConstData, m_constBuffIndex);
		}
		//DrawData�֘A
		{
			SetInitDrawData();
			if (UpdateDrawMatrix()) {
				UpdateConstdata();
			}
		}
		//Collision�֘A
		{
			m_isFixed = true;
			auto scene = App::GetTypedSceneBase<Scene>();
			scene->RegisterCollsionObject(this);
		}
	}

	void ItemSquare::UpdateConstdata() {
		m_spriteConstData = {};
		m_spriteConstData.diffuse = m_drawData.m_diffuse;
		m_spriteConstData.emissive = m_drawData.m_emissive;
		m_spriteConstData.worldProj = m_drawData.m_world;
		m_spriteConstData.worldProj *= m_drawData.m_proj;
		m_constantBuffer->Copy(m_spriteConstData);
	}

	void ItemSquare::OnCollisionEnter(BaseSquare* other) {
		auto scene = App::GetTypedSceneBase<Scene>();
		scene->GetCollisionManager()->RemoveObject(this);
		m_drawData.m_emissive = Float4(1.0f);
		m_drawData.m_dirtyflag = true;
	}


	void ItemSquare::OnUpdate() {
		if (UpdateDrawMatrix()) {
			UpdateConstdata();
		}
	}

	void ItemSquare::OnDraw() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		auto scene = App::GetTypedSceneBase<Scene>();
		//�p�C�v���C�X�e�[�g
		scene->SetGPUPipelineState();
		//Sampler
		scene->SetGPULinearWrapSampler();
		//�e�N�X�`���̍X�V�ƃV�F�[�_���\�[�X�r���[�̍쐬
		m_texture->UpdateSRAndCreateSRV(commandList);
		//Srv�̃n���h���̐ݒ�
		CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		//RootSignature���t0�ɐݒ�
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"t0"), srvHandle);
		//Cbv�̃n���h����ݒ�
		CD3DX12_GPU_DESCRIPTOR_HANDLE cbvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		//RootSignature���b0�ɐݒ�
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"b0"), cbvHandle);
		//�`�揈��
		commandList->IASetVertexBuffers(0, 1, &m_ptSquareMesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_ptSquareMesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_ptSquareMesh->GetNumIndices(), 1, 0, 0, 0);
	}


}
//end basedx12
