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
#include "scene/MayaScene.h"
#include "utils/MayaUtils.h"

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
   for (geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
      delete it->second;
   m_geometryList.clear();

   for (instanceListIterType it = m_instanceList.begin();
        it != m_instanceList.end(); ++it)
      delete (*it);
   m_instanceList.clear();
   m_isSelected = false;
   m_isDirty = true;
}

void CArnoldProceduralData::Draw(int DrawMode)
{
   for (geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
   {
      if (it->second->Visible())
         it->second->Draw(DrawMode);
   }

   for (instanceListIterType it = m_instanceList.begin();
        it != m_instanceList.end(); ++it)
      (*it)->Draw(DrawMode);
}

size_t CArnoldProceduralData::PointCount(StandinSelectionFilter filter) const
{
    size_t totalPoints = 0;
    for (geometryListIterType it = m_geometryList.begin();
         it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible(filter))
        {
            totalPoints += it->second->PointCount();
        }
    }

    for (instanceListIterType it = m_instanceList.begin();
         it != m_instanceList.end(); ++it)
    {
        totalPoints += (*it)->GetGeometry().PointCount();
    }
    return totalPoints;
}

size_t CArnoldProceduralData::SharedVertexCount(StandinSelectionFilter filter) const
{
    size_t totalPoints = 0;
    for (geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible(filter))
            totalPoints += it->second->SharedVertexCount();
    }

    for (instanceListIterType it = m_instanceList.begin();
        it != m_instanceList.end(); ++it)
        totalPoints += (*it)->GetGeometry().SharedVertexCount();
    return totalPoints;
}

size_t CArnoldProceduralData::WireIndexCount(StandinSelectionFilter filter) const
{
    size_t total = 0;
    for (geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible(filter))
        {
            total += it->second->WireIndexCount();
        }
    }

    for (instanceListIterType it = m_instanceList.begin();
        it != m_instanceList.end(); ++it)
    {
        total += (*it)->GetGeometry().WireIndexCount();
        
    }
    return total;
}

size_t CArnoldProceduralData::TriangleIndexCount(bool sharedVertices, StandinSelectionFilter filter) const
{
    size_t total = 0;
    for (geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible(filter))
        {
            total += it->second->TriangleIndexCount(sharedVertices);
        }
    }

    for (instanceListIterType it = m_instanceList.begin();
        it != m_instanceList.end(); ++it)
    {
        total += (*it)->GetGeometry().TriangleIndexCount(sharedVertices);
        
    }
    return total;
}

size_t CArnoldProceduralData::VisibleGeometryCount(StandinSelectionFilter filter) const
{
    size_t total = 0;
    for (geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible(filter))
            total++;
    }

    for (instanceListIterType it = m_instanceList.begin();
        it != m_instanceList.end(); ++it)
    {
        total++;
    }
    return total;
}

CArnoldBaseProcedural::CArnoldBaseProcedural() : m_nodeDirtyId(0), m_data(NULL)
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
   m_nodeDirtyId = MNodeMessage::addNodeDirtyCallback(me, NodeDirtyCallback, this);
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
   CArnoldBaseProcedural* nonConstThis = const_cast<CArnoldBaseProcedural*> (this);
   CArnoldProceduralData* geom = nonConstThis->geometry();
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
      if(CMayaScene::GetArnoldSession() && CMayaScene::GetArnoldSession()->IsBatch())
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
   CArnoldProceduralData* geom = nonConstThis->geometry();
   m_data->m_isSelected = false;
   for (CArnoldProceduralData::geometryListIterType it = m_data->m_geometryList.begin(); it != m_data->m_geometryList.end(); ++it)
   {
      CArnoldDrawGeometry* g = it->second;
      MString nodeName(it->first.c_str());

      bool selected = false;

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

   static const AtString polymesh_str("polymesh");
   static const AtString points_str("points");
   static const AtString procedural_str("procedural");
   static const AtString box_str("box");
   static const AtString ginstance_str("ginstance");
   
   AtNodeIterator* iter = AiUniverseGetNodeIterator(universe, AI_NODE_SHAPE);
   while (!AiNodeIteratorFinished(iter))
   {
      AtNode* node = AiNodeIteratorGetNext(iter);
      // Note: the procedural node itself isn't supposed to be in this universe
      //if (node == procedural)
      //   continue;
      if (node)
      {  
         MString nodeName = MString(AiNodeGetName(node));
         CArnoldDrawGeometry* g = 0;
         if (AiNodeIs(node, polymesh_str))
            g = new CArnoldDrawPolymesh(node);
         else if (AiNodeIs(node, points_str))
            g = new CArnoldDrawPoints(node);
         else if(AiNodeIs(node, procedural_str))
            g = new CArnoldDrawProcedural(node);
         else if(AiNodeIs(node, box_str))
            g = new CArnoldDrawBox(node);
         else
            continue;
         if (g->Invalid())
         {
            delete g;
            continue;
         }
         if (g->Visible())
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
   }

   AiNodeIteratorDestroy(iter);
   

   iter = AiUniverseGetNodeIterator(universe, AI_NODE_SHAPE);

   while (!AiNodeIteratorFinished(iter))
   {
      AtNode* node = AiNodeIteratorGetNext(iter);
      // the procedural is supposed to be in its own universe, not in this one
      // if (node == procedural)
      //   continue;
      if (node)
      {
         if (AiNodeGetByte(node, "visibility") == 0)
            continue;
         AtMatrix total_matrix = AiM4Identity();
         bool inherit_xform = true;
         bool isInstance = false;
         while(AiNodeIs(node, ginstance_str))
         {                  
            isInstance = true;
            AtMatrix current_matrix = AiNodeGetMatrix(node, "matrix");
            if (inherit_xform)
            {
               total_matrix = AiM4Mult(total_matrix, current_matrix);
            }
            inherit_xform = AiNodeGetBool(node, "inherit_xform");
            node = (AtNode*)AiNodeGetPtr(node, "node");
         }
         if (!isInstance)
            continue;
         if (AiNodeIs(node, polymesh_str) || AiNodeIs(node, points_str) || AiNodeIs(node, procedural_str))
         {
            std::string nodeName(AiNodeGetName(node));
            CArnoldProceduralData::geometryListIterType giter = m_data->m_geometryList.find(nodeName);
            if (giter != m_data->m_geometryList.end())
            {
               CArnoldDrawGInstance* gi = new CArnoldDrawGInstance(giter->second, total_matrix, inherit_xform);
               m_data->m_instanceList.push_back(gi);
               m_data->m_bbox.expand(gi->GetBBox());
            }
         }               
      }
   }

   AiNodeIteratorDestroy(iter);
}