#pragma once
#include "stdafx.h"

namespace basedx12 {

    class GameStage : public Stage {
        //�����v�Z�I�u�W�F�N�g�̍쐬
        void CreatePhysicsObjects();
    public:
        GameStage() :Stage() {}
        virtual ~GameStage() {}
        virtual void OnInit()override;
    };

}
//end basedx12
