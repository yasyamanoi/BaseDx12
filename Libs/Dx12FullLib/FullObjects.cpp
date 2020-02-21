#include "stdafx.h"

namespace basedx12 {

	void Stage::PushBackGameObject(const shared_ptr<GameObject>& Ptr) {
		//このステージはクリエイト後である
		if (IsCreated()) {
			m_waitAddObjects.push_back(Ptr);
		}
		else {
			//クリエイト前
			m_gameObjects.push_back(Ptr);
		}
	//	TransformInit();
	}

	//削除オブジェクトの設定
	void Stage::RemoveBackGameObject(const shared_ptr<GameObject>& Ptr) {
		m_waitRemoveObjects.push_back(Ptr);
	}

	void SceneEx::OnUpdate() {
		auto stagePtr = GetActiveStage();
		stagePtr->OnUpdate();
	}
	void SceneEx::OnRender() {
		auto stagePtr = GetActiveStage();
		stagePtr->OnRender();
	}
	void SceneEx::OnDestroy() {
		auto stagePtr = GetActiveStage();
		stagePtr->OnDestroy();
	}


}
//end basedx12
