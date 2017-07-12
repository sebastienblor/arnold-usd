//*****************************************************************************
// Copyright 2016 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//*****************************************************************************

// Code written by Cave (www.cavevfx.com) for Autodesk in 2016
// Written by Erdem Taylan

#pragma once

#include "bifrostrender_decl.h"
#include "bifrostrender_visitors.h"

namespace Bifrost{
namespace RenderCore{

// check number of polys
bool BIFROSTRENDERAPI_DECL checkPolyCount( size_t vertexCount, size_t polyCount );

// allocation for mesher polys and data
void BIFROSTRENDERAPI_DECL allocatePolyData( size_t vertexCount, size_t polyCount, FrameData *frameData, CvInt **vertices, CvFloat **pos, CvFloat **normal, CvFloat **motionPos);

// prepare data for mesh export
void BIFROSTRENDERAPI_DECL preparePolyData(	size_t vertexCount,
											size_t polyCount,
											MarchingCubes *mesher,
											FrameData *frameData,
											ImplicitsInputData *inData,
											CvInt *vertices,
											CvFloat *pos,
											CvFloat *normal,
											CvFloat *motionPos );

}}

