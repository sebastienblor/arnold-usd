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

#include "usdArnoldShaderReader.h"

#include <mayaUsd/fileio/jobs/jobArgs.h>
#include <mayaUsd/fileio/primReaderArgs.h>
#include <mayaUsd/fileio/primReaderContext.h>
#include <mayaUsd/fileio/shaderReader.h>
#include <mayaUsd/fileio/shaderReaderRegistry.h>
#include <mayaUsd/fileio/translators/translatorUtil.h>
#include <mayaUsd/fileio/utils/roundTripUtil.h>
#include <mayaUsd/utils/util.h>

#include <pxr/base/tf/diagnostic.h>
#include <pxr/base/tf/staticTokens.h>
#include <pxr/base/tf/stringUtils.h>
#include <pxr/base/tf/token.h>
#include <pxr/pxr.h>
#include <pxr/usd/usd/attribute.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usdShade/input.h>
#include <pxr/usd/usdShade/shader.h>
#include <pxr/usd/usdShade/tokens.h>
#include <pxr/usd/usdShade/utils.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MGlobal.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>

#include <string>
#include <utility>
#include <vector>

PXR_NAMESPACE_OPEN_SCOPE

// clang-format off
TF_DEFINE_PRIVATE_TOKENS(
    _tokens,
    ((MayaShaderOutputName, "outColor"))
    ((out, "out"))

);
// clang-format on

/* static */
void UsdArnoldShaderReader::RegisterReader(
    const TfToken& mayaNodeTypeName,
    const TfToken& usdShaderId,
    const TfToken& materialConversion)
{
    UsdMayaShaderReaderRegistry::Register(
        usdShaderId,
        [materialConversion](const UsdMayaJobImportArgs& importArgs) {
            return UsdArnoldShaderReader::CanImport(importArgs, materialConversion);
        },
        [mayaNodeTypeName](const UsdMayaPrimReaderArgs& readerArgs) {
            return std::make_shared<UsdArnoldShaderReader>(readerArgs, mayaNodeTypeName);
        });
}

/* static */
UsdMayaShaderReader::ContextSupport UsdArnoldShaderReader::CanImport(
    const UsdMayaJobImportArgs& importArgs,
    const TfToken&              materialConversion)
{
    if (materialConversion.IsEmpty() || importArgs.GetMaterialConversion() == materialConversion) {
        // This shader reader advertises "Fallback" support so that any more
        // specialized readers for a particular shader ID can take precedence.
        return ContextSupport::Fallback;
    }

    return ContextSupport::Unsupported;
}

static UsdMayaShadingNodeType
_ComputeShadingNodeTypeForMayaTypeName(const TfToken& mayaNodeTypeName)
{
    // Use NonShading as a fallback.
    UsdMayaShadingNodeType shadingNodeType = UsdMayaShadingNodeType::NonShading;

    MStatus status;
    MString cmd;
    status = cmd.format("getClassification ^1s", mayaNodeTypeName.GetText());
    CHECK_MSTATUS_AND_RETURN(status, shadingNodeType);

    MStringArray compoundClassifications;
    status = MGlobal::executeCommand(cmd, compoundClassifications, false, false);
    CHECK_MSTATUS_AND_RETURN(status, shadingNodeType);

    // Consider both arnold and maya shaders
    static const std::vector<std::pair<std::string, UsdMayaShadingNodeType>> _classificationsToTypes
        = { { "rendernode/arnold/texture", UsdMayaShadingNodeType::Texture },
            { "rendernode/arnold/utility", UsdMayaShadingNodeType::Utility },
            { "rendernode/arnold/shader", UsdMayaShadingNodeType::Shader },
            { "texture", UsdMayaShadingNodeType::Texture },
            { "utility", UsdMayaShadingNodeType::Utility },
            { "shader", UsdMayaShadingNodeType::Shader }
        };

    // The docs for getClassification() are pretty confusing. You'd think that
    // the string array returned would give you each "classification", but
    // instead, it's a list of "single compound classification string by
    // joining the individual classifications with ':'".

    // Loop over the compoundClassifications, though I believe
    // compoundClassifications will always have size 0 or 1.

#if MAYA_API_VERSION >= 20190000
    for (const MString& compoundClassification : compoundClassifications) {
#else
    for (unsigned int i = 0u; i < compoundClassifications.length(); ++i) {
        const MString& compoundClassification = compoundClassifications[i];
#endif
        const std::string compoundClassificationStr(compoundClassification.asChar());
        for (const std::string& classification : TfStringSplit(compoundClassificationStr, ":")) {
            for (const auto& classPrefixAndType : _classificationsToTypes) {
                if (TfStringStartsWith(classification, classPrefixAndType.first)) {
                    return classPrefixAndType.second;
                }
            }
        }
    }
    return shadingNodeType;
}


UsdArnoldShaderReader::UsdArnoldShaderReader(
    const UsdMayaPrimReaderArgs& readerArgs,
    const TfToken&               mayaNodeTypeName)
    : UsdMayaShaderReader(readerArgs)
{
}

/* override */
bool UsdArnoldShaderReader::Read(UsdMayaPrimReaderContext* context)
{
    const UsdPrim&       prim = _GetArgs().GetUsdPrim();
    const UsdShadeShader shaderSchema = UsdShadeShader(prim);
    if (!shaderSchema)
        return false;
    
    TfToken shaderId;
    if (!shaderSchema.GetShaderId(&shaderId))
        return false;
    
    std::string usdShaderId(shaderId.GetText());
    std::string mayaType = usdShaderId;
    const ArnoldShaderMap &shadersMap = GetArnoldUsdToMayaNodes();
    ArnoldShaderMap::const_iterator it = shadersMap.find(usdShaderId);
    if (it != shadersMap.end())
        mayaType = it->second;

    _mayaNodeTypeName = TfToken(mayaType);
    _mayaShadingNodeType = _ComputeShadingNodeTypeForMayaTypeName(_mayaNodeTypeName);

    MStatus           status;
    MObject           mayaObject;
    MFnDependencyNode depNodeFn;
    if (!(UsdMayaTranslatorUtil::CreateShaderNode(
              MString(prim.GetName().GetText()),
              _mayaNodeTypeName.GetText(),
              _mayaShadingNodeType,
              &status,
              &mayaObject)
          && depNodeFn.setObject(mayaObject))) {
        TF_RUNTIME_ERROR(
            "Could not create node of type %s for shader '%s'. "
            "Probably missing a loadPlugin.\n",
            _mayaNodeTypeName.GetText(),
            prim.GetPath().GetText());
        return false;
    }

    context->RegisterNewMayaNode(prim.GetPath().GetString(), mayaObject);

    const ArnoldShaderMap &paramsMap = GetArnoldUsdToMayaAttrs(usdShaderId);

    for (const UsdShadeInput& input : shaderSchema.GetInputs()) {
        const UsdAttribute& usdAttr = input.GetAttr();
        const std::string  usdAttrName = usdAttr.GetBaseName().GetString();
        std::string mayaAttrName = usdAttrName;
        ArnoldShaderMap::const_iterator it = paramsMap.find(usdAttrName);
        if (it != paramsMap.end())
            mayaAttrName = it->second;

        MPlug attrPlug = depNodeFn.findPlug(mayaAttrName.c_str(), &status);
        if (status != MS::kSuccess) {
            continue;
        }

        unsigned int index = 0u;
        if (UsdMayaRoundTripUtil::GetAttributeArray(usdAttr, &index)) {
            attrPlug = attrPlug.elementByLogicalIndex(index, &status);
            if (status != MS::kSuccess) {
                continue;
            }
        }

        UsdMayaUtil::setPlugValue(usdAttr, attrPlug);
    }

    return true;
}

/* override */
TfToken UsdArnoldShaderReader::GetMayaNameForUsdAttrName(const TfToken& usdAttrName) const
{
    TfToken               usdBaseName;
    UsdShadeAttributeType usdAttrType;
    std::tie(usdBaseName, usdAttrType) = UsdShadeUtils::GetBaseNameAndType(usdAttrName);

    // The one edge case we're handling here is the connection to a "top-level"
    // shader from one of its Material prim's terminal outputs. We make an
    // assumption about the name of the Maya shading node's primary output
    // attribute.
    if (usdAttrType == UsdShadeAttributeType::Output
        && (usdBaseName == UsdShadeTokens->surface || usdBaseName == UsdShadeTokens->displacement
            || usdBaseName == UsdShadeTokens->volume || usdBaseName == _tokens->out)) {
        return _tokens->MayaShaderOutputName;
    }

    const UsdPrim&       prim = const_cast<UsdArnoldShaderReader*>(this)->_GetArgs().GetUsdPrim();
    const UsdShadeShader shaderSchema = UsdShadeShader(prim);
  
    TfToken shaderId;
    if (!shaderSchema || !shaderSchema.GetShaderId(&shaderId))
        return usdBaseName;
    
    std::vector<std::string> attrNameSplit = TfStringSplit(usdAttrName.GetString(), ":");
    std::string attrName = attrNameSplit.back();

    const ArnoldShaderMap &paramsMap = GetArnoldUsdToMayaAttrs(shaderId);
    if (!paramsMap.empty())
    {
        ArnoldShaderMap::const_iterator it = paramsMap.find(attrName);
        if (it != paramsMap.end()){
            attrName = it->second;
        }
    }        

    usdBaseName = TfToken(attrName);
    return usdBaseName;
}

PXR_NAMESPACE_CLOSE_SCOPE
