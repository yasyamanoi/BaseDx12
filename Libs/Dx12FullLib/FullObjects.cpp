#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	ステージ
	//--------------------------------------------------------------------------------------
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

	void Stage::UpdateGameObjects() {
		for (auto& v : m_gameObjects) {
			v->OnUpdate();
		}
	}


	void Stage::RenderGameObjects() {
		for (auto& v : m_gameObjects) {
			v->OnRender();
		}
	}


	//--------------------------------------------------------------------------------------
	///	フルバージョンにおけるシーンの親クラス
	//--------------------------------------------------------------------------------------
	void SceneEx::OnUpdate() {
		auto stagePtr = GetActiveStage();
		stagePtr->UpdateGameObjects();
		stagePtr->OnUpdate();
	}
	void SceneEx::OnRender() {
		auto stagePtr = GetActiveStage();
		stagePtr->RenderGameObjects();
		stagePtr->OnRender();
	}
	void SceneEx::OnDestroy() {
		auto stagePtr = GetActiveStage();
		stagePtr->OnDestroy();
	}




}
//end basedx12
