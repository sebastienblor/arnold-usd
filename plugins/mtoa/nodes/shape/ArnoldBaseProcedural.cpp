#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif

#include "ArnoldBaseProcedural.h"
#include "nodes/ArnoldNodeIDs.h"

#include "translators/DagTranslator.h"
#include "utils/Universe.h"
#include "utils/MayaUtils.h"
#include "utils/ConstantStrings.h"

#include <ai_render.h>
#include <ai_dotass.h>
#include <ai_universe.h>
#include <ai_node_entry.h>
#include <ai_array.h>
#include <ai_nodes.h>
#include <ai_matrix.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MRenderUtil.h>
#include <maya/MBoundingBox.h>
#include <maya/MFloatVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MPlug.h>
#include <maya/MPoint.h>
#include <maya/MSelectInfo.h>
#include <maya/MDrawInfo.h>
#include <maya/MDrawRequest.h>
#include <maya/MDrawRequestQueue.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MDrawData.h>
#include <maya/MPlug.h>
#include <maya/MRenderUtil.h>
#include <maya/MMatrix.h>
#include <maya/MStringArray.h>
#include <maya/MString.h>
#include <maya/MDrawRequest.h>
#include <maya/MSelectionMask.h>
#include <maya/MSelectionList.h>
#include <maya/MFnNumericData.h>

#include <cstdio>
#include <fstream>
#include <string>

#define LEAD_COLOR            18 // green
#define ACTIVE_COLOR       15 // white
#define ACTIVE_AFFECTED_COLOR 8  // purple
#define DORMANT_COLOR         4  // blue
#define HILITE_COLOR       17 // pale blue

CArnoldProceduralData::CArnoldProceduralData()
{
   m_bbox = MBoundingBox(MPoint(-1.0f, -1.0f, -1.0f), MPoint(1.0f, 1.0f, 1.0f));
   m_isSelected = false;
   m_mode = 0;
   m_isDirty = true;
}

CArnoldProceduralData::~CArnoldProceduralData()
{
   Clear();
}

void CArnoldProceduralData::Clear()
{
   for (CArnoldDrawGeometry::geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
      delete it->second;
   m_geometryList.clear();
   m_isSelected = false;
   m_isDirty = true;
}

size_t CArnoldProceduralData::PointCount(StandinSelectionFilter filter) const
{
    size_t totalPoints = 0;
    for (CArnoldDrawGeometry::geometryListIterType it = m_geometryList.begin();
         it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible(filter))
        {
            totalPoints += it->second->PointCount();
        }
    }

    return totalPoints;
}

size_t CArnoldProceduralData::SharedVertexCount(StandinSelectionFilter filter) const
{
    size_t totalPoints = 0;
    for (CArnoldDrawGeometry::geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible(filter))
            totalPoints += it->second->SharedVertexCount();
    }

    return totalPoints;
}

size_t CArnoldProceduralData::WireIndexCount(StandinSelectionFilter filter) const
{
    size_t total = 0;
    for (CArnoldDrawGeometry::geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible(filter))
        {
            total += it->second->WireIndexCount();
        }
    }
    return total;
}

size_t CArnoldProceduralData::TriangleIndexCount(bool sharedVertices, StandinSelectionFilter filter) const
{
    size_t total = 0;
    for (CArnoldDrawGeometry::geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible(filter))
        {
            total += it->second->TriangleIndexCount(sharedVertices);
        }
    }
    return total;
}

static inline size_t ComputeVisibleGeometries(StandinSelectionFilter filter, 
                           const CArnoldDrawGeometry::geometryListType *geometries)
{
   if (geometries == nullptr)
      return 0;
   size_t total = 0;

   for (CArnoldDrawGeometry::geometryListIterType it = geometries->begin();
      it != geometries->end(); ++it)
   {
      auto geom = it->second;
      if (!geom->Visible(filter))
         continue;

      if (geom->HasChildGeometry()) 
         total += ComputeVisibleGeometries(filter, geom->GetChildGeometry());
      else 
         total++;
    }
    return total;
}
size_t CArnoldProceduralData::VisibleGeometryCount(StandinSelectionFilter filter) const
{
   return ComputeVisibleGeometries(filter, &m_geometryList);
}

CArnoldBaseProcedural::CArnoldBaseProcedural() : m_data(NULL), m_nodeDirtyId(0)
{
}

CArnoldBaseProcedural::~CArnoldBaseProcedural()
{
   if (m_nodeDirtyId != 0)
      MMessage::removeCallback(m_nodeDirtyId);

   delete m_data;
   m_data = NULL;

}

void CArnoldBaseProcedural::postConstructor()
{
   // This call allows the shape to have shading groups assigned
   setRenderable(true);
   MObject me = thisMObject();
   m_nodeDirtyId = MNodeMessage::addNodeDirtyPlugCallback(me, NodeDirtyCallback, this);
}

MStatus CArnoldBaseProcedural::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void CArnoldBaseProcedural::initializeCommonAttributes()
{
   MFnTypedAttribute tAttr;
   MFnNumericAttribute nAttr;
   MFnEnumAttribute eAttr;

   MObject mode = eAttr.create("mode", "mode", 0);
   eAttr.addField("Bounding Box", DM_BOUNDING_BOX);
   eAttr.addField("Per Object Bounding Box", DM_PER_OBJECT_BOUNDING_BOX);
   eAttr.addField("Polywire", DM_POLYWIRE);
   eAttr.addField("Wireframe", DM_WIREFRAME);
   eAttr.addField("Point Cloud", DM_POINT_CLOUD);
   eAttr.addField("Shaded Polywire", DM_SHADED_POLYWIRE);
   eAttr.addField("Shaded", DM_SHADED);
   addAttribute(mode);

   MObject selectedItems = tAttr.create("selectedItems", "selected_items", MFnData::kString);
   tAttr.setHidden(true);
   tAttr.setStorable(false);
   tAttr.setWritable(false);
   tAttr.setInternal(true);
   addAttribute(selectedItems);
}

bool CArnoldBaseProcedural::isBounded() const
{
   return true;
}

MBoundingBox CArnoldBaseProcedural::boundingBox() const
{
   // Returns the bounding box for the shape.
   if (m_data == nullptr)
      MBoundingBox(MPoint(-1.0f, -1.0f, -1.0f), MPoint(1.0f, 1.0f, 1.0f));

   CArnoldBaseProcedural* nonConstThis = const_cast<CArnoldBaseProcedural*> (this);
   nonConstThis->geometry();
   MPoint bbMin = m_data->m_bbox.min();
   MPoint bbMax = m_data->m_bbox.max();

   float minCoords[4];
   float maxCoords[4];

   bbMin.get(minCoords);
   bbMax.get(maxCoords);
   return MBoundingBox (minCoords, maxCoords);
}

//
// This function gets the draw mode from the shape
//
int CArnoldBaseProcedural::drawMode()
{
   MFnDependencyNode node(thisMObject());
   MStatus status;
   MPlug plug = node.findPlug("mode", true, &status);
   int mode;
   plug.getValue(mode);
   if (m_data)
      m_data->m_mode = mode;
   return mode;
}


MSelectionMask CArnoldBaseProcedural::getShapeSelectionMask() const
{
	MSelectionMask::SelectionType selType = MSelectionMask::kSelectMeshes;
    return MSelectionMask( selType );
}

void* CArnoldBaseProcedural::creator()
{
   return new CArnoldBaseProcedural();
}


CArnoldProceduralData* CArnoldBaseProcedural::geometry()
{
   if (m_data == NULL || m_data->m_isDirty)
   {
         // If we are in a batch render, it is not needed and it will cause the render crash. 
      if (IsBatch())
      {
         m_data->m_bbox.clear();
         return m_data;
      }
      updateGeometry();
      // at this point m_data should have been allocated
      if (m_data)
         m_data->m_isDirty = false;
   }

   return m_data;
}

void CArnoldBaseProcedural::UpdateSelectedItems()
{
   if (m_data == NULL)
      return;
   
   MStatus status;
   MPlug selPlug = MFnDependencyNode(thisMObject()).findPlug("selectedItems", true, &status);
   MString selectedItems;
   selPlug.getValue(selectedItems);
   MStringArray selectedItemsList;

   if (selectedItems.length() > 0)
   {
      selectedItems.split(',', selectedItemsList);
   }
   m_data->m_xformSelections.clear();
   m_data->m_selectedMap.clear();

   for (unsigned int i = 0; i < selectedItemsList.length(); ++i)
   {
      const MString &sel = selectedItemsList[i];
      if (sel.asChar()[sel.length() - 1] == '*')
      {
         m_data->m_xformSelections.append(sel.substringW(0, sel.length() - 2));
      }
      else
         m_data->m_selectedMap.insert(std::string(sel.asChar()));
   }
   
   CArnoldBaseProcedural* nonConstThis = const_cast<CArnoldBaseProcedural*> (this);
   nonConstThis->geometry();
   m_data->m_isSelected = false;
   for (CArnoldDrawGeometry::geometryListIterType it = m_data->m_geometryList.begin(); it != m_data->m_geometryList.end(); ++it)
   {
      CArnoldDrawGeometry* g = it->second;
      MString nodeName(it->first.c_str());

      if (g)
      {
         bool selected = (m_data->m_selectedMap.find(std::string(nodeName.asChar())) != m_data->m_selectedMap.end());
         if (!selected)
         {
            for (unsigned int i = 0; i < m_data->m_xformSelections.length(); ++i)
            {
               const MString &sel = m_data->m_xformSelections[i];
               if (nodeName.length() > sel.length() && nodeName.substringW(0, sel.length() - 1) == sel)
                  selected = true;
            }
         }
         if (selected)
            m_data->m_isSelected = true;

         g->SetSelected(selected);
      }      
   }
}

void CArnoldBaseProcedural::updateGeometry()
{
   if (m_data == NULL)
      m_data = new CArnoldProceduralData;
}

void CArnoldBaseProcedural::NodeDirtyCallback(MObject& node, MPlug& plug, void* clientData)
{
   CArnoldBaseProcedural *baseProc = static_cast<CArnoldBaseProcedural*>(clientData);
   if (baseProc)
      baseProc->NodeChanged(plug);
}

void CArnoldBaseProcedural::NodeChanged(MPlug &plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);

   if (m_data)
      m_data->m_isDirty = true;
   if (plugName == "selectedItems")
      UpdateSelectedItems();
}


void CArnoldBaseProcedural::DrawUniverse(const AtUniverse *universe)
{
   if (m_data == NULL)
      return;

   //clear current geo
   m_data->m_bbox.clear();
   m_data->Clear();
   
   // Set the viewport mode in case we find nested procedurals
   AtProcViewportMode viewport_mode = AI_PROC_BOXES;
   switch (m_data->m_mode)
   {
      case DM_BOUNDING_BOX:
      case DM_PER_OBJECT_BOUNDING_BOX:
         viewport_mode = AI_PROC_BOXES;
         break;
      case DM_POLYWIRE: // filled polygon
      case DM_WIREFRAME: // wireframe
      case DM_SHADED_POLYWIRE: // shaded polywire
      case DM_SHADED: // shaded
         viewport_mode = AI_PROC_POLYGONS;
         break;
      case DM_POINT_CLOUD: // points
         viewport_mode = AI_PROC_POINTS;
         break;
   }

   AtNodeIterator* iter = AiUniverseGetNodeIterator(universe, AI_NODE_SHAPE);
   std::vector<std::pair<CArnoldDrawGInstance *, std::string> > instances;
   int idx = 0;
   while (!AiNodeIteratorFinished(iter))
   {
      AtNode* node = AiNodeIteratorGetNext(iter);
      if (node == nullptr)
         continue;
   
      MString nodeName = MString(AiNodeGetName(node));

      if (nodeName.length() == 0)
      {
         nodeName = "__unnamed_proc_";
         nodeName += idx++;
         AiNodeSetStr(node, str::name, AtString(nodeName.asChar()));
      }

      CArnoldDrawGeometry* g = 0;
      bool isInstance = false;
      if (AiNodeIs(node, str::polymesh))
         g = new CArnoldDrawPolymesh(node);
      else if (AiNodeIs(node, str::points))
         g = new CArnoldDrawPoints(node);
      else if(AiNodeEntryGetDerivedType(AiNodeGetNodeEntry(node)) == AI_NODE_SHAPE_PROCEDURAL)
         g = new CArnoldDrawProcedural(node, viewport_mode);
      else if(AiNodeIs(node, str::box))
         g = new CArnoldDrawBox(node);
      else if (AiNodeIs(node, str::ginstance))
      {
         AtNode *source = (AtNode*)AiNodeGetPtr(node, str::node);
         if (!source)
            continue;
         AtMatrix total_matrix = AiNodeGetMatrix(node, str::matrix);
         bool inherit_xform = AiNodeGetBool(node, str::inherit_xform);
         AtNode *instanceNode = (AtNode*)AiNodeGetPtr(node, str::node);
         while(AiNodeIs(instanceNode, str::ginstance))
         {                  
            AtMatrix current_matrix = AiNodeGetMatrix(instanceNode, str::matrix);
            if (inherit_xform)
               total_matrix = AiM4Mult(total_matrix, current_matrix);
            
            inherit_xform = AiNodeGetBool(instanceNode, str::inherit_xform);
            instanceNode = (AtNode*)AiNodeGetPtr(instanceNode, str::node);
         }

         std::pair<CArnoldDrawGInstance *, std::string> instance(new CArnoldDrawGInstance(node, total_matrix, inherit_xform), AiNodeGetName(source));
         g = instance.first;
         instances.push_back(instance);
         isInstance = true;
      }
      else
         continue;
      if (g->Invalid())
      {
         delete g;
         continue;
      }
      if (!isInstance && g->Visible())
         m_data->m_bbox.expand(g->GetBBox());  

       
      bool selected = (m_data->m_selectedMap.find(std::string(nodeName.asChar())) != m_data->m_selectedMap.end());
      if (!selected)
      {
         for (unsigned int i = 0; i < m_data->m_xformSelections.length(); ++i)
         {
            const MString &sel = m_data->m_xformSelections[i];
            if (nodeName.length() > sel.length() && nodeName.substringW(0, sel.length() - 1) == sel)
               selected = true;
         }
      }
      if (selected)
         m_data->m_isSelected = true;
      
      g->SetSelected(selected);
      m_data->m_geometryList.insert(std::make_pair(std::string(AiNodeGetName(node)), g));
   }

   AiNodeIteratorDestroy(iter);
   
   for (auto instance : instances)
   {
      CArnoldDrawGInstance *g = instance.first;
      if (instance.first == nullptr)
         continue;
      
      CArnoldDrawGeometry::geometryListIterType srcIter = m_data->m_geometryList.find(instance.second);
      if (srcIter == m_data->m_geometryList.end() || srcIter->second == nullptr)
         continue;

      g->SetGeometryNode(srcIter->second);
      if (g->Visible())
         m_data->m_bbox.expand(g->GetBBox());
   }
}