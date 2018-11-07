#include "CurveCollectorTranslator.h"

#include <maya/MRenderLineArray.h>
#include <maya/MRenderLine.h>
#include <maya/MDagPathArray.h>
#include <maya/MPointArray.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MRenderUtil.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MDagMessage.h>
#include <maya/MItDag.h>
#include <maya/MMatrix.h>
#include <vector>

typedef std::vector<float> CurveWidths;

void CCurveCollectorTranslator::NodeInitializer(CAbTranslator context)
{
}

AtNode* CCurveCollectorTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("curves");
}

void CCurveCollectorTranslator::Init()
{  
   CShapeTranslator::Init();
   ComputeCurvesList(m_curveDagPaths);
}

void CCurveCollectorTranslator::ComputeCurvesList(MDagPathArray &pathArray)
{
   pathArray.clear();
   MDagPath rootPath(GetMayaDagPath());
   rootPath.pop();

   MItDag   dagIterator(MItDag::kDepthFirst, MFn::kInvalid);
   MDagPath path;
      
   for (dagIterator.reset(rootPath); (!dagIterator.isDone()); dagIterator.next())
   {
      if (dagIterator.getPath(path))
      {
         if (!path.hasFn(MFn::kCurve))
            continue;
         if (path.apiType() == MFn::kTransform)
            continue;

         if (MFnDagNode(path).isIntermediateObject())
            continue;
         pathArray.append(path);
      }      
   }      
}

static void ChildCurvesDirtyCallback(MObject& node, MPlug& plug, void* clientData)
{
   CCurveCollectorTranslator *translator = (CCurveCollectorTranslator*) clientData;
   translator->SetUpdateMode(CNodeTranslator::AI_RECREATE_NODE);
   translator->RequestUpdate();
}

static void ChildCurvesDeletedCallback(MObject& node, MDGModifier& modifier, void* clientData)
{
   CCurveCollectorTranslator *translator = (CCurveCollectorTranslator*) clientData;
   translator->SetUpdateMode(CNodeTranslator::AI_RECREATE_TRANSLATOR);
   translator->RequestUpdate();
}
static void ChildParentingChangedCallback(MDagPath &child, MDagPath &parent, void *clientData)
{
   CCurveCollectorTranslator *translator = (CCurveCollectorTranslator*) clientData;
   translator->SetUpdateMode(CNodeTranslator::AI_RECREATE_TRANSLATOR);
   CDagTranslator::ExportDagPath(child);
   translator->RequestUpdate();
}

static void ChildAddedCallback(MDagPath &child, MDagPath &parent, void *clientData)
{
   CCurveCollectorTranslator *translator = (CCurveCollectorTranslator*) clientData;
   translator->SetUpdateMode(CNodeTranslator::AI_RECREATE_TRANSLATOR);

   MItDag   dagIterator(MItDag::kDepthFirst, MFn::kInvalid);
   MDagPath path;
      
   for (dagIterator.reset(child); (!dagIterator.isDone()); dagIterator.next())
   {
      if (dagIterator.getPath(path))
      {
         CDagTranslator *oldTranslator = (CDagTranslator *)CNodeTranslator::GetTranslator(path);
         if (oldTranslator)
            oldTranslator->SetUpdateMode(CNodeTranslator::AI_DELETE_NODE);
      }
   }
   translator->RequestUpdate();
}
static void ChildRemovedCallback(MDagPath &child, MDagPath &parent, void *clientData)
{
   CCurveCollectorTranslator *translator = (CCurveCollectorTranslator*) clientData;
   translator->SetUpdateMode(CNodeTranslator::AI_RECREATE_TRANSLATOR);
   CDagTranslator::ExportDagPath(child);
   translator->RequestUpdate();
}

void CCurveCollectorTranslator::AddUpdateCallbacks()
{
   CShapeTranslator::AddUpdateCallbacks();
   MCallbackId id;
   MStatus status;
   MDagPath rootPath = GetMayaDagPath();
   rootPath.pop();

   id = MDagMessage::addChildAddedDagPathCallback(rootPath, 
                                                   ChildAddedCallback, 
                                                   this, 
                                                   &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   id = MDagMessage::addChildRemovedDagPathCallback(rootPath, 
                                                   ChildRemovedCallback, 
                                                   this, 
                                                   &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);


   // loop over each curveDagPaths, and add nodedirty over parents until m_dagPath is found
   for (unsigned int i = 0; i < m_curveDagPaths.length(); ++i)
   {
      MStatus status;
      MDagPath dagPath(m_curveDagPaths[i]);

      for(; dagPath.length() > 0 && !(dagPath == rootPath); dagPath.pop())
      {
         MObject node = dagPath.node();
         if (node != MObject::kNullObj)
         {
            // We can use the normal NodeDirtyCallback here.
            MCallbackId id = MNodeMessage::addNodeDirtyCallback(node,
                                                                ChildCurvesDirtyCallback,
                                                                this,
                                                                &status);
            if (MS::kSuccess == status) RegisterUpdateCallback(id);

            id = MNodeMessage::addNodeAboutToDeleteCallback(node,
                                                   ChildCurvesDeletedCallback,
                                                   this,
                                                   &status);
            if (MS::kSuccess == status) RegisterUpdateCallback(id);


            id = MDagMessage::addParentAddedDagPathCallback(dagPath,
                                                   ChildParentingChangedCallback, 
                                                   this, 
                                                   &status);
            if (MS::kSuccess == status) RegisterUpdateCallback(id);

            id = MDagMessage::addParentRemovedDagPathCallback(dagPath, 
                                                   ChildParentingChangedCallback, 
                                                   this, 
                                                   &status);
            if (MS::kSuccess == status) RegisterUpdateCallback(id);

            

         }
      }
   }
   
   // add callbacks for all children
   // nodeDirty, nodeDestroy, nodeRenamed, etc...
   CShapeTranslator::AddUpdateCallbacks();
}

struct CCurvesData
{
   std::vector<int> numPoints;
   std::vector<AtVector> points;
   std::vector<AtVector> referencePoints;
   std::vector<AtVector> orientations;
   std::vector<CurveWidths *> curveWidthsPerResolution; 
   std::vector<CurveWidths *> widthProfilePerResolution; 
};


// FIXME we're not handling what happens when the child curves are transformed...
// would need to apply the conversion matrix

static MStatus GetCurveSegments(MDagPath& curvePath, CCurvesData &curvesData, 
      int sampleRate, MPlug *widthPlug, MRampAttribute *rampAttr, bool exportReference, bool motion, bool orientations)
{
   MObject curve(curvePath.node());
   MFnDependencyNode fnDepNodeCurve(curve);
   MStatus stat;
   MPlug outputCurvePlug = fnDepNodeCurve.findPlug("editPoints", &stat);
   if (stat != MStatus::kSuccess)
      return MS::kSuccess;
   
   MMatrix curveMtx = curvePath.inclusiveMatrix(&stat);
   const static MMatrix identityMtx;
   bool hasMatrix = (curveMtx != identityMtx); 

   MFnNurbsCurve nurbsCurve(curve);

   double start, end;
   unsigned int numcvs;
   double incPerSample;

   
   nurbsCurve.getKnotDomain(start, end);
   // need to add 1 extremity sample #3637
   numcvs = (unsigned int)std::ceil((end - start) * sampleRate) + 1; 
   
   incPerSample = 1.0 / sampleRate;

   if (numcvs <= 1)
      return MStatus::kFailure;

   curvesData.numPoints.push_back(numcvs);
      
   // get the parameter referenceObject for each child curve
   MPlugArray conns;
   MPlug plug;

   plug = fnDepNodeCurve.findPlug("referenceObject", &stat);
   plug.connectedTo(conns, true, false);
   bool hasReferenceObject = (exportReference && motion == false && conns.length() > 0);

   MPoint point;
   if (hasReferenceObject)
   {   
      // if some of the previous curves didn't have reference points, fill the missing ones with the actual points    
      if (curvesData.referencePoints.size() < curvesData.points.size())
      {
         for (size_t i = curvesData.referencePoints.size(); i < curvesData.points.size(); ++i)
            curvesData.referencePoints.push_back(curvesData.points[i]);
      }

      MFnNurbsCurve referenceCurve(conns[0].node(), &stat);
      if (stat)
      {
         referenceCurve.getKnotDomain(start, end);
         incPerSample = (end - start) / (double)numcvs;
         for(unsigned int i = 0; i < numcvs - 1; i++)
         {
            referenceCurve.getPointAtParam(AiMin(start + incPerSample * (double)i, end), point, MSpace::kObject);
            curvesData.referencePoints.push_back(AtVector((float)point.x, (float)point.y, (float)point.z));
         }
         referenceCurve.getPointAtParam(end, point, MSpace::kObject);
         curvesData.referencePoints.push_back(AtVector((float)point.x, (float)point.y, (float)point.z));
      } 
   }
   MVector curveNormal;
   double normalLength;
   double param;
   for(unsigned int i = 0; i < (numcvs - 1); i++)
   {
      param = AiMin(start + incPerSample * (double)i, end);
      nurbsCurve.getPointAtParam(param, point, MSpace::kObject);

      if (hasMatrix)
         point *= curveMtx;

      curvesData.points.push_back(AtVector((float)point.x, (float)point.y, (float)point.z));
      if (orientations)
      {
         // when param = start, this function returns a null vector
         if (i == 0)
            param += 0.001;
         curveNormal = nurbsCurve.normal(param, MSpace::kObject);
         if (hasMatrix)
            curveNormal *= curveMtx;

         normalLength = curveNormal.length();
         if (normalLength > AI_EPSILON)
            curveNormal /= normalLength;
         else
            curveNormal = MVector(1., 0., 0.); // just to avoid crashes in arnold side

         curvesData.orientations.push_back(AtVector((float)curveNormal.x, (float)curveNormal.y, (float)curveNormal.z));
      }   

      // this extra point is only stored for motion since in ExportMotion the array is just copied. 
      // Otherwise it's better to let Export() duplicate the necessary vertices, this way we keep more information
      if (motion == true && i == 0)
      {
         curvesData.points.push_back(AtVector((float)point.x, (float)point.y, (float)point.z));
         if (orientations)
            curvesData.orientations.push_back(AtVector((float)curveNormal.x, (float)curveNormal.y, (float)curveNormal.z));  
      }

   }
   nurbsCurve.getPointAtParam(end, point, MSpace::kObject);
   if (hasMatrix)
      point *= curveMtx;

   curvesData.points.push_back(AtVector((float)point.x, (float)point.y, (float)point.z));

   if (orientations)
   {  // when param = end, this function returns a null vector
      curveNormal = nurbsCurve.normal(end - 0.001, MSpace::kObject);
      if (hasMatrix)
         curveNormal *= curveMtx;

      normalLength = curveNormal.length();
      if (normalLength > AI_EPSILON)
         curveNormal /= normalLength;
      else
         curveNormal = MVector(1., 0., 0.); // just to avoid crashes on arnold side


      curvesData.orientations.push_back(AtVector((float)curveNormal.x, (float)curveNormal.y, (float)curveNormal.z));
   }   

   // this extra point is only stored for motion since in ExportMotion the array is just copied. 
   // Otherwise it's better to let Export() duplicate the necessary vertices, this way we keep more information
   if (motion)
   {
      curvesData.points.push_back(AtVector((float)point.x, (float)point.y, (float)point.z));
      if (orientations)
      {
         // curveNormals was set just above         
         curvesData.orientations.push_back(AtVector((float)curveNormal.x, (float)curveNormal.y, (float)curveNormal.z));
      }   
   }
   
   if (widthPlug)
   {
      if (numcvs >= curvesData.curveWidthsPerResolution.size())
      {
         curvesData.curveWidthsPerResolution.resize(numcvs + 1, NULL);
      }
      
      
      // need to set the widths per amount of cvs here
      if (curvesData.curveWidthsPerResolution[numcvs] == NULL)
      {
         curvesData.curveWidthsPerResolution[numcvs] = new CurveWidths();
         curvesData.curveWidthsPerResolution[numcvs]->resize(numcvs);

         MFloatArray uCoords;
         MFloatArray vCoords;
         uCoords.setLength(numcvs);
         vCoords.setLength(numcvs);
         MFloatVectorArray resultColors;
         MFloatVectorArray resultTransparencies;
         MFloatArray filterSizes;
         filterSizes.setLength(numcvs);
         for (unsigned int i = 0; i < numcvs; ++i)
         {
            uCoords[i] = 0.0f;
            vCoords[i] = (float)i / (float)(numcvs + 1);
            filterSizes[i] = 0.001f;
         }
         MRenderUtil::sampleShadingNetwork(widthPlug->name(), numcvs, false, false, MFloatMatrix().setToIdentity(), 
                                          0, &uCoords, &vCoords, 0, 0, 0, 0, &filterSizes,
                                          resultColors, resultTransparencies);
         for (unsigned int i = 0; i < numcvs; ++i)
            (*curvesData.curveWidthsPerResolution[numcvs])[i] = resultColors[i].x;
      }
   }

   if (rampAttr)
   {
      if (numcvs >= curvesData.widthProfilePerResolution.size())
      {
         curvesData.widthProfilePerResolution.resize(numcvs + 1, NULL);
      }
      if (curvesData.widthProfilePerResolution[numcvs] == NULL)
      {
         curvesData.widthProfilePerResolution[numcvs] = new CurveWidths();
         curvesData.widthProfilePerResolution[numcvs]->resize(numcvs);

         float widthVal;
         for (unsigned int i = 0; i < numcvs; ++i)
         {
            rampAttr->getValueAtPosition((float)i / (float)(numcvs + 1), widthVal);
            (*(curvesData.widthProfilePerResolution[numcvs]))[i] = widthVal;
         }
      }
   }

   return MStatus::kSuccess;
}

static inline bool IsRampConstant(MRampAttribute &rampAttr)
{
   MIntArray indices;
   MFloatArray positions;
   MFloatArray values;
   MIntArray interps;
   MStatus status;

   rampAttr.getEntries(indices, positions, values, interps, &status);
   if (status != MS::kSuccess) 
      return true;

   if (values.length() <= 1)
      return true;

   for (unsigned int i = 1; i < values.length(); ++i)
      if (std::abs(values[i] - values[0]) > AI_EPSILON)
         return false; // found a change, not constant

   // no change was found, this is constant
   return true;
}

void CCurveCollectorTranslator::Export( AtNode *curve )
{
   // We don't export the matrix, as it's already considered in each curves inclusiveMatrix.
   // Since the curve collector is necessarily the curves parent, we would be applying the matrix twice (#3228)
   //ExportMatrix(curve);
   ProcessRenderFlags(curve);

   MPlug plug;
   MFnDependencyNode fnNode(GetMayaObject());
 
   plug = FindMayaPlug("aiExportRefPoints");
   bool exportReferenceObject = (!plug.isNull()) ? plug.asBool() : false;
   
   if (RequiresShaderExport())
   {
      AtNode* shader = NULL;
      MPlugArray curveShaderPlugs;
      plug = FindMayaPlug("aiCurveShader");
      if (!plug.isNull())
      {
         plug.connectedTo(curveShaderPlugs, true, false);
         if (curveShaderPlugs.length() > 0)
         {
            shader = ExportConnectedNode(curveShaderPlugs[0]);
         }
      }
      
      if (shader == NULL)
      {
         // check if the internal root shader was already created in a previous export
         shader = GetArnoldNode("hairShader");
         if (shader == NULL)
            shader = AddArnoldNode("standard_hair", "hairShader");
         
         // Add shader uparam and vparam names
         //AiNodeSetStr(shader, "uparam", "uparamcoord");
         //AiNodeSetStr(shader, "vparam", "vparamcoord");
      }
      SetRootShader(shader);      
   }   

   m_sampleRate = 5;
   plug = FindMayaPlug("aiSampleRate");
   if (!plug.isNull())
   {
     m_sampleRate =  plug.asInt();
     if (m_sampleRate < 1)
      m_sampleRate = 1;
   }

   // curve specific Arnold render settings.
   plug = FindMayaPlug("aiMinPixelWidth");
   if (!plug.isNull()) AiNodeSetFlt(curve, "min_pixel_width", plug.asFloat());

   // Mode is an enum, 0 == ribbon, 1 == tubes, 2 == oriented
   plug = FindMayaPlug("aiMode");
   if (!plug.isNull()) AiNodeSetInt(curve, "mode", plug.asInt());

   AiNodeSetStr(curve, "basis", "catmull-rom");

   CCurvesData curvesData;
   // get all the positions

   float globalWidth = 1.0;
   std::vector<float> widths;
   MPlug widthPlug = FindMayaPlug("aiCurveWidth");
   if (!widthPlug.isNull())
      globalWidth =  widthPlug.asFloat();

   MStatus stat;
   MPlugArray conns;
   widthPlug.connectedTo(conns, true, false);
   bool widthConnected = (conns.length() > 0);

   MPlug widthProfile = FindMayaPlug("aiWidthProfile");

   MStatus status;
   MRampAttribute widthProfileAttr(widthProfile);

   bool hasWidthProfile = !IsRampConstant(widthProfileAttr);

   if (!hasWidthProfile)
   {
      float widthVal = 1.f;
      // ramp is constant, get the width at the root of the curve;
      // and apply it to the global width
      widthProfileAttr.getValueAtPosition(0, widthVal);
      globalWidth *= widthVal;
   }


   if (!widthConnected && !hasWidthProfile)
      AiNodeSetFlt(curve, "radius", globalWidth / 2.f);

   // if curves mode is "oriented" we need to output the orientations
   bool needOrientation = (AiNodeGetInt(curve, "mode") == 2);

   // now loop over the curve childs
   for (unsigned int i = 0; i < m_curveDagPaths.length(); ++i)
   {
      //MFnDagNode fnDagNodeCurveShape(objectCurveShape);
      //MFnDependencyNode(objectCurveShape).findPlug("");

      // Get curve lines
      stat = GetCurveSegments(m_curveDagPaths[i], curvesData, m_sampleRate, 
            (widthConnected) ? &widthPlug : NULL, (hasWidthProfile) ? &widthProfileAttr : NULL, exportReferenceObject, 
            false, needOrientation);
      if (stat != MStatus::kSuccess) 
         continue;
   }

   AtArray* curveNumPoints  = AiArrayAllocate(curvesData.numPoints.size(), 1, AI_TYPE_INT);
   unsigned int totalNumPoints = 0;
   unsigned int totalNumPointsInterp = 0;



   for (size_t i = 0; i < curvesData.numPoints.size(); ++i)
   {
      const int numPoints = (int)curvesData.numPoints[i];
      const int numPointsInterpolation = numPoints + 2;

      totalNumPoints += numPoints;
      totalNumPointsInterp += numPointsInterpolation;

      // Set num points
      AiArraySetInt(curveNumPoints, i, numPointsInterpolation);
   }
   AiNodeSetArray(curve, "num_points", curveNumPoints);

   // FIXME verify the first call to IsMotionBlurEnabled() is not necessary
   bool deformedPoints = IsMotionBlurEnabled() && IsMotionBlurEnabled(MTOA_MBLUR_DEFORM) && RequiresMotionData();

   AtArray* curvePoints     = AiArrayAllocate(totalNumPointsInterp, (deformedPoints) ? GetNumMotionSteps() : 1, AI_TYPE_VECTOR);
   AtArray* curveWidths     = (widthConnected || hasWidthProfile) ? AiArrayAllocate(totalNumPoints, 1, AI_TYPE_FLOAT) : NULL;
   AtArray* referenceCurvePoints = (!curvesData.referencePoints.empty()) ? AiArrayAllocate(totalNumPoints, 1, AI_TYPE_VECTOR) : NULL;

   AtArray* orientVectors   = (needOrientation) ? AiArrayAllocate(totalNumPointsInterp, (deformedPoints) ? GetNumMotionSteps() : 1, AI_TYPE_VECTOR) : NULL;
   int pointIndex = 0;

   // if the motion doesn't start on frame, there can be an offset here
   int pointArrayIndex = (deformedPoints) ? GetMotionStep() * totalNumPointsInterp : 0;
   
   // loop over each curve
   for (size_t i = 0; i < curvesData.numPoints.size(); ++i)
   {

      if (needOrientation)
         AiArraySetVec(orientVectors, pointArrayIndex, curvesData.orientations[pointIndex]);         

      // duplicating first point for this curve
      AiArraySetVec(curvePoints, pointArrayIndex++, curvesData.points[pointIndex]);

      // fill each CV
      for (int j = 0; j < curvesData.numPoints[i]; ++j)
      {
         if (referenceCurvePoints)
            AiArraySetVec(referenceCurvePoints, pointIndex, curvesData.referencePoints[pointIndex]);


         if (curveWidths)
         {
            float pointWidth = (widthConnected) ? curvesData.curveWidthsPerResolution[curvesData.numPoints[i]]->at(j) : globalWidth;

            if (hasWidthProfile)
               pointWidth *= curvesData.widthProfilePerResolution[curvesData.numPoints[i]]->at(j);
             
            AiArraySetFlt(curveWidths, pointIndex, pointWidth/2.f);
         }
         if (needOrientation)
            AiArraySetVec(orientVectors, pointArrayIndex, curvesData.orientations[pointIndex]);         

         AiArraySetVec(curvePoints, pointArrayIndex++, curvesData.points[pointIndex++]);
      }

      if (needOrientation)
         AiArraySetVec(orientVectors, pointArrayIndex, curvesData.orientations[pointIndex - 1]);         

      // duplicate last CV for this curve
      AiArraySetVec(curvePoints, pointArrayIndex++, curvesData.points[pointIndex - 1]);
   }
   AiNodeSetArray(curve, "points", curvePoints);

   if (needOrientation)
      AiNodeSetArray(curve, "orientations", orientVectors);

   if (referenceCurvePoints)
   {
      AiNodeDeclare(curve, "Pref", "varying POINT");
      AiNodeSetArray(curve, "Pref", referenceCurvePoints);
   }

   if (curveWidths)
      AiNodeSetArray(curve, "radius", curveWidths);

   // clear the widths per resolution
   for (size_t i = 0; i < curvesData.curveWidthsPerResolution.size(); ++i)
      delete curvesData.curveWidthsPerResolution[i];

   for (size_t i = 0; i < curvesData.widthProfilePerResolution.size(); ++i)
      delete curvesData.widthProfilePerResolution[i];

}

void CCurveCollectorTranslator::ExportMotion( AtNode *curve )
{
   if (!IsMotionBlurEnabled()) return;

   // We don't export the matrix, as it's already considered in each curves inclusiveMatrix.
   // Since the curve collector is necessarily the curves parent, we would be applying the matrix twice (#3228)
   //ExportMatrix(curve);

   bool deformedPoints = IsMotionBlurEnabled() && IsMotionBlurEnabled(MTOA_MBLUR_DEFORM) && RequiresMotionData();
   if (!deformedPoints)
      return;

   CCurvesData curvesData;
   MStatus stat;

      // if curves mode is "oriented" we need to output the orientations
   bool needOrientation = (AiNodeGetInt(curve, "mode") == 2);

   // now loop over the curve childs
   for (unsigned int i = 0; i < m_curveDagPaths.length(); ++i)
   {
      // Get curve lines
      stat = GetCurveSegments(m_curveDagPaths[i], curvesData, m_sampleRate, NULL, NULL, false, true, needOrientation);
      if (stat != MStatus::kSuccess) 
         continue;
   }

   // just export the points for motion steps
   AtArray *curvePoints = AiNodeGetArray(curve, "points");
   AtArray *orientVectors = (needOrientation) ? AiNodeGetArray(curve, "orientations") : NULL;

   unsigned int totalNumPointsInterp = AiArrayGetNumElements(curvePoints);

   int stepOffset = GetMotionStep() * totalNumPointsInterp;
   

   totalNumPointsInterp = AiMin(totalNumPointsInterp, (unsigned int)curvesData.points.size());

   for (unsigned int i = 0; i < totalNumPointsInterp; ++i)
   {
      if (needOrientation)
         AiArraySetVec(orientVectors, i + stepOffset, curvesData.orientations[i]);

      AiArraySetVec(curvePoints, i + stepOffset, curvesData.points[i]);
   }


}

void CCurveCollectorTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   SetUpdateMode(AI_RECREATE_NODE);
   
   CShapeTranslator::NodeChanged(node, plug);
}
   


