#include "ArnoldSkyDomeLightGeometryOverride.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MTransformationMatrix.h>
#include <maya/M3dView.h>
#include <maya/MUIDrawManager.h>
#include <maya/MShaderManager.h>
#include <maya/MTextureManager.h>
#include <maya/MGlobal.h>

#include <iostream>

#include "ViewportUtils.h"

#include <ai.h>

// Static CPU data buffers for reuse
MFloatVectorArray CArnoldSkyDomeLightGeometryOverride::s_filledPositions;
MFloatArray CArnoldSkyDomeLightGeometryOverride::s_filledUvs[3];
MUintArray CArnoldSkyDomeLightGeometryOverride::s_filledIndexing;
MFloatVectorArray CArnoldSkyDomeLightGeometryOverride::s_wirePositions;
MUintArray CArnoldSkyDomeLightGeometryOverride::s_wireIndexing;
unsigned int CArnoldSkyDomeLightGeometryOverride::s_divisions[2] = { 20, 20 };

MString CArnoldSkyDomeLightGeometryOverride::s_wireframeItemName = "wireframeSkyDome";
MString CArnoldSkyDomeLightGeometryOverride::s_texturedItemName	= "texturedSkyDome";

MString colorParameterName_   = "solidColor";

MHWRender::MPxGeometryOverride* 
	CArnoldSkyDomeLightGeometryOverride::Creator(const MObject& obj)
{
	return new CArnoldSkyDomeLightGeometryOverride(obj);
}

CArnoldSkyDomeLightGeometryOverride::CArnoldSkyDomeLightGeometryOverride(const MObject& obj) 
	: MHWRender::MPxGeometryOverride(obj)
	, m_format(0)
	, m_radius(1000)
	, m_wireframeShader(0)
	, m_texturedShader(0)
	, p_skydomeNode(0)
{
	m_wireframeColor[0] = m_wireframeColor[1] = m_wireframeColor[2] = m_wireframeColor[3] = 1.0f;

	// get the skydome object from the MObject
	MStatus status;
	MFnDependencyNode node(obj, &status);
	if (status)
	{
		p_skydomeNode = dynamic_cast<CArnoldSkyDomeLightNode*>(node.userNode());
	}

	// Acquire resources
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (renderer)
	{
		const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
		if (!shaderMgr)
			return;
		m_wireframeShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidShader);
		m_texturedShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidTextureShader);
	}
}

CArnoldSkyDomeLightGeometryOverride::~CArnoldSkyDomeLightGeometryOverride()
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
			if (m_texturedShader)
			{
				shaderMgr->releaseShader(m_texturedShader);
				m_texturedShader = 0;
			}
		}

		// Release texture
		MHWRender::MTextureManager* textureManager = renderer->getTextureManager();
		if (textureManager)
		{
		}
	}
}

MHWRender::DrawAPI CArnoldSkyDomeLightGeometryOverride::supportedDrawAPIs() const
{
	return (MHWRender::kAllDevices);
}

void CArnoldSkyDomeLightGeometryOverride::updateDG()
{

}

bool CArnoldSkyDomeLightGeometryOverride::isIndexingDirty(const MHWRender::MRenderItem &item) 
{ 
	// Indexing data never needs to be updated
	return true; 
}

bool CArnoldSkyDomeLightGeometryOverride::isStreamDirty(const MHWRender::MVertexBufferDescriptor &desc) 
{ 
	// Stream data never needs to be updated
	return true; 
}

void CArnoldSkyDomeLightGeometryOverride::updateRenderItems(const MDagPath &path, MHWRender::MRenderItemList& list)
{
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (!renderer)
		return;

	const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
	if (!shaderMgr)
		return;

	MHWRender::MTextureManager* textureManager = renderer->getTextureManager();
	if (!textureManager)
		return;

	MFnDependencyNode depNode(path.node());

	// Get format
	m_format = 0;
	MStatus status;
	MPlug plug = depNode.findPlug("format", &status);
	if (status && !plug.isNull())
		m_format = plug.asInt();

	// zeroing the translation component out
	path.inclusiveMatrix().get(m_modelMatrix);
	m_modelMatrix[3][0] = 0.0f;
	m_modelMatrix[3][1] = 0.0f;
	m_modelMatrix[3][2] = 0.0f;
	MMatrix modelMatrix(m_modelMatrix);

	// Apply scale
	m_radius = 1.0;
	plug = depNode.findPlug("skyRadius", &status);
	if (status && !plug.isNull())
		m_radius = plug.asFloat();
	if (m_radius != 1.0)
	{
		MTransformationMatrix transformMatrix(modelMatrix);
		double scale[3] = { m_radius, m_radius, m_radius };
		transformMatrix.addScale( scale, MSpace::kWorld );
		modelMatrix = transformMatrix.asMatrix();
	}

	// Add in a wireframe render item
	MHWRender::MRenderItem* wireframeItem = 0;
	int index = list.indexOf(s_wireframeItemName);
	if (index < 0)
	{
		wireframeItem = MHWRender::MRenderItem::Create(
			s_wireframeItemName,
			MHWRender::MRenderItem::DecorationItem,
			MHWRender::MGeometry::kLines);
		wireframeItem->setDrawMode(MHWRender::MGeometry::kWireframe);
		wireframeItem->depthPriority(MHWRender::MRenderItem::sActiveWireDepthPriority);
		wireframeItem->setExcludedFromPostEffects(true);
		wireframeItem->enable(true);
		list.append(wireframeItem);
	}
	else
	{
		wireframeItem = list.itemAt(index);
	}
	if (wireframeItem && m_wireframeShader)
	{
		// Update the wireframe color
		MColor color = MHWRender::MGeometryUtilities::wireframeColor(path);
		m_wireframeColor[0] = color.r;
		m_wireframeColor[1] = color.g;
		m_wireframeColor[2] = color.b;
		m_wireframeColor[3] = color.a;

		// Update the color
		m_wireframeShader->setParameter(colorParameterName_, m_wireframeColor);
		wireframeItem->setShader(m_wireframeShader, &s_wireframeItemName);

		// Update the matrix
		wireframeItem->setMatrix(&modelMatrix);
	}

	// Add in a textured render item
	MHWRender::MRenderItem* texturedItem = NULL;

	index = list.indexOf(s_texturedItemName);
	if (index < 0)
	{
		texturedItem = MHWRender::MRenderItem::Create(
			s_texturedItemName,
			MHWRender::MRenderItem::MaterialSceneItem,
			MHWRender::MGeometry::kTriangles);
		texturedItem->setDrawMode((MHWRender::MGeometry::DrawMode)
			(MHWRender::MGeometry::kShaded | MHWRender::MGeometry::kTextured));		
		texturedItem->enable(true);
		texturedItem->setExcludedFromPostEffects(true);
		texturedItem->castsShadows(false);
		texturedItem->receivesShadows(false);
		list.append(texturedItem);
	}
	else
	{
		texturedItem = list.itemAt(index);
	}

	if (texturedItem)
	{
		MHWRender::MTexture* texture = 0;

		// Update the matrix
		texturedItem->setMatrix(&modelMatrix);

		// Performance issue to recompute the shader program even if not used ?
		MHWRender::MShaderInstance *shaderInst = m_texturedShader;
		/* if (mIblShape->cmEnabled_)
		{
		if(mIblShape->inputColorSpace_ != mIblShape->inputColorSpaceLast_
		||
		mIblShape->workingColorSpace_ != mIblShape->workingColorSpaceLast_)
		{
		mIblSurfaceShaderCM 
		= mIblSurfaceShader->createShaderInstanceWithColorManagementFragment(mIblShape->inputColorSpace_);
		mIblShape->inputColorSpaceLast_ = mIblShape->inputColorSpace_;
		mIblShape->workingColorSpaceLast_ = mIblShape->workingColorSpace_;
		}
		assert(mIblSurfaceShaderCM != NULL);
		if (mIblSurfaceShaderCM)
		{
		shaderInst = mIblSurfaceShaderCM;
		}
		}
		*/

		if (m_texturedShader)
		{
			// Get texture
			MHWRender::MTexture* texture = NULL;

			// Dummy fallback texture.
#if 1
			unsigned int targetWidth = 128;
			unsigned int targetHeight = 128;
			unsigned char* textureData = new unsigned char[4*targetWidth*targetHeight];
			if (textureData)
			{
				for (unsigned int y = 0; y < targetHeight; y++)
				{
					unsigned char* pPixel = textureData + (y * targetWidth * 4);
					for (unsigned int x = 0; x < targetWidth; x++)
					{
						bool checker = (((x >> 5) & 1) ^ ((y >> 5) & 1)) != 0;
						*pPixel++ = checker ? 255 : 0;
						*pPixel++ = 0;
						*pPixel++ = 0;
						*pPixel++ = 255;
					}
				}
			}
			MHWRender::MTextureDescription colorTextureDesc;
			colorTextureDesc.fWidth = targetWidth;
			colorTextureDesc.fHeight = targetHeight;
			colorTextureDesc.fDepth = 1;
			colorTextureDesc.fBytesPerRow = 4*targetWidth;
			colorTextureDesc.fBytesPerSlice = colorTextureDesc.fBytesPerRow*targetHeight;
			texture = textureManager->acquireTexture("", colorTextureDesc, textureData);
			delete [] textureData;
#endif
#if 0
			texture = textureManager->acquireTexture("c:/work/data/swatches.jpg");
			if (texture)
			{
				MGlobal::displayInfo("Acquire dummy texture");
				MStatus status = textureManager->saveTexture(texture, "c:/Work/Data/dummy1.jpg"); // To disk
				if (status == MStatus::kSuccess)
					MGlobal::displayInfo("Save dummy texture");
			}
#endif
			// Set texture
			MHWRender::MTextureAssignment textureAssignment;
			textureAssignment.texture = texture;
			//texture->setHasAlpha(true);
			bool assignedTexture = false;
			MStatus status = shaderInst->setParameter("map", textureAssignment);
			if (MStatus::kSuccess == status)
				assignedTexture = true;
			//MGlobal::displayInfo(status == MStatus::kSuccess ? "Assigned map" : "Failed map");
/*
			// Set transparency
			float alphaThreshold = 0.0f;       
			status = shaderInst->setParameter("alphaThreshold", &alphaThreshold);
			MGlobal::displayInfo(status == MStatus::kSuccess ? "Assigned alphaThreshold" : "Failed alphaThreshold" );
			float defaultColor[4] = { 1.0f, 0.2f, 5.0f, 0.3f };
			status = shaderInst->setParameter("defaultColor", defaultColor);
			MGlobal::displayInfo(status == MStatus::kSuccess ? "Assigned defaultColor" : "Failed defaultColor" );
			// Set outputMode 4 = Multiply defaultColor * texture
			status = shaderInst->setParameter("outputMode", 4);
			MGlobal::displayInfo(status == MStatus::kSuccess ? "Assigned outputMode" : "Failed outputMode" );

			shaderInst->setParameter("alphaGain", 0.0f);
			shaderInst->setParameter("alphaOffset", 0.5f);
			status = shaderInst->setParameter("outputMode", 1);
*/
			//shaderInst->setIsTransparent(true);
			
			texturedItem->setShader(shaderInst, &s_texturedItemName);
		}

		// Release reference to texture, the shader will hold a reference to keep
		// it alive for shading
		if (texture)
		{
			//	textureManager->releaseTexture(texture);
		} 
	}
}

static AtVector SphereVertex(float phi, float theta)
{
	AtVector ret;
	ret.y = cosf(theta);
	float t = sinf(theta);
	ret.x = t * sinf(phi);
	ret.z = t * cosf(phi);
	return ret;
}

void CArnoldSkyDomeLightGeometryOverride::createSkyDomeGeometry(unsigned int divisions[2],
																float radius)
{
	// Allocate memory
	unsigned int numIndices = divisions[0] * divisions[1] * 6;
	unsigned int divisionsX1 = divisions[0] + 1;
	unsigned int divisionsY1 = divisions[1] + 1;
	unsigned int numVertices = divisionsX1 * divisionsY1;
	s_filledPositions.setLength(numVertices);
	s_filledUvs[0].setLength(numVertices*2);
	s_filledUvs[1].setLength(numVertices*2);
	s_filledUvs[2].setLength(numVertices*2);
	s_filledIndexing.setLength(numIndices);

	unsigned int numVertices_wire = divisions[0] * divisions[1] * 8;
	unsigned int numIndicies_wire = divisions[0] * divisions[1] * 8;
	s_wirePositions.setLength(numVertices_wire);
	s_wireIndexing.setLength(numIndicies_wire);

	AtVector dir;
	float u, v, u2, v2, u3, v3;

	// Create filled data
	for (unsigned int x = 0; x < divisionsX1; ++x)
	{
		const float phi = (float)AI_PITIMES2 * (float)x / (float)divisions[0];

		for (unsigned int y = 0; y < divisionsY1; ++y)
		{         
			const float theta = (float)AI_PI * (float)y / (float)divisions[1];
			dir = SphereVertex(phi, theta);            

			AiMappingMirroredBall(&dir, &u, &v); // Mirrored Ball
			AiMappingAngularMap(&dir, &u2, &v2);  // Angular
			AiMappingLatLong(&dir, &u3, &v3);  // Latlong

			const int id = x + y * divisionsX1;
			s_filledUvs[0][id*2] = u;
			s_filledUvs[0][id*2+1] = v;
			//MString outstr("uv = ");
			//outstr += u;
			//outstr += ", ";
			//outstr += v;
			//MGlobal::displayInfo(outstr);

			s_filledUvs[1][id*2] = u2;
			s_filledUvs[1][id*2+1] = v2;
			s_filledUvs[2][id*2] = u3;
			s_filledUvs[2][id*2+1] = v3;

			s_filledPositions[id] = MFloatVector( dir.x*radius,
				dir.y*radius,
				dir.z*radius);
		}
	}

	int indexCounter = 0;
	for (unsigned int x = 0; x < (unsigned int)divisions[0]; ++x)
	{
		const int x1 = x + 1;
		for (unsigned int y = 0; y < (unsigned int)divisions[1]; ++y)
		{
			const int y1 = y + 1;
			s_filledIndexing[indexCounter++] = x + y * divisionsX1;
			s_filledIndexing[indexCounter++] = x1 + y * divisionsX1;
			s_filledIndexing[indexCounter++] = x + y1 * divisionsX1;

			s_filledIndexing[indexCounter++] = x1 + y * divisionsX1;
			s_filledIndexing[indexCounter++] = x1 + y1 * divisionsX1;
			s_filledIndexing[indexCounter++] = x + y1 * divisionsX1;
		}  
	}

	// Create wireframe data
	unsigned int index = 0;
	for (unsigned int x = 0; x < divisions[0]; ++x)
	{
		float phiB = (float)AI_PITIMES2 * (float)x / (float)divisions[0];
		float phiE = (float)AI_PITIMES2 * (float)(x + 1) / (float)divisions[1];
		for (unsigned int y = 0; y < divisions[1]; ++y)
		{
			float thetaB = (float)AI_PI * (float)y / (float)divisions[0];
			float thetaE = (float)AI_PI * (float)(y + 1) / (float)divisions[1];

			AtVector dir = SphereVertex(phiB, thetaB);
			s_wirePositions[index++] = MFloatVector( dir.x*radius,
				dir.y*radius,
				dir.z*radius);
			dir = SphereVertex(phiE, thetaB);
			s_wirePositions[index++] = MFloatVector( dir.x*radius,
				dir.y*radius,
				dir.z*radius);

			dir = SphereVertex(phiB, thetaE);
			s_wirePositions[index++] = MFloatVector( dir.x*radius,
				dir.y*radius,
				dir.z*radius);
			dir = SphereVertex(phiE, thetaE);
			s_wirePositions[index++] = MFloatVector( dir.x*radius,
				dir.y*radius,
				dir.z*radius);

			dir = SphereVertex(phiB, thetaB);
			s_wirePositions[index++] = MFloatVector( dir.x*radius,
				dir.y*radius,
				dir.z*radius);
			dir = SphereVertex(phiB, thetaE);
			s_wirePositions[index++] = MFloatVector( dir.x*radius,
				dir.y*radius,
				dir.z*radius);

			dir = SphereVertex(phiE, thetaB);
			s_wirePositions[index++] = MFloatVector( dir.x*radius,
				dir.y*radius,
				dir.z*radius);
			dir = SphereVertex(phiE, thetaE);
			s_wirePositions[index++] = MFloatVector( dir.x*radius,
				dir.y*radius,
				dir.z*radius);
		}
	}
	for (unsigned int x = 0; x < divisions[0]*divisions[1]*8; ++x)
	{
		s_wireIndexing[x] = x;
	}
}

void CArnoldSkyDomeLightGeometryOverride::populateGeometry(const MHWRender::MGeometryRequirements &requirements, 
	const MHWRender::MRenderItemList &renderItems, 
	MHWRender::MGeometry &data)
{
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (!renderer)
		return;

	const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
	if (!shaderMgr)
		return;

	// Build geometry
	// Already created so don't create again
	if (s_filledPositions.length() == 0)
	{	
		unsigned int dim[2] = { 8, 8 };
		createSkyDomeGeometry(dim, 1.0f);
	}

	// Filled vertex data
	MHWRender::MVertexBuffer* filledPositionBuffer = NULL;
	MHWRender::MVertexBuffer* filledUvBuffer         = NULL;
	MHWRender::MIndexBuffer*  filledIndexBuffer = NULL;
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
				// Update position buffer for textured render item
				if (vertexBufforDescriptor.name() == s_texturedItemName)
				{		
					unsigned int totalVerts = s_filledPositions.length();
					if (totalVerts > 0)
					{
						filledPositionBuffer = data.createVertexBuffer(vertexBufforDescriptor);
						if (filledPositionBuffer)
						{
							filledPositionBuffer->update(&s_filledPositions[0], 0, totalVerts, true);
						}
					}
				}
				// Update position buffer for wireframe render item
				else if (vertexBufforDescriptor.name() == s_wireframeItemName)
				{
					unsigned int totalVerts = s_wirePositions.length();
					if (totalVerts > 0)
					{
						wirePositionBuffer = data.createVertexBuffer(vertexBufforDescriptor);
						if (wirePositionBuffer)
						{
							wirePositionBuffer->update(&s_wirePositions[0], 0, totalVerts, true);
						}
					}
				}
			}
			break;
		case MHWRender::MGeometry::kTexture:
			{
				// Update texture coordinate buffer for textured render item
				if (vertexBufforDescriptor.name() == s_texturedItemName)
				{	
					// Get the set of uvs based on the format
					unsigned int numUvs = s_filledUvs[m_format].length() / 2;
					if (numUvs > 0)
					{
						filledUvBuffer = data.createVertexBuffer(vertexBufforDescriptor);
						if (filledUvBuffer)
						{
							MStatus status = filledUvBuffer->update(&s_filledUvs[m_format], 0, numUvs, true);
							if (status == MStatus::kSuccess)
							{
								//MGlobal::displayInfo("Loaded UVS");
							}
						}
					}
				}
			}
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

		MString renderItemName = item->name();
		if (item->name() == s_texturedItemName)
		{
			unsigned int indexCount = s_filledIndexing.length();
			if (indexCount)
			{
				filledIndexBuffer = data.createIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);
				if (filledIndexBuffer)
				{
					filledIndexBuffer->update(&s_filledIndexing[0], 0, indexCount, true);
					item->associateWithIndexBuffer(filledIndexBuffer);
				}
			}
		}
		else if (item->name() == s_wireframeItemName)
		{
			unsigned int indexCount = s_wireIndexing.length();
			if (indexCount)
			{
				wireIndexBuffer = data.createIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);
				if (wireIndexBuffer)
				{
					wireIndexBuffer->update(&s_wireIndexing[0], 0, indexCount, true);
					item->associateWithIndexBuffer(wireIndexBuffer);
				}
			}
		}
	}
}

void CArnoldSkyDomeLightGeometryOverride::cleanUp() 
{
}

