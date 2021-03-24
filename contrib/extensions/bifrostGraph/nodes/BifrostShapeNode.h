#pragma once

#include "nodes/shape/ArnoldBaseProcedural.h"
#include "attributes/AttrHelper.h"
#include <maya/MPxHwShaderNode.h>
#include <maya/MObjectArray.h>

#include <extension/AbMayaNode.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>

#include <vector>

class CBifrostShapeNode
   :  public CArnoldBaseProcedural
{

public:
   CBifrostShapeNode();
   virtual ~CBifrostShapeNode();

   virtual void postConstructor();

   CAbMayaNode m_abstract;
   static MTypeId id;

   static void* creator();
   static MStatus initialize();
   virtual void updateGeometry();
   
   static CAbMayaNode s_abstract;
   static MString s_classification;   

   static MObject s_input;
   static MObject s_dirtyFlag;

   static MCallbackId s_NewNodeCallbackId;
   static MCallbackId s_idleCallbackId;
   static void NewNodeCallback(MObject & node, void *);
   static void IdleCallback(void *);
   static void UpdateBifrostGraphConnections();

   static void  GraphDirtyCallback(MObject& node, MPlug& plug, void* clientData);
   MCallbackId m_graphChangedId;

};  // class CArnoldShaderNode


