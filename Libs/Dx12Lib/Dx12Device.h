#pragma once
#include "stdafx.h"

namespace basedx12 {
    class Dx12Device {
    public:
        Dx12Device();
        virtual ~Dx12Device();
        virtual void OnInit() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnRender() = 0;
        virtual void OnDestroy() = 0;
        virtual void OnKeyDown(UINT8 /*key*/) {}
        virtual void OnKeyUp(UINT8 /*key*/) {}
    protected:
        void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
        // Adapter info.
        bool m_useWarpDevice;
        // Viewport dimensions.
        float m_aspectRatio;
    };

}
// end namespace basedx12
