#pragma once
#include "stdafx.h"

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
        shared_ptr<Dx12Mesh> m_pcTriangleMesh;
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
        shared_ptr<Dx12Mesh> m_pcTriangleMesh;
        //PositionColor用パイプラインステート（コンスタントバッファあり）
        ComPtr<ID3D12PipelineState> m_pcConstPipelineState;
    public:
        MoveTriangle() {}
        ~MoveTriangle() {}
        void OnInit();
        void OnUpdate();
        void OnRender();
    };

    class MoveSquare {
        //PositionTextureの四角形メッシュ
        shared_ptr<Dx12Mesh> m_ptSquareMesh;
        //テクスチャ
        shared_ptr<Dx12Texture> m_SkyTexture;
        //PositionTexture用パイプラインステート（コンスタントバッファあり）
        ComPtr<ID3D12PipelineState> m_ptConstPipelineState;
    public:
        MoveSquare() {}
        ~MoveSquare() {}
        void OnInit();
        void OnUpdate();
        void OnRender();
    };


	class GameStage : public Stage {
        FixedTriangle m_FixedTriangle;
        MoveTriangle m_MoveTriangle;
        MoveSquare m_MoveSquare;
        //コンスタントバッファの実体
        SceneConstantBuffer m_constantBufferData;
        //コンスタントバッファ
        shared_ptr<ConstantBuffer> m_ConstantBuffer;
    public:
		GameStage():Stage(){}
		virtual ~GameStage() {}
        virtual void OnInit() override;
		virtual void OnUpdate()override;
		virtual void OnRender()override;
		virtual void OnDestroy()override {}

	};
}
// end basedx12

