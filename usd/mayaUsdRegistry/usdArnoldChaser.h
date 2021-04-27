#ifndef MAYAUSD_ARNOLD_SHADER_CHASER_H
#define MAYAUSD_ARNOLD_SHADER_CHASER_H

#include <mayaUsd/base/api.h>
#include <mayaUsd/fileio/jobs/jobArgs.h>
#include <mayaUsd/fileio/shaderWriter.h>
#include <mayaUsd/fileio/writeJobContext.h>

#include <pxr/base/tf/token.h>
#include <pxr/pxr.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/usd/timeCode.h>

#include <mayaUsd/fileio/chaser/exportChaserRegistry.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>

#include <unordered_map>

#include "mayaUsdRegistry.h"
PXR_NAMESPACE_OPEN_SCOPE



class ArnoldUsdChaser : public UsdMayaExportChaser
{
public:
    ArnoldUsdChaser(
        UsdStagePtr                                                     stage,
        const UsdMayaExportChaserRegistry::FactoryContext::DagToUsdMap& dagToUsd
        );

    virtual ~ArnoldUsdChaser() { }

    virtual bool ExportDefault() override
    {        
        return true;
    }

    virtual bool ExportFrame(const UsdTimeCode& frame) override
    {
        return true;
    }

private:
    
};


PXR_NAMESPACE_CLOSE_SCOPE

#endif