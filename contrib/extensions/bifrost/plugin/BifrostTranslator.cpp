#include "BifrostTranslator.h"

#include <maya/MObject.h>
#include <maya/MFileObject.h>
#include <maya/MTime.h>
#include <maya/MMatrix.h>
#include <maya/MFnTransform.h>
#include <maya/MBoundingBox.h>
#include <maya/MFnMeshData.h>

#include <ai.h>

#include "extension/Extension.h"
#include "extension/ExtensionsManager.h"
#include "compatibility.h"
#include "helpers.h"

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

   AiNodeSetStr(shape, "field_channel", "distance");

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
   const float3& offsets = dagNode.findPlug("ocean_blending_offsets").asMDataHandle().asFloat3();
   height += offsets[1];
   dimensions.x += offsets[0];
   dimensions.z += offsets[2];

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

void BifrostTranslator::NodeInitializer( CAbTranslator context )
{
   ArnoldBifrost::MAttrData extData(context.maya, "bifrostShape");

   // common
   extData.setup("opaque",   "opaque", false).makeInput();
   extData.setup("matte",    "matte", false).makeInput();
   extData.setup("tileMode", "tile_mode", false).makeInput();

   // surface
   extData.setup("surfaceType",      "surface_type",       std::initializer_list<const char*>{"Mesh","Implicit"}).makeInput();
   extData.setup("smoothing",        "smoothing",          true).makeInput();
   extData.setup("subdivisions",     "subdivisions",       1).setSoft(1,5).setMin(1).makeInput();
   extData.setup("implicitStepSize", "implicit_step_size", 0.025f).setSoft(0.f,1.f).setMin(0.f).makeInput();
   extData.setup("implicitSamples",  "implicit_samples",   4).setSoft(1,10).setMin(1).makeInput();

   // points
   extData.setup("pointsType",     "points_type",      std::initializer_list<const char*>{"Points","Spheres"}).setDefault(1).makeInput();
   extData.setup("pointsStepSize", "points_step_size", 0.025f).setSoft(0.f,1.f).setMin(0.f).makeInput();
   extData.setup("chunkSize",      "chunk_size",       100000).setMin(1).makeInput();

   // volume
   extData.setup("volumeStepSize", "volume_step_size", 0.025f).setSoft(0.f,1.f).setMin(0.f).makeInput();

   ArnoldBifrost::Compatibility::NodeInitializer(context);

   if (!LoadBifrostProcedural())
      AiMsgWarning("Bifrost to Arnold package not installed in %s" , s_bifrostProceduralPath.asChar());
}

void BifrostTranslator::Uninitialize()
{
   ArnoldBifrost::Compatibility::Uninitialize();
}
