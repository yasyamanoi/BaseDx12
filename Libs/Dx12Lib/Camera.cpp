#include "stdafx.h"

namespace basedx12 {

    Camera::Camera(const Float3& eye, const Float3& lookDirection):
        m_eye(eye),
        m_lookDirection(lookDirection),
        m_upDirection(0, 1, 0),
        m_fovY(XM_PIDIV4),
        m_nearPlane(1.0f),
        m_farPlane(1000.0f)

    {
        float w = (float)App::GetGameWidth();
        float h = (float)App::GetGameHeight();
        m_aspectRatio = w / h;
    }


    Mat4x4 Camera::GetViewMatrix()
    {
        return (Mat4x4)XMMatrixLookToLH(m_eye, m_lookDirection, m_upDirection);
    }

    Mat4x4 Camera::GetProjectionMatrix()
    {
        return (Mat4x4)XMMatrixPerspectiveFovLH(m_fovY, m_aspectRatio, m_nearPlane, m_farPlane);
    }

    shared_ptr<Camera>
    Camera::CreateCameraWithDir(const Float3& eye, const Float3& lookDirection) {
        try {
            shared_ptr<Camera> Ptr = shared_ptr<Camera>(new Camera(eye, lookDirection));
            return Ptr;
        }
        catch (...) {
            throw;
        }

    }

    shared_ptr<Camera>
    Camera::CreateCameraWithAt(const Float3& eye, const Float3& at) {
        try {
            Float3 dir = at - eye;
            dir.normalize();
            shared_ptr<Camera> Ptr = shared_ptr<Camera>(new Camera(eye, dir));
            return Ptr;
        }
        catch (...) {
            throw;
        }
    }



}
//end basedx12

