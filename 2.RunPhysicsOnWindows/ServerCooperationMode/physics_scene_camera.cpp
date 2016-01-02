#include <ctype.h>
#include "physics_scene_camera.h"
#include "foundation/PxMat33.h"

using namespace physx;

namespace PhysicsSceneRender
{
	Camera::Camera(const physx::PxVec3 &eye, const physx::PxVec3 &dir)
		:m_eye(eye), m_dir(dir.getNormalized()), m_mouse_x(0), m_mouse_y(0)
	{
	}
	
	void Camera::HandleMouse(int x, int y)
	{
		m_mouse_x = x;
		m_mouse_x = y;
	}

	bool Camera::HandleKey(unsigned char key, float speed /*= 1.0f*/)
	{
		PxVec3 view_y = m_dir.cross(PxVec3(0, 1, 0)).getNormalized();
		switch (toupper(key))
		{
			case 'W': m_eye += m_dir * speed; break;
			case 'S': m_eye -= m_dir * speed; break;
			case 'A': m_eye -= view_y * speed; break;
			case 'D': m_eye += view_y * speed; break;
			default:	return false;
		}

		return true;
	}

	void Camera::HandleAnalogMove(float x, float y)
	{
		PxVec3 view_y = m_dir.cross(PxVec3(0, 1, 0)).getNormalized();
		m_eye += m_dir * y;
		m_eye += view_y * y;
	}

	void Camera::HandleMotion(int x, int y)
	{
		int dx = m_mouse_x - x;
		int dy = m_mouse_y - y;

		PxVec3 viewY = m_dir.cross(PxVec3(0, 1, 0)).getNormalized();

		PxQuat qx(PxPi * dx / 180.0f, PxVec3(0, 1, 0));
		m_dir = qx.rotate(m_dir);
		PxQuat qy(PxPi * dy / 180.0f, viewY);
		m_dir = qy.rotate(m_dir);

		m_dir.normalize();

		m_mouse_x = x;
		m_mouse_y = y;
	}

	physx::PxTransform Camera::GetTransform() const
{
		PxVec3 viewY = m_dir.cross(PxVec3(0, 1, 0));

		if (viewY.normalize() < 1e-6f)
			return PxTransform(m_eye);

		PxMat33 m(m_dir.cross(viewY), viewY, -m_dir);
		return PxTransform(m_eye, PxQuat(m));
	}

	physx::PxVec3 Camera::GetEye() const
{
		return m_eye;
	}

	physx::PxVec3 Camera::GetDir() const
{
		return m_dir;
	}
}