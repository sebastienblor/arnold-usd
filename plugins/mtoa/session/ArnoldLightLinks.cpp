#include "ArnoldLightLinks.h"

#include "SessionOptions.h"

#include <maya/MSelectionList.h>
#include <maya/MObject.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>

#include <tbb/tick_count.h>

#include <ai.h>

#include <algorithm>

void CArnoldLightLinks::ClearLightLinks()
{
   m_lightLinks.clear();
   m_shadowLinks.clear();
   
   m_lightMode = MTOA_LIGHTLINK_NONE;
   m_lightMode = MTOA_SHADOWLINK_NONE;
}

void CArnoldLightLinks::SetLinkingMode(int light, int shadow)
{
   m_lightMode = light;
   m_shadowMode = shadow;
}

void ReadLinks(const MPlug& plug, std::map<std::string, std::vector<AtNode*> >& target, 
        std::map<std::string, AtNode*>& lightMap)
{
   unsigned int numElements = plug.numElements();
   for (unsigned int i = 0; i < numElements; ++i)
   {
      MPlug cPlug = plug.elementByLogicalIndex(i);
      MPlugArray paLight, paObject;
      cPlug.child(0).connectedTo(paLight, true, false);
      cPlug.child(1).connectedTo(paObject, true, false);
      if (paLight.length() == 0 || paObject.length() == 0)
         continue;
      MFnDependencyNode light(paLight[0].node());
      MFnDependencyNode object(paObject[0].node());
      std::map<std::string, AtNode*>::iterator lightMapIter = lightMap.find(light.name().asChar());
      if (lightMapIter != lightMap.end())
         target[object.name().asChar()].push_back(lightMapIter->second);
   }
}

void AppendIgnores(std::map<std::string, std::vector<AtNode*> >& source,
        std::map<std::string, std::vector<AtNode*> >& target,
        std::vector<AtNode*> aLights)
{
   for (std::map<std::string, std::vector<AtNode*> >::iterator it = source.begin(); it != source.end(); ++it)
   {
      std::vector<AtNode*> lights;
      for(std::vector<AtNode*>::iterator it2 = aLights.begin(); it2 != aLights.end(); ++it2)
      {
         if (std::find(it->second.begin(), it->second.end(), *it2) == it->second.end())
            lights.push_back(*it2);
      }
      target[it->first] = lights;
   }
}

void CArnoldLightLinks::ParseLightLinks()
{
   std::map<std::string, AtNode*> lightMap;
   std::vector<AtNode*> lights;
   AtNodeIterator* niter = AiUniverseGetNodeIterator(AI_NODE_LIGHT);
   while(!AiNodeIteratorFinished(niter))
   {
      AtNode* node = AiNodeIteratorGetNext(niter);
      lights.push_back(node);
      lightMap[AiNodeGetName(node)] = node;
   }
   AiNodeIteratorDestroy(niter);
   
   MItDependencyNodes nodeIter(MFn::kLightLink);
   for (;!nodeIter.isDone(); nodeIter.next())
   {
      MFnDependencyNode dNode(nodeIter.item());
      std::map<std::string, std::vector<AtNode*> > tempMap;
      if (m_lightMode == MTOA_LIGHTLINK_MAYA)
      {
         ReadLinks(dNode.findPlug("link"), m_lightLinks, lightMap);               
         ReadLinks(dNode.findPlug("ignore"), tempMap, lightMap);         
         AppendIgnores(tempMap, m_lightLinks, lights);
         tempMap.clear();
      }
      
      if (m_shadowMode == MTOA_SHADOWLINK_MAYA)
      {
         ReadLinks(dNode.findPlug("shadowLink"), m_shadowLinks, lightMap);         
         ReadLinks(dNode.findPlug("shadowIgnore"), tempMap, lightMap);
         AppendIgnores(tempMap, m_shadowLinks, lights);
      }
   }
}

void CArnoldLightLinks::ExportLightLinking(AtNode* shape, MFnDependencyNode& dNode)
{
   const std::string name = dNode.name().asChar();
   if (m_lightMode == MTOA_LIGHTLINK_MAYA)
   {
      std::map<std::string, std::vector<AtNode*> >::iterator it = m_lightLinks.find(name);
   
      if (it != m_lightLinks.end())
      {
         AtArray* lights = AiArrayAllocate(it->second.size(), 1, AI_TYPE_NODE);
         unsigned int id = 0;
         for (std::vector<AtNode*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2, ++id)
            AiArraySetPtr(lights, id, *it2);
         AiNodeSetBool(shape, "use_light_group", true);
         AiNodeSetArray(shape, "light_group", lights);
         if (m_shadowMode == MTOA_SHADOWLINK_LIGHT)
         {
            AiNodeSetBool(shape, "use_shadow_group", true);
            if (id == 0)
               AiNodeSetArray(shape, "shadow_group", AiArrayAllocate(0, 1, AI_TYPE_NODE));
            else
               AiNodeSetArray(shape, "shadow_group", AiArrayCopy(lights));
         }
      }
   }
   
   if (m_shadowMode == MTOA_SHADOWLINK_MAYA)
   {
      std::map<std::string, std::vector<AtNode*> >::iterator it = m_shadowLinks.find(name);
   
      if (it != m_shadowLinks.end())
      {
         AtArray* lights = AiArrayAllocate(it->second.size(), 1, AI_TYPE_NODE);
         unsigned int id = 0;
         for (std::vector<AtNode*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2, ++id)
            AiArraySetPtr(lights, id, *it2);
         AiNodeSetBool(shape, "use_shadow_group", true);
         AiNodeSetArray(shape, "shadow_group", lights);
      }
   }
}
