#include "StandinsTranslator.h"

#include "render/RenderSession.h"
#include "attributes/AttrHelper.h"
#include "utils/time.h"
#include "scene/MayaScene.h"

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

/// overrides CShapeTranslator::ProcessRenderFlags to ensure that we don't set aiOpaque unless overrideOpaque is enabled
void CArnoldStandInsTranslator::ProcessRenderFlags(AtNode* node)
{
   AiNodeSetInt(node, "visibility", ComputeVisibility());

   MPlug plug;
   plug = FindMayaPlug("aiSelfShadows");
   if (!plug.isNull()) AiNodeSetBool(node, "self_shadows", plug.asBool());

   // for standins, we check
   plug = FindMayaPlug("overrideOpaque");
   if (plug.isNull() || plug.asBool())
   {
      plug = FindMayaPlug("aiOpaque");
      if (!plug.isNull()) AiNodeSetBool(node, "opaque", plug.asBool());
   }
   plug = FindMayaPlug("receiveShadows");
   if (!plug.isNull()) AiNodeSetBool(node, "receive_shadows", plug.asBool());

   // Sub-Surface Scattering
   plug = FindMayaPlug("aiSssSampleDistribution");
   if (!plug.isNull()) AiNodeSetInt(node, "sss_sample_distribution", plug.asInt());

   plug = FindMayaPlug("aiSssSampleSpacing");
   if (!plug.isNull()) AiNodeSetFlt(node, "sss_sample_spacing", plug.asFloat());

}

void CArnoldStandInsTranslator::Export(AtNode* anode)
{
   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(anode));
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportInstance(anode, m_masterDag);
   }
   else
   {
      ExportProcedural(anode, false);
   }
}

void CArnoldStandInsTranslator::ExportMotion(AtNode* anode, unsigned int step)
{
   ExportMatrix(anode, step);
}

void CArnoldStandInsTranslator::Update(AtNode* anode)
{
   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(anode));
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportInstance(anode, m_masterDag);
   }
   else
   {
      ExportProcedural(anode, true);
   }
}

void CArnoldStandInsTranslator::UpdateMotion(AtNode* anode, unsigned int step)
{
   ExportMatrix(anode, step);
}

// Deprecated : Arnold support procedural instance, but it's not safe.
//
AtNode* CArnoldStandInsTranslator::ExportInstance(AtNode *instance, const MDagPath& masterInstance)
{
   AtNode* masterNode = AiNodeLookUpByName(masterInstance.partialPathName().asChar());

   AiNodeSetStr(instance, "name", m_dagPath.partialPathName().asChar());

   ExportMatrix(instance, 0);

   AiNodeSetPtr(instance, "node", masterNode);
   AiNodeSetBool(instance, "inherit_xform", false);
   
   int visibility = AiNodeGetInt(masterNode, "visibility");
   AiNodeSetInt(instance, "visibility", visibility);

   m_DagNode.setObject(masterInstance);
   
   if (m_DagNode.findPlug("overrideShaders").asBool() &&
      (CMayaScene::GetRenderSession()->RenderOptions()->outputAssMask() & AI_NODE_SHADER))
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

   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroupPlug.isNull())
   {
      AtNode *shader = ExportNode(shadingGroupPlug);
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

      start = dso.index('#');
      end = dso.rindex('#');

      if(start >= 0)
      {

         dso.substring(start,end).split('.',pattern);
         newDso = dso.substring(0,start-1) + "#" + dso.substring(end+1,dso.length());
         dso = newDso;
         MString a, b;
         

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

         resolved = MRenderUtil::exactFileTextureName(dso, useFrameExtension, frameNumber, filename);
      
         if (!resolved)
         {
            frameNumber = frameExtWithHash;
            resolved = MRenderUtil::exactFileTextureName(dso, useFrameExtension, frameNumber, filename);
         }
      
         if (!resolved)
         {
            // If file has ".ass.gz" extension, MRenderUtil::exactFileTextureName has problems to
            //  find the file.
            int len = dso.length();
            if (len > 8 && dso.substring(len - 7, len - 1) == ".ass.gz")
            {
               MString baseName = dso.substring(0, len - 9) + frameExt + ".ass.gz";
               resolved = MRenderUtil::exactFileTextureName(baseName, false, frameNumber, filename);
            }
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

      AiNodeSetStr(procedural, "dso", resolvedName.asChar());

      MPlug deferStandinLoad = m_DagNode.findPlug("deferStandinLoad");
      if (!deferStandinLoad.asBool())
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
