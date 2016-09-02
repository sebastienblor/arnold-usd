#include "ArnoldLightBlockerDrawOverride.h"

#include <iostream>
#include <vector>

#include "ViewportUtils.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/M3dView.h>
#include <maya/MUIDrawManager.h>
#include <maya/MPointArray.h>
#include <maya/MUintArray.h>

bool CArnoldLightBlockerDrawOverride::s_isValid = false;
bool CArnoldLightBlockerDrawOverride::s_isInitialized = false;

struct CArnoldLightBlockerUserData : public MUserData{
	short m_primitiveType;
	MPointArray p_positions;
	MUintArray p_indices;
    float m_wireframeColor[4];

    CArnoldLightBlockerUserData() : MUserData(false) 
	{
		m_wireframeColor[0] = m_wireframeColor[1] = m_wireframeColor[2] = m_wireframeColor[3] = 1.0f;
		m_primitiveType = 6;
	}

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
		
		MFnDependencyNode depNode(objPath.node());		
		MStatus status;
		short primitiveType = 0;
        MPlug plug = depNode.findPlug("geometryType", &status);
        if (status && !plug.isNull())
        {
			primitiveType = plug.asShort();
		}
		bool primitiveTypeChanged = (m_primitiveType != primitiveType);

		// Change in primitive type
        if (primitiveType == 0) // Box
        {
			if (primitiveTypeChanged)
			{
				double scale[3] = { 1.0, 1.0, 1.0 };
				double offset[3] = { 0.0, 0.0, 0.0 };
				CGBoxPrimitive::generateData(p_positions, p_indices, scale, offset);
			}
			m_primitiveType = 0;
        }
        else if (primitiveType == 1) // Sphere
        {
			if (primitiveTypeChanged)
			{
				const double radius = 0.5;
				const unsigned int dimensions = 16;
				CGSpherePrimitive::generateData(p_positions, p_indices, radius, dimensions);
			}
			m_primitiveType = 1;			
		}
        else if (primitiveType == 2) // Plane
        {
			if (primitiveTypeChanged)
			{
				double scale[3] = { 0.5, 0.5, 0.5 };
				CGQuadPrimitive::generateData(p_positions, p_indices, scale);
			}
			m_primitiveType = 2;			
		}
        else if (primitiveType == 3) // Cylinder
        {
			if (primitiveTypeChanged)
			{
				double scale[3] = { 0.5, 0.5, 0.5 };
				CGCylinderPrimitive::generateData(p_positions, p_indices, scale);
			}
			m_primitiveType = 3;			
		}
	}

    ~CArnoldLightBlockerUserData()
    {
    }
};

MHWRender::MPxDrawOverride* CArnoldLightBlockerDrawOverride::creator(const MObject& obj)
{
    return new CArnoldLightBlockerDrawOverride(obj);
}

CArnoldLightBlockerDrawOverride::CArnoldLightBlockerDrawOverride(const MObject& obj) :
    MHWRender::MPxDrawOverride(obj, draw
#if MAYA_API_VERSION >= 201700
	, false 
#endif
	)
{

}

CArnoldLightBlockerDrawOverride::~CArnoldLightBlockerDrawOverride()
{

}

bool CArnoldLightBlockerDrawOverride::isBounded(
                                        const MDagPath& objPath,
                                        const MDagPath& cameraPath) const
{
    return true;
}

MBoundingBox CArnoldLightBlockerDrawOverride::boundingBox(
                                                const MDagPath& objPath,
                                                const MDagPath& cameraPath) const
{
    MBoundingBox bbox;
    bbox.expand(MPoint(-0.5, -0.5, -0.5));
    bbox.expand(MPoint(0.5, 0.5, 0.5));
    return bbox;
}

bool CArnoldLightBlockerDrawOverride::disableInternalBoundingBoxDraw() const
{
    return false;
}

MUserData* CArnoldLightBlockerDrawOverride::prepareForDraw(
                                                    const MDagPath& objPath,
                                                    const MDagPath& cameraPath,
                                                    const MHWRender::MFrameContext& frameContext,
                                                    MUserData* oldData)
{
	initializeUserData();

    if (s_isValid)
    {
        CArnoldLightBlockerUserData* data = reinterpret_cast<CArnoldLightBlockerUserData*>(oldData);
        if (!data)
            data = new CArnoldLightBlockerUserData();
        data->update(objPath);
        return data;
    }
    else return 0;
}

MHWRender::DrawAPI CArnoldLightBlockerDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kAllDevices);
}

// Generate user data for each primitive type
void CArnoldLightBlockerDrawOverride::initializeUserData()
{
    if (s_isInitialized == false)
    {
		s_isInitialized = true;
        s_isValid = true;
    }
}

void CArnoldLightBlockerDrawOverride::draw(
                                    const MHWRender::MDrawContext& context,
                                    const MUserData* data)
{
}

void CArnoldLightBlockerDrawOverride::addUIDrawables(const MDagPath& objPath,
												MHWRender::MUIDrawManager& drawManager,
												const MHWRender::MFrameContext& frameContext,
												const MUserData* data)
{
    if (s_isValid == false)
        return;

    const CArnoldLightBlockerUserData* userData = reinterpret_cast<const CArnoldLightBlockerUserData*>(data);
    if (userData == 0)
        return;

	drawManager.beginDrawable();
	drawManager.setColor( userData->m_wireframeColor );
	drawManager.mesh(MHWRender::MUIDrawManager::kLines, 
		userData->p_positions, NULL, NULL, &userData->p_indices );
	drawManager.endDrawable();
}