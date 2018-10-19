#include "ArnoldAiImageShaderOverride.h"
#include "ViewportUtils.h"

#include <maya/MShaderManager.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MFragmentManager.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MRenderUtil.h>
#include <algorithm>

#include <utils/MayaUtils.h>

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldAiImageShaderOverride::creator(const MObject& obj)
{
   return new ArnoldAiImageShaderOverride(obj);
}

ArnoldAiImageShaderOverride::ArnoldAiImageShaderOverride(const MObject& obj)
: MPxSurfaceShadingNodeOverride(obj)
, m_object(obj)
, m_samplerState(NULL)
{
    MStringArray reqs;
    reqs.append("textureMapping");
    if (LoadFragmentGraph("imageShader", reqs))
        m_fragmentName = "imageShader";
}

ArnoldAiImageShaderOverride::~ArnoldAiImageShaderOverride()
{
	if(m_samplerState)
	{
		MHWRender::MStateManager::releaseSamplerState(m_samplerState);
		m_samplerState = 0;
	}
}

MHWRender::DrawAPI ArnoldAiImageShaderOverride::supportedDrawAPIs() const
{
   // need to make sure it supports OpenGL core profile (#2551)
   return MHWRender::kAllDevices;
}

MString ArnoldAiImageShaderOverride::fragmentName() const
{
   return m_fragmentName;
}

void ArnoldAiImageShaderOverride::getCustomMappings(
   MHWRender::MAttributeParameterMappingList& mappings)
{
	MHWRender::MAttributeParameterMapping mapMapping(
		"map", "", false, true);
	mappings.append(mapMapping);

	MHWRender::MAttributeParameterMapping textureSamplerMapping(
		"textureSampler", "", false, true);
	mappings.append(textureSamplerMapping);
}

void ArnoldAiImageShaderOverride::updateDG()
{
}

void ArnoldAiImageShaderOverride::updateShader(MHWRender::MShaderInstance& shader,
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

                for (int w = 0; w < 2; ++w)
                {
					switch(wrapMode[w])
					{
					// FIXME restore the original constants instead of hardcoding the values !
					// but make sure it compiles on linux/osx (without c++11)
					default:
					case 0:
						wrapMode[w] = 1;//MHWRender::MSamplerState::TextureAddress::kTexWrap; // arnold periodic
						break;
					case 1:
						wrapMode[w] = 4;//MHWRender::MSamplerState::TextureAddress::kTexBorder; // arnold black
						break;
					case 2:
						wrapMode[w] = 3;//MHWRender::MSamplerState::TextureAddress::kTexClamp; // arnold clamp
						break;
					case 3:
						wrapMode[w] = 2;//MHWRender::MSamplerState::TextureAddress::kTexMirror; // arnold mirror
						break;
					}
                }

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

                    name = resolveFilePathForSequences(name, node.findPlug("frame").asInt());
                    MRenderUtil::exactFileTextureName(name, false, "", filename);

                    MHWRender::MTexture* texture =
                        textureManager->acquireTexture(filename, 0, false);
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

bool ArnoldAiImageShaderOverride::valueChangeRequiresFragmentRebuild(const MPlug* plug) const
{
	if (plug == NULL)
		return false;
	
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
