#include "ArnoldAssTranslator.h"
#include "ArnoldExportAssCmd.h"
#include "ArnoldRenderCmd.h"
#include "ArnoldIprCmd.h"
#include "nodes/ArnoldRenderOptions.h"
#include "nodes/ArnoldAOV.h"
#include "nodes/shaders/atmosphere/ArnoldFogShader.h"
#include "nodes/shaders/atmosphere/ArnoldVolumeScatteringShader.h"
#include "nodes/shaders/background/ArnoldSkyShader.h"
#include "nodes/shaders/displacement/ArnoldDisplacementShader.h"
#include "nodes/shaders/light/ArnoldBarndoorShader.h"
#include "nodes/shaders/light/ArnoldGoboShader.h"
#include "nodes/shaders/light/ArnoldLightBlockerShader.h"
#include "nodes/shaders/light/ArnoldLightDecayShader.h"
#include "nodes/shaders/surface/ArnoldAmbientOcclusionShader.h"
#include "nodes/shaders/surface/ArnoldStandardShader.h"
#include "nodes/shaders/surface/ArnoldUtilityShader.h"
#include "nodes/shaders/surface/ArnoldMeshInfoShader.h"
#include "nodes/shaders/surface/ArnoldWireframeShader.h"
#include "nodes/shaders/surface/ArnoldHairShader.h"
#include "nodes/shaders/surface/ArnoldRaySwitchShader.h"

#include <ai_render.h>

#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#ifdef _WIN32
#define MTOA_DLLEXPORT __declspec(dllexport)
#else
#define MTOA_DLLEXPORT __attribute__ ((visibility("default")))
#endif

#define MTOA_VENDOR "SolidAngle"
#define MTOA_VERSION "0.3"
#define MAYA_VERSION "Any"

namespace // <anonymous>
{

void RegisterArnoldNodes(MObject object)
{
	MStatus status;
	MFnPlugin plugin(object);
	const MString ShaderClass("shader/surface");
	const MString DisplacementClass("shader/displacement");
	const MString LightClass("light");

	// RENDER OPTIONS
	status = plugin.registerNode("ArnoldRenderOptions",
			CArnoldRenderOptionsNode::id, CArnoldRenderOptionsNode::creator,
			CArnoldRenderOptionsNode::initialize);

	// AOV
	status = plugin.registerNode("ArnoldAOV", CArnoldAOVNode::id,
			CArnoldAOVNode::creator, CArnoldAOVNode::initialize);

	// Ray switch shader [could be surface or environment...]
	status = plugin.registerNode("ArnoldRaySwitchShader",
			CArnoldRaySwitchShaderNode::id,
			CArnoldRaySwitchShaderNode::creator,
			CArnoldRaySwitchShaderNode::initialize, MPxNode::kDependNode,
			&ShaderClass);
	// Surface Shaders
	status = plugin.registerNode("ArnoldAmbientOcclusionShader",
			CArnoldAmbientOcclusionShaderNode::id,
			CArnoldAmbientOcclusionShaderNode::creator,
			CArnoldAmbientOcclusionShaderNode::initialize,
			MPxNode::kDependNode, &ShaderClass);
	status = plugin.registerNode("ArnoldStandardShader",
			CArnoldStandardShaderNode::id, CArnoldStandardShaderNode::creator,
			CArnoldStandardShaderNode::initialize, MPxNode::kDependNode,
			&ShaderClass);
	status = plugin.registerNode("ArnoldUtilityShader",
			CArnoldUtilityShaderNode::id, CArnoldUtilityShaderNode::creator,
			CArnoldUtilityShaderNode::initialize, MPxNode::kDependNode,
			&ShaderClass);
	status = plugin.registerNode("ArnoldMeshInfoShader",
			CArnoldMeshInfoShaderNode::id, CArnoldMeshInfoShaderNode::creator,
			CArnoldMeshInfoShaderNode::initialize, MPxNode::kDependNode,
			&ShaderClass);
	status = plugin.registerNode("ArnoldWireframeShader",
			CArnoldWireframeShaderNode::id,
			CArnoldWireframeShaderNode::creator,
			CArnoldWireframeShaderNode::initialize, MPxNode::kDependNode,
			&ShaderClass);
	status = plugin.registerNode("ArnoldHairShader", CArnoldHairShaderNode::id,
			CArnoldHairShaderNode::creator, CArnoldHairShaderNode::initialize,
			MPxNode::kDependNode, &ShaderClass);

	// Displacement Shaders
	status = plugin.registerNode("ArnoldDisplacementShader",
			CArnoldDisplacementShaderNode::id,
			CArnoldDisplacementShaderNode::creator,
			CArnoldDisplacementShaderNode::initialize, MPxNode::kDependNode,
			&DisplacementClass);

	// Light Shaders
	status = plugin.registerNode("ArnoldBarndoorShader",
			CArnoldBarndoorShaderNode::id, CArnoldBarndoorShaderNode::creator,
			CArnoldBarndoorShaderNode::initialize);
	status = plugin.registerNode("ArnoldGoboShader", CArnoldGoboShaderNode::id,
			CArnoldGoboShaderNode::creator, CArnoldGoboShaderNode::initialize);
	status = plugin.registerNode("ArnoldLightBlockerShader",
			CArnoldLightBlockerShaderNode::id,
			CArnoldLightBlockerShaderNode::creator,
			CArnoldLightBlockerShaderNode::initialize);
	status = plugin.registerNode("ArnoldLightDecayShader",
			CArnoldLightDecayShaderNode::id,
			CArnoldLightDecayShaderNode::creator,
			CArnoldLightDecayShaderNode::initialize);

	// Special shaders (not visible from Maya shaders menu)
	status = plugin.registerNode("ArnoldFogShader", CArnoldFogShaderNode::id,
			CArnoldFogShaderNode::creator, CArnoldFogShaderNode::initialize);
	status = plugin.registerNode("ArnoldSkyShader", CArnoldSkyShaderNode::id,
			CArnoldSkyShaderNode::creator, CArnoldSkyShaderNode::initialize,
			MPxNode::kLocatorNode, &LightClass);
	status = plugin.registerNode("ArnoldVolumeScatteringShader",
			CArnoldVolumeScatteringShaderNode::id,
			CArnoldVolumeScatteringShaderNode::creator,
			CArnoldVolumeScatteringShaderNode::initialize);
}

void UnregisterArnoldNodes(MObject object)
{
	MStatus status;
	MFnPlugin plugin(object);

	// RENDER OPTIONS
	status = plugin.deregisterNode(CArnoldRenderOptionsNode::id);
	// Remove creation callback
	MDGMessage::removeCallback(CArnoldRenderOptionsNode::sId);

	// AOV
	status = plugin.deregisterNode(CArnoldAOVNode::id);

	// Ray switch shader
	status = plugin.deregisterNode(CArnoldRaySwitchShaderNode::id);

	// Surface Shaders
	status = plugin.deregisterNode(CArnoldAmbientOcclusionShaderNode::id);
	status = plugin.deregisterNode(CArnoldStandardShaderNode::id);
	status = plugin.deregisterNode(CArnoldUtilityShaderNode::id);
	status = plugin.deregisterNode(CArnoldMeshInfoShaderNode::id);
	status = plugin.deregisterNode(CArnoldWireframeShaderNode::id);
	status = plugin.deregisterNode(CArnoldHairShaderNode::id);

	// Displacement Shaders
	status = plugin.deregisterNode(CArnoldDisplacementShaderNode::id);

	// Light Shaders
	status = plugin.deregisterNode(CArnoldBarndoorShaderNode::id);
	status = plugin.deregisterNode(CArnoldGoboShaderNode::id);
	status = plugin.deregisterNode(CArnoldLightBlockerShaderNode::id);
	status = plugin.deregisterNode(CArnoldLightDecayShaderNode::id);

	status = plugin.deregisterNode(CArnoldFogShaderNode::id);
	status = plugin.deregisterNode(CArnoldSkyShaderNode::id);
	status = plugin.deregisterNode(CArnoldVolumeScatteringShaderNode::id);
}

} // namespace <anonymous>

MTOA_DLLEXPORT MStatus initializePlugin(MObject object)
{
	MStatus status;

	MFnPlugin plugin(object, MTOA_VENDOR, MTOA_VERSION, MAYA_VERSION);

	AiBegin();

	// TODO: Add proper checking and handling of returned status
	status = plugin.registerCommand("arnoldRender", CArnoldRenderCmd::creator,
			CArnoldRenderCmd::newSyntax);
	status = plugin.registerCommand("arnoldIpr", CArnoldIprCmd::creator,
			CArnoldIprCmd::newSyntax);
	status = plugin.registerCommand("arnoldExportAss",
			CArnoldExportAssCmd::creator, CArnoldExportAssCmd::newSyntax);
	/* status = plugin.registerFileTranslator(	CArnoldAssTranslator::fileType,
	 CArnoldAssTranslator::fileIcon,
	 CArnoldAssTranslator::creator,
	 CArnoldAssTranslator::optionScript,
	 CArnoldAssTranslator::optionDefault,
	 false );
	 */
	status = plugin.registerFileTranslator(CArnoldAssTranslator::fileType,
			CArnoldAssTranslator::fileIcon, CArnoldAssTranslator::creator,
			NULL, NULL, false);

	RegisterArnoldNodes(object);

	MGlobal::executePythonCommand(
			MString(
					"import mtoa.cmds.registerArnoldRenderer;mtoa.cmds.registerArnoldRenderer.registerArnoldRenderer()"));

	AiEnd();

	return MS::kSuccess;
}

MTOA_DLLEXPORT MStatus uninitializePlugin(MObject object)
{
	MStatus status;
	MFnPlugin plugin(object);

	MGlobal::executePythonCommand(
			MString(
					"import mtoa.cmds.unregisterArnoldRenderer;mtoa.cmds.unregisterArnoldRenderer.unregisterArnoldRenderer()"));

	UnregisterArnoldNodes(object);

	// TODO: Add proper checking and handling of returned status
	status = plugin.deregisterCommand("arnoldRender");
	status = plugin.deregisterCommand("arnoldIpr");
	status = plugin.deregisterCommand("arnoldExportAss");
	status = plugin.deregisterFileTranslator(CArnoldAssTranslator::fileType);

	return MS::kSuccess;
}
