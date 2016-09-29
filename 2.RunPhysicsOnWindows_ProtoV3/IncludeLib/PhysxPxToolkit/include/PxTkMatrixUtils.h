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

#ifndef PX_TOOLKIT_MATRIX_UTILS_H
#define PX_TOOLKIT_MATRIX_UTILS_H

#include "common/PxPhysXCommonConfig.h"
#include "foundation/PxMat33.h"

namespace PxToolkit
{
	using namespace physx;
	void	setRotX(PxMat33& m, PxReal angle);
	void	setRotY(PxMat33& m, PxReal angle);
	void	setRotZ(PxMat33& m, PxReal angle);

	PxQuat	getRotXQuat(float angle);
	PxQuat	getRotYQuat(float angle);
	PxQuat	getRotZQuat(float angle);
}

#endif
