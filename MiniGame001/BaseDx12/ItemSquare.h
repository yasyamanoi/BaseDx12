#pragma once
#include "stdafx.h"

namespace basedx12 {

	class ItemSquare : public BaseSquare {
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
		ItemSquare(const MapData& data) :
			BaseSquare(data)
		{}
		virtual ~ItemSquare() {}
		virtual void OnCollisionEnter(BaseSquare* other) override;
		virtual void OnInit() override;
		virtual void OnUpdate()override;
		virtual void OnDraw() override;
	};

}
//end basedx12
