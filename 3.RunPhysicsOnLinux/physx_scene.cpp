#include "physx_scene.h"
#include "physx_common.h"
#include "commlib/logging.h"
#include <fstream>

using namespace physx;
using namespace killer::proto;

PhysxScene::PhysxScene() {

}

PhysxScene::~PhysxScene() {
}

void PhysxScene::Release() {
    if (NULL != px_material_) {
        px_material_->release();
    }

    if (NULL != px_scene_) {
        px_scene_->release();
    }

	px_dispatcher_->release();
}

int PhysxScene::InitBase() {
	//场景描述符
	PxSceneDesc sceneDesc(PhysxCommonSigleton::get_mutable_instance().GetPhysics().getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.18f, 0.0f);
	px_dispatcher_ = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = px_dispatcher_;
	sceneDesc.filterShader  = PxDefaultSimulationFilterShader;

	//创建场景和默认材质
	px_scene_ = PhysxCommonSigleton::get_mutable_instance().GetPhysics().createScene(sceneDesc);
	px_material_ = PhysxCommonSigleton::get_mutable_instance().GetPhysics().createMaterial(0.5f, 0.5f, 0.6f);

	//初始化角色控制管理器
	// px_controller_mgr_ = PxCreateControllerManager(*px_scene_);

    return 0;
}

int PhysxScene::InitFromFile(const std::string& file_path) {
	std::ifstream input(file_path.c_str(), std::ios::in | std::ios::binary);
	if (!input) {
		LOG_ERROR(0, 0, "can't open file|%s", file_path.c_str());
		return -1;
	}

	U3DPhysxSceneConfig scene_config;
	if (!scene_config.ParseFromIstream(&input)) {
        LOG_ERROR(0, 0, "Parse U3DPhysxSceneConfig error");
		return -2;
	}

    LOG_INFO(0, 0, "SCENE_CONFIG|%s", scene_config.ShortDebugString().c_str());

	for (int i = 0; i < scene_config.box_collider_size(); ++i) {
		U3DPhysxBox b = scene_config.box_collider(i);
		AddBoxFromU3D(b);
	}

	for (int i = 0; i < scene_config.sphere_collider_size(); ++i) {
		U3DPhysxSphere s = scene_config.sphere_collider(i);
		AddSphereFromU3D(s);
	}

	for (int i = 0; i < scene_config.capsule_collider_size(); ++i) {
		U3DPhysxCapsule c = scene_config.capsule_collider(i);
		AddCapsuleFromU3D(c);
	}

	return 0;
}

void PhysxScene::AddBoxFromU3D(killer::proto::U3DPhysxBox& box) {
	if (box.x_extents() <= 0.0f || box.y_extents() <= 0.0f || box.z_extents() <= 0.0f) {
        LOG_ERROR(0, 0, "box error|%s", box.ShortDebugString().c_str());
		return;
	}

	PxShape *shape = PhysxCommonSigleton::get_mutable_instance().GetPhysics().createShape(PxBoxGeometry(box.x_extents() * 0.5f, box.y_extents() * 0.5f, box.z_extents() * 0.5), *px_material_);

	PxTransform localTrans(PxVec3(PxReal(box.pos().x()), PxReal(box.pos().y()), PxReal(box.pos().z())));
	PxRigidStatic *actor = PhysxCommonSigleton::get_mutable_instance().GetPhysics().createRigidStatic(localTrans);

	actor->attachShape(*shape);
	px_scene_->addActor(*actor);

	shape->release();
}

void PhysxScene::AddSphereFromU3D(killer::proto::U3DPhysxSphere& sphere) {
	if (sphere.radius() <= 0.0f) {
        LOG_ERROR(0, 0, "sphere error|%s", sphere.ShortDebugString().c_str());
		return;
	}

	PxShape *shape = PhysxCommonSigleton::get_mutable_instance().GetPhysics().createShape(PxSphereGeometry(sphere.radius()), *px_material_);

	PxTransform localTrans(PxVec3(PxReal(sphere.pos().x()), PxReal(sphere.pos().y()), PxReal(sphere.pos().z())));
	PxRigidStatic *actor = PhysxCommonSigleton::get_mutable_instance().GetPhysics().createRigidStatic(localTrans);

	actor->attachShape(*shape);
	px_scene_->addActor(*actor);

	shape->release();
}

void PhysxScene::AddCapsuleFromU3D(killer::proto::U3DPhysxCapsule& capsule) {
	if (capsule.raduis() <= 0.0f || capsule.height() <= 0.0f) {
        LOG_ERROR(0, 0, "capsule error|%s", capsule.ShortDebugString().c_str());
		return;
	}

	PxShape *shape = PhysxCommonSigleton::get_mutable_instance().GetPhysics().createShape(PxCapsuleGeometry(capsule.raduis(), capsule.height() * 0.5), *px_material_);
	PxTransform localTrans(PxVec3(PxReal(capsule.pos().x()), PxReal(capsule.pos().y()), PxReal(capsule.pos().z())));
	PxRigidStatic *actor = PhysxCommonSigleton::get_mutable_instance().GetPhysics().createRigidStatic(localTrans);

	actor->attachShape(*shape);
	px_scene_->addActor(*actor);

	shape->release();
}

bool PhysxScene::CheckReachable(physx::PxVec3 src, physx::PxVec3 dst) {
    PxVec3 dir = dst - src;
    PxReal max_distance = sqrt(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);
    PxRaycastBuffer hit;
    bool is_hit = px_scene_->raycast(src, dir.getNormalized(), max_distance, hit);

    return !is_hit;
}



