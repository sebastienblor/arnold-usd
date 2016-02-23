#include "ArnoldPhotometricLightDrawOverride.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MTransformationMatrix.h>
#include <maya/M3dView.h>
#include <maya/MUIDrawManager.h>
#include <maya/MEventMessage.h>

#include <iostream>

#include "ViewportUtils.h"

#include <ai.h>

// Buffers for drawing
MPointArray CArnoldPhotometricLightDrawOverride::s_positions;
MUintArray CArnoldPhotometricLightDrawOverride::s_indexing;

bool CArnoldPhotometricLightDrawOverride::s_isValid = false;
bool CArnoldPhotometricLightDrawOverride::s_isInitialized = false;

MHWRender::MPxDrawOverride* CArnoldPhotometricLightDrawOverride::creator(const MObject& obj)
{
    return new CArnoldPhotometricLightDrawOverride(obj);
}

CArnoldPhotometricLightDrawOverride::CArnoldPhotometricLightDrawOverride(const MObject& obj) :
    MHWRender::MPxDrawOverride(obj, draw, false) // false = Mark as never dirty. Requires explicit dirty handling
{
	MStatus status;
	MFnDependencyNode node(obj, &status);
	m_node = status ? node.userNode() : 0;

	if (m_node)
	{
		m_ModelEditorChangedCbId = MEventMessage::addEventCallback(
										"modelEditorChanged", OnModelEditorChanged, this);
	}
}

CArnoldPhotometricLightDrawOverride::~CArnoldPhotometricLightDrawOverride()
{

}

void CArnoldPhotometricLightDrawOverride::OnModelEditorChanged(void *clientData)
{
	// Mark the node as being dirty so that it can update on display appearance changes
	CArnoldPhotometricLightDrawOverride *ovr = static_cast<CArnoldPhotometricLightDrawOverride*>(clientData);
	if (ovr && ovr->m_node)
	{
		MHWRender::MRenderer::setGeometryDrawDirty(ovr->m_node->thisMObject());
	}
}

bool CArnoldPhotometricLightDrawOverride::isBounded(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    return false;
}

MBoundingBox CArnoldPhotometricLightDrawOverride::boundingBox(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const
{
    MBoundingBox bbox;
    MFnDependencyNode depNode(objPath.node());

    
    return bbox;
}

bool CArnoldPhotometricLightDrawOverride::disableInternalBoundingBoxDraw() const
{
    return true;
}

struct SArnoldPhotometricLightUserData : public MUserData{
    float m_wireframeColor[4];

    SArnoldPhotometricLightUserData() : MUserData(false) { }
    
    void update(const MDagPath& objPath)
    {
		// Update the wireframe color
		if (MHWRender::kDormant == MHWRender::MGeometryUtilities::displayStatus(objPath))
		{
			m_wireframeColor[0] = 0.75f;
			m_wireframeColor[1] = 0.f;
			m_wireframeColor[2] = 0.f;
			m_wireframeColor[3] = 0.2f;
		}
		else
		{
			MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
			m_wireframeColor[0] = color.r;
			m_wireframeColor[1] = color.g;
			m_wireframeColor[2] = color.b;
			m_wireframeColor[3] = color.a;
		}
    }

    ~SArnoldPhotometricLightUserData()
    {
    }
};


MUserData* CArnoldPhotometricLightDrawOverride::prepareForDraw(
        const MDagPath& objPath,
        const MDagPath& cameraPath,
        const MHWRender::MFrameContext& frameContext,
        MUserData* oldData)
{
    initializeUserData();
    if (s_isValid)
    {
        SArnoldPhotometricLightUserData* data = reinterpret_cast<SArnoldPhotometricLightUserData*>(oldData);
        if (!data)
            data = new SArnoldPhotometricLightUserData();
        data->update(objPath);
        return data;
    }
    else return 0;
}

MHWRender::DrawAPI CArnoldPhotometricLightDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kAllDevices);
}

void CArnoldPhotometricLightDrawOverride::draw(const MHWRender::MDrawContext& context, const MUserData* data)
{
	// No-op
}

// Generate new static CPU data once
void CArnoldPhotometricLightDrawOverride::initializeUserData()
{
    if ((s_isInitialized == false))
    {
        s_isInitialized = true;
        s_isValid = true;

        CGPhotometricLightPrimitive::generateData(CArnoldPhotometricLightDrawOverride::s_positions,
												  CArnoldPhotometricLightDrawOverride::s_indexing);

    }
}

void CArnoldPhotometricLightDrawOverride::addUIDrawables(const MDagPath& objPath,
												MHWRender::MUIDrawManager& drawManager,
												const MHWRender::MFrameContext& frameContext,
												const MUserData* data)
{
	if (s_isValid == false)
        return;

	// Check for light type exclusion
	MUint64 typeExclusions = frameContext.objectTypeExclusions();
	if (typeExclusions & MHWRender::MFrameContext::kExcludeLights)
		return;

    const SArnoldPhotometricLightUserData* userData = reinterpret_cast<const SArnoldPhotometricLightUserData*>(data);
    if (userData == 0)
        return;

	drawManager.beginDrawable();
	drawManager.setColor( userData->m_wireframeColor );
	drawManager.mesh(MHWRender::MUIDrawManager::kLines, 
		CArnoldPhotometricLightDrawOverride::s_positions, NULL, NULL, 
		&CArnoldPhotometricLightDrawOverride::s_indexing );
	drawManager.endDrawable();
}
