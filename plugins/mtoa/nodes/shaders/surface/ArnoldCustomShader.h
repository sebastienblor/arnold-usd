#ifndef ARNOLD_CUSTOM_SHADER_H
#define ARNOLD_CUSTOM_SHADER_H

#include "nodes/ArnoldNodeHelper.h"
#include <maya/MPxHwShaderNode.h>
#include <maya/MObjectArray.h>

#include <vector>

class CArnoldCustomShaderNode
   :  public MPxNode
{

private:

   static MObjectArray s_PlugsAffecting;

public:

   virtual void postConstructor();

   virtual MStatus compute(const MPlug& plug, MDataBlock& data);

   static void* creator();
   static MStatus initialize();

   static MString s_shaderName;

   // Input attributes
   static MObject SAttr[];

   static std::vector<CStaticAttrHelper> s_nodeHelpers;

};  // class CArnoldCustomShaderNode

#endif // ARNOLD_CUSTOM_SHADER_H
