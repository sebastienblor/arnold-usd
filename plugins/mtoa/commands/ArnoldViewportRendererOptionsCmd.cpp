#include "ArnoldViewportRendererOptionsCmd.h"
#include "scene/MayaScene.h"

#include <maya/MArgList.h>

void* CArnoldViewportRendererOptionsCmd::creator()
{
   return new CArnoldViewportRendererOptionsCmd;
}

MStatus CArnoldViewportRendererOptionsCmd::doIt(const MArgList& /*args*/)
{
    CMayaScene::GetRenderSession()->OpenInteractiveRendererOptions();
    return MS::kSuccess;
}

