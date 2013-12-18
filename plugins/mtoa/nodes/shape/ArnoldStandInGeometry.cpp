#include "ArnoldStandInGeometry.h"

#include <maya/MMatrix.h>

#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>

CArnoldStandInGeometry::CArnoldStandInGeometry(AtNode* node)
{
   m_BBMin.x = AI_BIG;
   m_BBMin.y = AI_BIG;
   m_BBMin.z = AI_BIG;
   
   m_BBMax.x = -AI_BIG;
   m_BBMax.y = -AI_BIG;
   m_BBMax.z = -AI_BIG;
   AiM4Identity(m_matrix);

   p_matrices = AiArrayCopy(AiNodeGetArray(node, "matrix"));
   AiArrayGetMtx(p_matrices, 0, m_matrix);
   m_visible = AiNodeGetInt(node, "visibility") != 0;
}

CArnoldStandInGeometry::~CArnoldStandInGeometry()
{
   AiArrayDestroy(p_matrices);
}

void CArnoldStandInGeometry::DrawBoundingBox() const
{
   glBegin(GL_LINES);
   
   glVertex3f(m_BBMin.x, m_BBMin.y, m_BBMin.z);
   glVertex3f(m_BBMin.x, m_BBMin.y, m_BBMax.z);
   
   glVertex3f(m_BBMin.x, m_BBMax.y, m_BBMin.z);
   glVertex3f(m_BBMin.x, m_BBMax.y, m_BBMax.z);
   
   glVertex3f(m_BBMax.x, m_BBMin.y, m_BBMin.z);
   glVertex3f(m_BBMax.x, m_BBMin.y, m_BBMax.z);
   
   glVertex3f(m_BBMax.x, m_BBMax.y, m_BBMin.z);
   glVertex3f(m_BBMax.x, m_BBMax.y, m_BBMax.z);
   
   glVertex3f(m_BBMin.x, m_BBMin.y, m_BBMin.z);
   glVertex3f(m_BBMin.x, m_BBMax.y, m_BBMin.z);
   
   glVertex3f(m_BBMin.x, m_BBMin.y, m_BBMax.z);
   glVertex3f(m_BBMin.x, m_BBMax.y, m_BBMax.z);
   
   glVertex3f(m_BBMax.x, m_BBMin.y, m_BBMin.z);
   glVertex3f(m_BBMax.x, m_BBMax.y, m_BBMin.z);
   
   glVertex3f(m_BBMax.x, m_BBMin.y, m_BBMax.z);
   glVertex3f(m_BBMax.x, m_BBMax.y, m_BBMax.z);
   
   glVertex3f(m_BBMin.x, m_BBMin.y, m_BBMin.z);
   glVertex3f(m_BBMax.x, m_BBMin.y, m_BBMin.z);
   
   glVertex3f(m_BBMin.x, m_BBMax.y, m_BBMin.z);
   glVertex3f(m_BBMax.x, m_BBMax.y, m_BBMin.z);
   
   glVertex3f(m_BBMin.x, m_BBMin.y, m_BBMax.z);
   glVertex3f(m_BBMax.x, m_BBMin.y, m_BBMax.z);
   
   glVertex3f(m_BBMin.x, m_BBMax.y, m_BBMax.z);
   glVertex3f(m_BBMax.x, m_BBMax.y, m_BBMax.z);
   
   glEnd();
}

void CArnoldStandInGeometry::Draw(int drawMode, bool applyTransform)
{
   if (applyTransform)
   {
      glPushMatrix();
      glMultMatrixf(&m_matrix[0][0]);
   }
   switch (drawMode)
   {
      case GM_BOUNDING_BOX:
         DrawBoundingBox();
         break;
      case GM_POLYGONS:
         DrawPolygons();
         break;
      case GM_WIREFRAME:
         DrawWireframe();
         break;
      case GM_POINTS:
         DrawPoints();
         break;
      case GM_NORMAL_AND_POLYGONS:
         DrawNormalAndPolygons();
         break;
      default:
         break;
   }
   if (applyTransform)
      glPopMatrix();
}

MBoundingBox CArnoldStandInGeometry::GetBBox(bool transformed)
{
   if (transformed)
   {
      
      MBoundingBox bboxRet;
      for (unsigned int i = 0; i < p_matrices->nkeys; ++i)
      {
         AtMatrix mtx;
         AiM4Identity(mtx);
         AiArrayGetMtx(p_matrices, i, mtx);
         MBoundingBox bbox(MPoint(m_BBMin.x, m_BBMin.y, m_BBMin.z), MPoint(m_BBMax.x, m_BBMax.y, m_BBMax.z));
         bbox.transformUsing(MMatrix(mtx));
         bboxRet.expand(bbox);
      }
      return bboxRet;
   }
   else return MBoundingBox(MPoint(m_BBMin.x, m_BBMin.y, m_BBMin.z), MPoint(m_BBMax.x, m_BBMax.y, m_BBMax.z));
}

bool CArnoldStandInGeometry::Visible() const
{
   return m_visible;
}

CArnoldPolymeshGeometry::CArnoldPolymeshGeometry(AtNode* node) : CArnoldStandInGeometry(node)
{  
   AtArray* vlist = AiNodeGetArray(node, "vlist");  
   
   if ((vlist != 0) && vlist->nelements)
   {
      m_vlist.resize(vlist->nelements);
      for (AtUInt32 i = 0; i < vlist->nelements; ++i)
      {
         AtPoint pnt = AiArrayGetPnt(vlist, i);
         if (!AiIsFinite(pnt.x) || !AiIsFinite(pnt.y) || !AiIsFinite(pnt.z))
            pnt = AI_V3_ZERO;
         m_BBMin.x = MIN(m_BBMin.x, pnt.x);
         m_BBMin.y = MIN(m_BBMin.y, pnt.y);
         m_BBMin.z = MIN(m_BBMin.z, pnt.z);
         
         m_BBMax.x = MAX(m_BBMax.x, pnt.x);
         m_BBMax.y = MAX(m_BBMax.y, pnt.y);
         m_BBMax.z = MAX(m_BBMax.z, pnt.z);
         
         m_vlist[i] = pnt;
      }
      for (AtUInt32 i = vlist->nelements; i < (vlist->nelements * vlist->nkeys); ++i)
      {
         AtPoint pnt = AiArrayGetPnt(vlist, i);
         if (!AiIsFinite(pnt.x) || !AiIsFinite(pnt.y) || !AiIsFinite(pnt.z))
            continue;
         m_BBMin.x = MIN(m_BBMin.x, pnt.x);
         m_BBMin.y = MIN(m_BBMin.y, pnt.y);
         m_BBMin.z = MIN(m_BBMin.z, pnt.z);
         
         m_BBMax.x = MAX(m_BBMax.x, pnt.x);
         m_BBMax.y = MAX(m_BBMax.y, pnt.y);
         m_BBMax.z = MAX(m_BBMax.z, pnt.z);
      }
   }
   
   AtArray* vidxs = AiNodeGetArray(node, "vidxs");
   
   if ((vidxs != 0) && vidxs->nelements)
   {
      m_vidxs.resize(vidxs->nelements);
      for (AtUInt32 i = 0; i < vidxs->nelements; ++i)
         m_vidxs[i] = AiArrayGetUInt(vidxs, i);
   }
   
   AtArray* nlist = AiNodeGetArray(node, "nlist");
   
   if ((nlist != 0) && nlist->nelements)
   {
      m_nlist.resize(nlist->nelements);
      for (AtUInt32 i = 0; i < nlist->nelements; ++i)
         m_nlist[i] = AiArrayGetVec(nlist, i);
   }
   
   AtArray* nidxs = AiNodeGetArray(node, "nidxs");
   
   if ((nidxs != 0) && nidxs->nelements)
   {
      m_nidxs.resize(nidxs->nelements);
      for (AtUInt32 i = 0; i < nidxs->nelements; ++i)
         m_nidxs[i] = AiArrayGetUInt(nidxs, i);
   }
   
   AtArray* nsides = AiNodeGetArray(node, "nsides");
   
   if ((nsides != 0) && nsides->nelements)
   {
      m_nsides.resize(nsides->nelements);
      for (AtUInt32 i = 0; i < nsides->nelements; ++i)
         m_nsides[i] = AiArrayGetUInt(nsides, i);
   }
}

CArnoldPolymeshGeometry::~CArnoldPolymeshGeometry()
{
   
}

void CArnoldPolymeshGeometry::DrawPolygons() const
{
   if ((m_vlist.size() == 0) || (m_vidxs.size() == 0))
      return;
   for (size_t i = 0, id = 0; i < m_nsides.size(); ++i)
   {
      const unsigned int ns = m_nsides[i];
      glBegin(GL_POLYGON);
      for (unsigned int j = 0; j < ns; ++j)
      {
         const unsigned int vid = m_vidxs[id++];
         glVertex3fv(&m_vlist[vid].x);
      }
      glEnd();
   }
}

void CArnoldPolymeshGeometry::DrawWireframe() const
{
   if ((m_vlist.size() == 0) || (m_vidxs.size() == 0))
      return;
   for (size_t i = 0, id = 0; i < m_nsides.size(); ++i)
   {
      const unsigned int ns = m_nsides[i];
      glBegin(GL_LINE_STRIP);
      for (unsigned int j = 0; j < ns; ++j, ++id)
      {
         const unsigned int vid = m_vidxs[id];
         glVertex3fv(&m_vlist[vid].x);
      }
      glEnd();
   }
}

void CArnoldPolymeshGeometry::DrawPoints() const
{
   if (m_vlist.size() == 0)
      return;
   glEnableClientState(GL_VERTEX_ARRAY);
   
   glVertexPointer(3, GL_FLOAT, 0, &m_vlist[0]);
   glDrawArrays(GL_POINTS, 0, (GLsizei)m_vlist.size());
   
   glDisableClientState(GL_VERTEX_ARRAY);
}

void CArnoldPolymeshGeometry::DrawNormalAndPolygons() const
{
   if ((m_nlist.size() == 0) || (m_nidxs.size() == 0))
   {
      DrawPolygons();
      return;
   }
   
   for (size_t i = 0, id = 0; i < m_nsides.size(); ++i)
   {
      const unsigned int ns = m_nsides[i];
      glBegin(GL_POLYGON);
      for (unsigned int j = 0; j < ns; ++j, ++id)
      {
         const unsigned int vid = m_vidxs[id];
         const unsigned int nid = m_nidxs[id];
         glNormal3fv(&m_nlist[nid].x);
         glVertex3fv(&m_vlist[vid].x);         
      }
      glEnd();
   }
}

CArnoldPointsGeometry::CArnoldPointsGeometry(AtNode* node) : CArnoldStandInGeometry(node)
{   
   AtArray* points = AiNodeGetArray(node, "points");
   
   if ((points != 0) && points->nelements > 0)
   {
      m_points.resize(points->nelements);
      for (AtUInt32 i = 0; i < points->nelements; ++i)
      {
         AtVector pnt = AiArrayGetVec(points, i);
         
         m_BBMin.x = MIN(m_BBMin.x, pnt.x);
         m_BBMin.y = MIN(m_BBMin.y, pnt.y);
         m_BBMin.z = MIN(m_BBMin.z, pnt.z);
         
         m_BBMax.x = MAX(m_BBMax.x, pnt.x);
         m_BBMax.y = MAX(m_BBMax.y, pnt.y);
         m_BBMax.z = MAX(m_BBMax.z, pnt.z);
         
         m_points[i] = pnt;
      }
   }
}

CArnoldPointsGeometry::~CArnoldPointsGeometry()
{
   
}

void CArnoldPointsGeometry::DrawPolygons() const
{
   DrawPoints();
}

void CArnoldPointsGeometry::DrawWireframe() const
{
   DrawPoints();
}

void CArnoldPointsGeometry::DrawPoints() const
{
   glEnableClientState(GL_VERTEX_ARRAY);
   
   glVertexPointer(3, GL_FLOAT, 0, &m_points[0]);
   glDrawArrays(GL_POINTS, 0, (GLsizei)m_points.size());
   
   glDisableClientState(GL_VERTEX_ARRAY);
}

void CArnoldPointsGeometry::DrawNormalAndPolygons() const
{
   DrawPoints();
}

CArnoldStandInGInstance::CArnoldStandInGInstance(CArnoldStandInGeometry* g, AtMatrix m, bool i) :
   p_geom(g), m_inheritXForm(i)
{
   AiM4Copy(m_matrix, m);
}

CArnoldStandInGInstance::~CArnoldStandInGInstance()
{

}

void CArnoldStandInGInstance::Draw(int DrawMode)
{
   glPushMatrix();
   glMultMatrixf(&m_matrix[0][0]);
   p_geom->Draw(DrawMode, m_inheritXForm);
   glPopMatrix();
}

MBoundingBox CArnoldStandInGInstance::GetBBox()
{
   if (m_inheritXForm)
   {
      MBoundingBox bbox = p_geom->GetBBox();
      bbox.transformUsing(MMatrix(m_matrix));
      return bbox;
   }
   else
   {
      MBoundingBox bbox = p_geom->GetBBox(false);
      bbox.transformUsing(MMatrix(m_matrix));
      return bbox;
   }
}

CArnoldProceduralGeometry::CArnoldProceduralGeometry(AtNode* node) : CArnoldStandInGeometry(node)
{
   m_BBMin = AiNodeGetPnt(node, "min");
   m_BBMax = AiNodeGetPnt(node, "max");
}

CArnoldProceduralGeometry::~CArnoldProceduralGeometry()
{
   
}

void CArnoldProceduralGeometry::DrawPolygons() const
{
   DrawBoundingBox();
}

void CArnoldProceduralGeometry::DrawWireframe() const
{
   DrawBoundingBox();
}

void CArnoldProceduralGeometry::DrawPoints() const
{
   DrawBoundingBox();
}

void CArnoldProceduralGeometry::DrawNormalAndPolygons() const
{
   DrawBoundingBox();
}

CArnoldBoxGeometry::CArnoldBoxGeometry(AtNode* node) : CArnoldStandInGeometry(node)
{
   m_BBMin = AiNodeGetPnt(node, "min");
   m_BBMax = AiNodeGetPnt(node, "max");
}

CArnoldBoxGeometry::~CArnoldBoxGeometry()
{
   
}

void CArnoldBoxGeometry::DrawPolygons() const
{
   DrawBoundingBox();
}

void CArnoldBoxGeometry::DrawWireframe() const
{
   DrawBoundingBox();
}

void CArnoldBoxGeometry::DrawPoints() const
{
   DrawBoundingBox();
}

void CArnoldBoxGeometry::DrawNormalAndPolygons() const
{
   DrawBoundingBox();
}
