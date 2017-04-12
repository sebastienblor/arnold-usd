#include "ArnoldStandardHairShaderOverride.h"
#include "ViewportUtils.h"

#include <maya/MShaderManager.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MFragmentManager.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>

MHWRender::MPxSurfaceShadingNodeOverride* ArnoldStandardHairShaderOverride::creator(const MObject& obj)
{
   return new ArnoldStandardHairShaderOverride(obj);
}

ArnoldStandardHairShaderOverride::ArnoldStandardHairShaderOverride(const MObject& obj)
: MPxSurfaceShadingNodeOverride(obj), m_object(obj), m_fragmentName("")
{
   MStringArray reqs;
   reqs.append("standardHairShaderCombiner");
   reqs.append("standardShaderLightingData");
   reqs.append("standardShaderDiffuse");
   reqs.append("standardShaderEmission");
   reqs.append("standardHairSpecular");
   reqs.append("standardHairShaderBase");
   reqs.append("standardFresnel");
   reqs.append("standardfloatPassThrough");
   reqs.append("standardfloat3PassThrough");
   reqs.append("standardShaderLightingCombiner");
   if (LoadFragmentGraph("standardHairShader", reqs))
      m_fragmentName = "standardHairShader";
}

ArnoldStandardHairShaderOverride::~ArnoldStandardHairShaderOverride()
{

}

MHWRender::DrawAPI ArnoldStandardHairShaderOverride::supportedDrawAPIs() const
{
   return MHWRender::kAllDevices;
}

MString ArnoldStandardHairShaderOverride::fragmentName() const
{
   return m_fragmentName;
}

void ArnoldStandardHairShaderOverride::getCustomMappings(
   MHWRender::MAttributeParameterMappingList& mappings)
{
	MHWRender::MAttributeParameterMapping mapping(
		"transparency", "aiTransparency", false, true);
	mappings.append(mapping);
}

void ArnoldStandardHairShaderOverride::updateDG()
{
	MStatus status;
	MFnDependencyNode node(m_object, &status);
	if (status)
	{
		float opacity[3];
		node.findPlug("opacityR").getValue(opacity[0]);
		node.findPlug("opacityG").getValue(opacity[1]);
		node.findPlug("opacityB").getValue(opacity[2]);

		// transfer the opacity values to a transparency attribute the VP2 API can recognize.
		node.findPlug("aiTransparencyR").setValue(1.0f - opacity[0]);
		node.findPlug("aiTransparencyG").setValue(1.0f - opacity[1]);
		node.findPlug("aiTransparencyB").setValue(1.0f - opacity[2]);
	}
}

void ArnoldStandardHairShaderOverride::updateShader(MHWRender::MShaderInstance& shader,
                                                const MHWRender::MAttributeParameterMappingList& mappings)
{
}

MString ArnoldStandardHairShaderOverride::bumpAttribute() const
{
	return "normalCamera";
}

MString ArnoldStandardHairShaderOverride::transparencyParameter() const
{
	return "transparency";
}
