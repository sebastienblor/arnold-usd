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
   fnDepNodePfxHair.findPlug("renderHairs").connectedTo(pArr, true, false);
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
   ExportTraceSets(curve, fnDepNodeHair.findPlug("aiTraceSets"));

   AtByte visibility = AI_RAY_ALL;
   plug = fnDepNodeHair.findPlug("castShadows");
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~AI_RAY_SHADOW;
   plug = fnDepNodeHair.findPlug("primaryVisibility");
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~AI_RAY_CAMERA;

   plug = fnDepNodeHair.findPlug("aiVisibleInDiffuseReflection");
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~(AI_RAY_DIFFUSE_REFLECT);
   plug = fnDepNodeHair.findPlug("aiVisibleInSpecularReflection");
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~AI_RAY_SPECULAR_REFLECT;   
   plug = fnDepNodeHair.findPlug("aiVisibleInDiffuseTransmission");
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~AI_RAY_DIFFUSE_TRANSMIT;
   plug = fnDepNodeHair.findPlug("aiVisibleInSpecularTransmission");
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~AI_RAY_SPECULAR_TRANSMIT;
   plug = fnDepNodeHair.findPlug("aiVisibleInVolume");
   if (!plug.isNull() && !plug.asBool())
      visibility &= ~AI_RAY_VOLUME;

   
   if (RequiresShaderExport())
   {
      AtNode* shader = NULL;
      
      plug = fnDepNodeHair.findPlug("aiOverrideHair");
      if (!plug.isNull() && plug.asBool())
      {
         MPlugArray curveShaderPlug;
         plug = fnDepNodeHair.findPlug("aiHairShader");
         if (!plug.isNull())
         {
            plug.connectedTo(curveShaderPlug, true, false);
            if (curveShaderPlug.length() > 0)
            {               
               shader = ExportConnectedNode(curveShaderPlug[0]);
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
            }
         }
      }
      // Default to the Hair shader if nothing else has been set.
      if (shader == NULL)
      {
         // First check if the internal root shader was already created in a previous export
         shader = GetArnoldNode("hairShader");
         if (shader == NULL)
            shader = AddArnoldNode("MayaHair", "hairShader");

         ProcessParameter(shader, "hairColor", AI_TYPE_RGB, fnDepNodeHair.findPlug("hairColor"));
         ProcessParameter(shader, "opacity", AI_TYPE_FLOAT, fnDepNodeHair.findPlug("opacity"));
         ProcessParameter(shader, "translucence", AI_TYPE_FLOAT, fnDepNodeHair.findPlug("translucence"));
         ProcessParameter(shader, "specularColor", AI_TYPE_RGB, fnDepNodeHair.findPlug("specularColor"));
         ProcessParameter(shader, "specularPower", AI_TYPE_FLOAT, fnDepNodeHair.findPlug("specularPower"));
         

         plug = fnDepNodeHair.findPlug("castShadows");

         if (plug.asBool())
            visibility = visibility | AI_RAY_SHADOW;
         else
            visibility = visibility & ~AI_RAY_SHADOW;

         //const bool diffuseRandConnected = ProcessParameter(shader, "diffuseRand", AI_TYPE_FLOAT, fnDepNodeHair.findPlug("diffuseRand")) != 0;
         //const bool specularRandConnected = ProcessParameter(shader, "specularRand", AI_TYPE_FLOAT, fnDepNodeHair.findPlug("specularRand")) != 0;
         //const bool hueRandConnected = ProcessParameter(shader, "hueRand", AI_TYPE_FLOAT, fnDepNodeHair.findPlug("hueRand")) != 0;
         //const bool valRandConnected = ProcessParameter(shader, "valRand", AI_TYPE_FLOAT, fnDepNodeHair.findPlug("valRand")) != 0;
         //const bool satRandConnected = ProcessParameter(shader, "satRand", AI_TYPE_FLOAT, fnDepNodeHair.findPlug("satRand")) != 0;      

         

         MRampAttribute rampAttr(fnDepNodeHair.findPlug("hairColorScale"), &status);
         if (status)
         {
            // add some treshold later
            // if the two of the closest point are closer than 1.f / 512.f
            // increase the Frequency to have at least 8-16-32 samples 
            // between any point
            const int sampleFrequency = 512; 
            const float sampleFrequencyDiv = 1.f / (float)(sampleFrequency - 1);
            AtArray* rampArr = AiArrayAllocate(512, 1, AI_TYPE_RGB);
            for (int i = 0; i < sampleFrequency; ++i)
            {
               MColor color(1.0, 1.0, 1.0, 1.0);
               rampAttr.getColorAtPosition((float)i * sampleFrequencyDiv, color);
               AtRGB aColor ((float)color.r , (float)color.g, (float)color.b);
               AiArraySetRGB(rampArr, i, aColor);
            }
            AiNodeSetArray(shader, "hairColorScale", rampArr);
         }
         plug = fnDepNodeHair.findPlug("aiIndirectDiffuse");
         AiNodeSetFlt(shader, "indirectDiffuse", plug.asFloat());
      }
      SetRootShader(shader);      
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

   plug = fnDepNodeHair.findPlug("aiExportHairUVs");
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

   plug = fnDepNodeHair.findPlug("aiExportHairColors");
   const bool exportCurveColors = plug.isNull() ? false : plug.asBool();
   
   AtArray* curveColors = 0;
   if (exportCurveColors)
      curveColors = AiArrayAllocate(numPoints, 1, AI_TYPE_RGB);

   plug = fnDepNodeHair.findPlug("aiOpaque");
   if (!plug.isNull())
      AiNodeSetBool(curve, "opaque", plug.asBool());
   plug = fnDepNodeHair.findPlug("aiSelfShadows");
   if (!plug.isNull())
      AiNodeSetBool(curve, "self_shadows", plug.asBool());
   plug = fnDepNodeHair.findPlug("receiveShadows");
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
   plug = fnDepNodeHair.findPlug("aiMinPixelWidth");
   if (!plug.isNull()) AiNodeSetFlt(curve, "min_pixel_width", plug.asFloat());

   // Mode is an enum, 0 == ribbon, 1 == tubes.
   plug = fnDepNodeHair.findPlug("aiMode");
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

   // eventually export dcc_name user attribute
   // FIXME this could be moved to a separate function but this might be temporary
   if (GetSessionOptions().GetExportFullPath() || GetSessionOptions().GetExportPrefix().length() > 0)
   {
      if (AiNodeLookUpUserParameter(curve, "dcc_name") == NULL)
         AiNodeDeclare(curve, "dcc_name", "constant STRING");
   
      MString partialName = m_dagPath.partialPathName();
      AiNodeSetStr(curve, "dcc_name", AtString(partialName.asChar()));
   }
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
      depNodeFollicle.findPlug("inputMesh").connectedTo(meshes, true, false);
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
