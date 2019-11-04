#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif
#include "ArnoldLightBlockerGeometryOverride.h"

#include <iostream>
#include <vector>

#include "ViewportUtils.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MShaderManager.h>
#include <maya/MUIDrawManager.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MUintArray.h>

MString CArnoldLightBlockerGeometryOverride::s_wireframeItemName = "wireframeBlocker";
MString CArnoldLightBlockerGeometryOverride::s_activeWireframeItemName = "active_wireframeBlocker";

static MString colorParameterName_   = "solidColor";

MHWRender::MPxGeometryOverride* CArnoldLightBlockerGeometryOverride::Creator(const MObject& obj)
{
    return new CArnoldLightBlockerGeometryOverride(obj);
}

CArnoldLightBlockerGeometryOverride::CArnoldLightBlockerGeometryOverride(const MObject& obj) :
    MHWRender::MPxGeometryOverride(obj)
	, m_primitiveType(6)
	, m_geometryDirty(true)
{
    m_wireframeColor[0] = m_wireframeColor[1] = m_wireframeColor[2] = m_wireframeColor[3];

	// Acquire resources
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (renderer)
	{
		const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
		if (shaderMgr)
		{
			m_wireframeShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidShader);
			m_activeWireframeShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidShader);
		}
	}
}

CArnoldLightBlockerGeometryOverride::~CArnoldLightBlockerGeometryOverride()
{
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (renderer)
	{
		// Release shaders
		const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
		if (shaderMgr)
		{
			if (m_wireframeShader)
			{
				shaderMgr->releaseShader(m_wireframeShader);
				m_wireframeShader = 0;
			}
			if (m_activeWireframeShader)
			{
				shaderMgr->releaseShader(m_activeWireframeShader);
				m_activeWireframeShader = 0;
			}
		}
	}
}

MHWRender::DrawAPI CArnoldLightBlockerGeometryOverride::supportedDrawAPIs() const
{
    return (MHWRender::kAllDevices);
}

bool CArnoldLightBlockerGeometryOverride::isIndexingDirty(const MHWRender::MRenderItem &item) 
{ 
	return true; 
}

bool CArnoldLightBlockerGeometryOverride::isStreamDirty(const MHWRender::MVertexBufferDescriptor &desc) 
{ 
	return true; 
}

void CArnoldLightBlockerGeometryOverride::updateRenderItems(const MDagPath &path, MHWRender::MRenderItemList& list)
{
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (!renderer)
		return;

	const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
	if (!shaderMgr)
		return;

	MFnDependencyNode depNode(path.node());

	// Check for a change in geometry type
	MStatus status;
	short primitiveType = 0;
    MPlug plug = depNode.findPlug("geometryType", true, &status);
    if (status && !plug.isNull())
    {
		primitiveType = plug.asShort();
	}
	m_geometryDirty = (m_primitiveType != primitiveType);	
	m_primitiveType = primitiveType;

	// Disable when not required
	bool needActiveItem = false;
	MHWRender::DisplayStatus displayStatus = MHWRender::MGeometryUtilities::displayStatus(path);
	switch (displayStatus) {
	case MHWRender::kLead:
	case MHWRender::kActive:
	case MHWRender::kHilite:
	case MHWRender::kActiveComponent:
		needActiveItem = true;
		break;
	default:
		break;
	};

	// 1. Add in a dormant wireframe render item
	MHWRender::MRenderItem* wireframeItem = 0;
	int index = list.indexOf(s_wireframeItemName);
	if (index < 0)
	{
		wireframeItem = MHWRender::MRenderItem::Create(
			s_wireframeItemName,
			MHWRender::MRenderItem::DecorationItem,
			MHWRender::MGeometry::kLines);
	
		wireframeItem->setDrawMode(MHWRender::MGeometry::kAll);
		wireframeItem->depthPriority(MHWRender::MRenderItem::sDormantWireDepthPriority);
		wireframeItem->enable(true);
		list.append(wireframeItem);
	}
	else
	{
		wireframeItem = list.itemAt(index);
	}
	if (wireframeItem && m_wireframeShader)
	{
		// Need dormant if not drawing active item
		wireframeItem->enable(!needActiveItem);

		// Update the wireframe color
		if (MHWRender::kDormant == MHWRender::MGeometryUtilities::displayStatus(path))
		{
			m_wireframeColor[0] = 0.75f;
			m_wireframeColor[1] = 0.f;
			m_wireframeColor[2] = 0.f;
			m_wireframeColor[3] = 1.0f;
		}
		else
		{
			MColor color = MHWRender::MGeometryUtilities::wireframeColor(path);
			m_wireframeColor[0] = color.r;
			m_wireframeColor[1] = color.g;
			m_wireframeColor[2] = color.b;
			m_wireframeColor[3] = color.a;
		}

		// Update the color
		m_wireframeShader->setParameter(colorParameterName_, m_wireframeColor);
		wireframeItem->setShader(m_wireframeShader);
	}

	// 2. Add in a active wireframe render item
	// 
	MHWRender::MRenderItem* activeWireframeItem = 0;
	index = list.indexOf(s_activeWireframeItemName);
	if (index < 0)
	{
		activeWireframeItem = MHWRender::MRenderItem::Create(
			s_activeWireframeItemName,
			MHWRender::MRenderItem::DecorationItem,
			MHWRender::MGeometry::kLines);

		activeWireframeItem->setDrawMode(MHWRender::MGeometry::kAll);
		activeWireframeItem->depthPriority(MHWRender::MRenderItem::sActiveLineDepthPriority);
		list.append(activeWireframeItem);
	}
	else
	{
		activeWireframeItem = list.itemAt(index);
	}
	if (activeWireframeItem && m_activeWireframeShader)
	{
		// Enable if need active item
		activeWireframeItem->enable(needActiveItem);

		MColor color = MHWRender::MGeometryUtilities::wireframeColor(path);
		m_wireframeColor[0] = color.r;
		m_wireframeColor[1] = color.g;
		m_wireframeColor[2] = color.b;
		m_wireframeColor[3] = color.a;
		m_activeWireframeShader->setParameter(colorParameterName_, m_wireframeColor);
		activeWireframeItem->setShader(m_activeWireframeShader);
	}
}

void CArnoldLightBlockerGeometryOverride::populateGeometry(const MHWRender::MGeometryRequirements &requirements, 
	const MHWRender::MRenderItemList &renderItems, 
	MHWRender::MGeometry &data)
{
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (!renderer)
		return;

	const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
	if (!shaderMgr)
		return;

	// Build geometry if dirty
	if (m_geometryDirty)
	{
		// Change in primitive type
        if (m_primitiveType == 0) // Box
        {
			double scale[3] = { 1.0, 1.0, 1.0 };
			double offset[3] = { 0.0, 0.0, 0.0 };
			CGBoxPrimitive::generateData(m_wirePositions, m_wireIndexing, scale, offset);
        }
        else if (m_primitiveType == 1) // Sphere
        {
			const double radius = 0.5;
			const unsigned int dimensions = 16;
			CGSpherePrimitive::generateData(m_wirePositions, m_wireIndexing, radius, dimensions);
		}
        else if (m_primitiveType == 2) // Plane
        {
			double scale[3] = { 0.5, 0.5, 0.5 };
			CGQuadPrimitive::generateData(m_wirePositions, m_wireIndexing, scale);
		}
        else if (m_primitiveType == 3) // Cylinder
        {
			double scale[3] = { 0.5, 0.5, 0.5 };
			CGCylinderPrimitive::generateData(m_wirePositions, m_wireIndexing, scale);
		}
		m_geometryDirty = false;
	}

	// Wire vertex data
	MHWRender::MVertexBuffer* wirePositionBuffer = NULL;
	MHWRender::MIndexBuffer*  wireIndexBuffer  = NULL;

	const MHWRender::MVertexBufferDescriptorList& listOfvertexBufforDescriptors = requirements.vertexRequirements();
	const int numberOfVertexRequirements = listOfvertexBufforDescriptors.length();

	MHWRender::MVertexBufferDescriptor vertexBufforDescriptor;
	for (int requirementNumber = 0; requirementNumber < numberOfVertexRequirements; requirementNumber++)
	{
		if (!listOfvertexBufforDescriptors.getDescriptor(requirementNumber, vertexBufforDescriptor))
		{
			continue;
		}

		switch (vertexBufforDescriptor.semantic())
		{
		case MHWRender::MGeometry::kPosition:
			{
				unsigned int totalVerts = m_wirePositions.length();
				if (totalVerts > 0)
				{
					wirePositionBuffer = data.createVertexBuffer(vertexBufforDescriptor);
					if (wirePositionBuffer)
					{
						wirePositionBuffer->update(&m_wirePositions[0], 0, totalVerts, true);
					}
				}
			}
			break;
		default:
			// do nothing for stuff we don't understand
			break;
		}
	}

	// Update indexing for render items
	//
	int numItems = renderItems.length();
	for (int i=0; i<numItems; i++)
	{
		const MHWRender::MRenderItem* item = renderItems.itemAt(i);

		if (!item)
		{
			continue;
		}

		if (item->name() == s_wireframeItemName ||
			item->name() == s_activeWireframeItemName)
		{
			unsigned int indexCount = m_wireIndexing.length();
			if (indexCount)
			{
				wireIndexBuffer = data.createIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);
				if (wireIndexBuffer)
				{
					wireIndexBuffer->update(&m_wireIndexing[0], 0, indexCount, true);
					item->associateWithIndexBuffer(wireIndexBuffer);
				}
			}
		}
	}
}

void CArnoldLightBlockerGeometryOverride::cleanUp() 
{
}
