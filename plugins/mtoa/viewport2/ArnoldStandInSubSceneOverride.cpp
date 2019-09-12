#include "ArnoldStandInSubSceneOverride.h"

#include "nodes/shape/ArnoldStandIns.h"
#include "nodes/options/ArnoldOptionsNode.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MEventMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MDGMessage.h>
#include <maya/MBoundingBox.h>
#include <maya/MPlugArray.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionContext.h>
#include <maya/M3dView.h>

#include <iostream>

#include <ai.h>

#include "utils/MayaUtils.h"
#include "ViewportUtils.h"

const MString colorParameterName_       = "solidColor";
const MString diffuseColorParameterName_ = "diffuseColor";

const MString unselectedItemName_       = "unselected_UI";
const MString selectedItemName_         = "selected_UI";
const MString leadItemName_             = "lead_UI";
const MString polyItemName_             = "polygons";
const MString shadedPolyItemName_       = "polygons_shaded";

const MString unselectedDeferItemName_  = "unselected_defer_UI";
const MString selectedDeferItemName_    = "selected_defer_UI";
const MString leadDeferItemName_        = "lead_defer_UI";

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
    void standInNodeDirtyPlugCallback(MObject& /*node*/, MPlug& plug, void* clientData)
    {
        MFnAttribute attr(plug.attribute());
        MString attrName = attr.name();

        if (attrName == "MinBoundingBox0" || attrName == "MinBoundingBox1" || attrName == "MinBoundingBox2" ||
            attrName == "MaxBoundingBox0" || attrName == "MaxBoundingBox1" || attrName == "MaxBoundingBox2")
            return; // we don't care about changes to these attributes.  They are set when processing the geometry.

        bool reuse = (attrName == "worldMatrix" || 
            attrName == "worldInverseMatrix" || 
            attrName == "parentMatrix" || 
            attrName == "parentInverseMatrix" || 
            attrName == "mode" || 
            attrName == "standInDrawOverride" || 
            attrName == "deferStandinLoad") ? true : false;
        static_cast<CArnoldStandInSubSceneOverride*>(clientData)->invalidate(reuse);
    }

    void standInAttributeChangedCallback(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug,void* clientData)
    {
        // The renderLayerInfo connection is added and removed due to render layer membership changed. We need to
        // invalidate the standIn in this case.
        MFnAttribute attr(plug.attribute());
        MString attrName = attr.name();
        if(attrName == "renderLayerInfo")
            static_cast<CArnoldStandInSubSceneOverride*>(clientData)->invalidate(false);
    }

    void renderLayerChangeCallback(void* clientData)
    {
        // If the render layer visibility has changed, we need to invalidate the standIn.
        static_cast<CArnoldStandInSubSceneOverride*>(clientData)->invalidate(false);
    }

    void globalOptionsChanged(MNodeMessage::AttributeMessage /*msg*/, MPlug& plug, MPlug& /*otherPlug*/, void *clientData)
    {
        // invalidate the draw data if the global draw override changes.
        if (plug.attribute() == CArnoldOptionsNode::s_enable_standin_draw)
            static_cast<CArnoldStandInSubSceneOverride*>(clientData)->invalidate(true);

        M3dView::scheduleRefreshAllViews();
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
, mSelectedSolidUIShader(NULL)
, mLeadSolidUIShader(NULL)
, mSolidShader(NULL)
, mBlinnShader(NULL)
, mShaderFromNode(NULL)
, mLocatorNode(obj)
, fLeadIndex(0)
, fNumInstances(0)
, mBBChanged(true)
, mOneTimeUpdate(true)
, mReuseBuffers(false)
, mAttribChangedID(0)
, mGlobalOptionsChangedID(0)
, mGlobalOptionsCreatedID(0)
, fLastTimeInvisible(false)
{
    MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
    if (!renderer)
        return;

    const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
    if (!shaderMgr)
        return;

	mSolidUIShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidShader);
    mSelectedSolidUIShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidShader);
    mLeadSolidUIShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidShader);
    mSolidShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidShader);
    mBlinnShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dBlinnShader);

    float  solidColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f};
    mSolidShader->setParameter(colorParameterName_, solidColor);
    mBlinnShader->setParameter(diffuseColorParameterName_, solidColor);

    float  solidiColor[4] = { 0.2f, 0.2f, 0.5f, 1.0f};
    mSelectedSolidUIShader->setParameter(colorParameterName_, solidiColor);
    

    mNodeDirtyPlugID = MNodeMessage::addNodeDirtyPlugCallback(mLocatorNode, standInNodeDirtyPlugCallback, this);
    MStatus status;
    MFnDagNode node(mLocatorNode, &status);
    MObject parentNode = node.parent(0);
    mAttribChangedID = MNodeMessage::addAttributeChangedCallback(parentNode, standInAttributeChangedCallback, this);
    mRenderLayerManagerChangeID = MEventMessage::addEventCallback("renderLayerManagerChange", renderLayerChangeCallback, this);
    mGlobalOptionsCreatedID = MDGMessage::addNodeAddedCallback(CArnoldStandInSubSceneOverride::globalOptionsAdded, "aiOptions", this);

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
            if(mSelectedSolidUIShader)
                shaderMgr->releaseShader(mSelectedSolidUIShader);
            if(mLeadSolidUIShader)
                shaderMgr->releaseShader(mLeadSolidUIShader);
            if(mSolidShader)
                shaderMgr->releaseShader(mSolidShader);
            if(mBlinnShader)
                shaderMgr->releaseShader(mBlinnShader);
            if(mShaderFromNode)
                shaderMgr->releaseShader(mShaderFromNode);
		}
    }
    mSolidUIShader = NULL;
    mSelectedSolidUIShader = NULL;
    mLeadSolidUIShader = NULL;
    mSolidShader = NULL;
    mBlinnShader = NULL;
    mShaderFromNode = NULL;

    MNodeMessage::removeCallback(mNodeDirtyPlugID);
    MNodeMessage::removeCallback(mAttribChangedID);
    MMessage::removeCallback(mRenderLayerManagerChangeID);
    if (mGlobalOptionsCreatedID != 0)
        MNodeMessage::removeCallback(mGlobalOptionsCreatedID);
    if (mGlobalOptionsChangedID != 0)
        MNodeMessage::removeCallback(mGlobalOptionsChangedID);
}

// Static callback is called whenever a aiOptions node is created
void CArnoldStandInSubSceneOverride::globalOptionsAdded(MObject& node, void* clientData)
{
    CArnoldStandInSubSceneOverride* ssOverride = static_cast<CArnoldStandInSubSceneOverride*>(clientData);
    ssOverride->globalOptionsAdded(node);
}

void CArnoldStandInSubSceneOverride::globalOptionsAdded(MObject& node)
{
    if (mGlobalOptionsChangedID != 0)
        MNodeMessage::removeCallback(mGlobalOptionsChangedID);

    mGlobalOptionsChangedID = MNodeMessage::addAttributeChangedCallback(node, globalOptionsChanged, this);
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
    return true;
}

bool CArnoldStandInSubSceneOverride::anyChanges(const MHWRender::MSubSceneContainer& container)
{
    MStatus status;
    MFnDagNode node(mLocatorNode, &status);
    if (!status) return false;
    MDagPathArray instances;
    if (!node.getAllPaths(instances) || instances.length() == 0) return false;

	// Check to see if there are any invisible instances.
	// If there are then we need to recompute.
	bool invisibleInstance = false;
	for(unsigned int i=0; i<instances.length(); i++) {		
		MHWRender::DisplayStatus displayStatus = MHWRender::MGeometryUtilities::displayStatus(instances[i]);
		if(displayStatus == MHWRender::kInvisible)
		{
			invisibleInstance = true;
			break;
		}
	}
	if(invisibleInstance)
	{
		fLastTimeInvisible = true;
	}
	else if(fLastTimeInvisible)
	{
		fLastTimeInvisible = false;
	}

    // there was a change to one or more instances, update required.
    if (updateInstanceData(instances))
        return true;

    // the container is not yet populated, update required.
    if (!container.count())
        return true;

    // Update required when something has changed.
    return mOneTimeUpdate||mBBChanged;
}

namespace {
    void clearRenderItem(MHWRender::MSubSceneContainer& container, const MString& itemName, bool reuse)
    {
        MHWRender::MRenderItem* item = container.find(itemName);
        if (item) // delete or disable the item 
        {
            if (reuse)
                item->enable(false);
            else
                container.remove(itemName);
        }
    }

    MHWRender::MRenderItem* findRenderItem(MHWRender::MSubSceneContainer& container, const MString& itemName, bool reuse)
    {
        MHWRender::MRenderItem* item = container.find(itemName);
        if (item && !reuse)
        {
            container.remove(itemName);
            return NULL;
        }
        return item;
    }
}

void CArnoldStandInSubSceneOverride::update(
    MHWRender::MSubSceneContainer& container, const MHWRender::MFrameContext& frameContext)
{
    if (!anyChanges(container))
        return;
    // initialize
    mOneTimeUpdate = false;
    //container.clear();

    // get data to use later.
    MStatus status;
    MFnDagNode node(mLocatorNode, &status);
    CArnoldStandInShape* standIn = static_cast<CArnoldStandInShape*>(node.userNode());
    CArnoldStandInGeom* geom = NULL;

    // shape instance gathering
    // ************************************
    MMatrixArray instanceMatrixArray;
    MMatrixArray selectedInstanceMatrixArray;
    MMatrixArray unselectedInstanceMatrixArray;
    int leadIndex = -1;
    getInstanceTransforms(instanceMatrixArray, selectedInstanceMatrixArray, unselectedInstanceMatrixArray, leadIndex);

    bool hasLeadItem = leadIndex >= 0;
    bool anyInstanceSelected = selectedInstanceMatrixArray.length() > 0;  //don't count lead item
    bool anyInstanceUnselected = unselectedInstanceMatrixArray.length() > 0;

    // end of shape instance gathering.
    // *************************************

    // get the drawOverride to determine if we enable display of standins
    int drawOverride = getDrawOverride();

    // get the draw mode
    int mode = standIn->drawMode();
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
    //bool wantDeferBox = (standIn->deferStandinLoad() && drawOverride <= 0);

    MHWRender::MRenderItem* shadedItem = NULL;
    MHWRender::MRenderItem* selectedItem = NULL;
    MHWRender::MRenderItem* unselectedItem = NULL;
    MHWRender::MRenderItem* leadItem = NULL;
    MHWRender::MRenderItem* selectedDeferItem = NULL;
    MHWRender::MRenderItem* unselectedDeferItem = NULL;
    MHWRender::MRenderItem* leadDeferItem = NULL;

    if (wantShaded)
    {
        // TODO: There are currently problems with flipped normals or negatively scaled geometry with the shader from node for some reason.
        //if (!mShaderFromNode)
        //    updateShaderFromNode();

        shadedItem = findRenderItem(container, shadedPolyItemName_, mReuseBuffers);
        if (!shadedItem)
        {
            geom = geom ? geom : standIn->geometry(); // load this as late as possible

            size_t sharedVertexCount = geom->SharedVertexCount();
            if (sharedVertexCount > 0)
            {
                shadedItem = getItem(container, shadedPolyItemName_, MHWRender::MGeometry::kTriangles,
                    MHWRender::MRenderItem::sDormantFilledDepthPriority);
                updateRenderItem(container, geom, shadedItem, sharedVertexCount, (mShaderFromNode ? mShaderFromNode : mBlinnShader), true);
            }
            else // no normals, try solid
            {
                wantShaded = false;
                wantSolid = true;
            }
        }
        else
            shadedItem->enable(true);
 
        if (!wantWireframe /*&& !wantDeferBox*/)
        {
            // if only drawing shaded mode then draw a box for the selected instances 
            // to give some indication of the selection.  Don't draw any boxes for unselected items.
            wantBox = true;
            anyInstanceUnselected = false;
        } 
    }
    if (!wantShaded) // clear or reset the item if not used
        clearRenderItem(container, shadedPolyItemName_, mReuseBuffers);

    if (wantSolid)
    {
        shadedItem = findRenderItem(container, polyItemName_, mReuseBuffers);
        if (!shadedItem)
        {
            geom = geom ? geom : standIn->geometry(); // load this as late as possible

            if (geom->TriangleIndexCount())
            {
                size_t totalPoints = geom->PointCount();
                shadedItem = getItem(container, polyItemName_, MHWRender::MGeometry::kTriangles,
                    MHWRender::MRenderItem::sDormantFilledDepthPriority);
                updateRenderItem(container, geom, shadedItem, totalPoints, mSolidShader);

            }
            else // no triangles, try wireframe
            {
                wantSolid = false;
                wantWireframe = true;
            }
        }
        else
            shadedItem->enable(true);
    }
    if (!wantSolid) // clear or reset the item if not used
        clearRenderItem(container, polyItemName_, mReuseBuffers);

    // update the UI items
    MHWRender::MGeometry::Primitive geometryType = MHWRender::MGeometry::kLines;


    MHWRender::MRenderItem** items[] = {&unselectedItem, &selectedItem, &leadItem};
    //MHWRender::MRenderItem** deferItems[] = { &unselectedDeferItem, &selectedDeferItem, &leadDeferItem };
    const MString itemNames[] = { unselectedItemName_, selectedItemName_, leadItemName_};
    const MHWRender::MShaderInstance* shaders[] = { mSolidUIShader, mSelectedSolidUIShader, mLeadSolidUIShader };
    //const MHWRender::MShaderInstance* shaders[] = { mSolidUIShader, mSolidUIShader, mSolidUIShader };
    if (wantWireframe)
    {
        geom = geom ? geom : standIn->geometry(); // load this as late as possible
        wantPoints = (geom->WireIndexCount() == 0); // if no wireframe try points
    }
    if (wantPoints)
    {
        geom = geom ? geom : standIn->geometry(); // load this as late as possible
        wantBoxes = (geom->PointCount() == 0); // if no points, try bounding boxes.
    }
    if (wantBoxes)
    {
        geom = geom ? geom : standIn->geometry(); // load this as late as possible
        wantBox = (geom->VisibleGeometryCount() == 0); // no visible geometry.  Draw a single box.
    }
	 bool hasSelection = (geom && geom->hasSelection) && (hasLeadItem);
     bool addSelectedBox = false;

     if (hasSelection)
     {
        if ( anyInstanceSelected || hasLeadItem) 
            unselectedInstanceMatrixArray = selectedInstanceMatrixArray = instanceMatrixArray;
        anyInstanceSelected = true;
        if (wantBox)
        {
            addSelectedBox = true;
            wantBoxes = true;
        }
     }
	 // arrays of elements each to handle selected, unselected, and lead items
    const bool test[] = {anyInstanceUnselected, anyInstanceSelected, hasLeadItem};

    const bool testType[] = { wantPoints, wantWireframe, wantBoxes, wantBox/*, wantDeferBox*/ };
    const MString typeNames[] = { "_wires", "_points", "_boxes", "_box", /*"_deferBox"*/ };

    // if we have any UI items to draw then add them here
    MHWRender::MRenderItem* thisItem = NULL;

    unsigned int depthPriority;
    StandinSelectionFilter selectionFilter = STANDIN_GEOM_ALL;

    // three different states (unselected, selected, and lead)
    // same geometry, different shaders.
    for (int x = 0; x < 3; ++x)
    {
        if (test[x])
        {
            if (hasSelection)
                selectionFilter = (x > 1) ? STANDIN_GEOM_SELECTED : STANDIN_GEOM_UNSELECTED;

            // five different types (wires, points, boxes, box, and/or deferBox)
            for (int i = 0; i < 4; ++i)
            {
                MString itemName = itemNames[x];
                itemName += typeNames[i];
                if (testType[i])
                {
                    // displaying "boxes" -> if we just added the box on top of the existing display,
                    // we want to skip "unselected"
                    if (i == 2 && addSelectedBox && selectionFilter != STANDIN_GEOM_SELECTED)
                        continue;

                    //bool isDeferBox = (i == 4);
                    thisItem = findRenderItem(container, itemName, mReuseBuffers);
                    if (!thisItem)
                    {
                        // Create the ui render item if needed
                        if(i == 1)
                        {
                            depthPriority = (x == 1 ? MHWRender::MRenderItem::sActivePointDepthPriority : MHWRender::MRenderItem::sDormantPointDepthPriority);
                        }
                        else
                        {
                            depthPriority = (x == 1 ? MHWRender::MRenderItem::sActiveWireDepthPriority : MHWRender::MRenderItem::sDormantWireDepthPriority);
                        }

                        geometryType = (i > 0) ? MHWRender::MGeometry::kLines : MHWRender::MGeometry::kPoints;
                        thisItem = getItem(container, itemName, geometryType, depthPriority);
                        if (i > 2) // first three are not cubes, last two are.
                        {
                            updateWireframeCubeItem(standIn, thisItem, shaders[x], false /*isDeferBox*/);
                        }
                        else
                        {
                            bool boxMode = (i == 2);
                            size_t totalPoints = wantBoxes ? geom->VisibleGeometryCount(selectionFilter)*kCubeCount : geom->PointCount(selectionFilter);
                            updateRenderItem(container, geom, thisItem, totalPoints, shaders[x], false, boxMode, selectionFilter);
                            
                        }
                    }
                    else
                        thisItem->enable(true);

                    //if (isDeferBox)
                        //*(deferItems[x]) = thisItem;
                    //else
                    *(items[x]) = thisItem;
                }
                else // clear or reset the item if not used
                    clearRenderItem(container, itemName, mReuseBuffers);
            }
        }
        else
        {
            // clear or reset the item if it is not used.  This includes the wires, points, boxes, box and deferred box.
            for (int i = 0; i < 4; ++i)
            {
                clearRenderItem(container, itemNames[x] + typeNames[i], mReuseBuffers);
            }
        }
    }
    if (leadItem)
        leadItem->setMatrix(&instanceMatrixArray[leadIndex]);
    if (leadDeferItem)
        leadDeferItem->setMatrix(&instanceMatrixArray[leadIndex]);

    // Set the transform array for each render item with multiple instances.
    // Note this has to happen after the geometry and shaders are set, otherwise it will fail.
    if (shadedItem)
        setInstanceTransformArray(*shadedItem, instanceMatrixArray);
    if (selectedItem)
        setInstanceTransformArray(*selectedItem, selectedInstanceMatrixArray);
    if (unselectedItem)
        setInstanceTransformArray(*unselectedItem, unselectedInstanceMatrixArray);

    if (selectedDeferItem)
        setInstanceTransformArray(*selectedDeferItem, selectedInstanceMatrixArray);
    if (unselectedDeferItem)
        setInstanceTransformArray(*unselectedDeferItem, unselectedInstanceMatrixArray);

    mBBChanged = false;
    mReuseBuffers = true;
}

MHWRender::MRenderItem* CArnoldStandInSubSceneOverride::getItem(
    MHWRender::MSubSceneContainer& container,
    const MString& name,
    MHWRender::MGeometry::Primitive primitiveType,
    unsigned int depthPriority,
    MHWRender::MGeometry::DrawMode drawMode)
{
    MHWRender::MRenderItem* item = MHWRender::MRenderItem::Create(name,
        MHWRender::MRenderItem::NonMaterialSceneItem, primitiveType);
    item->setDrawMode(drawMode);
    item->depthPriority(depthPriority);
#if MAYA_API_VERSION >= 201800
    item->setCompatibleWithMayaInstancer(true);
#endif
    container.add(item);
    return item;
}

void CArnoldStandInSubSceneOverride::updateWireframeCubeItem(CArnoldStandInShape* standIn, 
    MHWRender::MRenderItem* item, const MHWRender::MShaderInstance* shader, bool scaled)
{
    // sanity check
    if(!item)
        return;

    // enable the item and set the shader
    item->enable(true);
    item->setShader(shader);

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
    MHWRender::MVertexBuffer verticesBuffer(
        MHWRender::MVertexBufferDescriptor("", MHWRender::MGeometry::kPosition, MHWRender::MGeometry::kFloat, 3));
    MHWRender::MIndexBuffer  indexBuffer(MHWRender::MGeometry::kUnsignedInt32);

    // acquire the vertex and index streams for write
    float* vertices = (float*)verticesBuffer.acquire(kCubeCount, true);
    unsigned int* indices = (unsigned int*)indexBuffer.acquire(kCubeCount, true);

    // Add the cube into the vertex buffer.
    for (int verticesPointerOffset = 0, currentVertex = 0 ; currentVertex < kCubeCount; ++currentVertex)
    {
        indices[currentVertex] = currentVertex;
        for (int elem = 0; elem < 3; ++elem)
            vertices[verticesPointerOffset++] = cube[currentVertex][elem] * float(bbMax[elem] - bbMin[elem]) + float(bbMin[elem]);
    }

    // commit the changes to the vertex and index buffers
    verticesBuffer.commit(vertices);
    indexBuffer.commit(indices);

    // create an array to house the vertex buffer
    MHWRender::MVertexBufferArray wireBuffers;
    wireBuffers.addBuffer("positions", &verticesBuffer);

    // pass the geometry to the render item
    setGeometryForRenderItem(*item, wireBuffers, indexBuffer, NULL);
}

void CArnoldStandInSubSceneOverride::updateRenderItem(MHWRender::MSubSceneContainer& container, CArnoldStandInGeom* geom, 
     MHWRender::MRenderItem* item, size_t totalCount, const MHWRender::MShaderInstance* shader, bool wantNormals, bool boxMode, StandinSelectionFilter selectionFilter)
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
            totalIndexCount = geom->TriangleIndexCount(wantNormals, selectionFilter);
            break;
        case MHWRender::MGeometry::kLines:
            totalIndexCount = geom->WireIndexCount(selectionFilter);
            break;
        case MHWRender::MGeometry::kPoints:
            totalIndexCount = totalCount;
            break;
        default:
            break;
        }
    }

    // create the vertex buffer and index buffers (TODO: Should get the existing ones if they already exists)
    MHWRender::MVertexBuffer verticesBuffer(
        MHWRender::MVertexBufferDescriptor("", MHWRender::MGeometry::kPosition, MHWRender::MGeometry::kFloat, 3));
    MHWRender::MIndexBuffer  indexBuffer(MHWRender::MGeometry::kUnsignedInt32);

    // acquire the streams for write
    float* vertices = (float*)verticesBuffer.acquire(totalCount, true);
    unsigned int* indices = (unsigned int*)indexBuffer.acquire(totalIndexCount, true);

    // create a vertex buffer for normals if required
    MHWRender::MVertexBuffer normalBuffer(
        MHWRender::MVertexBufferDescriptor("", MHWRender::MGeometry::kNormal, MHWRender::MGeometry::kFloat, 3));
    float* normals = NULL;
    if (wantNormals)
    {
        // acquire the vertex stream for write
        normals = (float*)normalBuffer.acquire(totalCount, true);
    }

    // Process each of the stand-in geometry items
    size_t startIndex = 0, pointOffset = 0;
    for (CArnoldStandInGeom::geometryListIterType it = geom->m_geometryList.begin();
        it != geom->m_geometryList.end(); ++it)
    {
        // we weren't testing IsVisible before, but it sounds like we should. 
        // The size of the buffers was computed based on the visibility
        if (it->second->Visible(selectionFilter)) 
        {            
            // fill the index, vertex, and optionally the normal streams with data from the geometry
            fillBuffers(*it->second, indices, vertices, normals, startIndex, pointOffset, 
                item->primitive(), wantNormals, boxMode);
        }
    }

    // process each of the instances
    for (CArnoldStandInGeom::instanceListIterType it = geom->m_instanceList.begin();
        it != geom->m_instanceList.end(); ++it)
    {
        // we weren't testing IsVisible before, but it sounds like we should. 
        // The size of the buffers was computed based on the visibility
        if ((*it)->GetGeometry().Visible(selectionFilter))
        {
            // fill the index, vertex, and optionally the normal streams with data from the geometry
            fillBuffers((*it)->GetGeometry(), indices, vertices, normals, startIndex, pointOffset, 
                item->primitive(), wantNormals, boxMode);
        }        
    }

    // commit the index and vertex buffers for completion
    indexBuffer.commit(indices);
    verticesBuffer.commit(vertices);
    if (wantNormals)
        normalBuffer.commit(normals);

    // create an array to house the vertex buffers
    MHWRender::MVertexBufferArray vertexBuffers;
    vertexBuffers.addBuffer("positions", &verticesBuffer);
    if (wantNormals)
        vertexBuffers.addBuffer("normals", &normalBuffer);

    // pass the geometry to the render item
    setGeometryForRenderItem(*item, vertexBuffers, indexBuffer, NULL);
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
    default:
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
    MPlug plug = node.findPlug("standInDrawOverride", true, &status);
    if (!plug.isNull() && status)
    {
        const int localDrawOverride = plug.asShort();
        if (localDrawOverride == 0) // use global settings
        {
            MObject ArnoldRenderOptionsNode = CArnoldOptionsNode::getOptionsNode();
            if (!ArnoldRenderOptionsNode.isNull())
                drawOverride = MFnDependencyNode(ArnoldRenderOptionsNode).findPlug("standin_draw_override", true).asShort();
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
                MPlug shaderPlug = fnSet.findPlug("surfaceShader", true);
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

bool CArnoldStandInSubSceneOverride::updateInstanceData(const MDagPathArray& instances)
{
    bool anyInstanceChanged = false;
    bool unselectedMaterialUpdated = false;
    bool selectedMaterialUpdated = false;
    bool leadMaterialUpdated = false;

    unsigned int instanceArrayLength = instances.length();
    unsigned int numInstances = 0;

    for (unsigned int instIdx=0; instIdx<instanceArrayLength; instIdx++)
    {
        const MDagPath& instance = instances[instIdx];
        if (!instance.isValid()  || !instance.isVisible())
            continue;

        unsigned int instanceNum = instance.instanceNumber();

        MHWRender::DisplayStatus displayStatus = MHWRender::MGeometryUtilities::displayStatus(instance);
        bool selected = (displayStatus == MHWRender::kHilite || displayStatus == MHWRender::kActive);
        bool lead = displayStatus == MHWRender::kLead;
        if (lead)
            fLeadIndex = instanceNum;

        // One time update for each of the shaders (selected, unselected, and lead)
        // ************************************************************************
        MHWRender::MShaderInstance* currentShader = NULL;
        if (!selectedMaterialUpdated && (selectedMaterialUpdated = selected))
            currentShader = mSelectedSolidUIShader;
        else if (!leadMaterialUpdated && (leadMaterialUpdated = lead))
            currentShader = mLeadSolidUIShader;
        else if (!unselectedMaterialUpdated && (unselectedMaterialUpdated = !selected))
            currentShader = mSolidUIShader;

        if (currentShader)   
        {
            MColor color = MHWRender::MGeometryUtilities::wireframeColor(instance);
            float  wireframeColor[4] = { color.r, color.g, color.b, 1.0f };
            currentShader->setParameter(colorParameterName_, wireframeColor);
        }
        // **************************************************************************

        InstanceInfo instanceInfo(instance, instance.inclusiveMatrix(), selected, lead);
        InstanceInfoMap::const_iterator iter = fInstanceInfoCache.find(instanceNum);
        if (iter == fInstanceInfoCache.end() ||
            iter->second.fSelected != instanceInfo.fSelected ||
            iter->second.fLead != instanceInfo.fLead ||
            !iter->second.fTransform.isEquivalent(instanceInfo.fTransform))
        {
            fInstanceInfoCache[instanceNum] = instanceInfo;
            anyInstanceChanged = true;
        }
        numInstances++;
    }

    if (fNumInstances != numInstances)
    {
        anyInstanceChanged = true;
        fNumInstances = numInstances;
    }

    return anyInstanceChanged;
}

void CArnoldStandInSubSceneOverride::getInstanceTransforms( 
    MMatrixArray& instanceMatrixArray, 
    MMatrixArray& selectedInstanceMatrixArray, 
    MMatrixArray& unselectedInstanceMatrixArray,
    int& leadIndex)
{
    unsigned int numInstances = 0;
    unsigned int numInstanceSelected = 0;
    unsigned int numInstanceUnselected = 0;

    instanceMatrixArray.setLength(fNumInstances);
    selectedInstanceMatrixArray.setLength(fNumInstances);
    unselectedInstanceMatrixArray.setLength(fNumInstances);

    MStatus status;
    MFnDagNode node(mLocatorNode, &status);
    MDagPathArray instances;
    node.getAllPaths(instances);

    // loop over the instances and fill the arrays.
    for (unsigned int instIdx=0; instIdx<instances.length(); instIdx++)
    {
        if(!instances[instIdx].isValid() || !instances[instIdx].isVisible())
            continue;

        InstanceInfo instanceInfo = fInstanceInfoCache[instIdx];
        instanceMatrixArray[numInstances] = instanceInfo.fTransform;
        if (instanceInfo.fLead)
            leadIndex = (int)numInstances;
        else if (instanceInfo.fSelected)
            selectedInstanceMatrixArray[numInstanceSelected++] = instanceInfo.fTransform;
        else
            unselectedInstanceMatrixArray[numInstanceUnselected++] = instanceInfo.fTransform;
        numInstances++;
    }

    instanceMatrixArray.setLength(numInstances);
    selectedInstanceMatrixArray.setLength(numInstanceSelected);
    unselectedInstanceMatrixArray.setLength(numInstanceUnselected);
}

bool CArnoldStandInSubSceneOverride::getInstancedSelectionPath(
    const MHWRender::MRenderItem& renderItem, const MHWRender::MIntersection& intersection, MDagPath& dagPath) const
{
    MString name = renderItem.name();

    // Are we looking for a lead item or a poly item?
    bool wantLeadItem = (name.indexW("lead") == 0); // starts with lead
    bool wantPolyItem = (name.indexW("polygon") == 0); // starts with polygon

    // Get the instance Id.  If looking for a lead item we know its id.
    int instanceId = wantLeadItem ? fLeadIndex : intersection.instanceID()-1;

    // If looking for selected or unselected items loop over the cache to count items we are interested in.
    // Stop when we find the index we are looking for and return the instance.
    unsigned int currentInstance = 0;
    bool wantSelectedItem = (name.indexW("selected") == 0); // starts with selected
    bool wantUnselectedItem = (name.indexW("unselected") == 0); // starts with unselected

    MStatus status;
    MFnDagNode node(mLocatorNode, &status);
    MDagPathArray instances;
    node.getAllPaths(instances);

    for (unsigned int instIdx=0; instIdx<instances.length(); instIdx++)
    {
        if(!instances[instIdx].isValid() || !instances[instIdx].isVisible())
            continue;

        // Get the instance from the cache by index.
		InstanceInfo instanceInfo = fInstanceInfoCache.find((int)instIdx)->second;

        if ((wantPolyItem || wantLeadItem ||
             (wantSelectedItem && instanceInfo.fSelected) ||
             (wantUnselectedItem && !(instanceInfo.fSelected || instanceInfo.fLead))) &&
            currentInstance++ == (unsigned int)instanceId) // keep track of how many interesting items we visit
        {
            // we have found the right index so return it.
            dagPath.set(instanceInfo.fInstance);
            if (dagPath.length() > 1)
                dagPath.pop(); // from shape to xform.
            return true;
        }
    }
    return false;
}
