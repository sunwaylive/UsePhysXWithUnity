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

#include "PxTkMatrixUtils.h"

using namespace PxToolkit;

void PxToolkit::setRotX(PxMat33& m, PxReal angle)
{
	m = PxMat33(PxIdentity);

	const PxReal cos = cosf(angle);
	const PxReal sin = sinf(angle);

	m[1][1] = m[2][2] = cos;
	m[1][2] = sin;
	m[2][1] = -sin;
}

void PxToolkit::setRotY(PxMat33& m, PxReal angle)
{
	m = PxMat33(PxIdentity);

	const PxReal cos = cosf(angle);
	const PxReal sin = sinf(angle);

	m[0][0] = m[2][2] = cos;
	m[0][2] = -sin;
	m[2][0] = sin;
}

void PxToolkit::setRotZ(PxMat33& m, PxReal angle)
{
	m = PxMat33(PxIdentity);

	const PxReal cos = cosf(angle);
	const PxReal sin = sinf(angle);

	m[0][0] = m[1][1] = cos;
	m[0][1] = sin;
	m[1][0] = -sin;
}

PxQuat PxToolkit::getRotXQuat(float angle)
{
	PxMat33 m;
	setRotX(m, angle);
	return PxQuat(m);
}

PxQuat PxToolkit::getRotYQuat(float angle)
{
	PxMat33 m;
	setRotY(m, angle);
	return PxQuat(m);
}

PxQuat PxToolkit::getRotZQuat(float angle)
{
	PxMat33 m;
	setRotZ(m, angle);
	return PxQuat(m);
}
