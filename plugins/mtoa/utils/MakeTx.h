#pragma once

#include "platform/Platform.h"
#include <maya/MString.h>

/// Generate a TX texture with maketx
DLLEXPORT void makeTx(MString filename, MString colorspace = "auto");

DLLEXPORT MStringArray expandTokens(MString filename);

/// Return the corresponding .tx texture if it exists, or filename if not
DLLEXPORT MString txTexture(MString filename);
