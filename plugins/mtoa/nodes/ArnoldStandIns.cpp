#include "ArnoldStandIns.h"
#include "nodes/ArnoldNodeIDs.h"
#include "scene/NodeTranslator.h"

#include <ai_render.h>
#include <ai_dotass.h>
#include <ai_universe.h>
#include <ai_node_entry.h>
#include <ai_array.h>
#include <ai_nodes.h>
#include <ai_matrix.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MRenderUtil.h>
#include <maya/MBoundingBox.h>
#include <maya/MFloatVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MPlug.h>
#include <maya/MPoint.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MDrawData.h>
#include <maya/MPlug.h>
#include <maya/MRenderUtil.h>
#include <maya/MMatrix.h>
#include <maya/MStringArray.h>
#include <maya/MString.h>
#include <maya/MDrawRequest.h>
#include <maya/MSelectionMask.h>
#include <maya/MSelectionList.h>
#include <maya/MFnNumericData.h>

#include <cstdio>
#include <fstream>
#include <string>

#define _USE_MGL_FT_
#include <maya/MGLFunctionTable.h>
static MGLFunctionTable *gGLFT = NULL;

#define LEAD_COLOR				18	// green
#define ACTIVE_COLOR			15	// white
#define ACTIVE_AFFECTED_COLOR	8	// purple
#define DORMANT_COLOR			4	// blue
#define HILITE_COLOR			17	// pale blue
MTypeId CArnoldStandInShape::id(ARNOLD_NODEID_STAND_INS);

CStaticAttrHelper CArnoldStandInShape::s_attrHelper(CArnoldStandInShape::addAttribute);

MObject CArnoldStandInShape::s_dso;
MObject CArnoldStandInShape::s_mode;
MObject CArnoldStandInShape::s_useFrameExtension;
MObject CArnoldStandInShape::s_frameNumber;
MObject CArnoldStandInShape::s_frameOffset;
MObject CArnoldStandInShape::s_data;
MObject CArnoldStandInShape::s_loadAtInit;
MObject CArnoldStandInShape::s_boundingBoxMin;
MObject CArnoldStandInShape::s_boundingBoxMax;

CArnoldStandInShape::CArnoldStandInShape()
{
   fGeometry = new CArnoldStandInGeom;
   fGeometry->mode = 0;
   fGeometry->geomLoaded = "";
   fGeometry->dso = "";
   fGeometry->assTocLoaded = false;
   fGeometry->BBmin = MPoint(-1.0f, -1.0f, -1.0f);
   fGeometry->BBmax = MPoint(1.0f, 1.0f, 1.0f);
   fGeometry->bbox = MBoundingBox(fGeometry->BBmin, fGeometry->BBmax);
   fGeometry->IsGeomLoaded = false;
   fGeometry->dList = 0;
   fGeometry->updateView = true;

}

CArnoldStandInShape::~CArnoldStandInShape()
{
   delete fGeometry;
}

/* override */
void CArnoldStandInShape::postConstructor()
{
   // This call allows the shape to have shading groups assigned
   setRenderable(true);
}

MStatus CArnoldStandInShape::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

// Load geometry from ass to display it in the viewport.
//
MStatus CArnoldStandInShape::GetPointsFromAss()
{

   CArnoldStandInShape* nonConstThis = const_cast<CArnoldStandInShape*> (this);
   CArnoldStandInGeom* geom = nonConstThis->geometry();

   MString assfile = geom->filename;
   if (assfile != "")
   {
      AiBegin();
      if (AiASSLoad(assfile.asChar()) == 0)
      {
         geom->geomLoaded = geom->filename;
         //clear current geo
         geom->faceList.clear();

         vector<AtPoint> vertices;
         vector<AtLong> vidxs;
         vector<AtUInt> nsides;

         int num_vertices;
         int num_vidxs;
         int num_nsides;

         int i;
         int obj = 0;

         AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_SHAPE);
         //iterate all shape in file

         while (!AiNodeIteratorFinished(iter))
         {
            AtNode *node = AiNodeIteratorGetNext(iter);
            if (node)
            {
               AtArray* myArray;
               bool computeBBox = false;
               if (AiNodeIs(node, "polymesh"))
               {

                  // We can load a exact BBox, or compute it if not available
                  //const char *aname = name.asChar() + 5;res = AiNodeLookUpUserParameter(node, "bboxMin");
                  MPoint min, max;

                  if (AiNodeLookUpUserParameter(node, "bboxMin") != NULL)
                  {
                     myArray = AiNodeGetArray(node, "bboxMin");
                     AtPoint pnt = AiArrayGetPnt(myArray, 0);
                     min = MVector(pnt.x, pnt.y, pnt.z);
                     if (obj != 0)
                        geom->bbox.expand(min);
                  }
                  else
                     computeBBox = true;

                  if (AiNodeLookUpUserParameter(node, "bboxMax") != NULL && computeBBox == false)
                  {
                     myArray = AiNodeGetArray(node, "bboxMax");
                     AtPoint pnt = AiArrayGetPnt(myArray, 0);
                     max = MVector(pnt.x, pnt.y, pnt.z);
                     if (obj != 0)
                        geom->bbox.expand(max);
                     else
                        geom->bbox = MBoundingBox(min, max);

                  }

                  vertices.clear();
                  vidxs.clear();
                  nsides.clear();

                  AtMatrix current_matrix;
                  AiNodeGetMatrix(node, "matrix", current_matrix);

                  myArray = AiNodeGetArray(node, "vlist");
                  if (myArray->type == 8)
                  {
                     num_vertices = myArray->nelements * myArray->nkeys;
                     for (i = 0; i < num_vertices; i++)
                     {
                        AtPoint localTmpPnt = AiArrayGetPnt(myArray, i);
                        AiM4PointByMatrixMult(&localTmpPnt, current_matrix, &localTmpPnt);
                        vertices.push_back(localTmpPnt);
                        if (computeBBox)
                           geom->bbox.expand(MPoint(MVector(localTmpPnt.x, localTmpPnt.y,
                                 localTmpPnt.z)));
                     }
                  }
                  myArray = AiNodeGetArray(node, "vidxs");

                  if (myArray->type == 2)
                  {
                     num_vidxs = myArray->nelements * myArray->nkeys;
                     for (i = 0; i < num_vidxs; i++)
                     {
                        AtUInt localTmpPnt = AiArrayGetUInt(myArray, i);
                        vidxs.push_back(localTmpPnt);

                     }
                  }

                  myArray = AiNodeGetArray(node, "nsides");
                  if (myArray->type == 2)
                  {
                     num_nsides = myArray->nelements * myArray->nkeys;
                     for (i = 0; i < num_nsides; i++)
                     {
                        AtUInt localTmpPnt = AiArrayGetUInt(myArray, i);
                        nsides.push_back(localTmpPnt);
                     }
                  }
                  // Now we treat the datas for drawing purpose.
                  geom->faceList.resize(obj + 1);
                  int cursor = 0;
                  for (int j = 0; j < num_nsides; j++)
                  {
                     geom->faceList[obj].resize(j + 1);

                     int c_nsides = nsides[j];
                     for (int k = cursor; k < cursor + c_nsides; k++)
                     {
                        AtLong face = vidxs[k];
                        AtPoint pnt = vertices[face];
                        geom->faceList[obj][j].push_back(pnt);
                     }
                     cursor = cursor + c_nsides;
                  }
                  ++obj;
               }
            }
         }

         geom->IsGeomLoaded = true;
         geom->updateView = true;
         geom->assTocLoaded = true;
         return MS::kSuccess;
      }
      else
      {
         geom->IsGeomLoaded = false;
         return MS::kFailure;
      }

   }
   return MS::kFailure;
}

bool CArnoldStandInShape::getInternalValueInContext(const MPlug& plug, MDataHandle& datahandle,
      MDGContext &context)
{
   bool isOk = true;
   if (plug == s_dso)
   {
      datahandle.set(fGeometry->dso);
      isOk = true;
   }
   else if (plug == s_mode)
   {
      datahandle.set(fGeometry->mode);
      isOk = true;
   }
   else if (plug == s_useFrameExtension)
   {
      datahandle.set(fGeometry->useFrameExtension);
      isOk = true;
   }
   else if (plug == s_frameNumber)
   {
      datahandle.set(fGeometry->frame);
      isOk = true;
   }
   else if (plug == s_frameOffset)
   {
      datahandle.set(fGeometry->frameOffset);
      isOk = true;
   }
   else if (plug == s_boundingBoxMin)
   {
      float3 value;
      GetPointPlugValue(plug, value);
      fGeometry->BBmin = MPoint(value[0], value[1], value[2]);
      isOk = true;
   }
   else if (plug == s_boundingBoxMax)
   {
      float3 value;
      GetPointPlugValue(plug, value);
      fGeometry->BBmax = MPoint(value[0], value[1], value[2]);
      isOk = true;
   }
   else
   {
      isOk = MPxSurfaceShape::getInternalValue(plug, datahandle);
   }
   return isOk;
}

bool CArnoldStandInShape::setInternalValueInContext(const MPlug& plug,
      const MDataHandle& datahandle, MDGContext &context)
{
   bool isOk = true;
   if (plug == s_dso)
   {
      isOk = true;
   }
   else if (plug == s_mode)
   {
      //fix for later : Be sure that mode is in range
      isOk = true;
   }
   else if (plug == s_useFrameExtension)
   {
      isOk = true;
   }
   else if (plug == s_frameNumber)
   {
      isOk = true;
   }
   else if (plug == s_frameOffset)
   {
      isOk = true;
   }
   else if (plug == s_boundingBoxMax)
   {
      isOk = true;
   }
   else if (plug == s_boundingBoxMin)
   {
      isOk = true;
   }
   else
   {
      isOk = MPxSurfaceShape::setInternalValueInContext(plug, datahandle, context);
   }

   return isOk;
}

bool CArnoldStandInShape::isBounded() const
{
   return true;
}

MStatus CArnoldStandInShape::GetPointPlugValue(MPlug plug, float3 & value)
{
   // Retrieve the value as an MObject
   //
   MObject object;
   plug.getValue(object);

   // Convert the MObject to a float3
   //
   MFnNumericData numDataFn(object);

   numDataFn.getData(value[0], value[1], value[2]);

   return MS::kSuccess;
}

void CArnoldStandInShape::LoadBoundingBox()
{
   CArnoldStandInShape* nonConstThis = const_cast<CArnoldStandInShape*> (this);
   CArnoldStandInGeom* geom = nonConstThis->geometry();

   MString path_val = geom->filename;

   MString fileBase = path_val.substringW(0, path_val.rindex('.') - 1);

   MString asstocfile = fileBase + ".asstoc";

   std::ifstream file(asstocfile.asChar());
   if (file.is_open())
   {
      std::string line;
      std::getline(file, line);

      char *str = new char[line.length() + 1];
      strcpy(str, line.c_str());

      strtok(str, " ");
      double xmin = atof(strtok(NULL, " "));
      double ymin = atof(strtok(NULL, " "));
      double zmin = atof(strtok(NULL, " "));
      double xmax = atof(strtok(NULL, " "));
      double ymax = atof(strtok(NULL, " "));
      double zmax = atof(strtok(NULL, " "));

      file.close();
      MPoint min(xmin, ymin, zmin);
      MPoint max(xmax, ymax, zmax);
      geom->bbox = MBoundingBox(min, max);
      geom->assTocLoaded = true;
      fGeometry->updateView = true;
   }
   else
   {
      geom->assTocLoaded = false;
   }
}

MBoundingBox CArnoldStandInShape::boundingBox() const
{
   // Returns the bounding box for the shape.
   CArnoldStandInShape* nonConstThis = const_cast<CArnoldStandInShape*> (this);
   CArnoldStandInGeom* geom = nonConstThis->geometry();
   return geom->bbox;

}

void* CArnoldStandInShape::creator()
{
   return new CArnoldStandInShape();
}

MStatus CArnoldStandInShape::initialize()
{
   MFnTypedAttribute tAttr;
   MFnNumericAttribute nAttr;
   MFnEnumAttribute eAttr;

   s_attrHelper.SetNode("procedural");
   s_attrHelper.MakeInput("self_shadows");
   s_attrHelper.MakeInput("opaque");

   CDagTranslator::MakeArnoldVisibilityFlags(s_attrHelper);

   s_dso = tAttr.create("dso", "dso", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   addAttribute(s_dso);

   s_mode = eAttr.create("mode", "mode", 0);
   eAttr.addField("bounding Box", 0);
   eAttr.addField("polywire", 1);
   eAttr.addField("wireframe", 2);
   eAttr.addField("point Cloud", 3);
   //eAttr.setInternal(true);
   addAttribute(s_mode);

   s_useFrameExtension = nAttr.create("useFrameExtension", "useFrameExtension",
         MFnNumericData::kBoolean, 0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   addAttribute(s_useFrameExtension);

   s_frameNumber = nAttr.create("frameNumber", "frameNumber", MFnNumericData::kInt, 0);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   addAttribute(s_frameNumber);

   s_frameOffset = nAttr.create("frameOffset", "frameOffset", MFnNumericData::kInt, 0);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   addAttribute(s_frameOffset);

   s_data = tAttr.create("data", "data", MFnData::kString);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   addAttribute(s_data);

   s_loadAtInit = s_attrHelper.MakeInput("load_at_init");

//   CAttrData data;
//   s_attrHelper("min", data);
//   data.name = "minBoundingBox";
//   s_attrHelper(data);

   s_boundingBoxMin = nAttr.create("MinBoundingBox", "min", MFnNumericData::k3Float, -1.0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   // nAttr.setStorable(true);
   addAttribute(s_boundingBoxMin);

//   CAttrData data;
//   s_attrHelper("max", data);
//   data.name = "maxBoundingBox";
//   s_attrHelper(data);

   s_boundingBoxMax = nAttr.create("MaxBoundingBox", "max", MFnNumericData::k3Float, 1.0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   //nAttr.setStorable(true);
   addAttribute(s_boundingBoxMax);

   return MStatus::kSuccess;
}

//
// This function gets the values of all the attributes and
// assigns them to the fGeometry. Calling MPlug::getValue
// will ensure that the values are up-to-date.
//
CArnoldStandInGeom* CArnoldStandInShape::geometry()

{

   //fGeometry->updateView = false;
   int tmpMode = fGeometry->mode;

   MString tmpFilename = fGeometry->filename;
   MString tmpDso = fGeometry->dso;

   MObject this_object = thisMObject();
   MPlug plug(this_object, s_dso);
   plug.getValue(fGeometry->dso);
   plug.setAttribute(s_mode);
   plug.getValue(fGeometry->mode);

   plug.setAttribute(s_useFrameExtension);
   plug.getValue(fGeometry->useFrameExtension);
   plug.setAttribute(s_frameNumber);
   plug.getValue(fGeometry->frame);
   plug.setAttribute(s_frameOffset);
   plug.getValue(fGeometry->frameOffset);

   MString frameNumber = "0";

   frameNumber += fGeometry->frame + fGeometry->frameOffset;
   bool resolved = MRenderUtil::exactFileTextureName(fGeometry->dso, fGeometry->useFrameExtension,
         frameNumber, fGeometry->filename);

   if (!resolved)
   {
      fGeometry->filename = fGeometry->dso;
   }

   if (fGeometry->filename != tmpFilename)
   {

      //refresh bounding box
      LoadBoundingBox();
      if (fGeometry->mode != 0 && fGeometry->filename != fGeometry->geomLoaded)
      {
         MStatus load = GetPointsFromAss();

         //if we cant load the geom, we force bounding box
         if (load != MS::kSuccess)
            fGeometry->mode = 0;
         fGeometry->updateView = true;

      }

   }

   if (fGeometry->mode != tmpMode)
   {
      fGeometry->updateView = true;
      // mode not the same
      // if the mode was 0, we switched to a mode that need geometry
      if (tmpMode == 0 && fGeometry->filename != fGeometry->geomLoaded)
      {
         MStatus load = GetPointsFromAss();
         if (load != MS::kSuccess)
            fGeometry->mode = 0;
         fGeometry->updateView = true;
      }
      // if the geom was not loaded, force mode bounding box


   }

   if (fGeometry->assTocLoaded == false)
   {
      float3 value;
      plug.setAttribute(s_boundingBoxMin);
      GetPointPlugValue(plug, value);

      fGeometry->BBmin = MPoint(value[0], value[1], value[2]);
      plug.setAttribute(s_boundingBoxMax);
      GetPointPlugValue(plug, value);
      fGeometry->BBmax = MPoint(value[0], value[1], value[2]);
      fGeometry->bbox = MBoundingBox(fGeometry->BBmin, fGeometry->BBmax);
      fGeometry->updateView = true;
   }

   //boundingBox();
   return fGeometry;
}

// UI IMPLEMENTATION

CArnoldStandInShapeUI::CArnoldStandInShapeUI()
{
}
CArnoldStandInShapeUI::~CArnoldStandInShapeUI()
{
}

void* CArnoldStandInShapeUI::creator()
{
   return new CArnoldStandInShapeUI();
}

void CArnoldStandInShapeUI::getDrawRequests(const MDrawInfo & info, bool /*objectAndActiveOnly*/,
      MDrawRequestQueue & queue)
{

   // The draw data is used to pass geometry through the
   // draw queue. The data should hold all the information
   // needed to draw the shape.
   //
   MDrawData data;
   MDrawRequest request = info.getPrototype(*this);
   CArnoldStandInShape* shapeNode = (CArnoldStandInShape*) surfaceShape();
   CArnoldStandInGeom* geom = shapeNode->geometry();
   getDrawData(geom, data);
   request.setDrawData(data);

   // Are we displaying meshes?
   if (!info.objectDisplayStatus(M3dView::kDisplayMeshes))
      return;

   // Use mode status to determine how to display object

   switch (geom->mode)
   {
   case 0:
      // bounding box
      getDrawRequestsWireFrame(request, info);
      queue.add(request);
      break;
   case 1:
      // geom
      getDrawRequestsWireFrame(request, info);
      queue.add(request);
      break;
   case 2:
      // wire
      getDrawRequestsWireFrame(request, info);
      queue.add(request);
      break;
   case 3:
      // points
      getDrawRequestsWireFrame(request, info);
      queue.add(request);
      break;
   default:
      break;
   }
}

void CArnoldStandInShapeUI::draw(const MDrawRequest & request, M3dView & view) const
{

   MDrawData data = request.drawData();
   CArnoldStandInGeom * geom = (CArnoldStandInGeom*) data.geometry();
   view.beginGL();

   if (geom->updateView)
   {
      if (geom->dList == 0)
         geom->dList = gGLFT->glGenLists(1);

      MBoundingBox m_bbox = geom->bbox;
      MPoint minPt = m_bbox.min();
      MPoint maxPt = m_bbox.max();
      double bottomLeftFront[3] =
      { minPt.x, minPt.y, minPt.z };
      double topLeftFront[3] =
      { minPt.x, maxPt.y, minPt.z };
      double bottomRightFront[3] =
      { maxPt.x, minPt.y, minPt.z };
      double topRightFront[3] =
      { maxPt.x, maxPt.y, minPt.z };
      double bottomLeftBack[3] =
      { minPt.x, minPt.y, maxPt.z };
      double topLeftBack[3] =
      { minPt.x, maxPt.y, maxPt.z };
      double bottomRightBack[3] =
      { maxPt.x, minPt.y, maxPt.z };
      double topRightBack[3] =
      { maxPt.x, maxPt.y, maxPt.z };

      switch (geom->mode)
      {
      case 0:
         gGLFT->glNewList(geom->dList, MGL_COMPILE);
         gGLFT->glBegin(MGL_LINE_STRIP);

         gGLFT->glVertex3dv(bottomLeftFront);
         gGLFT->glVertex3dv(bottomLeftBack);
         gGLFT->glVertex3dv(topLeftBack);
         gGLFT->glVertex3dv(topLeftFront);
         gGLFT->glVertex3dv(bottomLeftFront);
         gGLFT->glVertex3dv(bottomRightFront);
         gGLFT->glVertex3dv(bottomRightBack);
         gGLFT->glVertex3dv(topRightBack);
         gGLFT->glVertex3dv(topRightFront);
         gGLFT->glVertex3dv(bottomRightFront);
         gGLFT->glEnd();

         gGLFT->glBegin(MGL_LINES);
         gGLFT->glVertex3dv(bottomLeftBack);
         gGLFT->glVertex3dv(bottomRightBack);

         gGLFT->glVertex3dv(topLeftBack);
         gGLFT->glVertex3dv(topRightBack);

         gGLFT->glVertex3dv(topLeftFront);
         gGLFT->glVertex3dv(topRightFront);
         gGLFT->glEnd();
         gGLFT->glEndList();
         break;

      case 1:
         gGLFT->glNewList(geom->dList, MGL_COMPILE);
         gGLFT->glPushAttrib(MGL_CURRENT_BIT);
         gGLFT->glEnable(MGL_POLYGON_OFFSET_FILL);

         for (AtUInt i = 0; i < geom->faceList.size(); ++i)
         {
            for (AtUInt j = 0; j < geom->faceList[i].size(); ++j)
            {
               gGLFT->glBegin(MGL_POLYGON);
               for (AtUInt k = 0; k < geom->faceList[i][j].size(); ++k)
               {
                  gGLFT->glColor4f(0.5, 0.5, 0.5, 1);
                  gGLFT->glVertex3f(geom->faceList[i][j][k].x, geom->faceList[i][j][k].y,
                        geom->faceList[i][j][k].z);
               }
               gGLFT->glEnd();
            }
         }

         gGLFT->glPopAttrib();
         for (AtUInt i = 0; i < geom->faceList.size(); ++i)
         {
            for (AtUInt j = 0; j < geom->faceList[i].size(); ++j)
            {
               gGLFT->glBegin(MGL_LINE_STRIP);
               for (AtUInt k = 0; k < geom->faceList[i][j].size(); ++k)
               {
                  gGLFT->glVertex3f(geom->faceList[i][j][k].x, geom->faceList[i][j][k].y,
                        geom->faceList[i][j][k].z);
               }
               gGLFT->glEnd();
            }
         }
         gGLFT->glEndList();
         break;

      case 2:
         gGLFT->glNewList(geom->dList, MGL_COMPILE);
         for (AtUInt i = 0; i < geom->faceList.size(); ++i)
         {
            for (AtUInt j = 0; j < geom->faceList[i].size(); ++j)
            {
               gGLFT->glBegin(MGL_LINE_STRIP);
               for (AtUInt k = 0; k < geom->faceList[i][j].size(); ++k)
               {
                  gGLFT->glVertex3f(geom->faceList[i][j][k].x, geom->faceList[i][j][k].y,
                        geom->faceList[i][j][k].z);
               }
               gGLFT->glEnd();
            }
         }
         gGLFT->glEndList();
         break;

      case 3:
         gGLFT->glPushAttrib(MGL_CURRENT_BIT);
         gGLFT->glEnable(MGL_POINT_SMOOTH);
         // Make round points, not square points and not working
         gGLFT->glHint(MGL_POINT_SMOOTH_HINT, MGL_NICEST);
         gGLFT->glEnable(MGL_BLEND);
         gGLFT->glDepthMask(MGL_TRUE);
         gGLFT->glBlendFunc(MGL_SRC_ALPHA, MGL_ONE_MINUS_SRC_ALPHA);
         gGLFT->glNewList(geom->dList, MGL_COMPILE);
         gGLFT->glBegin(MGL_POINTS);

         for (AtUInt i = 0; i < geom->faceList.size(); ++i)
         {
            for (AtUInt j = 0; j < geom->faceList[i].size(); ++j)
            {
               for (AtUInt k = 0; k < geom->faceList[i][j].size(); ++k)
               {
                  gGLFT->glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
                  gGLFT->glVertex3f(geom->faceList[i][j][k].x, geom->faceList[i][j][k].y,
                        geom->faceList[i][j][k].z);
               }
            }
         }
         gGLFT->glEnd();
         gGLFT->glEndList();
         gGLFT->glDisable(MGL_POINT_SMOOTH);
         gGLFT->glPopAttrib();
         break;
      }
      geom->updateView = false;
   }
   gGLFT->glCallList(geom->dList);
   view.endGL();

}

void CArnoldStandInShapeUI::getDrawRequestsWireFrame(MDrawRequest& request, const MDrawInfo& info)
{
   request.setToken(kDrawBoundingBox);
   M3dView::DisplayStatus displayStatus = info.displayStatus();
   M3dView::ColorTable activeColorTable = M3dView::kActiveColors;
   M3dView::ColorTable dormantColorTable = M3dView::kDormantColors;
   switch (displayStatus)
   {
   case M3dView::kLead:
      request.setColor(LEAD_COLOR, activeColorTable);
      break;
   case M3dView::kActive:
      request.setColor(ACTIVE_COLOR, activeColorTable);
      break;
   case M3dView::kActiveAffected:
      request.setColor(ACTIVE_AFFECTED_COLOR, activeColorTable);
      break;
   case M3dView::kDormant:
      request.setColor(DORMANT_COLOR, dormantColorTable);
      break;
   case M3dView::kHilite:
      request.setColor(HILITE_COLOR, activeColorTable);
      break;
   default:
      break;
   }

}

bool CArnoldStandInShapeUI::select(MSelectInfo &selectInfo, MSelectionList &selectionList,
      MPointArray &worldSpaceSelectPts) const
//
// Select function. Gets called when the bbox for the object is selected.
// This function just selects the object without doing any intersection tests.
//
{
   MSelectionMask priorityMask(MSelectionMask::kSelectObjectsMask);
   MSelectionList item;
   item.add(selectInfo.selectPath());
   MPoint xformedPt;
   selectInfo.addSelection(item, xformedPt, selectionList, worldSpaceSelectPts, priorityMask, false);
   return true;
}

