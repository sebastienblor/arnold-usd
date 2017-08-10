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

#ifndef _COREDEFS_H_
#define _COREDEFS_H_


#ifdef __APPLE__
	#define BIFROST_APPLE
#elif __linux__
	#define BIFROST_LINUX
#elif _WIN64 || _WIN32 || _Windows
	#define BIFROST_WINDOWS
#endif

# if defined(BIFROST_LINUX)
	#define __STDC_FORMAT_MACROS
#endif

#if defined(BIFROST_LINUX)
	#include <stdint.h>
//	#include <inttypes.h>
#elif defined(BIFROST_WINDOWS)
	#include <stdint.h>
//	#include "msinttypes.h"
#endif

#define IFNOTSILENT if ( inData->diagnostics.silent == 0 ) 
#define IFNOTSILENTCLASS if ( c_inData->diagnostics.silent == 0 ) 
#define IFNOTSILENTINFUNCTION if ( diagnostics.silent == 0 )

#define NOPROC false
#define PROC true

#define LOG( a ) std::cerr << a << std::endl

#define MAYA_EPS 1.0E-05f

#endif // _BifrostDefs_h_
