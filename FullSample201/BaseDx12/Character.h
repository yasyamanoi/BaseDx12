#pragma once
#include "stdafx.h"

namespace basedx12 {

    class MoveTriangle :public GameObject {
        float m_posSpan;
    public:
        MoveTriangle(const shared_ptr<Stage>& stage) :
            GameObject(stage),
            m_posSpan(1.0f)
        {}
        ~MoveTriangle() {}
        virtual void OnInit()override;
        virtual void OnUpdate()override;
    };

    class MoveTriangle2 :public GameObject {
        float m_posSpan;
    public:
        MoveTriangle2(const shared_ptr<Stage>& stage) :
            GameObject(stage),
            m_posSpan(2.0f)
        {}
        ~MoveTriangle2() {}
        virtual void OnInit()override;
        virtual void OnUpdate()override;
    };



    class MoveSquare :public GameObject {
        float m_posSpan;
        //PositionTexture�̎l�p�`���b�V��
        //shared_ptr<Dx12Mesh> m_ptSquareMesh;
        ////�e�N�X�`��
        //shared_ptr<Dx12Texture> m_SkyTexture;
        ////PositionTexture�p�p�C�v���C���X�e�[�g�i�R���X�^���g�o�b�t�@����j
        //ComPtr<ID3D12PipelineState> m_ptConstPipelineState;
    public:
        MoveSquare(const shared_ptr<Stage>& stage) :
            GameObject(stage),
            m_posSpan(3.0f)
        {}
        ~MoveSquare() {}
        virtual void OnInit()override;
        virtual void OnUpdate()override;
    };
}
//end basedx12
