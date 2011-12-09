#include "StandinsTranslator.h"

#include "render/RenderSession.h"
#include "attributes/AttrHelper.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

#include <maya/MBoundingBox.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>


#include <maya/MString.h>

void CArnoldStandInsTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "procedural");
   CShapeTranslator::MakeCommonAttributes(helper);

   CAttrData data;
   data.defaultValue.BOOL = true;
   data.name = "overrideLightLinking";
   data.shortName = "oll";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL = true;
   data.name = "overrideShaders";
   data.shortName = "osh";
   helper.MakeInputBoolean(data);
}

AtNode* CArnoldStandInsTranslator::CreateArnoldNodes()
{
   m_isMasterDag = IsMasterInstance(m_masterDag);
   if (m_isMasterDag)
   {
      AtNode * tmpRes = AddArnoldNode("procedural");
      return  tmpRes;
   }
   else
   {
      AtNode * tmpRes = AddArnoldNode("ginstance");
      return  tmpRes;
   }
}


void CArnoldStandInsTranslator::Export(AtNode* anode)
{
   const char* nodeType = AiNodeEntryGetName(anode->base_node);
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportInstance(anode, m_masterDag);
   }
   else
   {
      ExportProcedural(anode, false);
   }
}

void CArnoldStandInsTranslator::ExportMotion(AtNode* anode, AtUInt step)
{
   ExportMatrix(anode, step);
}

// Deprecated : Arnold support procedural instance, but it's not safe.
//
AtNode* CArnoldStandInsTranslator::ExportInstance(AtNode *instance, const MDagPath& masterInstance)
{
   AtNode* masterNode = AiNodeLookUpByName(masterInstance.partialPathName().asChar());
   int instanceNum = m_dagPath.instanceNumber();

   AiNodeSetStr(instance, "name", m_dagPath.partialPathName().asChar());

   ExportMatrix(instance, 0);

   AiNodeSetPtr(instance, "node", masterNode);
   AiNodeSetBool(instance, "inherit_xform", false);

   m_DagNode.setObject(masterInstance);
   
   if (m_DagNode.findPlug("overrideShaders").asBool())
   {
      ExportStandinsShaders(instance);
   }
   if (m_DagNode.findPlug("overrideLightLinking").asBool())
   {
      ExportLightLinking(instance);
   }

   return instance;
}

void CArnoldStandInsTranslator::ExportShaders()
{
   AiMsgWarning( "[mtoa] Shaders untested with new multitranslator and standin code.");
   /// TODO: Test shaders with standins.
   ExportStandinsShaders(GetArnoldRootNode());
}

void CArnoldStandInsTranslator::ExportStandinsShaders(AtNode* procedural)
{
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   std::vector<AtNode*> meshShaders;

   MObject shadingGroup = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroup.isNull())
   {
      MPlugArray connections;
      MFnDependencyNode fnDGNode(shadingGroup);
      MPlug shaderPlug = fnDGNode.findPlug("surfaceShader");
      shaderPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         // shader assigned to node
         AtNode* shader = ExportNode(connections[0]);
         AiNodeSetPtr(procedural, "shader", shader);
         meshShaders.push_back(shader);
      }
      else
         AiMsgWarning("[mtoa] ShadingGroup %s has no surfaceShader input.",
               fnDGNode.name().asChar());
   }
   //
   // DISPLACEMENT
   //
   // currently does not work for per-face assignment
   if (!shadingGroup.isNull())
   {
      MPlugArray connections;
      MFnDependencyNode fnDGNode(shadingGroup);
      MPlug shaderPlug = fnDGNode.findPlug("displacementShader");
      shaderPlug.connectedTo(connections, true, false);

      // are there any connections to displacementShader?
      if (connections.length() > 0)
      {
         MFnDependencyNode dispNode(connections[0]);

         // Note that disp_height has no actual influence on the scale of the displacement if it is vector based
         // it only influences the computation of the displacement bounds
         AiNodeSetFlt(procedural, "disp_height", dispNode.findPlug("disp_height").asFloat());
         AiNodeSetFlt(procedural, "disp_padding", dispNode.findPlug("disp_padding").asFloat());
         AiNodeSetFlt(procedural, "disp_zero_value", dispNode.findPlug("disp_zero_value").asFloat());

         connections.clear();
         dispNode.findPlug("disp_map").connectedTo(connections, true, false);

         if (connections.length() > 0)
         {
            AtNode* dispImage(ExportNode(connections[0]));

            MPlug pVectorDisp = dispNode.findPlug("vector_displacement", false);
            if (!pVectorDisp.isNull() && pVectorDisp.asBool())
            {
               AtNode* tangentToObject = AiNode("tangentToObjectSpace");
               MPlug pVectorDispScale = dispNode.findPlug("vector_displacement_scale", false);
               // FIXME : do this using a translator instead
               ProcessParameter(tangentToObject, "scale", AI_TYPE_VECTOR, pVectorDispScale);
               AiNodeLink(dispImage, "map", tangentToObject);

               AiNodeSetPtr(procedural, "disp_map", tangentToObject);
            }
            else
            {
               AiNodeSetPtr(procedural, "disp_map", dispImage);
            }
         }
      }
   }
}

void CArnoldStandInsTranslator::ExportBoundingBox(AtNode* procedural)
{
   MBoundingBox boundingBox = m_DagNode.boundingBox();
   MPoint bbMin = boundingBox.min();
   MPoint bbMax = boundingBox.max();

   float minCoords[4];
   float maxCoords[4];

   bbMin.get(minCoords);
   bbMax.get(maxCoords);

   AiNodeSetPnt(procedural, "min", minCoords[0], minCoords[1], minCoords[2]);
   AiNodeSetPnt(procedural, "max", maxCoords[0], maxCoords[1], maxCoords[2]);
}


AtNode* CArnoldStandInsTranslator::ExportProcedural(AtNode* procedural, bool update)
{
   m_DagNode.setObject(m_dagPath.node());

   AiNodeSetStr(procedural, "name", m_dagPath.partialPathName().asChar());

   ExportMatrix(procedural, 0);
   ProcessRenderFlags(procedural);
   if (m_DagNode.findPlug("overrideShaders").asBool())
   {
      ExportStandinsShaders(procedural);
   }
   if (m_DagNode.findPlug("overrideLightLinking").asBool())
   {
      ExportLightLinking(procedural);
   }

   if (!update)
   {
      MString dso = m_DagNode.findPlug("dso").asString().expandEnvironmentVariablesAndTilde();
      MString filename;

      float frame = m_DagNode.findPlug("frameNumber").asFloat();
      float frameOffset = m_DagNode.findPlug("frameOffset").asFloat();
      bool useFrameExtension = m_DagNode.findPlug("useFrameExtension").asBool();

      MString frameNumber = "0";

      float framestep = frame + frameOffset;

      bool subFrames = ((framestep - floor(framestep)) >= 0.001);
      char frameExt[64];
      if (subFrames)
      {
         int fullFrame = (int) floor(framestep);
         int subFrame = (int) floor((framestep - fullFrame) * 1000);
         sprintf(frameExt, ".%04d.%03d", fullFrame, subFrame);
      }
      else
      {
         sprintf(frameExt, ".%04d", (int) framestep);
      }
      frameNumber = frameExt;

      bool resolved = MRenderUtil::exactFileTextureName(dso, useFrameExtension, frameExt, filename);

      MString resolvedName;
      if (resolved)
         resolvedName = filename.asChar();
      else
         resolvedName = dso.asChar();

      unsigned int nchars = resolvedName.numChars();
      if (nchars > 3 && resolvedName.substringW(nchars-3, nchars) == ".so")
      {
         resolvedName = resolvedName.substringW(0, nchars-4)+LIBEXT;
      }
      else if (nchars > 4 && resolvedName.substringW(nchars-4, nchars) == ".dll")
      {
         resolvedName = resolvedName.substringW(0, nchars-5)+LIBEXT;
      }
      else if (nchars > 6 && resolvedName.substringW(nchars-6, nchars) == ".dylib")
      {
         resolvedName = resolvedName.substringW(0, nchars-7)+LIBEXT;
      }

      AiNodeSetStr(procedural, "dso", resolvedName.asChar());

      MPlug loadInit = m_DagNode.findPlug("loadAtInit");
      if (!loadInit.asBool())
         AiNodeSetBool(procedural, "load_at_init", true);
      else
         ExportBoundingBox(procedural);

      MPlug data = m_DagNode.findPlug("data");
      int sizeData = strlen(data.asString().asChar());
      if (sizeData != 0)
      {
         AiNodeSetStr(procedural, "data", data.asString().expandEnvironmentVariablesAndTilde().asChar());
      }

   }
   return procedural;
}
