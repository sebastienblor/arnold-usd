#include "UsdProxyShapeTranslator.h"

#include <maya/MRenderLineArray.h>
#include <maya/MRenderLine.h>
#include <maya/MDagPathArray.h>
#include <maya/MPointArray.h>
#include <maya/MHairSystem.h>
#include <maya/MFnPfxGeometry.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MAnimControl.h>
#include <maya/MSelectionList.h>
#include <maya/MDGModifier.h>
#include <maya/MDGMessage.h>
#include <maya/MEventMessage.h>

#include <algorithm>
#include <string>

void CUsdProxyShapeTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "usd");
   CShapeTranslator::MakeCommonAttributes(helper);

   helper.MakeInput("namespace");
   //// operators
   CAttrData data;
   data.name = "operators";
   data.shortName = "operators";
   data.type = AI_TYPE_NODE;
   data.isArray = true;
   helper.MakeInput(data);

}

AtNode* CUsdProxyShapeTranslator::CreateArnoldNodes()
{

   return AddArnoldNode("usd");
}

// Callback is called whenever a gpuCache node is created
void CUsdProxyShapeTranslator::timeChangedCallback(void* clientData)
{

   CUsdProxyShapeTranslator * translator = static_cast< CUsdProxyShapeTranslator* >(clientData);
   if (translator != NULL)
   {
      // translator->SetUpdateMode(AI_RECREATE_NODE);
      translator->RequestUpdate();
   }

}

void CUsdProxyShapeTranslator::AddUpdateCallbacks()
{

   MStatus status;
   MCallbackId id;

   MObject object = GetMayaObject();
   id = MEventMessage::addEventCallback("timeChanged", 
                                        CUsdProxyShapeTranslator::timeChangedCallback,
                                        this,
                                        &status);

   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   CShapeTranslator::AddUpdateCallbacks();
}

void CUsdProxyShapeTranslator::Export( AtNode *shape )
{
   if (shape == NULL)
      return;

   // ExportMatrix(shape);
   // ProcessRenderFlags(shape);

   // export gpuCache parameters   
   MPlug filenamePlug = FindMayaPlug("filePath");
   if (!filenamePlug.isNull())
   {
      MString filename = filenamePlug.asString().expandEnvironmentVariablesAndTilde();
      GetSessionOptions().FormatProceduralPath(filename);
      AiNodeSetStr(shape, "filename", filename.asChar());
   }

   MPlug geomPlug = FindMayaPlug("primPath");
   if (!geomPlug.isNull())
   {
      std::string geomPath = geomPlug.asString().asChar();
      std::replace( geomPath.begin(), geomPath.end(), '|', '/'); // Maya converts '/' into '|' 
      AiNodeSetStr(shape, "object_path", geomPath.c_str());
   } 

   if (RequiresShaderExport())
      ExportShaders();

   MTime curTime = MAnimControl::currentTime();

   AiNodeSetFlt(shape, "frame", float(FindMayaPlug("time").asFloat()));
  
   ExportProcedural(shape);
}


void CUsdProxyShapeTranslator::ExportShaders()
{
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

void CUsdProxyShapeTranslator::ExportMotion(AtNode *shape)
{
   if (shape == NULL)
      return;
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(shape);

}

void CUsdProxyShapeTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   m_attrChanged = true; // this flag tells me that I've been through a NodeChanged call
   MString plugName = plug.partialName(false, false, false, false, false, true);

   if (plugName == "selectedItems" || plugName == "selected_items" || 
      plugName == "MinBoundingBox0" || plugName == "MinBoundingBox1" || plugName == "MinBoundingBox2" || 
      plugName == "MaxBoundingBox0" || plugName == "MaxBoundingBox1" || plugName == "MaxBoundingBox2") return;

   // we're calling directly the shape translator function, as we don't want to make it a AI_RECREATE_NODE
   CShapeTranslator::NodeChanged(node, plug);  
}
