#include "ArnoldLightLinks.h"

#include <maya/MSelectionList.h>
#include <maya/MObject.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>

#include <tbb/tick_count.h>

#include <ai.h>

CArnoldLightLinks::CArnoldLightLinks()
{

}

CArnoldLightLinks::~CArnoldLightLinks()
{
   
}

void CArnoldLightLinks::ClearLightLinks()
{
   p_lights = 0;
   m_numLights = 0;
   m_tempLights.clear();
   m_ignores.clear();
   m_links.clear();
}

void CArnoldLightLinks::ParseLightLinks()
{
   ClearLightLinks();
   std::map<std::string, AtNode*> lightMap;
   AtNode** lights = AiUniverseGetLights();
   p_lights = lights;
   while (*lights)
   {
      AtNode* light = *(lights++);
      lightMap[AiNodeGetName(light)] = light;
      ++m_numLights;
   }
   if (m_numLights)
      m_tempLights.resize(m_numLights);
   
   tbb::tick_count tc = tbb::tick_count::now();
   m_links.clear();
   MItDependencyNodes nodeIter(MFn::kLightLink);
   std::cerr << "Parsing Light Links\n";
   for (;!nodeIter.isDone(); nodeIter.next())
   {
      MFnDependencyNode dNode(nodeIter.item());
      MPlug plug = dNode.findPlug("link"); // checking links
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
            m_links[object.name().asChar()].insert(lightMapIter->second);
      }
      plug = dNode.findPlug("ignore"); // cheking ignores
      numElements = plug.numElements();
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
            m_ignores[object.name().asChar()].insert(lightMapIter->second);
      }
   }
   std::cerr << "Finished parsing Light Links in " << (tbb::tick_count::now() - tc).seconds() << " seconds\n";
}

void CArnoldLightLinks::ExportLightLinking(AtNode* shape)
{
   const char* shapeName = AiNodeGetName(shape);
   
   std::map<std::string, std::set<AtNode*> >::iterator it = m_links.find(shapeName);
   
   if (it != m_links.end()) // link mode
   {
      std::cerr << "Exporting light linking!\n";
      AtArray* lights = AiArrayAllocate(it->second.size(), 1, AI_TYPE_NODE);
      unsigned int id = 0;
      for (std::set<AtNode*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2, ++id)
         AiArraySetPtr(lights, id, *it2);
      AiNodeSetBool(shape, "use_light_group", true);
      AiNodeSetArray(shape, "light_group", lights);
      AiNodeSetBool(shape, "use_shadow_group", true);
      AiNodeSetArray(shape, "shadow_group", AiArrayCopy(lights));
      return;
   }
   
   it = m_ignores.find(shapeName);
   
   if (it != m_ignores.end()) // ignores
   {
      std::cerr << "Exporting light ingore!\n";
      unsigned int id = 0;
      for (unsigned int i = 0; i < m_numLights; ++i)
      {
         AtNode* light = p_lights[i];
         if (it->second.find(light) == it->second.end())
            m_tempLights[id++] = light;
      }
      
      if (id)
      {
         AtArray* lights = AiArrayAllocate(id, 1, AI_TYPE_NODE);
         for (unsigned int i = 0; i < id; ++i)
            AiArraySetPtr(lights, i, m_tempLights[i]);
         AiNodeSetBool(shape, "use_light_group", true);
         AiNodeSetArray(shape, "light_group", lights);
         AiNodeSetBool(shape, "use_shadow_group", true);
         AiNodeSetArray(shape, "shadow_group", AiArrayCopy(lights));
      }
      return;
   }
   
   // no light links needs to be exported
}
