//#pragma once
#ifndef _MTOA_POLYTRIANGULATOR
#define _MTOA_POLYTRIANGULATOR

#include <vector>

#include <maya/MFnMesh.h>
#include <utils/PolyUtils.h>

//==============================================================================
// CLASS Triangulator
//==============================================================================

// This class reads mesh data that is written by an arbitrary Alembic exporter.
// Triangulate a polygon mesh and convert multi-indexed streams to single-indexed streams.
//
class Triangulator
{
public:
    Triangulator(
        const std::vector<float>& positions, 
        const std::vector<unsigned int>& faceCounts, 
        const std::vector<unsigned int>& faceIndices,
        std::vector<float>* normals,
        std::vector<unsigned int>* normalIndices);

    ~Triangulator();

    void initialize();

    // compute in remapVertAttribs()
    std::vector<float>          fMappedPositions;
    std::vector<float>          fMappedNormals;
        
    // compute in computeWireIndices()
    std::vector<unsigned int>   fWireIndices;

    // compute in triangulate()
    std::vector<unsigned int>   fTriangleIndices;  

protected:
    // Prohibited and not implemented.
    Triangulator(const Triangulator&);
    const Triangulator& operator= (const Triangulator&);

    // Polygon Indices
    const std::vector<unsigned int>& fFaceCounts;
    const std::vector<unsigned int>& fFaceIndices;
    
    // Positions
    const std::vector<float>& fPositions;

    // Normals
    std::vector<float>* fNormals;
    std::vector<unsigned int>* fNormalIndices;
        
private:
    void convertMultiIndexedStreams();
    void remapVertAttribs();
    void computeWireIndices();
    void triangulate();

    template<size_t SIZE>
    void convertMultiIndexedStream(
        const std::vector<float>& attribArray,
        const std::vector<unsigned int>& indexArray,
        std::vector<float>& outStream)
    {
        // map the indexed array to direct array
        size_t numVerts = indexArray.size();
        outStream.resize(numVerts * SIZE);
        for (size_t i = 0; i < numVerts; i++) {
            for (size_t j = 0; j < SIZE; j++) {
                outStream[i * SIZE + j] = attribArray[indexArray[i] * SIZE + j];
            }
        }
    }

    // compute in convertMultiIndexedStreams()
    size_t                      fNumVertices;
    std::vector<unsigned int>   fVertAttribsIndices;
    std::vector<unsigned int>   fMappedFaceIndices;

    // Empty normals
    std::vector<float>          fNoNormals;
    std::vector<unsigned int>   fNoNormalIndices;
};

#endif //_MTOA_POLYTRIANGULATOR
