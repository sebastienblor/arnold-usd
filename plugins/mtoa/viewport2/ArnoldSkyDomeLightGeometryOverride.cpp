#include "ArnoldSkyDomeLightGeometryOverride.h"

#include <maya/MHWGeometryUtilities.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MTransformationMatrix.h>
#include <maya/M3dView.h>
#include <maya/MUIDrawManager.h>
#include <maya/MShaderManager.h>
#include <maya/MTextureManager.h>
#include <maya/MStateManager.h>
#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>

#include <iostream>

#include "ViewportUtils.h"

#include <ai.h>

// Static CPU data buffers for reuse
MFloatVectorArray CArnoldSkyDomeLightGeometryOverride::s_filledPositions;
MFloatArray CArnoldSkyDomeLightGeometryOverride::s_filledUvs[3];
MUintArray CArnoldSkyDomeLightGeometryOverride::s_filledIndexing;
MFloatVectorArray CArnoldSkyDomeLightGeometryOverride::s_wirePositions;
MUintArray CArnoldSkyDomeLightGeometryOverride::s_wireIndexing;
unsigned int CArnoldSkyDomeLightGeometryOverride::s_divisions[2] = { 0, 0 };

// Static depth stencil and rasterizer states
const MHWRender::MDepthStencilState* s_oldDepthStencilState = 0;
const MHWRender::MRasterizerState* s_oldRasterizerState = 0;

MString CArnoldSkyDomeLightGeometryOverride::s_wireframeItemName = "wireframeSkyDome";
MString CArnoldSkyDomeLightGeometryOverride::s_activeWireframeItemName = "active_wireframeSkyDome";
MString CArnoldSkyDomeLightGeometryOverride::s_texturedItemName	= "texturedSkyDome";

MString colorParameterName_   = "solidColor";

// User data to pass for pre and post shader state setup / cleanup
struct CArnoldSkyDomeLighUserData : public MUserData
{
	CArnoldSkyDomeLighUserData()
		: MUserData(false /*deleteAfterUse*/)
		, m_depthStencilState(0)
		, m_rasterizerState(0)
	{}

	~CArnoldSkyDomeLighUserData()
	{
		m_depthStencilState = 0;
		m_rasterizerState = 0;
	}

	const MHWRender::MDepthStencilState*	m_depthStencilState;
	const MHWRender::MRasterizerState*		m_rasterizerState;
};

MHWRender::MPxGeometryOverride* 
	CArnoldSkyDomeLightGeometryOverride::Creator(const MObject& obj)
{
	return new CArnoldSkyDomeLightGeometryOverride(obj);
}

CArnoldSkyDomeLightGeometryOverride::CArnoldSkyDomeLightGeometryOverride(const MObject& obj) 
	: MHWRender::MPxGeometryOverride(obj)
	, m_format(0)
	, m_radius(0)
	, m_wireframeShader(0)
	, m_activeWireframeShader(0)
	, m_texturedShader(0)
	, p_skydomeNode(0)
	, m_depthStencilState(0)
	, m_cullNoneState(0)
	, m_cullBackState(0)
	, m_cullFrontState(0)
	, m_geometryDirty(true)
	, m_flipVData(true)
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
		if (shaderMgr)
		{
			m_wireframeShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidShader,
				preDrawCallback, postDrawCallback);
			m_activeWireframeShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidShader,
				preDrawCallback, postDrawCallback);
			m_texturedShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidTextureShader,
				preDrawCallback, postDrawCallback);
		}

		createDisplayStates();
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
			if (m_activeWireframeShader)
			{
				shaderMgr->releaseShader(m_activeWireframeShader);
				m_activeWireframeShader = 0;
			}
			if (m_texturedShader)
			{
				shaderMgr->releaseShader(m_texturedShader);
				m_texturedShader = 0;
			}
		}

		destroyDisplayStates();
	}
}

MHWRender::DrawAPI CArnoldSkyDomeLightGeometryOverride::supportedDrawAPIs() const
{
	return (MHWRender::kAllDevices);
}

// Create state objects 
void CArnoldSkyDomeLightGeometryOverride::createDisplayStates()
{
	if (!m_depthStencilState)
	{
		MHWRender::MDepthStencilStateDesc desc;
		desc.depthWriteEnable = false;
		m_depthStencilState = MHWRender::MStateManager::acquireDepthStencilState(desc);
	}
	if (!m_cullNoneState)
	{
		MHWRender::MRasterizerStateDesc desc;
		desc.cullMode =  MHWRender::MRasterizerState::kCullNone;
		//desc.fillMode = MHWRender::MRasterizerState::kFillWireFrame;
		m_cullNoneState = MHWRender::MStateManager::acquireRasterizerState(desc);
	}	
	if (!m_cullBackState)
	{
		MHWRender::MRasterizerStateDesc desc;
		desc.cullMode =  MHWRender::MRasterizerState::kCullBack;
		//desc.fillMode = MHWRender::MRasterizerState::kFillWireFrame;
		m_cullBackState = MHWRender::MStateManager::acquireRasterizerState(desc);
	}
	if (!m_cullFrontState)
	{
		MHWRender::MRasterizerStateDesc desc;
		desc.cullMode =  MHWRender::MRasterizerState::kCullFront;
		//desc.fillMode = MHWRender::MRasterizerState::kFillWireFrame;
		m_cullFrontState = MHWRender::MStateManager::acquireRasterizerState(desc);
	}
}

// destroy state objects
void CArnoldSkyDomeLightGeometryOverride::destroyDisplayStates()
{
	if (m_depthStencilState)
	{
		MHWRender::MStateManager::releaseDepthStencilState(m_depthStencilState);
		m_depthStencilState = 0;
	}
	if (m_cullNoneState)
	{
		MHWRender::MStateManager::releaseRasterizerState(m_cullNoneState);
		m_cullNoneState = 0;
	}	
	if (m_cullBackState)
	{
		MHWRender::MStateManager::releaseRasterizerState(m_cullBackState);
		m_cullBackState = 0;
	}
	if (m_cullFrontState)
	{
		MHWRender::MStateManager::releaseRasterizerState(m_cullFrontState);
		m_cullFrontState = 0;
	}
}

void CArnoldSkyDomeLightGeometryOverride::preDrawCallback(
	MHWRender::MDrawContext&			context,
	const MHWRender::MRenderItemList&	renderItemList,
	MHWRender::MShaderInstance*			shaderInstance)
{
	// Set forceOpaque flag to true if we are drawing a non-PE pattern pass
	// This pass draws an alpha mask to choose which part of the target
	// is not affected by post-FX.
	const MHWRender::MPassContext& passCtx = context.getPassContext();
	const MStringArray& passSem = passCtx.passSemantics();
	for (unsigned int i=0; i<passSem.length(); i++)
	{
		const MString& sem = passSem[i];
		if (sem == MHWRender::MPassContext::kNonPEPatternPassSemantic)
		{
			//shaderInstance->setParameter(sForceOpaque, true);
			break;
		}
	}

	// Override the default 
	const MHWRender::MRenderItem* item = renderItemList.itemAt(0);
	if (item)
	{
		CArnoldSkyDomeLighUserData* userData =
			dynamic_cast<CArnoldSkyDomeLighUserData*>(item->customData());
		if (userData)
		{
			MHWRender::MStateManager* stateMgr = context.getStateManager();

			// Override depth stencil state
			if (userData->m_depthStencilState)
			{
				s_oldDepthStencilState = stateMgr->getDepthStencilState();
				stateMgr->setDepthStencilState(userData->m_depthStencilState);
			}

			// Override rasterizer state
			if (userData->m_rasterizerState)
			{
				s_oldRasterizerState = stateMgr->getRasterizerState();
				stateMgr->setRasterizerState(userData->m_rasterizerState);
			}
		}
	}
}

/*!
	Post-draw particle render items callback
*/
void CArnoldSkyDomeLightGeometryOverride::postDrawCallback(
	MHWRender::MDrawContext&			context,
	const MHWRender::MRenderItemList&	renderItemList,
	MHWRender::MShaderInstance*			shaderInstance)
{
	// See the comments above in pre-draw callback. Restore settings.
	const MHWRender::MPassContext& passCtx = context.getPassContext();
	const MStringArray& passSem = passCtx.passSemantics();
	for (unsigned int i=0; i<passSem.length(); i++)
	{
		const MString& sem = passSem[i];
		if (sem == MHWRender::MPassContext::kNonPEPatternPassSemantic)
		{
			//shaderInstance->setParameter(sForceOpaque, false);
			break;
		}
	};

	// Restore the depth stencil state
	if (s_oldDepthStencilState)
	{
		MHWRender::MStateManager* stateMgr = context.getStateManager();
		stateMgr->setDepthStencilState(s_oldDepthStencilState);
		s_oldDepthStencilState = 0;
	}

	// Restore the rasterizer state if any
	if (s_oldRasterizerState)
	{
		MHWRender::MStateManager* stateMgr = context.getStateManager();
		stateMgr->setRasterizerState(s_oldRasterizerState);
		s_oldRasterizerState = 0;
	}
}

void CArnoldSkyDomeLightGeometryOverride::updateDG()
{

}

bool CArnoldSkyDomeLightGeometryOverride::isIndexingDirty(const MHWRender::MRenderItem &item) 
{ 
	// Indexing data never needs to be updated
	return false; 
}

bool CArnoldSkyDomeLightGeometryOverride::isStreamDirty(const MHWRender::MVertexBufferDescriptor &desc) 
{ 
	// Stream data never needs to be updated (to add in)
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

	// Cache format for uv geometry update
	m_format = 0;
	MStatus status;
	MPlug plug = depNode.findPlug("format", &status);
	if (status && !plug.isNull())
		m_format = plug.asInt();

	// Cache scale value for position geometry update
	// Currently need a rebuild if radius changed
	plug = depNode.findPlug("skyRadius", &status);
	if (status && !plug.isNull())
	{
		float newRadius = plug.asFloat();
		if (newRadius != m_radius)
		{
			m_radius = newRadius;
			m_geometryDirty = true;
		}
	}

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
	
		wireframeItem->setDrawMode(MHWRender::MGeometry::kWireframe);
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
			m_wireframeColor[3] = 0.2f;
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

		// Set custom data for state overrides
		CArnoldSkyDomeLighUserData* userData =
			dynamic_cast<CArnoldSkyDomeLighUserData*>(wireframeItem->customData());
		if (!userData)
		{
			userData = new CArnoldSkyDomeLighUserData();
		}
		if (userData)
		{
			userData->m_depthStencilState = 0; // m_depthStencilState;
			userData->m_rasterizerState = 0; 
		}
		wireframeItem->setCustomData(userData);
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

		// Set custom data for state overrides
		CArnoldSkyDomeLighUserData* userData =
			dynamic_cast<CArnoldSkyDomeLighUserData*>(activeWireframeItem->customData());
		if (!userData)
		{
			userData = new CArnoldSkyDomeLighUserData();
		}
		if (userData)
		{
			userData->m_depthStencilState = 0; //m_depthStencilState;
			userData->m_rasterizerState = 0; 
		}
		activeWireframeItem->setCustomData(userData);
	}

	// 3. Add in a textured render item
	MHWRender::MRenderItem* texturedItem = NULL;

	index = list.indexOf(s_texturedItemName);
	if (index < 0)
	{
		texturedItem = MHWRender::MRenderItem::Create(
			s_texturedItemName,
			// Note: we don't want this to be treated like a material item
			MHWRender::MRenderItem::DecorationItem, 
			MHWRender::MGeometry::kTriangles);
		list.append(texturedItem);
	}
	else
	{
		texturedItem = list.itemAt(index);
	}

	if (texturedItem)
	{
		texturedItem->setDrawMode((MHWRender::MGeometry::DrawMode)
			(MHWRender::MGeometry::kShaded | MHWRender::MGeometry::kTextured));	
		texturedItem->depthPriority(MHWRender::MRenderItem::sDormantFilledDepthPriority);
		texturedItem->enable(true);

		// Set custom data for state overrides
		CArnoldSkyDomeLighUserData* userData =
			dynamic_cast<CArnoldSkyDomeLighUserData*>(texturedItem->customData());
		if (!userData)
		{
			userData = new CArnoldSkyDomeLighUserData();
		}
		// Set depth stencil state
		//
		userData->m_depthStencilState = m_depthStencilState;
		
		// Set culling state
		int facing = 0;
		MPlug facingPlug  = depNode.findPlug("skyFacing");
		facingPlug.getValue(facing);
		if (facing == 0)
			userData->m_rasterizerState = m_cullBackState;
		else if (facing == 1)
			userData->m_rasterizerState = m_cullFrontState;
		else 
		{
			// This is not quite correct since we want
			// to be able to turn on depth write still
			// to allow picking through the shaded item.
			// The VP1 implementation draws back and then front
			// to achive this in order. TBD how to do this in VP2.
			// Mostly matters when the skydome is made semi-transparent.
			// Maybe can lower the selection priority instead.
			userData->m_rasterizerState = 0;
			userData->m_depthStencilState = 0;
		}
		texturedItem->setCustomData(userData);

		MHWRender::MTexture* texture = 0;

		MHWRender::MShaderInstance *shaderInst = m_texturedShader;

		if (m_texturedShader)
		{
			// Color effects
			float alphaGain = 1.0f;
			float alphaOffset = 0.0f;
			float colorGain[3] = { 1.0f, 1.0f, 1.0f };
			float colorOffset[3] = { 0.0f, 0.0f, 0.0f };
			bool invert = false;

			// Get texture
			MHWRender::MTexture* texture = 0;

			// Examine the node connected to the "color" attribute of the skydome node.
			// If one exists then acqure a texture by plug. If it's a file texture
			// we should get a direct read, otherwise a bake will occur.
			//
			unsigned int colorBakeSize = 256; 
			if (p_skydomeNode)
			{
				colorBakeSize = p_skydomeNode->NumSampleBase();
			}
			bool useUnconnectedColor = true;
			MObject connectedObject = MObject::kNullObj;
			MPlug plug = depNode.findPlug("color");
			if (!texture)
			{
				if (!plug.isNull())
				{
					MPlugArray plugArray;
					if (plug.connectedTo(plugArray, true, false) && plugArray.length() == 1)
					{
						// Get texture from plug
						// TODO : Do not require a V-flip for procedurals. Need to check this.
						//
						const MPlug& connectedPlug = plugArray[0];
						connectedObject = connectedPlug.node();
						if (connectedObject.hasFn( MFn::kFileTexture))
						{
							MString fileTextureName;
							MRenderUtil::exactFileTextureName(connectedObject, fileTextureName);
							if (fileTextureName.length())
							{
								texture = textureManager->acquireTexture(fileTextureName);
							}
							// Need v-flip for file textures 
							m_flipVData = true;
						}
						else
						{
							texture = textureManager->acquireTexture("", connectedPlug, colorBakeSize, colorBakeSize);
							// Don't need v-flip for file textures 
							m_flipVData = false;
						}
						if (texture)
						{
							useUnconnectedColor = false;
						}
					}
				}
			}

			// If no texture found then get unconnected plug value and make a 1x1 texture of
			// constant color using it.
			if (useUnconnectedColor)
			{
				float r = 0.5f;
				float g = 0.5f;
				float b = 0.5f;
				if (!plug.isNull())
				{
					plug.child(0).getValue(r);
					plug.child(1).getValue(g);
					plug.child(2).getValue(b);
				}

				unsigned char texData[4];
				texData[0] = (unsigned char)(255 * r);
				texData[1] = (unsigned char)(255 * g);
				texData[2] = (unsigned char)(255 * b);
				texData[3] = 255;
				MHWRender::MTextureDescription desc;
				desc.setToDefault2DTexture();
				desc.fWidth = 1;
				desc.fHeight = 1;
				desc.fFormat = MHWRender::kR8G8B8A8_UNORM;
				texture = textureManager->acquireTexture("", desc, texData);
			}
			else
			{
				// If any of these attributes exist use them
				if (connectedObject != MObject::kNullObj)
				{
					MFnDependencyNode depNode(connectedObject);
					MPlug plug = depNode.findPlug("colorGain");
					if (!plug.isNull())
					{
						plug.child(0).getValue(colorGain[0]);
						plug.child(1).getValue(colorGain[1]);
						plug.child(2).getValue(colorGain[2]);
					}
					MPlug plug2 = depNode.findPlug("colorOffset");
					if (!plug2.isNull())
					{
						plug2.child(0).getValue(colorOffset[0]);
						plug2.child(1).getValue(colorOffset[1]);
						plug2.child(2).getValue(colorOffset[2]);
					}
					MPlug plug3 = depNode.findPlug("alphaGain");
					if (!plug3.isNull())
					{
						plug3.getValue(alphaGain);
					}
					MPlug plug4 = depNode.findPlug("alphaOffset");
					if (!plug4.isNull())
					{
						plug4.getValue(alphaOffset);
					}
					MPlug plug5 = depNode.findPlug("invert");
					if (!plug5.isNull())
					{
						plug5.getValue(invert);
					}
				}
			}

			// Set texture
			MHWRender::MTextureAssignment textureAssignment;
			textureAssignment.texture = texture;
			if (texture)
			{
				texture->setHasAlpha(true);
			}
			bool assignedTexture = false;
			MStatus status = shaderInst->setParameter("map", textureAssignment);
			if (MStatus::kSuccess == status)
				assignedTexture = true;

			// Check for transparency
			//
            float hwTexAlpha  = useUnconnectedColor ? 0.3f : depNode.findPlug("hwtexalpha").asFloat();

			float alphaThreshold = 0.0f;       
			status = shaderInst->setParameter("alphaThreshold", &alphaThreshold);			
			float defaultColor[4] = { hwTexAlpha, hwTexAlpha, hwTexAlpha, hwTexAlpha  };
			status = shaderInst->setParameter("defaultColor", defaultColor);	
			// Set outputMode 4 = Multiply defaultColor * texture
			status = shaderInst->setParameter("outputMode", 4);

			// Set V flip if needed
			status = shaderInst->setParameter("verticalFlip", m_flipVData);

			// Make shader transparent as necessary
			shaderInst->setIsTransparent(hwTexAlpha < 1.0f);

			// Set color effects
			shaderInst->setParameter("colorGain", colorGain);
			shaderInst->setParameter("colorOffset", colorOffset);
			shaderInst->setParameter("alphaOffset", alphaOffset);
			shaderInst->setParameter("alphaGain", alphaGain);
			shaderInst->setParameter("invert", invert);

			// Set sampler
			shaderInst->setParameter("samp", MHWRender::MSamplerState::kAnisotropic);

			texturedItem->setShader(shaderInst, &s_texturedItemName);
		}

		// Release reference to texture, the shader will hold a reference to keep
		// it alive for shading
		if (texture)
		{
			textureManager->releaseTexture(texture);
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

void CArnoldSkyDomeLightGeometryOverride::createFilledSkyDomeGeometry(unsigned int divisions[2],
																float radius)
{
	//divisions[0] = divisions[1] = 4;

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

	AtVector dir, dirLL;
	float u, v, u2, v2, u3, v3;
	float prev_u = 110.0;
	float prev_u2 = 110.0;
	float prev_u3 = 110.0;

	// Create filled data
	for (unsigned int x = 0; x < divisionsX1; ++x)
	{
		const float phi = (float)AI_PITIMES2 * (float)x / (float)divisions[0];

		for (unsigned int y = 0; y < divisionsY1; ++y)
		{         
			const float theta = (float)AI_PI * (float)y / (float)divisions[1];
			dir = SphereVertex(phi, theta);            
			dirLL = SphereVertex(AI_PI + phi, theta);

			AiMappingMirroredBall(&dir, &u, &v);	// Mirrored Ball
			AiMappingAngularMap(&dir, &u2, &v2);	// Angular
			AiMappingLatLong(&dirLL, &u3, &v3);		// Lat-long
#if 1
			if (y == divisions[1])
			{
				u3 = prev_u3;
			}
			else
			{
				prev_u3 = u3;
			}
#endif
			if (x == divisions[0])
			{
				u3 = 0.0f;
			}

			const int id = x + y * divisionsX1;

#if 0
			MString g("x,y =");
			g += x; g += ",", g += y;
			g += ". Phi="; g += phi;
			g += ". Theta="; g += theta;
			g += ". Direction = ";
			g += dir.x; g+= ","; g += dir.y; g += ",", g += dir.z;
			g += ". U,V = ";
			g += u3; g += ","; g += v3;
/*			g += ". M U,V = ";
			g += u; g += ","; g += v;
			g += ". A U,V = ";
			g += u2; g += ","; g += v2; */
			g += ". ID = "; g += id;
			MGlobal::displayInfo(g);
#endif

			s_filledUvs[0][id*2] = u;
			s_filledUvs[0][id*2+1] = v;	
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
}


void CArnoldSkyDomeLightGeometryOverride::createWireSkyDomeGeometry(unsigned int divisions[2],
																float radius)
{
	// Allocate memory
	unsigned int numVertices_wire = divisions[0] * divisions[1] * 8;
	unsigned int numIndicies_wire = divisions[0] * divisions[1] * 8;
	s_wirePositions.setLength(numVertices_wire);
	s_wireIndexing.setLength(numIndicies_wire);

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

	// Build geometry if dirty
	if (m_geometryDirty)
	{
		unsigned int wire_dim[2] = { 32, 32 };
		createWireSkyDomeGeometry(wire_dim, m_radius*1.001f);
		unsigned int filled_dim[2] = { 128, 128 };
		createFilledSkyDomeGeometry(filled_dim, m_radius);

		m_geometryDirty = false;
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
				else 
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
							MStatus status = filledUvBuffer->update(&(s_filledUvs[m_format][0]), 0, numUvs, true);
							if (status != MStatus::kSuccess)
							{
								//MGlobal::displayInfo("Unable to load UVS for skydome");
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
		else if (item->name() == s_wireframeItemName ||
				 item->name() == s_activeWireframeItemName)
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
