#pragma once
#include <ctype.h>
#include <iostream>
#include <fstream>

#include "PxPhysicsAPI.h"
#include "characterkinematic/PxControllerManager.h"
#include "characterkinematic/PxControllerBehavior.h"
#include "../ProtoGeneratedScrips/Scene.pb.h"

using namespace killer::proto;
using namespace physx;

#define CONTACT_OFFSET			    0.01f
//	#define CONTACT_OFFSET			0.1f
//	#define STEP_OFFSET				0.01f
#define STEP_OFFSET				    0.05f
//	#define STEP_OFFSET				0.1f
//	#define STEP_OFFSET				0.2f

//	#define SLOPE_LIMIT				0.8f
#define SLOPE_LIMIT				    0.0f
//	#define INVISIBLE_WALLS_HEIGHT	6.0f
#define INVISIBLE_WALLS_HEIGHT	    0.0f
//	#define MAX_JUMP_HEIGHT			4.0f
#define MAX_JUMP_HEIGHT			    0.0f

static const float gScaleFactor      = 1.0f;
static const float gStandingSize     = 20.00f * gScaleFactor;
static const float gCrouchingSize    = 5.0f * gScaleFactor;
static const float gControllerRadius = 3.0f * gScaleFactor;

class PhysicsSceneManager
{
public:
	PhysicsSceneManager();
	~PhysicsSceneManager();

public:
	void ResetData();
	void StepPhysics(bool interactive);
	bool InitPhysics();
	void CleanPhysics();
	void KeyPress(const char key, const PxTransform& camera);

	bool InitSceneFromFile(); //从客户端导出的文件中读取配置，生成物理场景
	void InitSceneTest();     //仅在框架搭建阶段， 作测试用

	bool CanStandup();
	void ResizeController(PxController* player);
	void MoveController();

private:
	void AddGround();
	void AddStack(const PxTransform& t, PxU32 size, PxReal halfExtent);
	void AddPlayer();
	PxRigidDynamic* CreateDynamicBall(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity);

	void AddBoxFromU3D(U3DPhysxBox& box);
	void AddSphereFromU3D(U3DPhysxSphere& sphere);
	void AddCapsuleFromU3D(U3DPhysxCapsule& capsule);
	void AddMeshFromU3D(U3DPhysxMesh& mesh);

private:
	PxFoundation*            m_foundation;
	PxPhysics*               m_physics;
	PxCooking*               m_cooking;
	PxScene*                 m_scene;
	PxMaterial*              m_material;
	PxControllerManager*     m_controller_mgr;
	PxController*            m_player;
	PxDefaultAllocator       m_allocator;
	PxDefaultErrorCallback	 m_error_callback;
	PxDefaultCpuDispatcher*  m_dispatcher;
	//box的位置
	int                      m_stack_z;
	//capsule参数
	float                    m_scale_factor;
	PxReal					 m_standing_size;
	PxReal					 m_crouching_size;
	float                    m_raduis;
	bool					 m_do_standup;
	bool					 m_is_crouching;
};

