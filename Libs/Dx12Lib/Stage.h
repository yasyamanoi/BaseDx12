#pragma once

namespace basedx12 {

    class Stage {
    public:
        Stage();
        virtual ~Stage();
        virtual void OnInit() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnRender() = 0;
        virtual void OnDestroy() = 0;
        virtual void OnKeyDown(UINT8 /*key*/) {}
        virtual void OnKeyUp(UINT8 /*key*/) {}
    };

}
//end basedx12
