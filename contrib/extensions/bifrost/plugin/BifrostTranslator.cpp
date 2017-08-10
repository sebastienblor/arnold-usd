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
#include <maya/MFileIO.h>

#include <ai.h>

#include "extension/Extension.h"
#include "extension/ExtensionsManager.h"
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
#ifdef _WIN32
   static MString s_bifrostProceduralPath = "C:/Program Files/Autodesk/Bifrost/1.5.0/Arnold-5.0.0.0/bin";
   static MString s_bifrostProcedural = "bifrost_procedural_0_1";
#endif
#ifdef _LINUX
   static MString s_bifrostProceduralPath = "/usr/autodesk/bifrost/1.5.0/Arnold-5.0.0.0";
   static MString s_bifrostProcedural = "libbifrost_procedural_0_1";
#endif
#ifdef _DARWIN
   static MString s_bifrostProceduralPath = "/Applications/Autodesk/Bifrost/1.5.0/arnold-5.0.0.0";
   static MString s_bifrostProcedural = "libbifrost_procedural_0_1";  
#endif
   static bool s_loadedProcedural = false;

   static bool LoadBifrostProcedural()
   {
      if (s_loadedProcedural)
         return true;

      MString bifrostEnvVar;
      MGlobal::executeCommand("getenv BIFROST_ARNOLD_PATH", bifrostEnvVar);
      if (bifrostEnvVar.length() > 0)
      {
         MString firstChar = bifrostEnvVar.substringW(0, 0);
         // eventually remove quotes from the path
         if (firstChar == "\"")
            bifrostEnvVar = bifrostEnvVar.substringW(1, bifrostEnvVar.length() -2);
         
         s_bifrostProceduralPath = bifrostEnvVar;
      }
      
      CExtension *extension = CExtensionsManager::GetExtensionByName("bifrostTranslator");
      if (extension)
      {
         MFileObject fo;
         fo.setRawFullName(s_bifrostProceduralPath);
         if (fo.exists())
         {
            extension->LoadArnoldPlugin(s_bifrostProcedural, s_bifrostProceduralPath);
            s_loadedProcedural = true;
            return true;
         }
      }
      
      return false;
   }

   // Workaround to replace old auto-assigned bifrost material with standard (surface/volume) arnold shaders
   MCallbackId addedCbId = 0, connectionCbId = 0;

   void removeCallback(MCallbackId& id)
   {
      if(id != 0) MMessage::removeCallback(id);
      id = 0;
   }
   void bifrostShapeAttributeChanged(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void*)
   {
      if(msg & MNodeMessage::kConnectionMade && MFnAttribute(plug.attribute()).name()=="instObjGroups" && 
      MFnAttribute(otherPlug.attribute()).name()=="dagSetMembers")
      {
         // connection to shading engine made => replace shader
         int renderType = MFnDependencyNode(plug.node()).findPlug("bifrostRenderType").asInt();

         MPlug render_as = MFnDependencyNode(plug.node()).findPlug("render_as");
         std::cerr << "RENDER TYPE: " << renderType << std::endl;
         switch(renderType){
         case 0: render_as.setInt(2); break; // Aero => Volume
         case 3: render_as.setInt(1); break; // Foam => Points
         default: render_as.setInt(0); // Surface
         }

         bool isVolume = renderType==0 || renderType==3; // Aero or Foam
         MString shaderType = isVolume? "aiStandardVolume" : "aiStandardSurface";

         MFnDependencyNode shadingGroup(otherPlug.node());
         MString oldShader = MFnDependencyNode(shadingGroup.findPlug("surfaceShader").source().node()).name();// oddly, even aero has a surfaceShader

         MString command = "undoInfo -openChunk; $sel = `selectedNodes`;"; // next line doesn't work with createNode -skipSelection...
         command += "string $oldShader = \""+oldShader+"\";string $newShader = `createNode "+shaderType+"`;replaceNode $oldShader $newShader;delete $oldShader;";
         if(renderType == 0)
         { // Aero => set density channel to smoke
            command += "string $densityChannelPlg = $newShader+\".densityChannel\"; setAttr $densityChannelPlg -type \"string\" \"smoke\"; ";
         }
         if(isVolume)
         {
            command += "string $srcPlug = `connectionInfo -sfd \""+shadingGroup.name()+".surfaceShader\"`;disconnectAttr $srcPlug \""+shadingGroup.name()+".surfaceShader\"; connectAttr $srcPlug \""+shadingGroup.name()+".volumeShader\";";
         }

         MString preset;
         if(renderType==0) 
         { // aero
            preset = "aiStandardVolume/Smoke.mel";
         } else if(renderType==1 || renderType==2)
         { // liquid
            preset = "aiStandardSurface/Deep_Water.mel";
         } else { // foam
            preset = "aiStandardVolume/Foam.mel";
         }

         command += "string $presetPath = `getenv(\"MTOA_PATH\")`; $presetPath += \"presets/attrPresets/"+preset+"\"; applyPresetToNode $newShader \"\" \"\" $presetPath 1;";

         command += "select $sel;undoInfo -closeChunk;";
         MGlobal::executeCommandOnIdle(command);
         removeCallback(connectionCbId);
      }
   }
   void bifrostShapeAdded(MObject& obj, void*)
   {
      removeCallback(connectionCbId);
      if(!MFileIO::isReadingFile() && !MGlobal::isUndoing())
      {  // && !MGlobal::isRedoing() => Temporary: Redoing bifrostShape creation is clearing redo stack anyway (which is wrong), so replace shader again...
         // must wait until shaging engine is connected to shape, otherwise shader assignment will be overridden by the old bifrost material
         // => registering temporary attribute change callback and removing it after material assignment
         connectionCbId = MNodeMessage::addAttributeChangedCallback(obj, bifrostShapeAttributeChanged);
      }
   }

} // namespace


AtNode* BifrostTranslator::CreateArnoldNodes()
{    
   if (!LoadBifrostProcedural())
      AiMsgError("Bifrost to Arnold package not installed in %s" , s_bifrostProceduralPath.asChar());

   MFnDagNode dagNode(m_dagPath.node());
   if(dagNode.findPlug("tile_mode").asBool())
      return AddArnoldNode("bifrost_blocks");
   
   int render_as = dagNode.findPlug("render_as").asInt();
   switch(render_as)
   {
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
   if(dagNode.findPlug("tile_mode").asBool())
   {
      ExportImplicit(dagNode, shape);
      return;
   }
   int render_as = dagNode.findPlug("render_as").asInt();
   switch(render_as)
   {
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

   if(RequiresShaderExport())
      ExportBifrostShader();
   
   ExportLightLinking( shape );

   MPlug objectPlug = dagNode.findPlug("object");
   AiNodeSetStr(shape, "object", objectPlug.asString().asChar());

   MString attrName = MFnAttribute(objectPlug.source().attribute()).name();
   MString propPrefix;
   if(attrName == "Liquid")
      propPrefix = "liquid";
   else if(attrName == "Foam")
      propPrefix = "foam";
   else if(attrName == "AeroObject")
      propPrefix = "aero";
   
   if(propPrefix.length() != 0)
   {
      MFnDependencyNode container(objectPlug.source().node());
      MFnDependencyNode properties(container.findPlug(propPrefix+"CacheProperties").source().node());
      AiNodeSetStr(shape, "cache_folder", (properties.findPlug(propPrefix+"CachePath").asString() + properties.findPlug(propPrefix+"CacheFileName").asString()).asChar());
   }

   ExportMatrix(shape);
   AiNodeSetBool(shape, "ignore_motion_blur", !IsMotionBlurEnabled());
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
   EXPORT_INT("smooth_mode");
   EXPORT_UINT("smooth_iterations");

   EXPORT_BOOL("export_laplacian");
   EXPORT_BOOL("export_curvature");

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
   EXPORT_STR("density_channel");
   EXPORT2_FLT("volume_smooth", "smooth");
   EXPORT2_UINT("volume_smooth_iterations", "smooth_iterations");
   EXPORT2_FLT("volume_step_size", "step_size");
}

void BifrostTranslator::ExportPoints(MFnDagNode &dagNode, AtNode *shape)
{
   ExportShape(dagNode, shape);
   EXPORT_FLT("radius");

   EXPORT_BOOL("enable_radius_channel");
   EXPORT_INT("radius_channel");

   bool hasVolume = false;
   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), m_dagPath.instanceNumber());
   if(!shadingGroupPlug.isNull()){
       MFnDependencyNode engine(shadingGroupPlug.node());
       if(!engine.findPlug("aiSurfaceShader").isDestination() && !engine.findPlug("surfaceShader").isDestination()){
            hasVolume = true;
       }
   }
   float step_size = dagNode.findPlug("points_step_size").asFloat();
   int mode = dagNode.findPlug("points_type").asInt();

   if(hasVolume){
      if(step_size <= 0){
         AiMsgWarning("[BIFROST TRANSLATOR] Points step size value '%f' is invalid for rendering bifrost points shape '%s' with a volume shader. Should be > 0, using 0.1.",
                      step_size, m_dagPath.fullPathName().asChar());
         step_size = 0.1f;
      }
      if(mode != 1){
         AiMsgWarning("[BIFROST TRANSLATOR] Points type can only be rendered as 'Spheres' when rendering bifrost points shape '%s' with a volume shader. Ignoring point type, using 'Spheres'.",
                      m_dagPath.fullPathName().asChar());
         mode = 1; // spheres
      }
   }else{
      step_size = 0;
   }
   AiNodeSetFlt(shape, "step_size", step_size);
   AiNodeSetInt(shape, "mode", mode); // spheres
}

MMatrix BifrostTranslator::getRelativeMatrix(const MPlug &source){
   // TODO: make sure right dag path is used
   //unsigned int index = source.logicalIndex();
   MFnDagNode dag(source.node());
   MDagPath thisPath = m_dagPath; thisPath.pop();
   MDagPath otherPath; CHECK_MSTATUS(dag.getPath(otherPath));
   return otherPath.inclusiveMatrix() * thisPath.inclusiveMatrixInverse();
}

void BifrostTranslator::ExportClipping(const MFnDagNode &dagNode, AtNode *shape)
{
   EXPORT_BOOL("clip");
   MPlug clipBoxPlug = dagNode.findPlug("clip_box");
   if(clipBoxPlug.isDestination())
   {
      MPlug source = clipBoxPlug.source();
      MMatrix matrix = this->getRelativeMatrix(source);
      MBoundingBox bbox = MFnDagNode(source.node()).boundingBox();
      bbox.transformUsing(matrix);
      AiNodeSetVec(shape, "clip_min", (float)bbox.min().x, (float)bbox.min().y, (float)bbox.min().z);
      AiNodeSetVec(shape, "clip_max", (float)bbox.max().x, (float)bbox.max().y, (float)bbox.max().z);
   } else
   {
      AiNodeSetBool(shape, "clip", false);
   }
}

void BifrostTranslator::ExportOceanPlane(const MFnDagNode &dagNode, AtNode *shape)
{
   EXPORT_BOOL("enable_ocean_blending");
   EXPORT_FLT("ocean_blending_radius");
   MPlug oceanMeshPlug = dagNode.findPlug("ocean_plane");
   float height = 0;
   MPoint center(0,0,0);
   MPoint dimensions(0,0,0);
   if(oceanMeshPlug.isDestination())
   {
      MPlug source = oceanMeshPlug.source();
      MMatrix matrix = this->getRelativeMatrix(source);
      MBoundingBox bbox = MFnDagNode(source.node()).boundingBox();
      bbox.transformUsing(matrix);
      height = (float)bbox.max().y;
      center = (bbox.max() + bbox.min())*.5;
      dimensions = (bbox.max() - bbox.min());
   }
   height += dagNode.findPlug("ocean_blending_offsetsY").asFloat();
   dimensions.x += dagNode.findPlug("ocean_blending_offsetsX").asFloat();
   dimensions.z += dagNode.findPlug("ocean_blending_offsetsZ").asFloat();

   AiNodeSetFlt(shape,"ocean_blending_height", height);
   AiNodeSetVec2(shape, "ocean_blending_center", (float)center.x, (float)center.z);
   AiNodeSetVec2(shape, "ocean_blending_dimension", (float)dimensions.x, (float)dimensions.z);
   EXPORT_STR("ocean_blending_out_channel");
}

void BifrostTranslator::ExportMotion( AtNode* shape ){
   if(IsMotionBlurEnabled()){
      ExportMatrix(shape);
   }
}

void BifrostTranslator::ExportBifrostShader(){
   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), m_dagPath.instanceNumber());
   if ( !shadingGroupPlug.isNull() ) {
      AtNode *rootShader = ExportConnectedNode(shadingGroupPlug);
      if (rootShader != NULL) {
         // Push the shader in the vector to be assigned later to mtoa_shading_groups
         AiNodeSetPtr(GetArnoldNode(), "shader", rootShader);
      }
   }
}

namespace {

   void GetDisplacement(MObject& obj, float& dispPadding)
   {
      MFnDependencyNode dNode(obj);
      MPlug plug = dNode.findPlug("aiDisplacementPadding");
      if (!plug.isNull())
         dispPadding = plug.asFloat();
   }

} // namespace

void BifrostTranslator::ExportDisplacement()
{
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
      enums.append("Laplacian Flow");\
      enums.append("Curvature Flow");\
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

   ADD_DBOOL("tile_mode", false);

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

   ADD_DFLT("implicit_step_size", 0.01f);
   ADD_DINT("implicit_samples", 5);

   ADD_DSTR("distance_channel", "distance");

   ADD_DFLT("levelset_droplet_reveal_factor",3);
   ADD_DFLT("levelset_surface_radius", 1.4f);
   ADD_DFLT("levelset_droplet_radius", 1.2f);
   ADD_DFLT("levelset_resolution_factor",1);
   ADD_DFLT("levelset_max_volume_of_holes_to_close", 8);

   ADD_DFLT("dilate", 0.f);
   ADD_DFLT("smooth", 0.f);
   ADD_DSMOOTH_ENUM("smooth_mode");
   ADD_DINT_HARDMIN("smooth_iterations", 1, 1);
   ADD_DFLT("erode", 0.f);

   ADD_DBOOL("export_laplacian", false);
   ADD_DBOOL("export_curvature", false);

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
      if (dgMod.addExtensionAttribute(MString("bifrostShape"), obj) != MStatus::kSuccess)
      {
         AiMsgError("[mtoa.attr] Unable to create extension attribute %s.%s", "bifrostShape", typeAttr.name().asChar());
      } else
      {
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
      if (dgMod.addExtensionAttribute(MString("bifrostShape"), obj) != MStatus::kSuccess)
      {
         AiMsgError("[mtoa.attr] Unable to create extension attribute %s.%s", "bifrostShape", typeAttr.name().asChar());
      } else
      {
         AiMsgDebug("[mtoa.attr] Added extension attribute %s.%s", "bifrostShape", typeAttr.name().asChar());
         CHECK_MSTATUS(dgMod.doIt());
      }
   }
   ADD_DFLT("ocean_blending_radius", 0);
   ADD_DFLT3("ocean_blending_offsets", 0, 0, 0);
   ADD_DSTR("ocean_blending_out_channel", "");

   // volume
   ADD_DSTR("density_channel", "density");
   ADD_DFLT("volume_smooth", 0);
   ADD_DINT("volume_smooth_iterations", 1);
   ADD_DFLT("volume_step_size", 0.1f);

   // points
   ADD_DFLT("radius", 0.01f);
   ADD_DPOINTS_TYPE("points_type");
   ADD_DFLT("points_step_size", 0.025f);

   ADD_DBOOL("enable_radius_channel", false);
   ADD_DSTR("radius_channel", "density");

   ADD_DINT("chunk_size", 100000);

   if (!LoadBifrostProcedural())
      AiMsgWarning("Bifrost to Arnold package not installed in %s" , s_bifrostProceduralPath.asChar());


   // add callback when a bifrost shape is created, so that we can assign the right material
   addedCbId = MDGMessage::addNodeAddedCallback(bifrostShapeAdded, "bifrostShape");
}

void BifrostTranslator::ClearCallbacks()
{
   removeCallback(addedCbId);
}
