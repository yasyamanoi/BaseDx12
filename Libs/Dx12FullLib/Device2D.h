#pragma once
#include "stdafx.h"

namespace basedx12 {

    //--------------------------------------------------------------------------------------
    ///	一般的な2Dデバイス
    //--------------------------------------------------------------------------------------
    class Default2DDivece : public Dx12Device {
        void LoadPipeline();
        void LoadAssets();
        void PopulateCommandList();
    public:
        Default2DDivece(UINT frameCount);
        virtual ~Default2DDivece() {}
        virtual void OnInit();
        virtual void OnUpdate();
        virtual void OnRender();
        virtual void OnDestroy();
    };


}
// end basedx12
