#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

	void Scene::OnInit() {
		ResetActiveBaseDevice<Default3DDivece>(3);
	}

	void Scene::OnInitAssets() {
		ResetActiveStage<GameStage>();
	}

}
//end badedx12


