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

#ifndef _AIPRIMITIVESTOOLS_H_
#define _AIPRIMITIVESTOOLS_H_

#include <cstring>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>

#include <ai.h>

#include <maya/MString.h>

#include <bifrostrendercore/bifrostrender_types.h>
#include <bifrostrendercore/bifrostrender_tools.h>
#include <bifrostrendercore/bifrostrender_primitivestools.h>

#include "Types.h"

void exportPrimitivesAI	(	Bifrost::RenderCore::PrimitivesInputData *inData,
                            Bifrost::RenderCore::PrimitivesFrameData *frameData,
                            int totalExported,
                            std::vector<Bifrost::RenderCore::CvToken>& toks,
                            std::vector<Bifrost::RenderCore::CvPointer>& ptr,
                            std::vector<int>& varIndex,
                            Bifrost::RenderCore::CvFloat *cPos,
                            Bifrost::RenderCore::CvFloat *cWidth,
                            Bifrost::RenderCore::CvFloat *cTra,
                            Bifrost::RenderCore::CvFloat *cNormal,
                            Bifrost::RenderCore::CvInt *cVerts,
                            Bifrost::RenderCore::CvInt *cCode,
                            void *usrData
                            );

#endif
