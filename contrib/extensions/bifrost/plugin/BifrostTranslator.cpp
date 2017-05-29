#include <string>
#include <vector>
#include <iostream>

#include <maya/MGlobal.h>
#include <maya/MObject.h>
#include <maya/MFileObject.h>
#include <maya/MTime.h>
#include <maya/MMatrix.h>
#include <maya/MFnTransform.h>
#include <maya/MBoundingBox.h>
#include <maya/MDGModifier.h>
#include <maya/MFnMeshData.h>
#include <bifrostapi/bifrost_om.h>
#include <bifrostapi/bifrost_query.h>
#include <bifrostapi/bifrost_object.h>
#include <bifrostapi/bifrost_activegraph.h>
#include <bifrostapi/bifrost_cacheresource.h>

#include <ai.h>

#include "extension/Extension.h"
#include "BifrostTranslator.h"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl

#define EXPORT_BOOL(name) AiNodeSetBool(shape, name, dagNode.findPlug(name).asBool())
#define EXPORT_INT(name) AiNodeSetInt(shape, name, dagNode.findPlug(name).asInt())
#define EXPORT_UINT(name) AiNodeSetUInt(shape, name, dagNode.findPlug(name).asInt())
#define EXPORT_FLT(name) AiNodeSetFlt(shape, name, dagNode.findPlug(name).asFloat())
#define EXPORT_FLT2(name) AiNodeSetVec2(shape, name, dagNode.findPlug(name "X").asFloat(), dagNode.findPlug(name "Y").asFloat())
#define EXPORT_FLT3(name) AiNodeSetVec(shape, name, dagNode.findPlug(name "X").asFloat(), dagNode.findPlug(name "Y").asFloat(), dagNode.findPlug(name "Z").asFloat())
#define EXPORT_STR(name) AiNodeSetStr(shape, name, dagNode.findPlug(name).asString().asChar())

#define EXPORT2_BOOL(name, aiName) AiNodeSetBool(shape, aiName, dagNode.findPlug(name).asBool())
#define EXPORT2_INT(name, aiName) AiNodeSetInt(shape, aiName, dagNode.findPlug(name).asInt())
#define EXPORT2_UINT(name, aiName) AiNodeSetUInt(shape, aiName, dagNode.findPlug(name).asInt())
#define EXPORT2_FLT(name, aiName) AiNodeSetFlt(shape, aiName, dagNode.findPlug(name).asFloat())
#define EXPORT2_FLT3(name, aiName) AiNodeSetVec(shape, aiName, dagNode.findPlug(name "X").asFloat(), dagNode.findPlug(name "Y").asFloat(), dagNode.findPlug(name "Z").asFloat())
#define EXPORT2_STR(name, aiName) AiNodeSetStr(shape, aiName, dagNode.findPlug(name).asString().asChar())

namespace {
    AtArray* Convert(const MStringArray& array){
        AtArray* out = AiArrayAllocate(array.length(), 1, AI_TYPE_STRING);
        for(unsigned int i = 0; i < array.length(); ++i){
            AiArraySetStr(out, i, array[i].asChar());
        }
        return out;
    }
}

AtNode* BifrostTranslator::CreateArnoldNodes()
{
    MFnDagNode dagNode(m_dagPath.node());
    int render_as = dagNode.findPlug("render_as").asInt();
    switch(render_as){
    case 0: return AddArnoldNode(dagNode.findPlug("surface_type").asInt()==0? "bifrost_polymesh" : "bifrost_implicit");
    case 1: return AddArnoldNode("bifrost_points");
    case 2: return AddArnoldNode("bifrost_volume");
    default: break;
    }
    AiMsgError("[mtoa.bifrost] Unknown 'render as' value: %d.\n", render_as);
    return nullptr;
}

void BifrostTranslator::Export( AtNode *shape ){
    MFnDagNode dagNode(m_dagPath.node());
    int render_as = dagNode.findPlug("render_as").asInt();
    switch(render_as){
    case 0:
        if(dagNode.findPlug("surface_type").asInt()==0) ExportPolymesh(dagNode, shape);
        else ExportImplicit(dagNode, shape);
        break;
    case 1: ExportPoints(dagNode, shape); break;
    case 2: ExportVolume(dagNode, shape); break;
    default: AiMsgError("[mtoa.bifrost] Unknown 'render as' value: %d.\n", render_as);
    }
}

void BifrostTranslator::ExportShape(MFnDagNode& dagNode, AtNode *shape)
{
    EXPORT_BOOL("opaque");
    EXPORT_BOOL("matte");
    AiNodeSetByte(shape, "visibility", AI_RAY_ALL);

    // common attributes
    EXPORT_FLT("velocity_scale");
    EXPORT_FLT("space_scale");
    const MTime sec(1.0, MTime::kSeconds);
    float fps = (float) sec.as(MTime::uiUnit());
    AiNodeSetFlt(shape, "fps", fps);

    this->ExportClipping(dagNode, shape);

    MString channels = dagNode.findPlug("channels").asString();
    MStringArray array;
    channels.split(' ', array);
    AiNodeSetArray(shape, "channels", Convert(array));

    if(RequiresShaderExport()){
        ExportBifrostShader();
    }
    ExportLightLinking( shape );

    // object / file
    Bifrost::API::ObjectModel om;
    Bifrost::API::Query query = om.createQuery();
    query.load(dagNode.findPlug("object").asString().asChar());
    Bifrost::API::Query::Binding binding = query.binding();
    AiNodeSetStr(shape, "activeGraph", binding.activeGraph.c_str());
    AiNodeSetStr(shape, "resourceId", binding.resID.c_str());
    AiNodeSetInt(shape, "frame", (int) binding.frame.value);
    Bifrost::API::RefArray objs = query.run();
    if(objs.count() == 1){
        Bifrost::API::Object obj = objs[0];
        AiNodeSetStr(shape, "object", obj.name().c_str());
        Bifrost::API::Runtime::ActiveGraph ag = om.activeGraph(binding.activeGraph);
        Bifrost::API::RefArray caches = ag.cacheResources(binding.frame);
        for(unsigned int i = 0; i < caches.count(); ++i){
            Bifrost::API::Runtime::CacheResource cache = caches[i];
            if(cache.objectName() == obj.name()){
                AiNodeSetStr(shape, "cache_folder", cache.folderPath().c_str());
                break;
            }
        }
    }else{
        AiMsgError("Failed to get bifrost object from query : %s", dagNode.findPlug("object").asString().asChar());
    }

    ExportMatrix(shape);
}

void BifrostTranslator::ExportSurface( MFnDagNode&  dagNode, AtNode *shape )
{
    ExportShape(dagNode, shape);
    EXPORT_INT("render_component");

    EXPORT2_STR("distance_channel", "field_channel");

    EXPORT_FLT("levelset_droplet_reveal_factor");
    EXPORT_FLT("levelset_surface_radius");
    EXPORT_FLT("levelset_droplet_radius");
    EXPORT_FLT("levelset_resolution_factor");
    EXPORT_FLT("levelset_max_volume_of_holes_to_close");

    EXPORT_FLT("dilate");
    EXPORT_FLT("erode");

    EXPORT_FLT("smooth");
    EXPORT_INT("smooth_iterations");

    this->ExportOceanPlane(dagNode, shape);
}

void BifrostTranslator::ExportPolymesh(MFnDagNode &dagNode, AtNode *shape)
{
    ExportSurface(dagNode, shape);

    // export mesh specific attributes
    EXPORT_UINT("subdivisions");
    ExportDisplacement();
    EXPORT_BOOL("smoothing");
}
void BifrostTranslator::ExportImplicit(MFnDagNode &dagNode, AtNode *shape)
{
    ExportSurface(dagNode, shape);
    AiNodeSetStr( shape, "solver", "levelset" );
    EXPORT2_FLT("implicit_step_size", "step_size");
    EXPORT2_UINT("implicit_samples", "samples");
}

void BifrostTranslator::ExportVolume(MFnDagNode &dagNode, AtNode *shape)
{
    ExportShape(dagNode, shape);
}

void BifrostTranslator::ExportPoints(MFnDagNode &dagNode, AtNode *shape)
{
    ExportShape(dagNode, shape);
    EXPORT_FLT("radius");
    EXPORT2_INT("points_type", "mode");
    EXPORT2_FLT("points_step_size", "step_size");
}

MMatrix BifrostTranslator::getRelativeMatrix(const MPlug &source){
    // TODO: make sure right dag path is used
    //unsigned int index = source.logicalIndex();
    MFnDagNode dag(source.node());
    MDagPath thisPath = m_dagPath; thisPath.pop();
    MDagPath otherPath; CHECK_MSTATUS(dag.getPath(otherPath));
    return otherPath.inclusiveMatrix() * thisPath.inclusiveMatrixInverse();
}

void BifrostTranslator::ExportClipping(const MFnDagNode &dagNode, AtNode *shape){
    EXPORT_BOOL("clip");
    MPlug clipBoxPlug = dagNode.findPlug("clip_box");
    if(clipBoxPlug.isDestination()){
        MPlug source = clipBoxPlug.source();
        MMatrix matrix = this->getRelativeMatrix(source);
        MBoundingBox bbox = MFnDagNode(source.node()).boundingBox();
        bbox.transformUsing(matrix);
        AiNodeSetVec(shape, "clip_min", bbox.min().x, bbox.min().y, bbox.min().z);
        AiNodeSetVec(shape, "clip_max", bbox.max().x, bbox.max().y, bbox.max().z);
    }else{
        AiNodeSetBool(shape, "clip", false);
    }
}

void BifrostTranslator::ExportOceanPlane(const MFnDagNode &dagNode, AtNode *shape){
    EXPORT_BOOL("enable_ocean_blending");
    EXPORT_FLT("ocean_blending_radius");
    MPlug oceanMeshPlug = dagNode.findPlug("ocean_plane");
    if(oceanMeshPlug.isDestination()){
        MPlug source = oceanMeshPlug.source();
        MMatrix matrix = this->getRelativeMatrix(source);
        MBoundingBox bbox = MFnDagNode(source.node()).boundingBox();
        bbox.transformUsing(matrix);
        float height = bbox.max().y;
        MPoint center = (bbox.max() + bbox.min())*.5;
        MPoint dimensions = (bbox.max() - bbox.min());
        AiNodeSetFlt(shape,"ocean_blending_height", height);
        AiNodeSetVec2(shape, "ocean_blending_center", center.x, center.z);
        AiNodeSetVec2(shape, "ocean_blending_dimension", dimensions.x, dimensions.z);
    }else{
        AiNodeSetBool(shape, "enable_ocean_blending", false);
    }
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

#define ADD_DRENDER_AS_ENUM(longName) \
    data.name = data.shortName = longName;\
    {\
        MStringArray enums;\
        enums.append("Surface");\
        enums.append("Points");\
        enums.append("Volume");\
        data.enums = enums;\
    }\
    helper.MakeInputEnum(data);

#define ADD_DSURFACE_TYPE_AS_ENUM(longName) \
    data.name = data.shortName = longName;\
    {\
        MStringArray enums;\
        enums.append("Mesh");\
        enums.append("Implicit");\
        data.enums = enums;\
    }\
    helper.MakeInputEnum(data);

#define ADD_DPOINTS_TYPE(longName) \
    data.name = data.shortName = longName;\
    {\
        MStringArray enums;\
        enums.append("Points");\
        enums.append("Spheres");\
        data.enums = enums;\
        data.defaultValue.INT() = 1;\
    }\
    helper.MakeInputEnum(data);


void BifrostTranslator::NodeInitializer( CAbTranslator context )
{
    CExtensionAttrHelper helper(context.maya, "standard");
    CAttrData data;

    // common
    ADD_DBOOL("opaque", true);
    ADD_DBOOL("matte", false);
    ADD_DBOOL("smoothing", true);
    ADD_DINT_HARDMIN("subdivisions", 1, 0);

    ADD_DRENDER_AS_ENUM("render_as");
    // TODO: remove the following attributes (non arnold specific)
    ADD_DDATA_ENUM("render_component");

    ADD_DFLT("velocity_scale", 1.f);
    ADD_DFLT("space_scale", 1.f);

    ADD_DSTR("channels", "vorticity");

    // surface
    ADD_DSURFACE_TYPE_AS_ENUM("surface_type");

    ADD_DFLT("implicit_step_size", 0.01);
    ADD_DINT("implicit_samples", 5);

    ADD_DSTR("distance_channel", "distance");

    ADD_DFLT("levelset_droplet_reveal_factor",3);
    ADD_DFLT("levelset_surface_radius", 1.4);
    ADD_DFLT("levelset_droplet_radius", 1.2);
    ADD_DFLT("levelset_resolution_factor",1);
    ADD_DFLT("levelset_max_volume_of_holes_to_close", 8);

    ADD_DFLT("dilate", 0.f);
    ADD_DFLT("smooth", 0.f);
    ADD_DSMOOTH_ENUM("smooth_mode");
    ADD_DINT_HARDMIN("smooth_iterations", 1, 1);
    ADD_DFLT("erode", 0.f);

    ADD_DBOOL("clip", false);
    {
        MStatus status;
        MFnTypedAttribute typeAttr;
        MObject obj = typeAttr.create("clip_box", "clip_box", MFnData::kMesh, &status);
        typeAttr.setStorable(false);
        typeAttr.setKeyable(true);
        typeAttr.setDisconnectBehavior(MFnAttribute::kReset);
        CHECK_MSTATUS(status);

        MDGModifier dgMod;
        if (dgMod.addExtensionAttribute(MString("bifrostShape"), obj) != MStatus::kSuccess){
           AiMsgError("[mtoa.attr] Unable to create extension attribute %s.%s", "bifrostShape", typeAttr.name().asChar());
        }else{
            AiMsgDebug("[mtoa.attr] Added extension attribute %s.%s", "bifrostShape", typeAttr.name().asChar());
            CHECK_MSTATUS(dgMod.doIt());
        }
    }

    ADD_DBOOL("enable_ocean_blending", false);
    {
        MStatus status;
        MFnTypedAttribute typeAttr;
        MObject obj = typeAttr.create("ocean_plane", "ocean_plane", MFnData::kMesh, &status);
        typeAttr.setStorable(false);
        typeAttr.setKeyable(true);
        typeAttr.setDisconnectBehavior(MFnAttribute::kReset);
        CHECK_MSTATUS(status);

        MDGModifier dgMod;
        if (dgMod.addExtensionAttribute(MString("bifrostShape"), obj) != MStatus::kSuccess){
           AiMsgError("[mtoa.attr] Unable to create extension attribute %s.%s", "bifrostShape", typeAttr.name().asChar());
        }else{
            AiMsgDebug("[mtoa.attr] Added extension attribute %s.%s", "bifrostShape", typeAttr.name().asChar());
            CHECK_MSTATUS(dgMod.doIt());
        }
    }
    ADD_DFLT("ocean_blending_radius", .1);

    // volume
    ADD_DSTR("density_channel", "density");

    // points
    ADD_DFLT("radius", 0.01);
    ADD_DPOINTS_TYPE("points_type");

    //ADD_DBOOL("enable_radius_channel", false);
    //ADD_DSTR("radius_channel", "density");

    //ADD_DBOOL("enable_camera_radius", false);
    //ADD_DFLT2("camera_distance", 0, 1);
    //ADD_DFLT2("camera_factor", 0, 1);
    //ADD_DFLT("camera_factor_exponent", 1);

    //ADD_DINT("skip", 0);
    ADD_DFLT("points_step_size", 0.025);
}
