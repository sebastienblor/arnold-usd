#include "ArnoldStandInGeometry.h"

#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>

CArnoldStandInGeometry::CArnoldStandInGeometry()
{
   
}

CArnoldStandInGeometry::~CArnoldStandInGeometry()
{
   
}

CArnoldPolymeshGeometry::CArnoldPolymeshGeometry(AtNode* node, AtMatrix inherited_matrix, bool inherit_xform, MBoundingBox& bbox)
{
   AtMatrix matrix;  
   
   if (inherit_xform)
   {
      AiNodeGetMatrix(node, "matrix", matrix);
      AiM4Mult(matrix, matrix, inherited_matrix);
   }
   else
      AiM4Copy(matrix, inherited_matrix);
   
   AtArray* vlist = AiNodeGetArray(node, "vlist");
   
   m_BBMin.x = AI_BIG;
   m_BBMin.y = AI_BIG;
   m_BBMin.z = AI_BIG;
   
   m_BBMax.x = -AI_BIG;
   m_BBMax.y = -AI_BIG;
   m_BBMax.z = -AI_BIG;
   
   if (vlist->nelements)
   {
      m_vlist.resize(vlist->nelements);
      for (AtUInt32 i = 0; i < vlist->nelements; ++i)
      {
         const AtPoint pnt = AiArrayGetPnt(vlist, i);
         AtPoint tmpPnt;
         AiM4PointByMatrixMult(&tmpPnt, matrix, &pnt);
         
         m_BBMin.x = MIN(m_BBMin.x, tmpPnt.x);
         m_BBMin.y = MIN(m_BBMin.y, tmpPnt.y);
         m_BBMin.z = MIN(m_BBMin.z, tmpPnt.z);
         
         m_BBMax.x = MAX(m_BBMax.x, tmpPnt.x);
         m_BBMax.y = MAX(m_BBMax.y, tmpPnt.y);
         m_BBMax.z = MAX(m_BBMax.z, tmpPnt.z);
         
         m_vlist[i] = tmpPnt;
      }
   }
   
   bbox.expand(MPoint(m_BBMin.x, m_BBMin.y, m_BBMin.z));
   bbox.expand(MPoint(m_BBMax.x, m_BBMax.y, m_BBMax.z));
   
   AtArray* vidxs = AiNodeGetArray(node, "vidxs");
   
   if (vidxs->nelements)
   {
      m_vidxs.resize(vidxs->nelements);
      for (AtUInt32 i = 0; i < vidxs->nelements; ++i)
         m_vidxs[i] = AiArrayGetUInt(vidxs, i);
   }
   
   AtArray* nlist = AiNodeGetArray(node, "nlist");
   
   if (nlist->nelements)
   {
      m_nlist.resize(nlist->nelements);
      for (AtUInt32 i = 0; i < nlist->nelements; ++i)
      {
         const AtVector pnt = AiArrayGetVec(nlist, i);
         AiM4VectorByMatrixMult(&m_nlist[i], matrix, &pnt);
      }
   }
   
   AtArray* nidxs = AiNodeGetArray(node, "nidxs");
   
   if (nidxs->nelements)
   {
      m_nidxs.resize(nidxs->nelements);
      for (AtUInt32 i = 0; i < nidxs->nelements; ++i)
         m_nidxs[i] = AiArrayGetUInt(nidxs, i);
   }
   
   AtArray* nsides = AiNodeGetArray(node, "nsides");
   
   if (nsides->nelements)
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
   for (size_t i = 0, id = 0; i < m_nsides.size(); ++i)
   {
      const AtUInt ns = m_nsides[i];
      glBegin(GL_POLYGON);
      for (AtUInt j = 0; j < ns; ++j)
      {
         const AtUInt vid = m_vidxs[id++];
         glVertex3fv(&m_vlist[vid].x);
      }
      glEnd();
   }
}

void CArnoldPolymeshGeometry::DrawWireframe() const
{
   for (size_t i = 0, id = 0; i < m_nsides.size(); ++i)
   {
      const AtUInt ns = m_nsides[i];
      glBegin(GL_LINE_STRIP);
      for (AtUInt j = 0; j < ns; ++j, ++id)
      {
         const AtUInt vid = m_vidxs[id];
         glVertex3fv(&m_vlist[vid].x);
      }
      glEnd();
   }
}

void CArnoldPolymeshGeometry::DrawPoints() const
{
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
      const AtUInt ns = m_nsides[i];
      glBegin(GL_POLYGON);
      for (AtUInt j = 0; j < ns; ++j, ++id)
      {
         const AtUInt vid = m_vidxs[id];
         const AtUInt nid = m_nidxs[id];
         glVertex3fv(&m_vlist[vid].x);
         glNormal3fv(&m_nlist[nid].x);
      }
      glEnd();
   }
}

void CArnoldPolymeshGeometry::DrawBoundingBox() const
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
