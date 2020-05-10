#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void MoveSquare::OnInit() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//���b�V��
		{
			m_ptSquareMesh = CreateSquareMesh(m_initData.m_scale.x, m_initData.m_scale.y);
		}
		//�e�N�X�`��
		{
			auto texFile = App::GetRelativeAssetsPath() + L"Blue.png";
			m_texture = CreateTextureFromFile(texFile, m_srvIndex);
		}
		//�R���X�^���g�o�b�t�@
		{
			m_constantBuffer = CreateConstantBuffer(m_spriteConstData, m_constBuffIndex);
		}
		//DrawData�֘A
		{
			SetInitDrawData();
			m_drawData.m_velocity = Float3(-120.0f, 0, 0);
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

	void MoveSquare::UpdateConstdata() {
		m_spriteConstData = {};
		m_spriteConstData.diffuse = Float4(1.0f);
		m_spriteConstData.emissive = Float4(0.0f);
		m_spriteConstData.worldProj = m_drawData.m_world;
		m_spriteConstData.worldProj *= m_drawData.m_proj;
		m_constantBuffer->Copy(m_spriteConstData);
	}

	void MoveSquare::OnUpdate() {
		float elapsedTime = App::GetElapsedTime();
		m_drawData.m_pos.x += m_drawData.m_velocity.x * elapsedTime;
		float halfW = static_cast<float>(App::GetGameWidth()) / 2.0f;
		halfW += m_widthMargin;
		if (abs(m_drawData.m_pos.x) >= halfW) {
			m_drawData.m_pos.x *= -1.0f;
		}
		m_drawData.m_dirtyflag = true;
		if (UpdateDrawMatrix()) {
			UpdateConstdata();
		}
	}

	void MoveSquare::OnCollisionEnter(BaseSquare* other) {
		auto ptr = dynamic_cast<WallSquare*>(other);
		if (ptr) {
			m_drawData.m_velocity.x *= -1.0f;
		}
	}


	void MoveSquare::OnDraw() {
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