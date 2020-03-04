#pragma once
#include "stdafx.h"

namespace basedx12 {

    class GameStage : public Stage {
        //物理計算オブジェクトの作成
        void CreatePhysicsObjects();
    public:
        GameStage() :Stage() {}
        virtual ~GameStage() {}
        virtual void OnInit()override;
    };

}
//end basedx12
