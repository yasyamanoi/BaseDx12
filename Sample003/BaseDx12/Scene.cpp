#include "stdafx.h"
#include "Scene.h"
#include "GameStage.h"

namespace basedx12 {

	void Scene::OnInit() {
		auto ptr = ResetActiveStage<GameStage>();
	}
	void Scene::OnUpdate() {
		GetActiveStage<GameStage>()->OnUpdate();
	}
	void Scene::OnRender() {
		GetActiveStage<GameStage>()->OnRender();

	}
	void Scene::OnDestroy() {
		GetActiveStage<GameStage>()->OnDestroy();
	}

	void Scene::OnKeyDown(UINT8 key) {
		GetActiveStage<GameStage>()->OnKeyDown(key);
	}
	void Scene::OnKeyUp(UINT8 key) {
		GetActiveStage<GameStage>()->OnKeyUp(key);
	}

}
//end basedx12

