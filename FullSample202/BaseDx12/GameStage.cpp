#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	//物理計算オブジェクトの作成
	void GameStage::CreatePhysicsObjects() {
		Quat qt1, qt2, qt3, qt4;
		qt1.rotationZ(2.0f);
		qt2.rotationX(0.7f);
		qt3.rotationZ(-2.0f);
		qt4.rotationX(-0.7f);
		//下の台
		AddGameObject<FixedPsBox>(Float3(30.0f, 1.0f, 30.0f), Quat(), Float3(0.0f, -0.5f, 0.0f));
		//上から降ってくるボックス
		AddGameObject<ActivePsBox>(Float3(2.0f, 1.0f, 1.0f), Quat(), Float3(2.0f, 1.0f, 4.0f));
		AddGameObject<ActivePsBox>(Float3(2.0f, 1.0f, 1.0f), qt2, Float3(1.0f, 3.0f, 3.0f));

	}


    void GameStage::OnInit() {
        auto camera = Camera::CreateCamera<MyCamera>(Float3(0, 3.0f, -5.0f), Float3(0, 0, 0));
		AddCamera(camera, L"MainCamera");
        auto lightSet = LightSet::CreateDefaultLights();
        AddLightSet(lightSet, L"MainLightSet");

		CreatePhysicsObjects();
        AddGameObject<MoveBox>();

		AddGameObject<Player>();

    }

}
//end basedx12
