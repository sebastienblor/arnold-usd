//#pragma once
#ifndef _MTOA_POLYUTILS
#define _MTOA_POLYUTILS

#include <vector>
#include <algorithm>

#ifdef _LINUX
#define MAP_NEEDS_TR1 1
#endif

#ifdef _DARWIN
#if MAYA_API_VERSION < 201600
#define MAP_NEEDS_TR1 1
#endif
#endif



#ifdef MAP_NEEDS_TR1
#include <tr1/unordered_set>
#include <tr1/unordered_map>
#else
#include <unordered_set>
#include <unordered_map>
#endif

#include <maya/MFnMesh.h>
#include <maya/MFloatVector.h>
#include <string.h>
// This class is used for generating wireframe indices.
//
using namespace std;

template<class INDEX_TYPE>
class WireIndicesGenerator
{
public:
    typedef INDEX_TYPE index_type;

    WireIndicesGenerator(size_t numFaceCounts, const unsigned int* faceCounts,
        size_t numFaceIndices, const index_type* faceIndices,
        const index_type* mappedFaceIndices)
        : fNumFaceCounts(numFaceCounts), fFaceCounts(faceCounts),
        fNumFaceIndices(numFaceIndices), fFaceIndices(faceIndices),
        fMappedFaceIndices(mappedFaceIndices),
        fNumWires(0)
    {}

    void compute()
    {
        if (fNumFaceCounts == 0 || fNumFaceIndices == 0) {
            return;
        }

        // pre-allocate buffers for the worst case
        size_t maxNumWires = fNumFaceIndices;
#ifdef _LINUX
        typedef typename std::tr1::unordered_set<WirePair, typename WirePair::Hash, typename WirePair::EqualTo> WireSet;
#else
#ifdef MAP_NEEDS_TR1
        typedef typename std::tr1::unordered_set<WirePair, typename WirePair::Hash, typename WirePair::EqualTo> WireSet;
#else
        typedef typename std::unordered_set<WirePair, typename WirePair::Hash, typename WirePair::EqualTo> WireSet;
#endif
#endif

        WireSet wireSet(size_t(maxNumWires / 0.75f));

        // insert all wires to the set
        size_t polyIndex = 0;
        size_t endOfPoly = fFaceCounts[polyIndex];
        for (size_t i = 0; i < fNumFaceIndices; i++) {
            // find the two vertices of the wireframe
            // v1 and v2 (face indices before splitting vertices) are hashed to
            // remove duplicated wireframe lines.
            // mappedV1 and mappedV2 are the actual indices to remapped
            // positions/normals/UVs
            index_type v1, v2, mappedV1, mappedV2;
            v1       = fFaceIndices[i];
            mappedV1 = fMappedFaceIndices[i];

            size_t v2Index;
            if (i + 1 == endOfPoly) {
                // wrap at the end of the polygon
                v2Index = i + 1 - fFaceCounts[polyIndex];
                if (++polyIndex < fNumFaceCounts) {
                    endOfPoly += fFaceCounts[polyIndex];
                }
            }
            else {
                v2Index = i + 1;
            }

            v2       = fFaceIndices[v2Index];
            mappedV2 = fMappedFaceIndices[v2Index];

            // insert
            wireSet.insert(WirePair(v1, v2, mappedV1, mappedV2));
        }

        // the number of wireframes
        size_t numWires = wireSet.size();

        // allocate buffers for wireframe indices
        std::vector<index_type> wireIndices(numWires * 2);

        size_t wireCount = 0;
        for (typename WireSet::iterator it = wireSet.begin(); it != wireSet.end(); ++it, ++wireCount)
        {
            const WirePair& pair = (*it);
            wireIndices[wireCount * 2] = pair.fMappedV1;
            wireIndices[wireCount * 2 + 1] = pair.fMappedV2;
        }

        fNumWires    = numWires;
        fWireIndices = wireIndices;
    }

    size_t numWires()    { return fNumWires; }
    std::vector<index_type>& wireIndices() { return fWireIndices; }

private:
    // Prohibited and not implemented.
    WireIndicesGenerator(const WireIndicesGenerator&);
    const WireIndicesGenerator& operator= (const WireIndicesGenerator&);

    // This class represents an unordered pair for wireframe indices
    struct WirePair
    {
        WirePair(index_type v1, index_type v2, 
            index_type mappedV1, index_type mappedV2)
            : fV1(v1), fV2(v2), fMappedV1(mappedV1), fMappedV2(mappedV2)
        {}

        struct Hash : std::unary_function<WirePair, std::size_t>
        {
            std::size_t operator()(const WirePair& pair) const
            {
                std::size_t seed = 0;
                if (pair.fV1 < pair.fV2) {
                    seed = (seed ^ (pair.fV1 << 1));
                    seed = (seed ^ (pair.fV2 << 1));
                }
                else {
                    seed = (seed ^ (pair.fV1 << 1));
                    seed = (seed ^ (pair.fV1 << 1));
                }
                return seed;
            }
        };

        struct EqualTo : std::binary_function<WirePair, WirePair, bool>
        {
            bool operator()(const WirePair& x, const WirePair& y) const
            {
                if (x.fV1 < x.fV2) {
                    if (y.fV1 < y.fV2) {
                        return (x.fV1 == y.fV1 && x.fV2 == y.fV2);
                    }
                    else {
                        return (x.fV1 == y.fV2 && x.fV2 == y.fV1);
                    }
                }
                else {
                    if (y.fV1 < y.fV2) {
                        return (x.fV2 == y.fV1 && x.fV1 == y.fV2);
                    }
                    else {
                        return (x.fV2 == y.fV2 && x.fV1 == y.fV1);
                    }
                }
            }
        };

        index_type fV1, fV2;
        index_type fMappedV1, fMappedV2;
    };

    // Input
    size_t              fNumFaceCounts;
    const unsigned int* fFaceCounts;

    size_t              fNumFaceIndices;
    const index_type*   fFaceIndices;
    const index_type*   fMappedFaceIndices;

    // Output
    size_t                  fNumWires;
    std::vector<index_type> fWireIndices;
};


// This class converts multi-indexed streams to single-indexed streams.
//
template<class INDEX_TYPE, size_t MAX_NUM_STREAMS = 16>
class MultiIndexedStreamsConverter
{
public:
    typedef INDEX_TYPE index_type;

    MultiIndexedStreamsConverter(size_t numFaceIndices, const index_type* faceIndices)
        : fNumFaceIndices(numFaceIndices), fFaceIndices(faceIndices), fNumStreams(0),
        fNumVertices(0)
    {
        // position stream
        addMultiIndexedStream(faceIndices);
    }

    void addMultiIndexedStream(const index_type* indices)
    {
        // indices can be NULL, sequence 0,1,2,3,4,5... is assumed
        fStreams[fNumStreams++] = indices;
        //assert(fNumStreams <= MAX_NUM_STREAMS);
    }

    void compute()
    {
        // pre-allocate buffers for the worst case
        std::vector<index_type> indicesRegion(fNumStreams * fNumFaceIndices);

        // the hash map to find unique combination of multi-indices
#ifdef _LINUX
        typedef std::tr1::unordered_map<IndexTuple,size_t,typename IndexTuple::Hash,typename IndexTuple::EqualTo> IndicesMap;
#else
#ifdef MAP_NEEDS_TR1
        typedef std::tr1::unordered_map<IndexTuple,size_t,typename IndexTuple::Hash,typename IndexTuple::EqualTo> IndicesMap;
#else
        typedef std::unordered_map<IndexTuple,size_t,typename IndexTuple::Hash,typename IndexTuple::EqualTo> IndicesMap;
#endif
#endif

        IndicesMap indicesMap(size_t(fNumFaceIndices / 0.75f));

        // fill the hash map with multi-indices
        size_t vertexAttribIndex = 0;  // index to the remapped vertex attribs
        std::vector<index_type> mappedFaceIndices(fNumFaceIndices);

        for (size_t i = 0; i < fNumFaceIndices; i++) {
            // find the location in the region to copy multi-indices
            index_type* indices = &indicesRegion[i * fNumStreams];

            // make a tuple consists of indices for positions, normals, UVs..
            for (unsigned int j = 0; j < fNumStreams; j++) {
                indices[j] = fStreams[j] ? fStreams[j][i] : (index_type)i;
            }

            // try to insert the multi-indices tuple to the hash map
            IndexTuple tuple(indices, fNumStreams, (unsigned int)i);

            typename IndicesMap::iterator got = indicesMap.find(tuple);
            if (got == indicesMap.end())
            {
                size_t val = vertexAttribIndex++;
                indicesMap.insert(std::make_pair(tuple, val));
                mappedFaceIndices[i] = (index_type)val;
            }
            else
            {
                // remap face indices
                mappedFaceIndices[i] = (index_type)got->second;
            }
        }

        // the number of unique combination is the size of vertex attrib arrays
        size_t numVertex = vertexAttribIndex;
        //assert(vertexAttribIndex == indicesMap.size());

        // allocate memory for the indices into faceIndices
        std::vector<unsigned int> vertAttribsIndices(numVertex);

        // build the indices (how the new vertex maps to the poly vert)
        for (typename IndicesMap::iterator it = indicesMap.begin(); it != indicesMap.end(); ++it)
        {
            const typename IndicesMap::value_type& pair = (*it);
            vertAttribsIndices[pair.second] = pair.first.faceIndex();
        }

        fMappedFaceIndices  = mappedFaceIndices;
        fVertAttribsIndices = vertAttribsIndices;
        fNumVertices        = numVertex;
    }

    unsigned int                       numStreams()         { return fNumStreams; }
    size_t                             numVertices()        { return fNumVertices; }
    std::vector<unsigned int>& vertAttribsIndices() { return fVertAttribsIndices; }
    std::vector<index_type>&   mappedFaceIndices()  { return fMappedFaceIndices; }

private:
    // Prohibited and not implemented.
    MultiIndexedStreamsConverter(const MultiIndexedStreamsConverter&);
    const MultiIndexedStreamsConverter& operator= (const MultiIndexedStreamsConverter&);

    // This class represents a multi-index combination
    class IndexTuple
    {
    public:
        IndexTuple(index_type* indices, unsigned int size, unsigned int faceIndex)
            : fIndices(indices), fFaceIndex(faceIndex), fSize(size)
        {}

        const index_type& operator[](unsigned int index) const
        {
            //assert(index < fSize);
            return fIndices[index];
        }

        unsigned int faceIndex() const 
        {
            return fFaceIndex;
        }

        struct Hash : std::unary_function<IndexTuple, std::size_t>
        {
            std::size_t operator()(const IndexTuple& tuple) const
            {
                std::size_t seed = 0;
                for (unsigned int i = 0; i < tuple.fSize; i++) {
                    seed = (seed ^ (tuple.fIndices[i] << 1));
                    //boost::hash_combine(seed, tuple.fIndices[i]);
                }
                return seed;
            }
        };

        struct EqualTo : std::binary_function<IndexTuple, IndexTuple, bool>
        {
            bool operator()(const IndexTuple& x, const IndexTuple& y) const
            {
                if (x.fSize == y.fSize) {
                    return memcmp(x.fIndices, y.fIndices, sizeof(index_type) * x.fSize) == 0;
                }
                return false;
            }
        };

    private:
        index_type*  fIndices;
        unsigned int fFaceIndex;
        unsigned int fSize;
    };

    // Input
    size_t            fNumFaceIndices;       
    const index_type* fFaceIndices;

    const index_type* fStreams[MAX_NUM_STREAMS];
    unsigned int      fNumStreams;

    // Output
    size_t                    fNumVertices;
    std::vector<unsigned int> fVertAttribsIndices;
    std::vector<index_type>   fMappedFaceIndices;
};

// This class drops indices for a vertex attrib
template<class INDEX_TYPE, size_t SIZE>
class IndicesDropper
{
public:
    typedef INDEX_TYPE index_type;

    IndicesDropper(const float* attribArray, const index_type* indexArray, size_t numVerts)
    {
        // map the indexed array to direct array
        std::vector<float> mappedAttribs(numVerts * SIZE);
        for (size_t i = 0; i < numVerts; i++) {
            for (size_t j = 0; j < SIZE; j++) {
                mappedAttribs[i * SIZE + j] = attribArray[indexArray[i] * SIZE + j];
            }
        }

        fMappedAttribs = mappedAttribs;
    }

    std::vector<float>& mappedAttribs() { return fMappedAttribs; }

private:
    // Prohibited and not implemented.
    IndicesDropper(const IndicesDropper&);
    const IndicesDropper& operator= (const IndicesDropper&);

    std::vector<float> fMappedAttribs;
};


// This class remaps multi-indexed vertex attribs (drop indices).
//
template<class INDEX_TYPE, size_t MAX_NUM_STREAMS = 16>
class MultiIndexedStreamsRemapper
{
public:
    typedef INDEX_TYPE index_type;

    MultiIndexedStreamsRemapper(const index_type* faceIndices,
        size_t numNewVertices, const unsigned int* vertAttribsIndices)
        : fFaceIndices(faceIndices), fNumNewVertices(numNewVertices),
        fVertAttribsIndices(vertAttribsIndices), fNumStreams(0)
    {}

    void addMultiIndexedStream(const float* attribs, const index_type* indices, bool faceVarying, int stride)
    {
        fAttribs[fNumStreams]     = attribs;
        fIndices[fNumStreams]     = indices;
        fFaceVarying[fNumStreams] = faceVarying;
        fStride[fNumStreams]      = stride;
        fNumStreams++;
    }

    void compute()
    {
        // remap vertex attribs
        for (unsigned int i = 0; i < fNumStreams; i++) {
            const float*      attribs     = fAttribs[i];
            const index_type* indices     = fIndices[i];
            bool              faceVarying = fFaceVarying[i];
            int               stride      = fStride[i];

            // allocate memory for remapped vertex attrib arrays
            std::vector<float> mappedVertAttrib(fNumNewVertices * stride);

            for (size_t j = 0; j < fNumNewVertices; j++) {
                // new j-th vertices maps to polyVertIndex-th poly vert
                unsigned int polyVertIndex = fVertAttribsIndices[j];

                // if the scope is varying/vertex, need to convert poly vert
                // index to vertex index
                index_type pointOrPolyVertIndex = faceVarying ?
polyVertIndex : fFaceIndices[polyVertIndex];

                // look up the vertex attrib index
                index_type attribIndex = indices ?
                    indices[pointOrPolyVertIndex] : pointOrPolyVertIndex;

                if (stride == 3) {
                    mappedVertAttrib[j * 3 + 0] = attribs[attribIndex * 3 + 0];
                    mappedVertAttrib[j * 3 + 1] = attribs[attribIndex * 3 + 1];
                    mappedVertAttrib[j * 3 + 2] = attribs[attribIndex * 3 + 2];
                }
                else if (stride == 2) {
                    mappedVertAttrib[j * 2 + 0] = attribs[attribIndex * 2 + 0];
                    mappedVertAttrib[j * 2 + 1] = attribs[attribIndex * 2 + 1];
                }
                else {
                    //assert(0);
                }
            }

            fMappedVertAttribs[i] = mappedVertAttrib;
        }
    }

    std::vector<float>& mappedVertAttribs(unsigned int index)
    {
        //assert(index < fNumStreams);
        return fMappedVertAttribs[index];
    }

private:
    // Prohibited and not implemented.
    MultiIndexedStreamsRemapper(const MultiIndexedStreamsRemapper&);
    const MultiIndexedStreamsRemapper& operator= (const MultiIndexedStreamsRemapper&);

    // Input
    const index_type*   fFaceIndices;
    size_t              fNumNewVertices;
    const unsigned int* fVertAttribsIndices;

    const float*      fAttribs[MAX_NUM_STREAMS];
    const index_type* fIndices[MAX_NUM_STREAMS];
    bool              fFaceVarying[MAX_NUM_STREAMS];
    int               fStride[MAX_NUM_STREAMS];
    unsigned int      fNumStreams;

    // Output, NULL means no change
    std::vector<float> fMappedVertAttribs[MAX_NUM_STREAMS];
};


// This class triangulates polygons.
//
template<class INDEX_TYPE>
class PolyTriangulator
{
public:
    typedef INDEX_TYPE index_type;

    PolyTriangulator(size_t numFaceCounts, const unsigned int* faceCounts,
        const index_type* faceIndices, bool faceIndicesCW,
        const float* positions, const float* normals)
        : fNumFaceCounts(numFaceCounts), fFaceCounts(faceCounts),
        fFaceIndices(faceIndices), fFaceIndicesCW(faceIndicesCW),
        fPositions(positions), fNormals(normals),
        fNumTriangles(0)
    {}

    void compute()
    {
        // empty mesh
        if (fNumFaceCounts == 0) {
            return;
        }

        // scan the polygons to estimate the buffer size
        size_t maxPoints      = 0;  // the max number of vertices in one polygon
        size_t totalTriangles = 0;  // the number of triangles in the mesh

        for (size_t i = 0; i < fNumFaceCounts; i++) {
            size_t numPoints = fFaceCounts[i];

            // ignore degenerate polygon
            if (numPoints < 3) {
                continue;
            }

            // max number of points in a polygon
            maxPoints = max(numPoints, maxPoints);

            // the number of triangles expected in the polygon
            size_t numTriangles = numPoints - 2;
            totalTriangles += numTriangles;
        }

        size_t maxTriangles = maxPoints - 2;  // the max number of triangles in a polygon

        // allocate buffers for the worst case
        std::vector<index_type>     indices(maxPoints);
        std::vector<unsigned short> triangles(maxTriangles * 3);
        std::vector<float>          aPosition(maxPoints * 2);
        std::vector<float>          aNormal;
        std::vector<index_type>     normalIds;
        if (fNormals) {
            aNormal.resize(maxPoints * 3);
        }

        std::vector<index_type> triangleIndices(totalTriangles * 3);

        // triangulate each polygon
        size_t triangleCount  = 0;  // the triangles
        for (size_t i = 0, polyVertOffset = 0; i < fNumFaceCounts; polyVertOffset += fFaceCounts[i], i++) {
            size_t numPoints = fFaceCounts[i];

            // ignore degenerate polygon
            if (numPoints < 3) {
                continue;
            }

            // no need to triangulate a triangle
            if (numPoints == 3) {
                if (fFaceIndicesCW) {
                    triangleIndices[triangleCount * 3 + 0] = fFaceIndices[polyVertOffset + 2];
                    triangleIndices[triangleCount * 3 + 1] = fFaceIndices[polyVertOffset + 1];
                    triangleIndices[triangleCount * 3 + 2] = fFaceIndices[polyVertOffset + 0];
                }
                else {
                    triangleIndices[triangleCount * 3 + 0] = fFaceIndices[polyVertOffset + 0];
                    triangleIndices[triangleCount * 3 + 1] = fFaceIndices[polyVertOffset + 1];
                    triangleIndices[triangleCount * 3 + 2] = fFaceIndices[polyVertOffset + 2];
                }
                triangleCount++;
                continue;
            }

            // 1) correct the polygon winding from CW to CCW
            if (fFaceIndicesCW)
            {
                for (size_t j = 0; j < numPoints; j++) {
                    size_t jCCW = numPoints - j - 1;
                    indices[j] = fFaceIndices[polyVertOffset + jCCW];
                }
            }
            else {
                for (size_t j = 0; j < numPoints; j++) {
                    indices[j] = fFaceIndices[polyVertOffset + j];
                }
            }

            // 2) compute the face normal (Newell's Method)
            MFloatVector faceNormal(0.0f, 0.0f, 0.0f);
            for (size_t j = 0; j < numPoints; j++) {
                const float* thisPoint = &fPositions[indices[j] * 3];
                const float* nextPoint = &fPositions[indices[(j + numPoints - 1) % numPoints] * 3];
                faceNormal.x += (thisPoint[1] - nextPoint[1]) * (thisPoint[2] + nextPoint[2]);
                faceNormal.y += (thisPoint[2] - nextPoint[2]) * (thisPoint[0] + nextPoint[0]);
                faceNormal.z += (thisPoint[0] - nextPoint[0]) * (thisPoint[1] + nextPoint[1]);
            }
            faceNormal.normalize();

            // 3) project the vertices to 2d plane by faceNormal
            float cosa, sina, cosb, sinb, cacb, sacb;
            sinb = -sqrtf(faceNormal[0] * faceNormal[0] + faceNormal[1] * faceNormal[1]);
            if (sinb < -1e-5) {
                cosb =  faceNormal[2];
                sina =  faceNormal[1] / sinb;
                cosa = -faceNormal[0] / sinb;

                cacb = cosa * cosb;
                sacb = sina * cosb;
            }
            else {
                cacb = 1.0f;
                sacb = 0.0f;
                sinb = 0.0f;
                sina = 0.0f;
                if (faceNormal[2] > 0.0f) {
                    cosa = 1.0f;
                    cosb = 1.0f;
                }
                else {
                    cosa = -1.0f;
                    cosb = -1.0f;
                }
            }

            for (size_t j = 0; j < numPoints; j++) {
                const float* point = &fPositions[indices[j] * 3];
                aPosition[j * 2 + 0] = cacb * point[0] - sacb * point[1] + sinb * point[2];
                aPosition[j * 2 + 1] = sina * point[0] + cosa * point[1];
            }

            // 4) copy normals
            MFloatVector averageNormal;
            if (fNormals) {
                for (size_t j = 0; j < numPoints; j++) {
                    averageNormal[0] += fNormals[indices[j] * 3 + 0];
                    averageNormal[1] += fNormals[indices[j] * 3 + 1];
                    averageNormal[2] += fNormals[indices[j] * 3 + 2];
                    aNormal[j * 3 + 0] = fNormals[indices[j] * 3 + 0];
                    aNormal[j * 3 + 1] = fNormals[indices[j] * 3 + 1];
                    aNormal[j * 3 + 2] = fNormals[indices[j] * 3 + 2];
                }
            }
            averageNormal.normalize();

            // 5) do triangulation
            int numResultTriangles = 0;
            MFnMesh::polyTriangulate(
                aPosition.data(),
                (unsigned int)numPoints,
                (unsigned int)numPoints,
                0,
                fNormals != NULL,
                aNormal.data(),
                triangles.data(),
                numResultTriangles);

            if (numResultTriangles == int(numPoints - 2)) {
                // triangulation success
                for (size_t j = 0; j < size_t(numResultTriangles); j++, triangleCount++) {
                    triangleIndices[triangleCount * 3 + 0] = indices[triangles[j * 3 + 0]];
                    triangleIndices[triangleCount * 3 + 1] = indices[triangles[j * 3 + 1]];
                    triangleIndices[triangleCount * 3 + 2] = indices[triangles[j * 3 + 2]];
                }
            }
            else {
                // triangulation failure, use the default triangulation
                for (size_t j = 1; j < numPoints - 1; j++, triangleCount++) {
                    triangleIndices[triangleCount * 3 + 0] = indices[0];
                    triangleIndices[triangleCount * 3 + 1] = indices[j];
                    triangleIndices[triangleCount * 3 + 2] = indices[j + 1];
                }
            }
        }

        fNumTriangles    = totalTriangles;
        fTriangleIndices = triangleIndices;
    }

    size_t numTriangles() { return fNumTriangles; }
    std::vector<index_type>& triangleIndices() { return fTriangleIndices; }

private:
    // Prohibited and not implemented.
    PolyTriangulator(const PolyTriangulator&);
    const PolyTriangulator& operator= (const PolyTriangulator&);

    // Input
    size_t              fNumFaceCounts;
    const unsigned int* fFaceCounts;
    const index_type*   fFaceIndices;
    bool                fFaceIndicesCW;
    const float*        fPositions;
    const float*        fNormals;

    // Output
    size_t                  fNumTriangles;
    std::vector<index_type> fTriangleIndices;
};


#endif //_MTOA_POLYTRIANGULATOR
