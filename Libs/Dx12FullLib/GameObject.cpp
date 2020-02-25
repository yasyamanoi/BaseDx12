#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	ゲーム配置オブジェクト親クラス実体
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
					L"所属ステージがnullです。自分自身がステージではありませんか？",
					L"if (!shptr)",
					L"GameObject::GetStage()"
				);
			}
			else {
				return nullptr;
			}
		}
		// 所属ステージがnullだった
		// 自分自身がステージの可能性がある
		return nullptr;
	}

	void GameObject::TransformInit() {
		auto Transptr = GetComponent<Transform>();
		Transptr->SetToBefore();
	}

	void GameObject::OnPreInit() {
		//Transform必須
		AddComponent<Transform>();
	}


	void GameObject::ComponentUpdate() {
		auto Transptr = GetComponent<Transform>();
		//マップを検証してUpdate
		list<type_index>::iterator it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			if (it2 != m_CompMap.end()) {
				//指定の型のコンポーネントが見つかった
				if (it2->second->IsUpdateActive()
					&& (it2->second != Transptr)
					) {
					it2->second->OnUpdate();
				}
			}
			it++;
		}
		//TransformのUpdate
		if (Transptr->IsUpdateActive()) {
			Transptr->OnUpdate();
		}
	}


	void GameObject::ComponentDraw() {
		//Transformがなければ例外
		auto Transptr = GetComponent<Transform>();
		//マップを検証してDraw
		list<type_index>::iterator it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			//指定の型のコンポーネントが見つかった
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
		//マップを検証してUpdate
		list<type_index>::iterator it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			if (it2 != m_CompMap.end()) {
				//指定の型のコンポーネントが見つかった
				if (it2->second != Transptr)
				{
					it2->second->OnDestroy();
				}
			}
			it++;
		}
		//TransformのUpdate
		if (Transptr->IsUpdateActive()) {
			Transptr->OnDestroy();
		}
	}

}
// end basedx12

