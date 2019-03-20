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
   if (IsMasterInstance())
   {
      // if type is vdb, create volume_openvdb
      MPlug plug = FindMayaPlug("type");
      if (plug.asInt() == 0)
         return AddArnoldNode("volume");
      else
         return AddArnoldNode("volume_implicit");

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
   

   AiNodeSetStr(instance, "name", CDagTranslator::GetArnoldNaming(m_dagPath).asChar());

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
/*
void CArnoldVolumeTranslator::ExportBoundingBox(AtNode* volume)
{
   AiNodeSetVec(volume, "min",
                        m_DagNode.findPlug("min").child(0).asFloat(),
                        m_DagNode.findPlug("min").child(1).asFloat(),
                        m_DagNode.findPlug("min").child(2).asFloat());
                        
   AiNodeSetVec(volume, "max",
                        m_DagNode.findPlug("max").child(0).asFloat(),
                        m_DagNode.findPlug("max").child(1).asFloat(),
                        m_DagNode.findPlug("max").child(2).asFloat());
}
*/

AtNode* CArnoldVolumeTranslator::ExportVolume(AtNode* volume, bool update)
{
   m_DagNode.setObject(m_dagPath.node());

   AiNodeSetStr(volume, "name", CDagTranslator::GetArnoldNaming(m_dagPath).asChar());

   ExportMatrix(volume);
   ProcessRenderFlags(volume);
   
   ExportVolumeShaders(volume);
   
   ExportLightLinking(volume);
   
   update = false;

   if (!update)
   {

/*      MPlug loadAtInit = m_DagNode.findPlug("loadAtInit");
      if (loadAtInit.asBool())
         AiNodeSetBool(volume, "load_at_init", true);
      
      ExportBoundingBox(volume);
*/
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
      
      AiNodeDeclare( volume, "filename", "constant STRING" );
      AiNodeSetStr( volume, "filename", newFilename.asChar() );
      
      MString grids = m_DagNode.findPlug("grids", true).asString();
      MStringArray gridList;
      grids.split(' ',gridList);
      
      if (gridList.length() > 0)
      {
         AiNodeDeclare( volume, "grids", "constant ARRAY STRING" );
         AtArray *ary = AiArrayAllocate(gridList.length(), 1, AI_TYPE_STRING);
         for(unsigned int i = 0; i < gridList.length(); i++)
         {
            AiArraySetStr(ary, i, gridList[i].asChar());
         }
         AiNodeSetArray( volume, "grids", ary);
      }
      float stepSize = m_DagNode.findPlug("stepSize", true).asFloat();
      if (m_DagNode.findPlug("autoStepSize", true).asBool())
         stepSize = 0.f;
      
      AiNodeSetFlt(volume, "step_size", stepSize);
      AiNodeSetFlt(volume, "step_scale", m_DagNode.findPlug("stepScale", true).asFloat());
      
      MString vGrids = m_DagNode.findPlug("velocityGrids", true).asString();
      MStringArray vGridList;
      vGrids.split(' ',vGridList);
      
      if (vGridList.length() > 0)
      {
         AiNodeDeclare( volume, "velocity_grids", "constant ARRAY STRING" );
         AtArray *ary = AiArrayAllocate(vGridList.length(), 1, AI_TYPE_STRING);
         for(unsigned int i = 0; i < vGridList.length(); i++)
         {
            AiArraySetStr(ary, i, vGridList[i].asChar());
         }
         AiNodeSetArray( volume, "velocity_grids", ary);
      }      
      
      AiNodeDeclare( volume, "velocity_scale", "constant FLOAT" );
      AiNodeSetFlt(volume, "velocity_scale", m_DagNode.findPlug("velocityScale", true).asFloat());
      
      AiNodeDeclare( volume, "velocity_fps", "constant FLOAT" );
      AiNodeSetFlt(volume, "velocity_fps", m_DagNode.findPlug("velocityFps", true).asFloat());
      
      AiNodeDeclare( volume, "motion_start", "constant FLOAT" );
      AiNodeSetFlt(volume, "motion_start", m_DagNode.findPlug("motionStart", true).asFloat());
      
      AiNodeDeclare( volume, "motion_end", "constant FLOAT" );
      AiNodeSetFlt(volume, "motion_end", m_DagNode.findPlug("motionEnd", true).asFloat());
      
      AiNodeDeclare( volume, "velocity_outlier_threshold", "constant FLOAT" );
      AiNodeSetFlt(volume, "velocity_outlier_threshold", m_DagNode.findPlug("velocityThreshold", true).asFloat());         
         
      AiNodeSetBool(volume, "disable_ray_extents", m_DagNode.findPlug("disableRayExtents", true).asBool());
      AiNodeSetBool(volume, "compress", m_DagNode.findPlug("compress", true).asBool());
      AiNodeSetFlt(volume, "volume_padding", m_DagNode.findPlug("volumePadding", true).asFloat());

      const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(volume));
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
   return volume;
}

void CArnoldVolumeTranslator::RequestUpdate()
{
   SetUpdateMode(AI_RECREATE_NODE);
   CShapeTranslator::RequestUpdate();
}