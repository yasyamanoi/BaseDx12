#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	�X�e�[�W
	//--------------------------------------------------------------------------------------
	void Stage::PushBackGameObject(const shared_ptr<GameObject>& Ptr) {
		//���̃X�e�[�W�̓N���G�C�g��ł���
		if (IsCreated()) {
			m_waitAddObjects.push_back(Ptr);
		}
		else {
			//�N���G�C�g�O
			m_gameObjects.push_back(Ptr);
		}
	//	TransformInit();
	}

	//�폜�I�u�W�F�N�g�̐ݒ�
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
	///	�t���o�[�W�����ɂ�����V�[���̐e�N���X
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