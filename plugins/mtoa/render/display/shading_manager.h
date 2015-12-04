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
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

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

   void Restore();
   void IsolateSelected();
   
   void SetShaderName(const std::string &name);
   const std::string &GetShaderName() const {return m_debugShaderName;}
   void ObjectNameChanged(const std::string &newName, const std::string &oldName);

private:
   static bool CheckShaderConnections(const AtNode *, const AtNode *);
   void RestoreShaders();
   void RestoreLights();

   std::tr1::unordered_map<std::string, std::string> m_shaderMap;
   std::tr1::unordered_map<std::string, float> m_lightIntensities;

   std::vector<std::string> m_disabledShaders;
   std::string  m_debugShaderName;
   
};

