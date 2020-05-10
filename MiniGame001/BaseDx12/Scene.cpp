#include "stdafx.h"
#include "Project.h"
#include "SceneMap.h"


namespace basedx12 {

	void Scene::OnInit() {
		//�t���[������3
		ResetActiveBaseDevice<GameDevice>(3);
	}
	void Scene::OnInitAssets() {
		auto baseDevice = App::GetBaseDevice();
		//�T���v���[(���L)
		{
			//�e�X�N�`���`��p�T���v���[(LinearClamp)
			m_linearClampSamplerIndex = baseDevice->GetSamplerNextIndex();
			CD3DX12_CPU_DESCRIPTOR_HANDLE linearClampSamplerDescriptorHandle(
				baseDevice->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
				m_linearClampSamplerIndex,
				baseDevice->GetSamplerDescriptorHandleIncrementSize()
			);
			Sampler::CreateSampler(SamplerState::LinearClamp, linearClampSamplerDescriptorHandle);
			//�e�X�N�`���`��p�T���v���[(LinearWrap)
			m_linearWrapSamplerIndex = baseDevice->GetSamplerNextIndex();
			CD3DX12_CPU_DESCRIPTOR_HANDLE linearWrapSamplerDescriptorHandle(
				baseDevice->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
				m_linearWrapSamplerIndex,
				baseDevice->GetSamplerDescriptorHandleIncrementSize()
			);
			Sampler::CreateSampler(SamplerState::LinearWrap, linearWrapSamplerDescriptorHandle);
		}
		//�p�C�v���C�X�e�[�g(���L)
		{
			D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeLineDesc;
			m_ptConstPipelineState
				= PipelineState::CreateDefault2D<VertexPositionTexture, VSPTSprite, PSPTSprite>(baseDevice->GetRootSignature(), pipeLineDesc);
		}
		//�}�b�v�̓ǂݍ���
		{
			g_Stages.push_back(g_StageData1);
		}
		// ���ꂼ��̃I�u�W�F�N�g�̏�����
		{
			auto& stage = g_Stages[0];
			for (auto& v : stage) {
				switch (v.m_charaType) {
					case CharaType::CellSquare:
						m_cellSquare.SetInitData(v);
					break;
					case CharaType::WallSquare:
						m_wallSquareVec.push_back(WallSquare(v));
					break;
					case CharaType::MoveSquare:
						m_moveSquareVec.push_back(MoveSquare(v));
					break;
					case CharaType::ItemSquare:
						m_itemSquareVec.push_back(ItemSquare(v));
						break;

					case CharaType::Player:
						m_player.SetInitData(v);
					break;
				}
			}
			//�e�I�u�W�F�N�g�̃|�C���^��z��ɒǉ�
			m_baseSquareVec.push_back(&m_cellSquare);
			for (auto& v : m_wallSquareVec) {
				m_baseSquareVec.push_back(&v);
			}
			for (auto& v : m_moveSquareVec) {
				m_baseSquareVec.push_back(&v);
			}
			for (auto& v : m_itemSquareVec) {
				m_baseSquareVec.push_back(&v);
			}

			m_baseSquareVec.push_back(&m_player);
			//�e�I�u�W�F�N�g�̏������i���z�֐��Ăяo���j
			for (auto v : m_baseSquareVec) {
				v->OnInit();
			}
		}
	}

	void Scene::SetGPULinearClampSampler() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//Sampler
		CD3DX12_GPU_DESCRIPTOR_HANDLE samplerHandle(
			baseDevice->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_linearClampSamplerIndex,
			baseDevice->GetSamplerDescriptorHandleIncrementSize()
		);
		//RootSignature���s0�ɐݒ�
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"s0"), samplerHandle);
	}

	void Scene::SetGPULinearWrapSampler() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		//Sampler
		CD3DX12_GPU_DESCRIPTOR_HANDLE samplerHandle(
			baseDevice->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_linearWrapSamplerIndex,
			baseDevice->GetSamplerDescriptorHandleIncrementSize()
		);
		//RootSignature���s0�ɐݒ�
		commandList->SetGraphicsRootDescriptorTable(baseDevice->GetGpuSlotID(L"s0"), samplerHandle);
	}

	void Scene::SetGPUPipelineState() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		commandList->SetPipelineState(m_ptConstPipelineState.Get());
	}

	void Scene::RegisterCollsionObject(BaseSquare* baseSquare) {
		m_collisionManager.RegisterObject(baseSquare);
	}


	void Scene::OnUpdate() {
		//�ʃf�[�^��befor�ɐݒ�
		for (auto v : m_baseSquareVec) {
			v->SetToBefore();
		}
		//�Փ˔���̑O����
		m_collisionManager.CollisionPre();
		//�e�I�u�W�F�N�g�̍X�V����
		for (auto v : m_baseSquareVec) {
			v->OnUpdate();
		}
		//�Փ˔���̃e�X�g
		m_collisionManager.CollisionTest();
	}
	void Scene::OnDraw() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�e�I�u�W�F�N�g�̕`�揈��
		for (auto v : m_baseSquareVec) {
			v->OnDraw();
		}
	}
	void Scene::OnDestroy() {
	}

}
//end badedx12
