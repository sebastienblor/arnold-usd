#pragma once

#include "nodes/ArnoldNodeIDs.h"
#include <ai.h>

#include <maya/MPxLocatorNode.h>
#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MBoundingBox.h>
#include <maya/MSelectionMask.h>

#if defined(_DARWIN)
   #include <OpenGL/gl.h>
#else 
   #include <GL/gl.h>
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
#ifdef ENABLE_VP2
#if MAYA_API_VERSION >= 201700
   virtual MSelectionMask getShapeSelectionMask() const;
#endif
#endif

   static void* creator();
   bool isAbstractClass() const;
   static MStatus initialize();
   void SampleSN(MPlug &colorPlug);
   void DrawUVSphere(float radius, int divisionsX, int divisionsY, int format, bool needsUV = true);
   void DrawSphereWireframe(float radius, int divisionsX, int divisionsY);
   void DrawSphereFilled(float radius, int divisionsX, int divisionsY);
   unsigned int NumSampleBase();

#ifdef ENABLE_VP2
#if MAYA_API_VERSION >= 201700
   virtual MStatus connectionMade( const MPlug& plug,
											 const MPlug& otherPlug,
											 bool asSrc );
	virtual MStatus connectionBroken( const MPlug& plug,
											 const MPlug& otherPlug,
											 bool asSrc );
	static void nodeDirtyEventCallback(MObject& node, MPlug& plug, void* clientData);
   MCallbackId m_dirtyCallbackId;
#endif
#endif

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
   std::vector<AtVector>      m_positionData;
   std::vector<AtVector2>     m_UVData;
   std::vector<unsigned int>        m_indexData;
   GLuint m_texture;
   GLuint m_vbo;

   // Need to check if sampling again is needed
   bool   m_goSample;
   float  m_cachedRadius;
   int    m_cachedDivisionX;
   int    m_cachedDivisionY;
   int    m_cachedFormat;

   
   virtual void postConstructor()
   {
      // Initialize colorData
      m_goSample        = true;
      m_cachedRadius    = -1.f;
      m_cachedDivisionX = -1;
      m_cachedDivisionY = -1;
      m_cachedFormat    = -1;
      m_texture         = -1;
      m_vbo             = -1;
      
      MObject obj = thisMObject();
      MNodeMessage::addNodePreRemovalCallback(obj, removeSphereLocator, this);
   }
   
   static void removeSphereLocator(MObject& node, void* clientData)
   {
      CSphereLocator* locator = reinterpret_cast<CSphereLocator*>(clientData);
      locator->m_colorData.clear();
      locator->m_positionData.clear();
      locator->m_UVData.clear();
      locator->m_indexData.clear();
      locator->m_goSample        = true;
      locator->m_cachedRadius    = -1.f;
      locator->m_cachedDivisionX = -1;
      locator->m_cachedDivisionY = -1;
      locator->m_cachedFormat    = -1;
   }

};  // class CSphereLocator
