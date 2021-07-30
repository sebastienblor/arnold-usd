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

#include "usdArnoldTranslators.h"
#include <mayaUsd\fileio\utils\writeUtil.h>
#include <maya/MGlobal.h>
#include <maya/MAnimControl.h>
#include <ai_nodes.h>
#include "writer.h"
#include "registry.h"
#include "prim_writer.h"


PXR_NAMESPACE_OPEN_SCOPE

// clang-format off
/*
TF_DEFINE_PRIVATE_TOKENS(
    _tokens,

);
*/
// clang-format on

struct ArnoldTranslatorsWriter {

    UsdArnoldWriter writer;
    UsdArnoldWriterRegistry *registry;

};
static ArnoldTranslatorsWriter *s_arnoldWriter = nullptr;
ArnoldSchemaExporter::ArnoldSchemaExporter(const UsdMayaPrimWriterSharedPtr& primWriter, UsdMayaWriteJobContext& jobCtx)
        : UsdMayaSchemaApiWriter(primWriter, jobCtx)
{
    if (s_arnoldWriter == nullptr) {
        s_arnoldWriter = new ArnoldTranslatorsWriter;
        
        s_arnoldWriter->registry = new UsdArnoldWriterRegistry(true);
        s_arnoldWriter->writer.SetRegistry(s_arnoldWriter->registry);

        MGlobal::executeCommand("arnoldScene -mode \"create\"");
        //MGlobal::executeCommand("setAttr defaultArnoldRenderOptions.exportMayaUsd 1");
    }
}

void ArnoldSchemaExporter::Write(const UsdTimeCode& time)
{
    // For now only export the default time
    if (time != UsdTimeCode::Default())
        return;

    UsdPrim prim = _primWriter->GetUsdPrim();
    MString dagName = _primWriter->GetDagPath().fullPathName();
    // Create a new session    

    MString pythonCmd = "import maya.cmds as cmds; cmds.arnoldScene(\"";
    pythonCmd += dagName;
    pythonCmd += "\", mode=\"convert_selected\")";
    MGlobal::displayWarning(pythonCmd);    
    MStringArray res;
    MGlobal::executePythonCommand(pythonCmd, res); 
    
    if (res.length() == 0)
        return;

    /* if we ever need to change the current frame
    float mayaTime = MAnimControl::currentTime().as(MTime::uiUnit());
    float usdTime = time.GetValue();
    if (usdTime != mayaTime)
    {
        MAnimControl::setCurrentTime(usdTime); 
    }
    */

    std::string arnoldNode = res[0].asChar();
    AtNode *node = AiNodeLookUpByName(nullptr, arnoldNode.c_str());
    if (node == nullptr)
        return;
    static AtString nameStr("name");

    // ensure the arnold primitive is named as the target usd prim
    AiNodeSetStr(node, nameStr, prim.GetPath().GetText());

    std::string objType = AiNodeEntryGetName(AiNodeGetNodeEntry(node));
    UsdArnoldPrimWriter *primWriter = s_arnoldWriter->registry->GetPrimWriter(objType);
    if (primWriter == nullptr)
        return;
    s_arnoldWriter->writer.SetFrame(time.GetValue());
    s_arnoldWriter->writer.SetUsdStage(prim.GetStage()); 

    primWriter->WriteNode(node, s_arnoldWriter->writer);
    // we'll need to destroy this node for future frames

}

void ArnoldSchemaExporter::PostExport()
{
    if (s_arnoldWriter)
    {
        MGlobal::executeCommand("arnoldScene -mode \"destroy\"");
        //MGlobal::executeCommand("setAttr defaultArnoldRenderOptions.exportMayaUsd 0");
        delete s_arnoldWriter->registry;
        delete s_arnoldWriter;
        s_arnoldWriter = nullptr;
    }
}
PXR_NAMESPACE_CLOSE_SCOPE