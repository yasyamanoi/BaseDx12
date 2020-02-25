#pragma once
#include "stdafx.h"

namespace basedx12 {

	class GameObject;
	class Stage;

	//--------------------------------------------------------------------------------------
	///	コンポーネント親クラス
	//--------------------------------------------------------------------------------------
	class Component :public ObjectInterface{
		weak_ptr<GameObject> m_gameObject;
		bool m_updateActive{ true };
		bool m_drawActive{ true };
	protected:
		explicit Component(const shared_ptr<GameObject>& gameObjectPtr) :
			m_gameObject(gameObjectPtr) {}
		virtual ~Component() {}
	public:
		shared_ptr<GameObject> GetGameObject() const {
			auto ptr = m_gameObject.lock();
			if (!ptr) {
				throw BaseException(
					L"GameObjectは有効ではありません",
					L"if (!shptr)",
					L"Component::GetGameObject()"
				);
			}
			else {
				return ptr;
			}
		}
		shared_ptr<Stage> GetStage() const;
		bool IsUpdateActive() const {
			return m_updateActive;
		}
		void SetUpdateActive(bool b) {
			m_updateActive = b;
		}
		bool IsDrawActive() const {
			return m_drawActive;
		}
		void SetDrawActive(bool b) {
			m_drawActive = b;
		}
		void AttachGameObject(const shared_ptr<GameObject>& GameObjectPtr);

	};

	//--------------------------------------------------------------------------------------
	///	変換コンポーネント
	//--------------------------------------------------------------------------------------
	class Transform : public Component {
		//1つ前の変数
		Float3 m_beforeScale;
		Float3 m_beforePivot;
		Quat m_beforeQuaternion;
		Float3 m_beforePosition;
		//現在の変数
		Float3 m_scale;
		Float3 m_pivot;
		Quat m_quaternion;
		Float3 m_position;
		bsm::Mat4x4 m_worldMatrix;
		bool m_dirtyFlg;
		//親オブジェクト
		weak_ptr<GameObject> m_parent;
	public:
		explicit Transform(const shared_ptr<GameObject>& gameObjectPtr);
		virtual ~Transform();
		Float3 GetBeforeScale() const;
		Float3 GetBeforePivot() const;
		Quat GetBeforeQuaternion() const;
		Float3 GetBeforeRotation() const;
		Float3 GetBeforePosition() const;
		Float3 GetBeforeWorldPosition() const;
		bool IsSameBeforeWorldMatrix(const Mat4x4& mat) const;
		const Mat4x4 GetBeforeWorldMatrix() const;
		Float3 GetScale() const;
		void SetScale(const Float3& scale);
		void SetScale(float x, float y, float z);
		Float3 GetPivot() const;
		void SetPivot(const Float3& pivot);
		void SetPivot(float x, float y, float z);
		Quat GetQuaternion() const;
		void SetQuaternion(const Quat& qt);
		Float3 GetRotation() const;
		void SetRotation(const Float3& rot);
		void SetRotation(float x, float y, float z);
		Float3 GetPosition() const;
		void SetPosition(const Float3& pos);
		void SetPosition(float x, float y, float z);
		void ResetPosition(const Float3& pos);
		Float3 GetWorldPosition();
		void SetWorldPosition(const Float3& pos);
		void ResetWorldPosition(const Float3& pos);
		bool IsSameWorldMatrix(const Mat4x4& mat);
		const Mat4x4& GetWorldMatrix() ;
		const Mat4x4& Get2DWorldMatrix() ;
		shared_ptr<GameObject> GetParent()const;
		void SetParent(const shared_ptr<GameObject>& obj);
		void ClearParent();
		Float3 GetVelocity() const;
		float GetMoveSize() const;
		void SetToBefore();
		Float3 GetForword();
		Float3 GetUp();
		Float3 GetRight();
		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override {}

		virtual void OnInit()override {}
		virtual void OnDestroy()override {}

	};


}
//end basedx12
