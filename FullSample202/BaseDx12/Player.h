#pragma once
#include "stdafx.h"

namespace basedx12 {

	class Player : public GameObject {
        // �R���g���[����������x�N�g���𓾂�
        Float3 GetMoveVector() const;
        //�X�s�[�h
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
