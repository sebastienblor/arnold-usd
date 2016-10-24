#include "utils/Version.h"
#include "ArnoldUpdateTxCmd.h"
#include "scene/MayaScene.h"
#include "utils/MakeTx.h"

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>
#include <ai_bbox.h>

#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <maya/MRenderUtil.h>
#include <maya/MFileIO.h>
#include <maya/MFileObject.h>
#include <maya/MFnRenderLayer.h>
#include <maya/MAnimControl.h>
#include <maya/MBoundingBox.h>

#include <math.h>

MSyntax CArnoldUpdateTxCmd::newSyntax()
{
   MSyntax syntax;
   // Do we want to add any argument here ?
   // maybe a selection ?
   return syntax;
}


MStatus CArnoldUpdateTxCmd::doIt(const MArgList& argList)
{
   MStatus status;
   //MArgDatabase args(syntax(), argList);

   updateAllTx();
   return MS::kSuccess;
}
