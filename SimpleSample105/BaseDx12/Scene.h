#pragma once
#include "GameDevice.h"

namespace basedx12 {

	DECLARE_DX12SHADER(BcVSPNTStaticPLShadow)
	DECLARE_DX12SHADER(BcPSPNTPLShadow)

	DECLARE_DX12SHADER(VSShadowmap)

	struct ShadowmapConstants {
		Mat4x4 world;
		Mat4x4 view;
		Mat4x4 proj;
	};

	struct BasicConstants
	{
		Float4 diffuseColor;
		Float4 emissiveColor;
		Float4 specularColorAndPower;

		Float4 lightDirection[3];
		Float4 lightDiffuseColor[3];
		Float4 lightSpecularColor[3];

		Float4 eyePosition;

		Float4 fogColor;
		Float4 fogVector;

		Mat4x4 world;
		Float4 worldInverseTranspose[3];
		Mat4x4 worldViewProj;
		//以下影
		Float4 lightPos;
		Float4 eyePos;
		Mat4x4 lightView;
		Mat4x4 lightProjection;
		Float4 bones[3 * 72];
	};


	class FixedBox {
		shared_ptr<BaseMesh> m_mesh;
		//コンスタントバッファ
		shared_ptr<ConstantBuffer> m_bcConstantBuffer;
		//コンスタントバッファのインデックス
		UINT m_bcConstBuffIndex;
		//コンスタントバッファの実体
		BasicConstants m_bcShadowSceneConstantsData;
		//テクスチャ
		shared_ptr<BaseTexture> m_skyTexture;
		//テクスチャ（シェーダリソース）のインデックス
		UINT m_srvIndex;
		//スケール
		Float3 m_scale;
		//回転
		Quat m_qt;
		//位置
		Float3 m_pos;
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
		shared_ptr<ConstantBuffer> m_bcConstantBuffer;
		//コンスタントバッファのインデックス
		UINT m_bcConstBuffIndex;
		//コンスタントバッファの実体
		BasicConstants m_bcShadowSceneConstantsData;
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
		void SetShadowmapConstants();
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
		void OnDrawShadowmap();
		void OnDraw();
	};


	class Scene :public SceneBase {
		FixedBox m_FixedBox;
		MoveBox m_MoveBox;
		//シャドウマップパイプライン
		ComPtr<ID3D12PipelineState> m_shadowmapPipelineState;
		//Basicパイプライン（影あり）
		ComPtr<ID3D12PipelineState> m_bcSceneShadowPipelineState;
		//カメラ
		shared_ptr<Camera> m_camera;
		//ライト
		Float3 m_lightPos[3];
		//GPUのスロットマップ
		map<wstring, UINT> m_gpuSlotMap;
		//フォグを使うかどうか
		bool m_fogEnabled;
	public:
		Scene() :SceneBase(), m_fogEnabled(true){}
		virtual ~Scene() {}
		ComPtr<ID3D12PipelineState> GetShadowmapPipelineState() const{
			return m_shadowmapPipelineState;
		}
		ComPtr<ID3D12PipelineState> GetBcPipelineState() const {
			return m_bcSceneShadowPipelineState;
		}
		shared_ptr<Camera> GetCamera() const{
			return m_camera;
		}
		const Float3& GetLightPos(UINT num) const {
			return m_lightPos[num];
		}
		void SetLightPos(UINT num,const Float3& pos) {
			m_lightPos[num] = pos;
		}
		void SetBcConstants(
			const Float3& scale,
			const Quat& qt,
			const Float3& pos,
			BasicConstants& ret
		);
		virtual void OnInit()override;
		virtual void OnInitAssets()override;
		virtual void OnUpdate()override;
		virtual void OnDraw()override {}
		virtual void OnDrawPath(UINT index)override;
		virtual void OnDestroy()override;

	};

}
//end basedx12


