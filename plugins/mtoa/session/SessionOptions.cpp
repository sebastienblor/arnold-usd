#include "platform/Platform.h"
#include "utils/MayaUtils.h"
#include "SessionOptions.h"

#include <ai_msg.h>

#include "maya/MFnDependencyNode.h"
#include "maya/MPlug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void ReplaceSlashes(MString& str, bool isDir = false)
{
#ifdef _WIN32
   MStringArray tmp;
   str.split('\\', tmp);
   if (tmp.length() > 1)
   {
      str = tmp[0];
      for (unsigned int i = 1; i < tmp.length(); ++i)
      {
         str += "/";
         str += tmp[i];
      }
   }
#endif
   if (isDir && (str.length() > 1) && (str.substring(str.length() - 1, str.length() - 1) != "/"))
      str += "/";
}

void ExpandEnvVariables(MString& str)
{
   std::string str2 = str.asChar();
   size_t token;
   while ((token = str2.find("]")) != std::string::npos)
      str2.replace(token, token + 1, "}");
   while ((token = str2.find("[")) != std::string::npos)
      str2.replace(token, token + 1, "${");
   str = str2.c_str();
   str = str.expandEnvironmentVariablesAndTilde();
}

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
         m_motion.steps           = fnArnoldRenderOptions.findPlug("motion_steps").asInt();
         m_motion.range_type      = fnArnoldRenderOptions.findPlug("range_type").asInt();
         m_motion.motion_frames   = fnArnoldRenderOptions.findPlug("motion_frames").asFloat();
         m_motion.motion_start    = fnArnoldRenderOptions.findPlug("motion_start").asFloat();
         m_motion.motion_end      = fnArnoldRenderOptions.findPlug("motion_end").asFloat();
      }
      else
      {
         m_motion.steps           = 1;
         m_motion.range_type      = MTOA_MBLUR_TYPE_START;
         m_motion.motion_frames   = 0;
      }

      plug = fnArnoldRenderOptions.findPlug("absolute_texture_paths");
      if (!plug.isNull())
         m_absoluteTexturePaths = plug.asBool();
      else
         m_absoluteTexturePaths = true;

      plug = fnArnoldRenderOptions.findPlug("texture_searchpath");
      if (!plug.isNull())
      {
         plug.asString().split(PATHSEP, m_textureSearchPaths);
         MStringArray arr = getSourceImagesPath();
         for (unsigned int i = 0; i < arr.length(); ++i)
            m_textureSearchPaths.append(arr[i]);
         for (unsigned int i = 0; i < m_textureSearchPaths.length(); ++i)
         {
            ExpandEnvVariables(m_textureSearchPaths[i]);
            ReplaceSlashes(m_textureSearchPaths[i], true);            
         }
      }
      else
         m_textureSearchPaths.clear();

      plug = fnArnoldRenderOptions.findPlug("absolute_procedural_paths");
      if (!plug.isNull())
         m_absoluteProceduralPaths = plug.asBool();
      else
         m_absoluteProceduralPaths = true;

      plug = fnArnoldRenderOptions.findPlug("procedural_searchpath");
      if (!plug.isNull())
      {         
         plug.asString().split(PATHSEP, m_proceduralSearchPaths);
         m_proceduralSearchPaths.append(getProjectFolderPath());
         for (unsigned int i = 0; i < m_proceduralSearchPaths.length(); ++i)
         {
            ExpandEnvVariables(m_proceduralSearchPaths[i]);
            ReplaceSlashes(m_proceduralSearchPaths[i], true);            
         }
      }
      else
         m_proceduralSearchPaths.clear();

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
   ReplaceSlashes(texturePath);
   if (m_absoluteTexturePaths)
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

void CSessionOptions::FormatProceduralPath(MString& proceduralPath) const
{
   ReplaceSlashes(proceduralPath);
   if (m_absoluteProceduralPaths)
      return;
   for (unsigned int i = 0; i < m_proceduralSearchPaths.length(); ++i)
   {
      const MString& currentSearchPath = m_proceduralSearchPaths[i];
      if (proceduralPath.indexW(currentSearchPath) == 0)
      {
         proceduralPath = proceduralPath.substringW(currentSearchPath.length(), proceduralPath.length());
         return;
      }
   }
}
