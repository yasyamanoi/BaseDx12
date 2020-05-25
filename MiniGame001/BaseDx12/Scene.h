#pragma once
#include "stdafx.h"

namespace basedx12 {

	struct ObjRect;
	struct ObjData;
	class Scene :public SceneBase {
		const UINT m_halfWidth = 48 / 2;
		const UINT m_cellSize = 32;
		const float m_sceneGravity = -980.0f;
		float m_lowSpan;
		//サンプラーのインデックス(共有する)
		UINT m_linearClampSamplerIndex;
		UINT m_linearWrapSamplerIndex;
		//PositionTexture用パイプラインステート（コンスタントバッファあり共有する）
		ComPtr<ID3D12PipelineState> m_ptConstPipelineState;
		//配置されるオブジェクトの親クラスポインタの配列
		vector<shared_ptr<BaseSquare>> m_baseSquareVec;
		//衝突判定マネージャ
		CollisionManager m_collisionManager;
		//ステージデータ読み込み
		//1つのオブジェクトデータのクリア
		void ClearTgtCelldata(const ObjRect& rect);
		//1つのオブジェクトデータの読み込み
		ObjRect ReadSimgleObject();
		//左上座標からセンター座標に変換
		void ConvertLeftTopDataToCenter(vector<ObjRect>& rectVec, vector<ObjData>& objVec);
	public:
		Scene() :SceneBase(), m_lowSpan(0.0f) {
		}
		virtual ~Scene() {}
		UINT GetHalfWidth() const {
			return m_halfWidth;
		}
		UINT GetCellSize() const {
			return m_cellSize;
		}

		float GetLowSpan() const {
			return m_lowSpan;
		}
		void RegisterCollsionObject(BaseSquare* pBaseSquare);

		CollisionManager* GetCollisionManager() {
			return &m_collisionManager;
		}
		float GetSceneGravity() const {
			return m_sceneGravity;
		}
		void SetGPULinearClampSampler();
		void SetGPULinearWrapSampler();
		void SetGPUPipelineState();
		virtual void OnInit()override;
		virtual void OnInitAssets()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
		virtual void OnDestroy()override;
	};

}
//end basedx12


