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

#include "usdArnoldShaderWriter.h"

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
#include <pxr/usd/usd/timeCode.h>
#include <pxr/usd/usdShade/input.h>
#include <pxr/usd/usdShade/output.h>
#include <pxr/usd/usdShade/shader.h>

#include <maya/MFnAttribute.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MGlobal.h>

#include <utility>

PXR_NAMESPACE_OPEN_SCOPE

// clang-format off
TF_DEFINE_PRIVATE_TOKENS(
    _tokens,
    ((out, "out"))
);

/* static */
void UsdArnoldShaderWriter::RegisterWriter(
    const TfToken& mayaNodeTypeName,
    const TfToken& usdShaderId,
    const TfToken& materialConversionName) 
{
    UsdMayaShaderWriterRegistry::Register(
        mayaNodeTypeName,
        [materialConversionName](const UsdMayaJobExportArgs& exportArgs) {
            return UsdArnoldShaderWriter::CanExport(exportArgs, materialConversionName);
        },
        [usdShaderId](
            const MFnDependencyNode& depNodeFn,
            const SdfPath&           usdPath,
            UsdMayaWriteJobContext&  jobCtx) {
            return std::make_shared<UsdArnoldShaderWriter>(
                depNodeFn, usdPath, jobCtx, usdShaderId);
        });
}

/* static */
UsdMayaShaderWriter::ContextSupport UsdArnoldShaderWriter::CanExport(
    const UsdMayaJobExportArgs& exportArgs,
    const TfToken&              materialConversionName)
{
    if (materialConversionName.IsEmpty()
        || exportArgs.convertMaterialsTo == materialConversionName) {
        return ContextSupport::Supported;
    }

    return ContextSupport::Unsupported;
}

UsdArnoldShaderWriter::UsdArnoldShaderWriter(
    const MFnDependencyNode& depNodeFn,
    const SdfPath&           usdPath,
    UsdMayaWriteJobContext&  jobCtx,
    const TfToken&           usdShaderId)
    : UsdMayaShaderWriter(depNodeFn, usdPath, jobCtx)
{
    UsdShadeShader shaderSchema = UsdShadeShader::Define(GetUsdStage(), GetUsdPath());
    if (!TF_VERIFY(
            shaderSchema,
            "Could not define UsdShadeShader at path '%s'\n",
            GetUsdPath().GetText())) {
        return;
    }

    _usdPrim = shaderSchema.GetPrim();
    if (!TF_VERIFY(
            _usdPrim,
            "Could not get UsdPrim for UsdShadeShader at path '%s'\n",
            shaderSchema.GetPath().GetText())) {
        return;
    }

    const ArnoldShaderMap &shaderMap = GetMayaToArnoldUsdAttrs(usdShaderId.GetText());
    shaderSchema.CreateIdAttr(VtValue(usdShaderId));

    for (unsigned int i = 0u; i < depNodeFn.attributeCount(); ++i) {
        const MObject      attrObj = depNodeFn.reorderedAttribute(i);
        MPlug              attrPlug = depNodeFn.findPlug(attrObj, true);
        const MFnAttribute attrFn(attrObj);

        if (attrPlug.isProcedural() || attrFn.isHidden()) {
            // The Maya docs say these should not be saved off.
            continue;
        }

        if (attrPlug.isChild()) {
            continue;
        }

        // For now, we only support arrays of length 1. If we encounter such an
        // array, we emit it's 0-th element.
        if (attrPlug.isArray()) {
            const unsigned int numElements = attrPlug.evaluateNumElements();
            if (numElements == 0u) {
                // Empty array, so skip this attribute.
                continue;
            }

            if (numElements > 1u) {
                TF_WARN(
                    "Array with multiple elements encountered for "
                    "attribute '%s' on node '%s'. Currently, only arrays "
                    "with a single element are supported.",
                    attrPlug.name().asChar(),
                    UsdMayaUtil::GetMayaNodeName(depNodeFn.object()).c_str());
            }

            attrPlug = attrPlug[0];
        }

        // Keep our authoring sparse by ignoring attributes with no values set
        // and no connections.
        if (!UsdMayaUtil::IsAuthored(attrPlug) && !attrPlug.isConnected()) {
            continue;
        }

        TfToken usdAttrName
            = TfToken(UsdMayaShadingUtil::GetStandardAttrName(attrPlug, false));
        if (usdAttrName.IsEmpty()) {
            continue;
        }

        // Get the corresponding USD attribute name for the input maya name                
        if (!shaderMap.empty())
        {
            ArnoldShaderMap::const_iterator it = shaderMap.find(usdAttrName.GetText());
            if (it != shaderMap.end())
                usdAttrName = TfToken(it->second);
        }

        const SdfValueTypeName valueTypeName = MayaUsd::Converter::getUsdTypeName(attrPlug);
        if (!valueTypeName) {
            // Unsupported Maya attribute type (e.g. "message" attributes).
            continue;
        }

        // If the Maya attribute is writable, we assume it must be an input.
        // Inputs can still be connected as sources to inputs on other shaders.
        if (attrFn.isWritable()) {
            UsdShadeInput input = shaderSchema.CreateInput(usdAttrName, valueTypeName);
            if (!input) {
                continue;
            }

            if (attrPlug.isElement()) {
                UsdMayaRoundTripUtil::MarkAttributeAsArray(input.GetAttr(), 0u);
            }

            // Add this input to the name/attrPlug map. We'll iterate through
            // these entries during Write() to set their values.
            _inputNameAttrMap.insert(std::make_pair(usdAttrName, attrPlug));
        } else if (attrPlug.isConnected()) {
            // Only author outputs for non-writable attributes if they are
            // connected.
            MPlugArray conns;
            attrPlug.connectedTo(conns, false, true);

            if (usdAttrName != UsdShadeTokens->surface)
                usdAttrName = _tokens->out;

            shaderSchema.CreateOutput(usdAttrName, valueTypeName);
        }
    }
}

/* override */
void UsdArnoldShaderWriter::Write(const UsdTimeCode& usdTime)
{
    UsdMayaShaderWriter::Write(usdTime);
    UsdShadeShader shaderSchema(_usdPrim);
    if (!TF_VERIFY(
            shaderSchema,
            "Could not get UsdShadeShader schema for UsdPrim at path '%s'\n",
            _usdPrim.GetPath().GetText())) {
        return;
    }

    for (const auto& inputAttrPair : _inputNameAttrMap) {
        const TfToken& inputName = inputAttrPair.first;
        const MPlug&   attrPlug = inputAttrPair.second;

        UsdShadeInput input = shaderSchema.GetInput(inputName);
        if (!input) {
            continue;
        }
        UsdMayaWriteUtil::SetUsdAttr(attrPlug, input.GetAttr(), usdTime, _GetSparseValueWriter());
    }
}

/* override */
TfToken
UsdArnoldShaderWriter::GetShadingAttributeNameForMayaAttrName(const TfToken& mayaAttrName)
{
    UsdShadeShader shaderSchema(_usdPrim);
    if (!shaderSchema) {
        return TfToken();
    }

    TfToken id;
    shaderSchema.GetIdAttr().Get(&id);
    std::string shaderId = id.GetString();
    TfToken usdAttrName = mayaAttrName;
    const ArnoldShaderMap &paramsMap = GetMayaToArnoldUsdAttrs(shaderId);

    if (!paramsMap.empty())
    {
        ArnoldShaderMap::const_iterator it = paramsMap.find(mayaAttrName.GetText());
        if (it != paramsMap.end())
            usdAttrName = TfToken(it->second);
    }

    const UsdShadeInput input = shaderSchema.GetInput(usdAttrName);
    if (input) {
        return input.GetFullName();
    }
    if (mayaAttrName != UsdShadeTokens->surface )
        usdAttrName = _tokens->out;

    const UsdShadeOutput output = shaderSchema.GetOutput(usdAttrName);
    if (output) {
        return output.GetFullName();
    }

    return TfToken();
}

PXR_NAMESPACE_CLOSE_SCOPE