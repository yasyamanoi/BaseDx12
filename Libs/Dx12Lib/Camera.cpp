#include "stdafx.h"

namespace basedx12 {

	void Camera::ThrowIfNotSafe() {
		auto span = m_at - m_eye;
		if (span.length() < 0.001f) {
			throw BaseException(
				L"カメラ位置とカメラ視点の位置が近すぎます。",
				L"Camera::ThrowIfNotSafe()"
			);
		}
	}


	Camera::Camera(const Float3& eye, const Float3& at) :
		m_eye(eye),
		m_at(at),
		m_up(0, 1, 0),
		m_fovY(XM_PIDIV4),
		m_nearPlane(0.01f),
		m_farPlane(125.0f)
	{
		float w = (float)App::GetGameWidth();
		float h = (float)App::GetGameHeight();
		m_aspectRatio = w / h;
	}


	Mat4x4 Camera::GetViewMatrix()
	{
		return (Mat4x4)XMMatrixLookAtLH(m_eye, m_at, m_up);
	}

	Mat4x4 Camera::GetProjectionMatrix()
	{
		return (Mat4x4)XMMatrixPerspectiveFovLH(m_fovY, m_aspectRatio, m_nearPlane, m_farPlane);
	}

}
//end basedx12

