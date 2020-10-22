#include "ArnoldStandInSubSceneOverride.h"

#include "nodes/shape/ArnoldStandIns.h"
#include "nodes/options/ArnoldOptionsNode.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MEventMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MDGMessage.h>
#include <maya/MBoundingBox.h>
#include <maya/MPlugArray.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionContext.h>
#include <maya/M3dView.h>

#include <iostream>

#include <ai.h>

#include "utils/MayaUtils.h"
#include "ViewportUtils.h"

MHWRender::MPxSubSceneOverride* CArnoldStandInSubSceneOverride::Creator(const MObject& obj)
{
    return new CArnoldStandInSubSceneOverride(obj);
}

CArnoldStandInSubSceneOverride::CArnoldStandInSubSceneOverride(const MObject& obj)
: CArnoldProceduralSubSceneOverride(obj)
{    
}

CArnoldStandInSubSceneOverride::~CArnoldStandInSubSceneOverride()
{
	
}
