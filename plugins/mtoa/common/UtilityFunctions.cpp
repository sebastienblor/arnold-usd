#include "UtilityFunctions.h"

// FIXME Arnold5 that's ugly, we should do a switch to make sure we're declaring the correct data type 
// in case arnold types change

static const char* declStrings[][4] = {
   {"constant BYTE", "constant ARRAY BYTE", "uniform BYTE", "varying BYTE"}, // AI_TYPE_BYTE
   {"constant INT", "constant ARRAY INT", "uniform INT", "varying INT"}, // AI_TYPE_INT
   {"constant UINT", "constant ARRAY UINT", "uniform UINT", "varying UINT"}, // AI_TYPE_UINT
   {"constant BOOL", "constant ARRAY BOOL", "uniform BOOL", "varying BOOL"}, // AI_TYPE_BOOLEAN
   {"constant FLOAT", "constant ARRAY FLOAT", "uniform FLOAT", "varying FLOAT"}, // AI_TYPE_FLOAT
   {"constant RGB", "constant ARRAY RGB", "uniform RGB", "varying RGB"}, // AI_TYPE_RGB
   {"constant RGBA", "constant ARRAY RGBA", "uniform RGBA", "varying RGBA"}, // AI_TYPE_RGBA
   {"constant VECTOR", "constant ARRAY VECTOR", "uniform VECTOR", "varying VECTOR"}, // AI_TYPE_VECTOR
   {"constant VECTOR2", "constant ARRAY VECTOR2", "uniform VECTOR2", "varying VECTOR2"}, // AI_TYPE_VECTOR2
   {"constant STRING", "constant ARRAY STRING", "uniform STRING", "varying STRING"}, // AI_TYPE_STRING
   {"constant POINTER", "constant ARRAY POINTER", "uniform POINTER", "varying POINTER"}, // AI_TYPE_POINTER
   {"constant NODE", "constant ARRAY NODE", "uniform NODE", "varying NODE"}, // AI_TYPE_NODE
   {"constant ARRAY", "constant ARRAY", "uniform ARRAY", "varying ARRAY"}, // AI_TYPE_ARRAY ??
   {"constant MATRIX", "constant ARRAY MATRIX", "uniform MATRIX", "varying MATRIX"}, // AI_TYPE_MATRIX
   {"constant ENUM", "constant ARRAY ENUM", "uniform ENUM", "varying ENUM"} // AI_TYPE_ENUM   
};

bool AiNodeDeclareConstant(AtNode* node, const char* name, unsigned int type)
{
   if (type > AI_TYPE_ENUM)
      return false;
   return AiNodeDeclare(node, name, declStrings[type][0]);
}

bool AiNodeDeclareConstantArray(AtNode* node, const char* name, unsigned int type)
{
   if (type > AI_TYPE_ENUM)
      return false;
   return AiNodeDeclare(node, name, declStrings[type][1]);
}

bool AiNodeDeclareUniform(AtNode* node, const char* name, unsigned int type)
{
   if (type > AI_TYPE_ENUM)
      return false;
   return AiNodeDeclare(node, name, declStrings[type][2]);
}

bool AiNodeDeclareVarying(AtNode* node, const char* name, unsigned int type)
{
   if (type > AI_TYPE_ENUM)
      return false;
   return AiNodeDeclare(node, name, declStrings[type][3]);
}
