#include <string>
#include <vector>
#include <iostream>

#include <maya/MGlobal.h>
#include <maya/MObject.h>
#include <maya/MFileObject.h>
#include <maya/MTime.h>

#include <ai.h>

#include "extension/Extension.h"
#include "BifrostTranslator.h"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl

#define EXPORT_BOOL(name) AiNodeSetBool(shape, name, dagNode.findPlug(name).asBool())
#define EXPORT_INT(name) AiNodeSetInt(shape, name, dagNode.findPlug(name).asInt())
#define EXPORT_FLT(name) AiNodeSetFlt(shape, name, dagNode.findPlug(name).asFloat())
#define EXPORT_FLT2(name) AiNodeSetVec2(shape, name, dagNode.findPlug(name "X").asFloat(), dagNode.findPlug(name "Y").asFloat())
#define EXPORT_FLT3(name) AiNodeSetVec(shape, name, dagNode.findPlug(name "X").asFloat(), dagNode.findPlug(name "Y").asFloat(), dagNode.findPlug(name "Z").asFloat())
#define EXPORT_STR(name) AiNodeSetStr(shape, name, dagNode.findPlug(name).asString().asChar())

#define EXPORT2_BOOL(aiName, name) AiNodeSetBool(shape, aiName, dagNode.findPlug(name).asBool())
#define EXPORT2_INT(aiName, name) AiNodeSetInt(shape, aiName, dagNode.findPlug(name).asInt())
#define EXPORT2_FLT(aiName, name) AiNodeSetFlt(shape, aiName, dagNode.findPlug(name).asFloat())
#define EXPORT2_FLT3(aiName, name) AiNodeSetVec(shape, aiName, dagNode.findPlug(name "X").asFloat(), dagNode.findPlug(name "Y").asFloat(), dagNode.findPlug(name "Z").asFloat())
#define EXPORT2_STR(aiName, name) AiNodeSetStr(shape, aiName, dagNode.findPlug(name).asString().asChar())

AtNode* BifrostTranslator::CreateArnoldNodes()
{
    return AddArnoldNode("bifrost_polymesh");
}

void BifrostTranslator::Export(AtNode* shape)
{
    MFnDagNode dagNode(m_dagPath.node());
    EXPORT_BOOL("opaque");
    EXPORT_BOOL("matte");
    AiNodeSetByte(shape, "visibility", AI_RAY_ALL);

    // common attributes
    EXPORT_INT("render_component");

    EXPORT_FLT("velocity_scale");
    EXPORT_FLT("space_scale");
    const MTime sec(1.0, MTime::kSeconds);
    float fps = (float) sec.as(MTime::uiUnit());
    AiNodeSetFlt(shape, "fps", fps);

    //ADD_DSTR("channels");

    EXPORT_BOOL("clip");
    EXPORT_FLT3("clip_min");
    EXPORT_FLT3("clip_max");

    // export shaders
    if(RequiresShaderExport()){
        ExportBifrostShader();
    }
    ExportLightLinking( shape );

    // object / file
    AiNodeSetStr(shape, "object", dagNode.findPlug("object").asString().asChar());

    ExportPolymesh(dagNode, shape);
}

void BifrostTranslator::ExportSurface( MFnDagNode&  dagNode, AtNode *shape )
{
    EXPORT_STR("distance_channel");

    EXPORT_FLT("dilate");
    EXPORT_FLT("erode");

    EXPORT_FLT("smooth");
    //ADD_DSMOOTH_ENUM("smooth_mode");
    EXPORT_INT("smooth_iterations");

    EXPORT_BOOL("enable_infinite_blending");
    EXPORT_FLT("enable_infinite_blending");
    EXPORT_FLT2("infinite_blending_center");
    EXPORT_FLT2("infinite_blending_dimension");
}

void BifrostTranslator::ExportPolymesh(MFnDagNode &dagNode, AtNode *shape)
{
    ExportSurface(dagNode, shape);

    // export mesh specific attributes
    EXPORT_INT("subdivisions");
    ExportDisplacement();
}

void BifrostTranslator::ExportMotion( AtNode* shape ){
    if(IsMotionBlurEnabled()){
        ExportMatrix(shape);
    }
}

void BifrostTranslator::ExportBifrostShader(){
    MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), 0);
    if ( !shadingGroupPlug.isNull() ) {
        AtNode *rootShader = ExportConnectedNode(shadingGroupPlug);
        if (rootShader != NULL) {
            // Push the shader in the vector to be assigned later to mtoa_shading_groups
            AiNodeSetPtr(GetArnoldNode(), "shader", rootShader);
        }
    }
}

namespace{

void GetDisplacement(MObject& obj, float& dispPadding) {
    MFnDependencyNode dNode(obj);
    MPlug plug = dNode.findPlug("aiDisplacementPadding");
    if (!plug.isNull())
        dispPadding = plug.asFloat();
}

}

void BifrostTranslator::ExportDisplacement(){
    MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), 0);
    if(shadingGroupPlug.isNull()) return;

    MPlugArray        connections;
    MFnDependencyNode fnDGShadingGroup(shadingGroupPlug.node());
    MPlug shaderPlug = fnDGShadingGroup.findPlug("displacementShader");
    shaderPlug.connectedTo(connections, true, false);

    // are there any connections to displacementShader?
    if (connections.length() > 0)
    {
        MObject dispNode = connections[0].node();
        float padding = 0;
        GetDisplacement(dispNode, padding);

        AtNode* dispImage(ExportConnectedNode(connections[0]));
        AtNode* node = GetArnoldNode();
        AiNodeSetPtr(node, "disp_map", dispImage);
        AiNodeSetFlt(node, "disp_padding", padding);
        AiNodeSetBool(node, "disp_autobump", true);
        AiNodeSetFlt(node, "disp_height", 1);
        AiNodeSetFlt(node, "disp_zero_value", 0);
    }
}

void BifrostTranslator::RequestUpdate()
{
    SetUpdateMode( AI_RECREATE_NODE );
    CShapeTranslator::RequestUpdate();
}

// ***************
//  ALL THE CODE BELLOW SHOULD BE REMOVED WHEN THESE ATTRIBUTES BECOME PART OF
//  BIFROST SHAPE IN MAYA 2018
// ***************

#define ADD_DSTR(longName, value) \
    data.name = data.shortName = longName;\
    data.stringDefault = MString(value); \
    helper.MakeInputString(data);

#define ADD_DFLT(longName, value) \
    data.name = data.shortName = longName;\
    data.hasSoftMin = data.hasSoftMax = true; \
    data.softMin.FLT() = 0.f;\
    data.softMax.FLT() = 1.f; \
    data.hasMin = data.hasMax = false; \
    data.defaultValue.FLT() = value;\
    helper.MakeInputFloat(data);

#define ADD_DINT(longName, value) \
    data.name = data.shortName = longName;\
    data.hasSoftMin = data.hasSoftMax = true; \
    data.softMin.INT() = 0;\
    data.softMax.INT() = 1; \
    data.hasMin = data.hasMax = false; \
    data.defaultValue.INT() = value;\
    helper.MakeInputInt(data);

#define ADD_DINT_HARDMIN(longName, value, minValue) \
    data.name = data.shortName = longName; \
    data.hasSoftMin = data.hasSoftMax = data.hasMin = true;\
    data.hasMax = false;\
    data.min.INT() = minValue; \
    data.softMin.INT() = minValue; \
    data.softMax.INT() = 2*value-minValue;\
    data.defaultValue.INT() = value;\
    helper.MakeInputInt(data);

#define ADD_DBOOL(longName, value) \
    data.name = data.shortName = longName;\
    data.defaultValue.BOOL() = value;\
    helper.MakeInputBoolean(data);

#define ADD_DFLT3(longName, x, y, z) \
    data.name = data.shortName = longName;\
    data.defaultValue.VEC() = AtVector(x,y,z);\
    helper.MakeInputVector(data);

#define ADD_DFLT2(longName, x, y) \
    data.name = data.shortName = longName;\
    data.defaultValue.VEC2() = AtVector2(x,y);\
    helper.MakeInputVector2(data);

#define ADD_DSMOOTH_ENUM(longName) \
    data.name = data.shortName = longName;\
    {\
        MStringArray enums;\
        enums.append("MeanValue");\
        enums.append("Gaussian");\
        enums.append("Median");\
        enums.append("LaplacianFlow");\
        enums.append("CurvatureFlow");\
        data.enums = enums;\
    }\
    helper.MakeInputEnum(data);

#define ADD_DFALLOFF_ENUM(longName) \
    data.name = data.shortName = longName;\
    {\
        MStringArray enums;\
        enums.append("Linear");\
        enums.append("Smooth");\
        enums.append("Smoother");\
        enums.append("None");\
        data.enums = enums;\
    }\
    helper.MakeInputEnum(data);

#define ADD_DDATA_ENUM(longName) \
    data.name = data.shortName = longName;\
    {\
        MStringArray enums;\
        enums.append("Voxels");\
        enums.append("Particles");\
        data.enums = enums;\
    }\
    helper.MakeInputEnum(data);


void BifrostTranslator::NodeInitializer( CAbTranslator context )
{
    CExtensionAttrHelper helper(context.maya, "standard");
    CAttrData data;

    ADD_DBOOL("opaque", true);
    ADD_DBOOL("matte", false);
    ADD_DBOOL("smoothing", true);
    ADD_DINT_HARDMIN("subdivisions", 1, 0);

    // TODO: remove the following attributes (non arnold specific)
    ADD_DDATA_ENUM("render_component");

    ADD_DFLT("velocity_scale", 1.f);
    ADD_DFLT("space_scale", 1.f);

    ADD_DSTR("channels", "vorticity");
    ADD_DSTR("distance_channel", "distance");

    ADD_DFLT("dilate", 0.f);
    ADD_DFLT("erode", 0.f);

    ADD_DFLT("smooth", 0.f);
    ADD_DSMOOTH_ENUM("smooth_mode");
    ADD_DINT_HARDMIN("smooth_iterations", 1, 1);

    ADD_DBOOL("clip", false);
    ADD_DFLT3("clip_min", -1.f, -1.f, -1.f);
    ADD_DFLT3("clip_max",  1.f,  1.f,  1.f);

    ADD_DBOOL("enable_infinite_blending", false);
    ADD_DFLT("enable_infinite_blending", 0);
    ADD_DFLT2("infinite_blending_center", 0, 0);
    ADD_DFLT2("infinite_blending_dimension", 0, 0);
}
