#include "ArnoldGenericShaderOverride.h"
#include "ViewportUtils.h"

#include <maya/MShaderManager.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MFragmentManager.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MRenderUtil.h>
#include <algorithm>

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldGenericShaderOverride::creator(const MObject& obj)
{
   return new ArnoldGenericShaderOverride(obj);
}

ArnoldGenericShaderOverride::ArnoldGenericShaderOverride(const MObject& obj)
: MPxSurfaceShadingNodeOverride(obj)
, m_object(obj)
, m_samplerState(NULL)
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
	if(m_samplerState)
	{
		MHWRender::MStateManager::releaseSamplerState(m_samplerState);
		m_samplerState = 0;
	}
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
}

void ArnoldGenericShaderOverride::updateShader(MHWRender::MShaderInstance& shader,
                                                const MHWRender::MAttributeParameterMappingList& mappings)
{
      MString resolvedMapName = "";
      MString resolvedSamplerName = "";
      
      const MHWRender::MAttributeParameterMapping* mapping =
            mappings.findByParameterName("map");
      if (mapping)
      {
          resolvedMapName = mapping->resolvedParameterName();
      }

      mapping = mappings.findByParameterName("textureSampler");
      if (mapping)
      {
          resolvedSamplerName = mapping->resolvedParameterName();
      }

	// Set the parameters on the shader
	if (resolvedMapName.length() > 0 && resolvedSamplerName.length() > 0)
	{	
        MStatus status;
        MFnDependencyNode node(m_object, &status);
        if(status && !m_samplerState)
        {
			int wrapMode[2] = { 0, 0 };
            MPlug plugS = node.findPlug("swrap");
            MPlug plugT = node.findPlug("twrap");
            if(!plugS.isNull() && !plugT.isNull())
            {
                plugS.getValue(wrapMode[0]);
                plugT.getValue(wrapMode[1]);

				std::transform(std::begin(wrapMode), std::end(wrapMode), std::begin(wrapMode), [](int mode)->int {
					switch(mode)
					{
					case 0:
						return 1;

					case 1:
						return 4;

					case 2:
						return 3;

					case 3:
						return 2;
					};

					return 1;
				});

				MHWRender::MSamplerStateDesc desc;
				desc.filter = MHWRender::MSamplerState::kAnisotropic;
				desc.maxAnisotropy = 16;
				desc.addressU = static_cast<MHWRender::MSamplerState::TextureAddress>(wrapMode[0]);
				desc.addressV = static_cast<MHWRender::MSamplerState::TextureAddress>(wrapMode[1]);
				m_samplerState = MHWRender::MStateManager::acquireSamplerState(desc);
                    
				if(m_samplerState)
					shader.setParameter(resolvedSamplerName, *m_samplerState);
            }
        }

		// Set texture
		MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
		if (renderer)
		{
			MHWRender::MTextureManager* textureManager =
				renderer->getTextureManager();
			if (textureManager)
			{
                MStatus status;
                MFnDependencyNode node(m_object, &status);
                if(status)
                {
                    MString name, filename;
                    node.findPlug("filename").getValue(name);
                    MRenderUtil::exactFileTextureName(name, false, "", filename);

                    MHWRender::MTexture* texture =
                        textureManager->acquireTexture(filename);
                    if (texture)
                    {
                        MHWRender::MTextureAssignment textureAssignment;
                        textureAssignment.texture = texture;
                        shader.setParameter(resolvedMapName, textureAssignment);

                        // release our reference now that it is set on the shader
                        textureManager->releaseTexture(texture);
                    }
                }
			}
		}
	}
}

bool ArnoldGenericShaderOverride::valueChangeRequiresFragmentRebuild(const MPlug* plug) const
{
	if (plug->partialName() == "swrap" || plug->partialName() == "twrap")
	{
		MHWRender::MStateManager::releaseSamplerState(m_samplerState);
		m_samplerState = 0;
	}
	else if (plug->partialName(false, false, false, false, false, true) == "colorSpace")
	{
		return true;
	}

	return MHWRender::MPxSurfaceShadingNodeOverride::valueChangeRequiresFragmentRebuild(plug);
}
