#pragma once
#include "stdafx.h"

namespace basedx12 {

    DECLARE_DX12SHADER(VSPCSprite)
    DECLARE_DX12SHADER(PSPCSprite)

    DECLARE_DX12SHADER(VSPCConstSprite)
    DECLARE_DX12SHADER(PSPCConstSprite)

    DECLARE_DX12SHADER(VSPTSprite)
    DECLARE_DX12SHADER(PSPTSprite)


    class GameDivece : public Dx12Device {

        //PositionColor�̎O�p�`���b�V��
        shared_ptr<Dx12Mesh> m_pcTriangleMesh;
        shared_ptr<Dx12Mesh> m_pntSquareMesh;
        //�e�N�X�`��
        shared_ptr<Dx12Texture> m_SkyTexture;


        //PositionColor�p�p�C�v���C���X�e�[�g�i�R���X�^���g�o�b�t�@�Ȃ��j
        ComPtr<ID3D12PipelineState> m_pcPipelineState;
        //PositionColor�p�p�C�v���C���X�e�[�g�i�R���X�^���g�o�b�t�@����j
        ComPtr<ID3D12PipelineState> m_pcConstPipelineState;
        //PositionTexture�p�p�C�v���C���X�e�[�g�i�R���X�^���g�o�b�t�@����j
        ComPtr<ID3D12PipelineState> m_ptConstPipelineState;


        struct SceneConstantBuffer
        {
            XMFLOAT4 offset;
            SceneConstantBuffer():
                offset(0.0f,0,0,0)
            {}
        };
        //�R���X�^���g�o�b�t�@�̎���
        SceneConstantBuffer m_constantBufferData;
        //�R���X�^���g�o�b�t�@
        shared_ptr<ConstantBuffer> m_ConstantBuffer;

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
