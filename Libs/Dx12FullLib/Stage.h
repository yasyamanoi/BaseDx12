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
		//物理計算
		BasePhysics m_basePhysics;
		//物理計算で使う空物理オブジェクトのset
		set<uint16_t> m_VacantPhysicsIndices;
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
		template<typename T>
		shared_ptr<T> GetTypedCamera(const wstring& key)const {
			shared_ptr<T> ptr = dynamic_pointer_cast<T>(GetCamera(key));
			if (ptr) {
				return ptr;
			}
			else {
				throw BaseException(
					L"カメラがキャストできません",
					L"Stage::GetTypedCamera()"
				);
			}
			return nullptr;
		}

		void AddLightSet(const shared_ptr<LightSet>& lightSet, const wstring& key);
		shared_ptr<LightSet> GetLightSet(const wstring& key)const;
		const BasePhysics& GetBasePhysics() const {
			return m_basePhysics;
		}
		BasePhysics& GetBasePhysics(){
			return m_basePhysics;
		}
		vector<shared_ptr<GameObject>>& GetGameObjects() {
			return m_gameObjects;
		}
		const vector<shared_ptr<GameObject>>& GetGameObjects()const {
			return m_gameObjects;
		}
		uint16_t GetVacantPhysicsIndex();
		void SetVacantPhysicsIndex(uint16_t index);

		virtual void UpdateGameObjects();
		virtual void DrawGameObjects();
		virtual void DestroyGameObjects();
		virtual void OnUpdate()override {}
		virtual void OnDraw()override {}
		virtual void OnDestroy()override {}
	};

}
//end basedx12
