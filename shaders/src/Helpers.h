
#ifndef HELPERS_H
#define HELPERS_H

#include <ai.h>

bool setRefPts(AtShaderGlobals *sg, AtPoint &tmpPts);
bool setRefNmrs(AtShaderGlobals *sg, AtVector &tmpNmrs);

void restorePts(AtShaderGlobals *sg, AtPoint tmpPts);
void restoreNmrs(AtShaderGlobals *sg, AtVector tmpNmrs);

#endif
