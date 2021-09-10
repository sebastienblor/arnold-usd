#include "ArnoldDrawGeometry.h"
#include <maya/MDistance.h>
#include <maya/MMatrix.h>

#if defined(_DARWIN)
   #include <OpenGL/gl.h>
#else
   #include <GL/gl.h>
#endif

// memcpy between AtMatrix and MMatrix doesn't work (see #3237)
// so we need to copy each element of the matrix
static MMatrix AtMatrixToMMatrix(const AtMatrix& mtx)
{
   MMatrix res;
   for (int j = 0; j < 4; ++j)
      for (int i = 0; i < 4; ++i)
         res[i][j] = mtx[i][j];

  return res;
}

CArnoldDrawGeometry::CArnoldDrawGeometry(AtNode* node)
{
   m_BBMin.x = AI_BIG;
   m_BBMin.y = AI_BIG;
   m_BBMin.z = AI_BIG;
   
   m_BBMax.x = -AI_BIG;
   m_BBMax.y = -AI_BIG;
   m_BBMax.z = -AI_BIG;
   
   p_matrices = AiArrayCopy(AiNodeGetArray(node, "matrix"));

   if(!AiArrayGetNumElements(p_matrices)){
	  AiArrayResize(p_matrices, 1, 1);
	  AiArraySetMtx(p_matrices, 0, AiM4Identity());
   }

   m_matrix = AiArrayGetMtx(p_matrices, 0);
   m_visible = AiNodeGetByte(node, "visibility") != 0;
   m_invalid = false;
}

CArnoldDrawGeometry::~CArnoldDrawGeometry()
{
   AiArrayDestroy(p_matrices);
}


MBoundingBox CArnoldDrawGeometry::GetBBox(bool transformed) const
{
   if (transformed)
   {   
      MBoundingBox bbox(MPoint(m_BBMin.x, m_BBMin.y, m_BBMin.z), MPoint(m_BBMax.x, m_BBMax.y, m_BBMax.z));

      // Note that we're no longer considering motion blur for the bbox. Viewport display doesn't deserve 
      // to loose more time to get the motion data. Other geometries don't show motion neither...
      MMatrix mmtx = AtMatrixToMMatrix(m_matrix);
      bbox.transformUsing(mmtx);
      MPoint min = bbox.min();
      MPoint max = bbox.max();
       
      return bbox;
   }
   else return MBoundingBox(MPoint(m_BBMin.x, m_BBMin.y, m_BBMin.z), MPoint(m_BBMax.x, m_BBMax.y, m_BBMax.z));
  
}

const AtMatrix& CArnoldDrawGeometry::GetMatrix() const
{
   return m_matrix;
}

bool CArnoldDrawGeometry::Visible(StandinSelectionFilter filter) const
{
   if (!m_visible)
      return false;

   switch(filter)
   {
      default:
      case STANDIN_GEOM_ALL:
         return true;
      case STANDIN_GEOM_UNSELECTED:
         return !m_selected;
      case STANDIN_GEOM_SELECTED:
         return m_selected;
   }
}

bool CArnoldDrawGeometry::Invalid() const
{
   return m_invalid;
}

CArnoldDrawPolymesh::CArnoldDrawPolymesh(AtNode* node) : CArnoldDrawGeometry(node),
    m_polyTriangulator(NULL),
    mTriangulator(NULL)
{  
   AtArray* vlist = AiNodeGetArray(node, "vlist");  
   unsigned nelements = AiArrayGetNumElements(vlist);
   if ( nelements )
   {
      m_vlist.resize(nelements);
      for (unsigned i = 0; i < nelements; ++i)
      {
         AtVector pnt = AiArrayGetVec(vlist, i);
         if (!AiIsFinite(pnt.x) || !AiIsFinite(pnt.y) || !AiIsFinite(pnt.z))
            pnt = AI_V3_ZERO;
         m_BBMin.x = AiMin(m_BBMin.x, pnt.x);
         m_BBMin.y = AiMin(m_BBMin.y, pnt.y);
         m_BBMin.z = AiMin(m_BBMin.z, pnt.z);
         
         m_BBMax.x = AiMax(m_BBMax.x, pnt.x);
         m_BBMax.y = AiMax(m_BBMax.y, pnt.y);
         m_BBMax.z = AiMax(m_BBMax.z, pnt.z);
         
         m_vlist[i] = pnt;
      }
      unsigned nelements = AiArrayGetNumElements(vlist);
      uint8_t nkeys = AiArrayGetNumKeys(vlist);
      for (unsigned i = nelements; i < (nelements * nkeys); ++i)
      {
         AtVector pnt = AiArrayGetVec(vlist, i);
         if (!AiIsFinite(pnt.x) || !AiIsFinite(pnt.y) || !AiIsFinite(pnt.z))
            continue;
         m_BBMin.x = AiMin(m_BBMin.x, pnt.x);
         m_BBMin.y = AiMin(m_BBMin.y, pnt.y);
         m_BBMin.z = AiMin(m_BBMin.z, pnt.z);
         
         m_BBMax.x = AiMax(m_BBMax.x, pnt.x);
         m_BBMax.y = AiMax(m_BBMax.y, pnt.y);
         m_BBMax.z = AiMax(m_BBMax.z, pnt.z);
      }
   }
   else
   {
      m_invalid = true;
      return;
   }
   
   AtArray* vidxs = AiNodeGetArray(node, "vidxs");
   unsigned vidxElements = AiArrayGetNumElements(vidxs);
   if (vidxElements)
   {
      m_vidxs.resize(vidxElements);
      for (unsigned i = 0; i < vidxElements; ++i)
         m_vidxs[i] = AiArrayGetUInt(vidxs, i);
   }
   else
   {
      m_invalid = true;
      return;
   }

   AtArray* nsides = AiNodeGetArray(node, "nsides");
   unsigned nsidesElements = AiArrayGetNumElements(nsides);
   if (nsidesElements)
   {
      m_nsides.resize(nsidesElements);
      for (unsigned i = 0; i < nsidesElements; ++i)
         m_nsides[i] = AiArrayGetUInt(nsides, i);
   }
   else
   {
      m_invalid = true;
      return;
   }
   
   AtArray* nlist = AiNodeGetArray(node, "nlist");
   AtArray* nidxs = AiNodeGetArray(node, "nidxs");
   unsigned nlistElements = AiArrayGetNumElements(nlist);
   unsigned nidxsElements = AiArrayGetNumElements(nidxs);
   if (nlistElements > 0 && nidxsElements > 0)
   {
      m_nlist.resize(nlistElements);
      for (unsigned i = 0; i < nlistElements; ++i)
         m_nlist[i] = AiArrayGetVec(nlist, i);     
   
      m_nidxs.resize(nidxsElements);
         for (unsigned i = 0; i < nidxsElements; ++i)
            m_nidxs[i] = AiArrayGetUInt(nidxs, i);
   }
   else // generate normals
   {
      m_nlist.resize(m_nsides.size());
      m_nidxs.resize(m_vidxs.size());

      const size_t numPolygons = m_nsides.size();
      size_t id = 0;
      for (size_t p = 0; p < numPolygons; ++p)
      {
         const size_t ns = m_nsides[p];
         AtVector n = AI_V3_ZERO;
         for (size_t i = 0; i < (ns - 2); ++i)
         {
            const AtVector& v0 = m_vlist[m_vidxs[id]];
            const AtVector& v1 = m_vlist[m_vidxs[id + i]];
            const AtVector& v2 = m_vlist[m_vidxs[id + i + 1]];
            n += AiV3Cross(v1 - v0, v2 - v0);
         }
         n = AiV3Normalize(n);
         m_nlist[p] = n;
         for (size_t i = 0; i < ns; ++i)
            m_nidxs[id + i] = static_cast<unsigned int>(p);
         id += ns;
      }
   }
}

CArnoldDrawPolymesh::~CArnoldDrawPolymesh()
{
   if (mTriangulator)
       delete mTriangulator;

   if (m_polyTriangulator)
       delete m_polyTriangulator;
}

size_t CArnoldDrawPolymesh::WireIndexCount() const
{
    if ((m_vlist.size() == 0) || (m_vidxs.size() == 0))
        return 0;

    size_t total = 0;
    for (size_t i = 0; i < m_nsides.size(); ++i)
    {
        const unsigned int ns = m_nsides[i];
        total += ns*2;
    }
    return total;
}

void CArnoldDrawPolymesh::GetWireIndexing(unsigned int* outIndices, unsigned int vertexOffset, bool sharedVertices) const
{
    if ((m_vlist.size() == 0) || (m_vidxs.size() == 0))
        return;
    size_t newId = 0;
    // for each polygon
    for (size_t i = 0, id = 0; i < m_nsides.size(); ++i)
    {
        size_t startId = id;
        const unsigned int ns = m_nsides[i];

        // generate an edge for each point on the perimeter 
        for (unsigned int j = 0; j < ns-1; ++j, ++id)
        {
            outIndices[newId++] = m_vidxs[id] + vertexOffset;
            outIndices[newId++] = m_vidxs[id+1] + vertexOffset;
        }
        if (ns > 1)
        {
            // close the polygon
            outIndices[newId++] = m_vidxs[id] + vertexOffset;
            outIndices[newId++] = m_vidxs[startId] + vertexOffset;
        }
        ++id; // advance to the next polygon
    }
}

void CArnoldDrawPolymesh::createPolyTriangulator()
{
    if (m_polyTriangulator == NULL)
    {
        // Triangulate polygons
        m_polyTriangulator = new PolyTriangulator<unsigned int> (
            m_nsides.size(), m_nsides.data(), m_vidxs.data(), true, &(m_vlist.data()[0][0]), NULL);
        m_polyTriangulator->compute();
    }
}

PolyTriangulator<unsigned int>& CArnoldDrawPolymesh::polyTriangulator() const
{
    if (m_polyTriangulator == NULL)
    {
        // one time non-const access to do deferred initialization
        const_cast<CArnoldDrawPolymesh*>(this)->createPolyTriangulator();
    }

    return *m_polyTriangulator;
}

void CArnoldDrawPolymesh::createMultiStreamTriangulator()
{
    if (mTriangulator == NULL)
    {
        // Triangulate polygons
        std::vector<float> pts(m_vlist.size()*3);
        memcpy(&pts[0], &m_vlist[0][0], m_vlist.size()*3*sizeof(float));
        std::vector<float> norms(m_nlist.size()*3);
        memcpy(&norms[0], &m_nlist[0][0], m_nlist.size()*3*sizeof(float));

        mTriangulator = new Triangulator (pts, m_nsides, m_vidxs, &norms, &m_nidxs);
    }
}

Triangulator& CArnoldDrawPolymesh::triangulator() const
{
    if (mTriangulator == NULL)
    {
        // one time non-const access to do deferred initialization
        const_cast<CArnoldDrawPolymesh*>(this)->createMultiStreamTriangulator();
    }

    return *mTriangulator;
}

size_t CArnoldDrawPolymesh::TriangleIndexCount(bool sharedVertices) const
{
    if (sharedVertices)
        return triangulator().fTriangleIndices.size();
    else
        return polyTriangulator().numTriangles() * 3;       
}

void CArnoldDrawPolymesh::GetTriangleIndexing(unsigned int* outIndices, unsigned int vertexOffset, bool sharedVertices) const
{
    const std::vector<unsigned int>& indices = sharedVertices
        ? triangulator().fTriangleIndices 
        : polyTriangulator().triangleIndices();

    for (size_t i = 0, loopCount = TriangleIndexCount(sharedVertices); i < loopCount ; ++i)
        outIndices[i] = indices[i] + vertexOffset;
}

size_t CArnoldDrawPolymesh::PointCount() const { return m_vlist.size(); }

void CArnoldDrawPolymesh::GetPoints(float* vertices, const AtMatrix* matrix) const
{
    const AtMatrix *m = (matrix) ? matrix : &m_matrix;
    for (unsigned int i = 0; i < m_vlist.size(); ++i)
    {
        AtVector* to = reinterpret_cast<AtVector*>(&vertices[i*3]);
        *to = AiM4PointByMatrixMult( *m, m_vlist[i]);
    }
}


void CArnoldDrawPolymesh::GetSharedNormals(float* outNormals, const AtMatrix* matrix) const
{
    const AtMatrix *m = (matrix) ? matrix : &m_matrix;
    //transform each normal by the matrix.
    for (size_t i = 0, size = SharedVertexCount(); i < size; ++i)
    {
        AtVector* to = reinterpret_cast<AtVector*>(&outNormals[i*3]);
        AtVector* from = reinterpret_cast<AtVector*>(&triangulator().fMappedNormals[i*3]);
        *to = AiM4VectorByMatrixMult(*m, *from);
    }
}

void CArnoldDrawPolymesh::GetSharedVertices(float* outVertices, const AtMatrix* matrix) const
{
    const AtMatrix *m = (matrix) ? matrix : &m_matrix;
    // transform each vertex by the matrix.
    for (size_t i = 0, size = SharedVertexCount(); i < size; ++i)
    {
        AtVector* to = reinterpret_cast<AtVector*>(&outVertices[i*3]);
        AtVector* from = reinterpret_cast<AtVector*>(&triangulator().fMappedPositions[i*3]);
        *to = AiM4PointByMatrixMult( *m, *from);
    }    
}

size_t CArnoldDrawPolymesh::SharedVertexCount() const
{
    return triangulator().fMappedPositions.size() > 0 ? triangulator().fMappedPositions.size()/3 : 0;
}

CArnoldDrawPoints::CArnoldDrawPoints(AtNode* node) : CArnoldDrawGeometry(node)
{   
   AtArray* points = AiNodeGetArray(node, "points");
   unsigned nelements = (points != 0) ? AiArrayGetNumElements(points) : 0;
   if (nelements > 0)
   {
      m_points.resize(nelements);
      for (unsigned i = 0; i < nelements; ++i)
      {
         AtVector pnt = AiArrayGetVec(points, i);
         
         m_BBMin.x = AiMin(m_BBMin.x, pnt.x);
         m_BBMin.y = AiMin(m_BBMin.y, pnt.y);
         m_BBMin.z = AiMin(m_BBMin.z, pnt.z);
         
         m_BBMax.x = AiMax(m_BBMax.x, pnt.x);
         m_BBMax.y = AiMax(m_BBMax.y, pnt.y);
         m_BBMax.z = AiMax(m_BBMax.z, pnt.z);
         
         m_points[i] = pnt;
      }
   }
   else
      m_invalid = true;
}

CArnoldDrawPoints::~CArnoldDrawPoints()
{
   
}

size_t CArnoldDrawPoints::PointCount() const { return m_points.size(); }

void CArnoldDrawPoints::GetPoints(float* vertices, const AtMatrix* matrix) const
{
    const AtMatrix *m = (matrix) ? matrix : &m_matrix;
    for (unsigned int i = 0; i < m_points.size(); ++i)
    {
        AtVector* to = reinterpret_cast<AtVector*>(&vertices[i*3]);
        *to = AiM4PointByMatrixMult( *m, m_points[i]);
    }
}


CArnoldDrawBox::CArnoldDrawBox(AtNode* node) : CArnoldDrawGeometry(node)
{
   m_BBMin = AiNodeGetVec(node, "min");
   m_BBMax = AiNodeGetVec(node, "max");
}

CArnoldDrawBox::~CArnoldDrawBox()
{
   
}

/** Instance
 *
 **/
CArnoldDrawGInstance::CArnoldDrawGInstance(AtNode *node, const AtMatrix &m, bool inheritXForm) : 

                                          CArnoldDrawGeometry(node),
                                          m_matrix(m),
                                          m_inheritXForm(m_inheritXForm)
{
}

CArnoldDrawGInstance::~CArnoldDrawGInstance()
{
}


MBoundingBox CArnoldDrawGInstance::GetBBox(bool transformed) const
{
   if (!m_geom)
      return MBoundingBox();

   MBoundingBox bbox = m_geom->GetBBox(m_inheritXForm && transformed);
   if (transformed)
      bbox.transformUsing(AtMatrixToMMatrix(m_matrix));

   return bbox;
}

const AtMatrix& CArnoldDrawGInstance::GetMatrix() const
{
    return m_matrix;
}

void CArnoldDrawGInstance::GetPoints(float* points, const AtMatrix* matrix) const
{
    if (m_geom)
        m_geom->GetPoints(points, &m_matrix);
}
void CArnoldDrawGInstance::GetSharedVertices(float* outVertices, const AtMatrix* matrix) const
{
    if (m_geom)
        m_geom->GetSharedVertices(outVertices, &m_matrix);
}
void CArnoldDrawGInstance::GetSharedNormals(float* outNormals, const AtMatrix* matrix) const
{
    if (m_geom) 
        m_geom->GetSharedNormals(outNormals, &m_matrix);
}



/** Procedurals display
 *
 **/
CArnoldDrawProcedural::CArnoldDrawProcedural(AtNode* procNode, AtProcViewportMode mode) :
                       CArnoldDrawGeometry(procNode)
{
   AtUniverse *universe = AiUniverse();
   
   MDistance dist(1.0, MDistance::uiUnit());
   AiNodeSetFlt(AiUniverseGetOptions(AiNodeGetUniverse(procNode)), "meters_per_unit", dist.asMeters());

   AiProceduralViewport(procNode, universe, mode);
   AtNodeIterator* iter = AiUniverseGetNodeIterator(universe, AI_NODE_SHAPE);

   m_bbox.clear();

   static const AtString polymesh_str("polymesh");
   static const AtString points_str("points");
   static const AtString procedural_str("procedural");
   static const AtString box_str("box");
   static const AtString ginstance_str("ginstance");
   static const AtString node_str("node");

   std::vector<std::pair<CArnoldDrawGInstance *, std::string> > instances;
   while (!AiNodeIteratorFinished(iter))
   {
      AtNode* node = AiNodeIteratorGetNext(iter);
      if (node == nullptr)
         continue;

      MString nodeName = MString(AiNodeGetName(node));
      CArnoldDrawGeometry* g = 0;
      bool isInstance = false;
      if (AiNodeIs(node, polymesh_str))
         g = new CArnoldDrawPolymesh(node);
      else if (AiNodeIs(node, points_str))
         g = new CArnoldDrawPoints(node);
      else if(AiNodeIs(node, procedural_str))
         g = new CArnoldDrawProcedural(node, mode);
      else if(AiNodeIs(node, box_str))
         g = new CArnoldDrawBox(node);
      else if (AiNodeIs(node, ginstance_str))
      {
         AtNode *source = (AtNode*)AiNodeGetPtr(node, node_str);
         if (!source)
            continue;
         AtMatrix total_matrix = AiNodeGetMatrix(node, "matrix");
         bool inherit_xform = AiNodeGetBool(node, "inherit_xform");
         AtNode *instanceNode = (AtNode*)AiNodeGetPtr(node, "node");
         while(AiNodeIs(instanceNode, ginstance_str))
         {                  
            AtMatrix current_matrix = AiNodeGetMatrix(instanceNode, "matrix");
            if (inherit_xform)
               total_matrix = AiM4Mult(total_matrix, current_matrix);
            
            inherit_xform = AiNodeGetBool(instanceNode, "inherit_xform");
            instanceNode = (AtNode*)AiNodeGetPtr(instanceNode, "node");
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
         m_bbox.expand(g->GetBBox());  

      m_geometryList.insert(std::make_pair(std::string(AiNodeGetName(node)), g));
   }
   AiNodeIteratorDestroy(iter);
   
   for (auto instance : instances)
   {
      CArnoldDrawGInstance *g = instance.first;
      if (instance.first == nullptr)
         continue;
      
      geometryListIterType srcIter = m_geometryList.find(instance.second);
      if (srcIter == m_geometryList.end() || srcIter->second == nullptr)
         continue;

      g->SetGeometryNode(srcIter->second);
      if (g->Visible())
         m_bbox.expand(g->GetBBox());
   }
   m_BBMin = AtVector((float)m_bbox.min().x, (float)m_bbox.min().y, (float)m_bbox.min().z);
   m_BBMax = AtVector((float)m_bbox.max().x, (float)m_bbox.max().y, (float)m_bbox.max().z);
   AiUniverseDestroy(universe);
}

CArnoldDrawProcedural::~CArnoldDrawProcedural()
{
   
}

size_t CArnoldDrawProcedural::PointCount() const
{
   size_t count = 0;
   for (const auto &geom : m_geometryList)
   {
      count += geom.second->PointCount();
   }
   return count;
}
void CArnoldDrawProcedural::GetPoints(float* points, const AtMatrix* matrix) const
{
    const AtMatrix *procMatrix = (matrix) ? matrix : &m_matrix;
    AtMatrix m;
    int offset = 0;
    for (const auto &geom : m_geometryList)
    {   
        float* to = reinterpret_cast<float*>(&points[offset*3]);
        m = AiM4Mult(geom.second->GetMatrix(), *procMatrix);
        geom.second->GetPoints(to, &m);
        offset += geom.second->PointCount();
    }
}
size_t CArnoldDrawProcedural::SharedVertexCount() const
{
    size_t count = 0;
    for (const auto &geom : m_geometryList)
    {
        count += geom.second->SharedVertexCount();
    }
    return count;
}
void CArnoldDrawProcedural::GetSharedVertices(float* outVertices, const AtMatrix* matrix) const
{
    const AtMatrix *procMatrix = (matrix) ? matrix : &m_matrix;
    AtMatrix m;
    int offset = 0;
    for (const auto &geom : m_geometryList)
    {   
        float* to = reinterpret_cast<float*>(&outVertices[offset*3]);
        m = AiM4Mult(geom.second->GetMatrix(), *procMatrix);
        geom.second->GetSharedVertices(to, &m);
        offset += geom.second->SharedVertexCount();
    }
}
void CArnoldDrawProcedural::GetSharedNormals(float* outNormals, const AtMatrix* matrix) const
{
    const AtMatrix *procMatrix = (matrix) ? matrix : &m_matrix;
    AtMatrix m;
    int offset = 0;
    for (const auto &geom : m_geometryList)
    {   
        float* to = reinterpret_cast<float*>(&outNormals[offset*3]);
        m = AiM4Mult(geom.second->GetMatrix(), *procMatrix);
        geom.second->GetSharedNormals(to, &m);
        offset += geom.second->SharedVertexCount();
    }
}
size_t CArnoldDrawProcedural::WireIndexCount() const
{
    size_t count = 0;
    for (const auto &geom : m_geometryList)
    {
        count += geom.second->WireIndexCount();
    }
    return count;
}
void CArnoldDrawProcedural::GetWireIndexing(unsigned int* outIndices, unsigned int vertexOffset, bool sharedVertices) const
{
    int procOffset = 0;
    for (const auto &geom : m_geometryList)
    {
        geom.second->GetWireIndexing(outIndices + procOffset, vertexOffset, sharedVertices);
        procOffset += geom.second->WireIndexCount();

        if (sharedVertices) 
            vertexOffset += geom.second->SharedVertexCount();
        else 
            vertexOffset += geom.second->PointCount();
    }

}
size_t CArnoldDrawProcedural::TriangleIndexCount(bool sharedVertices) const
{
   size_t count = 0;
    for (const auto &geom : m_geometryList)
    {
        count += geom.second->TriangleIndexCount(sharedVertices);
    }
    return count;
}
void CArnoldDrawProcedural::GetTriangleIndexing(unsigned int* outIndices, unsigned int vertexOffset, bool sharedVertices) const
{
    int procOffset = 0;
    for (const auto &geom : m_geometryList)
    {
        geom.second->GetTriangleIndexing(outIndices + procOffset, vertexOffset, sharedVertices);
        procOffset += geom.second->TriangleIndexCount();

        if (sharedVertices) 
            vertexOffset += geom.second->SharedVertexCount();
        else 
            vertexOffset += geom.second->PointCount();
    }

}

