#pragma once

#include "attributes/AttrHelper.h"
#include "common/UnorderedContainer.h"
#include "ArnoldDrawGeometry.h"

#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>
#include <maya/MNodeMessage.h>
#include <ai_nodes.h>

#include <vector>
#include <map>

enum ProceduralDrawingMode{
   DM_BOUNDING_BOX,
   DM_PER_OBJECT_BOUNDING_BOX,
   DM_POLYWIRE,
   DM_WIREFRAME,
   DM_POINT_CLOUD,
   DM_SHADED_POLYWIRE,
   DM_SHADED
};
// Geometry class
class DLLEXPORT CArnoldProceduralData
{
public:

   CArnoldProceduralData();
   ~CArnoldProceduralData();
   
   MBoundingBox m_bbox;   
   int m_mode;
   CArnoldDrawGeometry::geometryListType m_geometryList;
   bool m_isSelected;
   bool m_isDirty;
   MStringArray m_xformSelections;
   unordered_set<std::string> m_selectedMap;

   
   void Clear();
   size_t PointCount(StandinSelectionFilter selected = STANDIN_GEOM_ALL) const;
   size_t SharedVertexCount(StandinSelectionFilter selected = STANDIN_GEOM_ALL) const;
   size_t VisibleGeometryCount(StandinSelectionFilter selected = STANDIN_GEOM_ALL) const;
   size_t WireIndexCount(StandinSelectionFilter selected = STANDIN_GEOM_ALL) const;
   size_t TriangleIndexCount(bool sharedVertices = false, StandinSelectionFilter selected = STANDIN_GEOM_ALL) const;
};

// Shape class - defines the non-UI part of a shape node
class DLLEXPORT CArnoldBaseProcedural: public MPxSurfaceShape
{

public:
   CArnoldBaseProcedural();
   virtual ~CArnoldBaseProcedural();

   virtual void postConstructor();
   virtual MStatus compute(const MPlug& plug, MDataBlock& data);
   virtual bool isBounded() const;
   virtual MBoundingBox boundingBox() const;
   
   virtual MSelectionMask getShapeSelectionMask() const;
   
   CArnoldProceduralData* geometry();
   virtual void updateGeometry();

   static void initializeCommonAttributes();
   static void* creator();

   void DrawUniverse(const AtUniverse *unverse);
   virtual void NodeChanged(MPlug &plug);

   
   int drawMode();
   static void  NodeDirtyCallback(MObject& node, MPlug& plug, void* clientData);

   void UpdateSelectedItems();

protected:


   CArnoldProceduralData *m_data;
   MCallbackId m_nodeDirtyId;

};
