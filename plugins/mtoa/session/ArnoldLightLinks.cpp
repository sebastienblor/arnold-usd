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

void CArnoldLightLinks::ClearLightLinks()
{
   m_arnoldLights.clear();
   
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
   const size_t numLights = m_arnoldLights.size();
   if (numLights)
      m_linkedLights.resize(numLights);
}

enum LightLinkMode{
   MTOA_LIGHTLINK_LINK,
   MTOA_LIGHTLINK_IGNORE
};

void CArnoldLightLinks::ExportLightLinking(AtNode* shape, MFnDependencyNode& dNode)
{
   if (m_lightMode == MTOA_LIGHTLINK_NONE && m_shadowMode == MTOA_SHADOWLINK_NONE)
      return;
   MPlug messagePlug = dNode.findPlug("message");
   static MPlugArray conns;
   messagePlug.connectedTo(conns, false, true);
   unsigned int numConnections = conns.length();
   if (numConnections == 0)
      return;
   // there are two modes, either ignoring specific nodes
   // or linking them, but it's always only one of them
   LightLinkMode linkMode = MTOA_LIGHTLINK_LINK;
   for (unsigned int i = 0; i < numConnections; ++i)
   {
      MPlug conn = conns[i];
      if (conn.node().hasFn(MFn::kLightLink))
      {
         std::cerr << "Found lightlink node\n";
      }
   }
}
