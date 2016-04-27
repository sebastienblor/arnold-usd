#include "PolyTriangulator.h"

Triangulator::Triangulator(
    const std::vector<float>& positions, 
    const std::vector<unsigned int>& faceCounts, 
    const std::vector<unsigned int>& faceIndices,
    std::vector<float>* normals,
    std::vector<unsigned int>* normalIndices) :
    fPositions(positions)
    , fFaceCounts(faceCounts)
    , fFaceIndices(faceIndices)
    , fNormals(normals)
    , fNormalIndices(normalIndices)
{
    initialize();
}

Triangulator::~Triangulator()
{
}

void Triangulator::initialize()
{
    // convert the poly mesh to a display friendly mesh.
    if (!fNormals)
    {
        // Ignore normals. 
        fNormals = &fNoNormals;
        fNormalIndices = & fNoNormalIndices;
    }

    convertMultiIndexedStreams();
    remapVertAttribs();
    //computeWireIndices();
    triangulate();
}

void Triangulator::convertMultiIndexedStreams()
{
    // convert multi-indexed streams to single-indexed streams
    // assume the scope is face varying
    //

    // input polygons data
    size_t numFaceIndices = fFaceIndices.size();
    const unsigned int* faceIndices = fFaceIndices.data();

    // input normals
    bool normalFaceVarying = true;
    const unsigned int* normalIndices = NULL;
    if (fNormals->size() > 0) {
        if (fNormalIndices->size() > 0) {
            normalIndices = fNormalIndices->data();
            //assert(fNormalIndices->size() == numFaceIndices);
        }
    }

    // determine the number of multi-indexed streams
    MultiIndexedStreamsConverter<unsigned int> converter(
        numFaceIndices, faceIndices);

    if (normalFaceVarying) {
        converter.addMultiIndexedStream(normalIndices);
    }

    // only one multi-indexed streams, no need to convert it
    if (converter.numStreams() == 1) {
        fVertAttribsIndices.clear();
        fMappedFaceIndices = fFaceIndices;
        fNumVertices       = fPositions.size() / 3;
        return;
    }

    // convert the multi-indexed streams
    converter.compute();

    // the mapped face indices
    fMappedFaceIndices = converter.mappedFaceIndices();

    // indices to remap streams
    fVertAttribsIndices = converter.vertAttribsIndices();
    fNumVertices        = converter.numVertices();
}

void Triangulator::remapVertAttribs()
{
    // remap vertex attribute streams according to the result of convertMultiIndexedStreams()

    // no multi-index streams, just drop indices
    if (fVertAttribsIndices.size() == 0) {
        // positions is the only stream, just use it
        fMappedPositions = fPositions;

        // get rid of normal indices
        if (fNormals->size() > 0) {
            if (fNormalIndices->size() > 0) {
                convertMultiIndexedStream<3>(*fNormals, *fNormalIndices, fMappedNormals);
            }
            else {
                fMappedNormals = *fNormals;
            }
        }

        return;
    }

    // input polygons data
    const float*                positions   = fPositions.data();
    const unsigned int*         faceIndices = fFaceIndices.data();

    // input normals
    const float*                normals       = NULL;
    const unsigned int*         normalIndices = NULL;
    if (fNormals->size() > 0) {
        normals = fNormals->data();
        if (fNormalIndices->size() > 0) {
            normalIndices = fNormalIndices->data();
        }
    }

    // set up multi-indexed streams remapper
    MultiIndexedStreamsRemapper<unsigned int> remapper(
        faceIndices, fNumVertices, fVertAttribsIndices.data());

    remapper.addMultiIndexedStream(positions, NULL, false, 3);

    if (normals) {
        remapper.addMultiIndexedStream(normals, normalIndices, true, 3);
    }

    // remap streams
    remapper.compute();

    fMappedPositions = remapper.mappedVertAttribs(0);

    unsigned int streamIndex = 1;
    if (normals) {
        fMappedNormals = remapper.mappedVertAttribs(streamIndex++);
    }
}

void Triangulator::computeWireIndices()
{
    // compute the wireframe indices
    //

    // input data
    size_t           numFaceCounts = fFaceCounts.size();
    const unsigned int* faceCounts = fFaceCounts.data();

    size_t                   numFaceIndices = fFaceIndices.size();
    const unsigned int* faceIndices = fFaceIndices.data();
    const unsigned int* mappedFaceIndices = fMappedFaceIndices.data();

    // Compute wireframe indices
    WireIndicesGenerator<unsigned int> wireIndicesGenerator(
        numFaceCounts, faceCounts, numFaceIndices, faceIndices, mappedFaceIndices);
    wireIndicesGenerator.compute();

    if (wireIndicesGenerator.numWires() == 0) {
        fWireIndices.clear();
        return;
    }

    fWireIndices = wireIndicesGenerator.wireIndices();
}

void Triangulator::triangulate()
{
    // triangulate the polygons
    // assume that there are no holes
    //

    // input data
    size_t           numFaceCounts = fFaceCounts.size();
    const unsigned int* faceCounts = fFaceCounts.data();

    const unsigned int* faceIndices = fMappedFaceIndices.data();

    const float* positions = fMappedPositions.data();
    const float* normals   = fMappedNormals.size() > 0 ? fMappedNormals.data() : NULL;

    if (numFaceCounts == 0) {
        fTriangleIndices.clear();
        return;
    }

    // Triangulate polygons
    PolyTriangulator<unsigned int> polyTriangulator(
        numFaceCounts, faceCounts, faceIndices, true, positions, normals);

    polyTriangulator.compute();

    fTriangleIndices = polyTriangulator.triangleIndices();
}