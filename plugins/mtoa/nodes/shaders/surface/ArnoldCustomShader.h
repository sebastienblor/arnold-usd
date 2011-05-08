#ifndef ARNOLDCUSTOMSHADER_H
#define ARNOLDCUSTOMSHADER_H

#include "utils/AttrHelper.h"
#include <maya/MPxHwShaderNode.h>
#include <maya/MObjectArray.h>

#include <extension/AbMayaNode.h>

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

   static CAbMayaNode s_abstract;

   // Input attributes
   static MObject SAttr[];

   static std::vector<CStaticAttrHelper> s_nodeHelpers;

};  // class CArnoldCustomShaderNode

#endif // ARNOLDCUSTOMSHADER_H
