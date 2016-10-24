#include "MakeTx.h"
#include <maya/MGlobal.h>

void makeTx(MString filename, MString colorspace, int *createdFiles, int *skippedFiles, int *errorFiles)
{
   MString cmd;
   MStringArray result;
   cmd.format("import mtoa.makeTx; mtoa.makeTx.makeTx('^1s', '^2s')", filename, colorspace);
   MGlobal::executePythonCommand(cmd, result);
   if (createdFiles) *createdFiles = result[0].asInt();
   if (skippedFiles) *skippedFiles = result[1].asInt();
   if (errorFiles) *errorFiles = result[2].asInt();
   
}

MStringArray expandFilename(MString filename)
{
   MStringArray result;
   MString cmd;
   cmd.format("import mtoa.makeTx; mtoa.makeTx.expandFilename('^1s')", filename);
   MGlobal::executePythonCommand(cmd, result);
   return result;
}

MString txTexure(MString filename)
{
   MString cmd;
   cmd.format("import mtoa.makeTx; mtoa.makeTx.txTexture('^1s')", filename);
   return MGlobal::executePythonCommandStringResult(cmd);
}

void updateAllTx()
{
   MString cmd = "import mtoa.txManager; mtoa.txManager.UpdateAllTx()";
   MGlobal::executePythonCommand(cmd);
}
