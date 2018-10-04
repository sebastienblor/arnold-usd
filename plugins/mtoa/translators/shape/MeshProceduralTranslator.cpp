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
   data.keyable = false;
   helper.MakeInputString(data);

   data.name = "data";
   data.shortName = "data";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputString(data);

   data.defaultValue.BOOL() = true;
   data.name = "aiOverrideLightLinking";
   data.shortName = "ai_override_light_linking";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL() = true;
   data.name = "aiOverrideShaders";
   data.shortName = "ai_override_shaders";
   data.channelBox = false;
   //data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiUseFrameExtension";
   data.shortName = "ai_use_frame_extension";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.INT() = 0;
   data.name = "aiFrameNumber";
   data.shortName = "ai_frame_number";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputInt(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiUseSubFrame";
   data.shortName = "ai_use_sub_frame";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.FLT() = 0.f;
   data.name = "aiFrameOffset";
   data.shortName = "ai_frame_offset";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputFloat(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiOverrideNodes";
   data.shortName = "ai_override_nodes";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.name = "aiNamespace";
   data.shortName = "ai_namespace";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputString(data);
      
   data.defaultValue.BOOL() = false;
   data.name = "aiOverrideReceiveShadows";
   data.shortName = "ai_override_receive_shadows";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);
      
   data.defaultValue.BOOL() = false;
   data.name = "aiOverrideDoubleSided";
   data.shortName = "ai_override_double_sided";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiOverrideSelfShadows";
   data.shortName = "ai_override_self_shadows";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiOverrideOpaque";
   data.shortName = "ai_override_opaque";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiOverrideMatte";
   data.shortName = "ai_override_matte";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

}






