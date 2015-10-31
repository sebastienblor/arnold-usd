/*
 * Common header file for macros, data types, prototypes and global vars
 * 
 * --------------------------------------------------------------------------
 * --------------------------------------------------------------------------
 */

#pragma once


#include <ai.h>

#include <math.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#ifdef _WIN32
#include <conio.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include <iostream>

#ifdef _WIN32
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif


enum RenderViewDebugShading
{
   RV_DBG_SHAD_DISABLED,
   RV_DBG_SHAD_BASIC,
   RV_DBG_SHAD_OCCLUSION,
   RV_DBG_SHAD_WIREFRAME,
   RV_DBG_SHAD_NORMAL,
   RV_DBG_SHAD_UV,
   RV_DBG_SHAD_PRIMITIVE_ID,
   RV_DBG_SHAD_OBJECT,
   RV_DBG_SHAD_BARY,
   RV_DBG_SHAD_ISOLATE_SELECTED
};



class CRvShadingManager
{
public:
   CRvShadingManager() {}
   ~CRvShadingManager() {}

   void restore();
   void isolateSelected();
   
   void setShaderName(const std::string &name);
   void objectNameChanged(const std::string &new_name, const std::string &old_name);

private:
   static bool checkShaderConnections(const AtNode *, const AtNode *);
   void restoreShaders();
   void restoreLights();

   std::tr1::unordered_map<std::string, std::string> m_shader_map;
   std::tr1::unordered_map<std::string, float> m_light_intensities;

   std::vector<std::string> m_disabled_shaders;
   std::string  m_debug_shader_name;
   
};

