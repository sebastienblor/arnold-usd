#pragma once

#include "attributes/AttrHelper.h"
#include "common/UnorderedContainer.h"
#include "ArnoldStandInGeometry.h"

#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>
#include <maya/MNodeMessage.h>
#include <ai_nodes.h>

#include <vector>
#include <map>

// Geometry class
class CArnoldStandInGeom
{
public:

   CArnoldStandInGeom();
   ~CArnoldStandInGeom();
   MString dso;
   MString data;
   MString filename;
   MString geomLoaded;
   int mode;
   int frame;
   float frameOffset;
   bool useFrameExtension;
   bool useSubFrame;
   bool IsGeomLoaded;
   MBoundingBox bbox;
//   bool deferStandinLoad;
   float scale;
   MPoint BBmin;
   MPoint BBmax;
   typedef unordered_map<std::string, CArnoldStandInGeometry*> geometryListType;
   typedef geometryListType::const_iterator geometryListIterType;
   geometryListType m_geometryList;
   typedef std::vector<CArnoldStandInGInstance*> instanceListType;
   typedef instanceListType::const_iterator instanceListIterType;
   instanceListType m_instanceList;
   int dList;
   int updateView;
   int updateBBox;
   int drawOverride;
   bool hasSelection;



   void Clear();
   void Draw(int DrawMode);
   size_t PointCount(StandinSelectionFilter selected = STANDIN_GEOM_ALL) const;
   size_t SharedVertexCount(StandinSelectionFilter selected = STANDIN_GEOM_ALL) const;
   size_t VisibleGeometryCount(StandinSelectionFilter selected = STANDIN_GEOM_ALL) const;
   size_t WireIndexCount(StandinSelectionFilter selected = STANDIN_GEOM_ALL) const;
   size_t TriangleIndexCount(bool sharedVertices = false, StandinSelectionFilter selected = STANDIN_GEOM_ALL) const;
};

// Shape class - defines the non-UI part of a shape node
class CArnoldStandInShape: public MPxSurfaceShape
{

public:
   CArnoldStandInShape();
   virtual ~CArnoldStandInShape();

   virtual void postConstructor();
   virtual MStatus compute(const MPlug& plug, MDataBlock& data);
   virtual bool getInternalValueInContext(const MPlug&, MDataHandle&,
         MDGContext &context);
   virtual bool setInternalValueInContext(const MPlug&, const MDataHandle&,
         MDGContext &context);

   virtual bool isBounded() const;
   virtual MBoundingBox boundingBox() const;
   
   virtual MSelectionMask getShapeSelectionMask() const;

   MStatus GetPointPlugValue( MPlug plug, float3 & value );
   MStatus SetPointPlugValue( MPlug plug, float3   value );
   void CreateBoundingBox();
   bool LoadBoundingBox();
   MStatus GetPointsFromAss();

   void UpdateSelectedItems();

   static void* creator();
   static MStatus initialize();
   CArnoldStandInGeom* geometry();

   int drawMode();
   //bool deferStandinLoad();

   static MTypeId id;
   static void  AttrChangedCallback(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void* clientData);
private:
   CArnoldStandInGeom fGeometry;
   // Attributes
   static CStaticAttrHelper s_attributes;
   static MObject s_dso;
   static MObject s_mode;
   static MObject s_useFrameExtension;
   static MObject s_frameNumber;
   static MObject s_useSubFrame;
   static MObject s_frameOffset;
   static MObject s_data;
   static MObject s_overrideNodes;
   static MObject s_namespaceName;
   //static MObject s_deferStandinLoad;
   static MObject s_scale;
   static MObject s_boundingBoxMin;
   static MObject s_boundingBoxMax;
   static MObject s_drawOverride;
   static MObject s_selectedItems;
   static MObject s_ignoreGroupNodes;
   static MObject s_abcLayers;

   MCallbackId m_attrChangeId;
   bool m_refreshAvoided;
}; // class CArnoldStandInShape


// UI class - defines the UI part of a shape node
class CArnoldStandInShapeUI: public MPxSurfaceShapeUI
{
public:
   CArnoldStandInShapeUI();
   virtual ~CArnoldStandInShapeUI();
   virtual void getDrawRequests(const MDrawInfo & info,
         bool objectAndActiveOnly, MDrawRequestQueue & requests);
   virtual void draw(const MDrawRequest & request, M3dView & view) const;
   virtual bool select(MSelectInfo &selectInfo, MSelectionList &selectionList,
         MPointArray &worldSpaceSelectPts) const;

   void getDrawRequestsWireFrame(MDrawRequest&, const MDrawInfo&);

   static void * creator();
   // Draw Tokens
   //
   enum
   {
      kDrawBoundingBox, kLastToken
   };

}; // class CArnoldStandInShapeUI
