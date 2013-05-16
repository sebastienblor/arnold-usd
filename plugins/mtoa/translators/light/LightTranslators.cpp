#include "LightTranslators.h"

#include <maya/MFnAmbientLight.h>
#include <maya/MFnAreaLight.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnMesh.h>
#include <maya/MItMeshPolygon.h>

// AmbientLight
//

void CAmbientLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);
}

void CAmbientLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "ambient_light");
   MakeCommonAttributes(helper);
}

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

   AiNodeSetFlt(light, "radius", FindMayaPlug("aiRadius").asFloat());

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

   AiNodeSetFlt(light, "cone_angle", static_cast<float>((fnLight.coneAngle() + fnLight.penumbraAngle()) * AI_RTOD));
   AiNodeSetFlt(light, "penumbra_angle", static_cast<float>(fabs(fnLight.penumbraAngle()) * AI_RTOD));
   AiNodeSetFlt(light, "cosine_power", static_cast<float>(fnLight.dropOff()));

   AiNodeSetFlt(light, "radius", FindMayaPlug("aiRadius").asFloat());

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

   AiV3Create(vertices[0], 1, 1, 0);
   AiV3Create(vertices[1], 1, -1, 0);
   AiV3Create(vertices[2], -1, -1, 0);
   AiV3Create(vertices[3], -1, 1, 0);

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

// SkyDomeLight
//
void CSkyDomeLightTranslator::GetMatrix(AtMatrix& matrix)
{
   MTransformationMatrix tm(m_dagPath.inclusiveMatrix());
   // Invert in Z to account for the env sphere being viewed from inside
   double scale[3] = {1.0, 1.0, -1.0};
   tm.addScale(scale, MSpace::kPreTransform);
   MMatrix m = tm.asMatrix();

   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) m[I][J];
      }
   }
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

double CalculateTriangleArea(const AtVector& p0, 
        const AtVector& p1, const AtVector& p2)
{
   const AtVector t0 = {p1.x - p0.x, p1.y - p0.y, p1.z - p0.z};
   const AtVector t1 = {p2.x - p0.x, p2.y - p0.y, p2.z - p0.z};
   return double(AiV3Length(AiV3Cross(t0, t1)) * 0.5f);
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
   
   MPlug plug = fnDepNode.findPlug("inputMesh");
   MObject meshObject;
   plug.getValue(meshObject);
   MFnMesh mesh(meshObject, &status); 
   if (!status) // simple mesh export at first, nothing to see here
      return;
   
   m_numVertices = mesh.numVertices();
   
   if (m_numVertices == 0)
      return;
   
   MString nodeName = AiNodeGetName(light);
   AtNode* meshNode = (AtNode*)AiNodeGetPtr(light, "mesh");
   if (meshNode == 0)
   {
      meshNode = AiNode("polymesh");
      AiNodeSetStr(meshNode, "name", (nodeName + MString("_mesh")).asChar());
   
      const AtVector* vertices = (const AtVector*)mesh.getRawPoints(&status);
      AtArray* vlist = AiArrayAllocate(m_numVertices, GetNumMotionSteps(), AI_TYPE_POINT);
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

      AtArray* vidxs = AiArrayAllocate(numIndices, 1, AI_TYPE_UINT);

      for(int i = 0, id = 0; i < numPolygons; ++i)
      {
         MIntArray vidx;
         int vertexCount = AiArrayGetUInt(nsides, i);
         mesh.getPolygonVertices(i, vidx);
         for (int j = 0; j < vertexCount; ++j)
            AiArraySetUInt(vidxs, id++, vidx[j]);  
      }
      AiNodeSetArray(meshNode, "vidxs", vidxs);

      AiNodeSetPtr(light, "mesh", meshNode);
      AiNodeSetPtr(meshNode, "shader", 0);
   }

   AtNode* shaderNode = (AtNode*)AiNodeGetPtr(meshNode, "shader");
   if (shaderNode == 0)
   {
      shaderNode = AiNode("meshLightMaterial");
      AiNodeSetStr(shaderNode, "name", (nodeName + MString("_shader")).asChar());
      AiNodeSetPtr(meshNode, "shader", shaderNode);
   }

   AiNodeSetArray(meshNode, "matrix", AiArrayCopy(AiNodeGetArray(light, "matrix")));
   if (fnDepNode.findPlug("lightVisible").asBool())
   {      
      AiNodeSetInt(meshNode, "visibility", AI_RAY_ALL);
      
      AtRGB color = AiNodeGetRGB(light, "color");
      const float light_gamma = AiNodeGetFlt(AiUniverseGetOptions(), "light_gamma");
      AiColorGamma(&color, light_gamma);
      color = color * AiNodeGetFlt(light, "intensity") * 
         powf(2.f, AiNodeGetFlt(light, "exposure"));      
      
      // if normalize is set to false, we need to multiply
      // the color with the surface area
      // doing a very simple triangulation, good for
      // approximating the Arnold one
      if (AiNodeGetBool(light, "normalize"))
      {
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
      
      AiNodeSetRGB(shaderNode, "color", color.r, color.g, color.b);
   }
   else
   {
      AiNodeSetInt(meshNode, "visibility", AI_RAY_GLOSSY);
      AiNodeSetRGB(shaderNode, "color", 0.f, 0.f, 0.f);
   }
}

void CMeshLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "mesh_light");
   // common attributes
   MakeCommonAttributes(helper);
   helper.MakeInput("shadow_color");
   helper.MakeInput("decay_type");
   
}

void CMeshLightTranslator::ExportMotion(AtNode* light, unsigned int step)
{
   AtMatrix matrix;
   GetMatrix(matrix);

   AtArray* matrices = AiNodeGetArray(light, "matrix");
   AiArraySetMtx(matrices, step, matrix);
   
   AtNode* meshNode = (AtNode*)AiNodeGetPtr(light, "mesh");
   if (meshNode != 0) // just simply copy the matrices
   {
      AiNodeSetArray(meshNode, "matrix", AiArrayCopy(AiNodeGetArray(light, "matrix")));
      AtArray* vlist = AiNodeGetArray(meshNode, "vlist");
       
      MFnDependencyNode fnDepNode(m_dagPath.node());
      MPlug plug = fnDepNode.findPlug("inputMesh");
      MObject meshObject;
      plug.getValue(meshObject);
      MFnMesh mesh(meshObject); // no need to check the status, because if it
      // worked for the first time, it`s going to work for the second
      
      MStatus status;
      const AtVector* vertices = (const AtVector*)mesh.getRawPoints(&status);
      for (int i = 0, j = m_numVertices * step; i < m_numVertices; ++i, ++j)
         AiArraySetVec(vlist, j, vertices[i]);      
   }  
}
