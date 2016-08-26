#include "VolumeTranslator.h"
#include "translators/NodeTranslatorImpl.h"
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
      AtNode * tmpRes = AddArnoldNode("volume");
      return  tmpRes;
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
   AtNode* masterNode = AiNodeLookUpByName(masterInstance.partialPathName().asChar());

   AiNodeSetStr(instance, "name", m_dagPath.partialPathName().asChar());

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
      if (meshShaders.size() > 0)
      {
         AiNodeDeclare(volume, "mtoa_shading_groups", "constant ARRAY NODE");
         AiNodeSetArray(volume, "mtoa_shading_groups",
                        AiArrayConvert(meshShaders.size(), 1, AI_TYPE_NODE, &(meshShaders[0])));
      }
   }
}

void CArnoldVolumeTranslator::ExportBoundingBox(AtNode* volume)
{
   AiNodeSetPnt(volume, "min",
                        m_DagNode.findPlug("min").child(0).asFloat(),
                        m_DagNode.findPlug("min").child(1).asFloat(),
                        m_DagNode.findPlug("min").child(2).asFloat());
                        
   AiNodeSetPnt(volume, "max",
                        m_DagNode.findPlug("max").child(0).asFloat(),
                        m_DagNode.findPlug("max").child(1).asFloat(),
                        m_DagNode.findPlug("max").child(2).asFloat());
}


AtNode* CArnoldVolumeTranslator::ExportVolume(AtNode* volume, bool update)
{
   m_DagNode.setObject(m_dagPath.node());

   AiNodeSetStr(volume, "name", m_dagPath.partialPathName().asChar());

   ExportMatrix(volume);
   ProcessRenderFlags(volume);
   
   ExportVolumeShaders(volume);
   
   ExportLightLinking(volume);
   

   if (!update)
   {
     
      AiNodeSetFlt(volume, "step_size", m_DagNode.findPlug("stepSize").asFloat());

      MPlug loadAtInit = m_DagNode.findPlug("loadAtInit");
      if (loadAtInit.asBool())
         AiNodeSetBool(volume, "load_at_init", true);
      
      ExportBoundingBox(volume);

      int type = m_DagNode.findPlug("type").asInt();
      
      if(type == VT_CUSTOM)
      {
         MString dso = m_DagNode.findPlug("dso").asString().expandEnvironmentVariablesAndTilde();
      
         unsigned int nchars = dso.numChars();
         if (nchars > 3 && dso.substringW(nchars-3, nchars) == ".so")
         {
            dso = dso.substringW(0, nchars-4)+LIBEXT;
         }
         else if (nchars > 4 && dso.substringW(nchars-4, nchars) == ".dll")
         {
            dso = dso.substringW(0, nchars-5)+LIBEXT;
         }
         else if (nchars > 6 && dso.substringW(nchars-6, nchars) == ".dylib")
         {
            dso = dso.substringW(0, nchars-7)+LIBEXT;
         }
         
         AiNodeSetStr(volume, "dso", dso.asChar());
      
         MPlug data = m_DagNode.findPlug("data");
         int sizeData = strlen(data.asString().asChar());
         if (sizeData != 0)
         {
            AiNodeSetStr(volume, "data", data.asString().expandEnvironmentVariablesAndTilde().asChar());
         }
      }
      else
      {
         MString openVDB = MString(getenv("MTOA_PATH")) + MString("/procedurals/volume_openvdb.so");
         
         AiNodeSetStr(volume, "dso", openVDB.expandEnvironmentVariablesAndTilde().asChar());
         
         MString filename = m_DagNode.findPlug("filename").asString();
         int start = 0;
         int end = 0;
         MString newFilename = "";
         char frameExt[64];

         start = filename.index('#');
         end = filename.rindex('#');
         
         if(start >= 0)
         {
            sprintf(frameExt, "%0*d", end - start + 1, m_DagNode.findPlug("frame").asInt());
            newFilename = filename.substring(0,start-1) + frameExt + filename.substring(end+1,filename.length());
         }
         else
         {
            newFilename = filename;
         }
         
         AiNodeDeclare( volume, "filename", "constant STRING" );
         AiNodeSetStr( volume, "filename", newFilename.expandEnvironmentVariablesAndTilde().asChar() );
         
         MString grids = m_DagNode.findPlug("grids").asString();
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
         
         MString vGrids = m_DagNode.findPlug("velocityGrids").asString();
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
         AiNodeSetFlt(volume, "velocity_scale", m_DagNode.findPlug("velocityScale").asFloat());
         
         AiNodeDeclare( volume, "velocity_fps", "constant FLOAT" );
         AiNodeSetFlt(volume, "velocity_fps", m_DagNode.findPlug("velocityFps").asFloat());
         
         AiNodeDeclare( volume, "velocity_shutter_start", "constant FLOAT" );
         AiNodeSetFlt(volume, "velocity_shutter_start", m_DagNode.findPlug("velocityShutterStart").asFloat());
         
         AiNodeDeclare( volume, "velocity_shutter_end", "constant FLOAT" );
         AiNodeSetFlt(volume, "velocity_shutter_end", m_DagNode.findPlug("velocityShutterEnd").asFloat());
         
         AiNodeDeclare(volume, "bounds_slack", "constant FLOAT" );
         AiNodeSetFlt(volume, "bounds_slack", m_DagNode.findPlug("padding").asFloat());
         
      }

   }
   return volume;
}
