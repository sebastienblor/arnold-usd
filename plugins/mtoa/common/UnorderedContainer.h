#pragma once

#include "platform/Platform.h"
#include <maya/MTypes.h>
#include <ai.h>

/**
 *  This file is meant to centralize types
 *  for Maps and Sets on the different Os
 **/

/*
Try with c++11, even for linux/Maya2018 (see #4009)
#ifdef _LINUX
#if MAYA_API_VERSION < 201800
#define UNORDERED_NEEDS_TR1 1
#endif
#endif
*/
 

#ifdef UNORDERED_NEEDS_TR1
#include <tr1/unordered_map>
#include <tr1/unordered_set>
using std::tr1::unordered_map;
using std::tr1::unordered_set;
using std::tr1::hash;
#else
#include <unordered_map>
#include <unordered_set>
using std::unordered_map;
using std::unordered_set;
using std::hash;

#endif
