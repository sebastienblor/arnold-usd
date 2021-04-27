#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4305)
#pragma warning(disable:4273)
#elif defined(_LINUX) || defined(_DARWIN)
#include <dlfcn.h>
#endif

#include "usdArnoldChaser.h"
#include "writer.h"

#include <mayaUsd/fileio/jobs/jobArgs.h>
#include <mayaUsd/fileio/shaderWriter.h>
#include <mayaUsd/fileio/shaderWriterRegistry.h>
#include <mayaUsd/fileio/utils/roundTripUtil.h>
#include <mayaUsd/fileio/utils/shadingUtil.h>
#include <mayaUsd/fileio/utils/writeUtil.h>
#include <mayaUsd/fileio/writeJobContext.h>
#include <mayaUsd/utils/converter.h>
#include <mayaUsd/utils/util.h>

#include <pxr/base/tf/diagnostic.h>
#include <pxr/base/tf/token.h>
#include <pxr/base/vt/value.h>
#include <pxr/pxr.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/sdf/valueTypeName.h>
#include <pxr/usd/usd/stage.h>

#include <mayaUsd/fileio/chaser/exportChaserRegistry.h>

#include <maya/MFnAttribute.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MGlobal.h>

#include <utility>
#include <ai.h>

PXR_NAMESPACE_OPEN_SCOPE

void AppendDagToNodesList(std::string &nodesList, const MDagPath &dag)
{
    if (!nodesList.empty())
        nodesList += ",";

    nodesList += std::string("'");
    nodesList += dag.partialPathName().asChar();
    nodesList += std::string("'");
}

ArnoldUsdChaser::ArnoldUsdChaser(
        UsdStagePtr                                                     stage,
        const UsdMayaExportChaserRegistry::FactoryContext::DagToUsdMap& dagToUsd
        )
{
    std::string nodesList;
    bool mergeTransformShape = true;
    bool checkNames = true;
    MString aiStandIn("aiStandIn");
    MString aiVolume("aiVolume");

    MString parentScope;
    for (const auto& p : dagToUsd) {
        const MDagPath& dag = p.first;
        const SdfPath&  usdPrimPath = p.second;
        
        if (dag.apiType() == MFn::kTransform)
        {
            MDagPath shapeDag = dag;
            shapeDag.extendToShape();
            MString typeName = MFnDependencyNode(shapeDag.node()).typeName();
            if (typeName == aiStandIn || typeName == aiVolume)
                AppendDagToNodesList(nodesList, shapeDag);
            
            continue; // no need to consider transform nodes
        }

        if (checkNames)
        {
            // store 2 names : first the shape and then its transform.
            // We'll check which one matches the prim names
            MStringArray dagNames;
            dagNames.append(dag.fullPathName());
            // now add the transform name
            MDagPath transformDag = dag;
            transformDag.pop();
            dagNames.append(transformDag.fullPathName());

            MString primName = usdPrimPath.GetText();

            for (unsigned int i = 0; i < dagNames.length(); ++i)
            {
                MString &dagName = dagNames[i];
                dagName.substitute(MString("|"), MString("/"));
                dagName.substitute(MString(":"), MString("_"));

                if (dagName.length() <= primName.length())
                {
                    MString suffix = primName.substringW( primName.length() - dagName.length(), primName.length() - 1);
                    if (suffix == dagName)
                    {
                        // found it !
                        checkNames = false;
                         // if we recognized the transform name, we need to merge transform & shapes
                        mergeTransformShape = (i > 0);
                        // now verify if we have a parent scope
                        if (dagName.length() < primName.length())
                            parentScope = primName.substringW(0, primName.length() - dagName.length() - 1);
                        
                        break;
                    }
                }
            }
        }
        
        AppendDagToNodesList(nodesList, dag);
    }
    int prevExportDagName = 0;
    MGlobal::executeCommand("getAttr \"defaultArnoldRenderOptions.exportDagName\"", prevExportDagName);
    MString cmd = "setAttr \"defaultArnoldRenderOptions.exportDagName\" ";
    if (mergeTransformShape)
        cmd += "1";
    else
        cmd += "0";
    MGlobal::executeCommand(cmd);

    int prevExportSeparator = 0;
    MGlobal::executeCommand("getAttr \"defaultArnoldRenderOptions.exportSeparator\"", prevExportSeparator);    
    MGlobal::executeCommand("setAttr defaultArnoldRenderOptions.exportSeparator 1");

    int prevExportFullPath = 0;
    MGlobal::executeCommand("getAttr \"defaultArnoldRenderOptions.exportFullPaths\"", prevExportFullPath);    
    MGlobal::executeCommand("setAttr defaultArnoldRenderOptions.exportFullPaths 1");

    MGlobal::executeCommand("setAttr defaultArnoldRenderOptions.exportMayaUsd 1");
    
    // Kill eventual arnold sessions (ARV, etc...)
    MGlobal::executeCommand("arnoldScene -mode \"destroy\"");
    // Create a new session
    MGlobal::executeCommand("arnoldScene -mode \"create\"");
    
    std::string pythonCmd = "import maya.cmds as cmds;nodes = [";
    pythonCmd += nodesList;
    pythonCmd += "]; cmds.arnoldScene(nodes, mode=\"convert_selected\")";

    // run arnoldScene -mode export_selected  ... list of dag paths
    MGlobal::executePythonCommand(pythonCmd.c_str()); 

    // Create an arnold-usd writer that will convert the arnold universe
    // into usd primitives in the current usdStage
    UsdArnoldWriter *writer = new UsdArnoldWriter();
    writer->SetUsdStage(stage); 
    if (parentScope.length() > 0)
    {
        writer->SetScope(parentScope.asChar());
    }
    writer->Write(nullptr);
    
    MGlobal::executeCommand("arnoldScene -mode \"destroy\"");
    // run arnoldScene -mode destroy to destroy this session

    cmd = "setAttr \"defaultArnoldRenderOptions.exportDagName\" ";
    cmd += prevExportDagName;
    MGlobal::executeCommand(cmd);

    cmd = "setAttr \"defaultArnoldRenderOptions.exportSeparator\" ";
    cmd += prevExportSeparator;
    MGlobal::executeCommand(cmd);
    
    cmd = "setAttr \"defaultArnoldRenderOptions.exportFullPaths\" ";
    cmd += prevExportFullPath;
    MGlobal::executeCommand(cmd);
    MGlobal::executeCommand("setAttr defaultArnoldRenderOptions.exportMayaUsd 0");
}

PXR_NAMESPACE_CLOSE_SCOPE