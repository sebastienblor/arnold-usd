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

#include <mayaUsd/fileio/shaderReaderRegistry.h>
#include <mayaUsd/fileio/shaderWriterRegistry.h>
#include <mayaUsd/fileio/shading/shadingModeRegistry.h>

#include "mayaUsdRegistry.h"
#include "usdArnoldShaderWriter.h"
#include "usdArnoldShaderReader.h"

#include <pxr/base/tf/registryManager.h>
#include <pxr/base/tf/staticTokens.h>
#include <pxr/base/tf/stringUtils.h>
#include <pxr/base/tf/token.h>
#include <pxr/pxr.h>

#include <ai.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>

#include <string>
#include <vector>
#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>

PXR_NAMESPACE_OPEN_SCOPE
static std::unordered_map<std::string, ArnoldShaderMap> s_mayaToUsdAttributes;
static std::unordered_map<std::string, ArnoldShaderMap> s_usdToMayaAttributes;
static std::unordered_map<std::string, std::string> s_usdToMayaNodes;
static std::unordered_map<std::string, std::string> s_mayaToUsdNodes;
static bool s_initMaps = true;

// clang-format off
TF_DEFINE_PRIVATE_TOKENS(
    _tokens,

    ((conversionName, "arnold"))
    ((renderContext, "arnold"))
    ((niceName, "Arnold Shaders"))
    ((exportDescription,
         "Dumps the bound shader in an Arnold UsdShade network"))
    ((importDescription,
        "Fetches back an Arnold shader network dumped as UsdShade"))
);
// clang-format on

#ifdef MTOA_USD_CHASER
REGISTER_SHADING_MODE_EXPORT_MATERIAL_CONVERSION(
    _tokens->conversionName,
    _tokens->renderContext,
    _tokens->niceName,
    _tokens->exportDescription);

REGISTER_SHADING_MODE_IMPORT_MATERIAL_CONVERSION(
    _tokens->conversionName,
    _tokens->renderContext,
    _tokens->niceName,
    _tokens->importDescription);
#endif

namespace {

std::string _MakeCamelCase(const std::string &s, bool prefix)
{
   std::string name;
   bool capitalize = false;
   char c;
   if (prefix)
   {
      name = "ai";
      capitalize = true;
   }
   for (size_t i = 0; i < s.length(); i++)
   {
        c = s[i];
        if (c == '_')
            capitalize = true;
        else
        {
            if (capitalize)
                c = toupper(c);
            
            name.append(1, c);
            capitalize = false;
        }
    }
    return name;
}
static inline void InitShaderMaps()
{
    // To be initialized only once
    if (s_initMaps)
    {
        s_initMaps = false;
        
        // If there's no active arnold session, we need to create one
        bool activeUniverse = AiUniverseIsActive();
        if (!activeUniverse)
            AiBegin();

        MString pluginLocation;
        // We need to get the location of the MtoA plugin in order to load the metadata
        // file next to it. This file will give us the mapping between arnold(usd) and maya names
        MGlobal::executeCommand(MString("pluginInfo -q -p \"mtoa\""), pluginLocation);
        int ind = pluginLocation.rindexW('.');
        if (ind > 0)
        {
            MString metadataFile = pluginLocation.substringW(0, ind - 1);
            metadataFile += MString(".mtd");
            // Load MtoA's metadatas
            AiMetaDataLoadFile(metadataFile.asChar());
        }

        // Now loop over all shader types known by arnold
        AtNodeEntryIterator* nodeIter = AiUniverseGetNodeEntryIterator(AI_NODE_SHADER);
        while (!AiNodeEntryIteratorFinished(nodeIter))
        {
            AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(nodeIter);
            std::string entryName = AiNodeEntryGetName(nentry);
            // The fallback for node names is to prefix the arnold node name by ai
            // and to make it camel-case
            std::string mayaType = _MakeCamelCase(entryName, true);
            bool hide = false;

            // ignore this arnold shader if it has the maya.hide metadata
            if (AiMetaDataGetBool(nentry, nullptr, "maya.hide", &hide) && hide)
                continue;
            
            AtString mayaNameStr;
            // check if an explicite maya node name is defined
            if (AiMetaDataGetStr(nentry, nullptr, "maya.name", &mayaNameStr))
                mayaType = mayaNameStr.c_str();
            
            // prefix the shader type (info:id) by arnold namespace
            entryName = std::string("arnold:") + entryName;
            // Store the "node types" maps on both directions (maya->usd + usd->maya)
            s_usdToMayaNodes[entryName] = mayaType.c_str();
            s_mayaToUsdNodes[mayaType.c_str()] = entryName;

            ArnoldShaderMap &usdToMayaParamsMap = s_usdToMayaAttributes[entryName];
            ArnoldShaderMap &mayaToUsdParamsMap = s_mayaToUsdAttributes[entryName];

            // Iterate over this node entry's parameters
            AtParamIterator* paramIt = AiNodeEntryGetParamIterator(nentry);
            while (!AiParamIteratorFinished(paramIt))
            {
                const AtParamEntry *paramEntry = AiParamIteratorGetNext(paramIt);
                const char *paramName = AiParamGetName(paramEntry);
                // as a fallback attribute name, make it camel-case. We don't prefix
                // with "ai" as these are arnold nodes that don't have this prefix in maya
                std::string paramNameStr = _MakeCamelCase(paramName, false);
                AtString mayaAttrNameStr;
                // check if an explicit maya name is set for this attribute
                if (AiMetaDataGetStr(nentry, paramName, "maya.name", &mayaAttrNameStr))
                    paramNameStr = mayaAttrNameStr.c_str();

                // Set attribute maps on both directions
                mayaToUsdParamsMap[paramNameStr] = paramName;
                usdToMayaParamsMap[paramName] = paramNameStr;
            }
            AiParamIteratorDestroy(paramIt);
        }
        AiNodeEntryIteratorDestroy(nodeIter);
        // If the arnold universe was created above, it's time to delete it
        if (!activeUniverse)
            AiEnd();

        // Now let's deal with the maya native shaders. We call the MEL command
        // "listNodeTypes" for these 3 types: shader, texture and utility.
        // For each of these shaders, if they were not found on the previous arnold list,
        // We register this node type to be converted
        const char* mayaShaderTypes[3] = {"shader", "texture", "utility"};
        for (int j = 0; j < 3; ++j)
        {
            MStringArray mayaShaders;
            MString cmd("listNodeTypes \"");
            cmd += MString(mayaShaderTypes[j]);
            cmd += MString("\"");
            MGlobal::executeCommand(cmd, mayaShaders);
            for (unsigned int i = 0; i < mayaShaders.length(); ++i)
            {
                std::string mayaShader = std::string(mayaShaders[i].asChar());
                if (s_mayaToUsdNodes.find(mayaShader) != s_mayaToUsdNodes.end())
                    continue;
                std::string mayaUsdShader = std::string("maya:") + mayaShader;
                s_usdToMayaNodes[mayaUsdShader] = mayaShader;
                s_mayaToUsdNodes[mayaShader] = mayaUsdShader;
            }
        }        
    }
}

template <typename F> void _RegisterMayaNodes(F _registryFunction)
{
    InitShaderMaps(); // initializes the nodeEntry map
    for (std::unordered_map<std::string, std::string>::iterator it = s_usdToMayaNodes.begin(); it != s_usdToMayaNodes.end(); ++it)
        _registryFunction(TfToken(it->second.c_str()), TfToken(it->first.c_str()), _tokens->conversionName);
}
} // namespace


const std::unordered_map<std::string, std::string> &GetArnoldUsdToMayaNodes()
{
    return s_usdToMayaNodes;
}
const std::unordered_map<std::string, std::string> &GetMayaToArnoldUsdNodes()
{
    return s_mayaToUsdNodes;
}
const ArnoldShaderMap &GetArnoldUsdToMayaAttrs(const std::string &shader)
{    
    return s_usdToMayaAttributes[shader];
}
const ArnoldShaderMap &GetMayaToArnoldUsdAttrs(const std::string &shader)
{    
    return s_mayaToUsdAttributes[shader];
}

#ifdef MTOA_USD_CHASER
TF_REGISTRY_FUNCTION(UsdMayaShaderWriterRegistry)
{
    _RegisterMayaNodes(UsdArnoldShaderWriter::RegisterWriter);
}

TF_REGISTRY_FUNCTION(UsdMayaShaderReaderRegistry)
{
    _RegisterMayaNodes(UsdArnoldShaderReader::RegisterReader);
}
#endif

/*
The chaser can be executed with a command like :
cmds.mayaUSDExport(
    file="output.usd",
    frameRange=[1, 10],
    chaser=['arnold'])
*/


PXR_NAMESPACE_CLOSE_SCOPE