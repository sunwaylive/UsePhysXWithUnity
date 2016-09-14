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

#ifndef PX_TOOLKIT_BMP_LOADER_H
#define PX_TOOLKIT_BMP_LOADER_H

	#include "extensions/PxDefaultStreams.h"

	namespace PxToolkit
	{

	class BmpLoader
	{
	public:

		BmpLoader(void);
		~BmpLoader(void);

		// returns true if success. Data is persists until the class is destructed.
		bool loadBmp(const char* filename); 
		bool loadBmp(PxFileHandle f);

		int				mWidth;
		int				mHeight;
		unsigned char*	mRGB;
		bool			mHasAlpha;
	};

	}

#endif
