#ifndef MAYAUSD_ARNOLD_SHADER_WRITER_H
#define MAYAUSD_ARNOLD_SHADER_WRITER_H

#include <mayaUsd/base/api.h>
#include <mayaUsd/fileio/jobs/jobArgs.h>
#include <mayaUsd/fileio/shaderWriter.h>
#include <mayaUsd/fileio/writeJobContext.h>

#include <pxr/base/tf/token.h>
#include <pxr/pxr.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/usd/timeCode.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>

#include <unordered_map>

#include "mayaUsdRegistry.h"
PXR_NAMESPACE_OPEN_SCOPE

class UsdArnoldShaderWriter : public UsdMayaShaderWriter
{
public:

    MAYAUSD_CORE_PUBLIC
    static void RegisterWriter(
        const TfToken& mayaNodeTypeName,
        const TfToken& usdShaderId,
        const TfToken& materialConversionName = TfToken());

    MAYAUSD_CORE_PUBLIC
    static ContextSupport CanExport(
        const UsdMayaJobExportArgs& exportArgs,
        const TfToken&              materialConversionName = TfToken());

    MAYAUSD_CORE_PUBLIC
    UsdArnoldShaderWriter(
        const MFnDependencyNode& depNodeFn,
        const SdfPath&           usdPath,
        UsdMayaWriteJobContext&  jobCtx,
        const TfToken&           usdShaderId);

    MAYAUSD_CORE_PUBLIC
    void Write(const UsdTimeCode& usdTime) override;
    
    MAYAUSD_CORE_PUBLIC
    TfToken GetShadingAttributeNameForMayaAttrName(const TfToken& mayaAttrName) override;

    
private:
    std::unordered_map<TfToken, MPlug, TfToken::HashFunctor> _inputNameAttrMap;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif