#pragma once
#include "GameDevice.h"

namespace basedx12 {


	DECLARE_DX12SHADER(VSPTSprite)
	DECLARE_DX12SHADER(PSPTSprite)

	DECLARE_DX12SHADER(VSPNTStatic)
	DECLARE_DX12SHADER(PSPNTStatic)


	struct SceneConstantBuffer
	{
		XMFLOAT4 offset;
		SceneConstantBuffer() :
			offset(0.0f, 0, 0, 0)
		{}
	};

	//2Dコンスタントバッファ
	struct SpriteConstantBuffer
	{
		Mat4x4 World;
		Float4 Emissive;
		Float4 Diffuse;
		SpriteConstantBuffer() :
			World(),
			Emissive(0.0f),
			Diffuse(1.0f)
		{
		};
	};



	class MoveSquare {
		float m_posSpan;
		//スケール
		Float3 m_scale;
		//回転
		Quat m_qt;
		//位置
		Float3 m_pos;
		//PositionTextureの四角形メッシュ
		shared_ptr<BaseMesh> m_ptSquareMesh;
		//PositionTexture用パイプラインステート（コンスタントバッファあり）
		ComPtr<ID3D12PipelineState> m_ptConstPipelineState;
		//テクスチャ
		shared_ptr<BaseTexture> m_SkyTexture;
		//テクスチャ（シェーダリソース）のインデックス
		UINT m_srvIndex;
		//コンスタントバッファ構造体の実体
		SpriteConstantBuffer m_constantBufferData;
		//コンスタントバッファ
		shared_ptr<ConstantBuffer> m_ConstantBuffer;
		//コンスタントバッファのインデックス
		UINT m_constBuffIndex;
	public:
		MoveSquare():
			m_posSpan(1.0f),
			m_scale(1.0f),
			m_qt(),
			m_pos(0.0f)
		{}
		~MoveSquare() {}
		void OnInit();
		void OnUpdate();
		void OnDraw();
	};

	//--------------------------------------------------------------------------------------
	///	Basicシェーダー用コンスタントバッファ
	//--------------------------------------------------------------------------------------
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
		//汎用フラグ
		XMUINT4 activeFlg;
		//以下影
		Float4 lightPos;
		Float4 eyePos;
		Mat4x4 lightView;
		Mat4x4 lightProjection;

		Float4 bones[3 * 72];

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
		BasicConstants m_constantBufferData;
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



		float m_posSpan;
		//エミッシブ色
		Float4 m_emissive;
		//デフューズ色
		Float4 m_diffuse;
		//透明度
		float m_alpha;
		//スペキュラーとパワー色
		Float4 m_specularColorAndPower;

		void SetConstants();

	public:
		MoveBox():
			m_posSpan(0.02f),
			m_scale(1.0f),
			m_qt(),
			m_pos(0.0f)
		{}
		~MoveBox() {}
		void OnInit();
		void OnUpdate();
		void OnDraw();
	};


	class Scene :public SceneBase {
		MoveSquare m_MoveSquare;
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


