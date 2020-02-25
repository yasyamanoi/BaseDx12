#include "stdafx.h"

namespace basedx12 {


	//--------------------------------------------------------------------------------------
	///	フルバージョンにおけるシーンの親クラス
	//--------------------------------------------------------------------------------------
	void SceneEx::OnUpdate() {
		auto stagePtr = GetActiveStage();
		stagePtr->UpdateGameObjects();
		stagePtr->OnUpdate();
	}
	void SceneEx::OnDraw() {
		auto stagePtr = GetActiveStage();
		stagePtr->DrawGameObjects();
//		stagePtr->OnDraw();
	}
	void SceneEx::OnDestroy() {
		auto stagePtr = GetActiveStage();
		stagePtr->DestroyGameObjects();
		stagePtr->OnDestroy();
	}




}
//end basedx12
