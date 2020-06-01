#include "stdafx.h"
#include "Project.h"
#include "SceneMap.h"


namespace basedx12 {

	void Scene::ClearTgtCelldata(const ObjRect& rect) {
		for (int row = rect.top; row <= rect.bottom; row++) {
			for (int col = rect.left; col <= rect.right; col++) {
				g_Celldata[row][col] = 0;
			}
		}
		int i = 0;
	}

	ObjRect Scene::ReadSimgleObject() {
		ObjRect ret = {};
		int selectType = 0;
		for (int row = 0; row < g_Celldata.size(); row++) {
			for (int col = 0; col < g_Celldata[row].size(); col++) {
				if (ret.width != 0) {
					continue;
				}
				if (selectType == 0 && g_Celldata[row][col] != 0) {
					ret.left = col;
					ret.top = row;
					ret.type = g_Celldata[row][col];
					selectType = g_Celldata[row][col];
				}
				if (selectType != 0 && g_Celldata[row][col] != selectType) {
					ret.right = col - 1;
					ret.width = col - ret.left;
				}
			}
			if (selectType != 0 && g_Celldata[row][ret.right] != selectType) {
				ret.bottom = row - 1;
				ret.height = row - ret.top;
				break;
			}
		}
		ClearTgtCelldata(ret);
		return ret;
	}

	void Scene::ConvertLeftTopDataToCenter(vector<ObjRect>& rectVec, vector<ObjData>& objVec) {
		int maxBottom = 0;
		//最大高さ
		for (auto& v : rectVec) {
			if (v.bottom > maxBottom) {
				maxBottom = v.bottom;
			}
		}
		for (auto& v : rectVec) {
			v.left -= m_halfWidth;
			v.right -= m_halfWidth;
			v.top = maxBottom - v.top;
			v.bottom = maxBottom - v.bottom;
		}
		objVec.clear();
		for (auto& v : rectVec) {
			ObjData data;
			data.type = v.type;
			data.centerX = (float)v.left + ((float)(v.right - v.left) / 2.0f);
			data.centerY = (float)v.bottom + ((float)(v.top - v.bottom) / 2.0f) - 11.0f;
			data.width = (float)(v.width);
			data.height = (float)(v.height);
			objVec.push_back(data);
		}
	}



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
		// それぞれのオブジェクトの初期化
		{
			ObjRect rect = {};
			vector<ObjRect> rectVec;
			vector<ObjData> objVec;
			do {
				rect = ReadSimgleObject();
				if (!rect.isNull()) {
					rectVec.push_back(rect);
				}
			} while (!rect.isNull());

			ConvertLeftTopDataToCenter(rectVec, objVec);
			//セルガイド
			MapData cellData(CharaType::CellSquare, Float3(41.0, 23.0, 1.0), Float3(0.0), Quat(), Float3(0.0));
			auto ptrCell = shared_ptr<CellSquare>(new CellSquare());
			ptrCell->SetInitData(cellData);
			m_baseSquareVec.push_back(ptrCell);
			MapData data;
			for (auto& v : objVec) {
				data.m_scale.x = v.width;
				data.m_scale.y = v.height;
				data.m_scale.z = 1.0f;
				data.m_pivot = Float3(0.0f);
				data.m_pos.x = v.centerX;
				data.m_pos.y = v.centerY;
				data.m_pos.z = 0.0f;
				switch (v.type) {
					case 1:
					{
						data.m_charaType = CharaType::TransSquare;
						auto ptrTrans = shared_ptr<TransSquare>(new TransSquare(data));
						m_baseSquareVec.push_back(ptrTrans);
					}
					break;
					case 2:
					{
						data.m_charaType = CharaType::WallSquare;
						auto ptrWall = shared_ptr<WallSquare>(new WallSquare(data));
						m_baseSquareVec.push_back(ptrWall);
					}
					break;
					case 3:
					{
						data.m_charaType = CharaType::MoveSquare;
						auto ptrMove = shared_ptr<MoveSquare>(new MoveSquare(data));
						m_baseSquareVec.push_back(ptrMove);
					}
					break;
					case 4:
					{
						data.m_charaType = CharaType::ItemSquare;
						auto ptrItem = shared_ptr<ItemSquare>(new ItemSquare(data));
						m_baseSquareVec.push_back(ptrItem);
					}
					break;

					case 5:
					{
						data.m_charaType = CharaType::Player;
						auto ptrPlayer = shared_ptr<Player>(new Player());
						ptrPlayer->SetInitData(data);
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
		m_collisionManager.TestPreCollision();
		//各オブジェクトの更新処理
		for (auto v : m_baseSquareVec) {
			v->OnUpdate();
		}
		//衝突判定のテスト
		m_collisionManager.TestMainCollision();
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
