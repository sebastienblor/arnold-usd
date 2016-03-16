#include "ArnoldStandInSubSceneOverride.h"
#include "nodes/shape/ArnoldStandIns.h"
#include "nodes/options/ArnoldOptionsNode.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MNodeMessage.h>
#include <maya/MBoundingBox.h>
#include <maya/MPlugArray.h>

#include <iostream>

#include <ai.h>

#include "utils/MayaUtils.h"
#include "ViewportUtils.h"

const MString colorParameterName_       = "solidColor";
const MString diffuseColorParameterName_ = "diffuseColor";
const MString wireframecubeItemName_    = "cubeLocatorWires";
const MString pointCloudItemName_       = "pointCloud";
const MString perObjectBoxItemName_     = "perObjectBox";
const MString wireframeItemName_        = "wireframe";
const MString polyItemName_             = "polygons";
const MString shadedPolyItemName_       = "shadedPolygons";
const MString deferStandInItemName_     = "deferredBox";

static const float cube[][3] = { {0.0f, 0.0f, 0.0f},
                     {1.0f, 0.0f, 0.0f},
                     {0.0f, 0.0f, 0.0f},
                     {0.0f, 1.0f, 0.0f},
                     {1.0f, 0.0f, 0.0f},
                     {1.0f, 1.0f, 0.0f},
                     {0.0f, 1.0f, 0.0f},
                     {1.0f, 1.0f, 0.0f},
                     
                     {0.0f, 0.0f, 1.0f},
                     {1.0f, 0.0f, 1.0f},
                     {0.0f, 0.0f, 1.0f},
                     {0.0f, 1.0f, 1.0f},
                     {1.0f, 0.0f, 1.0f},
                     {1.0f, 1.0f, 1.0f},
                     {0.0f, 1.0f, 1.0f},
                     {1.0f, 1.0f, 1.0f},
                     
                     {0.0f, 0.0f, 0.0f},
                     {0.0f, 0.0f, 1.0f},
                     {1.0f, 0.0f, 0.0f},
                     {1.0f, 0.0f, 1.0f},
                     {0.0f, 1.0f, 0.0f},
                     {0.0f, 1.0f, 1.0f},
                     {1.0f, 1.0f, 0.0f},
                     {1.0f, 1.0f, 1.0f},};
                     

static const int kCubeCount = 24;

namespace {
    void standInAttributeChanged(MNodeMessage::AttributeMessage /*msg*/, MPlug& /*plug*/, MPlug& /*otherPlug*/, void *clientData)
    {
        static_cast<CArnoldStandInSubSceneOverride*>(clientData)->invalidate();
    }

    void globalOptionsChanged(MNodeMessage::AttributeMessage /*msg*/, MPlug& plug, MPlug& /*otherPlug*/, void *clientData)
    {
        // invalidate the draw data if the global draw override changes.
        if (plug.attribute() == CArnoldOptionsNode::s_enable_standin_draw)
            static_cast<CArnoldStandInSubSceneOverride*>(clientData)->invalidate();
    }

    // Helper class for link lost callback
    class StandInShadedItemUserData : public MUserData
    {
    public:
        StandInShadedItemUserData(CArnoldStandInSubSceneOverride* ov)
            : MUserData(true), fOverride(ov) {}
        ~StandInShadedItemUserData() { fOverride = NULL; }

        CArnoldStandInSubSceneOverride* fOverride;
    };

    void standInShadedItemLinkLost(MHWRender::MShaderInstance* /*shaderInstance*/, MUserData* userData)
    {
        StandInShadedItemUserData* data = dynamic_cast<StandInShadedItemUserData*>(userData);
        if (data && data->fOverride)
            data->fOverride->releaseShadedMaterial();
    }
}

CArnoldStandInSubSceneOverride::CArnoldStandInSubSceneOverride(const MObject& obj)
: MHWRender::MPxSubSceneOverride(obj)
, mSolidUIShader(NULL)
, mSolidShader(NULL)
, mBlinnShader(NULL)
, mShaderFromNode(NULL)
, mLocatorNode(obj)
, mBBChanged(true)
, mOneTimeUpdate(true)
, mAttribChangedID(0)
{
    MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
    if (!renderer)
        return;

    const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
    if (!shaderMgr)
        return;

	mSolidUIShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidShader);
    mSolidShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidShader);
    mBlinnShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dBlinnShader);

    float  solidColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f};
    mSolidShader->setParameter(colorParameterName_, solidColor);
    mBlinnShader->setParameter(diffuseColorParameterName_, solidColor);

    mAttribChangedID = MNodeMessage::addAttributeChangedCallback(mLocatorNode, standInAttributeChanged, this);

    // TODO: Need to do something different if the option node does not yet exist. (like create it)
    MObject arnoldRenderOptionsNode = CArnoldOptionsNode::getOptionsNode();
    if (!arnoldRenderOptionsNode.isNull())
        mGlobalOptionsChangedID = MNodeMessage::addAttributeChangedCallback(arnoldRenderOptionsNode, globalOptionsChanged, this);
}

CArnoldStandInSubSceneOverride::~CArnoldStandInSubSceneOverride()
{
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (renderer)
	{
    	const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
    	if (shaderMgr)
    	{
            if(mSolidUIShader)
                shaderMgr->releaseShader(mSolidUIShader);
            if(mSolidShader)
                shaderMgr->releaseShader(mSolidShader);
            if(mBlinnShader)
                shaderMgr->releaseShader(mBlinnShader);
            if(mShaderFromNode)
                shaderMgr->releaseShader(mShaderFromNode);
		}
    }
    mSolidUIShader = NULL;
    mSolidShader = NULL;
    mBlinnShader = NULL;
    mShaderFromNode = NULL;

    MNodeMessage::removeCallback(mAttribChangedID);
    if (mGlobalOptionsChangedID != 0)
        MNodeMessage::removeCallback(mGlobalOptionsChangedID);
}

MHWRender::DrawAPI CArnoldStandInSubSceneOverride::supportedDrawAPIs() const
{
	// this plugin supports both GL and DX
	return (MHWRender::kAllDevices);
}

bool CArnoldStandInSubSceneOverride::requiresUpdate(
    const MHWRender::MSubSceneContainer& container,
    const MHWRender::MFrameContext& frameContext) const
{
    if (!container.count())
        return true;

    // set the shader
    if (mSolidUIShader)
    {
        MStatus status;
        MFnDagNode node(mLocatorNode, &status);
        if (!status) return false;

        // get the wireframe color for this node 
        MDagPath path;
        node.getPath(path);
        MColor color = MHWRender::MGeometryUtilities::wireframeColor(path);

        float  wireframeColor[4] = { color.r, color.g, color.b, 1.0f };
        mSolidUIShader->setParameter(colorParameterName_, wireframeColor);
    }

    // Update when something has changed
    return mBBChanged || mOneTimeUpdate;
}

void CArnoldStandInSubSceneOverride::releaseShadedMaterial()
{
    MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
    if (renderer)
    {
        const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
        if (shaderMgr && mShaderFromNode)
            shaderMgr->releaseShader(mShaderFromNode);
    }
    mShaderFromNode = NULL;
    mOneTimeUpdate = true;
}

MHWRender::MRenderItem* CArnoldStandInSubSceneOverride::getItem(
    MHWRender::MSubSceneContainer& container,
    const MString& name, 
    MHWRender::MGeometry::Primitive primitiveType,
    unsigned int depthPriority,
    MHWRender::MGeometry::DrawMode drawMode)
{
    // Find or create the item if needed
    MHWRender::MRenderItem* item = container.find(name);
    if (!item)
    {
        item = MHWRender::MRenderItem::Create(
            name,
            MHWRender::MRenderItem::NonMaterialSceneItem,
            primitiveType);
        item->setDrawMode(drawMode);
        item->depthPriority(depthPriority);
        container.add(item);
    }

    return item;
}

void CArnoldStandInSubSceneOverride::update(
    MHWRender::MSubSceneContainer& container,
    const MHWRender::MFrameContext& frameContext)
{
    // initialize
    mOneTimeUpdate = false;
    container.clear();

    // get data to use later.
    MStatus status;
    MFnDagNode node(mLocatorNode, &status);
    CArnoldStandInShape* standIn = static_cast<CArnoldStandInShape*>(node.userNode());
    CArnoldStandInGeom* geom = standIn->geometry();

    // get the drawOverride to determine if we enable display of standins
    int drawOverride = getDrawOverride();

    // get the draw mode
    int mode = geom->mode;
    if (drawOverride == 1 || drawOverride == 3)
        mode = 0; // bounding box
    else if (drawOverride == 2)
        mode = -1; // disable draw

    // check the mode and process accordingly
    bool wantBox(mode==0);
    bool wantBoxes(mode==1);
    bool wantSolid(mode==2);
    bool wantWireframe(mode==2||mode==3||mode==5);
    bool wantPoints(mode==4);
    bool wantShaded(mode==5||mode==6);

    if (wantShaded)
    {
        size_t sharedVertexCount = geom->SharedVertexCount();
        if (sharedVertexCount > 0)
        {
            // TODO: There are currently problems with flipped normals or negatively scaled geometry with the shader from node for some reason.
            if (!mShaderFromNode)
                updateShaderFromNode();

            MHWRender::MRenderItem* item = getItem(container, shadedPolyItemName_, MHWRender::MGeometry::kTriangles, 
                MHWRender::MRenderItem::sDormantFilledDepthPriority);
            updateRenderItem(container, geom, item, sharedVertexCount, (mShaderFromNode ? mShaderFromNode : mBlinnShader), true);
        }
        else // no normals, try solid
            wantSolid = true;
    }

    size_t totalPoints = geom->PointCount();
    if (wantSolid)
    {
        if (geom->TriangleIndexCount())
        {
	        MHWRender::MRenderItem* item = getItem(container, polyItemName_, MHWRender::MGeometry::kTriangles, 
	            MHWRender::MRenderItem::sDormantFilledDepthPriority);
	        updateRenderItem(container, geom, item, totalPoints, mSolidShader);
        } 
        else // no triangles, try wireframe
            wantWireframe = true;
    }

    if (wantWireframe)
    {
        if (geom->WireIndexCount() > 0)
        {
            MHWRender::MRenderItem* item = getItem(container, wireframeItemName_, MHWRender::MGeometry::kLines, 
                MHWRender::MRenderItem::sActiveLineDepthPriority);
            updateRenderItem(container, geom, item, totalPoints, mSolidUIShader, false);
        }
        else // no wires, try points.
            wantPoints = true;
    }

    if (wantPoints)
    {
        if (geom->PointCount() > 0)
        {
	        MHWRender::MRenderItem* item = getItem(container, pointCloudItemName_, MHWRender::MGeometry::kPoints,
	            MHWRender::MRenderItem::sActiveLineDepthPriority);
	        updateRenderItem(container, geom, item, totalPoints, mSolidUIShader);
        } 
        else // no points, try bounding boxes.
            wantBoxes = true;
    }

    if (wantBoxes)
    {
        if (geom->VisibleGeometryCount() > 0)
        {
	        MHWRender::MRenderItem* item = getItem(container, perObjectBoxItemName_, MHWRender::MGeometry::kLines,
	            MHWRender::MRenderItem::sActiveLineDepthPriority);
	        updateRenderItem(container, geom, item, geom->VisibleGeometryCount()*kCubeCount, mSolidUIShader, false, true);
        } 
        else // no visible geometry.  Draw a single box.
            wantBox = true;
    }

    if (wantBox)
    {
        // Create the wire box render item if needed
        MHWRender::MRenderItem* item = getItem(container, wireframecubeItemName_, MHWRender::MGeometry::kLines,
            MHWRender::MRenderItem::sActiveLineDepthPriority);
        updateWireframeCubeItem(standIn, item, false);
    }

    if (geom->deferStandinLoad && drawOverride <= 0)
    {
        // Create the wire box render item for the defer load.
        MHWRender::MRenderItem* item = getItem(container, deferStandInItemName_, MHWRender::MGeometry::kLines,
            MHWRender::MRenderItem::sActiveLineDepthPriority);
        updateWireframeCubeItem(standIn, item, true);
    }

    mBBChanged = false;
}

void CArnoldStandInSubSceneOverride::updateWireframeCubeItem(CArnoldStandInShape* standIn, MHWRender::MRenderItem* item, bool scaled)
{
    // sanity check
    if(!item)
        return;

    // enable the item and set the shader
    item->enable(true);
    item->setShader(mSolidUIShader);

    // Get the bounding box from the stand-in
    MPoint bbMin;
    MPoint bbMax;
    if (scaled)
    {
        MBoundingBox box = standIn->boundingBox();
        bbMin = box.min(); bbMax = box.max();
    }
    else
    {
        CArnoldStandInGeom* geom = standIn->geometry();
        bbMin = geom->BBmin; bbMax = geom->BBmax;
    }

    // create the vertex and index buffers (TODO: Should get the existing ones if they already exists)
    const MHWRender::MVertexBufferDescriptor posDesc("", MHWRender::MGeometry::kPosition, MHWRender::MGeometry::kFloat, 3);
    MHWRender::MVertexBuffer* verticesBuffer = new MHWRender::MVertexBuffer(posDesc);
    MHWRender::MIndexBuffer*  indexBuffer = new MHWRender::MIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);

    // acquire the vertex and index streams for write
    float* vertices = (float*)verticesBuffer->acquire(kCubeCount);
    unsigned int* indices = (unsigned int*)indexBuffer->acquire(kCubeCount);

    // Add the cube into the vertex buffer.
    for (int verticesPointerOffset = 0, currentVertex = 0 ; currentVertex < kCubeCount; ++currentVertex)
    {
        indices[currentVertex] = currentVertex;
        for (int elem = 0; elem < 3; ++elem)
            vertices[verticesPointerOffset++] = cube[currentVertex][elem] * float(bbMax[elem] - bbMin[elem]) + float(bbMin[elem]);
    }

    // commit the changes to the vertex and index buffers
    verticesBuffer->commit(vertices);
    indexBuffer->commit(indices);

    // create an array to house the vertex buffer
    MHWRender::MVertexBufferArray wireBuffers;
    wireBuffers.addBuffer("positions", verticesBuffer);

    // pass the geometry to the render item
    setGeometryForRenderItem(*item, wireBuffers, *indexBuffer, NULL);
}

void CArnoldStandInSubSceneOverride::updateRenderItem(MHWRender::MSubSceneContainer& container, CArnoldStandInGeom* geom, 
                                                          MHWRender::MRenderItem* item, size_t totalCount, 
                                                          MHWRender::MShaderInstance* shader, bool wantNormals, bool boxMode)
{
    // sanity check
    if(!item)
        return;

    // enable the item and set the shader
    item->enable(true);
    item->setShader(shader);

    // get the total number of indices based on the mode or primitive type
    size_t totalIndexCount = 0;
    if (boxMode)
        totalIndexCount = totalCount;
    else
    {
        switch (item->primitive())
        {
        case MHWRender::MGeometry::kTriangles:
            totalIndexCount = geom->TriangleIndexCount(wantNormals);
            break;
        case MHWRender::MGeometry::kLines:
            totalIndexCount = geom->WireIndexCount();
            break;
        case MHWRender::MGeometry::kPoints:
            totalIndexCount = totalCount;
            break;
        }
    }

    // create the vertex buffer and index buffers (TODO: Should get the existing ones if they already exists)
    const MHWRender::MVertexBufferDescriptor posDesc("", MHWRender::MGeometry::kPosition, MHWRender::MGeometry::kFloat, 3);
    MHWRender::MVertexBuffer* verticesBuffer = new MHWRender::MVertexBuffer(posDesc);
    MHWRender::MIndexBuffer*  indexBuffer = new MHWRender::MIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);

    // acquire the streams for write
    float* vertices = (float*)verticesBuffer->acquire(totalCount);
    unsigned int* indices = (unsigned int*)indexBuffer->acquire(totalIndexCount);

    // create a vertex buffer for normals if required
    MHWRender::MVertexBuffer* normalBuffer = NULL;
    float* normals = NULL;
    if (wantNormals)
    {
        const MHWRender::MVertexBufferDescriptor normDesc("", MHWRender::MGeometry::kNormal, MHWRender::MGeometry::kFloat, 3);
        normalBuffer = new MHWRender::MVertexBuffer(normDesc);

        // acquire the vertex stream for write
        normals = (float*)normalBuffer->acquire(totalCount);
    }

    // Process each of the stand-in geometry items
    size_t startIndex = 0, pointOffset = 0;
    for (CArnoldStandInGeom::geometryListIterType it = geom->m_geometryList.begin();
        it != geom->m_geometryList.end(); ++it)
    {
        // fill the index, vertex, and optionally the normal streams with data from the geometry
        fillBuffers(*it->second, indices, vertices, normals, startIndex, pointOffset, 
            item->primitive(), wantNormals, boxMode);
    }

    // process each of the instances
    for (CArnoldStandInGeom::instanceListIterType it = geom->m_instanceList.begin();
        it != geom->m_instanceList.end(); ++it)
    {
        // fill the index, vertex, and optionally the normal streams with data from the geometry
        fillBuffers((*it)->GetGeometry(), indices, vertices, normals, startIndex, pointOffset, 
            item->primitive(), wantNormals, boxMode);
    }

    // commit the index and vertex buffers for completion
    indexBuffer->commit(indices);
    verticesBuffer->commit(vertices);
    if (normalBuffer)
        normalBuffer->commit(normals);

    // create an array to house the vertex buffers
    MHWRender::MVertexBufferArray vertexBuffers;
    vertexBuffers.addBuffer("positions", verticesBuffer);
    if (normalBuffer)
        vertexBuffers.addBuffer("normals", normalBuffer);

    // pass the geometry to the render item
    setGeometryForRenderItem(*item, vertexBuffers, *indexBuffer, NULL);
}

void CArnoldStandInSubSceneOverride::fillBuffers(const CArnoldStandInGeometry& standIn, 
    unsigned int* indices, float* vertices, float* normals, size_t& startIndex, size_t& pointOffset,
    const MHWRender::MGeometry::Primitive& primitive, bool wantNormals, bool boxMode)
{
    if (!standIn.Visible()) return;

    if (boxMode)
    {       
        // Add the cube into the vertex and index buffer.
        MBoundingBox box = standIn.GetBBox();
        for (int currentVertex = 0 ; currentVertex < kCubeCount; ++currentVertex)
        {
            indices[currentVertex+startIndex] = currentVertex+pointOffset;
            for (int elem = 0; elem < 3; ++elem)
                vertices[(currentVertex+pointOffset)*3+elem] = cube[currentVertex][elem] * float(box.max()[elem] - box.min()[elem]) + float(box.min()[elem]);
        }
        pointOffset += kCubeCount;
        startIndex += kCubeCount;
    }
    else
    {
        // get the triangle, wire, or point indexing based on the primitive type.
        startIndex += getIndexing(standIn, indices+startIndex, pointOffset, primitive, wantNormals);

        // get the vertex streams (normals are optional)
        pointOffset += getVertexStreams(standIn, vertices+(pointOffset*3), (normals ? normals+(pointOffset*3) : NULL));
    }
}

size_t CArnoldStandInSubSceneOverride::getIndexing(
    const CArnoldStandInGeometry& standIn, unsigned int* indices, unsigned int pointOffset, 
    const MHWRender::MGeometry::Primitive& primitive, bool wantNormals)
{
    size_t indexCount(0);
    switch (primitive)
    {
    case MHWRender::MGeometry::kTriangles:
        indexCount = standIn.TriangleIndexCount(wantNormals);
        standIn.GetTriangleIndexing(indices, pointOffset, wantNormals);
        break;
    case MHWRender::MGeometry::kLines:
        indexCount = standIn.WireIndexCount();
        standIn.GetWireIndexing(indices, pointOffset);
        break;
    case MHWRender::MGeometry::kPoints:
        indexCount = standIn.PointCount();
        for(unsigned int i = 0; i < indexCount; ++i)
            indices[i] = i+pointOffset;
        break;
    }
    return indexCount;
}

size_t CArnoldStandInSubSceneOverride::getVertexStreams(const CArnoldStandInGeometry& standIn, float* vertices, float* normals)
{
    // transform and add the points
    const AtMatrix& matrix = standIn.GetMatrix();

    if (normals)
    {
        // get the vertices and normals as single indexed streams
        // (must have the same counts)
        standIn.GetSharedVertices(vertices, &matrix);
        standIn.GetSharedNormals(normals, &matrix);
        return standIn.SharedVertexCount();
    }
    else
    {
        standIn.GetPoints(vertices, &matrix);
        return standIn.PointCount();
    }
}

int CArnoldStandInSubSceneOverride::getDrawOverride()
{
    MStatus status;
    MFnDagNode node(mLocatorNode, &status);

    int drawOverride = 0;
    MPlug plug = node.findPlug("standInDrawOverride", &status);
    if (!plug.isNull() && status)
    {
        const int localDrawOverride = plug.asShort();
        if (localDrawOverride == 0) // use global settings
        {
            MObject ArnoldRenderOptionsNode = CArnoldOptionsNode::getOptionsNode();
            if (!ArnoldRenderOptionsNode.isNull())
                drawOverride = MFnDependencyNode(ArnoldRenderOptionsNode).findPlug("standin_draw_override").asShort();
        }
        else
            drawOverride = localDrawOverride - 1;
    }
    return drawOverride;
}

void CArnoldStandInSubSceneOverride::updateShaderFromNode()
{
    MStatus status;
    MFnDagNode node(mLocatorNode, &status);

    MPlugArray connectedPlugs;
    MObjectArray sets, comps;
    if (node.getConnectedSetsAndMembers(0, sets, comps, false))
    {
        for (unsigned int i=0; i<sets.length(); i++)
        {
            MFnDependencyNode fnSet(sets[i], &status);
            if (status)
            {
                MPlug shaderPlug = fnSet.findPlug("surfaceShader");
                if (!shaderPlug.isNull())
                {
                    shaderPlug.connectedTo(connectedPlugs, true, false);
                    if (connectedPlugs.length() > 0) break;
                }
            }
        }
    }

    if (connectedPlugs.length() > 0)
    {
        MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
        if (renderer)
        {
            const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
            if (shaderMgr)
            {
                //MDagPath path;
                //node.getPath(path);
                MDagPathArray paths;
                if (node.getAllPaths(paths) == MS::kSuccess)
                {
                    StandInShadedItemUserData* userData = new StandInShadedItemUserData(this);
                    mShaderFromNode = shaderMgr->getShaderFromNode(connectedPlugs[0].node(), 
                        paths[0], standInShadedItemLinkLost, userData, 0, 0/*, true*/);
                }
            }
        }
    }
}