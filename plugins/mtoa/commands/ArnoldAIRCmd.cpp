#include "ArnoldAIRCmd.h"

#include "scene/MayaScene.h"
#include "render/MaterialView.h"

#include <maya/MArgDatabase.h>
#include <maya/MGlobal.h>
#include <maya/MRenderUtil.h>

namespace {
    const char* cameraFlag = "camera";
    const char* cameraFlagShort = "cam";

    const char* widthFlag = "width";
    const char* widthFlagShort = "w";

    const char* heightFlag = "height";
    const char* heightFlagShort = "h";

    const char* modeFlag = "mode";
    const char* modeFlagShort = "m";
}

void* CArnoldAIRCmd::creator()
{
    return new CArnoldAIRCmd();
}

MSyntax CArnoldAIRCmd::newSyntax()
{
    MSyntax syntax;

    syntax.addFlag(cameraFlagShort, cameraFlag, MSyntax::kSelectionItem);
    syntax.addFlag(widthFlagShort, widthFlag, MSyntax::kUnsigned);
    syntax.addFlag(heightFlagShort, heightFlag, MSyntax::kUnsigned);
    syntax.addFlag(modeFlagShort, modeFlag, MSyntax::kString);

    return syntax;
}

MStatus CArnoldAIRCmd::doIt(const MArgList& argList)
{
    MStatus status;
    MArgDatabase args(syntax(), argList, &status);

    if (!status)
    {
        status.perror("[AIR] Error parsing the arg list.");
        return MS::kFailure;
    }

    if (!args.isFlagSet(modeFlag)) // TODO: check for a running AIR session, like in the IPR command
        return MS::kSuccess;

    const MString mode = args.flagArgumentString(modeFlag, 0);

    // creating the universe
    // doing the initial translation
    // adding the callbacks for tracing the updates
    // starting the render
    if (mode == "start")
    {
        if (!args.isFlagSet(cameraFlag))
        {
            MGlobal::displayError("[AIR] No camera argument was passed to the air command.");
            return MS::kFailure;
        }

        // Make sure no material view session is active
        CMaterialView::SuspendRenderer();

        CMayaScene::End();

        MCommonRenderSettingsData renderGlobals;
        MRenderUtil::getCommonRenderSettings(renderGlobals);

        CMayaScene::ExecuteScript(renderGlobals.preMel);
        CMayaScene::ExecuteScript(renderGlobals.preRenderMel);
    }
    // updating the universe
    // with the changes stored since either
    // start or the last update
    else if (mode == "update")
    {
        if (!CMayaScene::IsActive(MTOA_SESSION_AIR))
        {
            MGlobal::displayError("[AIR] There is no active AIR session.");
            return MS::kFailure;
        }

    }
    // stopping rendering
    // clearing update queue
    else if (mode == "stop")
    {
        if (!CMayaScene::IsActive(MTOA_SESSION_AIR))
        {
            MGlobal::displayError("[AIR] There is no active AIR session.");
            return MS::kFailure;
        }

        MCommonRenderSettingsData renderGlobals;
        MRenderUtil::getCommonRenderSettings(renderGlobals);

        CMayaScene::End();

        CMayaScene::ExecuteScript(renderGlobals.postRenderMel);
        CMayaScene::ExecuteScript(renderGlobals.postMel);

      // Resume material view session
      CMaterialView::ResumeRenderer();
    }
    else
    {
        MGlobal::displayError("[AIR] Invalid mode flag.");
        return MS::kFailure;
    }

    return MS::kSuccess;
}
