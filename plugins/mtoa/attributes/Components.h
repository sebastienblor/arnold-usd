#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <ai_nodes.h>

#include <maya/MStatus.h>
#include <maya/MPlug.h>

enum ComponentType
{
   COMPONENT_TYPE_INVALID,
   COMPONENT_TYPE_NONE,
   COMPONENT_TYPE_NORMAL,
   COMPONENT_TYPE_OUTALPHA
};

ComponentType IsFloatComponent(const MPlug &plug, MPlug &attrResult);
AtNode* InsertConversionNodes(const MPlug &shaderOutputPlug, ComponentType compMode, AtNode* shader);


#endif
