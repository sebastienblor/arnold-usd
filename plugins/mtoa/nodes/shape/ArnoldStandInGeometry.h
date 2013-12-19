#pragma once

#include <ai.h>

#include <vector>
#include <string>

#include <maya/MBoundingBox.h>

enum GeometryDrawingMode{
   GM_BOUNDING_BOX,
   GM_POLYGONS,
   GM_WIREFRAME,
   GM_POINTS,
   GM_NORMAL_AND_POLYGONS
};

// interface for drawing
// so we could add support for curves
// pointclouds or other primitives
class CArnoldStandInGeometry{
protected:
   CArnoldStandInGeometry(AtNode* node); // basic visibility and matrix queries
   
   AtVector m_BBMin, m_BBMax;
   AtMatrix m_matrix;
   AtArray* p_matrices;
   bool m_visible;
   bool m_invalid;

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
   virtual ~CArnoldStandInGeometry();

   virtual void Draw(int drawMode, bool applyTransform = true);   
   MBoundingBox GetBBox(bool transformed = true);
   bool Visible() const;
   bool Invalid() const;
};

class CArnoldPolymeshGeometry : public CArnoldStandInGeometry{
private:
   std::vector<AtVector> m_vlist;
   std::vector<unsigned int> m_vidxs;
   std::vector<AtVector> m_nlist;
   std::vector<unsigned int> m_nidxs;
   std::vector<unsigned int> m_nsides;   

   void DrawPolygons() const;
   void DrawWireframe() const;
   void DrawPoints() const;
   void DrawNormalAndPolygons() const;
public:
   CArnoldPolymeshGeometry(AtNode* node);
   ~CArnoldPolymeshGeometry();  
};

class CArnoldPointsGeometry : public CArnoldStandInGeometry{
private:
   std::vector<AtVector> m_points;

   void DrawPolygons() const;
   void DrawWireframe() const;
   void DrawPoints() const;
   void DrawNormalAndPolygons() const;
public:
   CArnoldPointsGeometry(AtNode* node);
   ~CArnoldPointsGeometry();  
};

class CArnoldStandInGInstance {
private:
   CArnoldStandInGeometry* p_geom;   
   AtMatrix m_matrix;
   bool m_inheritXForm;
public:
   CArnoldStandInGInstance(CArnoldStandInGeometry* g, AtMatrix m, bool i);
   ~CArnoldStandInGInstance();

   void Draw(int drawMode);
   MBoundingBox GetBBox();
};

class CArnoldProceduralGeometry : public CArnoldStandInGeometry{
private:
   void DrawPolygons() const;
   void DrawWireframe() const;
   void DrawPoints() const;
   void DrawNormalAndPolygons() const;
public:
   CArnoldProceduralGeometry(AtNode* node);
   ~CArnoldProceduralGeometry();
};

class CArnoldBoxGeometry : public CArnoldStandInGeometry{
private:
   void DrawPolygons() const;
   void DrawWireframe() const;
   void DrawPoints() const;
   void DrawNormalAndPolygons() const;
public:
   CArnoldBoxGeometry(AtNode* node);
   ~CArnoldBoxGeometry();
};
