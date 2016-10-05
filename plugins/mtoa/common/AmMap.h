#pragma once

#include "platform/Platform.h"
#include <ai.h>

/**
 *  This file is meant to centralize types
 *  for Maps and Sets on the different Os
 **/
#ifdef _LINUX
#define UNORDERED_NEEDS_TR1 1
#endif

#ifdef _DARWIN
#if MAYA_API_VERSION < 201600
#define UNORDERED_NEEDS_TR1 1
#endif
#endif

#ifdef UNORDERED_NEEDS_TR1
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#define AmMap std::tr1::unordered_map
#define AmSet std::tr1::unordered_set 

#else
#include <unordered_map>
#include <unordered_set>
#define AmMap std::unordered_map
#define AmSet std::unordered_set

#endif
