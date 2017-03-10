#include "UtilityFunctions.h"
#include <string>
#include <algorithm>
static const std::string s_constantStr("constant ");
static const std::string s_constantArrayStr("constant ARRAY ");
static const std::string s_uniformStr("uniform ");
static const std::string s_varyingStr("varying ");   

bool AiNodeDeclareConstant(AtNode* node, const char* name, unsigned int type)
{
   if (type > AI_TYPE_ENUM)
      return false;

   std::string declStr = s_constantStr + AiParamGetTypeName(type);
   return AiNodeDeclare(node, name, declStr.c_str());
}

bool AiNodeDeclareConstantArray(AtNode* node, const char* name, unsigned int type)
{
   if (type > AI_TYPE_ENUM)
      return false;

   std::string declStr = s_constantArrayStr + AiParamGetTypeName(type);
   return AiNodeDeclare(node, name, declStr.c_str());
}

bool AiNodeDeclareUniform(AtNode* node, const char* name, unsigned int type)
{
   if (type > AI_TYPE_ENUM)
      return false;

   std::string declStr = s_uniformStr + AiParamGetTypeName(type);
   return AiNodeDeclare(node, name, declStr.c_str());
}

bool AiNodeDeclareVarying(AtNode* node, const char* name, unsigned int type)
{
   if (type > AI_TYPE_ENUM)
      return false;

   std::string declStr = s_varyingStr + AiParamGetTypeName(type);
   return AiNodeDeclare(node, name, declStr.c_str());
}
