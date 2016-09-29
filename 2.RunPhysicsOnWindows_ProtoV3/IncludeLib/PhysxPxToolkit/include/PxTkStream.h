/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#ifndef PX_TOOLKIT_STREAM_H
#define PX_TOOLKIT_STREAM_H

#include "extensions/PxDefaultStreams.h"

#include "cooking/PxCooking.h"
namespace physx
{
	class PxTriangleMesh;
	class PxConvexMesh;
	class PxPhysics;
	class PxCooking;
	class PxTriangleMeshDesc;
}

namespace PxToolkit
{
	using namespace physx;

	// temporarily located here
	physx::PxTriangleMesh* createTriangleMesh32(physx::PxPhysics& physics, physx::PxCooking& cooking, const PxVec3* verts, PxU32 vertCount, const PxU32* indices32, PxU32 triCount, bool insert = false);
	physx::PxConvexMesh* createConvexMesh(physx::PxPhysics& physics, physx::PxCooking& cooking, const PxVec3* verts, PxU32 vertCount, PxConvexFlags flags);
	physx::PxConvexMesh* createConvexMeshSafe(physx::PxPhysics& physics, physx::PxCooking& cooking, const PxVec3* verts, PxU32 vertCount, PxConvexFlags flags, PxU32 vLimit = 256);
	
	//this function is for MultiMaterial and MultiMaterialTerrain
	physx::PxTriangleMesh* createTriangleMesh32(physx::PxPhysics& physics, physx::PxCooking& cooking, physx::PxTriangleMeshDesc* meshDesc, bool insert = false);
}



#endif
