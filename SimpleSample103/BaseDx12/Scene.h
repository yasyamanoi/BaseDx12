#pragma once
#include "GameDevice.h"

namespace basedx12 {




	DECLARE_DX12SHADER(VSPNT)
	DECLARE_DX12SHADER(PSPNT)

	struct SimpleConstants {
		Mat4x4 worldViewProj;
	};

	class FixedBox {
		shared_ptr<Camera> m_camera;
		shared_ptr<LightSet> m_lightSet;
		shared_ptr<BaseMesh> m_mesh;
		ComPtr<ID3D12PipelineState> m_pipelineState;
		//コンスタントバッファ
		shared_ptr<ConstantBuffer> m_constantBuffer;
		//コンスタントバッファのインデックス
		UINT m_constBuffIndex;
		//コンスタントバッファの実体
		SimpleConstants m_simpleConstantsData;
		//テクスチャ
		shared_ptr<BaseTexture> m_SkyTexture;
		//テクスチャ（シェーダリソース）のインデックス
		UINT m_srvIndex;
		//スケール
		Float3 m_scale;
		//回転
		Quat m_qt;
		//位置
		Float3 m_pos;

		void SetSimpleConstants();

	public:
		FixedBox() :
			m_scale(10.0f,1.0f,10.0f),
			m_qt(),
			m_pos(0.0f, -0.5f, 0.0f)
		{}
		~FixedBox() {}
		void OnInit();
		void OnUpdate() {}
		void OnDraw();
	};



	class MoveBox{
		shared_ptr<Camera> m_camera;
		shared_ptr<LightSet> m_lightSet;
		shared_ptr<BaseMesh> m_mesh;
		ComPtr<ID3D12PipelineState> m_pipelineState;
		//コンスタントバッファ
		shared_ptr<ConstantBuffer> m_constantBuffer;
		//コンスタントバッファのインデックス
		UINT m_constBuffIndex;
		//コンスタントバッファの実体
		SimpleConstants m_simpleConstantsData;
		//テクスチャ
		shared_ptr<BaseTexture> m_wallTexture;
		//テクスチャ（シェーダリソース）のインデックス
		UINT m_srvIndex;
		//スケール
		Float3 m_scale;
		//回転
		Quat m_qt;
		//位置
		Float3 m_pos;
		float m_posSpan;
		void SetSimpleConstants();
	public:
		MoveBox():
			m_posSpan(0.02f),
			m_scale(1.0f),
			m_qt(),
			m_pos(0.0f,0.5f,0.0f)
		{}
		~MoveBox() {}
		void OnInit();
		void OnUpdate();
		void OnDraw();
	};


	class Scene :public SceneBase {
		FixedBox m_FixedBox;
		MoveBox m_MoveBox;
	public:
		Scene() :SceneBase() {}
		virtual ~Scene() {}
		virtual void OnInit()override;
		virtual void OnInitAssets()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
		virtual void OnDestroy()override;
	};

}
//end basedx12


