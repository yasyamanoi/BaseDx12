#pragma once
#include "stdafx.h"

namespace basedx12 {


    struct SceneConstantBuffer
    {
        XMFLOAT4 offset;
        SceneConstantBuffer() :
            offset(0.0f, 0, 0, 0)
        {}
    };

    class FixedTriangle :public GameObject {
        //PositionColorの三角形メッシュ
        shared_ptr<Dx12Mesh> m_pcTriangleMesh;
        //PositionColor用パイプラインステート（コンスタントバッファなし）
        ComPtr<ID3D12PipelineState> m_pcPipelineState;
    public:
        FixedTriangle(const shared_ptr<Stage>& stage):GameObject(stage){}
        ~FixedTriangle() {}
        virtual void OnInit()override;
        virtual void OnUpdate()override {}
        virtual void OnDraw()override;
    };

    class MoveTriangle :public GameObject {
    public:
        MoveTriangle(const shared_ptr<Stage>& stage) :GameObject(stage) {}
        ~MoveTriangle() {}
        virtual void OnInit()override;
        virtual void OnUpdate()override;
    };

    class MoveTriangle2 :public GameObject {
    public:
        MoveTriangle2(const shared_ptr<Stage>& stage) :GameObject(stage) {}
        ~MoveTriangle2() {}
        virtual void OnInit()override;
        virtual void OnUpdate()override;
    };



    class MoveSquare :public GameObject {
        //PositionTextureの四角形メッシュ
        shared_ptr<Dx12Mesh> m_ptSquareMesh;
        //テクスチャ
        shared_ptr<Dx12Texture> m_SkyTexture;
        //PositionTexture用パイプラインステート（コンスタントバッファあり）
        ComPtr<ID3D12PipelineState> m_ptConstPipelineState;
    public:
        MoveSquare(const shared_ptr<Stage>& stage) :GameObject(stage) {}
        ~MoveSquare() {}
        virtual void OnInit()override;
        virtual void OnUpdate()override;
    };

    class GameStage : public Stage {
        //コンスタントバッファの実体
        SceneConstantBuffer m_constantBufferData;
        //コンスタントバッファ
        shared_ptr<ConstantBuffer> m_ConstantBuffer;
    public:
        GameStage() :Stage(){}
        virtual ~GameStage() {}
        virtual void OnInit()override;
    };

	class Scene :public SceneEx {
	public:
		Scene() :SceneEx() {}
		virtual ~Scene() {}

		virtual void OnInit()override;
		virtual void OnInitAssets()override;
	};

}
//end basedx12


