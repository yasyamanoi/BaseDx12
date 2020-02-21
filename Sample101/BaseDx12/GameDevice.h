#pragma once
#include "stdafx.h"

namespace basedx12 {


    class GameDivece : public Dx12Device {
        // App resources.
        void LoadPipeline();
        void LoadAssets();
        void PopulateCommandList();

    public:
        GameDivece(UINT frameCount);
        virtual ~GameDivece() {}

        virtual void OnInit()override;
        virtual void OnUpdate()override;
        virtual void OnRender()override;
        virtual void OnDestroy()override;

    };

}
//end basedx12
