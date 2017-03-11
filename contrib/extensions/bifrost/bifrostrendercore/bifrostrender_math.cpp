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

#include <bifrostrendercore/bifrostrender_math.h>
#include <bifrostrendercore/bifrostrender_types.h>

namespace Bifrost{
namespace RenderCore{

float smoothStep(float startAt, float endAt, float evalAt)
{
    // Scale, bias and saturate x to 0..1 range
	evalAt = clip<float>( (evalAt - startAt) / (endAt - startAt), 0.0f, 1.0f); 
    // Evaluate polynomial
    return evalAt * evalAt * (3.0f - 2.0f * evalAt);
}

float smootherStep(float startAt, float endAt, float evalAt)
{
    // Scale, and clamp x to 0..1 range
    evalAt = clip<float>( (evalAt - startAt) / (endAt - startAt), 0.0f, 1.0f);
    // Evaluate polynomial
	return evalAt * evalAt * evalAt * ( evalAt * ( evalAt * 6.0f - 15.0f) + 10.0f);
}

float calcBlendVal ( FalloffType falloffType, float startAt, float endAt, float evalAt)
{
	switch (falloffType) {
		case kLinear:
			return std::max(0.0f, evalAt - startAt) / (endAt - startAt);
			break;
		case kSmoothStep:
			return smoothStep( startAt, endAt, evalAt);
			break;
		case kSmootherStep:
			return smootherStep( startAt, endAt, evalAt);
			break;
		default:
			return 0.0f;
			break;
	}
}

// These are for calculating implicit field falloff
// for an implicit cube centered around origin with size in x = y = z = 1

float geoff( float r2 )
{
    if ( r2 >= 1.0f ) {
		return 0.f;
	}

	return ( ( 3.0f - r2 ) * r2 - 3.0f ) * r2 + 1.0f;
}

float dgeoff(float r2)
{
    if ( r2 >= 1.0f ) {
		return 0.0f;
	}

    return ( 6.0f - 3.0f * r2 ) * r2 - 3.0f;
}

void isq(CvInterval sq, CvInterval x)
{
	if ( x[0] >= 0 ){
		sq[0] = x[0] * x[0];
		sq[1] = x[1] * x[1];
	} else if( x[1] <= 0 ) {
		sq[0] = x[1] * x[1];
		sq[1] = x[0] * x[0];
	} else {
		sq[0] = 0;
		sq[1] = -x[0] > x[1] ? x[0] * x[0] : x[1] * x[1];
	}
}

void imax(CvInterval max, CvInterval a, CvInterval b)
{
    max[0] = b[0] > a[0] ? b[0] : a[0];
    max[1] = b[1] > a[1] ? b[1] : a[1];
}

void igeoff(CvInterval g, CvInterval r2)
{
    g[0] = geoff( r2[1] );
    g[1] = geoff( r2[0] );
}

}}
