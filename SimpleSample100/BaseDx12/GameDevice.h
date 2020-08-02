#pragma once
#include "stdafx.h"

namespace basedx12 {


    class GameDevice : public BaseDevice {
        // App resources.
        void LoadPipeline();
        void LoadAssets();
        void PopulateCommandList();

    public:
        GameDevice(UINT frameCount);
        virtual ~GameDevice() {}

        virtual void OnInit()override;
        virtual void OnUpdate()override;
        virtual void OnDraw()override;
        virtual void OnDestroy()override;

    };

}
//end basedx12
