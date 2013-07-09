#include "platform/Platform.h"
#include "SessionOptions.h"

#include <ai_msg.h>

#include "maya/MFnDependencyNode.h"
#include "maya/MPlug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
         m_lightlink = ArnoldLightLinkMode(plug.asInt());

      plug = fnArnoldRenderOptions.findPlug("slnk");
      if (!plug.isNull())
         m_shadowlink = ArnoldShadowLinkMode(plug.asInt());

      // Motion blur options
      if (fnArnoldRenderOptions.findPlug("mb_en").asBool())
      {
         m_motion.enable_mask   = (fnArnoldRenderOptions.findPlug("mb_en").asBool() * MTOA_MBLUR_LIGHT)
                                | (fnArnoldRenderOptions.findPlug("mb_en").asBool() * MTOA_MBLUR_CAMERA)
                                | (fnArnoldRenderOptions.findPlug("mb_en").asBool() * MTOA_MBLUR_OBJECT)
                                | (fnArnoldRenderOptions.findPlug("mb_den").asBool() * MTOA_MBLUR_DEFORM)
                                | (fnArnoldRenderOptions.findPlug("mb_en").asBool() * MTOA_MBLUR_SHADER);
      }
      else
         m_motion.enable_mask     = MTOA_MBLUR_DISABLE;

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

      plug = fnArnoldRenderOptions.findPlug("relative_texture_paths");
      if (!plug.isNull())
         m_relativeTexturePaths = plug.asBool();

      plug = fnArnoldRenderOptions.findPlug("texture_searchpath");
      if (!plug.isNull())
         plug.asString().split(PATHSEP, m_textureSearchPaths);
      else
         m_textureSearchPaths.clear();

      status = MStatus::kSuccess;
   }
   else
   {
      AiMsgError("[mtoa] No known Arnold render options");
      status = MStatus::kFailure;
   }

   return status;
}

void CSessionOptions::FormatTexturePath(MString& texturePath) const
{
   if (!m_relativeTexturePaths)
      return;
   for (unsigned int i = 0; i < m_textureSearchPaths.length(); ++i)
   {
      const MString& currentSearchPath = m_textureSearchPaths[i];
      if (texturePath.indexW(currentSearchPath) == 0)
      {
         texturePath = texturePath.substringW(currentSearchPath.length(), texturePath.length());
         return;
      }
   }
}
