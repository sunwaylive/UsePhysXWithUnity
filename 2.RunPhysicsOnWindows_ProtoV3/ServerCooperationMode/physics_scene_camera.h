#ifndef PHYSICS_SCENE_CAMERA_H
#define PHYSICS_SCENE_CAMERA_H

#include "foundation/PxTransform.h"
#include <iostream>

namespace PhysicsSceneRender
{
	class Camera
	{
	public:
		Camera(const physx::PxVec3 &eye, const physx::PxVec3 &dir);
		~Camera(){};

		void HandleMouse(int x, int y);
		bool HandleKey(unsigned char key, float speed = 0.7f);
		void HandleMotion(int x, int y);
		void HandlePassiveMotion(int x, int y);
		void HandleAnalogMove(float x, float y);

		physx::PxVec3      GetEye()       const;
		physx::PxVec3      GetDir()       const;
		physx::PxTransform GetTransform() const;

	private:
		physx::PxVec3   m_eye;
		physx::PxVec3   m_dir;
		int             m_mouse_x;
		int             m_mouse_y;
	};
}

#endif