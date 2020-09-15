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

  

   // simple polygons, without normals
   virtual void DrawPolygons() const = 0;
   
   // wireframe cage
   virtual void DrawWireframe() const = 0;
   
   // points from vertices
   virtual void DrawPoints() const = 0;
   
   // polygons and normals for shaded mode
   virtual void DrawNormalAndPolygons() const = 0;
   
   // bounding box mode
   virtual void DrawBoundingBox() const;
public:
   virtual ~CArnoldDrawGeometry();

   virtual void Draw(int drawMode, bool applyTransform = true);   
   MBoundingBox GetBBox(bool transformed = true) const;
   const AtMatrix& GetMatrix() const;

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
   virtual void GetWireIndexing(unsigned int* outIndices, unsigned int vertexOffset = 0) const {}

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

   void DrawPolygons() const;
   void DrawWireframe() const;
   void DrawPoints() const;
   void DrawNormalAndPolygons() const;

   void createPolyTriangulator();
   PolyTriangulator<unsigned int>& polyTriangulator() const;

   void createMultiStreamTriangulator();
   Triangulator& triangulator() const;

public:
   CArnoldDrawPolymesh(AtNode* node);
   ~CArnoldDrawPolymesh();  

   // get the raw points
   virtual size_t PointCount() const;
   virtual void GetPoints(float* points, const AtMatrix* matrix = NULL) const;

   virtual size_t SharedVertexCount() const;
   virtual void GetSharedVertices(float* outVertices, const AtMatrix* matrix) const;
   virtual void GetSharedNormals(float* outNormals, const AtMatrix* matrix) const;

   virtual size_t WireIndexCount() const;
   virtual void GetWireIndexing(unsigned int* outIndices, unsigned int vertexOffset) const;
   virtual size_t TriangleIndexCount(bool sharedVertices = false) const;
   virtual void GetTriangleIndexing(unsigned int* outIndices, unsigned int vertexOffset, bool sharedVertices = false) const;
};

class DLLEXPORT CArnoldDrawPoints : public CArnoldDrawGeometry{
private:
   std::vector<AtVector> m_points;

   void DrawPolygons() const;
   void DrawWireframe() const;
   void DrawPoints() const;
   void DrawNormalAndPolygons() const;

public:
   CArnoldDrawPoints(AtNode* node);
   ~CArnoldDrawPoints();  

   virtual size_t PointCount() const;
   virtual void GetPoints(float* points, const AtMatrix* matrix = NULL) const;
};

class DLLEXPORT CArnoldDrawGInstance {
private:
   CArnoldDrawGeometry* p_geom;   
   AtMatrix m_matrix;
   bool m_inheritXForm;
public:
   CArnoldDrawGInstance(CArnoldDrawGeometry* g, const AtMatrix &m, bool i);
   ~CArnoldDrawGInstance();

   void Draw(int drawMode);
   MBoundingBox GetBBox() const;
   const AtMatrix& GetMatrix() const;
   const CArnoldDrawGeometry& GetGeometry() const;
};

class DLLEXPORT CArnoldDrawProcedural : public CArnoldDrawGeometry {
private:
   void DrawPolygons() const;
   void DrawWireframe() const;
   void DrawPoints() const;
   void DrawNormalAndPolygons() const;
public:
   CArnoldDrawProcedural(AtNode* node);
   ~CArnoldDrawProcedural();
};

class DLLEXPORT CArnoldDrawBox : public CArnoldDrawGeometry {
private:
   void DrawPolygons() const;
   void DrawWireframe() const;
   void DrawPoints() const;
   void DrawNormalAndPolygons() const;
public:
   CArnoldDrawBox(AtNode* node);
   ~CArnoldDrawBox();
};
