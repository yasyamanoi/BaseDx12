#pragma once
#include "stdafx.h"

namespace basedx12 {
    class Dx12Device {
        //デバイスとスワップチェーンは基底クラスに置く
        ComPtr<ID3D12Device> m_device;
        ComPtr<IDXGISwapChain3> m_swapChain;
    public:
        Dx12Device();
        virtual ~Dx12Device();
        virtual void OnInit() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnRender() = 0;
        virtual void OnDestroy() = 0;
        virtual void OnKeyDown(UINT8 /*key*/) {}
        virtual void OnKeyUp(UINT8 /*key*/) {}
        ComPtr<ID3D12Device> GetID3D12Device() const {
            return m_device;
        }
        ComPtr<IDXGISwapChain3> GetIDXGISwapChain3() const {
            return m_swapChain;
        }
    protected:
        static const UINT FrameCount = 3;
        void SetID3D12Device(const ComPtr<ID3D12Device>& device) {
            m_device = device;
        }
        void SetIDXGISwapChain3(const ComPtr<IDXGISwapChain3>& swapChain){
            m_swapChain = swapChain;
        }
        void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
        // Adapter info.
        bool m_useWarpDevice;

        // Viewport
        CD3DX12_VIEWPORT m_viewport;
        CD3DX12_RECT m_scissorRect;
        float m_aspectRatio;

        void MoveToNextFrame();
        void WaitForGpu();

        void CreateRTVandCmdAllocators();
        void SyncAndWaitForGpu();

        //コマンドキュー
        ComPtr<ID3D12CommandQueue> m_commandQueue;
        ComPtr<ID3D12Fence> m_fence;
        UINT64 m_fenceValues[FrameCount];
        UINT m_frameIndex;
        HANDLE m_fenceEvent;

        //レンダリングターゲート
        ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
        //コマンドアロケータ
        ComPtr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];

        //デスクプリタヒープ
        //レンダリングターゲートビューのデスクプリタヒープ
        ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
        UINT m_rtvDescriptorSize;
        //コンスタントバッファとシェーダリソース用デスクプリタヒープ
        ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
        //CbvSrvのデスクプリタハンドルのインクリメントサイズ
        UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
        //サンプラー用
        ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;

        //GPU側デスクプリタのハンドルの配列
        vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;

        //コマンドリスト
        ComPtr<ID3D12GraphicsCommandList> m_commandList;

        //ルートシグネチャ
        ComPtr<ID3D12RootSignature> m_rootSignature;


    };

}
// end namespace basedx12
