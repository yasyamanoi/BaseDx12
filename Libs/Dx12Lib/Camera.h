#pragma once
#include "stdafx.h"


namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	カメラクラス
	//--------------------------------------------------------------------------------------
	class Camera
	{
	protected:
		Float3 m_eye;
		Float3 m_at;
		Float3 m_up;
		float m_fovY;
		float m_aspectRatio;
		float m_nearPlane;
		float m_farPlane;
		Camera(const Float3& eye, const Float3& at);
		void ThrowIfNotSafe();
	public:
		virtual ~Camera() {}
		Mat4x4 GetViewMatrix();
		Mat4x4 GetProjectionMatrix();
		template <typename T>
		static shared_ptr<T>
		CreateCamera(const Float3& eye, const Float3& at) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T(eye,at));
			auto chkPtr = dynamic_pointer_cast<Camera>(Ptr);
			if (!chkPtr) {
				throw BaseException(
					L"指定のクラスはカメラに変換できません",
					L"Camera::CreateCamera()"
				);
			}
			Ptr->ThrowIfNotSafe();
			return Ptr;
		}
		Float3 GetEye() const { return m_eye; }
		void SetEye(const Float3& eye) {
			m_eye = eye;
			ThrowIfNotSafe();
		}
		Float3 GetAt() const { return m_at; }
		void SetAt(const Float3& at) {
			m_at = at; 
			ThrowIfNotSafe();
		}
		Float3 GetUpDirection() const { return m_up; }
		void SetUp(const Float3& up) {
			m_up = up; 
			m_up.normalize();
		}
		float GetFovY()const { return m_fovY; }
		void SetFovY(float fov) { m_fovY = fov; }
		float GetAspectRatio()const { return m_aspectRatio; }
		void SetAspectRatio(float ratio) { m_aspectRatio = ratio; }
		float GetNearPlane()const { return m_nearPlane; }
		void SetNearPlane(float nrar) { m_nearPlane = nrar; }
		float GetFarPlane()const { return m_farPlane; }
		void SetFarPlane(float f) { m_farPlane = f; }
		virtual void OnUpdate() {}
	};
}
//end basedx12
