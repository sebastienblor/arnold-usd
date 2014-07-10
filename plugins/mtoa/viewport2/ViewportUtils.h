#pragma once

#include <maya/MString.h>
#include <maya/MStringArray.h>

void SetFragmentSearchPath(const MString& path);
bool LoadFragmentGraph(const MString& fragmentGraph, const MStringArray& requirements);
bool LoadShadeFragment(const MString& shadeFragment);
bool InitializeGLEW();