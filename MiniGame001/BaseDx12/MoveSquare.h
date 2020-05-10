#pragma once
#include "stdafx.h"

namespace basedx12 {

	class MoveSquare : public BaseSquare {
		const float m_widthMargin = 64.0f + 16.0f;
		//PositionTextureの四角形メッシュ
		shared_ptr<BaseMesh> m_ptSquareMesh;
		//テクスチャ
		shared_ptr<BaseTexture> m_texture;
		//テクスチャ（シェーダリソース）のインデックス
		UINT m_srvIndex;
		//コンスタントバッファ
		shared_ptr<ConstantBuffer> m_constantBuffer;
		//コンスタントバッファ構造体の実体
		SpriteConstantBuffer m_spriteConstData;
		//コンスタントバッファのインデックス
		UINT m_constBuffIndex;
		virtual void UpdateConstdata()override;
	public:
		MoveSquare(const MapData& data) :
			BaseSquare(data)
		{}
		virtual ~MoveSquare() {}
		virtual void OnCollisionEnter(BaseSquare* other) override;
		virtual void OnInit() override;
		virtual void OnUpdate()override;
		virtual void OnDraw() override;
		float GetWidthMargin() const {
			return m_widthMargin;
		}
	};


}
//end basedx12
