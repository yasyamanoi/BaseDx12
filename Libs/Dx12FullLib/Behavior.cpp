#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	�s���N���X�̐e�N���X
	//--------------------------------------------------------------------------------------
	Behavior::Behavior(const shared_ptr<GameObject>& gameObjectPtr) :
		m_gameObject(gameObjectPtr)
	{}
	Behavior::~Behavior() {}
	shared_ptr<GameObject> Behavior::GetGameObject() const {
		auto shptr = m_gameObject.lock();
		if (!shptr) {
			throw BaseException(
				L"GameObject�͗L���ł͂���܂���",
				L"if (!shptr)",
				L"Behavior::GetGameObject()"
			);
		}
		else {
			return shptr;
		}
	}

	shared_ptr<Stage> Behavior::GetStage() const {
		return GetGameObject()->GetStage();
	}

}
//end basedx12

