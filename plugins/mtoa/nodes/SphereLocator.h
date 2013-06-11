#pragma once

#include "nodes/ArnoldNodeIDs.h"
#include <ai_types.h>

#include <maya/MPxLocatorNode.h>
#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MBoundingBox.h>

#if defined(_DARWIN)
   #include <OpenGL/gl.h>
   #include <OpenGL/glu.h>
#else 
   #include <GL/gl.h>
   #include <GL/glu.h>
#endif   

#include <maya/MDGMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MStatus.h>
#include <vector>

class CSphereLocator
   :  public MPxLocatorNode
{

public:

   CSphereLocator();
   virtual ~CSphereLocator();
   
   static MTypeId id;

   virtual void draw(M3dView& view, const MDagPath& DGpath, M3dView::DisplayStyle style, M3dView::DisplayStatus status);
   virtual void OnDraw(M3dView& view, M3dView::DisplayStyle style, M3dView::DisplayStatus status);
   virtual bool isBounded() const;
   virtual bool drawLast() const;
   virtual bool isTransparent() const;
   virtual bool excludeAsLocator() const;
   virtual MBoundingBox boundingBox() const;
   virtual bool setInternalValueInContext(const MPlug &plug, const MDataHandle &handle, MDGContext &context);

   static void* creator();
   bool isAbstractClass();
   static MStatus initialize();
   void SampleSN(MPlug &colorPlug);
   void DrawUVSphere(float radius, int divisionsX, int divisionsY, int format);
   void DrawSphereWireframe(float radius, int divisionsX, int divisionsY);
   void DrawSphereFilled(float radius, int divisionsX, int divisionsY);
   unsigned int NumSampleBase();

   // Input attributes
   static MObject s_colorR;
   static MObject s_colorG;
   static MObject s_colorB;
   static MObject s_color;
   static MObject s_format;

   static MObject s_skyRadius;
   static MObject s_skyFacing;
   static MObject s_sampling;
   static MObject s_hwtexalpha;

   std::vector<unsigned char> m_colorData;
   std::vector<float>         m_UData;
   std::vector<float>         m_VData;

   // Need to check if sampling again is needed
   bool   m_goSample;
   bool   m_goUVSample;
   
   virtual void postConstructor()
   {
      // Initialize colorData
      m_goSample    = true;
      m_goUVSample  = true;
      
      MObject obj = thisMObject();
      MNodeMessage::addNodePreRemovalCallback(obj, removeSphereLocator, this);
   }
   
   static void removeSphereLocator(MObject& node, void* clientData)
   {
      CSphereLocator* locator = reinterpret_cast<CSphereLocator*>(clientData);
      locator->m_colorData.clear();
      locator->m_UData.clear();
      locator->m_VData.clear();
      locator->m_goSample = true;
      locator->m_goUVSample = true;
   }

};  // class CSphereLocator
