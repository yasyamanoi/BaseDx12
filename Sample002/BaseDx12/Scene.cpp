#include "stdafx.h"
#include "Scene.h"

namespace basedx12 {

	void Scene::OnInit() {
		ResetActiveDx12Device<GameDivece>();
	}
	void Scene::OnUpdate() {
		App::GetDx12Device()->OnUpdate();
	}
	void Scene::OnRender() {
		App::GetDx12Device()->OnRender();
	}
	void Scene::OnDestroy() {
		App::GetDx12Device()->OnDestroy();
	}

}
//end badedx12

