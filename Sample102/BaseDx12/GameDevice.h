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
        static const UINT FrameCount = 2;

        struct Vertex
        {
            XMFLOAT3 position;
            XMFLOAT4 color;
        };


        //�p�C�v���C���p
        CD3DX12_VIEWPORT m_viewport;
        CD3DX12_RECT m_scissorRect;

        //�����_�����O�^�[�Q�[�g
        ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
        //�R�}���h�A���P�[�^
        ComPtr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];
        //�R�}���h�L���[
        ComPtr<ID3D12CommandQueue> m_commandQueue;

        //�f�X�N�v���^�q�[�v
        //�����_�����O�^�[�Q�[�g�r���[�̃f�X�N�v���^�q�[�v
        ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
        UINT m_rtvDescriptorSize;
        //�R���X�^���g�o�b�t�@�ƃV�F�[�_���\�[�X�p�f�X�N�v���^�q�[�v
        ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
        //CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
        UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
        //�T���v���[�p
        ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;

        //GPU���f�X�N�v���^�̃n���h���̔z��
        vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;

        //�R�}���h���X�g
        ComPtr<ID3D12GraphicsCommandList> m_commandList;

        //�ʃ��\�[�X
        //���[�g�V�O�l�`��
        ComPtr<ID3D12RootSignature> m_rootSignature;


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
        // Synchronization objects.
        UINT m_frameIndex;
        HANDLE m_fenceEvent;
        ComPtr<ID3D12Fence> m_fence;
        UINT64 m_fenceValues[FrameCount];

        void LoadPipeline();
        void LoadAssets();
        void PopulateCommandList();
        void MoveToNextFrame();
        void WaitForGpu();

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
