#pragma once
#include "stdafx.h"

namespace basedx12 {

	class Player : public GameObject {
        // コントローラから方向ベクトルを得る
        Float3 GetMoveVector() const;
        //スピード
        float m_Speed;
    public:
        explicit Player(const shared_ptr<Stage>& stage) :
            GameObject(stage),
            m_Speed(6.0f)
        {}
        virtual ~Player() {}
        virtual void OnInit()override;
        virtual void OnUpdate()override;

	};


}
//end basedx12
