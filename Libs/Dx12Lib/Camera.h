#pragma once
#include "stdafx.h"


namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	ÉJÉÅÉâÉNÉâÉX
	//--------------------------------------------------------------------------------------
	class Camera
	{
	protected:
		Float3 m_eye;
		Float3 m_lookDirection;
		Float3 m_upDirection;
		float m_fovY;
		float m_aspectRatio;
		float m_nearPlane;
		float m_farPlane;
		Camera(const Float3& eye, const Float3& lookDirection);
	public:
		virtual ~Camera() {}
		Mat4x4 GetViewMatrix();
		Mat4x4 GetProjectionMatrix();
		static shared_ptr<Camera>
			CreateCameraWithDir(const Float3& eye, const Float3& lookDirection);
		static shared_ptr<Camera>
			CreateCameraWithAt(const Float3& eye, const Float3& at);
		Float3 GetEye() const { return m_eye; }
		void SetEye(const Float3& eye) { m_eye = eye; }
		Float3 GetLookDirection() const { return m_lookDirection; }
		void SetLookDirection(const Float3& dir) { m_lookDirection = dir; }
		Float3 GetUpDirection() const { return m_upDirection; }
		void SetUpDirection(const Float3& dir) { m_upDirection = dir; }
		float GetFovY()const { return m_fovY; }
		void SetFovY(float fov) { m_fovY = fov; }
		float GetAspectRatio()const { return m_aspectRatio; }
		void SetAspectRatio(float ratio) { m_aspectRatio = ratio; }
		float GetNearPlane()const { return m_nearPlane; }
		void SetNearPlane(float nrar) { m_nearPlane = nrar; }
		float GetFarPlane()const { return m_farPlane; }
		void SetFarPlane(float f) { m_farPlane = f; }
	};
}
//end basedx12
