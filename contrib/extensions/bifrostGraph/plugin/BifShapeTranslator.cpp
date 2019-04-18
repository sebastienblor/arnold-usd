#include "BifShapeTranslator.h"

#include <bifData.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MProfiler.h>
#include <maya/MSelectionContext.h>
#include <maya/MPlug.h>
#include <maya/MDataHandle.h>
#include <maya/MFnPluginData.h>

static bool s_bifrostBoardSupported = false;


void CBifShapeTranslator::NodeInitializer(CAbTranslator context)
{
   s_bifrostBoardSupported = (AiNodeEntryLookUp("bifrost_graph") != NULL);
   if (!s_bifrostBoardSupported)
      return;

   CExtensionAttrHelper helper(context.maya, "bifrost_graph");
   CShapeTranslator::MakeCommonAttributes(helper);

   // bifrost_board procedural parameters
   helper.MakeInput("filename");
   helper.MakeInput("compound");

   // procedural namespace parameter
   helper.MakeInput("namespace");

   //// operators attribute for adding operators to custom procedurals
   CAttrData data;
   data.name = "operators";
   data.shortName = "operators";
   data.type = AI_TYPE_NODE;
   data.isArray = true;
   helper.MakeInput(data);
}

AtNode* CBifShapeTranslator::CreateArnoldNodes()
{
   if (!s_bifrostBoardSupported) return NULL;

   return AddArnoldNode("bifrost_graph");
}

void CBifShapeTranslator::Export( AtNode *shape )
{

   // // test code taken from bifSubSCeneOverride.h in maya-bifrostboard-plugin
   // MFnDagNode dagNode(m_dagPath.node()); // get the bifShape node
   // MPlug viewport = FindMayaPlug("viewport");
   // MDataHandle handle;
   // plug.getValue(handle);
   // MFnPluginData fnData(handle.data());
   // BifData* data = static_cast<BifData*>(fnData.data(&status));
   // if (data)
   //   {
   //       if (!data->empty()) // only one type of data (BifrostExp::Object) so if not empty it must be it!
   //       {
   //           unsigned int index = 1;
   //           for (auto iter = data->bifrostObjects().begin(); iter != data->bifrostObjects().end(); ++iter, ++index)
   //           {
   //               Amino::Ptr< const BifrostExp::Object > bifrostObj = *iter;
   //           }
   //       }
   //    }

   // export BifShape parameters
   MPlug filenamePlug = FindMayaPlug("aiFilename");
   if (!filenamePlug.isNull())
   {
      MString filename = filenamePlug.asString();
      AiNodeSetStr(shape, "filename", filename.asChar());
   }

   MPlug geomPlug = FindMayaPlug("aiCompound");
   if (!geomPlug.isNull())
   {
      MString geomPath = geomPlug.asString();
      AiNodeSetStr(shape, "compound", geomPath.asChar());
   }

   if (RequiresShaderExport())
      ExportShaders();

   ExportProcedural(shape);
}


void CBifShapeTranslator::ExportShaders()
{
   if (s_bifrostBoardSupported == false)
      return;

   AtNode *node = GetArnoldNode();
   if (node == NULL)
      return;

   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroupPlug.isNull())
   {
      AtNode *shader = ExportConnectedNode(shadingGroupPlug);
      if (shader != NULL)
      {
         AiNodeSetPtr(node, "shader", shader);
      }
      else
      {
         AiNodeSetPtr(node, "shader", NULL);
      }
   }
}

void CBifShapeTranslator::ExportMotion(AtNode *shape)
{
   if (s_bifrostBoardSupported == false || shape == NULL)
      return;
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(shape);

}

