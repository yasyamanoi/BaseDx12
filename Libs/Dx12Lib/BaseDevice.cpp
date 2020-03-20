#include "stdafx.h"

namespace basedx12 {

	BaseDevice::BaseDevice(UINT frameCount, UINT cbvSrvUavMax) :
		m_frameCount(frameCount),
		m_fenceValues(m_frameCount),
		m_renderTargets(m_frameCount),
		m_commandAllocators(m_frameCount),
		m_viewport(0.0f, 0.0f, static_cast<float>(App::GetGameWidth()), static_cast<float>(App::GetGameHeight())),
		m_scissorRect(0, 0, static_cast<LONG>(App::GetGameWidth()), static_cast<LONG>(App::GetGameHeight())),
		m_cbvSrvUavMax(cbvSrvUavMax),
		m_cbvSrvUavSendIndex(0)
	{
		m_aspectRatio = static_cast<float>(App::GetGameWidth()) / static_cast<float>(App::GetGameHeight());
	}

	BaseDevice::~BaseDevice() {}

	UINT BaseDevice::GetCbvSrvUavNextIndex() {
		if (m_cbvSrvUavSendIndex >= m_cbvSrvUavMax) {
			throw BaseException(
				L"これ以上シェーダリソースとコンスタントバッファは増やせません。\n",
				L"Default2DDivece::GetCbvSrvUavNextIndex()"
			);
		}
		return m_cbvSrvUavSendIndex++;
	}



	_Use_decl_annotations_
	void BaseDevice::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
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

	void BaseDevice::SyncAndWaitForGpu() {
		ThrowIfFailed(GetID3D12Device()->CreateFence(m_fenceValues[m_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
		m_fenceValues[m_frameIndex]++;

		// Create an event handle to use for frame synchronization.
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_fenceEvent == nullptr)
		{
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}
		//GPUの処理待ち
		WaitForGpu();
	}



	// Wait for pending GPU work to complete.
	void BaseDevice::WaitForGpu()
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
	void BaseDevice::MoveToNextFrame()
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
