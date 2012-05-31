

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
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "curves");
   CShapeTranslator::MakeCommonAttributes(helper);
   helper.MakeInput("min_pixel_width");
   helper.MakeInput("mode");

   CAttrData data;

   data.defaultValue.BOOL = true;
   data.name = "primaryVisibility";
   data.shortName = "vis";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL = true;
   data.name = "castsShadows";
   data.shortName = "csh";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL = true;
   data.name = "aiExportHairIDs";
   data.shortName = "ai_export_hair_ids";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = false;
   data.name = "aiExportHairUVs";
   data.shortName = "ai_export_hair_uvs";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = false;
   data.name = "aiOverrideHair";
   data.shortName = "ai_override_hair";
   helper.MakeInputBoolean(data);

   data.name = "aiHairShader";
   data.shortName = "ai_hair_shader";
   helper.MakeInputNode(data);
}

AtNode* CHairTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("curves");
}

void CHairTranslator::Export( AtNode *curve )
{
   Update(curve);
}

void CHairTranslator::Update( AtNode *curve )
{
   m_hairInfo = MObject(m_dagPath.node());

   MFnDagNode fnDagNodeHairShape(m_hairInfo);
   MFnDependencyNode fnDepNodeHair(m_hairInfo);
   
   const unsigned int numLines = GetHairLines(m_hairInfo, m_hairLines);
   
   // The shader nodes
   // TODO: Kill these and export it properly.
   AtNode* shader       = NULL;
   
   // Set curve matrix for step 0
   ExportMatrix(curve, 0);

   MPlug plug;
   plug = FindMayaPlug("aiOverrideHair");
   if (!plug.isNull() && plug.asBool())
   {
      MPlugArray curveShaderPlug;
      plug = FindMayaPlug("aiHairShader");
      if (!plug.isNull())
      {
         plug.connectedTo(curveShaderPlug, true, false);
         if (curveShaderPlug.length() > 0)
         {
            shader = ExportRootShader(curveShaderPlug[0]);
         }
      }
   }

   // Default to the Hair shader if nothing else has been set.
   if (shader == NULL)
   {
      shader = AiNode("hair");
      MString hairShaderName = fnDepNodeHair.name();
      hairShaderName += "_hairShader";
      AiNodeSetStr(shader, "name", hairShaderName.asChar());

      // Add shader uparam and vparam names
      AiNodeSetStr(shader, "uparam", "uparamcoord");
      AiNodeSetStr(shader, "vparam", "vparamcoord");
      
      shader = ExportRootShader(shader);

      // TODO PROCESS MAYA HAIR SHADING ATTRIBUTES
      // With mtoa Hair shader (also TODO) ticket 109
      // https://trac.solidangle.com/mtoa/ticket/109
   }

   plug = FindMayaPlug("aiExportHairUVs");
   m_export_curve_uvs = plug.isNull() ? false : plug.asBool();

   // TODO : MMeshIntersector is useless for UVs query
   // until it returns a closest face as well as a closest point
   m_hasConnectedShapes = false;
   // The U and V param coords arrays
   AtArray* curveUParamCoord = NULL;
   AtArray* curveVParamCoord = NULL;
   if (m_export_curve_uvs)
   {
      curveUParamCoord = AiArrayAllocate(numLines, 1, AI_TYPE_FLOAT);
      curveVParamCoord = AiArrayAllocate(numLines, 1, AI_TYPE_FLOAT);

      // Get connected shapes
      MDagPathArray connectedShapes;
      GetHairShapeMeshes(m_hairInfo, connectedShapes);
      m_hasConnectedShapes = connectedShapes.length() > 0;
      // Prepare connected shape info and intersection data
      // Support hairsystem that was applied to only one mesh
      if (m_hasConnectedShapes)
      {
         MDagPath shapePath = connectedShapes[0];
         MObject shapeNode = shapePath.node();
         const MMatrix matrix = shapePath.inclusiveMatrix();
         m_meshInt.create(shapeNode, matrix);
         m_mesh.setObject(shapeNode);
      }
   }

   plug = FindMayaPlug("aiExportHairIDs");
   m_export_curve_id = true;
   if (!plug.isNull())
   {
      m_export_curve_id = plug.asBool();
   }

   AtArray * curveID = NULL;
   if (m_export_curve_id)
   {
      curveID = AiArrayAllocate(numLines, 1, AI_TYPE_UINT);
   }

   ProcessRenderFlags(curve);    
                  
   // Allocate memory for all curve points and widths
   AtArray* curvePoints = AiArrayAllocate(m_hairLines.m_numPointsInterpolation, GetNumMotionSteps(), AI_TYPE_POINT);
   AtArray* curveWidths = AiArrayAllocate(m_hairLines.m_numPoints, GetNumMotionSteps(), AI_TYPE_FLOAT);
   AtArray* curveColors = AiArrayAllocate(numLines, GetNumMotionSteps(), AI_TYPE_RGB);
   AtArray* curveNumPoints = AiArrayAllocate(numLines, GetNumMotionSteps(), AI_TYPE_UINT);

   ProcessHairLines(0,
                    curvePoints,
                    curveNumPoints,
                    curveWidths,
                    curveColors,
                    curveID,
                    curveUParamCoord,
                    curveVParamCoord);
                    
   // Clear out the temporary arrays.
   clear();

   // Extra attributes
   AiNodeDeclare(curve, "colors",      "uniform  ARRAY RGB");

   // Assign shader
   if (shader != NULL) AiNodeSetPtr(curve, "shader", shader);


   // Hair specific Arnold render settings.
   plug = FindMayaPlug("aiMinPixelWidth");
   if (!plug.isNull()) AiNodeSetFlt(curve, "min_pixel_width", plug.asFloat());

   // Mode is an enum, 0 == ribbon, 1 == tubes.
   plug = FindMayaPlug("aiMode");
   if (!plug.isNull()) AiNodeSetInt(curve, "mode", plug.asInt());

   AiNodeSetStr(curve, "basis", "catmull-rom");

   // Set all arrays on the curve node
   AiNodeSetArray(curve, "radius",                    curveWidths);
   AiNodeSetArray(curve, "num_points",                curveNumPoints);
   AiNodeSetArray(curve, "points",                    curvePoints);
   AiNodeSetArray(curve, "colors",                    curveColors);

   if(m_export_curve_uvs)
   {
      AiNodeDeclare(curve, "uparamcoord", "uniform FLOAT");
      AiNodeDeclare(curve, "vparamcoord", "uniform FLOAT");
      AiNodeSetArray(curve, "uparamcoord", curveUParamCoord);
      AiNodeSetArray(curve, "vparamcoord", curveVParamCoord);
   }

   if(m_export_curve_id)
   {
      AiNodeDeclare(curve, "curve_id", "uniform UINT");
      AiNodeSetArray(curve, "curve_id", curveID);
   }
}

void CHairTranslator::ExportMotion(AtNode *curve, unsigned int step)
{
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(curve, step);

   // Same for object deformation, early out if it's not set.
   if (!IsMotionBlurEnabled(MTOA_MBLUR_DEFORM)) return;

   // Get hair lines
   MObject objectHairShape(m_dagPath.node());
   GetHairLines(objectHairShape, m_hairLines);
   
   ProcessHairLines(step,
                    AiNodeGetArray(curve, "points"),
                    AiNodeGetArray(curve, "num_points"),
                    AiNodeGetArray(curve, "radius"),
                    AiNodeGetArray(curve, "colors")
                    );

   // Clear out the temporary arrays.
   clear();
}

void CHairTranslator::ProcessHairLines(unsigned int step,
                                       AtArray* curvePoints,
                                       AtArray* curveNumPoints,
                                       AtArray* curveWidths,
                                       AtArray* curveColors,
                                       AtArray* curveID,
                                       AtArray* curveUParamCoord,
                                       AtArray* curveVParamCoord)
{

   // Iterate over all the lines
   int numPoints = 0;
   int numPointsInterpolation = 0;   
   for (unsigned int strand = 0; strand < m_hairLines.get().size(); ++strand)
   {
      MStatus status;

      MVectorArray line;
      MDoubleArray widths;
      MVectorArray colors;
      
      m_hairLines.get()[strand].GetCurvePoints(line);      
      m_hairLines.get()[strand].GetCurveWidths(widths);      
      m_hairLines.get()[strand].GetCurveColors(colors);
      m_hairLines.get()[strand].GetCurvePoints(line);      
      const int renderLineLength = line.length();

      const int pointsInterpolationLine = renderLineLength + 2;

      // Set num points
      AiArraySetUInt(curveNumPoints, strand, pointsInterpolationLine);
      

      // Set UVs
      if (m_export_curve_uvs)
      {
         // We should get the UV from the closest mesh for all connected shapes
         // To support a hairsystem that was applied to more than one mesh
         AtVector2 uvparam(AI_P2_ZERO);
         float2 root_uv;
         m_hairLines.get()[strand].GetCurveRootUV(root_uv);
         uvparam.x = root_uv[0];
         uvparam.y = root_uv[1];
         // TODO : leave an option to use exact but slow method?
         if (m_hasConnectedShapes)
            uvparam = GetHairRootUVs(line[0], m_meshInt, m_mesh);
         AiArraySetFlt(curveUParamCoord, strand, uvparam.x);
         AiArraySetFlt(curveVParamCoord, strand, uvparam.y);
      }

      if (m_export_curve_id)
      {
         AiArraySetUInt(curveID, strand, strand);
      }
   
      // Ignore one or less cv curves
      if (renderLineLength > 1)
      {
         
         MVector* lineVertex = &(line[0]);
         
         // We need a couple extra points for interpolation, Start and end
         // One at the beginning and one at the end (Only for points)
         AtPoint curvePoint;
         AiV3Create(curvePoint, static_cast<float>(lineVertex->x), static_cast<float>(lineVertex->y), static_cast<float>(lineVertex->z));
         AiArraySetPnt(curvePoints,
                       numPointsInterpolation + (step * pointsInterpolationLine),
                       curvePoint);

         AiArraySetRGB(curveColors,
                       strand + (step * m_hairLines.get().size()),
                       AiColorCreate(static_cast<float>(colors[0].x), static_cast<float>(colors[0].y), static_cast<float>(colors[0].z)));
                       
         // Run down the strand adding the points and widths.
         for (int j = 0; j < renderLineLength; ++j, ++lineVertex)
         {
            AiV3Create(curvePoint, static_cast<float>(lineVertex->x), static_cast<float>(lineVertex->y), static_cast<float>(lineVertex->z));
            AiArraySetPnt(curvePoints,
                          j+1 + numPointsInterpolation + (step * pointsInterpolationLine),
                          curvePoint);
            // Animated widths are not supported, so just export on step 0
            if (step == 0)
            {
               AiArraySetFlt(curveWidths, j + numPoints, static_cast<float>(widths[j]/2.0));
               // AiArraySetRGB(curveColors, j + curveLineStartsIdx, AiColorCreate(colors[j].x, colors[j].y, colors[j].z));
            }
         }

         // Last point duplicated.
         AiArraySetPnt(curvePoints,
                       renderLineLength+1 + numPointsInterpolation + (step * pointsInterpolationLine),
                       curvePoint);  

         numPoints += renderLineLength;
         numPointsInterpolation += pointsInterpolationLine;
       }
    }
 }
 
AtVector2 CHairTranslator::GetHairRootUVs(const MVector& lineStart, MMeshIntersector& meshInt, MFnMesh& mesh)
{
   // Find the closest point on mesh from hair lineStart
   // And returns the UV at this point

   AtVector2 rootUVs;
   float uv[2];
 
   // FIXME: meshInt.getClosestPoint(point, closest) is useless here since it doesn't return the closest face
   // and thus mesh.getUVAtPoint is called without passing a face id, which means a full intersection search again
   // (this time without the MMeshIntersector kd-tree class to help)
   MPoint point(lineStart.x, lineStart.y, lineStart.z);
   MPointOnMesh closest;
   meshInt.getClosestPoint(point, closest);
   MPoint closestPoint(closest.getPoint());

   MString currentUVSet = mesh.currentUVSetName();
   mesh.getUVAtPoint(closestPoint, uv, MSpace::kObject, &currentUVSet);
   
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

unsigned int CHairTranslator::GetHairLines(MObject& hair, CHairLines& hairLines)
{
   unsigned int numMainLines = 0;

   // Check out if there are pfx hairs
   MPlugArray pfxHairPlug;
   MFnDependencyNode fnDepNodeHair(hair);
   fnDepNodeHair.findPlug("outputRenderHairs").connectedTo(pfxHairPlug, false, true);
   if (pfxHairPlug.length() > 0)
   {
      MFnPfxGeometry pfxHair(pfxHairPlug[0].node());

      MRenderLineArray mainLines;
      MRenderLineArray leafLines;
      MRenderLineArray flowerLines;

      // And then all lines, we'll only need mainLines
      // getLineData (MRenderLineArray &mainLines, MRenderLineArray &leafLines, MRenderLineArray &flowerLines,
      // bool doLines, bool doTwist, bool doWidth, bool doFlatness, bool doParameter,
      // bool doColor, bool doIncandescence, bool doTransparency, bool worldSpace)
      pfxHair.getLineData(mainLines, leafLines, flowerLines, true, true, true, true, true, true, true, false, true);
      numMainLines += mainLines.length();
      hairLines.get().reserve(numMainLines);

      // Move all renderlines to our CHairLine
      for (unsigned int i = 0; i < numMainLines; i++)
      {
         MStatus status;
         MRenderLine renderLine = mainLines.renderLine(i, &status);
         MVectorArray line  = renderLine.getLine();
         MDoubleArray width = renderLine.getWidth();
         MVectorArray color = renderLine.getColor();
                
         // TODO : no idea how to get that info from the paintFx node,
         // or to get the corresponding follicle for a specific paintFx curve
         float root_uv[2]   = {0, 0};
         
         CHairLine hairLine;
         hairLine.SetCurvePoints(line);
         hairLine.SetCurveWidths(width);
         hairLine.SetCurveColors(color);
         hairLine.SetCurveRootUV(root_uv);     
         
         hairLines.m_numPoints += line.length();                
         hairLines.m_numPointsInterpolation += line.length() + 2;         
         hairLines.get().push_back(hairLine);
         
      }

      // As told in the docs, destructor does not free memory allocated by mainLines, leafLines etc
      mainLines.deleteArray();
      leafLines.deleteArray();
      flowerLines.deleteArray();
   }

   // Check for NURBS Curves
   MPlug outputHairPlug = fnDepNodeHair.findPlug("outputHair");
   if (outputHairPlug.numConnectedElements() > 0)
   {
      numMainLines += outputHairPlug.numConnectedElements();
      hairLines.get().reserve(numMainLines);

      float nurbsHairWidth = fnDepNodeHair.findPlug("hairWidth").asFloat();
      // NURBS hairs get width from the hairsystem multiplied by the rampattribute in that same node
      MRampAttribute hairWidthScale(fnDepNodeHair.findPlug("hairWidthScale"));

      // For all follicles, get the connected curve
      for (unsigned int i = 0; i < outputHairPlug.numConnectedElements(); i++)
      {
         MPlug connection = outputHairPlug.elementByPhysicalIndex(i);
         MPlugArray follicleHairPlug;
         connection.connectedTo(follicleHairPlug, false, true);

         if (follicleHairPlug.length() > 0)
         {
            MPlugArray nurbsHairPlug;
            MObject follicleNode = follicleHairPlug[0].node();
            MFnDependencyNode depNodeCurveHair(follicleNode);
            MString name = depNodeCurveHair.name();

            float root_uv[2];
            root_uv[0] = depNodeCurveHair.findPlug("parameterU").asFloat();
            root_uv[1] = depNodeCurveHair.findPlug("parameterV").asFloat();

            // TODO: handle blend, evaluate possible connection at follicle uvs
            MPlug colorPlug = depNodeCurveHair.findPlug("color");
            MVector fcolor;
            fcolor = MVector(colorPlug.child(0).asFloat(), colorPlug.child(1).asFloat(), colorPlug.child(2).asFloat());

            depNodeCurveHair.findPlug("outCurve").connectedTo(nurbsHairPlug, false, true);
            if (nurbsHairPlug.length() > 0)
            {
               MFnNurbsCurve follicleCurve(nurbsHairPlug[0].node());

               MPointArray cvs;
               follicleCurve.getCVs(cvs, MSpace::kWorld);
               unsigned int numcvs = cvs.length();

               MVectorArray line(numcvs);
               MDoubleArray width(numcvs);
               MVectorArray color(numcvs, fcolor);

               // Transform from MPointArray to MVectorArray
               for (unsigned int j = 0; j < numcvs; j++)
               {
                  MVector vector(cvs[j]);
                  line[j] = vector;
                  double rampParam;
                  follicleCurve.getParamAtPoint(cvs[j], rampParam);

                  // Transform the param value to 0 .. 1 range
                  rampParam = rampParam / float(follicleCurve.numSpans());
                  float rampValue;
                  hairWidthScale.getValueAtPosition(float(rampParam), rampValue);
                  width[j] = nurbsHairWidth * rampValue;
               }

               CHairLine hairLine;
               hairLine.SetCurvePoints(line);
               hairLine.SetCurveWidths(width);
               hairLine.SetCurveColors(color);
               hairLine.SetCurveRootUV(root_uv);
               hairLines.get().push_back(hairLine);
            }
         }
      }
   }

   return hairLines.get().size();
}
