#pragma once
#include "stdafx.h"
#include "FrameResource.h"

namespace basedx12 {

    class GameDivece : public Dx12Device {
        static const UINT FrameCount = 3;
        static const UINT CityRowCount = 10;
        static const UINT CityColumnCount = 3;
        static const bool UseBundles = true;

        // Pipeline objects.
        CD3DX12_VIEWPORT m_viewport;
        CD3DX12_RECT m_scissorRect;
        ComPtr<IDXGISwapChain3> m_swapChain;
        ComPtr<ID3D12Device> m_device;
        ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
        ComPtr<ID3D12Resource> m_depthStencil;
        ComPtr<ID3D12CommandAllocator> m_commandAllocator;
        ComPtr<ID3D12CommandQueue> m_commandQueue;
        ComPtr<ID3D12RootSignature >m_rootSignature;
        ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
        ComPtr<ID3D12DescriptorHeap> m_cbvSrvHeap;
        ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
        ComPtr<ID3D12DescriptorHeap> m_samplerHeap;
        ComPtr<ID3D12PipelineState> m_pipelineState1;
        ComPtr<ID3D12PipelineState> m_pipelineState2;
        ComPtr<ID3D12GraphicsCommandList> m_commandList;

        // App resources.
        UINT m_numIndices;
        ComPtr<ID3D12Resource> m_vertexBuffer;
        ComPtr<ID3D12Resource> m_indexBuffer;
        ComPtr<ID3D12Resource> m_texture;
        D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
        D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
        StepTimer m_timer;
        UINT m_cbvSrvDescriptorSize;
        UINT m_rtvDescriptorSize;
        SimpleCamera m_camera;

        // Frame resources.
        std::vector<FrameResource*> m_frameResources;
        FrameResource* m_pCurrentFrameResource;
        UINT m_currentFrameResourceIndex;

        // Synchronization objects.
        UINT m_frameIndex;
        UINT m_frameCounter;
        HANDLE m_fenceEvent;
        ComPtr<ID3D12Fence> m_fence;
        UINT64 m_fenceValue;

        void LoadPipeline();
        void LoadAssets();
        void CreateFrameResources();
        void PopulateCommandList(FrameResource* pFrameResource);

    public:
        GameDivece();
        virtual ~GameDivece() {}

        virtual void OnInit();
        virtual void OnUpdate();
        virtual void OnRender();
        virtual void OnDestroy();
        virtual void OnKeyDown(UINT8 key);
        virtual void OnKeyUp(UINT8 key);

    };

}
//end basedx12
