
#include "MayaScene.h"

#include <maya/MPlugArray.h>
#include <maya/MPlug.h>
#include <maya/MFnPfxGeometry.h>
#include <maya/MRenderLineArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MRenderLine.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MStatus.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPathArray.h>
#include <maya/MHairSystem.h>
#include <maya/MObjectArray.h>
#include <maya/MIntArray.h>
#include <maya/MFnMesh.h>
#include <maya/MPoint.h>

#include <ai_nodes.h>
#include <ai_vector.h>
#include <ai_ray.h>

namespace
{
   AtVector2 GetHairRootUVs(const MVectorArray& line, int index, const MDagPathArray& shapes)
   {
      AtVector2 returned_vector;
      
      // Find the closest point from the line[0] to the surface (shapes[index])
      MFnMesh mesh(shapes[0].node());
      MPoint point(line[0].x, line[0].y, line[0].z);
      MPoint closest;
      float uv[2];
      mesh.getClosestPoint(point, closest, MSpace::kObject);
      mesh.getUVAtPoint(closest, uv, MSpace::kObject, &mesh.currentUVSetName());
      
      returned_vector.x = uv[0];
      returned_vector.y = uv[1];
      return returned_vector;
   }

   MDagPathArray GetHairShapeMeshes(const MObject& hair)
   {
      MDagPathArray shapes;
      MObjectArray  follicles;
      MIntArray     indices;

      // Loop through all follicles to find the shapes connected
      MHairSystem::getFollicle(hair, follicles, indices);
      for(int i=0; i<follicles.length(); i++)
      {
         MFnDependencyNode depNodeFollicle(follicles[i]);
         MPlugArray        meshes;
         depNodeFollicle.findPlug("inputMesh").connectedTo(meshes, true, false);
         MFnDagNode meshDagNode(meshes[0].node());
         MDagPath dagPath;
         meshDagNode.getPath(dagPath);
         shapes.append(dagPath);
      }
      return shapes; 
   }
}

void CMayaScene::ExportHair(const MDagPath& dagPath, AtUInt step)
{
   AtMatrix matrix;

   MFnDagNode fnDagNodeHairShape(dagPath.node());

   MFnDagNode dagNodeTransform(dagPath.transform());
   MDagPath dagPathTransform;
   dagNodeTransform.getPath(dagPathTransform);
   GetMatrix(matrix,dagPathTransform);

   // Get the pfx hair node
   MPlugArray pfxHairPlug;
   MFnDependencyNode depNodeHair(dagPath.node());
   depNodeHair.findPlug("outputRenderHairs").connectedTo(pfxHairPlug, false, true);
   MFnPfxGeometry pfxHair(pfxHairPlug[0].node());

   // Get connected shapes
   MDagPathArray m_shapesConnected = GetHairShapeMeshes(dagPath.node());

   // And then all lines, we'll only need mainLines
   MRenderLineArray mainLines;
   MRenderLineArray leafLines;
   MRenderLineArray flowerLines;
   pfxHair.getLineData(mainLines, leafLines, flowerLines, true, true, true, true, true, true, true, false, true);
   int numMainLines = mainLines.length();

   // Check if motion blur is ON for hairs
   MFnDagNode fnDagNode(pfxHairPlug[0].node());
   bool mb = m_fnArnoldRenderOptions->findPlug("motion_blur_enable").asBool()  &&
        m_fnArnoldRenderOptions->findPlug("mb_objects_enable").asBool() &&
        fnDagNode.findPlug("motionBlurred").asBool();

   bool mb_deform = m_fnArnoldRenderOptions->findPlug("motion_blur_enable").asBool()  &&
               m_fnArnoldRenderOptions->findPlug("mb_object_deform_enable").asBool() &&
               fnDagNode.findPlug("motionBlurred").asBool();

   // Check if custom attributes have been created, ignore them otherwise
   MStatus status;
   fnDagNodeHairShape.findPlug("override_hair", &status);
   bool customAttributes = (status == MS::kSuccess);
   bool transformHairDefinition = true;

   AtNode* shader;
   bool  receive_shadows, self_shadows, sss_use_gi;
   AtUInt  sss_max_samples, mode;
   AtInt visibility = 65535;

   // Primary visibility is located on the pfxHairShape Node
   if (!fnDagNode.findPlug("primaryVisibility").asBool())
      visibility &= ~AI_RAY_CAMERA;
 
   AtFloat sss_sample_spacing, min_pixel_width;

   // The curve node for this hair
   AtNode*  curve;

   // The curve_points array 
   AtArray* curve_points;

   // The curve_widths array 
   AtArray* curve_widths;
   
   // The num_points array (int array the size of numLines, no motionsteps)
   AtArray* curve_num_points = AiArrayAllocate(numMainLines, 1, AI_TYPE_INT);

   // The uparamcoords array
   AtArray* curve_uparamcoord = AiArrayAllocate(numMainLines, 1, AI_TYPE_FLOAT);

   // The uparamcoords array
   AtArray* curve_vparamcoord = AiArrayAllocate(numMainLines, 1, AI_TYPE_FLOAT);

   // A couple of arrays to keep track of where each hairline begins in the
   // points array (step 0)
   AtArray* curve_next_line_starts_interp = AiArrayAllocate(numMainLines, 1, AI_TYPE_INT);
   AtArray* curve_next_line_starts = AiArrayAllocate(numMainLines, 1, AI_TYPE_INT);

   if (step==0)
   {
      if (customAttributes)
      {
         // Check if we override maya's hair attributes with an arnold hair shader
         if (depNodeHair.findPlug("override_hair").asBool())
         {
            MPlugArray hairShaderPlug;
            depNodeHair.findPlug("hair_shader").connectedTo(hairShaderPlug, true, false);
            if (hairShaderPlug.length()>0)
            {
               shader = ExportShader(hairShaderPlug[0].node());
               // Avoid later processing since there is a shader connected
               transformHairDefinition = false;
            }
         }

         // Attributes
         receive_shadows    = fnDagNodeHairShape.findPlug("receive_shadows").asBool();
         self_shadows       = fnDagNodeHairShape.findPlug("self_shadows").asBool();
         sss_use_gi         = fnDagNodeHairShape.findPlug("sss_use_gi").asBool();
         sss_max_samples    = fnDagNodeHairShape.findPlug("sss_max_samples").asInt();
         sss_sample_spacing = fnDagNodeHairShape.findPlug("sss_sample_spacing").asFloat();
         min_pixel_width    = fnDagNodeHairShape.findPlug("min_pixel_width").asFloat();
         mode               = fnDagNodeHairShape.findPlug("mode").asInt();

         if (!fnDagNodeHairShape.findPlug("castsShadows").asBool())
            visibility &= ~AI_RAY_SHADOW;

         if (!fnDagNodeHairShape.findPlug("visibleInReflections").asBool())
            visibility &= ~AI_RAY_REFLECTED;

         if (!fnDagNodeHairShape.findPlug("visibleInRefractions").asBool())
            visibility &= ~AI_RAY_REFRACTED;

         if (!fnDagNodeHairShape.findPlug("diffuse_visibility").asBool())
            visibility &= ~AI_RAY_DIFFUSE;

         if (!fnDagNodeHairShape.findPlug("glossy_visibility").asBool())
            visibility &= ~AI_RAY_GLOSSY;
      }

      // No custom shader assigned, try to transform maya hair's definition
      if (transformHairDefinition == true)
      {
         shader = AiNode("hair");
         MString hairShaderName = depNodeHair.name();
         hairShaderName += "_hairShader";
         AiNodeSetStr(shader, "name", hairShaderName.asChar());
 
         // TODO PROCESS MAYA HAIR SHADING ATTRIBUTES
         // With mtoaHair shader (also TODO) ticket 109 https://trac.solidangle.com/mtoa/ticket/109
      }

      // iterate over all lines to get sizes for AiArrayAllocate
      int num_points = 0;
      int num_points_interpolation = 0;
      for(int i=0; i<numMainLines; i++)
      {
         MStatus status;
         MRenderLine m_renderLine = mainLines.renderLine(i, &status);
         MVectorArray line        = m_renderLine.getLine();
         int renderLineLength     = line.length();
         num_points += renderLineLength;
         
         num_points_interpolation += renderLineLength + 2;

         // Set num_points
         AiArraySetInt(curve_num_points, i, (renderLineLength+2));
 
         // Get the UV values of the surface point on which the hair is rooted
         if (customAttributes)
         {
            AtVector2 uvparam = GetHairRootUVs(line, i, m_shapesConnected);
            AiArraySetFlt(curve_uparamcoord, i, uvparam.x);
            AiArraySetFlt(curve_vparamcoord, i, uvparam.y);
         }

         // Store start point for the line on the array
         AiArraySetInt(curve_next_line_starts_interp, i, (num_points_interpolation));
         AiArraySetInt(curve_next_line_starts, i, (num_points));
      }

      // Allocate memory for remaining arrays     
      if(mb_deform || mb)
      {
         curve_points      = AiArrayAllocate(num_points_interpolation, m_motionBlurData.motion_steps, AI_TYPE_POINT);
      }
      else
      {
         curve_points = AiArrayAllocate(num_points_interpolation, 1, AI_TYPE_POINT);
      }
      curve_widths      = AiArrayAllocate(num_points, 1, AI_TYPE_FLOAT);

  

      // Create the curve node
      curve = AiNode("curves");
      AiNodeSetStr(curve, "name", dagPath.fullPathName().asChar());
      AiNodeSetMatrix(curve, "matrix", matrix);

      // Assign shader
      AiNodeSetPtr(curve, "shader", shader);

      // Add shader uparam and vparam names            
      AiNodeSetStr(shader, "uparam", "uparamcoord");
      AiNodeSetStr(shader, "vparam", "vparamcoord");

      // Visibility
      AiNodeSetInt(curve, "visibility", visibility);

      AiNodeSetStr(curve, "basis", "catmull-rom");

      // Extra attributes
      if (customAttributes)
      {
         AiNodeSetBool(curve, "receive_shadows", receive_shadows);
         AiNodeSetBool(curve, "self_shadows", self_shadows);
         AiNodeSetBool(curve, "sss_use_gi", sss_use_gi);
         AiNodeSetInt(curve, "sss_max_samples", sss_max_samples);
         AiNodeSetFlt(curve, "sss_sample_spacing", sss_sample_spacing);
         AiNodeSetFlt(curve, "min_pixel_width", min_pixel_width);
         AiNodeSetInt(curve, "mode", mode);

         // User-Data attributes
         AiNodeDeclare(curve, "uparamcoord", "uniform FLOAT");
         AiNodeDeclare(curve, "vparamcoord", "uniform FLOAT");
         AiNodeDeclare(curve, "next_line_starts_interp", "constant ARRAY INT");
         AiNodeDeclare(curve, "next_line_starts", "constant ARRAY INT");

      }

      // Set all arrays on the curve node
      AiNodeSetArray(curve, "num_points", curve_num_points);
      AiNodeSetArray(curve, "points", curve_points);
      AiNodeSetArray(curve, "radius", curve_widths);
      AiNodeSetArray(curve, "uparamcoord", curve_uparamcoord);
      AiNodeSetArray(curve, "vparamcoord", curve_vparamcoord);
      AiNodeSetArray(curve, "next_line_starts_interp", curve_next_line_starts_interp);
      AiNodeSetArray(curve, "next_line_starts", curve_next_line_starts);
   }
   else
   {
      // Step != 0, get curves node and arrays
      curve = AiNodeLookUpByName(dagPath.fullPathName().asChar());
      curve_points = AiNodeGetArray(curve, "points");
      curve_num_points = AiNodeGetArray(curve, "num_points");
      curve_next_line_starts_interp = AiNodeGetArray(curve, "next_line_starts_interp");
      curve_next_line_starts = AiNodeGetArray(curve, "next_line_starts");
   }


   int curve_num_points_per_step  = AiArrayGetInt(curve_next_line_starts_interp, (numMainLines-1));
   // Process all hair lines
   for(int i=0; i<numMainLines; i++)
   {
      MStatus status;
      MRenderLine m_renderLine = mainLines.renderLine(i, &status);
      MVectorArray line        = m_renderLine.getLine();
      int renderLineLength     = line.length();

      // Ignore one or less cv curves
      if (renderLineLength>1)
      {
         MDoubleArray width     = m_renderLine.getWidth();
         AtPoint curve_point;

         if (mb_deform || mb || (step == 0))
         {

            int curve_line_interp_starts_i = i ? AiArrayGetInt(curve_next_line_starts_interp, i-1) : 0 ;
            int curve_line_starts_i = i ? AiArrayGetInt(curve_next_line_starts, i-1) : 0 ;

            for(int j=0; j<renderLineLength; j++)
            {
               // We need a couple extra points for interpolation
               // One at the beginning and one at the end ( JUST POINTS , NO ATTRS )
               if(j==0)
               {
                  AiV3Create(curve_point, (AtFloat) line[j].x, (AtFloat) line[j].y, (AtFloat) line[j].z);
                  AiArraySetPnt(curve_points, (j + curve_line_interp_starts_i + (step * curve_num_points_per_step)), curve_point);
               }
 
               AiV3Create(curve_point, (AtFloat) line[j].x, (AtFloat) line[j].y, (AtFloat) line[j].z);
               AiArraySetPnt(curve_points, ((j+1) + curve_line_interp_starts_i + (step * curve_num_points_per_step)), curve_point);

               // Animated widths are not supported, so just on STEP 0
               if (step == 0)
               {
                  AiArraySetFlt(curve_widths, (j + curve_line_starts_i) , width[j]);
               }

               if(j==(renderLineLength-1))
               {
                  AiV3Create(curve_point, (AtFloat) line[j].x, (AtFloat) line[j].y, (AtFloat) line[j].z);
                  AiArraySetPnt(curve_points, ((j+2) + curve_line_interp_starts_i + (step * curve_num_points_per_step)), curve_point);
               }
            }
         }
      }
   
   }

   // As told in the docs, destructor does not free mem allocated by mainLines, leafLines etc
   mainLines.deleteArray();
   leafLines.deleteArray();
   flowerLines.deleteArray();
   
}
