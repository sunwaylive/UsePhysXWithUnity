#pragma once

#include "PhysXSDK/Include/PxPhysicsAPI.h"
#include "commlib/singleton.h"

class PhysxCommon {
public:
	PhysxCommon();
	~PhysxCommon();

    int Init();
    void Release();

    physx::PxFoundation& GetFoundation() {
        return *px_foundation_;
    }

    physx::PxPhysics& GetPhysics() {
        return *px_physics_;
    }

private:
    physx::PxFoundation* px_foundation_;
    physx::PxPhysics* px_physics_;

	physx::PxDefaultAllocator px_allocator_;
	physx::PxDefaultErrorCallback px_error_callback_;
};

typedef tnt::Singleton<PhysxCommon> PhysxCommonSigleton;

