
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
#include <maya/MEventMessage.h>

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
MObject CBifrostShapeNode::s_input;
MCallbackId CBifrostShapeNode::s_NewNodeCallbackId = 0;
MCallbackId CBifrostShapeNode::s_idleCallbackId = 0;

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

   CArnoldBaseProcedural::initializeCommonAttributes();

   // If the env variable BIFROST_EXPERIMENTAL is set, 
   // we automatically create an arnoldBifrostShape node for 
   // every bifrostGraph in the scene. We need to add it to existing nodes
   // (unless they already have such a connection), and to all bifrost graphs that 
   // will be created later one.
   const char* envVar = getenv("BIFROST_EXPERIMENTAL");
   MString envVarStr = (envVar) ? MString(envVar) : MString("");
   if (envVarStr == MString("1"))
   {
      // First update the connections for existing bifrost graphs.
      // This can happen if MtoA is loaded after having created bifrost graphs.
      UpdateBifrostGraphConnections();
      // Now add a callback to be told every time a new "bifrostGraphShape" node is created 
      // in the scene
      MStatus status;
      MCallbackId id = MDGMessage::addNodeAddedCallback(NewNodeCallback, "bifrostGraphShape", NULL, &status);
      if (status == MS::kSuccess)
         s_NewNodeCallbackId = id;
         
   }

   return MS::kSuccess;
}

void CBifrostShapeNode::NewNodeCallback(MObject & node, void *)
{  
   // When a scene is opened with an already existing connection, 
   // we won't see this connection if we call "UpdateBifrostGraphConnections"
   // right away. We need to let maya finish what it's doing before checking for
   // existing connections. So we use an "idle" callback
   if (s_idleCallbackId == 0)
      s_idleCallbackId = MEventMessage::addEventCallback("idle",
                                    IdleCallback, nullptr);
}

void CBifrostShapeNode::IdleCallback(void *)
{
   if (s_idleCallbackId)
   {
      MMessage::removeCallback(s_idleCallbackId);
      s_idleCallbackId = 0;
   }

   UpdateBifrostGraphConnections();
}
void CBifrostShapeNode::UpdateBifrostGraphConnections()
{
   MStringArray bifrostNodes;
   MGlobal::executeCommand("ls -typ bifrostGraphShape", bifrostNodes); // get selected shading groups and export them
   for (unsigned int i = 0; i < bifrostNodes.length(); ++i)
   {      
      MStringArray connections; 
      MString connectionsCmd = MString("listConnections -d 1 -s 0 -type \"arnoldBifrostShape\" \"") + bifrostNodes[i] + MString(".message\"");
      MGlobal::executeCommand(connectionsCmd, connections);
      if (connections.length() > 0)
         continue;
      
      // At this point, no connection to an arnoldBifrostShape was found, let's add one
      MString arnoldNodeName;
      MString createCmd = MString("createNode \"arnoldBifrostShape\" -name \"arnold") + bifrostNodes[i] + MString("\"");
      MGlobal::executeCommand(createCmd, arnoldNodeName);

      // get the transform for both the bifrost graph and the arnold node, to parent 
      // the transforms and not the shapes
      MString listRelativesCmd = MString("listRelatives -p ") + arnoldNodeName;
      MStringArray arnoldTransformName;
      MGlobal::executeCommand(listRelativesCmd, arnoldTransformName);

      listRelativesCmd = MString("listRelatives -p ") + bifrostNodes[i];
      MStringArray bifrostTransformName;
      MGlobal::executeCommand(listRelativesCmd, bifrostTransformName);

      if (arnoldTransformName.length() > 0 && bifrostTransformName.length() > 0)
      {
         MString parentCmd = MString ("parent -r ") + arnoldTransformName[0] + MString(" ") + bifrostTransformName[0];
         MGlobal::executeCommand(parentCmd);
      }
      MString connectCmd = MString("connectAttr -f ") + bifrostNodes[i] + MString(".message ") + arnoldNodeName + MString(".inputData");
      MGlobal::executeCommand(connectCmd);

   }
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
         foundBifrostGraph = true; 
         MFnDependencyNode bifrostNode(connectedNode);
         if (bifrostNode.typeName() != MString("bifrostGraphShape"))
            return;
      }
   }

   if (!foundBifrostGraph)
      return;

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

   MPlug vpRenderPlug = fnNode.findPlug("viewportRenderSelect", true);
   if (!vpRenderPlug.isNull())
   {
      int vpRenderValue = vpRenderPlug.asInt();
      if (vpRenderValue == 0)
         vpRenderPlug.setValue(2);
      else if (vpRenderValue == 1)
         vpRenderPlug.setValue(3);
   }

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
   AiProceduralViewport(proc, universe, viewport_mode, paramValues);
   DrawUniverse(universe);

   AiParamValueMapDestroy(paramValues);
   AiUniverseDestroy(universe);
   AiUniverseDestroy(proc_universe);

   if (universeCreated)
      AiEnd();
}
