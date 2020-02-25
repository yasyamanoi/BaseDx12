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
		Ptr->TransformInit();
	}

	//�폜�I�u�W�F�N�g�̐ݒ�
	void Stage::RemoveBackGameObject(const shared_ptr<GameObject>& Ptr) {
		m_waitRemoveObjects.push_back(Ptr);
	}

	void Stage::RemoveTargetGameObject(const shared_ptr<GameObject>& targetobj) {
		auto it = m_gameObjects.begin();
		while (it != m_gameObjects.end()) {
			if (*it == targetobj) {
				//�폜����邱�Ƃ��I�u�W�F�N�g�ɓ`����
				targetobj->ComponentDestroy();
				targetobj->OnDestroy();
				m_gameObjects.erase(it);
				return;
			}
			it++;
		}
	}


	void Stage::SetWaitToObjectVec() {
		if (!m_waitRemoveObjects.empty()) {
			for (auto Ptr : m_waitRemoveObjects) {
				RemoveTargetGameObject(Ptr);
			}
		}
		m_waitRemoveObjects.clear();
		if (!m_waitAddObjects.empty()) {
			for (auto Ptr : m_waitAddObjects) {
				m_gameObjects.push_back(Ptr);
			}
		}
		m_waitAddObjects.clear();
	}


	void Stage::UpdateGameObjects() {
		SetWaitToObjectVec();
		for (auto& v : m_gameObjects) {
			if (v->IsUpdateActive()) {
				auto v2 = v->GetComponent<Transform>();
				v2->SetToBefore();
			}
		}
		for (auto& v : m_gameObjects) {
			v->OnUpdate();
		}
		for (auto& v : m_gameObjects) {
			if (v->IsUpdateActive()) {
				v->ComponentUpdate();
			}
		}
	}


	void Stage::DrawGameObjects() {
		for (auto& v : m_gameObjects) {
			if (v->IsDrawActive()) {
				v->OnDraw();
			}
		}
	}

	void Stage::DestroyGameObjects() {
		for (auto& v : m_gameObjects) {
			v->ComponentDestroy();
		}
		for (auto& v : m_gameObjects) {
			v->OnDestroy();
		}
	}



}
//end basedx12
