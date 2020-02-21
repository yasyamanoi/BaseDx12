#include "stdafx.h"
#include "GameStage.h"
#include "Scene.h"


namespace basedx12 {

	void Scene::OnInit() {
		//ここでは使用するデバイスを決める
		ResetActiveDx12Device<Default2DDivece>(3);
	}
	void Scene::OnInitAssets() {
		//ここでは最初に設置するステージを決める
        auto stagePtr = ResetActiveStage<GameStage>();
    }


}
//end badedx12


