#include "LightTranslators.h"

#include <maya/MFnAreaLight.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnMesh.h>
#include <maya/MItMeshPolygon.h>

// DirectionalLight
//

void CDirectionalLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AiNodeSetFlt(light, "angle", FindMayaPlug("aiAngle").asFloat());
   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());
}

void CDirectionalLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "distant_light");
   // common attributes
   MakeCommonAttributes(helper);
   // directional light attributes
   helper.MakeInput("angle");
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
}
// PointLight
//

void CPointLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   MPlug plug;
   MFnPointLight fnLight(m_dagPath);

   float radius = FindMayaPlug("aiRadius").asFloat(); 
   m_session->ScaleDistance(radius); 
   AiNodeSetFlt(light, "radius", radius); 

   AiNodeSetInt(light,  "decay_type",      FindMayaPlug("aiDecayType").asInt());
   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());
}

void CPointLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "point_light");
   // common attributes
   MakeCommonAttributes(helper);
   // point light attributes
   helper.MakeInput("decay_type");
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("radius");
}

// SpotLight
//

void CSpotLightTranslator::Export(AtNode* light)
{
   MPlug plug;
   MFnSpotLight fnLight(m_dagPath);

   CLightTranslator::Export(light);

   AiNodeSetFlt(light, "cone_angle", static_cast<float>((fnLight.coneAngle() + MAX(0.0, fnLight.penumbraAngle()) * 2.0f) * AI_RTOD));
   AiNodeSetFlt(light, "penumbra_angle", static_cast<float>(fabs(fnLight.penumbraAngle()) * AI_RTOD));
   AiNodeSetFlt(light, "cosine_power", static_cast<float>(fnLight.dropOff()));

   float radius = FindMayaPlug("aiRadius").asFloat(); 
   m_session->ScaleDistance(radius); 
   AiNodeSetFlt(light, "radius", radius); 

   AiNodeSetInt(light,  "decay_type",      FindMayaPlug("aiDecayType").asInt());
   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());

   AiNodeSetFlt(light, "aspect_ratio", FindMayaPlug("aiAspectRatio").asFloat());
   AiNodeSetFlt(light, "lens_radius", FindMayaPlug("aiLensRadius").asFloat());
}

void CSpotLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "spot_light");
   // common attributes
   MakeCommonAttributes(helper);
   // spot light attributes
   helper.MakeInput("decay_type");
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("aspect_ratio");
   helper.MakeInput("radius");
   helper.MakeInput("lens_radius");
}

// Quad AreaLight
//

void CQuadLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AtPoint vertices[4];

   AiV3Create(vertices[3], 1, 1, 0);
   AiV3Create(vertices[0], 1, -1, 0);
   AiV3Create(vertices[1], -1, -1, 0);
   AiV3Create(vertices[2], -1, 1, 0);

   AiNodeSetArray(light, "vertices", AiArrayConvert(4, 1, AI_TYPE_POINT, vertices));

   AiNodeSetInt(light,  "decay_type",      FindMayaPlug("aiDecayType").asInt());
   AiNodeSetInt(light, "resolution", FindMayaPlug("aiResolution").asInt());
   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());
   
   // This translator is used both for Maya Area light and Arnold Quad light. Maya light has the shadowColor attribute,
   // so, if this attribute is found, it has already been exported in the CLightTranslator::Export method
   // If shadowColor is not found, it is an Arnold Quad light and we will need to export the aiShadowColor attribute
   MPlug shadowColorPlug = FindMayaPlug("shadowColor");
   if (shadowColorPlug.isNull())
   {
      AiNodeSetRGB(light, "shadow_color", FindMayaPlug("aiShadowColorR").asFloat(), FindMayaPlug("aiShadowColorG").asFloat(), FindMayaPlug("aiShadowColorB").asFloat());
   }
}

void CQuadLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "quad_light");
   // common attributes
   MakeCommonAttributes(helper);
   // quad light attributes
   helper.MakeInput("shadow_color");
   helper.MakeInput("decay_type");
   helper.MakeInput("resolution");
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
}

// Cylinder AreaLight
//
void CCylinderLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AiNodeSetInt(light,  "decay_type",      FindMayaPlug("aiDecayType").asInt());
   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());
   
   AiNodeSetRGB(light, "shadow_color", FindMayaPlug("aiShadowColorR").asFloat(), FindMayaPlug("aiShadowColorG").asFloat(), FindMayaPlug("aiShadowColorB").asFloat());

   MTransformationMatrix tm(m_dagPath.inclusiveMatrix());
   double scale[3] = {1.0f, 1.0f, 1.0f};
   tm.getScale(scale, MSpace::kPreTransform);
   AiNodeSetFlt(light, "radius", float (scale[0]+scale[2])/2.0f);
}

void CCylinderLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "cylinder_light");
   // common attributes
   MakeCommonAttributes(helper);
   helper.MakeInput("shadow_color");
   helper.MakeInput("decay_type");
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
}

// Disk AreaLight
//
void CDiskLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AiNodeSetInt(light,  "decay_type",      FindMayaPlug("aiDecayType").asInt());
   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());
   
   AiNodeSetRGB(light, "shadow_color", FindMayaPlug("aiShadowColorR").asFloat(), FindMayaPlug("aiShadowColorG").asFloat(), FindMayaPlug("aiShadowColorB").asFloat());
   
   MTransformationMatrix tm(m_dagPath.inclusiveMatrix());
   double scale[3] = {1.0f, 1.0f, 1.0f};
   tm.getScale(scale, MSpace::kPreTransform);
   AiNodeSetFlt(light, "radius", float (scale[0]+scale[1])/2.0f);
}

void CDiskLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "disk_light");
   // common attributes
   MakeCommonAttributes(helper);
   helper.MakeInput("shadow_color");
   helper.MakeInput("decay_type");
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
}

void CSkyDomeLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AiNodeSetInt(light, "resolution", FindMayaPlug("resolution").asInt());
   AiNodeSetInt(light, "format", FindMayaPlug("format").asInt());
   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());
   MPlug shadowColorPlug = FindMayaPlug("aiShadowColor");
   if (!shadowColorPlug.isNull())
   {
      AiNodeSetRGB(light, "shadow_color", shadowColorPlug.child(0).asFloat(), 
              shadowColorPlug.child(1).asFloat(), shadowColorPlug.child(2).asFloat());
   }
}

void CSkyDomeLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "skydome_light");
   // Cannot be created both on Node and here
   MakeCommonAttributes(helper);
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("shadow_color");
}


void CPhotometricLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());
   AiNodeSetStr(light, "filename", FindMayaPlug("aiFilename").asString().asChar());
   MPlug shadowColorPlug = FindMayaPlug("aiShadowColor");
   if (!shadowColorPlug.isNull())
   {
      AiNodeSetRGB(light, "shadow_color", shadowColorPlug.child(0).asFloat(), 
              shadowColorPlug.child(1).asFloat(), shadowColorPlug.child(2).asFloat());
   }
}

void CPhotometricLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "photometric_light");
   // Cannot be created both on Node and here
   MakeCommonAttributes(helper);
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("shadow_color");
   helper.MakeInput("filename");
}

// Mesh AreaLight

double CalculateTriangleArea(const AtVector& p0, 
        const AtVector& p1, const AtVector& p2)
{
   const AtVector t0 = {p1.x - p0.x, p1.y - p0.y, p1.z - p0.z};
   const AtVector t1 = {p2.x - p0.x, p2.y - p0.y, p2.z - p0.z};
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

   const AtVector* vertices = (const AtVector*)mesh.getRawPoints(&status);
   int steps = GetNumMotionSteps();
   AtArray* vlist = AiArrayAllocate(m_numVertices, IsMotionBlurEnabled(MTOA_MBLUR_DEFORM) ? steps : 1, AI_TYPE_POINT);
   for (int i = 0; i < m_numVertices; ++i)
      AiArraySetVec(vlist, i, vertices[i]);

   AiNodeSetArray(meshNode, "vlist", vlist);

   const int numPolygons = mesh.numPolygons();
   AtArray* nsides = AiArrayAllocate(numPolygons, 1, AI_TYPE_UINT);

   unsigned int numIndices = 0;

   for(int i = 0; i < numPolygons; ++i)
   {
      int vertexCount = mesh.polygonVertexCount(i);
      numIndices += (unsigned int)vertexCount;
      AiArraySetUInt(nsides, i, vertexCount);
   }

   AiNodeSetArray(meshNode, "nsides", nsides);

   bool exportUVs = false;
   int numUVSets = mesh.numUVSets();
   AtArray* uvidxs = 0;

   if (numUVSets > 0)
   {
      int numUVs = mesh.numUVs();
      if (numUVs > 0)
      {
         exportUVs = true;
         AtArray* uv = AiArrayAllocate(numUVs, 1, AI_TYPE_POINT2);
         uvidxs = AiArrayAllocate(numIndices, 1, AI_TYPE_UINT);
      
         MFloatArray uArray, vArray;
         mesh.getUVs(uArray, vArray);

         for (int j = 0; j < numUVs; ++j)
         {
            AtPoint2 atv;
            atv.x = uArray[j];
            atv.y = vArray[j];
            AiArraySetPnt2(uv, j, atv);
         }
         AiNodeSetArray(meshNode, "uvlist", uv);
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
   AiNodeSetArray(meshNode, "vidxs", vidxs);
   if (exportUVs)
      AiNodeSetArray(meshNode, "uvidxs", uvidxs);

   AiNodeSetPtr(meshNode, "shader", NULL);

   const int subdivision = FindMayaPlug("aiSubdivType").asInt();
   if (subdivision!=0)
   {
      if (subdivision==1)
         AiNodeSetStr(meshNode, "subdiv_type",           "catclark");
      else
         AiNodeSetStr(meshNode, "subdiv_type",           "linear");
      AiNodeSetByte(meshNode, "subdiv_iterations",     FindMayaPlug("aiSubdivIterations").asInt());
      AiNodeSetInt(meshNode, "subdiv_adaptive_metric",FindMayaPlug("aiSubdivAdaptiveMetric").asInt());
      AiNodeSetFlt(meshNode, "subdiv_adaptive_error",    FindMayaPlug("aiSubdivPixelError").asFloat());
      AiNodeSetInt(meshNode, "subdiv_adaptive_space",    FindMayaPlug("aiSubdivAdaptiveSpace").asInt());
      AiNodeSetInt(meshNode, "subdiv_uv_smoothing",   FindMayaPlug("aiSubdivUvSmoothing").asInt());
      AiNodeSetBool(meshNode, "subdiv_smooth_derivs", FindMayaPlug("aiSubdivSmoothDerivs").asBool());

      ProcessParameter(meshNode, "subdiv_dicing_camera", AI_TYPE_NODE, "aiSubdivDicingCamera");
   }
   return meshNode;
}

MObject CMeshLightTranslator::GetMeshObject() const
{
   return m_dagPath.node();
}

void CMeshLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);
   
   MStatus status;
   
   MFnDependencyNode fnDepNode(m_dagPath.node());
   
   AiNodeSetInt(light, "decay_type", FindMayaPlug("aiDecayType").asInt());
   AiNodeSetRGB(light, "shadow_color", FindMayaPlug("aiShadowColorR").asFloat(),
           FindMayaPlug("aiShadowColorG").asFloat(), FindMayaPlug("aiShadowColorB").asFloat());
   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());
   
   MObject meshObject = GetMeshObject();
   AtNode* meshNode = ExportSimpleMesh(meshObject);
   if (meshNode == NULL)
   {
      AiMsgWarning("[mtoa] Failed to export mesh for mesh_light");
      return;
   }

   AiNodeSetPtr(light, "mesh", meshNode);

   AtNode* shaderNode = GetArnoldNode("shader");
   AiNodeSetPtr(meshNode, "shader", shaderNode);

   if (FindMayaPlug("aiUseColorTemperature").asBool())
   {
      AtRGB color = ConvertKelvinToRGB(FindMayaPlug("aiColorTemperature").asFloat());
      AiNodeSetRGB(shaderNode, "color", color.r, color.g, color.b);
   }
   else
      ProcessParameter(shaderNode, "color", AI_TYPE_RGB, FindMayaPlug("color"));

   AiNodeSetArray(meshNode, "matrix", AiArrayCopy(AiNodeGetArray(light, "matrix")));
   if (fnDepNode.findPlug("lightVisible").asBool())
   {      
      AiNodeSetByte(meshNode, "visibility", AI_RAY_ALL);
      
      AtRGB colorMultiplier = AI_RGB_WHITE;
      const float light_gamma = AiNodeGetFlt(AiUniverseGetOptions(), "light_gamma");
      AiColorGamma(&colorMultiplier, light_gamma);
      colorMultiplier = colorMultiplier * AiNodeGetFlt(light, "intensity") * 
         powf(2.f, AiNodeGetFlt(light, "exposure"));
      
      // if normalize is set to false, we need to multiply
      // the color with the surface area
      // doing a very simple triangulation, good for
      // approximating the Arnold one
      if (AiNodeGetBool(light, "normalize"))
         NormalizeColor(meshObject, colorMultiplier);
      
      AiNodeSetRGB(shaderNode, "color_multiplier", colorMultiplier.r, colorMultiplier.g, colorMultiplier.b);
   }
   else
   {
      AiNodeSetByte(meshNode, "visibility", AI_RAY_GLOSSY);
      AiNodeSetRGB(shaderNode, "color_multiplier", 0.f, 0.f, 0.f);
   }
}

void CMeshLightTranslator::Delete()
{
   for (std::map<std::string, AtNode*>::iterator it = m_atNodes.begin(); it != m_atNodes.end(); ++it)
      AiNodeDestroy(it->second);
   m_atNode = NULL;
   m_atNodes.clear();
}

void CMeshLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "mesh_light");
   // common attributes
   MakeCommonAttributes(helper);
   helper.MakeInput("shadow_color");
   helper.MakeInput("decay_type");
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_shadows");
   helper.MakeInput("cast_volumetric_shadows");
   CAttrData data;

   data.name = "color";
   data.shortName = "sc";
   data.defaultValue.RGB = AI_RGB_WHITE;
   data.keyable = false;
   data.channelBox = true;
   helper.MakeInputRGB(data);

   data.name = "intensity";
   data.shortName = "intensity";
   data.min.FLT = 0.0f;
   data.softMax.FLT = 10.0f;
   data.defaultValue.FLT = 1.0f;
   data.keyable = false;
   data.channelBox = true;
   helper.MakeInputFloat(data);

   data.name = "emitDiffuse";
   data.shortName = "emitDiffuse";
   data.defaultValue.BOOL = true;
   data.keyable = false;
   data.channelBox = true;
   helper.MakeInputBoolean(data);

   data.name = "emitSpecular";
   data.shortName = "emitSpecular";
   data.defaultValue.BOOL = true;
   data.keyable = false;
   data.channelBox = true;
   helper.MakeInputBoolean(data);

   data.name = "lightVisible";
   data.shortName = "light_visible";
   data.defaultValue.BOOL = false;
   data.channelBox = false;
   helper.MakeInputBoolean(data);
}

void CMeshLightTranslator::ExportMotion(AtNode* light, unsigned int step)
{
   AtMatrix matrix;
   GetMatrix(matrix);

   AtArray* matrices = AiNodeGetArray(light, "matrix");
   AiArraySetMtx(matrices, step, matrix);
   
   AtNode* meshNode = (AtNode*)AiNodeGetPtr(light, "mesh");
   if (meshNode != NULL) // just simply copy the matrices
   {
      AiNodeSetArray(meshNode, "matrix", AiArrayCopy(AiNodeGetArray(light, "matrix")));
      AtArray* vlist = AiNodeGetArray(meshNode, "vlist");

      if (vlist->nkeys == 1)
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

         AtArray* vlist_new = AiArrayAllocate(vlist->nelements, 1, AI_TYPE_POINT);

         for (int i = 0; i < m_numVertices; ++i)
            AiArraySetVec(vlist_new, i, AiArrayGetVec(vlist, i));

         AiNodeSetArray(meshNode, "vlist", vlist_new);
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
