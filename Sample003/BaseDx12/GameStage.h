#pragma once
#include "stdafx.h"
#include "GameDevice.h"

namespace basedx12 {

    class GameStage : public Stage {
        GameDivece m_device;
    public:
        GameStage();
        virtual ~GameStage() {}

        virtual void OnInit();
        virtual void OnUpdate();
        virtual void OnRender();
        virtual void OnDestroy();
        virtual void OnKeyDown(UINT8 key);
        virtual void OnKeyUp(UINT8 key);

    };

}
//end basedx12


