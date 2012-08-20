#include "ArnoldStandInGeometry.h"

MGLFunctionTable* CArnoldStandInGeometry::g_GLFT = 0;

CArnoldStandInGeometry::CArnoldStandInGeometry()
{
   
}

CArnoldStandInGeometry::~CArnoldStandInGeometry()
{
   
}

void CArnoldStandInGeometry::setGLFTable(MGLFunctionTable* table)
{
   g_GLFT = table;
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
   
   if (vlist->nelements)
   {
      m_vlist.resize(vlist->nelements);
      for (AtUInt32 i = 0; i < vlist->nelements; ++i)
      {
         const AtPoint pnt = AiArrayGetPnt(vlist, i);
         AtPoint tmpPnt;
         AiM4PointByMatrixMult(&tmpPnt, matrix, &pnt);
         bbox.expand(MPoint(tmpPnt.x, tmpPnt.y, tmpPnt.z));
         m_vlist[i] = tmpPnt;
      }
   }
   
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
      g_GLFT->glBegin(MGL_POLYGON);
      for (AtUInt j = 0; j < ns; ++j)
      {
         const AtUInt vid = m_vidxs[id++];
         g_GLFT->glVertex3fv(&m_vlist[vid].x);
      }
      g_GLFT->glEnd();
   }
}

void CArnoldPolymeshGeometry::DrawWireframe() const
{
   for (size_t i = 0, id = 0; i < m_nsides.size(); ++i)
   {
      const AtUInt ns = m_nsides[i];
      g_GLFT->glBegin(MGL_LINE_STRIP);
      for (AtUInt j = 0; j < ns; ++j, ++id)
      {
         const AtUInt vid = m_vidxs[id];
         g_GLFT->glVertex3fv(&m_vlist[vid].x);
      }
      g_GLFT->glEnd();
   }
}

void CArnoldPolymeshGeometry::DrawPoints() const
{
   g_GLFT->glEnableClientState(MGL_VERTEX_ARRAY);
   
   g_GLFT->glVertexPointer(3, MGL_FLOAT, 0, &m_vlist[0]);
   g_GLFT->glDrawArrays(MGL_POINTS, 0, m_vlist.size());
   
   g_GLFT->glDisableClientState(MGL_VERTEX_ARRAY);
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
      g_GLFT->glBegin(MGL_POLYGON);
      for (AtUInt j = 0; j < ns; ++j, ++id)
      {
         const AtUInt vid = m_vidxs[id];
         const AtUInt nid = m_nidxs[id];
         g_GLFT->glVertex3fv(&m_vlist[vid].x);
         g_GLFT->glNormal3fv(&m_nlist[nid].x);
      }
      g_GLFT->glEnd();
   }
}
