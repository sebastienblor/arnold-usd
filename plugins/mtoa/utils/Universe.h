#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include "MtoaLog.h"

#include "ai.h"

DLLEXPORT void SetMetafile(MString metafile);

DLLEXPORT MStatus ReadMetafile();

// return if the universe had to be initialized (and thus should be uninitialized), false if it was already active
DLLEXPORT MStatus InitArnoldUniverse();

#endif // UNIVERSE_H_
