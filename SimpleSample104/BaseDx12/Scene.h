#pragma once
#include "GameDevice.h"

namespace basedx12 {

	DECLARE_DX12SHADER(VSPNT)
	DECLARE_DX12SHADER(PSPNT)

	DECLARE_DX12SHADER(VSPNTShadow)
	DECLARE_DX12SHADER(PSPNTShadow)


	DECLARE_DX12SHADER(VSShadowmap)

	struct ShadowmapConstants {
		Mat4x4 world;
		Mat4x4 view;
		Mat4x4 proj;
	};

	struct SimpleConstants {
		Mat4x4 worldViewProj;
	};

	struct ShadowSceneConstants {
		/// ワールド行列
		Mat4x4 World;
		/// ビュー行列
		Mat4x4 View;
		/// 射影行列
		Mat4x4 Projection;
		/// ライト位置
		Float4 LightPos;
		/// Eyeの位置
		Float4 EyePos;
		/// ライトビュー行列
		Mat4x4 LightView;
		/// ライト射影行列
		Mat4x4 LightProjection;
	};




	class FixedBox {
		shared_ptr<BaseMesh> m_mesh;
		//コンスタントバッファ
		shared_ptr<ConstantBuffer> m_constantBuffer;
		//コンスタントバッファのインデックス
		UINT m_constBuffIndex;
		//コンスタントバッファの実体
		ShadowSceneConstants m_shadowSceneConstantsData;
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
		void SetShadowSceneConstants();
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
		//メッシュ
		shared_ptr<BaseMesh> m_mesh;
		//シャドウマップコンスタントバッファ
		shared_ptr<ConstantBuffer> m_shadowmapConstantBuffer;
		///シャドウマップコンスタントバッファのインデックス
		UINT m_shadowmapConstBuffIndex;
		//シャドウマップコンスタントバッファの実体
		ShadowmapConstants m_shadowmapConstantsData;
		//シーン用コンスタントバッファ
		shared_ptr<ConstantBuffer> m_constantBuffer;
		//シーン用コンスタントバッファのインデックス
		UINT m_constBuffIndex;
		//シーン用コンスタントバッファの実体
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
		void SetShadowmapConstants();
	public:
		MoveBox():
			m_posSpan(0.02f),
			m_scale(1.0f),
			m_qt(),
			m_pos(0.0f,1.0f,0.0f)
		{}
		~MoveBox() {}
		void OnInit();
		void OnUpdate();
		void OnDrawShadowmap();
		void OnDraw();
	};


	class Scene :public SceneBase {
		FixedBox m_FixedBox;
		MoveBox m_MoveBox;
		//シャドウマップパイプライン
		ComPtr<ID3D12PipelineState> m_shadowmapPipelineState;
		//シーン描画用パイプライン(影無し)
		ComPtr<ID3D12PipelineState> m_scenePipelineState;
		//シーン描画用パイプライン（影あり）
		ComPtr<ID3D12PipelineState> m_sceneShadowPipelineState;
		//カメラ
		shared_ptr<Camera> m_camera;
		//ライト
		Float3 m_lightPos;
	public:
		Scene() :SceneBase(), m_lightPos(0.0f, 10.0f, 0.0) {}
		virtual ~Scene() {}
		ComPtr<ID3D12PipelineState> GetShadowmapPipelineState() const{
			return m_shadowmapPipelineState;
		}
		ComPtr<ID3D12PipelineState> GetScenePipelineState() const{
			return m_scenePipelineState;
		}
		ComPtr<ID3D12PipelineState> GetSceneShadowPipelineState() const {
			return m_sceneShadowPipelineState;
		}
		shared_ptr<Camera> GetCamera() const{
			return m_camera;
		}
		const Float3& GetLightPos() const {
			return m_lightPos;
		}
		void SetLightPos(const Float3& pos) {
			m_lightPos = pos;
		}
		virtual void OnInit()override;
		virtual void OnInitAssets()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override {}
		virtual void OnDrawPath(UINT index)override;
		virtual void OnDestroy()override;

	};

}
//end basedx12


