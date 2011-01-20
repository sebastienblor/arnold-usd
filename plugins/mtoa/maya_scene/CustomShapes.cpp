#include "MayaScene.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

#include <maya/MDagPath.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MGlobal.h>
#include <maya/MBoundingBox.h>

static bool StringInList(const MString &str, const MStringArray &ary)
{
   for (unsigned int i = 0; i < ary.length(); ++i)
   {
      if (ary[i] == str)
      {
         return true;
      }
   }
   return false;
}


// Get shading engine associated with a custom shape
//
void CMayaScene::GetCustomShapeInstanceShader(const MDagPath &path, MFnDependencyNode &shadingEngineNode)
{
   // Get instance shadingEngine
   shadingEngineNode.setObject(MObject::kNullObj);

   char buffer[64];

   MStringArray connections;
   MGlobal::executeCommand("listConnections -s 1 -d 0 -c 1 -type shadingEngine "+path.fullPathName(), connections);

   MSelectionList sl;

   if (connections.length() == 2)
   {
      sl.add(connections[1]);
   }
   else if (connections.length() > 2)
   {
      sprintf(buffer, "[%d]", path.instanceNumber());
      MString iidx = buffer;

      for (unsigned int cidx = 0; cidx < connections.length(); cidx += 2)
      {
         MString conn = connections[cidx];

         if (conn.length() < iidx.length())
         {
            continue;
         }

         if (conn.substring(conn.length() - iidx.length(), conn.length() - 1) != iidx)
         {
            continue;
         }

         sl.add(connections[cidx+1]);
         break;
      }
   }

   if (sl.length() == 1)
   {
      MObject shadingEngineObj;
      sl.getDependNode(0, shadingEngineObj);

      shadingEngineNode.setObject(shadingEngineObj);
   }
}

bool CMayaScene::RegisterCustomShape(std::string &shapeType)
{
   static const char whitespaces[] = " \t\n\v";

   // strip leading and trailing spaces
   size_t w0 = shapeType.find_first_not_of(whitespaces);

   if (w0 != std::string::npos)
   {
      size_t w1 = shapeType.find_last_not_of(whitespaces);

      shapeType = shapeType.substr(w0, w1-w0+1);

      if (shapeType.length() > 0)
      {
         // check if shapeType already registered
         std::map<std::string, CCustomData>::iterator it = m_customShapes.find(shapeType);

         if (it == m_customShapes.end())
         {
            // check if export command can be found
            std::string scriptName = "mtoa_export_" + shapeType;

            MString rv = MGlobal::executeCommandStringResult("whatIs " + MString(scriptName.c_str()));

            if (rv != "Unknown")
            {
               m_customShapes[shapeType].exportCmd = scriptName.c_str();

               std::string scriptName = "mtoa_cleanup_" + shapeType;

               rv = MGlobal::executeCommandStringResult("whatIs " + MString(scriptName.c_str()));

               if (rv == "Unknown")
               {
                  scriptName = "";
               }

               m_customShapes[shapeType].cleanupCmd = scriptName.c_str();

               return true;
            }
         }
      }
   }

   return false;
}

void CMayaScene::ExportCustomShape(const MDagPath &dagPath, AtUInt step, const MString &exportCmd, const MString &cleanupCmd)
{
   bool isMasterDag = false;

   MFnDagNode node(dagPath.node());

   bool transformBlur = m_motionBlurData.enabled &&
                        m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool() &&
                        node.findPlug("motionBlur").asBool();

   bool deformBlur = m_motionBlurData.enabled &&
                     m_fnArnoldRenderOptions->findPlug("mb_object_deform_enable").asBool() &&
                     node.findPlug("motionBlur").asBool();

   char buffer[64];

   MString command = exportCmd;
   command += "(";

   sprintf(buffer, "%f", m_currentFrame);
   command += buffer;
   command += ", ";

   sprintf(buffer, "%d", step);
   command += buffer;
   command += ", ";

   sprintf(buffer, "%f", (step < m_motionBlurData.frames.size() ? m_motionBlurData.frames[step] : m_currentFrame));
   command += buffer;
   command += ", ";

   // List of arnold attributes the custom shape export command has overriden
   MStringArray attrs;

   MDagPath masterDag;
   if (IsMasterInstance(dagPath, masterDag))
   {
      command += "\"" + dagPath.fullPathName() + "\", 0, \"\")";
      isMasterDag = true;
      // If IsMasterInstance return true, masterDag won't get set
      masterDag = dagPath;
   }
   else
   {
      command += "\"" + dagPath.fullPathName() + "\", 1, \"" + masterDag.fullPathName() + "\")";
   }

   MStatus status = MGlobal::executeCommand(command, attrs);
   if (!status)
   {
      AiMsgError("[mtoa] ERROR: Failed to export custom shape \"%s\".", node.name().asChar());
      return;
   }


   MFnDependencyNode masterShadingEngine;
   MFnDependencyNode shadingEngine;
   float dispHeight = 0.0f;

   // Figure out displacement height
   // -> suppose disp_map has its value in [0, 1] range and disp_height is constant)
   // -> all instances share the same displacement shader, the one of the master dag
   // NOTE: this might not work properly if the master shape overrides the disp_height
   //       and instances do not use the ginstance but another procedural node

   GetCustomShapeInstanceShader(masterDag, masterShadingEngine);

   if (masterShadingEngine.object() != MObject::kNullObj)
   {
      MPlug pDispShader = masterShadingEngine.findPlug("displacementShader");
      if (!pDispShader.isNull())
      {
         MPlugArray conns;

         pDispShader.connectedTo(conns, true, false);

         if (conns.length() > 0)
         {
            MFnDependencyNode dispNode(conns[0].node());

            MPlug pDispHeight = dispNode.findPlug("disp_height");

            if (!pDispHeight.isNull())
            {
               dispHeight = pDispHeight.asFloat();
            }
         }
      }
   }

   AtNode *anode = AiNodeLookUpByName(dagPath.fullPathName().asChar());

   if (anode)
   {
      GetCustomShapeInstanceShader(dagPath, shadingEngine);

      AtMatrix matrix;
      MMatrix mmatrix = dagPath.inclusiveMatrix();
      ConvertMatrix(matrix, mmatrix);

      // Set transformation matrix
      if (transformBlur && !StringInList("matrix", attrs))
      {
         if (step == 0)
         {
            AtArray* matrices = AiArrayAllocate(1, m_motionBlurData.motion_steps, AI_TYPE_MATRIX);
            AiArraySetMtx(matrices, step, matrix);
            AiNodeSetArray(anode, "matrix", matrices);
         }
         else
         {
            AtArray* matrices = AiNodeGetArray(anode, "matrix");
            AiArraySetMtx(matrices, step, matrix);
         }
      }
      else
      {
         AiNodeSetMatrix(anode, "matrix", matrix);
      }

      // Set bounding box (in local space)
      if (!StringInList("min", attrs) && !StringInList("max", attrs))
      {
         if (step == 0)
         {
            // set for the first time
            MBoundingBox bbox = node.boundingBox();

            MPoint bmin = bbox.min(); // * mmatrix;
            MPoint bmax = bbox.max(); // * mmatrix;

            bmin.x -= dispHeight;
            bmin.y -= dispHeight;
            bmin.z -= dispHeight;

            bmax.x += dispHeight;
            bmax.y += dispHeight;
            bmax.z += dispHeight;

            AiNodeSetPnt(anode, "min", static_cast<float>(bmin.x), static_cast<float>(bmin.y), static_cast<float>(bmin.z));
            AiNodeSetPnt(anode, "max", static_cast<float>(bmax.x), static_cast<float>(bmax.y), static_cast<float>(bmax.z));
         }
         else
         {
            if (transformBlur || deformBlur)
            {
               AtPoint cmin = AiNodeGetPnt(anode, "min");
               AtPoint cmax = AiNodeGetPnt(anode, "max");

               MBoundingBox bbox = node.boundingBox();

               MPoint bmin = bbox.min(); // * mmatrix;
               MPoint bmax = bbox.max(); // * mmatrix;

               bmin.x -= dispHeight;
               bmin.y -= dispHeight;
               bmin.z -= dispHeight;

               bmax.x += dispHeight;
               bmax.y += dispHeight;
               bmax.z += dispHeight;

               if (bmin.x < cmin.x)
                  cmin.x = static_cast<float>(bmin.x);
               if (bmin.y < cmin.y)
                  cmin.y = static_cast<float>(bmin.y);
               if (bmin.z < cmin.z)
                  cmin.z = static_cast<float>(bmin.z);
               if (bmax.x > cmax.x)
                  cmax.x = static_cast<float>(bmax.x);
               if (bmax.y > cmax.y)
                  cmax.y = static_cast<float>(bmax.y);
               if (bmax.z > cmax.z)
                  cmax.z = static_cast<float>(bmax.z);

               AiNodeSetPnt(anode, "min", cmin.x, cmin.y, cmin.z);
               AiNodeSetPnt(anode, "max", cmax.x, cmax.y, cmax.z);
            }
         }
      }

      if (step == 0)
      {
         // Set common attributes
         MPlug plug;

         if (isMasterDag)
         {
            // For master node only if there are instances

            if (!StringInList("subdiv_type", attrs))
            {
               plug = node.findPlug("subdiv_type");
               if (!plug.isNull())
               {
                  if (AiNodeEntryLookUpParameter(anode->base_node, "subdiv_type") != 0 ||
                      AiNodeDeclare(anode, "subdiv_type", "constant INT"))
                  {
                     AiNodeSetInt(anode, "subdiv_type", plug.asInt());
                  }
               }
            }

            if (!StringInList("subdiv_iterations", attrs))
            {
               plug = node.findPlug("subdiv_iterations");
               if (!plug.isNull())
               {
                  if (AiNodeEntryLookUpParameter(anode->base_node, "subdiv_iterations") != 0 ||
                      AiNodeDeclare(anode, "subdiv_iterations", "constant INT"))
                  {
                     AiNodeSetInt(anode, "subdiv_iterations", plug.asInt());
                  }
               }
            }

            if (!StringInList("subdiv_adaptive_metric", attrs))
            {
               plug = node.findPlug("subdiv_adaptive_metric");
               if (!plug.isNull())
               {
                  if (AiNodeEntryLookUpParameter(anode->base_node, "subdiv_adaptive_metric") != 0 ||
                      AiNodeDeclare(anode, "subdiv_adaptive_metric", "constant INT"))
                  {
                     AiNodeSetInt(anode, "subdiv_adaptive_metric", plug.asInt());
                  }
               }
            }

            if (!StringInList("subdiv_pixel_error", attrs))
            {
               plug = node.findPlug("subdiv_pixel_error");
               if (!plug.isNull())
               {
                  if (AiNodeEntryLookUpParameter(anode->base_node, "subdiv_pixel_error") != 0 ||
                      AiNodeDeclare(anode, "subdiv_pixel_error", "constant FLOAT"))
                  {
                     AiNodeSetFlt(anode, "subdiv_pixel_error", plug.asFloat());
                  }
               }
            }

            if (!StringInList("subdiv_dicing_camera", attrs))
            {
               plug = node.findPlug("subdiv_dicing_camera");
               if (!plug.isNull())
               {
                  MString cameraName = plug.asString();

                  AtNode *cameraNode = NULL;

                  if (cameraName != "" && cameraName != "Default")
                  {
                     cameraNode = AiNodeLookUpByName(cameraName.asChar());
                  }

                  if (AiNodeEntryLookUpParameter(anode->base_node, "subdiv_dicing_camera") != 0 ||
                      AiNodeDeclare(anode, "subdiv_dicing_camera", "constant NODE"))
                  {
                     AiNodeSetPtr(anode, "subdiv_dicing_camera", cameraNode);
                  }
               }
            }

            if (!StringInList("smoothing", attrs))
            {
               plug = node.findPlug("smoothShading");
               if (!plug.isNull())
               {
                  if (AiNodeEntryLookUpParameter(anode->base_node, "smoothing") != 0 ||
                      AiNodeDeclare(anode, "smoothing", "constant BOOL"))
                  {
                     AiNodeSetBool(anode, "smoothing", plug.asBool());
                  }
               }
            }

            // Set diplacement shader
            if (!StringInList("disp_map", attrs))
            {
               if (shadingEngine.object() != MObject::kNullObj)
               {
                  MPlugArray shaderConns;

                  MPlug shaderPlug = shadingEngine.findPlug("displacementShader");

                  shaderPlug.connectedTo(shaderConns, true, false);

                  if (shaderConns.length() > 0)
                  {
                     MFnDependencyNode dispNode(shaderConns[0].node());

                     if (!StringInList("disp_height", attrs) &&
                         (AiNodeEntryLookUpParameter(anode->base_node, "disp_height") != 0 ||
                          AiNodeDeclare(anode, "disp_height", "constant FLOAT")))
                     {
                        plug = dispNode.findPlug("disp_height");
                        if (!plug.isNull())
                        {
                           AiNodeSetFlt(anode, "disp_height", plug.asFloat());
                        }
                     }

                     if (!StringInList("disp_zero_value", attrs) &&
                         (AiNodeEntryLookUpParameter(anode->base_node, "disp_zero_value") != 0 ||
                          AiNodeDeclare(anode, "disp_zero_value", "constant FLOAT")))
                     {
                        plug = dispNode.findPlug("disp_zero_value");
                        if (!plug.isNull())
                        {
                           AiNodeSetFlt(anode, "disp_zero_value", plug.asFloat());
                        }
                     }

                     shaderConns.clear();
                     dispNode.findPlug("disp_map").connectedTo(shaderConns, true, false);

                     if (shaderConns.length() > 0 &&
                         (AiNodeEntryLookUpParameter(anode->base_node, "disp_map") != 0 ||
                          AiNodeDeclare(anode, "disp_map", "constant NODE")))
                     {
                        plug = dispNode.findPlug("disp_map");
                        if (!plug.isNull())
                        {
                           MString attrName = shaderConns[0].partialName(false, false, false, false, false, true);
                           AtNode* dispImage(ExportShader(shaderConns[0].node(), attrName));

                           MPlug pVectorDisp = dispNode.findPlug("vector_displacement");
                           if (!pVectorDisp.isNull() && pVectorDisp.asBool())
                           {
                              AtNode* tangentToObject = AiNode("tangentToObjectSpace");
                              ProcessShaderParameter(dispNode, "vector_displacement_scale", tangentToObject, "scale", AI_TYPE_VECTOR);
                              AiNodeLink(dispImage, "map", tangentToObject);

                              AiNodeSetPtr(anode, "disp_map", tangentToObject);
                           }
                           else
                           {
                              AiNodeSetPtr(anode, "disp_map", dispImage);
                           }
                        }
                     }

                     AiNodeSetBool(anode, "autobump", dispNode.findPlug("autobump").asBool());
                  }
               }
            }
         }

         if (!StringInList("sidedness", attrs))
         {
            plug = node.findPlug("doubleSided");
            if (!plug.isNull())
            {
               AiNodeSetInt(anode, "sidedness", plug.asBool() ? 65535 : 0);

               // only set invert_normals if doubleSided attribute could be found
               if (!StringInList("invert_normals", attrs))
               {
                  plug = node.findPlug("opposite");
                  if (!plug.isNull())
                  {
                     AiNodeSetBool(anode, "invert_normals", plug.asBool());
                  }
               }
            }
         }

         if (!StringInList("sss_max_samples", attrs))
         {
            plug = node.findPlug("sss_max_samples");
            if (!plug.isNull())
            {
               AiNodeSetInt(anode, "sss_max_samples", plug.asInt());
            }
         }

         if (!StringInList("sss_sample_spacing", attrs))
         {
            plug = node.findPlug("sss_sample_spacing");
            if (!plug.isNull())
            {
               AiNodeSetFlt(anode, "sss_sample_spacing", plug.asFloat());
            }
         }

         if (!StringInList("sss_use_gi", attrs))
         {
            plug = node.findPlug("sss_use_gi");
            if (!plug.isNull())
            {
               AiNodeSetBool(anode, "sss_use_gi", plug.asBool());
            }
         }

         if (!StringInList("receive_shadows", attrs))
         {
            plug = node.findPlug("receiveShadows");
            if (!plug.isNull())
            {
               AiNodeSetBool(anode, "receive_shadows", plug.asBool());
            }
         }

         if (!StringInList("self_shadows", attrs))
         {
            plug = node.findPlug("self_shadows");
            if (!plug.isNull())
            {
               AiNodeSetBool(anode, "self_shadows", plug.asBool());
            }
         }

         if (!StringInList("opaque", attrs))
         {
            plug = node.findPlug("opaque");
            if (!plug.isNull())
            {
               AiNodeSetBool(anode, "opaque", plug.asBool());
            }
         }

         if (!StringInList("visibility", attrs))
         {
            AtInt visibility = AI_RAY_ALL;

            plug = node.findPlug("castsShadows");
            if (!plug.isNull() && !plug.asBool())
            {
               visibility &= ~AI_RAY_SHADOW;
            }

            plug = node.findPlug("primaryVisibility");
            if (!plug.isNull() && !plug.asBool())
            {
               visibility &= ~AI_RAY_CAMERA;
            }

            plug = node.findPlug("visibleInReflections");
            if (!plug.isNull() && !plug.asBool())
            {
               visibility &= ~AI_RAY_REFLECTED;
            }

            plug = node.findPlug("visibleInRefractions");
            if (!plug.isNull() && !plug.asBool())
            {
               visibility &= ~AI_RAY_REFRACTED;
            }

            plug = node.findPlug("diffuse_visibility");
            if (!plug.isNull() && !plug.asBool())
            {
               visibility &= ~AI_RAY_DIFFUSE;
            }

            plug = node.findPlug("glossy_visibility");
            if (!plug.isNull() && !plug.asBool())
            {
               visibility &= ~AI_RAY_GLOSSY;
            }

            AiNodeSetInt(anode, "visibility", visibility);
         }

         // Set surface shader
         if (!StringInList("shader", attrs))
         {
            if (shadingEngine.object() != MObject::kNullObj)
            {
               MPlugArray shaderConns;

               MPlug shaderPlug = shadingEngine.findPlug("surfaceShader");

               shaderPlug.connectedTo(shaderConns, true, false);

               if (shaderConns.length() > 0)
               {
                  AtNode *shader = ExportShader(shaderConns[0].node());
                  AiNodeSetPtr(anode, "shader", shader);
               }
            }
         }
      }

      // Call cleanup command
      if ((m_motionBlurData.enabled == false || int(step) >= (int(m_motionBlurData.motion_steps) - 1)) && cleanupCmd != "")
      {
         command = cleanupCmd;
         command += "(\"";
         command += dagPath.fullPathName();
         command += "\", ";

         if (isMasterDag)
         {
            command += "0, \"\")";
         }
         else
         {
            command += "1, \"";
            command += masterDag.fullPathName();
            command += "\")";
         }

         status = MGlobal::executeCommand(command);

         if (!status)
         {
            AiMsgError("[mtoa] ERROR: Failed to cleanup custom shape \"%s\".", node.name().asChar());
            return;
         }
      }
   }
}

void CMayaScene::GetCustomShapes()
{
   static const char separators[] = ",:;";

   MString csVar = "$MTOA_CUSTOM_SHAPES";

   MString csExpVar = csVar.expandEnvironmentVariablesAndTilde();

   if (csExpVar != csVar)
   {
      std::string shapeType;
      std::string shapeList = csExpVar.asChar();

      size_t p0 = 0;
      size_t p1 = shapeList.find_first_of(separators, p0);

      while (p1 != std::string::npos)
      {
         shapeType = shapeList.substr(p0, p1-p0);

         if (RegisterCustomShape(shapeType))
         {
            MGlobal::displayInfo("[mtoa] Registered custom shape \"" + MString(shapeType.c_str()) + "\"");
         }

         p0 = p1 + 1;
         p1 = shapeList.find_first_of(separators, p0);
      }

      shapeType = shapeList.substr(p0);

      if (RegisterCustomShape(shapeType))
      {
         MGlobal::displayInfo("[mtoa] Registered custom shape \"" + MString(shapeType.c_str()) + "\"");
      }
   }
}
