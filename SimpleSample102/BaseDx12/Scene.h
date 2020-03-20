#pragma once
#include "GameDevice.h"

namespace basedx12 {

	DECLARE_DX12SHADER(VSPCSprite)
	DECLARE_DX12SHADER(PSPCSprite)

	DECLARE_DX12SHADER(VSPCConstSprite)
	DECLARE_DX12SHADER(PSPCConstSprite)

	DECLARE_DX12SHADER(VSPTSprite)
	DECLARE_DX12SHADER(PSPTSprite)

	struct SceneConstantBuffer
	{
		XMFLOAT4 offset;
		SceneConstantBuffer() :
			offset(0.0f, 0, 0, 0)
		{}
	};

	class FixedTriangle {
		//PositionColorの三角形メッシュ
		shared_ptr<BaseMesh> m_pcTriangleMesh;
		//PositionColor用パイプラインステート（コンスタントバッファなし）
		ComPtr<ID3D12PipelineState> m_pcPipelineState;
	public:
		FixedTriangle() {}
		~FixedTriangle() {}
		void OnInit();
		void OnUpdate() {}
		void OnRender();
	};

	class MoveTriangle {
		//PositionColorの三角形メッシュ
		shared_ptr<BaseMesh> m_pcTriangleMesh;
		//PositionColor用パイプラインステート（コンスタントバッファあり）
		ComPtr<ID3D12PipelineState> m_pcConstPipelineState;
		//コンスタントバッファ構造体の実体
		SceneConstantBuffer m_constantBufferData;
		//コンスタントバッファ
		shared_ptr<ConstantBuffer> m_ConstantBuffer;
		//コンスタントバッファのインデックス
		UINT m_constBuffIndex;
	public:
		MoveTriangle(){}
		~MoveTriangle() {}
		void OnInit();
		void OnUpdate();
		void OnRender();
	};

	class MoveSquare {
		//PositionTextureの四角形メッシュ
		shared_ptr<BaseMesh> m_ptSquareMesh;
		//PositionTexture用パイプラインステート（コンスタントバッファあり）
		ComPtr<ID3D12PipelineState> m_ptConstPipelineState;
		//テクスチャ
		shared_ptr<BaseTexture> m_SkyTexture;
		//テクスチャ（シェーダリソース）のインデックス
		UINT m_srvIndex;
		//コンスタントバッファ構造体の実体
		SceneConstantBuffer m_constantBufferData;
		//コンスタントバッファ
		shared_ptr<ConstantBuffer> m_ConstantBuffer;
		//コンスタントバッファのインデックス
		UINT m_constBuffIndex;
	public:
		MoveSquare() {}
		~MoveSquare() {}
		void OnInit();
		void OnUpdate();
		void OnRender();
	};

	class Scene :public SceneBase {
		FixedTriangle m_FixedTriangle;
		MoveTriangle m_MoveTriangle;
		MoveSquare m_MoveSquare;
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


