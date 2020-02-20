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

        virtual void OnInit();
        virtual void OnUpdate();
        virtual void OnRender();
        virtual void OnDestroy();

    };

}
//end basedx12
