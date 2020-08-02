#pragma once
#include "stdafx.h"

namespace basedx12 {

	class GameObject;
	class Stage;
	//--------------------------------------------------------------------------------------
	///	行動クラスの親クラス
	//--------------------------------------------------------------------------------------
	class Behavior : public ObjectInterface {
		weak_ptr<GameObject> m_gameObject;
	protected:
		explicit Behavior(const shared_ptr<GameObject>& gameObjectPtr);
		virtual ~Behavior();
	public:
		shared_ptr<GameObject> GetGameObject() const;
		shared_ptr<Stage> GetStage() const;
		virtual void OnInit() {}
	};


}
//end basedx12
