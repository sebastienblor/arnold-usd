#include "UtilityFunctions.h"

#define AI_TYPE_BYTE          0x00  /**< Byte (an 8-bit sized unsigned integer) */
#define AI_TYPE_INT           0x01  /**< Integer */
#define AI_TYPE_UINT          0x02  /**< Unsigned integer */
#define AI_TYPE_BOOLEAN       0x03  /**< Boolean (either true or false) */
#define AI_TYPE_FLOAT         0x04  /**< Single-precision floating point number */
#define AI_TYPE_RGB           0x05  /**< RGB struct */
#define AI_TYPE_RGBA          0x06  /**< RGBA struct */
#define AI_TYPE_VECTOR        0x07  /**< XYZ vector */
#define AI_TYPE_POINT         0x08  /**< XYZ point */
#define AI_TYPE_POINT2        0x09  /**< XY point */
#define AI_TYPE_STRING        0x0A  /**< C-style character string */
#define AI_TYPE_POINTER       0x0B  /**< Arbitrary pointer */
#define AI_TYPE_NODE          0x0C  /**< Pointer to an Arnold node */
#define AI_TYPE_ARRAY         0x0D  /**< AtArray */
#define AI_TYPE_MATRIX        0x0E  /**< 4x4 matrix */
#define AI_TYPE_ENUM          0x0F  /**< Enumeration (see \ref AtEnum) */
#define AI_TYPE_UNDEFINED     0xFF  /**< Undefined, you should never encounter a parameter of this type */
#define AI_TYPE_NONE          0xFF  /**< No type */

static const char* declStrings[][4] = {
   {"constant BYTE", "constant ARRAY BYTE", "uniform BYTE", "varying BYTE"}, // AI_TYPE_BYTE
   {"constant INT", "constant ARRAY INT", "uniform INT", "varying INT"}, // AI_TYPE_INT
   {"constant UINT", "constant ARRAY UINT", "uniform UINT", "varying UINT"}, // AI_TYPE_UINT
   {"constant BOOL", "constant ARRAY BOOL", "uniform BOOL", "varying BOOL"}, // AI_TYPE_BOOLEAN
   {"constant FLOAT", "constant ARRAY FLOAT", "uniform FLOAT", "varying FLOAT"}, // AI_TYPE_FLOAT
   {"constant RGB", "constant ARRAY RGB", "uniform RGB", "varying RGB"}, // AI_TYPE_RGB
   {"constant RGBA", "constant ARRAY RGBA", "uniform RGBA", "varying RGBA"}, // AI_TYPE_RGBA
   {"constant VECTOR", "constant ARRAY VECTOR", "uniform VECTOR", "varying VECTOR"}, // AI_TYPE_VECTOR
   {"constant POINT", "constant ARRAY POINT", "uniform POINT", "varying POINT"}, // AI_TYPE_POINT
   {"constant POINT2", "constant ARRAY POINT2", "uniform POINT2", "varying POINT2"}, // AI_TYPE_POINT2
   {"constant STRING", "constant ARRAY STRING", "uniform STRING", "varying STRING"}, // AI_TYPE_STRING
   {"constant POINTER", "constant ARRAY POINTER", "uniform POINTER", "varying POINTER"}, // AI_TYPE_POINTER
   {"constant NODE", "constant ARRAY NODE", "uniform NODE", "varying NODE"}, // AI_TYPE_NODE
   {"constant ARRAY", "constant ARRAY", "uniform ARRAY", "varying ARRAY"}, // AI_TYPE_ARRAY ??
   {"constant MATRIX", "constant ARRAY MATRIX", "uniform MATRIX", "varying MATRIX"}, // AI_TYPE_MATRIX
   {"constant ENUM", "constant ARRAY ENUM", "uniform ENUM", "varying ENUM"} // AI_TYPE_ENUM   
};

void AiNodeDeclareConstant(AtNode* node, const char* name, unsigned int type)
{
   if (type > AI_TYPE_ENUM)
      return;
   AiNodeDeclare(node, name, declStrings[0][type]);
}

void AiNodeDeclareConstantArray(AtNode* node, const char* name, unsigned int type)
{
   if (type > AI_TYPE_ENUM)
      return;
   AiNodeDeclare(node, name, declStrings[1][type]);
}

void AiNodeDeclareUniform(AtNode* node, const char* name, unsigned int type)
{
   if (type > AI_TYPE_ENUM)
      return;
   AiNodeDeclare(node, name, declStrings[2][type]);
}

void AiNodeDeclareVarying(AtNode* node, const char* name, unsigned int type)
{
   if (type > AI_TYPE_ENUM)
      return;
   AiNodeDeclare(node, name, declStrings[3][type]);
}
