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
        void SetID3D12Device(const ComPtr<ID3D12Device>& device) {
            m_device = device;
        }
        void SetIDXGISwapChain3(const ComPtr<IDXGISwapChain3>& swapChain){
            m_swapChain = swapChain;
        }
        void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
        // Adapter info.
        bool m_useWarpDevice;
        // Viewport dimensions.
        float m_aspectRatio;
    };

}
// end namespace basedx12
