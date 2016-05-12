#include "MakeTx.h"
#include <maya/MGlobal.h>

void makeTx(MString filename, MString colorspace)
{
    MString cmd;
    cmd.format("import mtoa.makeTx; mtoa.makeTx.makeTx('^1s', '^2s')", filename, colorspace);
    MGlobal::executePythonCommand(cmd);
}

MString txTexure(MString filename)
{
    MString cmd;
    cmd.format("import mtoa.makeTx; mtoa.makeTx.txTexture('^1s')", filename);
    return MGlobal::executePythonCommandStringResult(cmd);
}
