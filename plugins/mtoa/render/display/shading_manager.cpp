/*
 * Windows window and event handling
 */

#include <string.h>
#include "shading_manager.h"


// as of now we're having to check the shaders connections at every re-render
// which isn't ideal at all.
// we should find a way to do it only once until the shaders connections change.
// Problem : how to find out that something has changed in the shaders connections ?
bool CRvShadingManager::CheckShaderConnections(const AtNode *start, const AtNode *end)
{
   if (start == end) return true;
   const AtNodeEntry *nentry = AiNodeGetNodeEntry(start);
   AtParamIterator *iter = AiNodeEntryGetParamIterator(nentry);

   while (!AiParamIteratorFinished(iter))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(iter);
      const char* paramName = AiParamGetName(paramEntry);
      AtNode *link = AiNodeGetLink(start, paramName);
      if (link == NULL) continue;

      if (CheckShaderConnections(link, end)) return true;
   }
   AiParamIteratorDestroy(iter);
   return false;
}

void CRvShadingManager::RestoreLights()
{
   AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_LIGHT);
   // re-enable all lights
   while (!AiNodeIteratorFinished(iter))
   {
      AtNode *light = AiNodeIteratorGetNext(iter);
      AiNodeSetDisabled(light, false);
   }
}

void CRvShadingManager::RestoreShaders()
{   
   if (m_shaderMap.empty() && m_disabledShaders.empty()) return;

   // re-enable all shaders that have been previously disabled
   // (those not attached to isolated shading tree)
   for (size_t i = 0; i < m_disabledShaders.size(); ++i)
   {
      AtNode *shader = AiNodeLookUpByName(m_disabledShaders[i].c_str());
      if (shader == NULL) continue;
      AiNodeSetDisabled(shader, false);
   }
   m_disabledShaders.clear();

   // restore all shape -> shader connections to the original ones
   AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_SHAPE);
   
   std::string mapKey;

   while (!AiNodeIteratorFinished(iter))
   {
      AtNode *shape = AiNodeIteratorGetNext(iter);
      const char *shapeName = AiNodeGetName(shape);

      AtArray *shapeShaders = AiNodeGetArray(shape, "shader");
      if (shapeShaders == NULL) continue;

      for (unsigned int g = 0; g < shapeShaders->nelements; ++g)
      {
         AtNode *shader = (AtNode *)AiArrayGetPtr(shapeShaders, g);
         if(shader == NULL) continue;

         mapKey = shapeName;

         char intBuf[512];
         sprintf(intBuf,"%i", g);
         mapKey += "#";
         mapKey += intBuf;

         AtNode *originalShader = AiNodeLookUpByName(m_shaderMap[mapKey].c_str());
         if (originalShader == NULL) continue;

         // now let's restore our original shader
         AiArraySetPtr(shapeShaders, g, originalShader);
      }
   }
   AiNodeIteratorDestroy(iter);
   m_shaderMap.clear();

}

void CRvShadingManager::Restore()
{
   RestoreShaders();
   RestoreLights();
}


void CRvShadingManager::IsolateSelected()
{
   AtNode *debugShader =  AiNodeLookUpByName (m_debugShaderName.c_str());
   AtNode *aiDefaultShader = AiNodeLookUpByName("ai_default_reflection_shader");
   bool allBlack = false;
   if(debugShader == NULL)
   {
      // the default shader is set to black matte
      debugShader = aiDefaultShader;
      allBlack = true;
   }

   const AtNodeEntry *entry = AiNodeGetNodeEntry(debugShader);
   int type = AiNodeEntryGetType(entry);

   if (type == AI_NODE_LIGHT)
   {
      // we shouldn't need to restore shaders as restore() is
      // called after a selection change
      //restoreShaders();

      // loop over lights in Arnold scene
      AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_LIGHT);
      while (!AiNodeIteratorFinished(iter))
      {
         AtNode *light = AiNodeIteratorGetNext(iter);
         // disable all but the selected light
         AiNodeSetDisabled(light, (light != debugShader));
      }
      AiNodeIteratorDestroy(iter);
      return;
   } 

   // we shouldn't need to restore lights as restore() is
   // called after a selection change
   //restoreLights();

   AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_SHAPE);
   
   std::string mapKey, shaderName;

   // loop over all shapes
   while (!AiNodeIteratorFinished(iter))
   {
      AtNode *shape = AiNodeIteratorGetNext(iter);
      const char *shapeName = AiNodeGetName(shape);

      // get the list of shaders attached to it
      AtArray *shapeShaders = AiNodeGetArray(shape, "shader");

      // no shaders ? get out
      if (shapeShaders == NULL) continue;

      // loop over all shaders for this shape      
      for (unsigned int g = 0; g < shapeShaders->nelements; ++g)
      {
         AtNode *shader = (AtNode *)AiArrayGetPtr(shapeShaders, g);
         if(shader == NULL) continue;
         shaderName = AiNodeGetStr(shader, "name");
         
         mapKey = shapeName;
         char intBuf[512];
         sprintf(intBuf,"%i", g);
         mapKey += "#";
         mapKey += intBuf;
         

         // let's store the original shader's name in a map (where the key is {shape}#{group})
         // in order to restore it later
         // we could store the AtNode* but it might cause dangling pointers
         // in future situations we're not thinking of right now

         // check if this is not already a debug shader !!!
         if (shader == aiDefaultShader || shaderName == m_debugShaderName || (shaderName.length() > 7 && shaderName[0] == '_' && shaderName.substr(0, 7) == "_rvdbg_"))
         {
            // this looks like a debug shader, don't store the map
         } else 
         {
            // this is my original shader
            // let's store its name in a map (where the key is {shape}#{group})
            // in order to restore it later
            // we could store the AtNode* but it might cause dangling pointers
            // in future situations we're not thinking of right now

            m_shaderMap[mapKey] = AiNodeGetName(shader);
            // Stored !!!
         }
         
         if (type == AI_NODE_SHADER)
         {

            if (debugShader == shader)
            {
               // this is the isolated shader, nothing to do
               
            } else if(allBlack == false && CheckShaderConnections(AiNodeLookUpByName(m_shaderMap[mapKey].c_str()), debugShader))
            {
               // the isolated shader is connected to this shading tree
               // attach debugShader

               // now assign debugShader
               AiArraySetPtr(shapeShaders, g, debugShader);
            } else 
            {
               // shader is not attached in my shading tree
               // let's disable it, and add its name to the disabled shaders list
               if (!AiNodeIsDisabled(shader))
               {
                  AiNodeSetDisabled(shader, true);
                  m_disabledShaders.push_back(shaderName);
               }
            }
         } else if (type == AI_NODE_SHAPE)
         {
            if (debugShader == shape)
            {
               // this shape is selected -> nothing to do 
            } else
            {               
               // this shape is not selected
               // we can't disable the shader as a single shader can be assigned to multiple shapes
               // we don't want to disable the shape as we want it (to be confirmed ?) as a black matte
               
               AiArraySetPtr(shapeShaders, g, aiDefaultShader);
            }
         }
      }
   }
   AiNodeIteratorDestroy(iter);
}

void CRvShadingManager::SetShaderName(const std::string &name)
{
   Restore();
   m_debugShaderName = name;
}

void CRvShadingManager::ObjectNameChanged(const std::string &new_name, const std::string &old_name)
{
   // depending on the order of events, we might be called before or after Arnold node has been renamed :-/
   AtNode *node = AiNodeLookUpByName(old_name.c_str());
   if (node == NULL)
   {
      // try with new name
      node = AiNodeLookUpByName(new_name.c_str());
   }

   if (node == NULL) return;

   int type = AiNodeEntryGetType(AiNodeGetNodeEntry(node));

   if (type == AI_NODE_SHAPE)
   {
      // check in the maps if there is data with the previous name
      // and eventually replace it
      if(!m_shaderMap.empty())
      {

         std::tr1::unordered_map<std::string, std::string>::iterator iter = m_shaderMap.begin();
         for ( ; iter != m_shaderMap.end(); iter++)
         {
            const std::string &shader_key = (*iter).first;
            if (shader_key.substr(0, shader_key.find('#')) == old_name)
            {
               std::string group_index_str = shader_key.substr(shader_key.find('#') + 1);
               unsigned int group_index = atoi(group_index_str.c_str());
               AtArray *ar = AiNodeGetArray(node, "shader");
               if (group_index >= 0 && group_index < ar->nelements)
               {
                  AiArraySetPtr(ar, group_index, AiNodeLookUpByName((*iter).second.c_str()));
               }
            }
         }
      }
   } else if (type == AI_NODE_SHADER)
   {
      // check in the maps if there is data with the previous name
      // and eventually replace it
      if(!m_shaderMap.empty())
      {
         std::tr1::unordered_map<std::string, std::string>::iterator iter = m_shaderMap.begin();
         for ( ; iter != m_shaderMap.end(); iter++)
         {
            //const std::string &shader_key = (*iter).first;
            std::string &shaderName = (*iter).second;

            if (shaderName == old_name)
            {
               (*iter).second = new_name;
            }
         }
      }

   } else if (type == AI_NODE_LIGHT)
   {
      if (!m_lightIntensities.empty())
      {
         std::tr1::unordered_map<std::string, float>::iterator iter = m_lightIntensities.begin();
         for ( ; iter != m_lightIntensities.end(); iter++)
         {
            const std::string &light_key = (*iter).first;

            if (light_key == old_name)
            {
               float intensity = (*iter).second;
               m_lightIntensities.erase(iter);
               m_lightIntensities[new_name] = intensity;
               break;
            }
         }
      }
   }
}
