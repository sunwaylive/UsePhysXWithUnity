#pragma once

#include "PhysXSDK/Include/PxPhysicsAPI.h"
#include "PhysXSDK/Include/characterkinematic/PxControllerManager.h"
#include "PhysXSDK/Include/characterkinematic/PxControllerBehavior.h"
#include "physx_scene_config.pb.h"

class PhysxScene {
public:
	PhysxScene();
	~PhysxScene();

    int InitBase();
    int InitFromFile(const std::string& file_path);

    void Release();

    // 检查两点之间是否有障碍物
    bool CheckReachable(physx::PxVec3 src, physx::PxVec3 dst);


private:
	void AddBoxFromU3D(killer::proto::U3DPhysxBox& box);
	void AddSphereFromU3D(killer::proto::U3DPhysxSphere& sphere);
	void AddCapsuleFromU3D(killer::proto::U3DPhysxCapsule& capsule);

private:
    physx::PxScene* px_scene_;
	physx::PxMaterial* px_material_;
	physx::PxControllerManager* px_controller_mgr_;
	physx::PxDefaultCpuDispatcher* px_dispatcher_;
};

