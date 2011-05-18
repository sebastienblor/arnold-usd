#ifndef ARNOLDSHADERNODE_H
#define ARNOLDSHADERNODE_H

#include "attributes/AttrHelper.h"
#include <maya/MPxHwShaderNode.h>
#include <maya/MObjectArray.h>

#include <extension/AbMayaNode.h>

#include <vector>

class CArnoldShaderNode
   :  public MPxNode
{

private:

   static MObjectArray s_PlugsAffecting;

public:

   virtual void postConstructor();

   virtual MStatus compute(const MPlug& plug, MDataBlock& data);

   CAbMayaNode m_abstract;

   static void* creator();
   static MStatus initialize();

   static CAbMayaNode s_abstract;

   // Input attributes
   static MObject SAttr[];

   static std::vector<CStaticAttrHelper> s_nodeHelpers;

};  // class CArnoldShaderNode

#endif // ARNOLDSHADERNODE_H
