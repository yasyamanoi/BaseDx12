#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	ステージ
	//--------------------------------------------------------------------------------------
	class Stage : public ObjectInterface {
		void PushBackGameObject(const shared_ptr<GameObject>& Ptr);
		void RemoveBackGameObject(const shared_ptr<GameObject>& Ptr);
		//追加・削除まちオブジェクトの追加と削除
		void RemoveTargetGameObject(const shared_ptr<GameObject>& targetobj);
		void SetWaitToObjectVec();
		vector<shared_ptr<GameObject>> m_gameObjects;
		vector<shared_ptr<GameObject>> m_waitAddObjects;
		vector<shared_ptr<GameObject>> m_waitRemoveObjects;
	protected:
		Stage() {}
		virtual ~Stage() {}

	public:
		template<typename T, typename... Ts>
		shared_ptr<T> AddGameObject(Ts&&... params) {
			try {
				auto Ptr = ObjectFactory::Create<T>(GetThis<Stage>(), params...);
				PushBackGameObject(Ptr);
				return Ptr;
			}
			catch (...) {
				throw;
			}
		}
		virtual void UpdateGameObjects();
		virtual void DrawGameObjects();
		virtual void DestroyGameObjects();
		virtual void OnUpdate()override {}
		virtual void OnDraw()override {}
		virtual void OnDestroy()override {}
	};

}
//end basedx12
