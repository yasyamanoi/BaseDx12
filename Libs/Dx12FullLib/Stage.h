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
		map<wstring, shared_ptr<Camera>> m_cameraMap;
		map<wstring, shared_ptr<LightSet>> m_lightSetMap;
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
		void AddCamera(const shared_ptr<Camera>& camera, const wstring& key);
		shared_ptr<Camera> GetCamera(const wstring& key)const;
		void AddLightSet(const shared_ptr<LightSet>& lightSet, const wstring& key);
		shared_ptr<LightSet> GetLightSet(const wstring& key)const;

		virtual void UpdateGameObjects();
		virtual void DrawGameObjects();
		virtual void DestroyGameObjects();
		virtual void OnUpdate()override {}
		virtual void OnDraw()override {}
		virtual void OnDestroy()override {}
	};

}
//end basedx12
