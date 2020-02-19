#pragma once
#include "stdafx.h"

namespace basedx12 {

    DECLARE_DX12SHADER(VSPCSprite)
    DECLARE_DX12SHADER(PSPCSprite)

    DECLARE_DX12SHADER(VSPCConstSprite)
    DECLARE_DX12SHADER(PSPCConstSprite)

    DECLARE_DX12SHADER(VSPTSprite)
    DECLARE_DX12SHADER(PSPTSprite)


    class GameDivece : public Dx12Device {

        //PositionColorの三角形メッシュ
        shared_ptr<Dx12Mesh> m_pcTriangleMesh;
        shared_ptr<Dx12Mesh> m_pntSquareMesh;
        //テクスチャ
        shared_ptr<Dx12Texture> m_SkyTexture;


        //PositionColor用パイプラインステート（コンスタントバッファなし）
        ComPtr<ID3D12PipelineState> m_pcPipelineState;
        //PositionColor用パイプラインステート（コンスタントバッファあり）
        ComPtr<ID3D12PipelineState> m_pcConstPipelineState;
        //PositionTexture用パイプラインステート（コンスタントバッファあり）
        ComPtr<ID3D12PipelineState> m_ptConstPipelineState;


        struct SceneConstantBuffer
        {
            XMFLOAT4 offset;
            SceneConstantBuffer():
                offset(0.0f,0,0,0)
            {}
        };
        //コンスタントバッファの実体
        SceneConstantBuffer m_constantBufferData;
        //コンスタントバッファ
        shared_ptr<ConstantBuffer> m_ConstantBuffer;

        void LoadPipeline();
        void LoadAssets();
        void PopulateCommandList();

    public:
        GameDivece();
        virtual ~GameDivece() {}

        virtual void OnInit();
        virtual void OnUpdate();
        virtual void OnRender();
        virtual void OnDestroy();

    };

}
//end basedx12
