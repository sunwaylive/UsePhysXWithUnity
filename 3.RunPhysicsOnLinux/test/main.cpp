#include "physx_wrapper/physx_common.h"
#include "physx_wrapper/physx_scene.h"
#include <iostream>
#include "commlib/logging.h"

int main() {
    PhysxCommonSigleton::get_mutable_instance().Init();

    PhysxScene* scene = new PhysxScene();
    scene->InitBase();
    scene->InitFromFile("wall.scene");

    physx::PxVec3 a(-1.96f, 0.83f, 0.0f);
    physx::PxVec3 b(3.84f, 0.68f, 1.11f);

    physx::PxVec3 c(2.59f, 1.0f, -3.9f);
    physx::PxVec3 d(-2.06f, 1.0f, -3.9f);

    bool is_reach = scene->CheckReachable(a, b);
    LOG_DEBUG(0, 0, "a b is_reach|%d", is_reach);

    is_reach = scene->CheckReachable(c, d);
    LOG_DEBUG(0, 0, "c d is_reach|%d", is_reach);


    scene->Release();

    PhysxCommonSigleton::get_mutable_instance().Release();
	return 0;
}

