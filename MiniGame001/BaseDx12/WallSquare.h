#pragma once
#include "stdafx.h"

namespace basedx12 {

	class WallSquare : public BaseSquare {
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
		WallSquare(const MapData& data) :
			BaseSquare(data)
		{}
		virtual ~WallSquare() {}
		virtual void OnInit() override;
		virtual void OnUpdate()override;
		virtual void OnDraw() override;
	};

}
//end basedx12
