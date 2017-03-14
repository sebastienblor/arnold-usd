#include "ArnoldGenericShaderOverride.h"
#include "ViewportUtils.h"

#include <maya/MShaderManager.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MFragmentManager.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MRenderUtil.h>

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldGenericShaderOverride::creator(const MObject& obj)
{
   return new ArnoldGenericShaderOverride(obj);
}

ArnoldGenericShaderOverride::ArnoldGenericShaderOverride(const MObject& obj)
: MPxSurfaceShadingNodeOverride(obj)
, m_object(obj)
, m_fragmentName("")
, fSamplerState(NULL)
{
   MStringArray reqs;
   reqs.append("genericShaderCombiner");
   reqs.append("standardShaderLightingData");
   reqs.append("standardShaderCombiner");
   reqs.append("standardShaderDiffuse");
   reqs.append("standardShaderLightingCombiner");
   reqs.append("textureMapping");
   if (LoadFragmentGraph("genericShader", reqs))
      m_fragmentName = "genericShader";
}

ArnoldGenericShaderOverride::~ArnoldGenericShaderOverride()
{

}

MHWRender::DrawAPI ArnoldGenericShaderOverride::supportedDrawAPIs() const
{
   // need to make sure it supports OpenGL core profile (#2551)
   return MHWRender::kAllDevices;
   // return MHWRender::kOpenGL | MHWRender::kDirectX11;
}

MString ArnoldGenericShaderOverride::fragmentName() const
{
   return m_fragmentName;
}

void ArnoldGenericShaderOverride::getCustomMappings(
   MHWRender::MAttributeParameterMappingList& mappings)
{
	MHWRender::MAttributeParameterMapping mapMapping(
		"map", "", false, true);
	mappings.append(mapMapping);

	MHWRender::MAttributeParameterMapping textureSamplerMapping(
		"textureSampler", "", false, true);
	mappings.append(textureSamplerMapping);
}

void ArnoldGenericShaderOverride::updateDG()
{
	// Pull the file name from the DG for use in updateShader
	MStatus status;
	MFnDependencyNode node(m_object, &status);
	if (status)
	{
		MString name;
		node.findPlug("filename").getValue(name);
		MRenderUtil::exactFileTextureName(name, false, "", m_fileName);
	}
}

void ArnoldGenericShaderOverride::updateShader(MHWRender::MShaderInstance& shader,
                                                const MHWRender::MAttributeParameterMappingList& mappings)
{
	// Handle resolved name caching
	if (fResolvedMapName.length() == 0)
	{
		const MHWRender::MAttributeParameterMapping* mapping =
			mappings.findByParameterName("map");
		if (mapping)
		{
			fResolvedMapName = mapping->resolvedParameterName();
		}
	}
	if (fResolvedSamplerName.length() == 0)
	{
		const MHWRender::MAttributeParameterMapping* mapping =
			mappings.findByParameterName("textureSampler");
		if (mapping)
		{
			fResolvedSamplerName = mapping->resolvedParameterName();
		}
	}

	// Set the parameters on the shader
	if (fResolvedMapName.length() > 0 && fResolvedSamplerName.length() > 0)
	{
		// Set sampler to linear-wrap
		if (!fSamplerState)
		{
			MHWRender::MSamplerStateDesc desc;
			desc.filter = MHWRender::MSamplerState::kAnisotropic;
			desc.maxAnisotropy = 16;
			fSamplerState = MHWRender::MStateManager::acquireSamplerState(desc);
		}
		if (fSamplerState)
		{
			shader.setParameter(fResolvedSamplerName, *fSamplerState);
		}

		// Set texture
		MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
		if (renderer)
		{
			MHWRender::MTextureManager* textureManager =
				renderer->getTextureManager();
			if (textureManager)
			{
				MHWRender::MTexture* texture =
					textureManager->acquireTexture(m_fileName);
				if (texture)
				{
					MHWRender::MTextureAssignment textureAssignment;
					textureAssignment.texture = texture;
					shader.setParameter(fResolvedMapName, textureAssignment);

					// release our reference now that it is set on the shader
					textureManager->releaseTexture(texture);
				}
			}
		}
	}
}

bool ArnoldGenericShaderOverride::valueChangeRequiresFragmentRebuild(const MPlug* plug) const
{
	return true;
}
