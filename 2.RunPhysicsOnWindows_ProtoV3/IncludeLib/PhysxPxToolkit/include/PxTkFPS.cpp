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

#include "PxTkFPS.h"

using namespace PxToolkit;

FPS::FPS()
{
	mFrames		= 0;
	mFrames2	= 0;
	mFPS		= 0.0f;
	mInstantFPS	= 0.0f;
}

FPS::~FPS()
{
}

void FPS::update()
{
	// Keep track of the time lapse and frame count
	mFrames++;
	mFrames2++;

	// Instant frame rate
	PxReal Delta = PxReal(mTimer2.peekElapsedSeconds());
	if(Delta > 0.01f)
	{
		mInstantFPS	= PxReal(mFrames2) / Delta;
		mTimer2.getElapsedSeconds();
		mFrames2	= 0;
	}

	// Update the frame rate once per second
	Delta = PxReal(mTimer.peekElapsedSeconds());
	if(Delta > 1.0f)
	{
		mFPS		= PxReal(mFrames) / Delta;
		mTimer.getElapsedSeconds();
		mFrames		= 0;
	}
}
