#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	�Q�[���z�u�I�u�W�F�N�g�e�N���X����
	//--------------------------------------------------------------------------------------
	GameObject::GameObject(const shared_ptr<Stage>& StagePtr) :
		ObjectInterface(),
		m_Stage(StagePtr)
	{}

	GameObject::~GameObject() {}

	shared_ptr<Stage> GameObject::GetStage(bool ExceptionActive) const {
		auto shptr = m_Stage.lock();
		if (shptr) {
			return shptr;
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"�����X�e�[�W��null�ł��B�������g���X�e�[�W�ł͂���܂��񂩁H",
					L"if (!shptr)",
					L"GameObject::GetStage()"
				);
			}
			else {
				return nullptr;
			}
		}
		// �����X�e�[�W��null������
		// �������g���X�e�[�W�̉\��������
		return nullptr;
	}

	void GameObject::TransformInit() {
		auto Transptr = GetComponent<Transform>();
		Transptr->SetToBefore();
	}

	void GameObject::OnPreInit() {
		//Transform�K�{
		AddComponent<Transform>();
	}


	void GameObject::ComponentUpdate() {
		auto Transptr = GetComponent<Transform>();
		//�}�b�v�����؂���Update
		list<type_index>::iterator it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			if (it2 != m_CompMap.end()) {
				//�w��̌^�̃R���|�[�l���g����������
				if (it2->second->IsUpdateActive()
					&& (it2->second != Transptr)
					) {
					it2->second->OnUpdate();
				}
			}
			it++;
		}
		//Transform��Update
		if (Transptr->IsUpdateActive()) {
			Transptr->OnUpdate();
		}
	}


	void GameObject::ComponentDraw() {
		//Transform���Ȃ���Η�O
		auto Transptr = GetComponent<Transform>();
		//�}�b�v�����؂���Draw
		list<type_index>::iterator it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			//�w��̌^�̃R���|�[�l���g����������
			if (it2 != m_CompMap.end()) {
				if (it2->second->IsDrawActive()
					) {
					it2->second->OnDraw();
				}
			}
			it++;
		}
	}


	void GameObject::OnDraw() {
		ComponentDraw();
	}

	void GameObject::ComponentDestroy() {
		auto Transptr = GetComponent<Transform>();
		//�}�b�v�����؂���Update
		list<type_index>::iterator it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			if (it2 != m_CompMap.end()) {
				//�w��̌^�̃R���|�[�l���g����������
				if (it2->second != Transptr)
				{
					it2->second->OnDestroy();
				}
			}
			it++;
		}
		//Transform��Update
		if (Transptr->IsUpdateActive()) {
			Transptr->OnDestroy();
		}
	}

}
// end basedx12

