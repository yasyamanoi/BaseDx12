#pragma once
#include "stdafx.h"

namespace basedx12 {


	class BaseSquare : public std::enable_shared_from_this<BaseSquare> {
	protected:
		MapData m_initData;
		DrawData m_drawData;
		bool m_isFixed;
		BaseSquare():m_isFixed(false){}
		BaseSquare(const MapData& data) {
			m_initData = data;
		}
		virtual ~BaseSquare() {}

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
		shared_ptr<BaseMesh> CreateSquareMesh(float u, float v);
		shared_ptr<BaseTexture> CreateTextureFromFile(const wstring& file);
		shared_ptr<ConstantBuffer> CreateConstantBuffer(const SpriteConstantBuffer& dat);
		void InitDrawResources(const wstring& textureFile);
		void SetInitDrawData();
		bool UpdateDrawMatrix();
		virtual void UpdateConstdata();
	public:
		void SetInitData(const MapData& data) {
			m_initData = data;
		}
		bool IsFixed() const {
			return m_isFixed;
		}
		OBB GetOBB() const;
		OBB GetBeforeOBB() const;
		void SetToBefore();
		void SetDirtyflag(bool b);
		virtual void OnInit() =0;
		virtual void OnUpdate();
		virtual void OnDraw();
		virtual void OnCollisionEnter(BaseSquare* other) {}
		Mat4x4 GetWorldMatrix()const;
		Mat4x4 GetBeforeWorldMatrix()const;
		Float3 GetWorldPosition() const;
		void SetWorldPosition(const Float3& pos);
		Float3 GetWorldVelocity() const;
		void SetWorldVelocity(const Float3& velo);
	};



}
//end basedx12
