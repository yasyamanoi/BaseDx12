#pragma once
#include "stdafx.h"

namespace basedx12 {

    //--------------------------------------------------------------------------------------
    ///	Dx12�f�o�C�X�N���X
    //--------------------------------------------------------------------------------------
    class Dx12Device : public ObjectInterface {
        //�R���X�^���g�o�b�t�@��MAX�T�C�Y
        const UINT m_constBuffMax;
        UINT m_constBuffSendIndex;
    protected:
        Dx12Device(UINT frameCount,UINT constBuffMax = 1024);
        virtual ~Dx12Device();
    public:
        UINT GetConstBuffNextIndex();
        UINT GetConstBuffMax() const {
            return m_constBuffMax;
        }
        UINT GetFrameCount()const {
            return m_frameCount;
        }
        UINT GetFrameIndex() const {
            return m_frameIndex;
        }
        bool IsUseWarpDevice() const {
            return m_useWarpDevice;
        }
        ComPtr<ID3D12Device> GetID3D12Device() const {
            return m_device;
        }
        ComPtr<IDXGISwapChain3> GetIDXGISwapChain3() const {
            return m_swapChain;
        }
        const CD3DX12_VIEWPORT& GetViewport() const {
            return m_viewport;
        }
        void SetViewport(const CD3DX12_VIEWPORT& viewport) {
            m_viewport = viewport;
        }
        const CD3DX12_RECT& GetScissorRect() const {
            return m_scissorRect;
        }
        void SetScissorRect(const CD3DX12_RECT& scissorRect) {
            m_scissorRect = scissorRect;
        }
        float GetAspectRatio() const {
            return m_aspectRatio;
        }
        ComPtr<ID3D12CommandQueue> GetCommandQueue() const {
            return m_commandQueue;
        }
        ComPtr<ID3D12Fence> GetFence() const {
            return m_fence;
        }
        const vector<UINT64>& GetFenceValues() const {
            return m_fenceValues;
        }
        const HANDLE& GetFenceEvent() const {
            return m_fenceEvent;
        }
        const vector<ComPtr<ID3D12Resource>>& GetRenderTargets() const {
            return m_renderTargets;
        }
        const vector<ComPtr<ID3D12CommandAllocator>> GetCommandAllocators() const {
            return m_commandAllocators;
        }
        ComPtr<ID3D12DescriptorHeap> GetRtvHeap() {
            return m_rtvHeap;
        }
        UINT GetRtvDescriptorSize() const {
            return m_rtvDescriptorSize;
        }
        ComPtr<ID3D12DescriptorHeap> GetCbvSrvUavDescriptorHeap() const {
            return m_CbvSrvUavDescriptorHeap;
        }
        UINT GetCbvSrvDescriptorHandleIncrementSize() const {
            return m_CbvSrvDescriptorHandleIncrementSize;
        }
        ComPtr<ID3D12DescriptorHeap> GetSamplerDescriptorHeap() const {
            return m_SamplerDescriptorHeap;
        }
        ComPtr<ID3D12GraphicsCommandList> GetCommandList() const {
            return m_commandList;
        }
        ComPtr<ID3D12RootSignature> GetRootSignature() const {
            return m_rootSignature;
        }
        bool Is3DDevice() const {
            return m_Is3DDevice;
        }
    protected:
        //3D�f�o�C�X���ǂ���
        bool m_Is3DDevice;
        //�t���[���֘A
        const UINT m_frameCount;
        UINT m_frameIndex = 0;
        //���b�v�A�_�v�^�[���g�����ǂ����ifalse�j
        bool m_useWarpDevice = false;
        //�f�o�C�X�ƃX���b�v�`�F�[��
        ComPtr<ID3D12Device> m_device;
        ComPtr<IDXGISwapChain3> m_swapChain;
        //�r���[�֘A
        CD3DX12_VIEWPORT m_viewport;
        CD3DX12_RECT m_scissorRect;
        float m_aspectRatio;
        //�R�}���h�L���[
        ComPtr<ID3D12CommandQueue> m_commandQueue;
        //�t�F���X�֘A
        ComPtr<ID3D12Fence> m_fence;
        vector<UINT64> m_fenceValues;
        HANDLE m_fenceEvent;
        //�����_�����O�^�[�Q�[�g
        vector<ComPtr<ID3D12Resource>> m_renderTargets;
        //�R�}���h�A���P�[�^
        vector<ComPtr<ID3D12CommandAllocator>> m_commandAllocators;
        //�f�X�N�v���^�q�[�v
        //�����_�����O�^�[�Q�[�g�r���[�̃f�X�N�v���^�q�[�v
        ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
        UINT m_rtvDescriptorSize = 0;
        //�f�v�X�X�e���V�b��
        ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
        UINT m_dsvDescriptorSize = 0;
        ComPtr<ID3D12Resource> m_depthStencil;
        //�R���X�^���g�o�b�t�@�ƃV�F�[�_���\�[�X�p�f�X�N�v���^�q�[�v
        ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
        //CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
        UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
        //�T���v���[�p
        ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
        //�R�}���h���X�g
        ComPtr<ID3D12GraphicsCommandList> m_commandList;
        //���[�g�V�O�l�`��
        ComPtr<ID3D12RootSignature> m_rootSignature;
        //�����o�֐�
        void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
        void MoveToNextFrame();
        void WaitForGpu();
        void SyncAndWaitForGpu();
    };

}
// end namespace basedx12
