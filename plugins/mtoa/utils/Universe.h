#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include "MtoaLog.h"

#include <maya/MString.h>
#include <maya/MStatus.h>

DLLEXPORT void SetMetafile(MString metafile);

DLLEXPORT MStatus ReadMetafile();

// return if the universe had to be initialized (and thus should be uninitialized), false if it was already active
DLLEXPORT MStatus ArnoldUniverseBegin();
DLLEXPORT MStatus ArnoldUniverseEnd();

#endif // UNIVERSE_H_
