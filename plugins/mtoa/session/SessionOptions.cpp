#include "SessionOptions.h"

#include <ai_msg.h>

#include "maya/MFnDependencyNode.h"
#include "maya/MPlug.h"

MStatus CSessionOptions::GetFromMaya()
{
   MStatus status;

   MObject node = GetArnoldRenderOptions();
   if (node != MObject::kNullObj)
   {
      MFnDependencyNode fnArnoldRenderOptions(node);
   
      // Light linking options
      MPlug plug;
      plug = fnArnoldRenderOptions.findPlug("llnk");
      if (!plug.isNull())
      {
         m_lightlink = ArnoldLightLinkMode(plug.asInt());
      }
      plug = fnArnoldRenderOptions.findPlug("slnk");
      if (!plug.isNull())
      {
         m_shadowlink = ArnoldShadowLinkMode(plug.asInt());
      }

      // Motion blur options
      if (fnArnoldRenderOptions.findPlug("mb_en").asBool())
      {
         m_motion.enable_mask   = fnArnoldRenderOptions.findPlug("mb_len").asBool() * MTOA_MBLUR_LIGHT
                                + fnArnoldRenderOptions.findPlug("mb_cen").asBool() * MTOA_MBLUR_CAMERA
                                + fnArnoldRenderOptions.findPlug("mb_oen").asBool() * MTOA_MBLUR_OBJECT
                                + fnArnoldRenderOptions.findPlug("mb_den").asBool() * MTOA_MBLUR_DEFORM
                                + fnArnoldRenderOptions.findPlug("mb_sen").asBool() * MTOA_MBLUR_SHADER;
      }
      else
      {
         m_motion.enable_mask     = MTOA_MBLUR_DISABLE;
      }
      if (m_motion.enable_mask)
      {
         m_motion.shutter_size    = fnArnoldRenderOptions.findPlug("shutter_size").asFloat();
         m_motion.shutter_offset  = fnArnoldRenderOptions.findPlug("shutter_offset").asFloat();
         m_motion.shutter_type    = fnArnoldRenderOptions.findPlug("shutter_type").asInt();
         m_motion.by_frame        = fnArnoldRenderOptions.findPlug("motion_frames").asFloat();
         m_motion.steps           = fnArnoldRenderOptions.findPlug("motion_steps").asInt();
      }
      else
      {
         m_motion.by_frame        = 0;
         m_motion.steps           = 1;
      }

      status = MStatus::kSuccess;
   }
   else
   {
      AiMsgError("[mtoa] No known Arnold render options");
      status = MStatus::kFailure;
   }

   return status;
}
