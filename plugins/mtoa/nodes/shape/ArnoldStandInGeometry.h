#ifndef ARNOLDSTANDINGEOMETRY_H
#define	ARNOLDSTANDINGEOMETRY_H

#include <ai.h>

#include <vector>

#include <maya/MGLFunctionTable.h>
#include <maya/MBoundingBox.h>

// interface for drawing
// so we could add support for curves
// pointclouds or other primitives
class CArnoldStandInGeometry{
protected:
   CArnoldStandInGeometry();
   static MGLFunctionTable* g_GLFT;
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
   
   static void setGLFTable(MGLFunctionTable* table);
};

class CArnoldPolymeshGeometry : public CArnoldStandInGeometry{
private:
   std::vector<AtVector> m_vlist;
   std::vector<AtUInt> m_vidxs;
   std::vector<AtVector> m_nlist;
   std::vector<AtUInt> m_nidxs;
   std::vector<AtUInt> m_nsides;
public:
   CArnoldPolymeshGeometry(AtNode* node, AtMatrix inherited_matrix, bool inherit_xform, MBoundingBox& bbox);
   ~CArnoldPolymeshGeometry();

   void DrawPolygons() const;
   void DrawWireframe() const;
   void DrawPoints() const;
   void DrawNormalAndPolygons() const;
};

#endif	/* ARNOLDSTANDINGEOMETRY_H */

