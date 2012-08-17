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
   m_linkedShadows.clear();
   
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
   {
      m_linkedLights.resize(m_numArnoldLights);
      m_linkedShadows.resize(m_numArnoldLights);
   }
}

void CArnoldLightLinks::AppendNodesToList(MFnDependencyNode& linkedNodes, std::vector<AtNode*>& nodeList, 
        size_t& numLinkedNodes)
{
   if (linkedNodes.typeName() == MString("objectSet"))
   {
      MStatus status;
      MFnSet objectSet(linkedNodes.object());
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
         if (it == m_arnoldLights.end())
            it = m_arnoldLights.find(dgPath.fullPathName().asChar() + 1); //if the shapeName is not unique we are using the full path name
         if (it != m_arnoldLights.end())
         {
            std::vector<AtNode*>::iterator itEnd = nodeList.begin() + numLinkedNodes;
            if (std::find(nodeList.begin(), itEnd, it->second) == itEnd)
               nodeList[numLinkedNodes++] = it->second;
         }
      }
   }
   else
   {
      std::map<std::string, AtNode*>::iterator it = m_arnoldLights.find(linkedNodes.name().asChar());
      if (it == m_arnoldLights.end())
      {
         MDagPath dgPath;
         MDagPath::getAPathTo(linkedNodes.object(), dgPath);
         it = m_arnoldLights.find(dgPath.fullPathName().asChar() + 1); //if the shapeName is not unique we are using the full path name
      }
      if (it != m_arnoldLights.end())
      {
         std::vector<AtNode*>::iterator itEnd = nodeList.begin() + numLinkedNodes;
         if (std::find(nodeList.begin(), itEnd, it->second) == itEnd)
            nodeList[numLinkedNodes++] = it->second;
      }
   }
}

void CArnoldLightLinks::HandleLightLinker(MPlug& conn, 
        size_t& numLinkedLights, size_t& numLinkedShadows,
        NodeLinkMode& lightLinkMode, NodeLinkMode& shadowLinkMode)
{
   MPlug parentPlug = conn.parent();
   std::string plugName = parentPlug.partialName(false, false, false, false, false, true).asChar();
   // sometimes this buffer is invalidated

   static MPlugArray conns2;
   parentPlug.child(0).connectedTo(conns2, true, false);

   if (conns2.length() == 0)
      return;

   if (m_lightMode == MTOA_LIGHTLINK_MAYA)
   {
      if ((lightLinkMode == MTOA_NODELINK_LINK) && (strncmp(plugName.c_str(), "link", 4) == 0)) // linking the light
      {
         // check for this being a light set
         MFnDependencyNode linkedLights(conns2[0].node());
         AppendNodesToList(linkedLights, m_linkedLights, numLinkedLights);
      }
      else if (strncmp(plugName.c_str(), "ignore", 6) == 0) // ignoring the light
      {
         lightLinkMode = MTOA_NODELINK_IGNORE;
         MFnDependencyNode linkedLights(conns2[0].node());
         AppendNodesToList(linkedLights, m_linkedLights, numLinkedLights);
      }
   }

   if (m_shadowMode == MTOA_SHADOWLINK_MAYA)
   {
      if ((shadowLinkMode == MTOA_NODELINK_LINK) && (strncmp(plugName.c_str(), "shadowLink", 10) == 0)) // linking the shadow
      {
         // check for this being a light set
         MFnDependencyNode linkedShadows(conns2[0].node());
         AppendNodesToList(linkedShadows, m_linkedShadows, numLinkedShadows);
      }
      else if (strncmp(plugName.c_str(), "shadowIgnore", 12) == 0) // ignoring the shadow
      {
         shadowLinkMode = MTOA_NODELINK_IGNORE;
         MFnDependencyNode linkedShadows(conns2[0].node());
         AppendNodesToList(linkedShadows, m_linkedShadows, numLinkedShadows);
      }
   }
}

void CArnoldLightLinks::CheckMessage(MFnDependencyNode& dNode, 
        size_t& numLinkedLights, size_t& numLinkedShadows, 
        NodeLinkMode& lightLinkMode, NodeLinkMode& shadowLinkMode)
{
   MPlug messagePlug = dNode.findPlug("message");
   static MPlugArray conns;
   messagePlug.connectedTo(conns, false, true);
   unsigned int numConnections = conns.length();   
   for (unsigned int i = 0; i < numConnections; ++i)
   {
      MPlug conn = conns[i];
      if (conn.node().hasFn(MFn::kLightLink))
         HandleLightLinker(conn, numLinkedLights, numLinkedShadows, lightLinkMode, shadowLinkMode);
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
   NodeLinkMode lightLinkMode = MTOA_NODELINK_LINK;
   NodeLinkMode shadowLinkMode = MTOA_NODELINK_LINK;
   size_t numLinkedLights = 0;
   size_t numLinkedShadows = 0;
   
   CheckMessage(dNode, numLinkedLights, numLinkedShadows, lightLinkMode, shadowLinkMode); // checking the outgoing message
   // for the node
   
   MPlug instObjGroupsPlug = dNode.findPlug("instObjGroups", &status);
   instObjGroupsPlug = instObjGroupsPlug.elementByLogicalIndex(0);
   static MPlugArray conns;
   instObjGroupsPlug.connectedTo(conns, false, true);
   unsigned int numConnections = conns.length();
   // check for the per face assignment light linking
   // only check for the first one
   // because we don't support per face light linking
   if (numConnections == 0)
   {
      instObjGroupsPlug = instObjGroupsPlug.child(0, &status);
      if (status)
      {
         instObjGroupsPlug = instObjGroupsPlug.elementByLogicalIndex(0);
         instObjGroupsPlug.connectedTo(conns, false, true);
         numConnections = conns.length();
      }
   }
   for (unsigned int i = 0; i < numConnections; ++i)
   {      
      MPlug conn = conns[i];
      if (conn.node().hasFn(MFn::kShadingEngine))
      {
         MFnDependencyNode shadingEngineNode(conn.node());
         CheckMessage(shadingEngineNode, numLinkedLights, numLinkedShadows, lightLinkMode, shadowLinkMode); 
         // checking the outgoing message
         // for the shadingEngine
      }
   }
   
   if (m_lightMode == MTOA_LIGHTLINK_MAYA)
   {   
      if (lightLinkMode == MTOA_NODELINK_IGNORE)
      {
         if (numLinkedLights != 0)
         {
            AiNodeSetBool(shape, "use_light_group", true);
            size_t numNonIgnoredLights = m_numArnoldLights - numLinkedLights;
            AtArray* lights = AiArrayAllocate(numNonIgnoredLights, 1, AI_TYPE_NODE);
            AiNodeSetArray(shape, "light_group", lights);
            if (numNonIgnoredLights)
            {
               std::vector<AtNode*>::iterator itEnd = m_linkedLights.begin() + numLinkedLights;
               for (std::map<std::string, AtNode*>::iterator it = m_arnoldLights.begin(); it != m_arnoldLights.end(); ++it)
               {
                  AtNode* light = it->second;
                  // the light is not ignored, so we can add it to the array                  
                  if (std::find(m_linkedLights.begin(), itEnd, light) == itEnd)
                     AiArraySetPtr(lights, --numNonIgnoredLights, light);
               }
            }
            if (m_shadowMode == MTOA_SHADOWLINK_LIGHT)
            {
               AiNodeSetBool(shape, "use_shadow_group", true);
               AiNodeSetArray(shape, "shadow_group", AiArrayCopy(lights));
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
            if (m_shadowMode == MTOA_SHADOWLINK_LIGHT)
            {
               AiNodeSetBool(shape, "use_shadow_group", true);
               AiNodeSetArray(shape, "shadow_group", AiArrayCopy(lights));
            }
         }
      }
   }
   
   if (m_shadowMode == MTOA_SHADOWLINK_MAYA)
   {
      if (shadowLinkMode == MTOA_NODELINK_IGNORE)
      {
         if (numLinkedShadows != 0)
         {
            AiNodeSetBool(shape, "use_shadow_group", true);
            size_t numNonIgnoredShadows = m_numArnoldLights - numLinkedShadows;
            AtArray* lights = AiArrayAllocate(numNonIgnoredShadows, 1, AI_TYPE_NODE);
            AiNodeSetArray(shape, "shadow_group", lights);
            if (numNonIgnoredShadows)
            {
               std::vector<AtNode*>::iterator itEnd = m_linkedShadows.begin() + numLinkedShadows;
               for (std::map<std::string, AtNode*>::iterator it = m_arnoldLights.begin(); it != m_arnoldLights.end(); ++it)
               {
                  AtNode* light = it->second;
                  // the light is not ignored, so we can add it to the array                  
                  if (std::find(m_linkedShadows.begin(), itEnd, light) == itEnd)
                     AiArraySetPtr(lights, --numNonIgnoredShadows, light);
               }
            }
         }
      }
      else
      {
         if (numLinkedShadows != m_numArnoldLights)
         {
            AiNodeSetBool(shape, "use_shadow_group", true);
            AtArray* lights = AiArrayAllocate(numLinkedShadows, 1, AI_TYPE_NODE);
            AiNodeSetArray(shape, "shadow_group", lights);
            for (size_t i = 0; i < numLinkedShadows; ++i)
               AiArraySetPtr(lights, i, m_linkedShadows[i]);
         }
      }
   }
}
