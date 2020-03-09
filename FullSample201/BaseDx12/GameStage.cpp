#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

    void GameStage::OnInit() {
        auto camera = Camera::CreateCamera<Camera>(Float3(0,3.0f,-5.0f), Float3(0, 0, 0));
        AddCamera(camera, L"MainCamera");
        auto lightSet = LightSet::CreateDefaultLights();
        AddLightSet(lightSet, L"MainLightSet");

        AddGameObject<MoveTriangle>();
        AddGameObject<MoveTriangle2>();
        AddGameObject<MoveSquare>();
        AddGameObject<MoveBox>();
    }

}
//end basedx12
