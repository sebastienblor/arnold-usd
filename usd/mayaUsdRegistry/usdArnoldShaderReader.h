
#ifndef MAYAUSD_ARNOLD_SHADER_READER_H
#define MAYAUSD_ARNOLD_SHADER_READER_H

#include <mayaUsd/base/api.h>
#include <mayaUsd/fileio/jobs/jobArgs.h>
#include <mayaUsd/fileio/primReaderArgs.h>
#include <mayaUsd/fileio/primReaderContext.h>
#include <mayaUsd/fileio/shaderReader.h>
#include <mayaUsd/fileio/translators/translatorUtil.h>

#include <pxr/base/tf/token.h>
#include <pxr/pxr.h>

#include "mayaUsdRegistry.h"

PXR_NAMESPACE_OPEN_SCOPE

class UsdArnoldShaderReader : public UsdMayaShaderReader
{
public:
    MAYAUSD_CORE_PUBLIC
    static void RegisterReader(
        const TfToken& mayaNodeTypeName,
        const TfToken& usdShaderId,
        const TfToken& materialConversion = TfToken());

    MAYAUSD_CORE_PUBLIC
    static ContextSupport CanImport(
        const UsdMayaJobImportArgs& importArgs,
        const TfToken&              materialConversion = TfToken());

    MAYAUSD_CORE_PUBLIC
    UsdArnoldShaderReader(
        const UsdMayaPrimReaderArgs& readerArgs,
        const TfToken&               mayaNodeTypeName);

#if MAYAUSD_MINOR_VERSION >= 13
    MAYAUSD_CORE_PUBLIC
    bool Read(UsdMayaPrimReaderContext& context) override;
#elif MAYAUSD_MAJOR_VERSION > 0
    MAYAUSD_CORE_PUBLIC
    bool Read(UsdMayaPrimReaderContext& context) override;
#else
    MAYAUSD_CORE_PUBLIC
    bool Read(UsdMayaPrimReaderContext* context) override;
#endif

    MAYAUSD_CORE_PUBLIC
    TfToken GetMayaNameForUsdAttrName(const TfToken& usdAttrName) const override;

private:
    TfToken                _mayaNodeTypeName;
    UsdMayaShadingNodeType _mayaShadingNodeType;
    TfToken                _usdNodeTypeName;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif