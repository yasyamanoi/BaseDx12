#include "stdafx.h"
#include "Scene.h"

namespace basedx12 {

	void Scene::OnInit() {
		//フレーム数は3に設定
		ResetActiveBaseDevice<GameDevice>(3);
	}

	void Scene::OnInitAssets() {
		//ここでシーン上のオブジェクトを構築
	}

	void Scene::OnUpdate() {
		//ここでシーン上の更新処理を記述
	}

	void Scene::OnDraw() {
		//ここでシーン上の描画処理を記述
	}

	void Scene::OnDestroy() {
		//ここでシーン上の終了処理を記述
	}

}
//end badedx12


