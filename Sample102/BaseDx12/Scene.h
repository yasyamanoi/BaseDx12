#pragma once
#include "GameDevice.h"

namespace basedx12 {

    DECLARE_DX12SHADER(VSPCSprite)
    DECLARE_DX12SHADER(PSPCSprite)

    DECLARE_DX12SHADER(VSPCConstSprite)
    DECLARE_DX12SHADER(PSPCConstSprite)

    DECLARE_DX12SHADER(VSPTSprite)
    DECLARE_DX12SHADER(PSPTSprite)

    struct SceneConstantBuffer
    {
        XMFLOAT4 offset;
        SceneConstantBuffer() :
            offset(0.0f, 0, 0, 0)
        {}
    };

    class FixedTriangle {
        //PositionColor�̎O�p�`���b�V��
        shared_ptr<Dx12Mesh> m_pcTriangleMesh;
        //PositionColor�p�p�C�v���C���X�e�[�g�i�R���X�^���g�o�b�t�@�Ȃ��j
        ComPtr<ID3D12PipelineState> m_pcPipelineState;
    public:
        FixedTriangle() {}
        ~FixedTriangle() {}
        void OnInitAssets();
        void OnUpdate() {}
        void OnRender();
    };

    class MoveTriangle {
        //PositionColor�̎O�p�`���b�V��
        shared_ptr<Dx12Mesh> m_pcTriangleMesh;
        //PositionColor�p�p�C�v���C���X�e�[�g�i�R���X�^���g�o�b�t�@����j
        ComPtr<ID3D12PipelineState> m_pcConstPipelineState;
    public:
        MoveTriangle(){}
        ~MoveTriangle() {}
        void OnInitAssets();
        void OnUpdate();
        void OnRender();
    };

    class MoveSquare {
        //PositionTexture�̎l�p�`���b�V��
        shared_ptr<Dx12Mesh> m_ptSquareMesh;
        //�e�N�X�`��
        shared_ptr<Dx12Texture> m_SkyTexture;
        //PositionTexture�p�p�C�v���C���X�e�[�g�i�R���X�^���g�o�b�t�@����j
        ComPtr<ID3D12PipelineState> m_ptConstPipelineState;
    public:
        MoveSquare() {}
        ~MoveSquare() {}
        void OnInitAssets();
        void OnUpdate();
        void OnRender();
    };

	class Scene :public SceneBase {
        FixedTriangle m_FixedTriangle;
        MoveTriangle m_MoveTriangle;
        MoveSquare m_MoveSquare;
        //�R���X�^���g�o�b�t�@�̎���
        SceneConstantBuffer m_constantBufferData;
        //�R���X�^���g�o�b�t�@
        shared_ptr<ConstantBuffer> m_ConstantBuffer;
	public:
		Scene() :SceneBase() {}
		virtual ~Scene() {}

		virtual void OnInit();
		virtual void OnInitAssets();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnDestroy();
	};

}
//end basedx12

