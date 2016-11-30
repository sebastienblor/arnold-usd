//*****************************************************************************
// Copyright 2015 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//*****************************************************************************

// Code written by Cave (www.cavevfx.com) for Autodesk in 2016
// Written by Erdem Taylan

#ifndef _AITYPES_H_
#define _AITYPES_H_

#include <map>
#include <vector>

#include <ai.h>

#include <bifrostrendercore/CoreTypes.h>
#include <bifrostrendercore/CoreObjectUserData.h>

struct AIProcNodeData {
	AtNode * proceduralNode;
	AtNode * camNode;
	std::vector<struct AtNode *> createdNodes;
	void *inData;
	AtShaderGlobals *bifrostCtx;
	std::map<AtByte, Bifrost::API::VoxelSampler *> samplerPool;
	AtMatrix world2Obj;
	AtMatrix obj2World;
	AtMatrix world2Cam;
	AtMatrix obj2Cam;

	int nofNodesCreated;
	void *frameData;

    Bifrost::RenderCore::CoreObjectUserData *objectRef;
	std::string objectName;
	std::string file;
};

#endif
