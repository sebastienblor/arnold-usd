//*****************************************************************************
// Copyright 2016 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//*****************************************************************************

// Code written by Cave (www.cavevfx.com) for Autodesk in 2015
// Written by Erdem Taylan

#pragma once

#include <algorithm>

#include "bifrostrender_decl.h"
#include "bifrostrender_types.h"

namespace Bifrost{
namespace RenderCore{

template <class T>
T clip ( T n, T lower, T upper) {
	return std::max(lower, std::min(n, upper));
}

template <class T>
T mix ( T n1, T n2, T blend) {
	return n1 * (1.0f - blend) + n2 * blend;
}

// The following two macros allow rand() to be used instead of drand48().
// This was done because Microsoft Visual C++ doesn't have drand48(). The rand() function
// isn't quite as random as drand48(), but it is good enough for this.

#if defined(WIN32)
	#define drand48() ( ( (double) rand() ) / ( (double) RAND_MAX + 1.0) )
	#define srand48(seed)	srand( (long) seed)
#endif

float BIFROSTRENDERAPI_DECL smoothstep(float startVal, float endVal, float evalAt);

float BIFROSTRENDERAPI_DECL smootherstep(float startVal, float endVal, float evalAt);

float BIFROSTRENDERAPI_DECL calcBlendVal ( FalloffType falloffType, float startAt, float endAt, float evalAt);

float BIFROSTRENDERAPI_DECL geoff( float r2 );

float BIFROSTRENDERAPI_DECL dgeoff(float r2);

void BIFROSTRENDERAPI_DECL isq(CvInterval sq, CvInterval x);

void BIFROSTRENDERAPI_DECL imax(CvInterval max, CvInterval a, CvInterval b);

void BIFROSTRENDERAPI_DECL igeoff(CvInterval g, CvInterval r2);

}}

