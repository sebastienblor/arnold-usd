#include "platform/Platform.h"
#include "utils/MayaUtils.h"
#include "SessionOptions.h"

#include <ai_msg.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MDistance.h>
#include <maya/MPlugArray.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace {
   enum ArnoldRenderUnit {
      RU_USE_MAYA_UNIT = 0,
      RU_USE_CUSTOM_SCALING,
      RU_INCH,
      RU_FEET,
      RU_YARD,
      RU_MILE,
      RU_MILLIMETER,
      RU_CENTIMETER,
      RU_KILOMETER,
      RU_METER
   };
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
      plug = fnArnoldRenderOptions.findPlug("llnk", true);
      if (!plug.isNull())
         m_lightlink = ArnoldLightLinkMode(plug.asInt());

      plug = fnArnoldRenderOptions.findPlug("slnk", true);
      if (!plug.isNull())
         m_shadowlink = ArnoldShadowLinkMode(plug.asInt());

      // Motion blur options
      // disable motion for material viewer and swatch rendering
      if (m_mode != MTOA_SESSION_MATERIALVIEW && m_mode != MTOA_SESSION_SWATCH && fnArnoldRenderOptions.findPlug("mb_en", true).asBool())
      {
         m_motion.enable_mask   = (fnArnoldRenderOptions.findPlug("mb_en", true).asBool() * MTOA_MBLUR_LIGHT)
                                | (fnArnoldRenderOptions.findPlug("mb_cen", true).asBool() * MTOA_MBLUR_CAMERA)
                                | (fnArnoldRenderOptions.findPlug("mb_en", true).asBool() * MTOA_MBLUR_OBJECT)
                                | (fnArnoldRenderOptions.findPlug("mb_den", true).asBool() * MTOA_MBLUR_DEFORM)
                                | (fnArnoldRenderOptions.findPlug("mb_sen", true).asBool() * MTOA_MBLUR_SHADER);
      }
      else
         m_motion.enable_mask     = MTOA_MBLUR_DISABLE;

      if (m_motion.enable_mask)
      {
         m_motion.steps           = fnArnoldRenderOptions.findPlug("motion_steps", true).asInt();
         m_motion.range_type      = fnArnoldRenderOptions.findPlug("range_type", true).asInt();
         m_motion.motion_frames   = fnArnoldRenderOptions.findPlug("motion_frames", true).asFloat();
         m_motion.motion_start    = fnArnoldRenderOptions.findPlug("motion_start", true).asFloat();
         m_motion.motion_end      = fnArnoldRenderOptions.findPlug("motion_end", true).asFloat();
      }
      else
      {
         m_motion.steps           = 1;
         m_motion.range_type      = MTOA_MBLUR_TYPE_START;
         m_motion.motion_frames   = 0;
      }

      plug = fnArnoldRenderOptions.findPlug("absolute_texture_paths", true);
      if (!plug.isNull())
         m_absoluteTexturePaths = plug.asBool();
      else
         m_absoluteTexturePaths = true;

      plug = fnArnoldRenderOptions.findPlug("exportAllShadingGroups", true);
      if (!plug.isNull())
         m_exportAllShadingGroups = plug.asBool();
      else
         m_exportAllShadingGroups = false;

      plug = fnArnoldRenderOptions.findPlug("exportFullPaths", true);
      if (!plug.isNull())
         m_exportFullPath = plug.asBool();

      plug = fnArnoldRenderOptions.findPlug("exportSeparator", true);
      if (!plug.isNull())
         m_exportSlashSeparator = (plug.asInt() == MTOA_EXPORT_SEPARATOR_SLASHES);

      plug = fnArnoldRenderOptions.findPlug("exportNamespace", true);
      if (!plug.isNull())
         m_exportNamespace = plug.asInt();

      plug = fnArnoldRenderOptions.findPlug("exportDagName", true);
      if (!plug.isNull())
         m_exportDagTransformNames = (plug.asInt() == MTOA_EXPORT_DAG_TRANSFORM);

      plug = fnArnoldRenderOptions.findPlug("texture_searchpath", true);
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

      plug = fnArnoldRenderOptions.findPlug("absolute_procedural_paths", true);
      if (!plug.isNull())
         m_absoluteProceduralPaths = plug.asBool();
      else
         m_absoluteProceduralPaths = true;

      plug = fnArnoldRenderOptions.findPlug("procedural_searchpath",true);
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

      const short renderUnit = fnArnoldRenderOptions.findPlug("renderUnit", true).asShort();

      MDistance dist(1.0, MDistance::uiUnit());

      switch (renderUnit)
      {
         case RU_USE_MAYA_UNIT:
            m_scaleFactor = 1.0;
            break;
         case RU_USE_CUSTOM_SCALING:
            m_scaleFactor = fnArnoldRenderOptions.findPlug("sceneScale", true).asDouble();
            break;
         case RU_INCH:
            m_scaleFactor = dist.asInches();
            break;
         case RU_FEET:
            m_scaleFactor = dist.asFeet();
            break;
         case RU_YARD:
            m_scaleFactor = dist.asYards();
            break;
         case RU_MILE:
            m_scaleFactor = dist.asMiles();
            break;
         case RU_MILLIMETER:
            m_scaleFactor = dist.asMillimeters();
            break;
         case RU_CENTIMETER:
            m_scaleFactor = dist.asCentimeters();
            break;
         case RU_KILOMETER:
            m_scaleFactor = dist.asKilometers();
            break;
         case RU_METER:
            m_scaleFactor = dist.asMeters();
            break;
         default:
            m_scaleFactor = 1.0;
      }

      

      m_origin = MVector(0.0, 0.0, 0.0);

      if (fnArnoldRenderOptions.findPlug("offsetOrigin", true).asBool())
      {
         MPlugArray conns;
         if (fnArnoldRenderOptions.findPlug("origin", true).connectedTo(conns, true, false, &status))
         {
            if (status && (conns.length() > 0))
            {
               MDagPath dgPath;
               if (MDagPath::getAPathTo(conns[0].node(), dgPath))
               {
                  MTransformationMatrix tMat = dgPath.inclusiveMatrix();
                  m_origin = tMat.getTranslation(MSpace::kWorld);
               }
            }
         }
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
