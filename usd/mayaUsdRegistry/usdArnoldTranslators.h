
#ifndef MAYAUSD_ARNOLD_TRANSLATORS_H
#define MAYAUSD_ARNOLD_TRANSLATORS_H
//#include <mayaUsd/fileio/schemaApiWriter.h>

#include <unordered_map>
#include <string>
#include <mayaUsd/fileio/primWriterRegistry.h>
#include <mayaUsd/fileio/primWriter.h>
#include <mayaUsd/fileio/writeJobContext.h>

PXR_NAMESPACE_OPEN_SCOPE
#if MAYAUSD_MINOR_VERSION >= 15
#define MTOA_USD_CHASER 1
#elif MAYAUSD_MAJOR_VERSION > 0
#define MTOA_USD_CHASER 1
#endif


/// Exports Arnold DAG nodes
class ArnoldDagWriter : public UsdMayaPrimWriter
{
public:
    ArnoldDagWriter(
        const MFnDependencyNode& depNodeFn,
        const SdfPath&           usdPath,
        UsdMayaWriteJobContext&  jobCtx);

    void Write(const UsdTimeCode& usdTime) override;

private:
    mutable std::string m_parentScope;
    mutable bool m_mergeTransformShapes;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif