#include <iostream>
#include "physics_scene_manager.h"

#define RENDERFORTEST

PhysicsSceneManager *g_scene_mgr = new PhysicsSceneManager();

int main()
{
#ifdef RENDERFORTEST
	extern void RenderLoop();
	RenderLoop();
#else
	static const PxU32 kframeCount = 100;
	if (!g_scene_mgr->InitPhysics())
	{
		std::cout << "Can't Initialize Scene Manager!" << std::endl;
		return -1;
	}

	std::cout << "Successfully Initialized! " << std::endl;

	for (PxU32 i = 0; i < kframeCount; i++)
		g_scene_mgr->StepPhysics(false);

	g_scene_mgr->CleanPhysics();
#endif

	return 0;
}
