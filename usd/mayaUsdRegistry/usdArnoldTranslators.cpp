//
// Copyright 2020 Autodesk
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4305)
#elif defined(_LINUX) || defined(_DARWIN)
#include <dlfcn.h>
#endif

#include <maya/MGlobal.h>
#include <maya/MAnimControl.h>
#include <ai_nodes.h>
#include <ai.h>
#include "writer.h"
#include "registry.h"
#include "prim_writer.h"
#include "usdArnoldTranslators.h"

#include <mayaUsd/fileio/jobs/jobArgs.h>
#include <mayaUsd/base/api.h>
#include <mayaUsd/fileio/shaderWriter.h>
#include <mayaUsd/fileio/writeJobContext.h>
#ifdef MTOA_USD_CHASER
#include <mayaUsd/fileio/jobContextRegistry.h>
#include <mayaUsd\fileio\utils\writeUtil.h>
#include <mayaUsd/fileio/schemaApiAdaptor.h>
#include <mayaUsd/fileio/schemaApiAdaptorRegistry.h>
#include <mayaUsd/fileio/chaser/exportChaserRegistry.h>
#endif

#include <pxr/usd/usdGeom/camera.h>
#include <pxr/usd/usdGeom/mesh.h>
#if PXR_VERSION >= 2111
#include <pxr/usd/usdLux/lightAPI.h>
#else
#include <pxr/usd/usdLux/light.h>
#endif
#include <pxr/usd/usdShade/material.h>
#include <pxr/usd/usdShade/materialBindingAPI.h>

#include <iostream>

static std::vector<UsdPrim> s_arnoldData;
static std::string s_renderCamera;
static std::string s_optionsNode;
static const MString s_tmpOptionsNodeName = "tmpArnoldMayaUsdOptions";

TF_DEFINE_PRIVATE_TOKENS(
    _tokens,
    (arnold)
    (ArnoldPolymeshAPI)
    (ArnoldCurvesAPI)
    (ArnoldPerspCameraAPI)
    (ArnoldDistantLightAPI)
    (ArnoldPointLightAPI)
    (ArnoldSphereLightAPI)
    (ArnoldAreaLightAPI)
    ((arnold_shader, "primvars:arnold:shader"))
    ((arnold_camera, "arnold:camera"))
);

PXR_NAMESPACE_OPEN_SCOPE
#ifdef MTOA_USD_CHASER

#include <mayaUsd/fileio/jobContextRegistry.h>

REGISTER_EXPORT_JOB_CONTEXT_FCT(
    Arnold,
    "Arnold",
    "Arnold")
{
    VtDictionary extraArgs;
    extraArgs[UsdMayaJobExportArgsTokens->apiSchema]
        = VtValue(std::vector<VtValue> { VtValue(_tokens->ArnoldPolymeshAPI.GetString()),
                                         VtValue(_tokens->ArnoldCurvesAPI.GetString()),
                                         VtValue(_tokens->ArnoldPerspCameraAPI.GetString()),
                                         VtValue(_tokens->ArnoldDistantLightAPI.GetString()),
                                         VtValue(_tokens->ArnoldPointLightAPI.GetString()),
                                         //VtValue(_tokens->ArnoldSphereLightAPI.GetString()),
                                         VtValue(_tokens->ArnoldAreaLightAPI.GetString())
                                          });
    extraArgs[UsdMayaJobExportArgsTokens->chaser]
        = VtValue(std::vector<VtValue> { VtValue(std::string("ArnoldUsdChaser")) });

/*
    extraArgs[UsdMayaJobExportArgsTokens->shadingMode]
        = VtValue(std::string("useRegistry")) ;

    extraArgs[UsdMayaJobExportArgsTokens->convertMaterialsTo]
        = VtValue(std::vector<VtValue> { VtValue(std::string("arnold")) });
*/
    return extraArgs;
}


class ArnoldSchemaExporter : public UsdMayaSchemaApiAdaptor
{
    using _baseClass = UsdMayaSchemaApiAdaptor;

public:
    ArnoldSchemaExporter(
        const MObjectHandle&     object,
        const TfToken&           schemaName,
        const UsdPrimDefinition* schemaPrimDef)
        : UsdMayaSchemaApiAdaptor(object, schemaName, schemaPrimDef)
    {
        if (s_optionsNode.empty())
        {
            MSelectionList list;
            list.add(s_tmpOptionsNodeName);
            if (list.length() > 0)
                return;

            // Here we want to create a new "options" node, inheriting from the default one,
            // so that we can override its attributes. It's a bit hacky, but for now it's
            // a simple way to get this to work : We're creating a new aiOptions node, and while 
            // looping over all of its attributes we're connecting them to the corresponding ones
            // in defaultArnoldRenderOptions. This way we can provide this new options node to 
            // "arnoldScene" and the export will be done with this new node. Once the export is
            // finished, we can delete this temporary node.

            MString optStr = "import maya.cmds as cmds\n";
            optStr += MString("tmpOptions = cmds.createNode(\"aiOptions\", name=\"")+ s_tmpOptionsNodeName + MString("\")\n");
            optStr += "attrs = cmds.listAttr(\"defaultArnoldRenderOptions\", hd=True,iu=True, c=True)\n";
            optStr += "for attr in attrs:\n";
            optStr  += "    if attr in ['exportMayaUsd','exportFullPaths','exportSeparator', 'exportDagName', 'exportPrefix']:\n";
            optStr  += "        continue\n";
            optStr += "    try:\n";
            optStr += "        cmds.connectAttr('defaultArnoldRenderOptions.{}'.format(attr), '{}.{}'.format(tmpOptions, attr))\n";
            optStr += "    except RuntimeError as err:\n";
            optStr += "        pass\n";
            optStr += "cmds.setAttr('{}.exportMayaUsd'.format(tmpOptions), 1)\n";
            MGlobal::executePythonCommand(optStr);
            s_optionsNode = s_tmpOptionsNodeName.asChar();
        }
    }

    ~ArnoldSchemaExporter() override { }

    bool CanAdapt() const override
    {
        // This class does not adapt in a freeform context.
        return true;
    }

    bool CanAdaptForExport(const UsdMayaJobExportArgs& jobArgs) const override
    {        
        // This class is specialized for export:
        if (!_handle.isValid()) {
            return false;
        }
        MFnDependencyNode depFn;
        if (depFn.setObject(_handle.object()) != MS::kSuccess)
            return false;

        m_parentScope = std::string(jobArgs.parentScope.GetString());
        if (!m_parentScope.empty())
            m_parentScope += "/";
        m_mergeTransformShapes = jobArgs.mergeTransformAndShape;
        return true;
    }

 
    bool CopyToPrim(
        const UsdPrim&             prim,
        const UsdTimeCode&         usdTime,
        UsdUtilsSparseValueWriter* valueWriter) const override
    {   
        MFnDependencyNode depFn;
        if (depFn.setObject(_handle.object()) != MS::kSuccess)
            return false;

        MString nodeName = depFn.name();
        // we'll be passing the tmp options node to "arnoldScene", 
        // so we want to set properly its attributes "exportDagName" and "exportPrefix"
        // to ensure they're consistent with mayausd
        std::string pythonCmd = "import maya.cmds as cmds;";
        if (!s_optionsNode.empty())
        {
            pythonCmd += "cmds.setAttr(\"";
            pythonCmd += s_optionsNode;
            pythonCmd += ".exportDagName\",";
            if (m_mergeTransformShapes)
                pythonCmd += "1";
            else
                pythonCmd += "0";

            if (!m_parentScope.empty())
            {
                pythonCmd += ");cmds.setAttr(\"";
                pythonCmd += s_optionsNode;
                pythonCmd += ".exportPrefix\",\"";
                pythonCmd += m_parentScope;
                pythonCmd += "\", type='string'";
            }
        }
        pythonCmd += ");nodes = [\"";
        pythonCmd += nodeName.asChar();
        pythonCmd += "\"]; cmds.arnoldScene(nodes, mode=\"convert_selected\",";
        if (!s_optionsNode.empty())
        {
            pythonCmd += "options=\"";
            pythonCmd += s_optionsNode;
            pythonCmd += "\",";
        }
        pythonCmd += "list=\"newNodes\")";
        MStringArray res;
        MGlobal::executePythonCommand(pythonCmd.c_str(), res); 
        if (res.length() > 0)
        {
            AtNode *node = AiNodeLookUpByName(nullptr, AtString(res[0].asChar()));
            if (node)
                AiNodeSetStr(node, AtString("name"), AtString(prim.GetPath().GetText()));
        }

        UsdArnoldWriter writer;
        writer.SetUsdStage(prim.GetStage()); 
        writer.SetWriteMaterialBindings(false);
        if (!usdTime.IsDefault())
            writer.SetFrame(usdTime.GetValue());

        int mask = AI_NODE_SHAPE |AI_NODE_SHADER;
#if PXR_VERSION >= 2111
        if (prim.HasAPI<UsdLuxLightAPI>())
#else
        if (prim.IsA<UsdLuxLight>())
#endif
            mask = AI_NODE_LIGHT;
        else if (prim.IsA<UsdGeomCamera>())
        {
            mask = AI_NODE_CAMERA;
            // check if this camera is renderable
            // Set the export camera
            if (depFn.findPlug("renderable", true).asBool())
               s_renderCamera = prim.GetPath().GetText();
        }
        else
            s_arnoldData.push_back(prim);

        writer.SetMask(mask); // change the mask depending on the node type

        writer.Write(nullptr);

        MGlobal::executeCommand("arnoldScene -mode \"destroy\"");
        return true;
    }
    mutable std::string m_parentScope;
    mutable bool m_mergeTransformShapes;
};

PXRUSDMAYA_REGISTER_SCHEMA_API_ADAPTOR(mesh, ArnoldPolymeshAPI, ArnoldSchemaExporter);
PXRUSDMAYA_REGISTER_SCHEMA_API_ADAPTOR(curves, ArnoldCurvesAPI, ArnoldSchemaExporter);
PXRUSDMAYA_REGISTER_SCHEMA_API_ADAPTOR(camera, ArnoldPerspCameraAPI, ArnoldSchemaExporter);
PXRUSDMAYA_REGISTER_SCHEMA_API_ADAPTOR(directionalLight, ArnoldDistantLightAPI, ArnoldSchemaExporter);
PXRUSDMAYA_REGISTER_SCHEMA_API_ADAPTOR(pointLight, ArnoldPointLightAPI, ArnoldSchemaExporter);
//PXRUSDMAYA_REGISTER_SCHEMA_API_ADAPTOR(curves, ArnoldSphereLightAPI, ArnoldSchemaExporter);
PXRUSDMAYA_REGISTER_SCHEMA_API_ADAPTOR(areaLight, ArnoldAreaLightAPI, ArnoldSchemaExporter);
PXRUSDMAYA_REGISTER_SCHEMA_API_ADAPTOR(shadingEngine, ArnoldMaterialAPI, ArnoldSchemaExporter);

class ArnoldUsdChaser : public UsdMayaExportChaser
{
public:
    ArnoldUsdChaser(
        UsdStagePtr                                                     stage,
        const UsdMayaExportChaserRegistry::FactoryContext::DagToUsdMap& dagToUsd
        );

    virtual ~ArnoldUsdChaser() { }

    bool ExportDefault() override;
    bool ExportFrame(const UsdTimeCode& frame) override;
    bool PostExport() override;
private:
    
};

ArnoldUsdChaser::ArnoldUsdChaser(
        UsdStagePtr                                                     stage,
        const UsdMayaExportChaserRegistry::FactoryContext::DagToUsdMap& dagToUsd
        )
{
    UsdTimeCode timeCode;
    MString nodeName = "defaultArnoldRenderOptions";
    std::string pythonCmd = "import maya.cmds as cmds;nodes = [\"";
    pythonCmd += nodeName.asChar();
    pythonCmd += "\"]; cmds.arnoldScene(nodes, mode=\"convert_selected\",";
    if (!s_optionsNode.empty())
    {
        pythonCmd += "options=\"";
        pythonCmd += s_optionsNode;
        pythonCmd += "\",";
    }
    pythonCmd += " list=\"newNodes\")";
    MGlobal::executePythonCommand(pythonCmd.c_str()); 
    
    UsdArnoldWriter writer;
    writer.SetUsdStage(stage); 
    
    writer.SetMask(AI_NODE_ALL); 
    writer.Write(nullptr);

    for (auto prim : s_arnoldData)
    {
        UsdShadeMaterial mat = UsdShadeMaterialBindingAPI(prim).GetDirectBinding().GetMaterial();
        if (!mat)
            continue; 

        UsdAttribute attr = prim.GetAttribute(_tokens->arnold_shader);
        if (!attr)
            continue;

        VtArray<std::string> vtArr;
        if (!attr.Get(&vtArr))
            continue;

        if (vtArr.empty())
            continue;
        std::string shader = vtArr[0];

        // find prim with this name
        if (shader.empty())
            continue;
        if (shader[0] != '/')
            shader = std::string("/") + shader;

        UsdPrim shaderPrim = stage->GetPrimAtPath(SdfPath(shader.c_str()));
        if (shaderPrim)
        {
            UsdShadeOutput surfaceOutput = mat.CreateSurfaceOutput(_tokens->arnold);
            std::string surfaceTargetName = shader + std::string(".outputs:surface");
            surfaceOutput.ConnectToSource(SdfPath(surfaceTargetName));
            attr.Clear();
        }
    }
    s_arnoldData.clear();
    // for meshes/curves, get the exported material. Then add an arnold context connection on the material to the shader
    // need to get the material before writer. Then re-introduce this connection after it, and delete the new materials primitive
    
    MGlobal::executeCommand("arnoldScene -mode \"destroy\"");

    static SdfPath s_optionsPath("/options");

    // Finally, set the export camera in the optios node
    UsdPrim optionsPrim = stage->GetPrimAtPath(s_optionsPath);
    if (optionsPrim)
    {
        UsdAttribute camAttr = optionsPrim.CreateAttribute(_tokens->arnold_camera, SdfValueTypeNames->String, false);
        if (camAttr && stage->GetPrimAtPath(s_optionsPath))
            camAttr.Set(s_renderCamera);

    }
}
bool ArnoldUsdChaser::ExportDefault()
{
   return true;
}

bool ArnoldUsdChaser::ExportFrame(const UsdTimeCode& frame)
{
    return true;
}

bool ArnoldUsdChaser::PostExport()
{
    if (!s_optionsNode.empty())
    {
        MString cmd = "delete ";
        cmd += MString(s_optionsNode.c_str());
        MGlobal::executeCommand(cmd);
        s_optionsNode = "";
    }
    return true;
}

PXRUSDMAYA_DEFINE_EXPORT_CHASER_FACTORY(ArnoldUsdChaser, ctx)
{
    return new ArnoldUsdChaser(ctx.GetStage(), ctx.GetDagToUsdMap());
}

#endif

ArnoldDagWriter::ArnoldDagWriter(
    const MFnDependencyNode& depNodeFn,
    const SdfPath&           usdPath,
    UsdMayaWriteJobContext&  jobCtx)
    : UsdMayaPrimWriter(depNodeFn, usdPath, jobCtx)
{
    const UsdMayaJobExportArgs& jobArgs = jobCtx.GetArgs();
    m_parentScope = std::string(jobArgs.parentScope.GetString());
    if (!m_parentScope.empty())
        m_parentScope += "/";
    m_mergeTransformShapes = jobArgs.mergeTransformAndShape;
    Write(UsdTimeCode::Default());
}

/* virtual */
void ArnoldDagWriter::Write(const UsdTimeCode& usdTime)
{
    MStatus status;
    UsdMayaPrimWriter::Write(usdTime);
    // Since write() above will take care of any animation on the node's
    // transform, we only want to proceed here if:
    // - We are at the default time and NO attributes on the shape are animated.
    //    OR
    // - We are at a non-default time and some attribute on the shape IS animated.
    if (usdTime.IsDefault() == _HasAnimCurves()) {
        return;
    }
    MFnDependencyNode depFn(GetMayaObject());
    MString nodeName = depFn.name();
    std::string pythonCmd = "import maya.cmds as cmds;nodes = [\"";
    pythonCmd += nodeName.asChar();
    pythonCmd += "\"];";
    if (!s_optionsNode.empty())
    {
        pythonCmd += "cmds.setAttr(\"";
        pythonCmd += s_optionsNode;
        pythonCmd += ".exportDagName\",";
        if (m_mergeTransformShapes)
            pythonCmd += "1";
        else
            pythonCmd += "0";

        if (!m_parentScope.empty())
        {
            pythonCmd += ");cmds.setAttr(\"";
            pythonCmd += s_optionsNode;
            pythonCmd += ".exportPrefix\",\"";
            pythonCmd += m_parentScope;
            pythonCmd += "\", type='string'";
        }
        pythonCmd += ");";
    }
    pythonCmd += "cmds.arnoldScene(nodes,";
    if (!s_optionsNode.empty())
    {
        pythonCmd += "options=\"";
        pythonCmd += s_optionsNode;
        pythonCmd += "\",";
    }
    pythonCmd += " mode=\"convert_selected\")";
    MGlobal::executePythonCommand(pythonCmd.c_str()); 
    
    UsdArnoldWriter writer;
    writer.SetUsdStage(GetUsdStage()); 
    if (!usdTime.IsDefault())
        writer.SetFrame(usdTime.GetValue());

    writer.SetMask(AI_NODE_LIGHT |AI_NODE_SHAPE | AI_NODE_OPERATOR); // change the mask depending on the node type
    writer.Write(nullptr);
}


PXRUSDMAYA_REGISTER_WRITER(aiSkyDomeLight, ArnoldDagWriter);
PXRUSDMAYA_REGISTER_WRITER(aiAreaLight, ArnoldDagWriter);
PXRUSDMAYA_REGISTER_WRITER(aiPhotometricLight, ArnoldDagWriter);
PXRUSDMAYA_REGISTER_WRITER(aiLightPortal, ArnoldDagWriter);
PXRUSDMAYA_REGISTER_WRITER(aiMeshPortal, ArnoldDagWriter);
PXRUSDMAYA_REGISTER_WRITER(aiStandIn, ArnoldDagWriter);
PXRUSDMAYA_REGISTER_WRITER(aiVolume, ArnoldDagWriter);
PXRUSDMAYA_REGISTER_WRITER(aiCurveCollector, ArnoldDagWriter);


PXR_NAMESPACE_CLOSE_SCOPE
