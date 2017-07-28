#include "StandinsTranslator.h"
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


#include <maya/MString.h>

void CArnoldStandInsTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "procedural");
   CShapeTranslator::MakeCommonAttributes(helper);

   CAttrData data;
   data.defaultValue.BOOL() = true;
   data.name = "overrideLightLinking";
   data.shortName = "oll";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL() = true;
   data.name = "overrideShaders";
   data.shortName = "osh";
   helper.MakeInputBoolean(data);
}

AtNode* CArnoldStandInsTranslator::CreateArnoldNodes()
{
   bool isInstanced = (!IsMasterInstance());

   // #2771 for interactive sessions, we export standin instances as 
   // several procedurals instead of ginstances. We could do this for all
   // kind of exports, but for now we only change it in IPR.
   // We can only do this for .ass files, which arnold core will
   // handle through the procedural cache. We don't want to duplicate other kind
   // of procedurals, since they'd really be duplicated.
   if (isInstanced && GetSessionOptions().IsInteractiveRender())
   {
      // check here if the filename is .ass for interactive renders
      m_DagNode.setObject(m_dagPath.node());
      MString dso = m_DagNode.findPlug("dso").asString();
      unsigned int nchars = dso.numChars();
      if (nchars > 4 && dso.substringW(nchars-4, nchars) == ".ass")
         isInstanced = false;
   }

   // Note that the code below is possible because RequestUpdate sets the 
   // update mode to AI_RECREATE_NODE, otherwise we could have invalid arnold nodes
   // when the filename is changed during IPR
   if (!isInstanced)
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

// FIXME : please remove all these hacks regarding the "override" attributes 
// once we no longer case about pre-2.0.2 compatibility
static void ArnoldStandInVisibilityOverride(AtByte &visibility, MPlug overridePlug, 
                                                MPlug visPlug, AtByte rayType)
{
   if (visPlug.isNull())
      return; // nothing to do here

   // FIXME to be removed once we get rid of these "override" attributes
   if (!overridePlug.isNull() && !overridePlug.asBool())
   {
      // the deprecated override parameter was left to False
      // need to set it to True so that we stop caring about it
      overridePlug.setBool(true);

      // if override was OFF and Visibility was OFF
      // it actually meant that my Visibility was ON
      if (!visPlug.asBool())
         visPlug.setBool(true);
   }

   // I can only remove visibility as compared to full visibility
   if (!visPlug.asBool())
      visibility &= ~rayType;
}
/**
*   Standins visibility override is a bit special  :
*   The procedural visibility will determine which rays make it to the procedural node. Then, the child node 
*   keeps its own visibility value (i.e. it doesn't inherit from the procedural parent as for other attributes).
*   This means that the resulting visibility value for the nested objects is the "intersection" of their own value
*   and the procedural's one. So the only thing we can do here is to remove bits of the visibility value.
**/
AtByte CArnoldStandInsTranslator::ComputeOverrideVisibility()
{
   // Usually invisible nodes are not exported at all, just making sure here
   if (!IsRenderable())
      return AI_RAY_UNDEFINED;

   AtByte visibility = AI_RAY_ALL;

   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overrideCastsShadows"), 
                                                FindMayaPlug("castsShadows"), 
                                                AI_RAY_SHADOW);

   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overridePrimaryVisibility"), 
                                                FindMayaPlug("primaryVisibility"), 
                                                AI_RAY_CAMERA);

   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overrideVisibleInDiffuseReflection"), 
                                                FindMayaPlug("aiVisibleInDiffuseReflection"), 
                                                AI_RAY_DIFFUSE_REFLECT);

   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overrideVisibleInSpecularReflection"), 
                                                FindMayaPlug("aiVisibleInSpecularReflection"), 
                                                AI_RAY_SPECULAR_REFLECT);
   
   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overrideVisibleInDiffuseTransmission"), 
                                                FindMayaPlug("aiVisibleInDiffuseTransmission"), 
                                                AI_RAY_DIFFUSE_TRANSMIT);

   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overrideVisibleInSpecularTransmission"), 
                                                FindMayaPlug("aiVisibleInSpecularTransmission"), 
                                                AI_RAY_SPECULAR_TRANSMIT);

   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overrideVisibleInVolume"), 
                                                FindMayaPlug("aiVisibleInVolume"), 
                                                AI_RAY_VOLUME);

   return visibility;
}

/// overrides CShapeTranslator::ProcessRenderFlags to ensure that we don't set aiOpaque unless overrideOpaque is enabled
void CArnoldStandInsTranslator::ProcessRenderFlags(AtNode* node)
{
   AiNodeSetByte(node, "visibility", ComputeOverrideVisibility());

   MPlug plug;
   
   plug = FindMayaPlug("overrideSelfShadows");
   if (plug.isNull() || plug.asBool())
   {
      plug = FindMayaPlug("aiSelfShadows");
      if (!plug.isNull()) AiNodeSetBool(node, "self_shadows", plug.asBool());
   }

   // for standins, we check
   plug = FindMayaPlug("overrideOpaque");
   if (plug.isNull() || plug.asBool())
   {
      plug = FindMayaPlug("aiOpaque");
      if (!plug.isNull()) AiNodeSetBool(node, "opaque", plug.asBool());
   }
   
   plug = FindMayaPlug("overrideReceiveShadows");
   if (plug.isNull() || plug.asBool())
   {
      plug = FindMayaPlug("receiveShadows");
      if (!plug.isNull()) AiNodeSetBool(node, "receive_shadows", plug.asBool());
   }
   
   plug = FindMayaPlug("overrideDoubleSided");
   if (plug.isNull() || plug.asBool())
   {
      plug = FindMayaPlug("doubleSided");
      
      if (!plug.isNull() && plug.asBool())
         AiNodeSetByte(node, "sidedness", AI_RAY_ALL);
      else
         AiNodeSetByte(node, "sidedness", 0);
   }
   
   // for standins, we check
   plug = FindMayaPlug("overrideMatte");
   if (plug.isNull() || plug.asBool())
   {
      plug = FindMayaPlug("aiMatte");
      if (!plug.isNull()) AiNodeSetBool(node, "matte", plug.asBool());
   }
   if (RequiresMotionData())
   {
      double motionStart, motionEnd;
      GetSessionOptions().GetMotionRange(motionStart, motionEnd);
      AiNodeSetFlt(node, "motion_start", (float)motionStart);
      AiNodeSetFlt(node, "motion_end", (float)motionEnd);
   }
}

void CArnoldStandInsTranslator::Export(AtNode* anode)
{
   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(anode));
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportInstance(anode, GetMasterInstance());
   }
   else
   {
      ExportProcedural(anode, IsExported());
   }
}

void CArnoldStandInsTranslator::ExportMotion(AtNode* anode)
{
   ExportMatrix(anode);
}

// Deprecated : Arnold support procedural instance, but it's not safe.
//
AtNode* CArnoldStandInsTranslator::ExportInstance(AtNode *instance, const MDagPath& masterInstance)
{
   CNodeTranslator *masterTr = GetTranslator(masterInstance);

   // in case master instance wasn't exported (#648)
   if (masterTr == NULL)
      masterTr = CDagTranslator::ExportDagPath(masterInstance);
   
   AtNode *masterNode = (masterTr) ? masterTr->GetArnoldNode() : NULL;

   AiNodeSetStr(instance, "name", CDagTranslator::GetArnoldNaming(m_dagPath).asChar());

   ExportMatrix(instance);

   AiNodeSetPtr(instance, "node", masterNode);
   AiNodeSetBool(instance, "inherit_xform", false);
  
   // #2858 we must recompute the visibility instead of getting the master's one
   // since we're not sure the master has already been exported 
   //AtByte visibility = AiNodeGetByte(masterNode, "visibility");
   //AiNodeSetByte(instance, "visibility", visibility);
   ProcessRenderFlags(instance);

   m_DagNode.setObject(masterInstance);
   
   if (m_DagNode.findPlug("overrideShaders").asBool() &&
      RequiresShaderExport())
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
   ExportStandinsShaders(GetArnoldNode());
}

void CArnoldStandInsTranslator::ExportStandinsShaders(AtNode* procedural)
{
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   std::vector<AtNode*> meshShaders;

   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroupPlug.isNull())
   {
      AtNode *shader = ExportConnectedNode(shadingGroupPlug);
      if (shader != NULL)
      {
         AiNodeSetPtr(procedural, "shader", shader);
         meshShaders.push_back(shader);
      }
      else
      {
         AiMsgWarning("[mtoa] [translator %s] ShadingGroup %s has no surfaceShader input",
               GetTranslatorName().asChar(), MFnDependencyNode(shadingGroupPlug.node()).name().asChar());
         /*AiMsgWarning("[mtoa] ShadingGroup %s has no surfaceShader input.",
               fnDGNode.name().asChar());*/
         AiNodeSetPtr(procedural, "shader", NULL);
      }
      if (meshShaders.size() > 0)
      {
         AiNodeDeclare(procedural, "mtoa_shading_groups", "constant ARRAY NODE");
         AiNodeSetArray(procedural, "mtoa_shading_groups",
                        AiArrayConvert(meshShaders.size(), 1, AI_TYPE_NODE, &(meshShaders[0])));
      }
   }
}
/*
void CArnoldStandInsTranslator::ExportBoundingBox(AtNode* procedural)
{
   int drawOverride = m_DagNode.findPlug("standin_draw_override").asShort(); 
   if (drawOverride == 0) 
   { 
      MObject ArnoldRenderOptionsNode = GetSessionOptions().GetArnoldRenderOptions(); 
      if (!ArnoldRenderOptionsNode.isNull()) 
         drawOverride = MFnDependencyNode(ArnoldRenderOptionsNode).findPlug("standin_draw_override").asShort(); 
   } 
   else 
      drawOverride -= 1;
      
   MPoint bbMin, bbMax;
   
   if (drawOverride == 3) 
   { 
      bbMin.x = m_DagNode.findPlug("MinBoundingBox0").asFloat(); 
      bbMin.y = m_DagNode.findPlug("MinBoundingBox1").asFloat(); 
      bbMin.z = m_DagNode.findPlug("MinBoundingBox2").asFloat(); 
      bbMax.x = m_DagNode.findPlug("MaxBoundingBox0").asFloat(); 
      bbMax.y = m_DagNode.findPlug("MaxBoundingBox1").asFloat(); 
      bbMax.z = m_DagNode.findPlug("MaxBoundingBox2").asFloat(); 
   } 
   else 
   { 
      MBoundingBox boundingBox = m_DagNode.boundingBox();
      bbMin = boundingBox.min();
      bbMax = boundingBox.max();
   }
   
   float minCoords[4];
   float maxCoords[4];

   bbMin.get(minCoords);
   bbMax.get(maxCoords);

   //AiNodeSetVec(procedural, "min", minCoords[0], minCoords[1], minCoords[2]);
   //AiNodeSetVec(procedural, "max", maxCoords[0], maxCoords[1], maxCoords[2]);
}
*/

AtNode* CArnoldStandInsTranslator::ExportProcedural(AtNode* procedural, bool update)
{
   m_DagNode.setObject(m_dagPath.node());

   AiNodeSetStr(procedural, "name", GetArnoldNaming(m_dagPath).asChar());

   ExportMatrix(procedural);
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

      int frame = m_DagNode.findPlug("frameNumber").asInt();
      float frameOffset = m_DagNode.findPlug("frameOffset").asFloat();
      bool useFrameExtension = m_DagNode.findPlug("useFrameExtension").asBool();
      bool useSubFrame = m_DagNode.findPlug("useSubFrame").asBool();

      MString frameNumber = "0";

      float framestep = frame + frameOffset;

      bool subFrames = ((framestep - floor(framestep)) >= 0.001);
      char frameExtWithHash[64];
      char frameExtWithDot[64];
      char frameExt[64];

      int start = 0;
      int end = 0;
      MStringArray pattern;
      MString newDso = "";
      int framePadding = 0;
      int subFramePadding = 0;
      bool resolved = false;
      MString a, b;
      
      start = dso.index('#');
      end = dso.rindex('#');

      if(start >= 0)
      {
         if(dso.substring(start-1,start-1) == "_")
            newDso = dso.substring(0,start-2) + ".#" + dso.substring(end+1,dso.length());
         else
            newDso = dso.substring(0,start-1) + "#" + dso.substring(end+1,dso.length());
         
         dso.substring(start,end).split('.',pattern);
         if(pattern.length() > 0)
         {
            framePadding = pattern[0].length();
            a = pattern[0];
         }
         if(pattern.length() > 1)
         {
            subFramePadding = pattern[1].length();
            b = pattern[1];
         }
      }
      else
      {
         newDso = dso;
      }

      if (subFrames || useSubFrame || (subFramePadding != 0))
      {
         int fullFrame = (int) floor(framestep);
         int subFrame = (int) floor((framestep - fullFrame) * 1000);
         sprintf(frameExtWithHash, "_%0*d.%0*d", framePadding, fullFrame, subFramePadding, subFrame);
         sprintf(frameExtWithDot, ".%0*d.%0*d", framePadding, fullFrame, subFramePadding, subFrame);
         sprintf(frameExt, "%0*d.%0*d", framePadding, fullFrame, subFramePadding, subFrame);
      }
      else
      {
         sprintf(frameExtWithHash, "_%0*d", framePadding, (int) framestep);
         sprintf(frameExtWithDot, ".%0*d", framePadding, (int) framestep);
         sprintf(frameExt, "%0*d", framePadding, (int) framestep);
      }
      frameNumber = frameExtWithDot;

      resolved = MRenderUtil::exactFileTextureName(newDso, useFrameExtension, frameNumber, filename);
   
      if (!resolved)
      {
         frameNumber = frameExtWithHash;
         resolved = MRenderUtil::exactFileTextureName(newDso, useFrameExtension, frameNumber, filename);
      }
   
      if (!resolved)
      {
         // If file has something after frame number, MRenderUtil::exactFileTextureName has problems to
         //  find the file.
         if (start >= 0)
         {
            MString baseName = dso.substring(0,start-1) + frameExt + dso.substring(end+1,dso.length());
            resolved = MRenderUtil::exactFileTextureName(baseName, false, frameNumber, filename);
         }
      }

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
      
      GetSessionOptions().FormatProceduralPath(resolvedName);
      AiNodeSetStr(procedural, "filename", resolvedName.asChar());
/*
      MPlug deferStandinLoad = m_DagNode.findPlug("deferStandinLoad");
      if (!deferStandinLoad.asBool())
         AiNodeSetBool(procedural, "load_at_init", true);
      else
         ExportBoundingBox(procedural);
*/

      MPlug data = m_DagNode.findPlug("data");
      int sizeData = strlen(data.asString().asChar());
      if (sizeData != 0)
      {
         AiNodeSetStr(procedural, "data", data.asString().expandEnvironmentVariablesAndTilde().asChar());
      }
      AiNodeSetBool(procedural, "override_nodes", m_DagNode.findPlug("overrideNodes").asBool());
   }
   return procedural;
}


void CArnoldStandInsTranslator::RequestUpdate()
{  
   SetUpdateMode(AI_RECREATE_NODE);
   CShapeTranslator::RequestUpdate();
   // this should propagate a request update on all other procedurals, standins, referencing me
}
