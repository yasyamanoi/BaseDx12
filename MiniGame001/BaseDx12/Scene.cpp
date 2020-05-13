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
						{
							auto ptrCell = shared_ptr<CellSquare>(new CellSquare());
							ptrCell->SetInitData(v);
							m_baseSquareVec.push_back(ptrCell);
						}
					break;
					case CharaType::WallSquare:
						{
							auto ptrWall = shared_ptr<WallSquare>(new WallSquare(v));
							m_baseSquareVec.push_back(ptrWall);
						}
					break;
					case CharaType::MoveSquare:
						{
							auto ptrMove = shared_ptr<MoveSquare>(new MoveSquare(v));
							m_baseSquareVec.push_back(ptrMove);
						}
					break;
					case CharaType::ItemSquare:
						{
							auto ptrItem = shared_ptr<ItemSquare>(new ItemSquare(v));
							m_baseSquareVec.push_back(ptrItem);
						}
						break;
					case CharaType::Player:
						{
							auto ptrPlayer = shared_ptr<Player>(new Player());
							ptrPlayer->SetInitData(v);
							m_baseSquareVec.push_back(ptrPlayer);
						}
						break;
				}
			}
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
		m_collisionManager.TestPreCollision();
		//�e�I�u�W�F�N�g�̍X�V����
		for (auto v : m_baseSquareVec) {
			v->OnUpdate();
		}
		//�Փ˔���̃e�X�g
		m_collisionManager.TestMainCollision();
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
