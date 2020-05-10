#include "stdafx.h"
#include "Project.h"
#include "SceneMap.h"


namespace basedx12 {

	void Scene::OnInit() {
		//フレーム数は3
		ResetActiveBaseDevice<GameDevice>(3);
	}
	void Scene::OnInitAssets() {
		auto baseDevice = App::GetBaseDevice();
		//サンプラー(共有)
		{
			//テスクチャ描画用サンプラー(LinearClamp)
			m_linearClampSamplerIndex = baseDevice->GetSamplerNextIndex();
			CD3DX12_CPU_DESCRIPTOR_HANDLE linearClampSamplerDescriptorHandle(
				baseDevice->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
				m_linearClampSamplerIndex,
				baseDevice->GetSamplerDescriptorHandleIncrementSize()
			);
			Sampler::CreateSampler(SamplerState::LinearClamp, linearClampSamplerDescriptorHandle);
			//テスクチャ描画用サンプラー(LinearWrap)
			m_linearWrapSamplerIndex = baseDevice->GetSamplerNextIndex();
			CD3DX12_CPU_DESCRIPTOR_HANDLE linearWrapSamplerDescriptorHandle(
				baseDevice->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
				m_linearWrapSamplerIndex,
				baseDevice->GetSamplerDescriptorHandleIncrementSize()
			);
			Sampler::CreateSampler(SamplerState::LinearWrap, linearWrapSamplerDescriptorHandle);
		}
		//パイプライステート(共有)
		{
			D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeLineDesc;
			m_ptConstPipelineState
				= PipelineState::CreateDefault2D<VertexPositionTexture, VSPTSprite, PSPTSprite>(baseDevice->GetRootSignature(), pipeLineDesc);
		}
		//マップの読み込み
		{
			g_Stages.push_back(g_StageData1);
		}
		// それぞれのオブジェクトの初期化
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
			//各オブジェクトのポインタを配列に追加
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
			//各オブジェクトの初期化（仮想関数呼び出し）
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
		//RootSignature上のs0に設定
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
		//RootSignature上のs0に設定
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
		//個別データをbeforに設定
		for (auto v : m_baseSquareVec) {
			v->SetToBefore();
		}
		//衝突判定の前処理
		m_collisionManager.CollisionPre();
		//各オブジェクトの更新処理
		for (auto v : m_baseSquareVec) {
			v->OnUpdate();
		}
		//衝突判定のテスト
		m_collisionManager.CollisionTest();
	}
	void Scene::OnDraw() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//各オブジェクトの描画処理
		for (auto v : m_baseSquareVec) {
			v->OnDraw();
		}
	}
	void Scene::OnDestroy() {
	}

}
//end badedx12
