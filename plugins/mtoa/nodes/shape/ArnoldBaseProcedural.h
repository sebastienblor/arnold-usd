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
   
   typedef unordered_map<std::string, CArnoldDrawGeometry*> geometryListType;
   typedef geometryListType::const_iterator geometryListIterType;
   typedef std::vector<CArnoldDrawGInstance*> instanceListType;
   typedef instanceListType::const_iterator instanceListIterType;

   MBoundingBox m_bbox;   
   int m_mode;
   geometryListType m_geometryList;
   instanceListType m_instanceList;
   bool m_isSelected;
   bool m_isDirty;
   MStringArray m_xformSelections;
   unordered_set<std::string> m_selectedMap;

   
   void Clear();
   void Draw(int DrawMode);
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

   
   int drawMode();
   static void  AttrChangedCallback(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void* clientData);
   void UpdateSelectedItems();

protected:


   CArnoldProceduralData *m_data;
   MCallbackId m_attrChangeId;

};
