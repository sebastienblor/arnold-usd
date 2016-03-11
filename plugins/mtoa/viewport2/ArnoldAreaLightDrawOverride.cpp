#include "ArnoldAreaLightDrawOverride.h"

#include <iostream>
#include <vector>

#include "ViewportUtils.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/M3dView.h>
#include <maya/MUIDrawManager.h>
#include <maya/MPointArray.h>
#include <maya/MUintArray.h>

bool CArnoldAreaLightDrawOverride::s_isValid = false;
bool CArnoldAreaLightDrawOverride::s_isInitialized = false;

// TODO check about delete after use, and
// how to reuse buffers, rather than always
// recreating them, this might won't cause
// much performance problems, but cleaner,
// the better
struct CArnoldAreaLightUserData : public MUserData{
	MString m_primitiveType;
	double m_scale[3];
	MPointArray p_positions;
	MUintArray p_indices;
    float m_wireframeColor[4];

    CArnoldAreaLightUserData() : MUserData(false) 
	{
		m_scale[0] = m_scale[1] = 1.0;
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
		
		MString primitiveType = "quad";
		double scale2[3] = { 1.0, 1.0, 1.0 };

		MFnDependencyNode depNode(objPath.node());		
		MStatus status;
        MPlug plug = depNode.findPlug("aiTranslator", &status);
        MTransformationMatrix modelMatrix(objPath.inclusiveMatrix());
        if (status && !plug.isNull())
        {
			primitiveType = plug.asString();
		}
		bool primitiveTypeChanged = (m_primitiveType != primitiveType);

		// Change in primitive type
        if (primitiveType == "disk")
        {
            double scale[3];
            modelMatrix.getScale(scale, MSpace::kWorld);
            if (scale[0] != scale[1]) // non uniform scaling across x and y
            {
                if (scale[0] != 0.0)
                    scale2[0] /= scale[0];
                if (scale[1] != 0)
                    scale2[1] /= scale[1];
                const double avs = (scale[0] + scale[1]) * 0.5;
                scale2[0] *= avs;
                scale2[1] *= avs;
            }

			if (primitiveTypeChanged || 
				scale2[0] != m_scale[0] ||
				scale2[1] != m_scale[1] ||
				scale2[2] != m_scale[2])
			{
				CGDiskLightPrimitive::generateData(p_positions, p_indices, scale2);

				m_scale[0] = scale2[0];
				m_scale[1] = scale2[1];
				m_scale[2] = scale2[2];
			}
			m_primitiveType = "disk";
        }
        else if (primitiveType == "cylinder")
        {
            double scale[3];
            modelMatrix.getScale(scale, MSpace::kWorld);
            if (scale[0] != scale[2]) // non uniform scaling across x and y
            {
                if (scale[0] != 0.0)
                    scale2[0] /= scale[0];
                if (scale[2] != 0)
                    scale2[2] /= scale[2];
                const double avs = (scale[0] + scale[2]) * 0.5;
                scale2[0] *= avs;
                scale2[2] *= avs;
            }

			if (primitiveTypeChanged ||
				scale2[0] != m_scale[0] ||
				scale2[1] != m_scale[1] ||
				scale2[2] != m_scale[2])
			{
				CGCylinderPrimitive::generateData(p_positions, p_indices, scale2);

				m_scale[0] = scale2[0];
				m_scale[1] = scale2[1];
				m_scale[2] = scale2[2];
			}
			m_primitiveType = "cylinder";			
		}
        else
		{
			if (primitiveTypeChanged)
			{
				CGQuadLightPrimitive::generateData(p_positions, p_indices, scale2);
				m_primitiveType = "quad";			
			}
		}
	}

    ~CArnoldAreaLightUserData()
    {
    }
};

MHWRender::MPxDrawOverride* CArnoldAreaLightDrawOverride::creator(const MObject& obj)
{
    return new CArnoldAreaLightDrawOverride(obj);
}

CArnoldAreaLightDrawOverride::CArnoldAreaLightDrawOverride(const MObject& obj) :
    MHWRender::MPxDrawOverride(obj, draw)
{

}

CArnoldAreaLightDrawOverride::~CArnoldAreaLightDrawOverride()
{

}

bool CArnoldAreaLightDrawOverride::isBounded(
                                        const MDagPath& objPath,
                                        const MDagPath& cameraPath) const
{
    return true;
}

MBoundingBox CArnoldAreaLightDrawOverride::boundingBox(
                                                const MDagPath& objPath,
                                                const MDagPath& cameraPath) const
{
    MBoundingBox bbox;
    bbox.expand(MPoint(-1.0, -1.0, 0.0));
    bbox.expand(MPoint(1.0, 1.0, -1.0));
    return bbox;
}

bool CArnoldAreaLightDrawOverride::disableInternalBoundingBoxDraw() const
{
    return false;
}

MUserData* CArnoldAreaLightDrawOverride::prepareForDraw(
                                                    const MDagPath& objPath,
                                                    const MDagPath& cameraPath,
                                                    const MHWRender::MFrameContext& frameContext,
                                                    MUserData* oldData)
{
	initializeUserData();

    if (s_isValid)
    {
        CArnoldAreaLightUserData* data = reinterpret_cast<CArnoldAreaLightUserData*>(oldData);
        if (!data)
            data = new CArnoldAreaLightUserData();
        data->update(objPath);
        return data;
    }
    else return 0;
}

MHWRender::DrawAPI CArnoldAreaLightDrawOverride::supportedDrawAPIs() const
{
    return (MHWRender::kAllDevices);
}

// Generate user data for each primitive type
void CArnoldAreaLightDrawOverride::initializeUserData()
{
    if ((s_isInitialized == false))
    {
		s_isInitialized = true;
        s_isValid = true;
    }
}

void CArnoldAreaLightDrawOverride::draw(
                                    const MHWRender::MDrawContext& context,
                                    const MUserData* data)
{
}

void CArnoldAreaLightDrawOverride::addUIDrawables(const MDagPath& objPath,
												MHWRender::MUIDrawManager& drawManager,
												const MHWRender::MFrameContext& frameContext,
												const MUserData* data)
{
    if (s_isValid == false)
        return;

	// Note that we don't need to add in a light type filter
	// since the classification string "drawdb/geometry/light" usage
	// will automatically do this for us.
	//
    const CArnoldAreaLightUserData* userData = reinterpret_cast<const CArnoldAreaLightUserData*>(data);
    if (userData == 0)
        return;

	drawManager.beginDrawable();
	drawManager.setColor( userData->m_wireframeColor );
	drawManager.mesh(MHWRender::MUIDrawManager::kLines, 
		userData->p_positions, NULL, NULL, &userData->p_indices );
	drawManager.endDrawable();
}