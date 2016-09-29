#pragma once
#include <ctype.h>
#include <iostream>
#include <fstream>

#include "PxPhysicsAPI.h"
#include "characterkinematic/PxControllerManager.h"
#include "characterkinematic/PxControllerBehavior.h"
#include "../ProtoGeneratedScrips/Scene.pb.h"

using namespace newborn::pb;
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

static const float kScaleFactor      = 1.0f;
static const float kStandingSize     = 20.00f * kScaleFactor;
static const float kCrouchingSize    = 5.0f   * kScaleFactor;
static const float kControllerRadius = 3.0f   * kScaleFactor;

class PhysicsSceneManager
{
public:
	PhysicsSceneManager();
	~PhysicsSceneManager();

public:
	void ResetData();
	bool InitPhysics();
	void CleanPhysics();

	void SetupPvdDebug();
	void StepPhysics(bool interactive);
	void KeyPress(const char key, const PxTransform& camera);

	bool InitSceneFromFile(); //load a scene from a file exported from Unity3D
	void InitSceneTest();     //for test only

	bool CanStandup();
	void ResizeController(PxController* player);
	void MoveController();

private:
	PxRigidDynamic* CreateDynamicBall(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity);
	void AddGround();
	void AddStack(const PxTransform& t, PxU32 size, PxReal halfExtent);
	void AddPlayer();

	void AddBoxFromU3D(U3DPhysxBox& box);
	void AddSphereFromU3D(U3DPhysxSphere& sphere);
	void AddCapsuleFromU3D(U3DPhysxCapsule& capsule);
	void AddMeshFromU3D(U3DPhysxMesh& mesh);

private:
	PxFoundation*                m_foundation;
	PxPhysics*                   m_physics;
	PxVisualDebuggerConnection * m_pvdCon;
	PxCooking*                   m_cooking;
	PxScene*                     m_scene;
	PxMaterial*                  m_material;
	PxControllerManager*         m_controller_mgr;
	PxController*                m_player;
	PxDefaultAllocator           m_allocator;
	PxDefaultErrorCallback	     m_error_callback;
	PxDefaultCpuDispatcher*      m_dispatcher;
	//the z-axis position of the stack of the boxes
	int                          m_stack_z;

	//parameters for capsule character controller
	float                        m_scale_factor;
	PxReal					     m_standing_size;
	PxReal					     m_crouching_size;
	float                        m_raduis;
	bool					     m_do_standup;
	bool					     m_is_crouching;
};

