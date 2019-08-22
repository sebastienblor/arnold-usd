#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif

#include "ArnoldVolumeGeometryOverride.h"

#include <iostream>
#include <vector>

#include "ViewportUtils.h"
#include "nodes/shape/ArnoldVolume.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MShaderManager.h>
#include <maya/MUIDrawManager.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MUintArray.h>

#if MAYA_API_VERSION >= 201700

MString CArnoldVolumeGeometryOverride::s_wireframeItemName = "volumeBox";
MString CArnoldVolumeGeometryOverride::s_activeWireframeItemName = "active_volumeBox";

static MString colorParameterName_   = "solidColor";

MHWRender::MPxGeometryOverride* CArnoldVolumeGeometryOverride::Creator(const MObject& obj)
{
    return new CArnoldVolumeGeometryOverride(obj);
}

CArnoldVolumeGeometryOverride::CArnoldVolumeGeometryOverride(const MObject& obj) :
    MHWRender::MPxGeometryOverride(obj)
	, m_geometryDirty(true)
{
	m_scale[0] = m_scale[1] = m_scale[2] = 1.0f;
	m_offset[0] = m_offset[1] = m_offset[2] = 0.0f;
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

CArnoldVolumeGeometryOverride::~CArnoldVolumeGeometryOverride()
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

MHWRender::DrawAPI CArnoldVolumeGeometryOverride::supportedDrawAPIs() const
{
    return (MHWRender::kAllDevices);
}

bool CArnoldVolumeGeometryOverride::isIndexingDirty(const MHWRender::MRenderItem &item) 
{ 
	return true; 
}

bool CArnoldVolumeGeometryOverride::isStreamDirty(const MHWRender::MVertexBufferDescriptor &desc) 
{ 
	return true; 
}

void CArnoldVolumeGeometryOverride::updateRenderItems(const MDagPath &path, MHWRender::MRenderItemList& list)
{
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (!renderer)
		return;

	const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
	if (!shaderMgr)
		return;

	// Check for scale and offset
	AtVector mn (
		-1.0f, -1.0f, -1.0f
    );

    AtVector mx (
        1.0f, 1.0f, 1.0f
    );
	MStatus status;
	MFnDependencyNode depNode(path.node(), &status);
    if(MStatus::kSuccess == status)
    {
        CArnoldVolumeShape * shapeNode = dynamic_cast<CArnoldVolumeShape*>(depNode.userNode());
        if( shapeNode != NULL)
        {
            MBoundingBox * bbox = shapeNode->geometry();
            mn.x = (float) bbox->min().x;
            mn.y = (float) bbox->min().y;
            mn.z = (float) bbox->min().z;
               
            mx.x = (float) bbox->max().x;
            mx.y = (float) bbox->max().y;
            mx.z = (float) bbox->max().z;
        }
    }

	float newScale[3] = { mx.x - mn.x, mx.y - mn.y, mx.z - mn.z };
    if (m_geometryDirty ||
		newScale[0] != m_scale[0] || 
		newScale[1] != m_scale[1] || 
		newScale[2] != m_scale[2])
	{
		m_scale[0] = newScale[0];
		m_scale[1] = newScale[1];
		m_scale[2] = newScale[2];
		m_geometryDirty = true;
	}
	if (m_geometryDirty || 
		m_offset[0] != mn.x ||
		m_offset[1] != mn.y ||
		m_offset[2] != mn.z)
	{
		m_offset[0] = mn.x;
		m_offset[1] = mn.y;
		m_offset[2] = mn.z;
		m_geometryDirty = true;
	}

	// If scale == 0 then we should still try and draw something. 
	// We can try to use a different display.

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

	// Get color
	MColor color = MHWRender::MGeometryUtilities::wireframeColor(path);
	m_wireframeColor[0] = color.r;
	m_wireframeColor[1] = color.g;
	m_wireframeColor[2] = color.b;
	m_wireframeColor[3] = color.a;

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
#if MAYA_API_VERSION >= 201800
		wireframeItem->setCompatibleWithMayaInstancer(true);
#endif
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

		// Update the color and shader
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

		// Update the color and shader
		m_activeWireframeShader->setParameter(colorParameterName_, m_wireframeColor);	
		activeWireframeItem->setShader(m_activeWireframeShader);
	}
}

void CArnoldVolumeGeometryOverride::populateGeometry(const MHWRender::MGeometryRequirements &requirements, 
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
		double scale[3] = { m_scale[0], m_scale[1], m_scale[2] };
		double offset[3] = { m_offset[0], m_offset[1], m_offset[2] };
		CGBoxPrimitive::generateData(m_wirePositions, m_wireIndexing, scale, offset, false);
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
		case MHWRender::MGeometry::kNormal:
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

void CArnoldVolumeGeometryOverride::cleanUp() 
{
}

#endif