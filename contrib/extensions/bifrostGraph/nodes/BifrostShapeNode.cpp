
#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif

#include <maya/MPxSurfaceShape.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MRenderUtil.h>
#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MDGModifier.h>
#include <maya/MStatus.h>
#include <maya/MFloatVector.h>
#include <maya/MSelectionList.h>
#include <maya/MDrawRequest.h>
#include <maya/M3dView.h>
#include <maya/MDrawData.h>
#include <maya/MDrawInfo.h>
#include <maya/MSelectInfo.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnStringArrayData.h>
#include <maya/MDrawRegistry.h>

#include "BifrostShapeNode.h"
#include "../plugin/BifrostUtils.h"
#include "extension/ExtensionsManager.h"
#include "extension/Extension.h"
#include "utils/Universe.h"
#include "viewport2/ArnoldProceduralSubSceneOverride.h"

#include <ai_node_entry.h>
#include <ai_params.h>
#include <ai_metadata.h>

MTypeId CBifrostShapeNode::id(0x00115DBD);
MString CBifrostShapeNode::s_classification("drawdb/subscene/arnold/procedural/bifrostGraphStandin"); 
CAbMayaNode CBifrostShapeNode::s_abstract;
MObject CBifrostShapeNode::s_outputBifrostDataStream;
MObject CBifrostShapeNode::s_input;


CBifrostShapeNode::CBifrostShapeNode() : CArnoldBaseProcedural()
{
   m_data = new CArnoldProceduralData();
}

void CBifrostShapeNode::postConstructor()
{
   // FIXME do we want to set the "mode" attribute to something else than "Bounding box" ?

   CArnoldBaseProcedural::postConstructor();
   // This call allows the shape to have shading groups assigned
   setRenderable(true);

   // TODO: use a metadata to define this
   setExistWithoutInConnections(true);
   setExistWithoutOutConnections(true);
   // No compute anyway
   setMPSafe(false);
   // Copy the abstract so that it can accessed on instances
   // (and saved before a new register overwrites it)
   m_abstract = s_abstract;
}


void* CBifrostShapeNode::creator()
{
   return new CBifrostShapeNode();
}

MStatus CBifrostShapeNode::initialize()
{
   MFnMessageAttribute mAttr;
   MFnTypedAttribute tAttr;

   s_input = mAttr.create("inputData", "inputData");
   mAttr.setStorable(true);
   mAttr.setReadable(true);
   mAttr.setWritable(true);
   addAttribute(s_input);

   s_outputBifrostDataStream = tAttr.create("outputBifrostDataStream", "os", MFnData::kDoubleArray, MObject::kNullObj);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   tAttr.setWritable(true);
   addAttribute(s_outputBifrostDataStream);

   CArnoldBaseProcedural::initializeCommonAttributes();

   MHWRender::MDrawRegistry::registerSubSceneOverrideCreator(
      s_classification,
      MString("aiBifrostGraphStandinOverride"),
      CArnoldProceduralSubSceneOverride::Creator);

   return MS::kSuccess;
}

MStatus CBifrostShapeNode::setDependentsDirty( const MPlug& plug, MPlugArray& plugArray)
{
   if (m_data)
      m_data->m_isDirty = true;

   // Signal to VP2 that we require an update. By default, do it for any attribute
   MHWRender::MRenderer::setGeometryDrawDirty(thisMObject());

   return MS::kSuccess;
}

void CBifrostShapeNode::updateGeometry()
{
   MObject connectedNode;
   bool foundBifrostGraph = false;
   MPlug inputPlug(thisMObject(), s_input);
   if (!inputPlug.isNull())
   {
      MPlugArray conns;
      inputPlug.connectedTo(conns, true, false);
      if (conns.length() > 0)
      {
         connectedNode = conns[0].node();
         foundBifrostGraph = true; // FIXME verify that it's really a bifrost graph
      }
   }


   bool universeCreated = ArnoldUniverseBegin();
   
   AtUniverse *universe = AiUniverse();
   AtUniverse *proc_universe = AiUniverse();

   if (!BifrostUtils::LoadBifrostProcedural())
      AiMsgError("Bifrost procedural could not be found");
   
   // This is an exact replica of what's happening when in the BifShapeTranslator
   // Only difference being, I'm not setting any motion steps
   
   // TODO : Factorize the bit below 

   AtNode *proc = AiNode(proc_universe,"bifrost_graph", name().asChar());
   MFnDependencyNode fnNode(connectedNode);
   MPlug bifrostData  = fnNode.findPlug("outputBifrostDataStream", true);
   MDataHandle dataStreamHandle;
   bifrostData.getValue(dataStreamHandle);
   MFnDoubleArrayData arrData(dataStreamHandle.data());
   MDoubleArray streamData = arrData.array();
   unsigned int nEle = streamData.length();
   size_t nBytes = static_cast<size_t>(nEle) * sizeof(double);
   AtArray *inputsArray = AiArray(1, 1, AI_TYPE_STRING, "input0");
   AiNodeSetArray(proc, "input_names", inputsArray);
   void* handle = BifrostUtils::s_arnoldBifrostPrepareHandle(&streamData[0], nBytes);
   AtArray *interpsArray = AiArray(1, 1, AI_TYPE_STRING, "handle");
   AtArray *dataArray = AiArrayAllocate(1, 1, AI_TYPE_POINTER);
   AiArraySetPtr(dataArray, 0, handle);
   AiNodeDeclare(proc, "bifrost:input0", "constant ARRAY POINTER");
   AiNodeSetArray(proc, "bifrost:input0", dataArray);
   AiNodeSetArray(proc, "input_interpretations", interpsArray);

/*
   MObject me = thisMObject();   
   CNodeTranslator *translator = CMayaScene::GetArnoldSession()->ExportNodeToUniverse(me, proc_universe);
   
   AtNode *proc = (translator) ? translator->GetArnoldNode() : NULL;*/
   AtProcViewportMode viewport_mode = AI_PROC_BOXES;
   switch (m_data->m_mode)
   {
      case DM_BOUNDING_BOX:
      case DM_PER_OBJECT_BOUNDING_BOX:
         viewport_mode = AI_PROC_BOXES;
         break;
      case DM_POLYWIRE: // filled polygon
      case DM_WIREFRAME: // wireframe
      case DM_SHADED_POLYWIRE: // shaded polywire
      case DM_SHADED: // shaded
         viewport_mode = AI_PROC_POLYGONS;
         break;
      case DM_POINT_CLOUD: // points
         viewport_mode = AI_PROC_POINTS;
         break;
   }

   // get the proc geo in a new universe
   // add param value for "procedural_only" (what for ?)AD8e33
   AtParamValueMap* paramValues = AiParamValueMap();
   AiParamValueMapSetBool(paramValues, AtString("procedural_only"), true);  
   AiParamValueMapSetInt(paramValues, AtString("recursion_level"), 0);  
   AiProceduralViewport(proc, universe, AI_PROC_POLYGONS, paramValues);
   DrawUniverse(universe);

   AiParamValueMapDestroy(paramValues);
   AiUniverseDestroy(universe);
   AiUniverseDestroy(proc_universe);

   if (universeCreated)
      AiEnd();
}
