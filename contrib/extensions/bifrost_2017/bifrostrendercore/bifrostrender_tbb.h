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

#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

#include <bifrostrendercore/bifrostrender_meshing.h>

namespace Bifrost{
namespace RenderCore{

class MeshSampler {
	private:
        MarchingCubes *c_mesher;
        float c_dx;

	public:
        MeshSampler( MarchingCubes *mesher, float dx )
            : c_mesher( mesher ), c_dx( dx ) {}

        void operator() ( const tbb::blocked_range<int>& r ) const;
};

}}
