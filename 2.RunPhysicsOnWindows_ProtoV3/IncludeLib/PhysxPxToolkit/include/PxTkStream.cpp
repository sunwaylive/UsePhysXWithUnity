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

#include "PxTkStream.h"
#include "PxAssert.h"
#include "PsFile.h"
#include "PxTkNamespaceMangle.h"
#include "PsIntrinsics.h"
#include "PxMath.h"
#include "PxPhysics.h"
#include "geometry/PxConvexMesh.h"
#include "cooking/PxCooking.h"
#include "PxBounds3.h"
#include "extensions/PxDefaultStreams.h"

using namespace PxToolkit;
///////////////////////////////////////////////////////////////////////////////

PxTriangleMesh* PxToolkit::createTriangleMesh32(PxPhysics& physics, PxCooking& cooking, const PxVec3* verts, PxU32 vertCount, const PxU32* indices32, PxU32 triCount, bool insert)
{
	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count			= vertCount;
	meshDesc.points.stride			= sizeof(PxVec3);
	meshDesc.points.data			= verts;

	meshDesc.triangles.count		= triCount;
	meshDesc.triangles.stride		= 3*sizeof(PxU32);
	meshDesc.triangles.data			= indices32;

	if(!insert)
	{
		PxDefaultMemoryOutputStream writeBuffer;
		bool status = cooking.cookTriangleMesh(meshDesc, writeBuffer);
		if(!status)
			return NULL;

		PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		return physics.createTriangleMesh(readBuffer);
	}
	else
	{
		return cooking.createTriangleMesh(meshDesc,physics.getPhysicsInsertionCallback());
	}
}

PxTriangleMesh* PxToolkit::createTriangleMesh32(PxPhysics& physics, PxCooking& cooking, PxTriangleMeshDesc* meshDesc, bool insert)
{
	if(!insert)
	{
		PxDefaultMemoryOutputStream writeBuffer;
		bool status = cooking.cookTriangleMesh(*meshDesc, writeBuffer);
		if(!status)
			return NULL;

		PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		return physics.createTriangleMesh(readBuffer);
	}
	else
	{
		return cooking.createTriangleMesh(*meshDesc,physics.getPhysicsInsertionCallback());
	}
}

PxConvexMesh* PxToolkit::createConvexMesh(PxPhysics& physics, PxCooking& cooking, const PxVec3* verts, PxU32 vertCount, PxConvexFlags flags)
{
	PxConvexMeshDesc convexDesc;
	convexDesc.points.count		= vertCount;
	convexDesc.points.stride	= sizeof(PxVec3);
	convexDesc.points.data		= verts;
	convexDesc.flags			= flags;

	PxDefaultMemoryOutputStream buf;
	if(!cooking.cookConvexMesh(convexDesc, buf))
		return NULL;

	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	return physics.createConvexMesh(input);
}

PxConvexMesh* PxToolkit::createConvexMeshSafe(PxPhysics& physics, PxCooking& cooking, const PxVec3* verts, PxU32 vertCount, PxConvexFlags flags, PxU32 vLimit)
{
	PxConvexMeshDesc convexDesc;
	convexDesc.points.count			= vertCount;
	convexDesc.points.stride		= sizeof(PxVec3);
	convexDesc.points.data			= verts;
	convexDesc.flags				= flags;
	convexDesc.vertexLimit			= vLimit;

	bool aabbCreated = false;

	PxDefaultMemoryOutputStream buf;
	bool retVal = cooking.cookConvexMesh(convexDesc, buf);
	if(!retVal)
	{
		if(!(flags & PxConvexFlag::eINFLATE_CONVEX))
		{
			convexDesc.flags |= PxConvexFlag::eINFLATE_CONVEX;
			retVal = cooking.cookConvexMesh(convexDesc, buf);
		}		
	}
	
	if(!retVal)
	{
		// create AABB
		PxBounds3 aabb;
		aabb.setEmpty();
		for (PxU32 i = 0; i < vertCount; i++)
		{
			aabb.include(verts[i]);
		}

		PxVec3  aabbVerts[8];
		aabbVerts[0] = PxVec3(aabb.minimum.x,aabb.minimum.y,aabb.minimum.z);
		aabbVerts[1] = PxVec3(aabb.maximum.x,aabb.minimum.y,aabb.minimum.z);
		aabbVerts[2] = PxVec3(aabb.maximum.x,aabb.maximum.y,aabb.minimum.z);
		aabbVerts[3] = PxVec3(aabb.minimum.x,aabb.maximum.y,aabb.minimum.z);

		aabbVerts[4] = PxVec3(aabb.minimum.x,aabb.minimum.y,aabb.maximum.z);
		aabbVerts[5] = PxVec3(aabb.maximum.x,aabb.minimum.y,aabb.maximum.z);
		aabbVerts[6] = PxVec3(aabb.maximum.x,aabb.maximum.y,aabb.maximum.z);
		aabbVerts[7] = PxVec3(aabb.minimum.x,aabb.maximum.y,aabb.maximum.z);

		convexDesc.points.count			= 8;
		convexDesc.points.stride		= sizeof(PxVec3);
		convexDesc.points.data			= &aabbVerts[0];
		convexDesc.flags				= flags;

		retVal = cooking.cookConvexMesh(convexDesc, buf);

		aabbCreated = true;
	}

	if(!retVal)
	{
		return NULL;
	}

	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	PxConvexMesh* mesh = physics.createConvexMesh(input);

	// vLimit test
	if(mesh && !aabbCreated && (flags & PxConvexFlag::eINFLATE_CONVEX))
	{
		bool iterate = true;
		PxU32 limit = vLimit;
		while(iterate)
		{		
			if(mesh->getNbVertices() > vLimit)
			{
				if(limit - (mesh->getNbVertices() - limit) > 4)
				{
					convexDesc.vertexLimit = limit - (mesh->getNbVertices() - limit);
				}
				else
				{
					convexDesc.vertexLimit = 4;
				}

				limit = convexDesc.vertexLimit;

				mesh->release();
				mesh = NULL;

				PxDefaultMemoryOutputStream buf2;
				retVal = cooking.cookConvexMesh(convexDesc, buf2);

				if(retVal)
				{
					PxDefaultMemoryInputData input2(buf2.getData(), buf2.getSize());
					mesh = physics.createConvexMesh(input2);

					if(!mesh)
						return NULL;
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				return mesh;
			}
		}
	}

	return mesh;
}


