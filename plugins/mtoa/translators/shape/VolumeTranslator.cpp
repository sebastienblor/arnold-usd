#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif

#include "VolumeTranslator.h"
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
#include <maya/MStringArray.h>

enum VolumeType{
   VT_CUSTOM,
   VT_OPEN_VDB
};

void CArnoldVolumeTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "volume");
   CShapeTranslator::MakeCommonAttributes(helper);
   CShapeTranslator::MakeMayaVisibilityFlags(helper);
}

AtNode* CArnoldVolumeTranslator::CreateArnoldNodes()
{
   m_DagNode.setObject(m_dagPath.node());
   if (IsMasterInstance())
   {
      // if type is vdb, create volume_openvdb
      MString filename = GetFileName();
      if (filename == "" )
      {
            return AddArnoldNode("box");
      }
      else
      {
         MPlug plug = FindMayaPlug("type");
         if (plug.asInt() == 0)
            return AddArnoldNode("volume");
         else
            return AddArnoldNode("volume_implicit");
      }
   }
   else
   {
      AtNode * tmpRes = AddArnoldNode("ginstance");
      return  tmpRes;
   }
}

void CArnoldVolumeTranslator::Export(AtNode* anode)
{
   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(anode));
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportInstance(anode, GetMasterInstance());
   }
   else
   {
      ExportVolume(anode, IsExported());
   }
}

void CArnoldVolumeTranslator::ExportMotion(AtNode* anode)
{
   ExportMatrix(anode);
}


AtNode* CArnoldVolumeTranslator::ExportInstance(AtNode *instance, const MDagPath& masterInstance)
{
   MFnDependencyNode masterDepNode(masterInstance.node());
   MPlug dummyPlug = masterDepNode.findPlug("matrix", true);
   // in case master instance wasn't exported (#648)
   // and also to create the reference between both translators
   AtNode *masterNode = (dummyPlug.isNull()) ? NULL : ExportConnectedNode(dummyPlug);
   

   AiNodeSetStr(instance, "name", GetSessionOptions().GetArnoldNaming(m_dagPath).asChar());

   ExportMatrix(instance);

   AiNodeSetPtr(instance, "node", masterNode);
   AiNodeSetBool(instance, "inherit_xform", false);
   
   AtByte visibility = AiNodeGetByte(masterNode, "visibility");
   AiNodeSetByte(instance, "visibility", visibility);

   return instance;
}

void CArnoldVolumeTranslator::ExportShaders()
{
   AiMsgWarning( "[mtoa] Shaders untested with new multitranslator and standin code.");
   /// TODO: Test shaders with Volume.
   ExportVolumeShaders(GetArnoldNode());
}

void CArnoldVolumeTranslator::ExportVolumeShaders(AtNode* volume)
{
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   std::vector<AtNode*> meshShaders;

   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroupPlug.isNull())
   {
      AtNode *shader = ExportConnectedNode(shadingGroupPlug);
      if (shader != NULL)
      {
         AiNodeSetPtr(volume, "shader", shader);
         meshShaders.push_back(shader);
      }
      else
      {
         AiMsgWarning("[mtoa] [translator %s] ShadingGroup %s has no surfaceShader input",
               GetTranslatorName().asChar(), MFnDependencyNode(shadingGroupPlug.node()).name().asChar());
         /*AiMsgWarning("[mtoa] ShadingGroup %s has no surfaceShader input.",
               fnDGNode.name().asChar());*/
         AiNodeSetPtr(volume, "shader", NULL);
      }
   }
}

MString CArnoldVolumeTranslator::GetFileName()
{
   MString filename = m_DagNode.findPlug("filename", true).asString();
   int start = 0;
   int end = 0;
   MString newFilename = "";
   char frameExt[64];

   start = filename.index('#');
   end = filename.rindex('#');
   
   if(start >= 0)
   {
      sprintf(frameExt, "%0*d", end - start + 1, m_DagNode.findPlug("frame", true).asInt());
      newFilename = filename.substring(0,start-1) + frameExt + filename.substring(end+1,filename.length());
   }
   else
   {
      newFilename = filename;
   }
   newFilename = newFilename.expandEnvironmentVariablesAndTilde();

   return newFilename;
}

AtNode* CArnoldVolumeTranslator::ExportVolume(AtNode* volume, bool update)
{

   AiNodeSetStr(volume, "name", GetSessionOptions().GetArnoldNaming(m_dagPath).asChar());

   ExportMatrix(volume);
   ProcessRenderFlags(volume);
   ExportVolumeShaders(volume);
   ExportLightLinking(volume);

   update = false;

   if (!update)
   {

      MString newFilename = GetFileName();
      const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(volume));
      float stepSize = m_DagNode.findPlug("stepSize", true).asFloat();
      if (m_DagNode.findPlug("autoStepSize", true).asBool())
         stepSize = 0.f;

      if (strcmp(nodeType, "box") == 0)
      {
         AiNodeSetVec(volume, "min",
                        -1.f,
                        -1.f,
                        -1.f);

         AiNodeSetVec(volume, "max",
                        1.f,
                        1.f,
                        1.f);

         if (stepSize == 0.f)
            stepSize = 0.01f;
      }
      else
      {

         AiNodeSetStr( volume, "filename", newFilename.asChar() );

         MString grids = m_DagNode.findPlug("grids", true).asString();
         MStringArray gridList;
         grids.split(' ',gridList);

         if (gridList.length() > 0)
         {
            AtArray *ary = AiArrayAllocate(gridList.length(), 1, AI_TYPE_STRING);
            for(unsigned int i = 0; i < gridList.length(); i++)
            {
               AiArraySetStr(ary, i, gridList[i].asChar());
            }
            AiNodeSetArray( volume, "grids", ary);
         }

         AiNodeSetFlt(volume, "step_scale", m_DagNode.findPlug("stepScale", true).asFloat());

         MString vGrids = m_DagNode.findPlug("velocityGrids", true).asString();
         MStringArray vGridList;
         vGrids.split(' ',vGridList);

         if (vGridList.length() > 0)
         {
            AtArray *ary = AiArrayAllocate(vGridList.length(), 1, AI_TYPE_STRING);
            for(unsigned int i = 0; i < vGridList.length(); i++)
            {
               AiArraySetStr(ary, i, vGridList[i].asChar());
            }
            AiNodeSetArray( volume, "velocity_grids", ary);
         }

         AiNodeSetFlt(volume, "velocity_scale", m_DagNode.findPlug("velocityScale", true).asFloat());

         AiNodeSetFlt(volume, "velocity_fps", m_DagNode.findPlug("velocityFps", true).asFloat());

         AiNodeSetFlt(volume, "velocity_outlier_threshold", m_DagNode.findPlug("velocityThreshold", true).asFloat());

         AiNodeSetBool(volume, "disable_ray_extents", m_DagNode.findPlug("disableRayExtents", true).asBool());
         AiNodeSetBool(volume, "compress", m_DagNode.findPlug("compress", true).asBool());
         AiNodeSetFlt(volume, "volume_padding", m_DagNode.findPlug("volumePadding", true).asFloat());

         if (strcmp(nodeType, "volume_implicit") == 0)
         {
            // export as implicit
            AiNodeSetFlt(volume, "threshold", m_DagNode.findPlug("threshold", true).asFloat());
            AiNodeSetUInt(volume, "samples", m_DagNode.findPlug("samples", true).asInt());
            AiNodeSetInt(volume, "solver", m_DagNode.findPlug("solver", true).asInt());
            AiNodeSetStr(volume, "field_channel", AtString(m_DagNode.findPlug("fieldChannel", true).asString().asChar()));

            MPlugArray conns;
            MPlug field = FindMayaPlug("field");
            field.connectedTo(conns, true, false);
            if (conns.length() > 0)
               AiNodeSetPtr(volume, "field", ExportConnectedNode(conns[0]));
            else
               AiNodeSetPtr(volume, "field", NULL);

         }
      }

      AiNodeSetFlt(volume, "step_size", stepSize);
      AiNodeSetFlt(volume, "motion_start", m_DagNode.findPlug("motionStart", true).asFloat());
      AiNodeSetFlt(volume, "motion_end", m_DagNode.findPlug("motionEnd", true).asFloat());

   }
   return volume;
}

void CArnoldVolumeTranslator::RequestUpdate()
{
   SetUpdateMode(AI_RECREATE_NODE);
   CShapeTranslator::RequestUpdate();
}