#pragma once

#include <ai.h>

#include <vector>
#include <string>

#include <maya/MBoundingBox.h>
#include "PolyTriangulator.h"

enum GeometryDrawingMode{
   GM_BOUNDING_BOX,
   GM_POLYGONS,
   GM_WIREFRAME,
   GM_POINTS,
   GM_NORMAL_AND_POLYGONS
};

 enum StandinSelectionFilter {
      STANDIN_GEOM_ALL,
      STANDIN_GEOM_UNSELECTED,
      STANDIN_GEOM_SELECTED
};

// interface for drawing
// so we could add support for curves
// point clouds or other primitives
class DLLEXPORT CArnoldDrawGeometry{
protected:
   CArnoldDrawGeometry(AtNode* node); // basic visibility and matrix queries
   
   AtVector m_BBMin, m_BBMax;
   AtMatrix m_matrix;
   AtArray* p_matrices;
   bool m_visible;
   bool m_invalid;
   bool m_selected;

  
public:

   virtual ~CArnoldDrawGeometry();

   virtual MBoundingBox GetBBox(bool transformed = true) const;
   virtual const AtMatrix& GetMatrix() const;

   bool Visible(StandinSelectionFilter filter = STANDIN_GEOM_ALL) const;
   bool Invalid() const;

   void SetSelected(bool b) {m_selected = b;}
   bool IsSelected() const {return m_selected;}
   
   // get the raw points
   virtual size_t PointCount() const { return 0; }
   virtual void GetPoints(float* points, const AtMatrix* matrix = NULL) const {}

   // get the vertices and normals after combining multiple index streams
   virtual size_t SharedVertexCount() const { return 0; }
   virtual void GetSharedVertices(float* outVertices, const AtMatrix* matrix = NULL) const {}
   virtual void GetSharedNormals(float* outNormals, const AtMatrix* matrix = NULL) const {}

   // get the wireframe indexing information
   virtual size_t WireIndexCount() const { return 0; }
   virtual void GetWireIndexing(unsigned int* outIndices, unsigned int vertexOffset = 0, bool sharedVertices = false) const {}

   // get the triangle indexing information
   virtual size_t TriangleIndexCount(bool sharedVertices = false) const { return 0; }
   virtual void GetTriangleIndexing(unsigned int* outIndices, unsigned int vertexOffset = 0, bool sharedVertices = false) const {}
};

class DLLEXPORT CArnoldDrawPolymesh : public CArnoldDrawGeometry{
private:
   std::vector<AtVector> m_vlist;
   std::vector<unsigned int> m_vidxs;
   std::vector<AtVector> m_nlist;
   std::vector<unsigned int> m_nidxs;
   std::vector<unsigned int> m_nsides;   

   // Triangulate polygons
   PolyTriangulator<unsigned int>* m_polyTriangulator;
   Triangulator* mTriangulator;

   void createPolyTriangulator();
   PolyTriangulator<unsigned int>& polyTriangulator() const;

   void createMultiStreamTriangulator();
   Triangulator& triangulator() const;

public:
   CArnoldDrawPolymesh(AtNode* node);
   ~CArnoldDrawPolymesh();  

   // get the raw points
   size_t PointCount() const override;
   void GetPoints(float* points, const AtMatrix* matrix = NULL) const override;

   size_t SharedVertexCount() const override;
   void GetSharedVertices(float* outVertices, const AtMatrix* matrix) const override;
   void GetSharedNormals(float* outNormals, const AtMatrix* matrix) const override;

   size_t WireIndexCount() const override;
   void GetWireIndexing(unsigned int* outIndices, unsigned int vertexOffset = 0, bool sharedVertices = false) const override;
   size_t TriangleIndexCount(bool sharedVertices = false) const override;
   void GetTriangleIndexing(unsigned int* outIndices, unsigned int vertexOffset, bool sharedVertices = false) const override;
};

class DLLEXPORT CArnoldDrawPoints : public CArnoldDrawGeometry{
private:
   std::vector<AtVector> m_points;

public:
   CArnoldDrawPoints(AtNode* node);
   ~CArnoldDrawPoints();  

   size_t PointCount() const override;
   void GetPoints(float* points, const AtMatrix* matrix = NULL) const override;
};

class DLLEXPORT CArnoldDrawGInstance : public CArnoldDrawGeometry {

public:
   CArnoldDrawGInstance(AtNode *node, const AtMatrix &m, bool m_inheritXForm);
   ~CArnoldDrawGInstance();

   void SetGeometryNode(CArnoldDrawGeometry* g) {m_geom = g;}

   MBoundingBox GetBBox(bool transformed = true) const override;
   const AtMatrix& GetMatrix() const override;
   const CArnoldDrawGeometry* GetGeometry() const {return m_geom;}

   // get the raw points
   size_t PointCount() const override {return (m_geom) ? m_geom->PointCount() : 0;}
   void GetPoints(float* points, const AtMatrix* matrix = NULL) const override;

   // get the vertices and normals after combining multiple index streams
   size_t SharedVertexCount() const override { return (m_geom) ? m_geom->SharedVertexCount() : 0; }
   void GetSharedVertices(float* outVertices, const AtMatrix* matrix = NULL) const override;
   void GetSharedNormals(float* outNormals, const AtMatrix* matrix = NULL) const override;

   // get the wireframe indexing information
   size_t WireIndexCount() const override { return (m_geom) ? m_geom->WireIndexCount() : 0; }
   void GetWireIndexing(unsigned int* outIndices, unsigned int vertexOffset = 0, bool sharedVertices = false) const override 
      {if (m_geom) m_geom->GetWireIndexing(outIndices, vertexOffset, sharedVertices);}

   // get the triangle indexing information
   size_t TriangleIndexCount(bool sharedVertices = false) const override { return (m_geom) ? m_geom->TriangleIndexCount(sharedVertices) : 0; }
   void GetTriangleIndexing(unsigned int* outIndices, unsigned int vertexOffset = 0, bool sharedVertices = false) const override 
      {if (m_geom) m_geom->GetTriangleIndexing(outIndices, vertexOffset, sharedVertices);}

private:
   CArnoldDrawGeometry* m_geom;   
   AtMatrix m_matrix;
   bool m_inheritXForm;
};

class DLLEXPORT CArnoldDrawProcedural : public CArnoldDrawGeometry {

public:
   CArnoldDrawProcedural(AtNode* node, AtProcViewportMode mode);
   ~CArnoldDrawProcedural();

   typedef unordered_map<std::string, CArnoldDrawGeometry*> geometryListType;
   typedef geometryListType::const_iterator geometryListIterType;

  // bounding box mode
   size_t PointCount() const override;
   void GetPoints(float* points, const AtMatrix* matrix = NULL) const override;
   size_t SharedVertexCount() const override;
   void GetSharedVertices(float* outVertices, const AtMatrix* matrix = NULL) const override;
   void GetSharedNormals(float* outNormals, const AtMatrix* matrix = NULL) const override;
   size_t WireIndexCount() const override;
   void GetWireIndexing(unsigned int* outIndices, unsigned int vertexOffset = 0, bool sharedVertices = false) const override;
   size_t TriangleIndexCount(bool sharedVertices = false) const override;
   void GetTriangleIndexing(unsigned int* outIndices, unsigned int vertexOffset = 0, bool sharedVertices = false) const override;

private:  

   geometryListType m_geometryList;
   MBoundingBox m_bbox;
};

class DLLEXPORT CArnoldDrawBox : public CArnoldDrawGeometry {

public:
   CArnoldDrawBox(AtNode* node);
   ~CArnoldDrawBox();
   
};
