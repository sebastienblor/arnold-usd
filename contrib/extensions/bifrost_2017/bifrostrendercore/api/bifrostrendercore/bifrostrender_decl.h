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

#ifndef ___COREDECL_H___
#define ___COREDECL_H___

	#ifdef __APPLE__
		#define BIFROSTRENDER_APPLE
	#elif __linux__
		#define BIFROSTRENDER_LINUX
	#elif _WIN64 || _WIN32 || _Windows
		#define BIFROSTRENDER_WINDOWS
	#endif

#if defined(BIFROSTRENDER_STANDALONE)

		#define BIFROSTRENDERAPI_DECL
	#elif defined(BIFROSTRENDER_WINDOWS)

		#ifdef BIFROSTRENDERAPI
			#ifdef __GNUC__
				#define BIFROSTRENDERAPI_DECL __attribute__((dllexport))
			#else
				#define BIFROSTRENDERAPI_DECL __declspec(dllexport)
			#endif
		#else
			#ifdef __GNUC__
				#define BIFROSTRENDERAPI_DECL __attribute__((dllimport))
			#else
				#define BIFROSTRENDERAPI_DECL __declspec(dllimport)
			#endif
		#endif
	#else
		#if __GNUC__ >= 4
			#define BIFROSTRENDERAPI_DECL __attribute__((visibility ("default")))
		#else
			#define BIFROSTRENDERAPI_DECL
		#endif
	#endif


#endif
