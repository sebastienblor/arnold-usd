#include "MeshProceduralTranslator.h"
#include "attributes/AttrHelper.h"
#include "utils/time.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

#include <maya/MBoundingBox.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MEventMessage.h>

#include <maya/MString.h>

//static MCallbackId s_idleCallback = 0;
//static std::vector<CMeshProceduralTranslator *> s_updatedProcedurals;

void CMeshProceduralTranslator::NodeInitializer(CAbTranslator context)
{   
   CExtensionAttrHelper helper(context.maya, "procedural");
   //CShapeTranslator::MakeCommonAttributes(helper);

   CAttrData data;
   data.name = "dso";
   data.shortName = "dso";
   data.channelBox = false;
   helper.MakeInputString(data);

   data.name = "data";
   data.shortName = "data";
   data.channelBox = false;
   helper.MakeInputString(data);

   data.defaultValue.BOOL() = true;
   data.name = "overrideLightLinking";
   data.shortName = "oll";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL() = true;
   data.name = "overrideShaders";
   data.shortName = "osh";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "useFrameExtension";
   data.shortName = "useFrameExtension";
   helper.MakeInputBoolean(data);

   data.defaultValue.INT() = 0;
   data.name = "frameNumber";
   data.shortName = "frameNumber";
   helper.MakeInputInt(data);

   data.defaultValue.BOOL() = false;
   data.name = "useSubFrame";
   data.shortName = "useSubFrame";
   helper.MakeInputBoolean(data);

   data.defaultValue.FLT() = 0.f;
   data.name = "frameOffset";
   data.shortName = "frameOffset";
   helper.MakeInputFloat(data);

   data.defaultValue.BOOL() = false;
   data.name = "overrideNodes";
   data.shortName = "override_nodes";
   helper.MakeInputBoolean(data);
      
   data.defaultValue.BOOL() = false;
   data.name = "overrideReceiveShadows";
   data.shortName = "overrideReceiveShadows";
   helper.MakeInputBoolean(data);
   
   
   data.defaultValue.BOOL() = false;
   data.name = "overrideDoubleSided";
   data.shortName = "overrideDoubleSided";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "overrideSelfShadows";
   data.shortName = "overrideSelfShadows";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "overrideOpaque";
   data.shortName = "overrideOpaque";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "overrideMatte";
   data.shortName = "overrideMatte";
   helper.MakeInputBoolean(data);

}






