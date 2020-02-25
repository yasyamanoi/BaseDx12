#include "stdafx.h"

namespace basedx12 {


	//--------------------------------------------------------------------------------------
	///	�t���o�[�W�����ɂ�����V�[���̐e�N���X
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
