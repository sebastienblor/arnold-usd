#include "ArnoldLightLinks.h"

#include "SessionOptions.h"

#include <maya/MSelectionList.h>
#include <maya/MObject.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>
#include <maya/MFnSet.h>

#include <ai.h>

#include <algorithm>
#include <cstring>

void CArnoldLightLinks::ClearLightLinks()
{
   m_arnoldLights.clear();
   m_numArnoldLights = 0;
   m_linkedLights.clear();
   
   m_lightMode = MTOA_LIGHTLINK_NONE;
   m_lightMode = MTOA_SHADOWLINK_NONE;
}

void CArnoldLightLinks::SetLinkingMode(int light, int shadow)
{
   m_lightMode = light;
   m_shadowMode = shadow;
}

void CArnoldLightLinks::ParseLightLinks()
{
   AtNodeIterator* niter = AiUniverseGetNodeIterator(AI_NODE_LIGHT);
   while(!AiNodeIteratorFinished(niter))
   {
      AtNode* node = AiNodeIteratorGetNext(niter);
      m_arnoldLights[AiNodeGetName(node)] = node;
   }
   AiNodeIteratorDestroy(niter);
   m_numArnoldLights = m_arnoldLights.size();
   if (m_numArnoldLights)
      m_linkedLights.resize(m_numArnoldLights);
}

void CArnoldLightLinks::AppendLightsToList(MFnDependencyNode& linkedLights, 
        size_t& numLinkedLights)
{
   if (linkedLights.typeName() == MString("objectSet"))
   {
      MStatus status;
      MFnSet objectSet(linkedLights.object());
      MSelectionList sList;
      objectSet.getMembers(sList, true);
      for (unsigned int i = 0; i < sList.length(); ++i)
      {
         MDagPath dgPath;
         if (!sList.getDagPath(i, dgPath))
            continue;
         dgPath.extendToShape();
         MFnDependencyNode linkedLight(dgPath.node(), &status);
         if (!status)
            continue;
         std::map<std::string, AtNode*>::iterator it = m_arnoldLights.find(linkedLight.name().asChar());
         if (it != m_arnoldLights.end())
            m_linkedLights[numLinkedLights++] = it->second;
      }
   }
   else
   {
      std::map<std::string, AtNode*>::iterator it = m_arnoldLights.find(linkedLights.name().asChar());
      if (it != m_arnoldLights.end())
         m_linkedLights[numLinkedLights++] = it->second;
   }
}

void CArnoldLightLinks::HandleLightLinker(MPlug& conn, size_t& numLinkedLights, LightLinkMode& linkMode)
{
      MPlug parentPlug = conn.parent();
      const char* plugName = parentPlug.partialName(false, false, false, false, false, true).asChar();

      static MPlugArray conns2;
      parentPlug.child(0).connectedTo(conns2, true, false);

      if (conns2.length() == 0)
         return;

      if ((linkMode == MTOA_LIGHTLINK_LINK) && (strncmp(plugName, "link", 4) == 0)) // linking the light
      {
         // check for this being a light set
         MFnDependencyNode linkedLights(conns2[0].node());
         AppendLightsToList(linkedLights, numLinkedLights);
      }
      else if (strncmp(plugName, "ignore", 6) == 0) // ignoring the light
      {
         linkMode = MTOA_LIGHTLINK_IGNORE;
         MFnDependencyNode linkedLights(conns2[0].node());
         AppendLightsToList(linkedLights, numLinkedLights);
      }
}

void CArnoldLightLinks::CheckMessage(MFnDependencyNode& dNode, size_t& numLinkedLights, LightLinkMode& linkMode)
{
   MPlug messagePlug = dNode.findPlug("message");
   static MPlugArray conns;
   messagePlug.connectedTo(conns, false, true);
   unsigned int numConnections = conns.length();   
   for (unsigned int i = 0; i < numConnections; ++i)
   {
      MPlug conn = conns[i];
      if (conn.node().hasFn(MFn::kLightLink))
      {
         HandleLightLinker(conn, numLinkedLights, linkMode);
      }
   }
}

void CArnoldLightLinks::ExportLightLinking(AtNode* shape, MFnDependencyNode& dNode)
{
   if ((m_lightMode == MTOA_LIGHTLINK_NONE && m_shadowMode == MTOA_SHADOWLINK_NONE) ||
           m_numArnoldLights == 0)
      return;
   // there are two modes, either ignoring specific nodes
   // or linking them, but it's always only one of them
   MStatus status;
   LightLinkMode linkMode = MTOA_LIGHTLINK_LINK;
   size_t numLinkedLights = 0;
   
   CheckMessage(dNode, numLinkedLights, linkMode); // checking the outgoing message
   // for the node
   
   MPlug instObjGroupsPlug = dNode.findPlug("instObjGroups", &status);
   instObjGroupsPlug = instObjGroupsPlug.elementByLogicalIndex(0);
   static MPlugArray conns;
   instObjGroupsPlug.connectedTo(conns, false, true);
   unsigned int numConnections = conns.length();   
   for (unsigned int i = 0; i < numConnections; ++i)
   {
      MPlug conn = conns[i];
      if (conn.node().hasFn(MFn::kShadingEngine))
      {
         MFnDependencyNode shadingEngineNode(conn.node());
         CheckMessage(shadingEngineNode, numLinkedLights, linkMode); // checking the outgoing message
         // for the shadingEngine
      }
   }
   
   if (linkMode == MTOA_LIGHTLINK_IGNORE)
   {
      if (numLinkedLights != 0)
      {
         AiNodeSetBool(shape, "use_light_group", true);
         AtArray* lights = AiArrayAllocate(m_numArnoldLights - numLinkedLights, 1, AI_TYPE_NODE);
         AiNodeSetArray(shape, "light_group", lights);
         if (m_numArnoldLights != numLinkedLights) // if the number is the same, then all of the lights are ignored
         {
            for (std::map<std::string, AtNode*>::iterator it = m_arnoldLights.begin(); it != m_arnoldLights.end(); ++it)
            {
               AtNode* light = it->second;
               // the light is not ignored, so we can add it to the array
               std::vector<AtNode*>::iterator itEnd = m_linkedLights.begin() + numLinkedLights;
               if (std::find(m_linkedLights.begin(), itEnd, light) == itEnd)
                  AiArraySetPtr(lights, --numLinkedLights, light); // just a trick, so no other variable is required
            }
         }
      }
   }
   else
   {
      if (numLinkedLights != m_numArnoldLights)
      {
         AiNodeSetBool(shape, "use_light_group", true);
         AtArray* lights = AiArrayAllocate(numLinkedLights, 1, AI_TYPE_NODE);
         AiNodeSetArray(shape, "light_group", lights);
         for (size_t i = 0; i < numLinkedLights; ++i)
            AiArraySetPtr(lights, i, m_linkedLights[i]);         
      }
   }
}
