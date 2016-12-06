#include "XGenSplineTranslator.h"
#include "extension/Extension.h"
#include "utils/time.h"

#include <maya/MFnDagNode.h>
#include <maya/MFnPluginData.h>
#include <maya/MPxData.h>
#include <maya/MTime.h>

#include <sstream>
#include <string>
#include <vector>


void CXgSplineDescriptionTranslator_ExportSplineData(MDagPath& dagPath, AtNode* procedural, unsigned int step)
{
    MFnDagNode fnDagNode(dagPath);

    // Apply the render overrides
    static const MString sApplyRenderOverrideCmd = "xgmSplineApplyRenderOverride ";
    MGlobal::executeCommand(sApplyRenderOverrideCmd + fnDagNode.partialPathName());

    // Stream out the spline data
    std::string data;
    MPlug       outPlug = fnDagNode.findPlug("outRenderData");
    MObject     outObj  = outPlug.asMObject();
    MPxData*    outData = MFnPluginData(outObj).data();
    if (outData)
    {
        std::ostringstream opaqueStrm;
        outData->writeBinary(opaqueStrm);
        data = opaqueStrm.str();
    }

    // Compute the padding bytes and number of array elements
    const unsigned int tail    = data.size() % sizeof(unsigned int);
    const unsigned int padding = (tail > 0) ? sizeof(unsigned int) - tail : 0;
    const unsigned int nelements = data.size() / sizeof(unsigned int) + (tail > 0 ? 1 : 0);

    // Set the padding size (useless trailing bytes)
    const MString paddingParam = MString("samplePadding_") + step;
    AiNodeDeclare(procedural, paddingParam.asChar(), "constant UINT");
    AiNodeSetUInt(procedural, paddingParam.asChar(), padding);

    // Set the data as array parameter
    const MString dataParam = MString("sampleData_") + step;
    AiNodeDeclare(procedural, dataParam.asChar(), "constant ARRAY UINT");

    AtArray* dataArray = AiArrayAllocate(nelements, 1, AI_TYPE_UINT);
    // FIXME Arnold5 : make sure this is the right thing to do 
    void *arrayData = AiArrayMap(dataArray);
    memcpy(arrayData, &data[0], data.size());
    //------------------------
    AiNodeSetArray(procedural, dataParam.asChar(), dataArray);
}

