/*
 * Windows window and event handling
 */

#include <string.h>
#include "shading_manager.h"



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
static AtNode *getShaderUtilityWhite()
{
   AtNode *utility_shader = AiNodeLookUpByName("_rvdbg_white");
   if (utility_shader == NULL)
   {

      // black mask
      utility_shader = AiNode("utility");
      AiNodeSetStr(utility_shader, "name", "_rvdbg_white");
      AiNodeSetStr(utility_shader, "color_mode", "color");
      AiNodeSetStr(utility_shader, "shade_mode", "flat");
      AiNodeSetRGB(utility_shader, "color", 1.f, 1.f, 1.f);
   }
   return utility_shader;

}


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
   // restore all lights to their original values
   if (m_light_intensities.empty()) return;

   std::tr1::unordered_map<std::string, float>::iterator iter = m_light_intensities.begin();
   for ( ; iter != m_light_intensities.end(); ++iter)
   {
      AiNodeSetFlt(AiNodeLookUpByName((*iter).first.c_str()), "intensity", (*iter).second);
   }
   m_light_intensities.clear();
}
void CRvShadingManager::restoreShaders()
{
   if (m_shader_map.empty()) return;

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
         map_key += "#" + std::to_string(g);

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
   bool all_black = false;
   if(debug_shader == NULL)
   {
      debug_shader = getShaderUtilityBlack();
      all_black = true;
   }

   const AtNodeEntry *entry = AiNodeGetNodeEntry(debug_shader);
   int type = AiNodeEntryGetType(entry);


   if (type == AI_NODE_LIGHT)
   {
      // first restore existing shaders if necessary
      restoreShaders();

      // loop over lights in Arnold scene
      AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_LIGHT);
   
      std::string map_key, shader_name;
      std::string light_name;
      while (!AiNodeIteratorFinished(iter))
      {
         AtNode *light = AiNodeIteratorGetNext(iter);
         float intensity = AiNodeGetFlt(light, "intensity");
         light_name = AiNodeGetName(light);

         bool stored_intensity = false;

         if (intensity == 0.f)
         {
            std::tr1::unordered_map<std::string, float>::iterator iter = m_light_intensities.find(light_name);
            if (iter != m_light_intensities.end())
            {
               // this light has been previously disabled
               // get the original intensity from the map
               intensity = (*iter).second;
               stored_intensity = true;
            }
         }

         // set the correct intensity in this map
         if (!stored_intensity) m_light_intensities[light_name] = intensity;

         if (light == debug_shader)
         {
            // i changed my selection and need to restore this light intensity
            // to the original one
            if (stored_intensity) AiNodeSetFlt(light, "intensity", intensity);
            
         } else
         {
            // this light is not selected, set now its intensity to 0
            AiNodeSetFlt(light, "intensity", 0.f);
         }

      }
      AiNodeIteratorDestroy(iter);
      return;
   } 

   if (!m_light_intensities.empty())
   {
      restoreLights();
   }


   AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_SHAPE);
   
   std::string map_key, shader_name;

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
         shader_name = AiNodeGetStr(shader, "name");
         
         map_key = shape_name;
         map_key += "#" + std::to_string(g);

         // check if this is not already a debug shader !!!
         if (shader_name == m_debug_shader_name || (shader_name.length() > 7 && shader_name[0] == '_' && shader_name.substr(0, 7) == "_rvdbg_"))
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
            // now let's assign our debug shader
            map_key = shape_name;
            map_key += "#" + std::to_string(g);
            
            if(all_black == false && checkShaderConnections(AiNodeLookUpByName(m_shader_map[map_key].c_str()), debug_shader))
            {
               AiArraySetPtr(shape_shaders, g, debug_shader);
            } else 
            {
               AiArraySetPtr(shape_shaders, g, getShaderUtilityBlack());
            }
         } else if (type == AI_NODE_SHAPE)
         {
            if (debug_shader == shape)
            {
               // do we want a white mask ? or the original shader ?
               //AiArraySetPtr(shape_shaders, g, getShaderUtilityWhite());
               AiArraySetPtr(shape_shaders, g, AiNodeLookUpByName(m_shader_map[map_key].c_str()));
            } else
            {
               
               AiArraySetPtr(shape_shaders, g, getShaderUtilityBlack());
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
void CRvShadingManager::applyDebugShading(RenderViewDebugShading d)
{
   if (d == RV_DBG_SHAD_DISABLED) 
   {
      restore();
   } else if (d == RV_DBG_SHAD_ISOLATE_SELECTED)
   {
      isolateSelected();
   } else
   {
      AtNode *utility_shader = AiNodeLookUpByName("_rvdbg_utility");
      if (utility_shader == NULL)
      {
         utility_shader = AiNode("utility");
         AiNodeSetStr(utility_shader, "name", "_rvdbg_utility");
      }
      
      if (d == RV_DBG_SHAD_WIREFRAME)
      {    
         AiNodeSetStr(utility_shader, "color_mode", "polywire");
         AiNodeSetStr(utility_shader, "shade_mode", "ndoteye");
      } else if (d == RV_DBG_SHAD_BASIC)
      {
         AiNodeSetStr(utility_shader, "color_mode", "color");
         AiNodeSetStr(utility_shader, "shade_mode", "ndoteye");
      } else if (d == RV_DBG_SHAD_OCCLUSION)
      {
         AiNodeSetStr(utility_shader, "color_mode", "color");
         AiNodeSetStr(utility_shader, "shade_mode", "ambocc");
      } else if (d == RV_DBG_SHAD_UV)
      {
         AiNodeSetStr(utility_shader, "color_mode", "uv");
         AiNodeSetStr(utility_shader, "shade_mode", "flat");
      } else if (d == RV_DBG_SHAD_NORMAL)
      {
         AiNodeSetStr(utility_shader, "color_mode", "n");
         AiNodeSetStr(utility_shader, "shade_mode", "flat");
      } else if (d == RV_DBG_SHAD_PRIMITIVE_ID)
      {
         AiNodeSetStr(utility_shader, "color_mode", "prims");
         AiNodeSetStr(utility_shader, "shade_mode", "flat");
      } else if (d == RV_DBG_SHAD_OBJECT)
      {
         AiNodeSetStr(utility_shader, "color_mode", "obj");
         AiNodeSetStr(utility_shader, "shade_mode", "flat");
      } else if (d == RV_DBG_SHAD_BARY)
      {
         AiNodeSetStr(utility_shader, "color_mode", "bary");
         AiNodeSetStr(utility_shader, "shade_mode", "flat");
      } 
      setDebugShader(utility_shader);
   }

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
               unsigned int group_index = std::stoi(group_index_str);
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
            const std::string &shader_key = (*iter).first;
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

void CRvShadingManager::setDebugShader(AtNode *debug_shader)
{
   if (debug_shader == NULL) return;
   std::string debug_shader_name = AiNodeGetName(debug_shader);
   
   AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_SHAPE);
   //m_shader_map.clear();


   std::string map_key, shader_name;

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
         shader_name = AiNodeGetStr(shader, "name");
         // check if this is not already a debug shader !!!
         if (shader_name == m_debug_shader_name || (shader_name.length() > 7 && shader_name[0] == '_' && shader_name.substr(0, 7) == "_rvdbg_"))
         {
            // this looks like a debug shader, don't store the map
         } else 
         {
            // this is my original shader
            // let's store its name in a map (where the key is {shape}#{group})
            // in order to restore it later
            // we could store the AtNode* but it might cause dangling pointers
            // in future situations we're not thinking of right now

            map_key = shape_name;
            map_key += "#" + std::to_string(g);

            m_shader_map[map_key] = AiNodeGetName(shader);
            // Stored !!!
         }
         // now let's assign our debug shader
         AiArraySetPtr(shape_shaders, g, debug_shader);
      }
   }
   AiNodeIteratorDestroy(iter);
   m_debug_shader_name = debug_shader_name;
}
