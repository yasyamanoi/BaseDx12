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
		//�����I�u�W�F�N�g�̃t�H�[�X�̏�����
		m_basePhysics.InitForce();
		for (auto& v : m_gameObjects) {
			v->OnUpdate();
		}
		m_basePhysics.Update(false);
		for (auto& v : m_gameObjects) {
			if (v->IsUpdateActive()) {
				v->ComponentUpdate();
			}
		}
		//�J������update
		for (auto it = m_cameraMap.begin(); it != m_cameraMap.end(); it++) {
			it->second->OnUpdate();
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

	void Stage::AddCamera(const shared_ptr<Camera>& camera, const wstring& key) {
		m_cameraMap[key] = camera;
	}

	shared_ptr<Camera> Stage::GetCamera(const wstring& key) const{
		auto it = m_cameraMap.find(key);
		if (it != m_cameraMap.end()) {
			return it->second;
		}
		else {
			throw BaseException(
				L"�w��̃L�[�̃J������������܂���",
				key,
				L"Stage::GetCamera()"
			);
		}
		return nullptr;
	}

	void Stage::AddLightSet(const shared_ptr<LightSet>& lightSet, const wstring& key) {
		m_lightSetMap[key] = lightSet;

	}
	shared_ptr<LightSet> Stage::GetLightSet(const wstring& key)const {
		auto it = m_lightSetMap.find(key);
		if (it != m_lightSetMap.end()) {
			return it->second;
		}
		else {
			throw BaseException(
				L"�w��̃L�[�̃��C�g�Z�b�g��������܂���",
				key,
				L"Stage::GetLightSet()"
			);
		}
		return nullptr;
	}


	uint16_t Stage::GetVacantPhysicsIndex() {
		uint16_t ret = UINT16_MAX;
		if (m_VacantPhysicsIndices.size() > 0) {
			//�󂢂Ă���ID������B�擪���擾
			auto it = m_VacantPhysicsIndices.begin();
			ret = *it;
			//ID���g���̂ł���ID���폜
			m_VacantPhysicsIndices.erase(ret);
		}
		return ret;
	}

	void Stage::SetVacantPhysicsIndex(uint16_t index) {
		m_VacantPhysicsIndices.insert(index);
	}



}
//end basedx12
