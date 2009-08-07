
#include "MayaScene.h"

#include <ai_msg.h>
#include <ai_nodes.h>

#include <maya/MColor.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnStringData.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

namespace // <anonymous>
{
   void ProcessShaderParameter(MFnDependencyNode shader, const char* param, AtNode* arnoldShader, const char* arnoldAttrib, int arnoldAttribType)
   {
      MPlugArray connections;

      MPlug plug = shader.findPlug(param);

      plug.connectedTo(connections, true, false);

      if (connections.length() == 0)
      {
         switch(arnoldAttribType)
         {
         case AI_TYPE_RGB:
            {
               AiNodeSetRGB(arnoldShader, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
            }
            break;
         case AI_TYPE_FLOAT:
            {
               AiNodeSetFlt(arnoldShader, arnoldAttrib, plug.asFloat());
            }
            break;
         case AI_TYPE_BOOLEAN:
            {
               AiNodeSetBool(arnoldShader, arnoldAttrib, plug.asBool());
            }
            break;
         case AI_TYPE_ENUM:
            {
               AiNodeSetInt(arnoldShader, arnoldAttrib, plug.asInt());
            }
            break;
         case AI_TYPE_INT:
            {
               AiNodeSetInt(arnoldShader, arnoldAttrib, plug.asInt());
            }
            break;
         case AI_TYPE_STRING:
            {
               AiNodeSetStr(arnoldShader, arnoldAttrib, plug.asString().asChar());
            }
            break;
         case AI_TYPE_VECTOR:
            {
               AiNodeSetVec(arnoldShader, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
            }
            break;
         case AI_TYPE_POINT:
            {
               AiNodeSetPnt(arnoldShader, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
            }
            break;
         }
      }
      else
      {
         // TODO: Implement this properly, with recursive export of connected nodes.

         MObject fileName;
         MPlug   filePlug;
         MFnDependencyNode fnDGNode; 

         fnDGNode.setObject(connections[0].node());

         filePlug = fnDGNode.findPlug("fileTextureName");

         filePlug.getValue(fileName);

         MFnStringData fileNameString(fileName);

         AtNode* image = AiNode("image");

         AiNodeSetStr(image, "image_map", fileNameString.string().asChar());
         AiNodeLink(image, arnoldAttrib, arnoldShader);
      }

   }  // ProcessColorAttrib()

   // This is a shortcut for Arnold shaders (parameter name is the same in Maya and Arnold)
   #define SHADER_PARAM(name, type) ProcessShaderParameter(mayaShader, name, shader, name, type)

   AtNode* ExportArnoldShader(MObject mayaShader)
   {
      AtNode* shader = NULL;
      MFnDependencyNode mayaNode(mayaShader);

      if (!strcmp(mayaNode.typeName().asChar(), "ArnoldStandardShader"))
      {
         shader = AiNode("standard");

         AiNodeSetStr(shader, "name", mayaNode.name().asChar());

         SHADER_PARAM("Fresnel", AI_TYPE_BOOLEAN);
         SHADER_PARAM("Fresnel_affect_diff", AI_TYPE_BOOLEAN);
         SHADER_PARAM("IOR", AI_TYPE_FLOAT);
         SHADER_PARAM("Katt", AI_TYPE_RGB);
         SHADER_PARAM("Kb", AI_TYPE_FLOAT);
         SHADER_PARAM("Kd", AI_TYPE_FLOAT);
         SHADER_PARAM("Kd_color", AI_TYPE_RGB);
         SHADER_PARAM("Kr", AI_TYPE_FLOAT);
         SHADER_PARAM("Kr_color", AI_TYPE_RGB);
         SHADER_PARAM("Krn", AI_TYPE_FLOAT);
         SHADER_PARAM("Ks", AI_TYPE_FLOAT);
         SHADER_PARAM("Ks_color", AI_TYPE_RGB);
         SHADER_PARAM("Ksn", AI_TYPE_FLOAT);
         SHADER_PARAM("Ksss", AI_TYPE_FLOAT);
         SHADER_PARAM("Ksss_color", AI_TYPE_RGB);
         SHADER_PARAM("Kt", AI_TYPE_FLOAT);
         SHADER_PARAM("Phong_exponent", AI_TYPE_FLOAT);
         SHADER_PARAM("bounce_factor", AI_TYPE_FLOAT);
         SHADER_PARAM("caustics", AI_TYPE_BOOLEAN);
         SHADER_PARAM("direct_diffuse", AI_TYPE_FLOAT);
         SHADER_PARAM("direct_specular", AI_TYPE_FLOAT);
         SHADER_PARAM("emission", AI_TYPE_FLOAT);
         SHADER_PARAM("emission_color", AI_TYPE_RGB);
         SHADER_PARAM("indirect_diffuse", AI_TYPE_FLOAT);
         SHADER_PARAM("indirect_specular", AI_TYPE_FLOAT);
         SHADER_PARAM("opacity", AI_TYPE_RGB);
         SHADER_PARAM("retro_reflector", AI_TYPE_BOOLEAN);
         SHADER_PARAM("specular_Fresnel", AI_TYPE_BOOLEAN);
         SHADER_PARAM("sss_radius", AI_TYPE_FLOAT);
      }
      else if (!strcmp(mayaNode.typeName().asChar(), "ArnoldUtilityShader"))
      {
         shader = AiNode("utility");

         AiNodeSetStr(shader, "name", mayaNode.name().asChar());

         SHADER_PARAM("color", AI_TYPE_RGB);
         SHADER_PARAM("color_mode", AI_TYPE_ENUM);
         SHADER_PARAM("shade_mode", AI_TYPE_ENUM);
      }
      else if (!strcmp(mayaNode.typeName().asChar(), "ArnoldAmbientOcclusionShader"))
      {
         shader = AiNode("ambient_occlusion");

         AiNodeSetStr(shader, "name", mayaNode.name().asChar());

         SHADER_PARAM("black", AI_TYPE_RGB);
         SHADER_PARAM("falloff", AI_TYPE_FLOAT);
         SHADER_PARAM("far_clip", AI_TYPE_FLOAT);
         SHADER_PARAM("near_clip", AI_TYPE_FLOAT);
         SHADER_PARAM("opacity", AI_TYPE_RGB);
         SHADER_PARAM("samples", AI_TYPE_INT);
         SHADER_PARAM("spread", AI_TYPE_FLOAT);
         SHADER_PARAM("white", AI_TYPE_RGB);
      }
      else if (!strcmp(mayaNode.typeName().asChar(), "ArnoldBackgroundImageShader"))
      {
         shader = AiNode("background_image");

         AiNodeSetStr(shader, "name", mayaNode.name().asChar());

         SHADER_PARAM("filename", AI_TYPE_STRING);
      }
      else if (!strcmp(mayaNode.typeName().asChar(), "ArnoldSkyShader"))
      {
         shader = AiNode("sky");

         AiNodeSetStr(shader, "name", mayaNode.name().asChar());

         if (mayaNode.findPlug("separate_colors").asBool())
         {
            SHADER_PARAM("emission_color", AI_TYPE_RGB);
            SHADER_PARAM("emission_intensity", AI_TYPE_FLOAT);
         }
         else
         {
            MPlug plug = mayaNode.findPlug("color");
            AiNodeSetRGB(shader, "emission_color", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
            AiNodeSetFlt(shader, "emission_intensity", mayaNode.findPlug("intensity").asFloat());
         }

         SHADER_PARAM("color", AI_TYPE_RGB);
         SHADER_PARAM("intensity", AI_TYPE_FLOAT);
         SHADER_PARAM("opaque_alpha", AI_TYPE_BOOLEAN);
         SHADER_PARAM("visible", AI_TYPE_BOOLEAN);
      }
      else if (!strcmp(mayaNode.typeName().asChar(), "ArnoldSky_HDRIShader"))
      {
         shader = AiNode("sky_HDRI");

         AiNodeSetStr(shader, "name", mayaNode.name().asChar());

         SHADER_PARAM("HDRI_map", AI_TYPE_STRING);
         SHADER_PARAM("X", AI_TYPE_VECTOR);
         SHADER_PARAM("Y", AI_TYPE_VECTOR);
         SHADER_PARAM("Z", AI_TYPE_VECTOR);
         SHADER_PARAM("X_angle", AI_TYPE_FLOAT);
         SHADER_PARAM("Y_angle", AI_TYPE_FLOAT);
         SHADER_PARAM("Z_angle", AI_TYPE_FLOAT);
         SHADER_PARAM("flip_X", AI_TYPE_BOOLEAN);
         SHADER_PARAM("flip_Y", AI_TYPE_BOOLEAN);
         SHADER_PARAM("flip_Z", AI_TYPE_BOOLEAN);
         SHADER_PARAM("format", AI_TYPE_ENUM);
         SHADER_PARAM("multiplier", AI_TYPE_FLOAT);
         SHADER_PARAM("opaque_alpha", AI_TYPE_BOOLEAN);
         SHADER_PARAM("rgb_multiplier", AI_TYPE_RGB);
         SHADER_PARAM("visibility", AI_TYPE_INT);
      }
      else if (!strcmp(mayaNode.typeName().asChar(), "ArnoldFogShader"))
      {
         shader = AiNode("fog");

         AiNodeSetStr(shader, "name", mayaNode.name().asChar());

         SHADER_PARAM("color", AI_TYPE_RGB);
         SHADER_PARAM("distance", AI_TYPE_FLOAT);
         SHADER_PARAM("ground_normal", AI_TYPE_VECTOR);
         SHADER_PARAM("ground_point", AI_TYPE_POINT);
         SHADER_PARAM("height", AI_TYPE_FLOAT);
      }
      else if (!strcmp(mayaNode.typeName().asChar(), "ArnoldVolumeScatteringShader"))
      {
         shader = AiNode("volume_scattering");

         AiNodeSetStr(shader, "name", mayaNode.name().asChar());

         SHADER_PARAM("affect_diffuse", AI_TYPE_FLOAT);
         SHADER_PARAM("affect_reflection", AI_TYPE_FLOAT);
         SHADER_PARAM("attenuation", AI_TYPE_FLOAT);
         SHADER_PARAM("density", AI_TYPE_FLOAT);
         SHADER_PARAM("eccentricity", AI_TYPE_FLOAT);
         SHADER_PARAM("importance_sampling", AI_TYPE_BOOLEAN);
         SHADER_PARAM("mscattering_depth", AI_TYPE_INT);
         SHADER_PARAM("mscattering_samples", AI_TYPE_INT);
         SHADER_PARAM("phase_function", AI_TYPE_ENUM);
         SHADER_PARAM("rgb_attenuation", AI_TYPE_RGB);
         SHADER_PARAM("rgb_density", AI_TYPE_RGB);
         SHADER_PARAM("samples", AI_TYPE_INT);
         SHADER_PARAM("sampling_pattern", AI_TYPE_ENUM);
      }
      else if (!strcmp(mayaNode.typeName().asChar(), "ArnoldWireframeShader"))
      {
         shader = AiNode("wireframe");

         AiNodeSetStr(shader, "name", mayaNode.name().asChar());

         SHADER_PARAM("edge_type", AI_TYPE_ENUM);
         SHADER_PARAM("fill_color", AI_TYPE_RGB);
         SHADER_PARAM("line_color", AI_TYPE_RGB);
         SHADER_PARAM("line_width", AI_TYPE_FLOAT);
         SHADER_PARAM("raster_space", AI_TYPE_BOOLEAN);
      }
      else if (!strcmp(mayaNode.typeName().asChar(), "ArnoldBSSRDFShader"))
      {
         shader = AiNode("bssrdf");

         AiNodeSetStr(shader, "name", mayaNode.name().asChar());

         SHADER_PARAM("color", AI_TYPE_ENUM);
         SHADER_PARAM("radius", AI_TYPE_RGB);
      }
      else
      {
         AiMsgWarning("[mtoa] Shader type not supported.");
      }

      return shader;

   } // ExportArnoldShader()

}  // namespace <anonymous>


MObject CMayaScene::GetNodeShader(MObject dagNode)
{
   MPlugArray        connections;
   MFnDependencyNode fnDGNode(dagNode);

   // Find the "instObjGroups" attribute. Follow that plug to see where it is connected.
   MPlug plug(dagNode, fnDGNode.attribute("instObjGroups"));

   plug.elementByLogicalIndex(0).connectedTo(connections, false, true);

   if (connections.length() != 1)
   {
      return MObject::kNullObj;
   }

   MObject shadingGroup(connections[0].node());

   fnDGNode.setObject(shadingGroup);

   MPlug shaderPlug(shadingGroup, fnDGNode.attribute("surfaceShader"));

   connections.clear();

   shaderPlug.connectedTo(connections, true, false);

   return connections[0].node();

}  // GetNodeShader()


AtNode* CMayaScene::ExportShader(MObject mayaShader)
{
   // First check if this shader has already been processed
   for (std::vector<CShaderData>::const_iterator it = m_processedShaders.begin(); (it != m_processedShaders.end()); ++it)
   {
      if (it->mayaShader == mayaShader)
      {
         return it->arnoldShader;
      }
   }

   AtNode* shader = NULL;

   MFnDependencyNode node(mayaShader);

   static char buffer[128];
   sprintf(buffer, "[mtoa] Exporting shader: %s", node.name().asChar());
   AiMsgDebug(buffer);

   switch (mayaShader.apiType())
   {
   case MFn::kLambert:
      {
         MFnLambertShader fnShader(mayaShader);

         shader = AiNode("lambert");
      
         AiNodeSetStr(shader, "name", fnShader.name().asChar());

         ProcessShaderParameter(node, "diffuse", shader, "Kd", AI_TYPE_FLOAT);
         ProcessShaderParameter(node, "color", shader, "Kd_color", AI_TYPE_RGB);
         ProcessShaderParameter(node, "outMatteOpacity", shader, "opacity", AI_TYPE_RGB);
      }
      break;

   case MFn::kPhong:
      {
      }
      break;

   case MFn::kBlinn:
      {
      }
      break;

   case MFn::kSurfaceShader:
      {
         shader = AiNode("flat");
      
         AiNodeSetStr(shader, "name", node.name().asChar());

         ProcessShaderParameter(node, "outColor", shader, "color", AI_TYPE_RGB);
         ProcessShaderParameter(node, "outMatteOpacity", shader, "opacity", AI_TYPE_RGB);
      }
      break;

   case MFn::kPluginDependNode:
      {
         shader = ExportArnoldShader(mayaShader);
      }
      break;

   default:
      {
         AiMsgWarning("[mtoa] Shader type not supported.");
      }
   }

   if (shader)
   {
      CShaderData   data;

      data.mayaShader   = mayaShader;
      data.arnoldShader = shader;

      m_processedShaders.push_back(data);
   }

   return shader;

}  // ExportShader()
