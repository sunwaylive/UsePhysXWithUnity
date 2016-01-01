#include "physx_common.h"
#include "commlib/logging.h"

using namespace physx;

PhysxCommon::PhysxCommon() {
}

PhysxCommon::~PhysxCommon() {
}

int PhysxCommon::Init() {
	px_foundation_ = PxCreateFoundation(PX_PHYSICS_VERSION, px_allocator_, px_error_callback_);
    if (NULL == px_foundation_) {
        LOG_ERROR(0, 0, "PxCreateFoundation failed");
        return -1;
    }

	// PxProfileZoneManager *profileZoneMgr = &PxProfileZoneManager::createProfileZoneManager(m_foundation);
	// px_physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *px_foundation_, PxTolerancesScale(), true, profileZoneMgr);
    px_physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *px_foundation_, PxTolerancesScale(), true);
    if (NULL == px_physics_) {
        LOG_ERROR(0, 0, "PxCreatePhysics failed");
        return -2;
    }

    return 0;
}

void PhysxCommon::Release() {
    if (NULL != px_physics_) {
        px_physics_->release();
    }

    if (NULL != px_foundation_) {
        px_foundation_->release();
    }
}


