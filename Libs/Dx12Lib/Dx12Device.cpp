#include "stdafx.h"

namespace basedx12 {

    Dx12Device::Dx12Device(UINT frameCount) :
        m_FrameCount(frameCount),
        m_fenceValues(m_FrameCount),
        m_renderTargets(m_FrameCount),
        m_commandAllocators(m_FrameCount),
        m_viewport(0.0f, 0.0f, static_cast<float>(App::GetGameWidth()), static_cast<float>(App::GetGameHeight())),
        m_scissorRect(0, 0, static_cast<LONG>(App::GetGameWidth()), static_cast<LONG>(App::GetGameHeight()))
    {
        m_aspectRatio = static_cast<float>(App::GetGameWidth()) / static_cast<float>(App::GetGameHeight());
    }

    Dx12Device::~Dx12Device() {}

    _Use_decl_annotations_
        void Dx12Device::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
    {
        ComPtr<IDXGIAdapter1> adapter;
        *ppAdapter = nullptr;

        for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, pass in "/warp" on the command line.
                continue;
            }

            // Check to see if the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }

        *ppAdapter = adapter.Detach();
    }

    void Dx12Device::CreateRTVandCmdAllocators() {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

        // Create a RTV and a command allocator for each frame.
        for (UINT n = 0; n < m_FrameCount; n++)
        {
            ThrowIfFailed(GetIDXGISwapChain3()->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
            GetID3D12Device()->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, m_rtvDescriptorSize);
            //�R�}���h�A���P�[�^
            m_commandAllocators[n] = CommandAllocator::CreateDefault();
        }

    }

    void Dx12Device::SyncAndWaitForGpu() {
        ThrowIfFailed(GetID3D12Device()->CreateFence(m_fenceValues[m_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
        m_fenceValues[m_frameIndex]++;

        // Create an event handle to use for frame synchronization.
        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_fenceEvent == nullptr)
        {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }
        //GPU�̏����҂�
        WaitForGpu();
    }



    // Wait for pending GPU work to complete.
    void Dx12Device::WaitForGpu()
    {
        // Schedule a Signal command in the queue.
        ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValues[m_frameIndex]));

        // Wait until the fence has been processed.
        ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent));
        WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

        // Increment the fence value for the current frame.
        m_fenceValues[m_frameIndex]++;
    }

    // Prepare to render the next frame.
    void Dx12Device::MoveToNextFrame()
    {
        // Schedule a Signal command in the queue.
        const UINT64 currentFenceValue = m_fenceValues[m_frameIndex];
        ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

        // Update the frame index.
        m_frameIndex = GetIDXGISwapChain3()->GetCurrentBackBufferIndex();

        // If the next frame is not ready to be rendered yet, wait until it is ready.
        if (m_fence->GetCompletedValue() < m_fenceValues[m_frameIndex])
        {
            ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent));
            WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
        }

        // Set the fence value for the next frame.
        m_fenceValues[m_frameIndex] = currentFenceValue + 1;
    }

}
// end namespace basedx12