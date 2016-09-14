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

#ifndef PX_TOOLKIT_FPS_H
#define PX_TOOLKIT_FPS_H

#include "common/PxPhysXCommonConfig.h"
#include "PsTime.h"

namespace PxToolkit
{
	using namespace physx;

	class FPS
	{
	public:
		FPS();
		~FPS();

						void		update();
		PX_FORCE_INLINE	float		getFPS()		const	{ return mFPS;			}
		PX_FORCE_INLINE	float		getInstantFPS()	const	{ return mInstantFPS;	}

	private:
		shdfnd::Time	mTimer;
		shdfnd::Time	mTimer2;
		PxReal			mFPS;
		PxReal			mInstantFPS;
		PxU32			mFrames;
		PxU32			mFrames2;
	};

}

#endif
