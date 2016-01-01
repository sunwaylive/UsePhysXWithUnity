#include "PhysicsSceneManager.h"

PhysicsSceneManager::PhysicsSceneManager()
{
	ResetData();
}

void PhysicsSceneManager::ResetData()
{
	m_scale_factor   = 15.0f;
	m_crouching_size = 0.25f * m_scale_factor;
	m_standing_size  = 1.0f * m_scale_factor;
	m_raduis         = 0.3f * m_scale_factor;
	m_stack_z        = 10.f;
	m_is_crouching   = false;
}

PhysicsSceneManager::~PhysicsSceneManager()
{
}

bool PhysicsSceneManager::InitPhysics()
{
	//foundation 和 physics在很多地方有用到， 要在一开始创建出来
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_error_callback);
	if (m_foundation == NULL)
	{
		std::cerr << "PxCreateFoundation failed!" << std::endl;
		exit(0);
	}

	PxProfileZoneManager *profileZoneMgr = &PxProfileZoneManager::createProfileZoneManager(m_foundation);
	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale(), true, profileZoneMgr);
	if (m_physics == NULL)
	{
		std::cerr << "PxCreatePhysics failed!" << std::endl;
		exit(0);
	}

	m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_foundation, PxCookingParams(PxTolerancesScale()));
	if (!m_cooking)
	{
		std::cerr << "PxCreateCooking failed!" << std::endl;
		exit(0);
	}

	//场景描述符
	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity       = PxVec3(0.0f, -9.18f, 0.0f);
	m_dispatcher            = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_dispatcher;
	sceneDesc.filterShader  = PxDefaultSimulationFilterShader;

	//创建场景和默认材质
	m_scene = m_physics->createScene(sceneDesc);
	m_material = m_physics->createMaterial(0.5f, 0.5f, 0.6f);

	//初始化角色控制管理器
	m_controller_mgr = PxCreateControllerManager(*m_scene);
	//When activated, the CCT module will automatically try to resolve the penetration, and move the CCT to a safe place where it does not overlap other objects anymore
	//因为从客户端美术做的场景中collider是有重叠的， 所以要关掉这个特性
	//m_controller_mgr->setOverlapRecoveryModule(true);
	//m_controller_mgr->setPreciseSweeps(false);

	//TODO:在这里创建场景
	//InitSceneTest();
	//从客户端导出的文件中初始化场景
	InitSceneFromFile();

	return true;
}

bool PhysicsSceneManager::InitSceneFromFile()
{
	//std::string file_path = "../scenes/battle_scene_pvp_03.scene";
	std::string file_path = "../Scenes/test.scene";
	std::ifstream input(file_path.c_str(), std::ios::in | std::ios::binary);
	if (!input)
	{
		std::cout << "can't open file " << std::endl;
		exit(0);
		return false;
	}

	//read file from client
	U3DPhysxScene cur_scene;
	if (!cur_scene.ParseFromIstream(&input))
	{
		std::cerr << "Can't parse scene collider data from file" << std::endl;
		return false;
	}

	std::cout << "Box Collider: " << cur_scene.box_collider_size() << std::endl;
	std::cout << "Sphere Collider: " << cur_scene.sphere_collider_size() << std::endl;
	std::cout << "Capsule Collider: " << cur_scene.capsule_collider_size() << std::endl;
	std::cout << "Mesh  Collider: " << cur_scene.mesh_collider_size() << std::endl;

	for (int i = 0; i < cur_scene.box_collider_size(); ++i)
	{
		U3DPhysxBox b = cur_scene.box_collider(i);
		AddBoxFromU3D(b);
	}

	for (int i = 0; i < cur_scene.sphere_collider_size(); ++i)
	{
		U3DPhysxSphere s = cur_scene.sphere_collider(i);
		AddSphereFromU3D(s);
	}

	for (int i = 0; i < cur_scene.capsule_collider_size(); ++i)
	{
		U3DPhysxCapsule c = cur_scene.capsule_collider(i);
		AddCapsuleFromU3D(c);
	}

	for (int i = 0; i < cur_scene.mesh_collider_size(); ++i)
	{
		U3DPhysxMesh m = cur_scene.mesh_collider(i);
		AddMeshFromU3D(m);
	}

	return true;
}

void PhysicsSceneManager::InitSceneTest()
{
	AddGround();
	AddPlayer();

	//创建box
	for (PxU32 i = 0; i < 1; i++)
		AddStack(PxTransform(PxVec3(0, 0, m_stack_z -= 10.0f)), 10, 2.0f);
}

bool PhysicsSceneManager::CanStandup()
{
	//检测玩家是否能站起来
	bool isCanStandup = true;
	PxSceneReadLock scopedLock(*m_scene);
	PxCapsuleController* capsuleCtrl = static_cast<PxCapsuleController*>(m_player);

	PxReal r = capsuleCtrl->getRadius();
	PxReal dh = m_standing_size - m_crouching_size - 2.0f * r; //delta height
	PxCapsuleGeometry geomTest(r, dh * 0.5f);

	PxExtendedVec3 position = m_player->getPosition();
	PxVec3 pos((float)position.x, (float)position.y + m_standing_size * 0.5 + r, (float)position.z);
	PxQuat orientation(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f));

	PxOverlapBuffer hit;
	if (m_scene->overlap(geomTest, PxTransform(pos, orientation), hit, 
		PxQueryFilterData(PxQueryFlag::eANY_HIT | PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC)))
	{
		isCanStandup = false;
	}

	return isCanStandup;
}

void PhysicsSceneManager::ResizeController(PxController *player)
{
	m_is_crouching = !m_is_crouching;
	PxSceneWriteLock scopeLock(*m_scene);
	if (m_is_crouching)
		player->resize(m_crouching_size);      //下蹲
	else
	{
		if(CanStandup())
			player->resize(m_standing_size);  //站立
	}
}

void PhysicsSceneManager::MoveController()
{

}

void PhysicsSceneManager::AddPlayer()
{
	//创建胶囊体
	PxControllerDesc* cDesc; //下面这个的父类
	PxCapsuleControllerDesc capsuleDesc;
	capsuleDesc.height              = m_standing_size;
	capsuleDesc.radius              = m_raduis;
	//capsuleDesc.                  = gCrouchingSize; //下蹲的高度
	capsuleDesc.climbingMode        = PxCapsuleClimbingMode::eCONSTRAINED;

	cDesc = &capsuleDesc;

	cDesc->density				= 10.0f;//desc.mProxyDensity; 
	cDesc->scaleCoeff			= 0.9f; //desc.mProxyScale;
	cDesc->material				= m_material;
	cDesc->position				= PxExtendedVec3(0,0,100) + PxExtendedVec3(0, capsuleDesc.height * 0.5f + capsuleDesc.radius, 0); //desc.mPosition;
	cDesc->slopeLimit			= SLOPE_LIMIT;
	cDesc->contactOffset		= CONTACT_OFFSET;
	cDesc->stepOffset			= STEP_OFFSET;
	cDesc->invisibleWallHeight	= INVISIBLE_WALLS_HEIGHT;
	cDesc->maxJumpHeight		= MAX_JUMP_HEIGHT;
	//cDesc->nonWalkableMode	= PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	/*cDesc->reportCallback		= desc.mReportCallback;
	cDesc->behaviorCallback		= desc.mBehaviorCallback;
	cDesc->volumeGrowth			= desc.mVolumeGrowth;*/

	m_player = (m_controller_mgr->createController(*cDesc));
}

void PhysicsSceneManager::AddGround()
{
	//创建地平面
	PxRigidStatic* groundPlane = PxCreatePlane(*m_physics, PxPlane(0, 1, 0, 0), *m_material);
	m_scene->addActor(*groundPlane);
}

void PhysicsSceneManager::AddStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape *shape = m_physics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *m_material);
	//下面好多actor都用到这个shape了， 引用计数管理
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j<size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic *actor = m_physics->createRigidDynamic(t.transform(localTm));
			actor->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*actor, 10.f);

			m_scene->addActor(*actor);
		}
	}

	shape->release();
}

PxRigidDynamic* PhysicsSceneManager::CreateDynamicBall(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*m_physics, t, geometry, *m_material, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	m_scene->addActor(*dynamic);
	return dynamic;
}

void PhysicsSceneManager::AddBoxFromU3D(U3DPhysxBox& box)
{
	if (box.x_extents() <= 0.0f || box.y_extents() <= 0.0f || box.z_extents() <= 0.0f)
	{
		std::cerr << "No." << box.id() << " BOX has negative extends " 
			<< "x: " << box.x_extents() << " y: " << box.y_extents() << " z: " <<box.z_extents() << std::endl;
		return;
	}

	PxShape *shape = m_physics->createShape(PxBoxGeometry(box.x_extents() * 0.5f, box.y_extents() * 0.5f, box.z_extents() * 0.5), *m_material);
	
	PxTransform localTrans(PxVec3(PxReal(box.pos().x()), PxReal(box.pos().y()), PxReal(box.pos().z())));
	PxRigidStatic *actor = m_physics->createRigidStatic(localTrans);

	actor->attachShape(*shape);
	m_scene->addActor(*actor);

	shape->release();
}

void PhysicsSceneManager::AddSphereFromU3D(U3DPhysxSphere& sphere)
{
	if (sphere.radius() <= 0.0f)
	{
		std::cerr << "No." << sphere.id() <<" SPHERE has wrong raduis!" <<" radius: " <<sphere.radius()<< std::endl;
		return;
	}

	PxShape *shape = m_physics->createShape(PxSphereGeometry(sphere.radius()), *m_material);

	PxTransform localTrans(PxVec3(PxReal(sphere.pos().x()), PxReal(sphere.pos().y()), PxReal(sphere.pos().z())));
	PxRigidStatic *actor = m_physics->createRigidStatic(localTrans);

	actor->attachShape(*shape);
	m_scene->addActor(*actor);

	shape->release();
}

void PhysicsSceneManager::AddCapsuleFromU3D(U3DPhysxCapsule& capsule)
{
	if (capsule.raduis() <= 0.0f || capsule.height() <= 0.0f)
	{
		std::cerr << "No." << capsule.id() << " CAPSULE has negative radius or height" 
			<< "capsule raduis: " << capsule.raduis() << " capsule height: " <<capsule.height()	<< std::endl;
		return;
	}

	PxShape *shape = m_physics->createShape(PxCapsuleGeometry(capsule.raduis(), capsule.height() * 0.5), *m_material);
	PxTransform localTrans(PxVec3(PxReal(capsule.pos().x()), PxReal(capsule.pos().y()), PxReal(capsule.pos().z())));
	PxRigidStatic *actor = m_physics->createRigidStatic(localTrans);

	actor->attachShape(*shape);
	m_scene->addActor(*actor);

	shape->release();
}

void PhysicsSceneManager::AddMeshFromU3D(U3DPhysxMesh& mesh)
{
	if (mesh.vertex_count() <= 0)
	{
		std::cerr << "Wrong Mesh, No vertices in the mesh" << std::endl;
		return;
	}

	int vertex_cnt = mesh.vertex_count();
	//world pos
	std::vector<PxVec3> convexVerts;
	for (int i = 0; i < vertex_cnt; ++i)
	{
		PxVec3 new_vertex(mesh.vertices(i).x(),
						  mesh.vertices(i).y(),
						  mesh.vertices(i).z());

		convexVerts.push_back(new_vertex);
	}

	std::cout << "mesh vertices num: " << convexVerts.size() << std::endl;

	PxConvexMeshDesc convex_desc;
	convex_desc.points.count  = vertex_cnt;
	convex_desc.points.stride = sizeof(PxVec3);
	convex_desc.points.data   = &convexVerts[0];
	convex_desc.flags         = PxConvexFlag::eCOMPUTE_CONVEX | PxConvexFlag::eINFLATE_CONVEX;

	PxDefaultMemoryOutputStream buf;
	if (!m_cooking->cookConvexMesh(convex_desc, buf))
		return;

	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	PxConvexMesh *convex_mesh = m_physics->createConvexMesh(input);

	PxShape *convex_shape = m_physics->createShape(PxConvexMeshGeometry(convex_mesh), *m_material);
	PxRigidStatic *mesh_actor = m_physics->createRigidStatic(PxTransform::createIdentity());
	mesh_actor->attachShape(*convex_shape);

	m_scene->addActor(*mesh_actor);

	convex_shape->release();
}


void PhysicsSceneManager::StepPhysics(bool)
{
	m_scene->simulate(1 / 60.f); //获取1/60 s之后世界的状态
	m_scene->fetchResults(true); //必须成对使用
}

void PhysicsSceneManager::CleanPhysics()
{
	m_scene->release();
	m_dispatcher->release();
	PxProfileZoneManager* profileZoneManager = m_physics->getProfileZoneManager();

	m_physics->release();
	profileZoneManager->release();
	m_foundation->release();
	m_cooking->release();

	printf("Physics Server done.\n");
}

void PhysicsSceneManager::KeyPress(const char key, const PxTransform& camera)
{
	switch (toupper(key))
	{
	case 'B':	AddStack(PxTransform(PxVec3(10, 10, m_stack_z -= 10.0f)), 10, 2.0f);						break;
	case 'L':	CreateDynamicBall(camera, PxSphereGeometry(3.0f), camera.rotate(PxVec3(0, 0, -1)) * 200);	break;
	case 'C':   ResizeController(m_player);                                                                 break;
	case '1':   m_player->move(PxVec3(1.0f, 0.0f, 0.0f), 0.5f, 0.1, PxControllerFilters());                 break;
	case '2':   m_player->move(PxVec3(-1.0f, 0.0f, 0.0f), 0.5f, 0.1, PxControllerFilters());                break;

	default: break;
	}
}