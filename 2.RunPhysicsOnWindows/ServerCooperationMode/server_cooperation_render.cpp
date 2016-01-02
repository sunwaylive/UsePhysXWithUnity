#define RENDERFORTEST

#ifdef RENDERFORTEST
#include <vector>
#include "PxPhysicsAPI.h"

#include "physics_scene_render.h"
#include "physics_scene_camera.h"
#include "physics_scene_manager.h"

using namespace physx;

extern PhysicsSceneManager *g_scene_mgr;

namespace
{
	PhysicsSceneRender::Camera* psr_camera;

	void MotionCallback(int x, int y)
	{
		psr_camera->HandleMotion(x, y);
	}
	
	void KeyboardCallback(unsigned char key, int x, int y)
	{
		if (key == 27)
			exit(0);

		if (!psr_camera->HandleKey(key))
			g_scene_mgr->KeyPress(key, psr_camera->GetTransform());
	}

	void MouseCallback(int button, int state, int x, int y)
	{
		psr_camera->HandleMouse(x, y);
	}

	void IdleCallback()
	{
		glutPostRedisplay();
	}

	void RenderCallback()
	{
		g_scene_mgr->StepPhysics(true);

		PhysicsSceneRender::StartRender(psr_camera->GetEye(), psr_camera->GetDir());

		//analyze the scene, render every actor
		PxScene *scene;
		PxGetPhysics().getScenes(&scene, 1);
		PxU32 nbActors = scene->getNbActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC | PxActorTypeSelectionFlag::eRIGID_STATIC);
		if (nbActors != 0)
		{
			std::vector<PxRigidActor*> actors(nbActors);
			scene->getActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC | PxActorTypeSelectionFlag::eRIGID_STATIC, (PxActor**)&actors[0], nbActors);
			PhysicsSceneRender::RenderActors(&actors[0], (PxU32)actors.size(), true);
		}

		PhysicsSceneRender::FinishRender();
	}

	void ExitCallback(void)
	{
		delete psr_camera;
		g_scene_mgr->CleanPhysics();
	}
}

void RenderLoop()
{
	psr_camera = new PhysicsSceneRender::Camera(PxVec3(50.f, 50.f, 50.f), PxVec3(-0.6f, -0.2f, -0.7f));

	PhysicsSceneRender::SetupDefaultWindow("Server Render For Debug");
	PhysicsSceneRender::SetupDefaultRenderState();

	glutIdleFunc(IdleCallback);
	glutDisplayFunc(RenderCallback);
	glutKeyboardFunc(KeyboardCallback);
	glutMouseFunc(MouseCallback);
	glutMotionFunc(MotionCallback);
	MotionCallback(0, 0);

	atexit(ExitCallback);

	g_scene_mgr->InitPhysics();
	glutMainLoop();
}

#endif