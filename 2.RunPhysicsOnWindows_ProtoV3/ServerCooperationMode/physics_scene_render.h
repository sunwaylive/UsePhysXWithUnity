#ifndef PHYSICS_SCENE_RENDER_H
#define PHYSICS_SCENE_RENDER_H

#include "PxPhysicsAPI.h"
#include "foundation/PxPreprocessor.h"

#if defined(PX_WINDOWS)
#include <windows.h>
#pragma warning(disable: 4505)
#include <GL/glut.h>

#elif defined(PX_LINUX)
#include <GL/glut.h>

#elif defined(PX_APPLE) && !defined(PX_APPLE_IOS)
#include <GLUT/glut.h>

#elif defined(PX_PS3)
#include <GL/glut.h>

#else
#error platform not supported
#endif

namespace PhysicsSceneRender
{
	void SetupDefaultWindow(const char *name);
	void SetupDefaultRenderState();

	void StartRender(const physx::PxVec3 &cameraEye, const physx::PxVec3 &cameraDir);
	void RenderActors(physx::PxRigidActor **actors, const physx::PxU32 numActors, bool shadow = false, const physx::PxVec3 &color = physx::PxVec3(0.0f, 0.75f, 0.0f));
	void FinishRender();
}

#define MAX_NUM_ACTOR_SHAPES 128

#endif //PHYSICS_SCENE_RENDER_H

