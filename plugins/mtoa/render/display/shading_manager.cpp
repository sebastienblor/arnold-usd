/*
 * Windows window and event handling
 */

#include <string.h>
#include "shading_manager.h"

/*
static AtNode *getShaderUtilityBlack()
{
   AtNode *utility_shader = AiNodeLookUpByName("_rvdbg_black");
   if (utility_shader == NULL)
   {

      // black mask
      utility_shader = AiNode("utility");
      AiNodeSetStr(utility_shader, "name", "_rvdbg_black");
      AiNodeSetStr(utility_shader, "color_mode", "color");
      AiNodeSetStr(utility_shader, "shade_mode", "flat");
      AiNodeSetRGB(utility_shader, "color", 0.f, 0.f, 0.f);
   }
   return utility_shader;
}
*/


// as of now we're having to check the shaders connections at every re-render
// which isn't ideal at all.
// we should find a way to do it only once until the shaders connections change.
// Problem : how to find out that something has changed in the shaders connections ?
bool CRvShadingManager::checkShaderConnections(const AtNode *start, const AtNode *end)
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

      if (checkShaderConnections(link, end)) return true;
   }
   AiParamIteratorDestroy(iter);
   return false;
}

void CRvShadingManager::restoreLights()
{
   AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_LIGHT);
   // re-enable all lights
   while (!AiNodeIteratorFinished(iter))
   {
      AtNode *light = AiNodeIteratorGetNext(iter);
      AiNodeSetDisabled(light, false);
   }
}

void CRvShadingManager::restoreShaders()
{   
   if (m_shader_map.empty() && m_disabled_shaders.empty()) return;

   // re-enable all shaders that have been previously disabled
   // (those not attached to isolated shading tree)
   for (size_t i = 0; i < m_disabled_shaders.size(); ++i)
   {
      AtNode *shader = AiNodeLookUpByName(m_disabled_shaders[i].c_str());
      if (shader == NULL) continue;
      AiNodeSetDisabled(shader, false);
   }
   m_disabled_shaders.clear();

   // restore all shape -> shader connections to the original ones
   AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_SHAPE);
   
   std::string map_key;

   while (!AiNodeIteratorFinished(iter))
   {
      AtNode *shape = AiNodeIteratorGetNext(iter);
      const char *shape_name = AiNodeGetName(shape);

      AtArray *shape_shaders = AiNodeGetArray(shape, "shader");
      if (shape_shaders == NULL) continue;

      for (unsigned int g = 0; g < shape_shaders->nelements; ++g)
      {
         AtNode *shader = (AtNode *)AiArrayGetPtr(shape_shaders, g);
         if(shader == NULL) continue;

         map_key = shape_name;

         char intBuf[512];
         sprintf(intBuf,"%i", g);
         map_key += "#";
         map_key += intBuf;

         AtNode *original_shader = AiNodeLookUpByName(m_shader_map[map_key].c_str());
         if (original_shader == NULL) continue;

         // now let's restore our original shader
         AiArraySetPtr(shape_shaders, g, original_shader);
      }
   }
   AiNodeIteratorDestroy(iter);
   m_shader_map.clear();

}

void CRvShadingManager::restore()
{
   restoreShaders();
   restoreLights();
}


void CRvShadingManager::isolateSelected()
{
   AtNode *debug_shader =  AiNodeLookUpByName (m_debug_shader_name.c_str());
   AtNode *ai_default_shader = AiNodeLookUpByName("ai_default_reflection_shader");
   bool all_black = false;
   if(debug_shader == NULL)
   {
      // the default shader is set to black matte
      debug_shader = ai_default_shader;
      all_black = true;
   }

   const AtNodeEntry *entry = AiNodeGetNodeEntry(debug_shader);
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
         AiNodeSetDisabled(light, (light != debug_shader));
      }
      AiNodeIteratorDestroy(iter);
      return;
   } 

   // we shouldn't need to restore lights as restore() is
   // called after a selection change
   //restoreLights();

   AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_SHAPE);
   
   std::string map_key, shader_name;

   // loop over all shapes
   while (!AiNodeIteratorFinished(iter))
   {
      AtNode *shape = AiNodeIteratorGetNext(iter);
      const char *shape_name = AiNodeGetName(shape);

      // get the list of shaders attached to it
      AtArray *shape_shaders = AiNodeGetArray(shape, "shader");

      // no shaders ? get out
      if (shape_shaders == NULL) continue;

      // loop over all shaders for this shape      
      for (unsigned int g = 0; g < shape_shaders->nelements; ++g)
      {
         AtNode *shader = (AtNode *)AiArrayGetPtr(shape_shaders, g);
         if(shader == NULL) continue;
         shader_name = AiNodeGetStr(shader, "name");
         
         map_key = shape_name;
         char intBuf[512];
         sprintf(intBuf,"%i", g);
         map_key += "#";
         map_key += intBuf;
         

         // let's store the original shader's name in a map (where the key is {shape}#{group})
         // in order to restore it later
         // we could store the AtNode* but it might cause dangling pointers
         // in future situations we're not thinking of right now

         // check if this is not already a debug shader !!!
         if (shader == ai_default_shader || shader_name == m_debug_shader_name || (shader_name.length() > 7 && shader_name[0] == '_' && shader_name.substr(0, 7) == "_rvdbg_"))
         {
            // this looks like a debug shader, don't store the map
         } else 
         {
            // this is my original shader
            // let's store its name in a map (where the key is {shape}#{group})
            // in order to restore it later
            // we could store the AtNode* but it might cause dangling pointers
            // in future situations we're not thinking of right now

            m_shader_map[map_key] = AiNodeGetName(shader);
            // Stored !!!
         }
         
         if (type == AI_NODE_SHADER)
         {

            if (debug_shader == shader)
            {
               // this is the isolated shader, nothing to do
               
            } else if(all_black == false && checkShaderConnections(AiNodeLookUpByName(m_shader_map[map_key].c_str()), debug_shader))
            {
               // the isolated shader is connected to this shading tree
               // attach debug_shader

               // now assign debug_shader
               AiArraySetPtr(shape_shaders, g, debug_shader);
            } else 
            {
               // shader is not attached in my shading tree
               // let's disable it, and add its name to the disabled shaders list
               if (!AiNodeIsDisabled(shader))
               {
                  AiNodeSetDisabled(shader, true);
                  m_disabled_shaders.push_back(shader_name);
               }
            }
         } else if (type == AI_NODE_SHAPE)
         {
            if (debug_shader == shape)
            {
               // this shape is selected -> nothing to do 
            } else
            {               
               // this shape is not selected
               // we can't disable the shader as a single shader can be assigned to multiple shapes
               // we don't want to disable the shape as we want it (to be confirmed ?) as a black matte
               
               AiArraySetPtr(shape_shaders, g, ai_default_shader);
            }
         }
      }
   }
   AiNodeIteratorDestroy(iter);
}

void CRvShadingManager::setShaderName(const std::string &name)
{
   restore();
   m_debug_shader_name = name;
}

void CRvShadingManager::objectNameChanged(const std::string &new_name, const std::string &old_name)
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
      if(!m_shader_map.empty())
      {

         std::tr1::unordered_map<std::string, std::string>::iterator iter = m_shader_map.begin();
         for ( ; iter != m_shader_map.end(); iter++)
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
      if(!m_shader_map.empty())
      {
         std::tr1::unordered_map<std::string, std::string>::iterator iter = m_shader_map.begin();
         for ( ; iter != m_shader_map.end(); iter++)
         {
            //const std::string &shader_key = (*iter).first;
            std::string &shader_name = (*iter).second;

            if (shader_name == old_name)
            {
               (*iter).second = new_name;
            }
         }
      }

   } else if (type == AI_NODE_LIGHT)
   {
      if (!m_light_intensities.empty())
      {
         std::tr1::unordered_map<std::string, float>::iterator iter = m_light_intensities.begin();
         for ( ; iter != m_light_intensities.end(); iter++)
         {
            const std::string &light_key = (*iter).first;

            if (light_key == old_name)
            {
               float intensity = (*iter).second;
               m_light_intensities.erase(iter);
               m_light_intensities[new_name] = intensity;
               break;
            }
         }
      }
   }
}
