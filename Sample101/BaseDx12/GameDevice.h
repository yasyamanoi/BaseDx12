#pragma once
#include "stdafx.h"

namespace basedx12 {

    DECLARE_DX12SHADER(VSPCSprite)
    DECLARE_DX12SHADER(PSPCSprite)


    class GameDivece : public Dx12Device {

        struct Vertex
        {
            XMFLOAT3 position;
            XMFLOAT4 color;
        };

        // App resources.
        ComPtr<ID3D12PipelineState> m_pipelineState;
        shared_ptr<Dx12Mesh> m_Dx12Mesh;
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
