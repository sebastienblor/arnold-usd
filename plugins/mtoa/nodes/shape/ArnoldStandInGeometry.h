#pragma once

#include <ai.h>

#include <vector>

#include <maya/MBoundingBox.h>

// interface for drawing
// so we could add support for curves
// pointclouds or other primitives
class CArnoldStandInGeometry{
protected:
   CArnoldStandInGeometry();
public:
   virtual ~CArnoldStandInGeometry();
   
   // simple polygons, without normals
   virtual void DrawPolygons() const = 0;
   
   // wireframe cage
   virtual void DrawWireframe() const = 0;
   
   // points from vertices
   virtual void DrawPoints() const = 0;
   
   // polygons and normals for shaded mode
   virtual void DrawNormalAndPolygons() const = 0;
   
   // bounding box mode
   virtual void DrawBoundingBox() const = 0;
};

class CArnoldPolymeshGeometry : public CArnoldStandInGeometry{
private:
   std::vector<AtVector> m_vlist;
   std::vector<AtUInt> m_vidxs;
   std::vector<AtVector> m_nlist;
   std::vector<AtUInt> m_nidxs;
   std::vector<AtUInt> m_nsides;
   AtVector m_BBMin, m_BBMax;
public:
   CArnoldPolymeshGeometry(AtNode* node, AtMatrix inherited_matrix, bool inherit_xform, MBoundingBox& bbox);
   ~CArnoldPolymeshGeometry();

   void DrawPolygons() const;
   void DrawWireframe() const;
   void DrawPoints() const;
   void DrawNormalAndPolygons() const;
   void DrawBoundingBox() const;
};

class CArnoldPointsGeometry : public CArnoldStandInGeometry{
private:
   
public:
   CArnoldPointsGeometry(AtNode* node, AtMatrix inherited_matrix, bool inherit_xfrom, MBoundingBox& bbox);
   ~CArnoldPointsGeometry();
   
   void DrawPolygons() const;
   void DrawWireframe() const;
   void DrawPoints() const;
   void DrawNormalAndPolygons() const;
   void DrawBoundingBox() const;
};
