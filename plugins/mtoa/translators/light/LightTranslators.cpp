#include "LightTranslators.h"
#include "utils/ConstantStrings.h"
#include <maya/MFnAreaLight.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnMesh.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MPlugArray.h>
#include <maya/MMatrix.h>

// DirectionalLight
//

void CDirectionalLightTranslator::Export(AtNode* light)
{
   if (light == nullptr)
      return;
   CLightTranslator::Export(light);

   AiNodeSetFlt(light, str::angle, FindMayaPlug("aiAngle").asFloat());
   AiNodeSetBool(light, str::cast_volumetric_shadows, FindMayaPlug("aiCastVolumetricShadows").asBool());
}

void CDirectionalLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "distant_light");
   // common attributes
   MakeCommonAttributes(helper);
   // directional light attributes
   helper.MakeInput("angle");
   helper.MakeInput("cast_volumetric_shadows");
}
// PointLight
//

void CPointLightTranslator::Export(AtNode* light)
{
   if (light == nullptr)
      return;

   CLightTranslator::Export(light);

   MPlug plug;
   MFnPointLight fnLight(m_dagPath);

   double radius = FindMayaPlug("aiRadius").asDouble() *  GetSessionOptions().GetScaleFactor(); 
   AiNodeSetFlt(light, str::radius, static_cast<float>(radius)); 

   AiNodeSetBool(light, str::cast_volumetric_shadows, FindMayaPlug("aiCastVolumetricShadows").asBool());

   AiNodeSetFlt(light, str::camera, FindMayaPlug("aiCamera").asFloat());
   AiNodeSetFlt(light, str::transmission, FindMayaPlug("aiTransmission").asFloat());
   
}

void CPointLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "point_light");
   // common attributes
   MakeCommonAttributes(helper);
   // point light attributes
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("radius");
   helper.MakeInput("camera");
   helper.MakeInput("transmission");
}

// SpotLight
//

void CSpotLightTranslator::Export(AtNode* light)
{
   if (light == nullptr)
      return;

   MPlug plug;
   MFnSpotLight fnLight(m_dagPath);

   CLightTranslator::Export(light);

   AiNodeSetFlt(light, str::cone_angle, static_cast<float>((fnLight.coneAngle() + AiMax(0.0, fnLight.penumbraAngle()) * 2.0f) * AI_RTOD));
   AiNodeSetFlt(light, str::penumbra_angle, static_cast<float>(fabs(fnLight.penumbraAngle()) * AI_RTOD));
   AiNodeSetFlt(light, str::cosine_power, static_cast<float>(fnLight.dropOff()));

   double radius = FindMayaPlug("aiRadius").asDouble() * GetSessionOptions().GetScaleFactor(); 
   AiNodeSetFlt(light, str::radius, static_cast<float>(radius)); 

   AiNodeSetBool(light, str::cast_volumetric_shadows, FindMayaPlug("aiCastVolumetricShadows").asBool());

   AiNodeSetFlt(light, str::aspect_ratio, FindMayaPlug("aiAspectRatio").asFloat());
   AiNodeSetFlt(light, str::lens_radius, FindMayaPlug("aiLensRadius").asFloat());
   AiNodeSetFlt(light, str::roundness, FindMayaPlug("aiRoundness").asFloat());
}

void CSpotLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "spot_light");
   // common attributes
   MakeCommonAttributes(helper);
   // spot light attributes
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("aspect_ratio");
   helper.MakeInput("radius");
   helper.MakeInput("lens_radius");
   helper.MakeInput("roundness");
}

// Quad AreaLight
//
// FIXME in which universe to flush the cache ?
void CQuadLightTranslator::Export(AtNode* light)
{
   if (light == nullptr)
      return;

   if (m_flushCache)
   {
      AiUniverseCacheFlush(GetUniverse(), AI_CACHE_QUAD);
      m_flushCache = false;
   }
   
   CLightTranslator::Export(light);

   AtVector vertices[4];

   vertices[3] = AtVector(1, 1, 0);
   vertices[0] = AtVector(1, -1, 0);
   vertices[1] = AtVector(-1, -1, 0);
   vertices[2] = AtVector(-1, 1, 0);

   AiNodeSetArray(light, str::vertices, AiArrayConvert(4, 1, AI_TYPE_VECTOR, vertices));

   AiNodeSetInt(light, str::resolution, FindMayaPlug("aiResolution").asInt());
   AiNodeSetFlt(light, str::spread, FindMayaPlug("aiSpread").asFloat());
   AiNodeSetFlt(light, str::roundness, FindMayaPlug("aiRoundness").asFloat());
   AiNodeSetFlt(light, str::soft_edge, FindMayaPlug("aiSoftEdge").asFloat());
   
   //AiNodeSetBool(light, "portal", FindMayaPlug("aiPortal").asBool()); removed it from here as we now have a dedicated light portal node
   AiNodeSetBool(light, str::cast_volumetric_shadows, FindMayaPlug("aiCastVolumetricShadows").asBool());
   
   // This translator is used both for Maya Area light and Arnold Quad light. Maya light has the shadowColor attribute,
   // so, if this attribute is found, it has already been exported in the CLightTranslator::Export method
   // If shadowColor is not found, it is an Arnold Quad light and we will need to export the aiShadowColor attribute
   MPlug shadowColorPlug = FindMayaPlug("shadowColor");
   if (shadowColorPlug.isNull())
   {
      AiNodeSetRGB(light, str::shadow_color, FindMayaPlug("aiShadowColorR").asFloat(), FindMayaPlug("aiShadowColorG").asFloat(), FindMayaPlug("aiShadowColorB").asFloat());
   }

   // Check if "color" is connected to a texture.
   // If so, we'll have to flush cache when color receives a signal
   MPlug colorPlug = FindMayaPlug("color");
   MPlugArray conn;
   colorPlug.connectedTo(conn, true, false);
   m_colorTexture = (conn.length() > 0);

   AiNodeSetFlt(light, str::camera, FindMayaPlug("aiCamera").asFloat());
   AiNodeSetFlt(light, str::transmission, FindMayaPlug("aiTransmission").asFloat());
   
}

void CQuadLightTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   CLightTranslator::NodeChanged(node, plug);

   // If nothing is plugged to the color attribute, we're done
   if (!m_colorTexture)
      return;

   // only the following parameters affect the quad light cache
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if (plugName == "color" || plugName == "resolution")
   {
      m_flushCache = true;
   }
}
void CQuadLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "quad_light");
   // common attributes
   MakeCommonAttributes(helper);
   // quad light attributes
   if (context.maya == MString("aiAreaLight"))
   {
      helper.MakeInput("shadow_color");
   }
   helper.MakeInput("resolution");
   helper.MakeInput("spread");
   helper.MakeInput("roundness");
   helper.MakeInput("soft_edge");
   //helper.MakeInput("portal"); removed it from here as we now have a dedicated light portal node
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("camera");
   helper.MakeInput("transmission");
}

// Cylinder AreaLight
//
void CCylinderLightTranslator::Export(AtNode* light)
{
   if (light == nullptr)
      return;

   CLightTranslator::Export(light);

   AiNodeSetBool(light, str::cast_volumetric_shadows, FindMayaPlug("aiCastVolumetricShadows").asBool());
   
   AiNodeSetRGB(light, str::shadow_color, FindMayaPlug("aiShadowColorR").asFloat(), FindMayaPlug("aiShadowColorG").asFloat(), FindMayaPlug("aiShadowColorB").asFloat());

   MTransformationMatrix tm(m_dagPath.inclusiveMatrix());
   double scale[3] = {1.0f, 1.0f, 1.0f};
   tm.getScale(scale, MSpace::kPreTransform);
   AiNodeSetFlt(light, str::radius, float (scale[0]+scale[2])/2.0f);

   AiNodeSetFlt(light, str::camera, FindMayaPlug("aiCamera").asFloat());
   AiNodeSetFlt(light, str::transmission, FindMayaPlug("aiTransmission").asFloat());
   
}

void CCylinderLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "cylinder_light");
   // common attributes
   MakeCommonAttributes(helper);
   helper.MakeInput("shadow_color");
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("camera");
   helper.MakeInput("transmission");
}

// Disk AreaLight
//
void CDiskLightTranslator::Export(AtNode* light)
{
   if (light == nullptr)
      return;

   CLightTranslator::Export(light);

   AiNodeSetBool(light, str::cast_volumetric_shadows, FindMayaPlug("aiCastVolumetricShadows").asBool());
   AiNodeSetFlt(light, str::spread, FindMayaPlug("aiSpread").asFloat());

   AiNodeSetRGB(light, str::shadow_color, FindMayaPlug("aiShadowColorR").asFloat(), FindMayaPlug("aiShadowColorG").asFloat(), FindMayaPlug("aiShadowColorB").asFloat());
   
   MTransformationMatrix tm(m_dagPath.inclusiveMatrix());
   double scale[3] = {1.0f, 1.0f, 1.0f};
   tm.getScale(scale, MSpace::kPreTransform);
   AiNodeSetFlt(light, str::radius, float (scale[0]+scale[1])/2.0f);

   AiNodeSetFlt(light, str::camera, FindMayaPlug("aiCamera").asFloat());
   AiNodeSetFlt(light, str::transmission, FindMayaPlug("aiTransmission").asFloat());
   
}

void CDiskLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "disk_light");
   // common attributes
   MakeCommonAttributes(helper);
   helper.MakeInput("shadow_color");
   helper.MakeInput("spread");
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("camera");
   helper.MakeInput("transmission");
}

void CSkyDomeLightTranslator::Export(AtNode* light)
{
   if (light == nullptr)
      return;

   if (m_flushCache)
   {
      AiUniverseCacheFlush(GetUniverse(), AI_CACHE_BACKGROUND);
      m_flushCache = false;
   }
   
   CLightTranslator::Export(light);

   AiNodeSetInt(light, str::resolution, FindMayaPlug("resolution").asInt());
   AiNodeSetInt(light, str::format, FindMayaPlug("format").asInt());
   AiNodeSetBool(light, str::cast_volumetric_shadows, FindMayaPlug("aiCastVolumetricShadows").asBool());
   AiNodeSetInt(light, str::portal_mode, FindMayaPlug("portal_mode").asInt());
   AiNodeSetFlt(light, str::camera, FindMayaPlug("camera").asFloat());
   AiNodeSetFlt(light, str::transmission, FindMayaPlug("transmission").asFloat());
   AiNodeSetBool(light, str::aov_indirect, FindMayaPlug("aiAovIndirect").asBool());
   MPlug shadowColorPlug = FindMayaPlug("aiShadowColor");
   if (!shadowColorPlug.isNull())
   {
      AiNodeSetRGB(light, str::shadow_color, shadowColorPlug.child(0).asFloat(), 
              shadowColorPlug.child(1).asFloat(), shadowColorPlug.child(2).asFloat());
   }
}

void CSkyDomeLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "skydome_light");
   // Cannot be created both on Node and here
   MakeCommonAttributes(helper);
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("shadow_color");
   helper.MakeInput("aov_indirect");
}

void CSkyDomeLightTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   // at next Export we'll want to flush the background cache.
   // This used to be done during the NodeDirty callback
   // but we must NOT interrupt renders or call arnold flush functions during maya's callbacks. 

   // only the following parameters affect the skydome cache
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if (plugName == "color" || plugName == "resolution" || plugName == "aiUseColorTemperature" 
      || plugName == "aiColorTemperature" || plugName == "format")
   {
      m_flushCache = true;
   }

   CLightTranslator::NodeChanged(node, plug);
}
void CPhotometricLightTranslator::Export(AtNode* light)
{
   if (light == nullptr)
      return;

   CLightTranslator::Export(light);
   AiNodeSetFlt(light, str::radius, FindMayaPlug("aiRadius").asFloat());
   AiNodeSetBool(light, str::cast_volumetric_shadows, FindMayaPlug("aiCastVolumetricShadows").asBool());

   MString filename = FindMayaPlug("aiFilename").asString();

   filename = filename.expandEnvironmentVariablesAndTilde();
   GetSessionOptions().FormatTexturePath(filename);
   
   AiNodeSetStr(light, str::filename, AtString(filename.asChar()));
   MPlug shadowColorPlug = FindMayaPlug("aiShadowColor");
   if (!shadowColorPlug.isNull())
   {
      AiNodeSetRGB(light, str::shadow_color, shadowColorPlug.child(0).asFloat(), 
              shadowColorPlug.child(1).asFloat(), shadowColorPlug.child(2).asFloat());
   }
}

void CPhotometricLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "photometric_light");
   // Cannot be created both on Node and here
   MakeCommonAttributes(helper);
   helper.MakeInput("radius");
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("shadow_color");
   helper.MakeInput("filename");

   // Register this parameter so that it appears in file path editor
   MString typeLabel;
   MGlobal::executeCommand("filePathEditor -query -typeLabel aiPhotometricLight.aiFilename", typeLabel);
   if (typeLabel != MString("IES"))
      MGlobal::executeCommand("filePathEditor -registerType aiPhotometricLight.aiFilename -typeLabel \"IES\"");
}

// Mesh AreaLight

double CalculateTriangleArea(const AtVector& p0, 
        const AtVector& p1, const AtVector& p2)
{
   const AtVector t0(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);
   const AtVector t1(p2.x - p0.x, p2.y - p0.y, p2.z - p0.z);
   return double(AiV3Length(AiV3Cross(t0, t1)) * 0.5f);
}

void NormalizeColor(const MObject& meshObject, AtRGB& color)
{
   MStatus status;
   MFnMesh mesh(meshObject, &status);
   const AtVector* vertices = (const AtVector*)mesh.getRawPoints(&status);

   const int numPolygons = mesh.numPolygons();
   double surfaceArea = 0.f;
   for (int i = 0; i < numPolygons; ++i)
   {
      MIntArray vidx;
      mesh.getPolygonVertices(i, vidx);
      const int vertexCount = vidx.length();
      if (vertexCount)
      {
         const AtVector p0 = vertices[vidx[0]];
         for (int j = 1; j < vertexCount - 1; ++j)
         {
            const AtVector p1 = vertices[vidx[j]];
            const AtVector p2 = vertices[vidx[j + 1]];
            surfaceArea += CalculateTriangleArea(p0, p1, p2);
         }
      }
   }
   color = color / (float)surfaceArea;
}

AtNode* CMeshLightTranslator::ExportSimpleMesh(const MObject& meshObject)
{
   MStatus status;
   MFnMesh mesh(meshObject, &status);
   if (!status) // simple mesh export at first, nothing to see here
      return NULL;

   m_numVertices = mesh.numVertices();
   if (m_numVertices == 0)
      return NULL;

   AtNode* meshNode = GetArnoldNode("mesh");
   // use Maya's smoothShading attribute for Arnold's polymesh.smoothing
   bool smoothing = true;
   // get Maya's smoothShading attribute from meshObject
   MFnDependencyNode fnNode(meshObject, &status);
   CHECK_MSTATUS(status);
   MPlug ssPlug = fnNode.findPlug("smoothShading", true, &status);
   CHECK_MSTATUS(status);
   smoothing = ssPlug.asBool(&status);
   CHECK_MSTATUS(status);
   AiNodeSetBool(meshNode, str::smoothing, smoothing);

   const AtVector* vertices = (const AtVector*)mesh.getRawPoints(&status);
   int steps = GetNumMotionSteps();
   bool deformMotion = RequiresMotionData() && IsMotionBlurEnabled(MTOA_MBLUR_DEFORM);
   AtArray* vlist = AiArrayAllocate(m_numVertices, deformMotion ? steps : 1, AI_TYPE_VECTOR);
   int vlistOffset = deformMotion ? m_numVertices * GetMotionStep() : 0;
   for (int i = 0; i < m_numVertices; ++i)
      AiArraySetVec(vlist, i + vlistOffset, vertices[i]);

   AiNodeSetArray(meshNode, str::vlist, vlist);

   const int numPolygons = mesh.numPolygons();
   AtArray* nsides = AiArrayAllocate(numPolygons, 1, AI_TYPE_UINT);

   unsigned int numIndices = 0;

   for(int i = 0; i < numPolygons; ++i)
   {
      int vertexCount = mesh.polygonVertexCount(i);
      numIndices += (unsigned int)vertexCount;
      AiArraySetUInt(nsides, i, vertexCount);
   }

   AiNodeSetArray(meshNode, str::nsides, nsides);

   bool exportUVs = false;
   int numUVSets = mesh.numUVSets();
   AtArray* uvidxs = 0;

   if (numUVSets > 0)
   {
      int numUVs = mesh.numUVs();
      if (numUVs > 0)
      {
         exportUVs = true;
         AtArray* uv = AiArrayAllocate(numUVs, 1, AI_TYPE_VECTOR2);
         uvidxs = AiArrayAllocate(numIndices, 1, AI_TYPE_UINT);
      
         MFloatArray uArray, vArray;
         mesh.getUVs(uArray, vArray);

         for (int j = 0; j < numUVs; ++j)
         {
            AtVector2 atv;
            atv.x = uArray[j];
            atv.y = vArray[j];
            AiArraySetVec2(uv, j, atv);
         }
         AiNodeSetArray(meshNode, str::uvlist, uv);
      }
   }

   AtArray* vidxs = AiArrayAllocate(numIndices, 1, AI_TYPE_UINT);
   int uv_id = 0;

   for(int i = 0, id = 0; i < numPolygons; ++i)
   {
      MIntArray vidx;
      int vertexCount = AiArrayGetUInt(nsides, i);
      mesh.getPolygonVertices(i, vidx);
      for (int j = 0; j < vertexCount; ++j)
      {
         AiArraySetUInt(vidxs, id, vidx[j]);
         if (exportUVs)
         {
            if (mesh.getPolygonUVid(i, j, uv_id) != MS::kSuccess)
            {
               uv_id = 0;
               AiMsgWarning("[MtoA] No uv coordinate exists for the default uv set at polygon %i at vertex %i on mesh %s.",
                            i, j, mesh.name().asChar());
            }
            AiArraySetUInt(uvidxs, id, uv_id);
         }
         ++id;
      }
   }
   AiNodeSetArray(meshNode, str::vidxs, vidxs);
   if (exportUVs)
      AiNodeSetArray(meshNode, str::uvidxs, uvidxs);

   AiNodeSetPtr(meshNode, str::shader, NULL);

   const int subdivision = FindMayaPlug("aiSubdivType").asInt();
   if (subdivision!=0)
   {
      if (subdivision==1)
         AiNodeSetStr(meshNode, str::subdiv_type, str::catclark);
      else
         AiNodeSetStr(meshNode, str::subdiv_type, str::linear);
      AiNodeSetByte(meshNode, str::subdiv_iterations, FindMayaPlug("aiSubdivIterations").asInt());
      AiNodeSetInt(meshNode, str::subdiv_adaptive_metric, FindMayaPlug("aiSubdivAdaptiveMetric").asInt());
      AiNodeSetFlt(meshNode, str::subdiv_adaptive_error, FindMayaPlug("aiSubdivPixelError").asFloat());
      AiNodeSetInt(meshNode, str::subdiv_adaptive_space, FindMayaPlug("aiSubdivAdaptiveSpace").asInt());
      AiNodeSetInt(meshNode, str::subdiv_uv_smoothing,  FindMayaPlug("aiSubdivUvSmoothing").asInt());
      AiNodeSetBool(meshNode, str::subdiv_smooth_derivs, FindMayaPlug("aiSubdivSmoothDerivs").asBool());
   }
   if (RequiresMotionData())
   {
      double motionStart, motionEnd;
      GetSessionOptions().GetMotionRange(motionStart, motionEnd);
      AiNodeSetFlt(meshNode, str::motion_start, (float)motionStart);
      AiNodeSetFlt(meshNode, str::motion_end, (float)motionEnd);
   }
   return meshNode;
}

MObject CMeshLightTranslator::GetMeshObject() const
{
   return m_dagPath.node();
}

void CMeshLightTranslator::Export(AtNode* light)
{
   if (light == nullptr)
      return;

   CLightTranslator::Export(light);
   
   MStatus status;
   
   MFnDependencyNode fnDepNode(m_dagPath.node());
   
   AiNodeSetRGB(light, str::shadow_color, FindMayaPlug("aiShadowColorR").asFloat(),
           FindMayaPlug("aiShadowColorG").asFloat(), FindMayaPlug("aiShadowColorB").asFloat());
   AiNodeSetBool(light, str::cast_volumetric_shadows, FindMayaPlug("aiCastVolumetricShadows").asBool());
   
   MObject meshObject = GetMeshObject();
   AtNode* meshNode = ExportSimpleMesh(meshObject);
   
   
   if (meshNode == NULL)
   {
      AiMsgWarning("[mtoa] Failed to export mesh for mesh_light");
      return;
   }
   
   ExportUserAttributes(meshNode, GetMayaObject(), this);

   AiNodeSetPtr(light, str::mesh, meshNode);

   AtNode* shaderTwoSided = GetArnoldNode("two_sided");
   AtNode* shaderRaySwitch = GetArnoldNode("shader");
   AtNode *shaderMult = GetArnoldNode("multShader");

   if (shaderTwoSided == NULL || shaderMult == NULL || shaderRaySwitch == NULL)
      return; // shouldn't happen

   AiNodeSetPtr(meshNode, str::shader, shaderRaySwitch);

   AiNodeLink(shaderMult, str::front, shaderTwoSided);
   // "back" should remain empty, so that it renders black on backfacing polygons
   
   AiNodeLink(shaderTwoSided, str::camera, shaderRaySwitch);
   AiNodeLink(shaderTwoSided, str::specular_transmission, shaderRaySwitch);
   // other ray types should render black

   if (FindMayaPlug("aiUseColorTemperature").asBool())
   {
      AtRGB color = ConvertKelvinToRGB(FindMayaPlug("aiColorTemperature").asFloat());
      AiNodeSetRGB(shaderMult, str::input1, color.r, color.g, color.b);
   }
   else
      ProcessParameter(shaderMult, str::input1, AI_TYPE_RGB, FindMayaPlug("color"));

   AiNodeSetArray(meshNode, str::matrix, AiArrayCopy(AiNodeGetArray(light, str::matrix)));
   if (fnDepNode.findPlug("lightVisible", true).asBool())
   {
      AiNodeSetByte(meshNode, str::visibility, AI_RAY_ALL);      
      AtRGB colorMultiplier = AI_RGB_WHITE;
      colorMultiplier = colorMultiplier * AiNodeGetFlt(light, str::intensity) * 
         powf(2.f, AiNodeGetFlt(light, str::exposure));      
      // if normalize is set to false, we need to multiply
      // the color with the surface area
      // doing a very simple triangulation, good for
      // approximating the Arnold one
      if (AiNodeGetBool(light, str::normalize))
         NormalizeColor(meshObject, colorMultiplier);
      
      AiNodeSetRGB(shaderMult, str::input2, colorMultiplier.r, colorMultiplier.g, colorMultiplier.b);
   }
   else
   {
      AiNodeSetByte(meshNode, str::visibility, AI_RAY_SPECULAR_REFLECT);
      AiNodeSetRGB(shaderMult, str::input2, 0.f, 0.f, 0.f);
   }
}


void CMeshLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "mesh_light");
   // common attributes
   MakeCommonAttributes(helper);
   helper.MakeInput("shadow_color");
   helper.MakeInput("cast_shadows");
   helper.MakeInput("cast_volumetric_shadows");
   CAttrData data;

   data.name = "color";
   data.shortName = "sc";
   data.defaultValue.RGB() = AI_RGB_WHITE;
   data.keyable = false;
   data.channelBox = false;
   helper.MakeInputRGB(data);

   data.name = "intensity";
   data.shortName = "intensity";
   data.min.FLT() = 0.0f;
   data.softMax.FLT() = 10.0f;
   data.defaultValue.FLT() = 1.0f;
   data.keyable = false;
   data.channelBox = false;
   helper.MakeInputFloat(data);

   data.name = "lightVisible";
   data.shortName = "light_visible";
   data.defaultValue.BOOL() = false;
   data.channelBox = false;
   helper.MakeInputBoolean(data);
}

void CMeshLightTranslator::ExportMotion(AtNode* light)
{
   if (light == nullptr)
      return;

   AtMatrix matrix;
   GetMatrix(matrix);
   int step = GetMotionStep();
   
   AtArray* matrices = AiNodeGetArray(light, str::matrix);
   AiArraySetMtx(matrices, step, matrix);
   
   AtNode* meshNode = (AtNode*)AiNodeGetPtr(light, str::mesh);
   if (meshNode != NULL) // just simply copy the matrices
   {
      AiNodeSetArray(meshNode, str::matrix, AiArrayCopy(AiNodeGetArray(light, str::matrix)));
      AtArray* vlist = AiNodeGetArray(meshNode, str::vlist);

      // As motion deform was disabled, I just allocated a single key
      if (AiArrayGetNumKeys(vlist) == 1)
         return;
       
      MFnDependencyNode fnDepNode(m_dagPath.node());
      MObject meshObject = GetMeshObject(); // if the returned value is directly given to the
      // MFnMesh constructor the mesh won't work, probably the MObject is destroyed somewhere
      // and we need to explicitly create the mobject on the stack to make it work
      MFnMesh mesh(meshObject); // no need to check the status, because if it
      // worked for the first time, it`s going to work for the second
      
      int numVerts = mesh.numVertices();
      if (numVerts != m_numVertices)
      {
         AiMsgWarning("[mtoa.translator]  %-30s | Number of vertices changed between motion steps: %d -> %d",
                    GetMayaNodeName().asChar(), m_numVertices, numVerts); // remove the vlist array and put one with a single key in it's plase

         AtArray* vlist_new = AiArrayAllocate(AiArrayGetNumElements(vlist), 1, AI_TYPE_VECTOR);

         for (int i = 0; i < m_numVertices; ++i)
            AiArraySetVec(vlist_new, i, AiArrayGetVec(vlist, i));

         AiNodeSetArray(meshNode, str::vlist, vlist_new);
      }
      else
      {
         MStatus status;
         const AtVector* vertices = (const AtVector*)mesh.getRawPoints(&status);
         for (int i = 0, j = m_numVertices * step; i < m_numVertices; ++i, ++j)
            AiArraySetVec(vlist, j, vertices[i]);
      }
      
   }  
}
void CMeshLightTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   const MString plugName = plug.partialName(false, false, false, false, false, true);
   
   bool recreate_geom = (plugName == "pnts" || plugName == "inMesh" || plugName == "dispResolution" || plugName == "useMeshSculptCache");
   recreate_geom = recreate_geom || (plugName.length() > 8 && plugName.substring(0,7) == "aiSubdiv")/*|| node.apiType() == MFn::kPluginShape*/;
   recreate_geom = recreate_geom || (plugName.indexW("mooth") >= 1);
   
   if (recreate_geom)
      SetUpdateMode(AI_RECREATE_NODE);
   
   CLightTranslator::NodeChanged(node, plug);
}


MObject CMeshLightNewTranslator::GetMeshObject() const
{
   MPlug inMesh = GetPlug(m_dagPath.node(), "inMesh");
   if (inMesh.isDestination())
   {
      MPlugArray plugArray;
      
      inMesh.connectedTo(plugArray,  true, false);
      MObject sourceNode;

      if (plugArray.length() > 0)
         sourceNode = plugArray[0].node();
      
      // equivalent code in maya 2017 
      //MObject sourceNode = inMesh.source().node();      
      
      if (sourceNode.hasFn(MFn::kMesh))
         return sourceNode;
      
   }
   return MObject::kNullObj;
}

void CMeshLightNewTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "mesh_light");
   // common attributes
   MakeCommonAttributes(helper);
   helper.MakeInput("shadow_color");
   helper.MakeInput("cast_shadows");
   helper.MakeInput("cast_volumetric_shadows");
}

void CMeshLightNewTranslator::AddUpdateCallbacks()
{
   // Need to add the update callbacks for the mesh object as well
   MObject meshObj = GetMeshObject();
   if (meshObj != MObject::kNullObj)
   {
      MStatus status;
      MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(meshObj,
                                                          NodeDirtyCallback,
                                                          this,
                                                          &status);
      if (MS::kSuccess == status) RegisterUpdateCallback(id);
   }
   // Call the base class to get the others.
   CDagTranslator::AddUpdateCallbacks();
}
