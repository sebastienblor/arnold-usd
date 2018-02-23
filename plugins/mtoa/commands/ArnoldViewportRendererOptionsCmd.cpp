#include "ArnoldViewportRendererOptionsCmd.h"
#include "scene/MayaScene.h"

#include <maya/MArgList.h>

void* CArnoldViewportRendererOptionsCmd::creator()
{
   return new CArnoldViewportRendererOptionsCmd;
}

MStatus CArnoldViewportRendererOptionsCmd::doIt(const MArgList& /*args*/)
{
    if (CMayaScene::IsActive(MTOA_SESSION_RENDERVIEW))
    {
        CMayaScene::GetRenderSession()->OpenInteractiveRendererOptions();
        return MS::kSuccess;
    }
   return MS::kSuccess;
}

