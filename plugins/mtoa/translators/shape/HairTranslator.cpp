#include "HairTranslator.h"

#include <maya/MRenderLineArray.h>
#include <maya/MRenderLine.h>
#include <maya/MDagPathArray.h>
#include <maya/MPointArray.h>
#include <maya/MHairSystem.h>
#include <maya/MFnPfxGeometry.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnNurbsCurve.h>


void CHairTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper("hairSystem", "curves");
   CShapeTranslator::MakeCommonAttributes(helper);
   helper.MakeInput("min_pixel_width");
   helper.MakeInput("mode");

   CAttrData data;

   data.defaultValue.BOOL() = true;
   data.name = "primaryVisibility";
   data.shortName = "vis";
   data.channelBox = false;
   data.keyable = false;       
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL() = true;
   data.name = "castsShadows";
   data.shortName = "csh";
   helper.MakeInputBoolean(data);
   
   
   data.defaultValue.BOOL() = false;
   data.name = "aiExportHairUVs";
   data.shortName = "ai_export_hair_uvs";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL() = false;
   data.name = "aiExportHairColors";
   data.shortName = "ai_export_hair_colors";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiOverrideHair";
   data.shortName = "ai_override_hair";
   helper.MakeInputBoolean(data);

   data.name = "aiHairShader";
   data.shortName = "ai_hair_shader";
   data.defaultValue.RGB() = AI_RGB_BLACK;
   helper.MakeInputRGB(data);

   data.defaultValue.FLT() = 1.0f;
   data.name = "aiIndirectDiffuse";
   data.shortName = "aiIndirectDiffuse";
   helper.MakeInputFloat(data);

}

AtNode* CHairTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("curves");
}

// FIXME should we set the update mode to RECREATE_NODE as CCurveTranslator ? (#2399)
void CHairTranslator::Export( AtNode *curve )
{

   m_hairInfo = MObject(m_dagPath.node());

   MFnDagNode fnDagNodeHairShape(m_hairInfo);
   MFnDependencyNode fnDepNodePfxHair(m_hairInfo);
   
   // look for the pfxHairShape to get transforms
   // visibilities etc correctly   
   MPlugArray pArr;
   MObject hairSystemObject;
   fnDepNodePfxHair.findPlug("renderHairs", true).connectedTo(pArr, true, false);
   if (pArr.length())
      hairSystemObject = pArr[0].node();
   else
      return;

   MFnDependencyNode fnDepNodeHair(hairSystemObject);
   
   // Set rest curve matrix
   ExportMatrix(curve);

   MPlug plug;  
   
   MStatus status;
   
   //ProcessRenderFlags(curve);
   ExportTraceSets(curve, fnDepNodeHair.findPlug("aiTraceSets", true));

   AtByte visibility = AI_RAY_ALL;
   plug = fnDepNodeHair.findPlug("castShadows", true);
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~AI_RAY_SHADOW;
   plug = fnDepNodeHair.findPlug("primaryVisibility", true);
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~AI_RAY_CAMERA;

   plug = fnDepNodeHair.findPlug("aiVisibleInDiffuseReflection", true);
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~(AI_RAY_DIFFUSE_REFLECT);
   plug = fnDepNodeHair.findPlug("aiVisibleInSpecularReflection", true);
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~AI_RAY_SPECULAR_REFLECT;   
   plug = fnDepNodeHair.findPlug("aiVisibleInDiffuseTransmission", true);
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~AI_RAY_DIFFUSE_TRANSMIT;
   plug = fnDepNodeHair.findPlug("aiVisibleInSpecularTransmission", true);
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~AI_RAY_SPECULAR_TRANSMIT;
   plug = fnDepNodeHair.findPlug("aiVisibleInVolume", true);
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~AI_RAY_VOLUME;

   
   if (RequiresShaderExport())
   {
      AtNode* shader = NULL;
      
      plug = fnDepNodeHair.findPlug("aiOverrideHair", true);
      if (!plug.isNull() && plug.asBool())
      {
         MPlugArray curveShaderPlug;
         plug = fnDepNodeHair.findPlug("aiHairShader", true);
         if (!plug.isNull())
         {
            plug.connectedTo(curveShaderPlug, true, false);
            if (curveShaderPlug.length() > 0)
            {               
               shader = ExportConnectedNode(curveShaderPlug[0]);
               // FIXME if matte is enabled -> export a matte shader
               /*
               if (shader)
               {
                  CNodeTranslator* shaderTranslator = GetTranslator(curveShaderPlug[0].node()); // the shading engine's translator is not called
                  // because there is no maya node for it

                  // FIXME: check if this works properly and if there's another way of doing this
                  if (shaderTranslator)
                  {
                     plug = shaderTranslator->FindMayaPlug("aiEnableMatte", &status);
                     if (status && !plug.isNull())
                        ProcessParameter(shader, "enable_matte", AI_TYPE_BOOLEAN, plug);
                     plug = shaderTranslator->FindMayaPlug("aiMatteColor", &status);
                     if (status && !plug.isNull())
                        ProcessParameter(shader, "matte_color", AI_TYPE_RGBA, plug);
                  }
               }
               */
            }
         }
      }
      // Default to the Hair shader if nothing else has been set.
      if (shader == NULL)
      {
         // First check if the internal root shader was already created in a previous export
         shader = GetArnoldNode("hairShader");
         if (shader == NULL)
            shader = AddArnoldNode("standard_hair", "hairShader");

         AtNode *multShader = GetArnoldNode("shaderMult");
         if (multShader == NULL)
            multShader = AddArnoldNode("multiply", "shaderMult");            

         AiNodeLink(multShader, "diffuse_color", shader);

         ProcessParameter(multShader, "input1", AI_TYPE_RGB, fnDepNodeHair.findPlug("hairColor"));
         // need to multiply the color with ramp_rgb
         MRampAttribute rampAttr(fnDepNodeHair.findPlug("hairColorScale", true), &status);
         if (status)
         {
            AtNode *rampShader = GetArnoldNode("shaderRamp");
            if (rampShader == NULL)
               rampShader = AddArnoldNode("ramp_rgb", "shaderRamp");
            
            AiNodeLink(rampShader, "input2", multShader);

            MPlug rampPlug = fnDepNodeHair.findPlug("hairColorScale");
            MObject opos = fnDepNodeHair.attribute("hairColorScale_Position");
            ProcessArrayParameter(rampShader, "position", rampPlug, AI_TYPE_FLOAT, &opos);
            MObject ocol = fnDepNodeHair.attribute("hairColorScale_Color");
            ProcessArrayParameter(rampShader, "color", rampPlug, AI_TYPE_RGB, &ocol);   
            MObject oint = fnDepNodeHair.attribute("hairColorScale_Interp");
            ProcessArrayParameter(rampShader, "interpolation", rampPlug, AI_TYPE_INT, &oint);

            AiNodeSetStr(rampShader, "type", "v");
            AiNodeSetStr(rampShader, "use_implicit_uvs", "curves_only");              
         }
         AiNodeSetFlt(shader, "diffuse", 1.f);

         MPlug opacPlug = fnDepNodeHair.findPlug("opacity");
         MPlugArray connections;
         opacPlug.connectedTo(connections, true, false);
         if (connections.length() > 0)
            AiNodeLink(ExportConnectedNode(connections[0]), "opacity", shader);
         else
         {
            float opacity = opacPlug.asFloat();
            AiNodeSetRGB(shader, "opacity", opacity, opacity, opacity);
         }

         MPlug transPlug = fnDepNodeHair.findPlug("translucence");
         connections.clear();
         transPlug.connectedTo(connections, true, false);
         if (connections.length() > 0)
            AiNodeLink(ExportConnectedNode(connections[0]), "transmission_tint", shader);
         else
         {
            float translucence = transPlug.asFloat();
            AiNodeSetRGB(shader, "transmission_tint", translucence, translucence, translucence);
         }
         ProcessParameter(shader, "specular_tint", AI_TYPE_RGB, fnDepNodeHair.findPlug("specularColor"));
         AiNodeSetRGB(shader, "specular2_tint", 0.f, 0.f, 0.f);
         float specRoughness = fnDepNodeHair.findPlug("specularPower").asFloat();
         AiNodeSetFlt(shader, "roughness", 1.f / specRoughness);
         plug = fnDepNodeHair.findPlug("aiIndirectDiffuse", true);
         AiNodeSetFlt(shader, "indirect_diffuse", plug.asFloat());
         
         plug = fnDepNodeHair.findPlug("castShadows", true);

         if (plug.asBool())
            visibility = visibility | AI_RAY_SHADOW;
         else
            visibility = visibility & ~AI_RAY_SHADOW;
         
      }
      AiNodeSetPtr(curve, "shader", shader);
   }
   
   AiNodeSetByte(curve, "visibility", visibility);  
   
   // Export hair data   
   MRenderLineArray mainLines;
   GetHairLines(m_hairInfo, mainLines);
   const unsigned int numLines = mainLines.length();
   
   // we need to get the number of points
   // as well the number of points needed for interpolation
   unsigned int numPoints = 0;
   m_numPointsInterpolation = 0;
   for (unsigned int i = 0; i < numLines; ++i)
   {
      MRenderLine renderLine = mainLines.renderLine(i, &status);
      
      const unsigned int np = renderLine.getLine().length();
      if (np > 1)
      {
         numPoints += np;
         m_numPointsInterpolation += np + 2;
      }
   }

   plug = fnDepNodeHair.findPlug("aiExportHairUVs", true);
   m_export_curve_uvs = plug.isNull() ? false : plug.asBool();

   // TODO : MMeshIntersector is useless for UVs query
   // until it returns a closest face as well as a closest point
   m_hasConnectedShapes = false;
   // The U and V param coords arrays
   AtArray* curveParamCoord = NULL;
   MMatrix shapeTransform;
   if (m_export_curve_uvs)
   {
      curveParamCoord = AiArrayAllocate(numLines, 1, AI_TYPE_VECTOR2);
      
      // Get connected shapes
      MDagPathArray connectedShapes;
      GetHairShapeMeshes(hairSystemObject, connectedShapes);
      m_hasConnectedShapes = connectedShapes.length() > 0;
      // Prepare connected shape info and intersection data
      // Support hairsystem that was applied to only one mesh
      if (m_hasConnectedShapes)
      {
         MDagPath shapePath = connectedShapes[0];
         MObject shapeNode = shapePath.node();
         const MMatrix matrix = shapePath.inclusiveMatrix();
         shapeTransform = matrix.inverse();
         m_mesh.setObject(shapePath);
      }
   }

   plug = fnDepNodeHair.findPlug("aiExportHairColors", true);
   const bool exportCurveColors = plug.isNull() ? false : plug.asBool();
   
   AtArray* curveColors = 0;
   if (exportCurveColors)
      curveColors = AiArrayAllocate(numPoints, 1, AI_TYPE_RGB);

   plug = fnDepNodeHair.findPlug("aiOpaque", true);
   if (!plug.isNull())
      AiNodeSetBool(curve, "opaque", plug.asBool());
   plug = fnDepNodeHair.findPlug("aiSelfShadows", true);
   if (!plug.isNull())
      AiNodeSetBool(curve, "self_shadows", plug.asBool());
   plug = fnDepNodeHair.findPlug("receiveShadows", true);
   if (!plug.isNull())
      AiNodeSetBool(curve, "receive_shadows", plug.asBool());
   
   int step = GetMotionStep();

   // Allocate the memory for parameters
   // No need for multiple keys with the points if deformation motion blur
   // Is not enabled
   AtArray* curvePoints = AiArrayAllocate(m_numPointsInterpolation, GetNumMotionSteps(), AI_TYPE_VECTOR);   
   AtArray* curveNumPoints = AiArrayAllocate(numLines, 1, AI_TYPE_UINT);
   AtArray* curveWidths = AiArrayAllocate(numPoints, 1, AI_TYPE_FLOAT);
   
   unsigned int iid = m_numPointsInterpolation * step ; // for the position data
   unsigned int id = 0; // for the general data   
   for (unsigned int i = 0; i < numLines; ++i)
   {
      MRenderLine renderLine = mainLines.renderLine(i, &status);
            
      MVectorArray line = renderLine.getLine();
      const unsigned int numPoints = line.length();
      
      if (numPoints < 2)
         continue;      
      
      MDoubleArray widths = renderLine.getWidth();
      
      MVectorArray colors;      
      if (exportCurveColors)
         colors = renderLine.getColor();
      
      MVector* lineVertex = &(line[0]);
      
      AtVector cv = AI_V3_ZERO;
      
      cv.x = (float)lineVertex->x;
      cv.y = (float)lineVertex->y;
      cv.z = (float)lineVertex->z;
      AiArraySetVec(curvePoints, iid++, cv);
      
      const unsigned int numPointsInterpolation = numPoints + 2;
      AiArraySetUInt(curveNumPoints, i, numPointsInterpolation);
      for (unsigned int j = 0; j < numPoints; ++j, ++lineVertex)
      {
         if (exportCurveColors)
         {
            MVector color = colors[j];
            AtRGB aColor ((float)color.x, (float)color.y, (float)color.z);
            AiArraySetRGB(curveColors, id, aColor);
         }
         AiArraySetFlt(curveWidths, id, (float)widths[j] / 2.0f);
         ++id;
         cv.x = (float)lineVertex->x;
         cv.y = (float)lineVertex->y;
         cv.z = (float)lineVertex->z;
         AiArraySetVec(curvePoints, iid++, cv);
      }
      // TODO : find a better solution for this
      AiArraySetVec(curvePoints, iid++, cv); 
       
      if (m_export_curve_uvs)
      {
         AtVector2 uvparam(AI_P2_ZERO);
         // TODO : leave an option to use exact but slow method?
         if (m_hasConnectedShapes)
            uvparam = GetHairRootUVs(line[0], m_mesh, shapeTransform);
         AiArraySetVec2(curveParamCoord, i, uvparam);

      }
   }
   
   AiNodeSetArray(curve, "points", curvePoints); 
   AiNodeSetArray(curve, "num_points", curveNumPoints);
   AiNodeSetArray(curve, "radius", curveWidths);
   if (exportCurveColors)
   {
      AiNodeDeclare(curve, "colors", "varying  RGB");
      AiNodeSetArray(curve, "colors", curveColors);
   }
   
   if (m_export_curve_uvs)
   {
      AiNodeSetArray(curve, "uvs", curveParamCoord);
   }

   // Hair specific Arnold render settings.
   plug = fnDepNodeHair.findPlug("aiMinPixelWidth", true);
   if (!plug.isNull()) AiNodeSetFlt(curve, "min_pixel_width", plug.asFloat());

   // Mode is an enum, 0 == ribbon, 1 == tubes.
   plug = fnDepNodeHair.findPlug("aiMode", true);
   if (!plug.isNull()) AiNodeSetInt(curve, "mode", plug.asInt());

   AiNodeSetStr(curve, "basis", "catmull-rom");
   
   if (RequiresMotionData())
   {
      double motionStart, motionEnd;
      GetSessionOptions().GetMotionRange(motionStart, motionEnd);
      AiNodeSetFlt(curve, "motion_start", (float)motionStart);
      AiNodeSetFlt(curve, "motion_end", (float)motionEnd);
   }
   
   mainLines.deleteArray();

   if (!GetSessionOptions().GetExportFullPath() || GetSessionOptions().GetExportPrefix().length() > 0)
   {
      if (AiNodeLookUpUserParameter(curve, "maya_full_name") == NULL)
         AiNodeDeclare(curve, "maya_full_name", "constant STRING");
   
      MString fullName = m_dagPath.fullPathName();
      AiNodeSetStr(curve, "maya_full_name", AtString(fullName.asChar()));
   }


}

void CHairTranslator::ExportMotion(AtNode *curve)
{
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(curve);

   // Same for object deformation, early out if it's not set.
   if (!IsMotionBlurEnabled(MTOA_MBLUR_DEFORM)) return;

   // Get hair lines
   MObject objectHairShape(m_dagPath.node());
   MRenderLineArray mainLines;
   GetHairLines(objectHairShape, mainLines, false);
   
   const unsigned int numLines = mainLines.length();
   MStatus status;
   
   AtArray* curvePoints = AiNodeGetArray(curve, "points");
   
   unsigned int iid = GetMotionStep() * m_numPointsInterpolation;
   for (unsigned int i = 0; i < numLines; ++i)
   {
      MRenderLine renderLine = mainLines.renderLine(i, &status);
      
      MVectorArray line = renderLine.getLine();
      
      const unsigned int numPoints = line.length();
      if (numPoints < 2)
         continue;     
      
      MVector* lineVertex = &(line[0]);
      
      AtVector cv = AI_V3_ZERO;
      
      cv.x = (float)lineVertex->x;
      cv.y = (float)lineVertex->y;
      cv.z = (float)lineVertex->z;
      AiArraySetVec(curvePoints, iid++, cv);
      
      for (unsigned int j = 0; j < numPoints; ++j, ++lineVertex)
      {
         cv.x = (float)lineVertex->x;
         cv.y = (float)lineVertex->y;
         cv.z = (float)lineVertex->z;
         AiArraySetVec(curvePoints, iid++, cv);
      }
      AiArraySetVec(curvePoints, iid++, cv);
   }
   
   mainLines.deleteArray();
}
 
AtVector2 CHairTranslator::GetHairRootUVs(const MVector& lineStart, MFnMesh& mesh, MMatrix shapeTransform)
{
   // Find the closest point on mesh from hair lineStart
   // And returns the UV at this point

   AtVector2 rootUVs;
   float2 uv;
 
   // no need to call closestPoint, getUVAtPoint already looks for
   // the closest point according to the documentation
   // From the API documentation : "Find the point closet to the given point, 
   // and return the UV value at that point."
   MPoint point(lineStart.x, lineStart.y, lineStart.z);
   // We need to transalte the point to object space as "mesh" does not have any transformation matrix
   point *= shapeTransform;
   
   MString currentUVSet = mesh.currentUVSetName();
   mesh.getUVAtPoint(point, uv, MSpace::kObject, &currentUVSet);
   
   rootUVs.x = uv[0];
   rootUVs.y = uv[1];

   return rootUVs;
}

void CHairTranslator::GetHairShapeMeshes(const MObject& hair, MDagPathArray& shapes)
{
   MObjectArray follicles;
   MIntArray indices;

   // Loop through all follicles to find all connected shapes
   MHairSystem::getFollicle(hair, follicles, indices);
   for (unsigned int i = 0; i < follicles.length(); i++)
   {
      MFnDependencyNode depNodeFollicle(follicles[i]);
      MPlugArray meshes;
      depNodeFollicle.findPlug("inputMesh", true).connectedTo(meshes, true, false);
      if (meshes.length() > 0)
      {
         MFnDagNode meshDagNode(meshes[0].node());
         MDagPath dagPath;
         meshDagNode.getPath(dagPath);
         // Check if it is already there
         bool append = true;
         for (unsigned int j = 0; j < shapes.length(); j++)
         {
            if (shapes[j] == dagPath)
            {
               append = false;
               break;
            }
         }
         if (append)
            shapes.append(dagPath);
      }
   }
}

void CHairTranslator::GetHairLines(MObject& hair, MRenderLineArray& mainLines, bool firstStep)
{
   MFnPfxGeometry pfxHair(hair);

   MRenderLineArray leafLines;
   MRenderLineArray flowerLines;

   // And then all lines, we'll only need mainLines
   // we don`t need twist, flatness, parameter, incandescence and transparency at all
   // and we only need color and width in the first step
   // getLineData (MRenderLineArray &mainLines, MRenderLineArray &leafLines, MRenderLineArray &flowerLines,
   // bool doLines, bool doTwist, bool doWidth, bool doFlatness, bool doParameter,
   // bool doColor, bool doIncandescence, bool doTransparency, bool worldSpace)
   pfxHair.getLineData(mainLines, leafLines, flowerLines, true, false, firstStep, false, false, firstStep, false, false, false);

   // removing the unused parameters
   leafLines.deleteArray();
   flowerLines.deleteArray();
}

void CHairTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   
   // we used to only set RECREATE_NODE for the .create attribute
   //MString plugName = plug.name().substring(plug.name().rindex('.'), plug.name().length()-1);
   //if ((plugName == ".create")) SetUpdateMode(AI_RECREATE_NODE);

   // but ticket #2399 showed that curves aren't updated properly in arnold core,
   // for example when the curves width change.
   // So now we're always forcing to recreate the node
   if (!IsTransformPlug(plug))
      SetUpdateMode(AI_RECREATE_NODE);
   
   CShapeTranslator::NodeChanged(node, plug);
}


