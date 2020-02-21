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

    class FixedTriangle : public GameObject {
        //PositionColorの三角形メッシュ
        shared_ptr<Dx12Mesh> m_pcTriangleMesh;
        //PositionColor用パイプラインステート（コンスタントバッファなし）
        ComPtr<ID3D12PipelineState> m_pcPipelineState;
    public:
        FixedTriangle(const shared_ptr<Stage>& stage):GameObject(stage){}
        ~FixedTriangle() {}
        void OnInit() override;
        void OnUpdate() override {}
        void OnRender()override;
    };

    class MoveTriangle : public GameObject {
        //PositionColorの三角形メッシュ
        shared_ptr<Dx12Mesh> m_pcTriangleMesh;
        //PositionColor用パイプラインステート（コンスタントバッファあり）
        ComPtr<ID3D12PipelineState> m_pcConstPipelineState;
    public:
        MoveTriangle(const shared_ptr<Stage>& stage) :GameObject(stage) {}
        ~MoveTriangle() {}
        void OnInit()override;
        void OnUpdate()override;
        void OnRender()override;
    };

    class MoveSquare : public GameObject {
        //PositionTextureの四角形メッシュ
        shared_ptr<Dx12Mesh> m_ptSquareMesh;
        //テクスチャ
        shared_ptr<Dx12Texture> m_SkyTexture;
        //PositionTexture用パイプラインステート（コンスタントバッファあり）
        ComPtr<ID3D12PipelineState> m_ptConstPipelineState;
    public:
        MoveSquare(const shared_ptr<Stage>& stage) :GameObject(stage) {}
        ~MoveSquare() {}
        void OnInit()override;
        void OnUpdate()override;
        void OnRender()override;
    };


	class GameStage : public Stage {
        //コンスタントバッファの実体
        SceneConstantBuffer m_constantBufferData;
        //コンスタントバッファ
        shared_ptr<ConstantBuffer> m_ConstantBuffer;
    public:
		GameStage():Stage(){}
		virtual ~GameStage() {}
        virtual void OnInit() override;
		virtual void OnUpdate()override;

	};
}
// end basedx12

