#include "ArnoldStandIns.h"
#include "nodes/ArnoldNodeIDs.h"
#include "translators/NodeTranslator.h"
#include "utils/Universe.h"
#include "scene/MayaScene.h"

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

#include <maya/MHardwareRenderer.h>
#include <maya/MGLFunctionTable.h>

#define LEAD_COLOR            18 // green
#define ACTIVE_COLOR       15 // white
#define ACTIVE_AFFECTED_COLOR 8  // purple
#define DORMANT_COLOR         4  // blue
#define HILITE_COLOR       17 // pale blue
MTypeId CArnoldStandInShape::id(ARNOLD_NODEID_STAND_INS);

CStaticAttrHelper CArnoldStandInShape::s_attributes(CArnoldStandInShape::addAttribute);

MObject CArnoldStandInShape::s_dso;
MObject CArnoldStandInShape::s_mode;
MObject CArnoldStandInShape::s_useFrameExtension;
MObject CArnoldStandInShape::s_frameNumber;
MObject CArnoldStandInShape::s_useSubFrame;
MObject CArnoldStandInShape::s_frameOffset;
MObject CArnoldStandInShape::s_data;
MObject CArnoldStandInShape::s_loadAtInit;
MObject CArnoldStandInShape::s_scale;
MObject CArnoldStandInShape::s_boundingBoxMin;
MObject CArnoldStandInShape::s_boundingBoxMax;

CArnoldStandInGeom::CArnoldStandInGeom()
{
   dso  = "";
   data = "";
   mode = 0;
   geomLoaded = "";
   scale = 1.0f;
   BBmin = MPoint(-1.0f, -1.0f, -1.0f);
   BBmax = MPoint(1.0f, 1.0f, 1.0f);
   bbox = MBoundingBox(BBmin, BBmax);
   IsGeomLoaded = false;
   updateView = true;
   useSubFrame = false;
   useFrameExtension = false;
   dList = 0;
}

CArnoldStandInShape::CArnoldStandInShape()
{
}

CArnoldStandInShape::~CArnoldStandInShape()
{
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
   MStatus status;

   CArnoldStandInShape* nonConstThis = const_cast<CArnoldStandInShape*> (this);
   CArnoldStandInGeom* geom = nonConstThis->geometry();

   MString assfile = geom->filename;
   MString dsoData = geom->data;
   bool AiUniverseCreated = false;
   if (assfile != "")
   {
      if (AiUniverseIsActive())
      {
         AiMsgWarning("[mtoa] There can only be one RenderSession active.");
         AiRenderAbort();
         CMayaScene::GetRenderSession()->InterruptRender();
         ArnoldUniverseEnd();
         CMayaScene::End();
      }
      
      AiUniverseCreated = ArnoldUniverseBegin();

      bool processRead = false;
      bool isSo = false;
      unsigned int nscn = assfile.numChars();
      MString ext = assfile.substringW(nscn-3, nscn).toLowerCase();
      if (ext == ".so")
      {
         isSo = true;
      }
      unsigned int nchars = assfile.numChars();
      if (nchars > 4 && assfile.substringW(nchars-3, nchars).toLowerCase() == ".so")
      {
         assfile = assfile.substringW(0, nchars-4)+LIBEXT;
         isSo = true;
      }
      else if (nchars > 4 && assfile.substringW(nchars-4, nchars).toLowerCase() == ".dll")
      {
         assfile = assfile.substringW(0, nchars-5)+LIBEXT;
         isSo = true;
      }
      else if (nchars > 4 && assfile.substringW(nchars-6, nchars).toLowerCase() == ".dylib")
      {
         assfile = assfile.substringW(0, nchars-7)+LIBEXT;
         isSo = true;
      }

      ext = assfile.substringW(nscn-4, nscn).toLowerCase();
      if (ext == ".ass" || (nscn > 7 && assfile.substring(nscn - 7, nscn).toLowerCase() == ".ass.gz"))
      {
         if (AiASSLoad(assfile.asChar()) == 0)
         {
            processRead = true;
         }
      }
      else if (ext == ".obj")
      {
         AtNode *options = AiUniverseGetOptions();
         AiNodeSetBool(options, "preserve_scene_data", true);
         AtNode * procedural = AiNode("procedural");
         AiNodeSetStr(procedural, "dso", assfile.asChar());
         AiNodeSetBool(procedural, "load_at_init", true);
         if (AiRender(AI_RENDER_MODE_FREE) == AI_SUCCESS)
            processRead = true;
         else
            processRead = false;
      }
      else if (isSo)
      {
         AtNode *options = AiUniverseGetOptions();
         AiNodeSetBool(options, "preserve_scene_data", true);
         AtNode * procedural = AiNode("procedural");
         AiNodeSetStr(procedural, "dso", assfile.asChar());
         AiNodeSetStr(procedural, "data", dsoData.asChar());
         AiNodeSetBool(procedural, "load_at_init", true);
         if (AiRender(AI_RENDER_MODE_FREE) == AI_SUCCESS)
            processRead = true;
         else
            processRead = false;
      }
      if (processRead)
      {
         geom->geomLoaded = geom->filename;
         //clear current geo
         geom->faceList.clear();
         geom->bbox.clear();

         vector<AtPoint> vertices;
         vector<AtUInt32> vidxs;
         vector<unsigned int> nsides;

         int num_vertices(0);
         int num_vidxs(0);
         int num_nsides(0);

         int i(0);
         int obj(0);

         AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_SHAPE);
         //iterate all shape in file

         while (!AiNodeIteratorFinished(iter))
         {
            AtNode *node = AiNodeIteratorGetNext(iter);
            if (node)
            {
               AtMatrix total_matrix;
               AiM4Identity(total_matrix);
               int inherit_xform = 1;
               AtArray* myArray;
               if (AiNodeIs(node, "ginstance"))
               {
                  while(AiNodeIs(node, "ginstance"))
                  {
                     AtMatrix current_matrix;
                     AiNodeGetMatrix(node, "matrix", current_matrix);
                     if (inherit_xform)
                     {
                        AiM4Mult(total_matrix, total_matrix, current_matrix);
                     }
                     inherit_xform = (int)AiNodeGetBool(node, "inherit_xform");
                     node = (AtNode*)AiNodeGetPtr(node, "node");
                  }
               }
               if (AiNodeIs(node, "polymesh"))
               {
                  // We can load a exact BBox, or compute it if not available
                  vertices.clear();
                  vidxs.clear();
                  nsides.clear();

                  AtMatrix current_matrix;
                  AiNodeGetMatrix(node, "matrix", current_matrix);
                  if (inherit_xform)
                  {
                     AiM4Mult(total_matrix, total_matrix, current_matrix);
                  }

                  myArray = AiNodeGetArray(node, "vlist");
                  if (myArray->type == 8)
                  {
                     num_vertices = myArray->nelements * myArray->nkeys;
                     vertices.resize(num_vertices);
                     for (i = 0; i < num_vertices; i++)
                     {
                        AtPoint localTmpPnt = AiArrayGetPnt(myArray, i);
                        AiM4PointByMatrixMult(&localTmpPnt, total_matrix, &localTmpPnt);
                        vertices[i] = localTmpPnt;
                        geom->bbox.expand(MPoint(MVector(localTmpPnt.x, localTmpPnt.y, localTmpPnt.z)));
                     }
                  }
                  myArray = AiNodeGetArray(node, "vidxs");

                  if (myArray->type == 2)
                  {
                     num_vidxs = myArray->nelements * myArray->nkeys;
                     vidxs.resize(num_vidxs);
                     for (i = 0; i < num_vidxs; i++)
                     {
                        unsigned int localTmpPnt = AiArrayGetUInt(myArray, i);
                        vidxs[i] = localTmpPnt;

                     }
                  }

                  myArray = AiNodeGetArray(node, "nsides");
                  if (myArray->type == 2)
                  {
                     num_nsides = myArray->nelements * myArray->nkeys;
                     nsides.resize(num_nsides);
                     for (i = 0; i < num_nsides; i++)
                     {
                        unsigned int localTmpPnt = AiArrayGetUInt(myArray, i);
                        nsides[i] = localTmpPnt;
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
                        AtUInt32 face = vidxs[k];
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
         status = MS::kSuccess;
      }
      else
      {
         geom->IsGeomLoaded = false;
         status = MS::kFailure;
      }

      if (AiUniverseCreated) ArnoldUniverseEnd();

   }
   else
   {
      status = MS::kFailure;
   }


   return status;
}

bool CArnoldStandInShape::getInternalValueInContext(const MPlug& plug, MDataHandle& datahandle,
      MDGContext &context)
{
   bool isOk = true;
   if (plug == s_dso)
   {
      datahandle.set(fGeometry.dso);
      isOk = true;
   }
   else if (plug == s_data)
   {
      datahandle.set(fGeometry.data);
      isOk = true;
   }
   else if (plug == s_mode)
   {
      datahandle.set(fGeometry.mode);
      isOk = true;
   }
   else if (plug == s_useFrameExtension)
   {
      datahandle.set(fGeometry.useFrameExtension);
      isOk = true;
   }
   else if (plug == s_frameNumber)
   {
      datahandle.set(fGeometry.frame);
      isOk = true;
   }
   else if (plug == s_useSubFrame)
   {
      datahandle.set(fGeometry.useSubFrame);
      isOk = true;
   }
   else if (plug == s_frameOffset)
   {
      datahandle.set(fGeometry.frameOffset);
      isOk = true;
   }
   else if (plug == s_scale)
   {
      datahandle.set(fGeometry.scale);
      isOk = true;
   }
   else if (plug == s_boundingBoxMin)
   {
      float3 value;
      GetPointPlugValue(plug, value);
      fGeometry.BBmin = MPoint(value[0], value[1], value[2]);
      isOk = true;
   }
   else if (plug == s_boundingBoxMax)
   {
      float3 value;
      GetPointPlugValue(plug, value);
      fGeometry.BBmax = MPoint(value[0], value[1], value[2]);
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
   else if (plug == s_data)
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
   else if (plug == s_useSubFrame)
   {
      isOk = true;
   }
   else if (plug == s_frameOffset)
   {
      isOk = true;
   }
   else if (plug == s_scale)
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

bool CArnoldStandInShape::LoadBoundingBox()
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
      return true;
   }
   else
   {
      return false;
   }
}

MBoundingBox CArnoldStandInShape::boundingBox() const
{
   // Returns the bounding box for the shape.
   CArnoldStandInShape* nonConstThis = const_cast<CArnoldStandInShape*> (this);
   CArnoldStandInGeom* geom = nonConstThis->geometry();
   MPoint bbMin = geom->bbox.min();
   MPoint bbMax = geom->bbox.max();

   float minCoords[4];
   float maxCoords[4];

   bbMin.get(minCoords);
   bbMax.get(maxCoords);

   minCoords[0] *= geom->scale;
   minCoords[1] *= geom->scale;
   minCoords[2] *= geom->scale;

   maxCoords[0] *= geom->scale;
   maxCoords[1] *= geom->scale;
   maxCoords[2] *= geom->scale;

   return MBoundingBox (minCoords, maxCoords);

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

   s_attributes.SetNode("procedural");

   CDagTranslator::MakeArnoldVisibilityFlags(s_attributes);

   s_dso = tAttr.create("dso", "dso", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   addAttribute(s_dso);

   s_mode = eAttr.create("mode", "mode", 0);
   eAttr.addField("Bounding Box", 0);
   eAttr.addField("Polywire", 1);
   eAttr.addField("Wireframe", 2);
   eAttr.addField("Point Cloud", 3);
   //eAttr.setInternal(true);
   addAttribute(s_mode);

   s_useFrameExtension = nAttr.create("useFrameExtension", "useFrameExtension",
         MFnNumericData::kBoolean, 0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   addAttribute(s_useFrameExtension);

   s_frameNumber = nAttr.create("frameNumber", "frameNumber", MFnNumericData::kFloat, 0);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   addAttribute(s_frameNumber);

   s_useSubFrame = nAttr.create("useSubFrame", "useSubFrame",
         MFnNumericData::kBoolean, 0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   addAttribute(s_useSubFrame);

   s_frameOffset = nAttr.create("frameOffset", "frameOffset", MFnNumericData::kFloat, 0);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   addAttribute(s_frameOffset);

   s_data = tAttr.create("data", "data", MFnData::kString);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   addAttribute(s_data);

   s_loadAtInit = s_attributes.MakeInput("load_at_init");

   s_scale = nAttr.create("BoundingBoxScale", "bboxScale", MFnNumericData::kFloat, 1.0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   addAttribute(s_scale);

   s_boundingBoxMin = nAttr.create("MinBoundingBox", "min", MFnNumericData::k3Float, -1.0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   addAttribute(s_boundingBoxMin);

   s_boundingBoxMax = nAttr.create("MaxBoundingBox", "max", MFnNumericData::k3Float, 1.0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   addAttribute(s_boundingBoxMax);

   s_attributes.MakeInput("opaque");
   s_attributes.MakeInput("self_shadows");
   s_attributes.MakeInput("sss_sample_distribution");
   s_attributes.MakeInput("sss_sample_spacing");

   return MStatus::kSuccess;
}

//
// This function gets the values of all the attributes and
// assigns them to the fGeometry. Calling MPlug::getValue
// will ensure that the values are up-to-date.
//
CArnoldStandInGeom* CArnoldStandInShape::geometry()

{
   int tmpMode = fGeometry.mode;

   MString tmpFilename = fGeometry.filename;
   MString tmpDso = fGeometry.dso;

   MObject this_object = thisMObject();
   MPlug plug(this_object, s_dso);
   plug.getValue(fGeometry.dso);

   plug.setAttribute(s_data);
   plug.getValue(fGeometry.data);

   plug.setAttribute(s_mode);
   plug.getValue(fGeometry.mode);

   plug.setAttribute(s_useFrameExtension);
   plug.getValue(fGeometry.useFrameExtension);

   plug.setAttribute(s_frameNumber);
   plug.getValue(fGeometry.frame);

   plug.setAttribute(s_useSubFrame);
   plug.getValue(fGeometry.useSubFrame);

   plug.setAttribute(s_frameOffset);
   plug.getValue(fGeometry.frameOffset);

   plug.setAttribute(s_scale);
   plug.getValue(fGeometry.scale);

   MString frameNumber = "0";

   float framestep = fGeometry.frame + fGeometry.frameOffset;

   bool subFrames = ((framestep - floor(framestep)) >= 0.001);
   char frameExt[64];
   if (subFrames || fGeometry.useSubFrame)
   {
      int fullFrame = (int) floor(framestep);
      int subFrame = (int) floor((framestep - fullFrame) * 1000);
      sprintf(frameExt, ".%04d.%03d", fullFrame, subFrame);
   }
   else
   {
      sprintf(frameExt, ".%04d", (int) framestep);
   }
   frameNumber = frameExt;

   bool resolved = MRenderUtil::exactFileTextureName(fGeometry.dso, fGeometry.useFrameExtension,
         frameNumber, fGeometry.filename);

   if (!resolved)
   {
      fGeometry.filename = fGeometry.dso;
   }

   if (fGeometry.filename != tmpFilename)
   {
      //refresh bounding box
      if (fGeometry.mode != 0 || !LoadBoundingBox())
      {
         GetPointsFromAss();
      }
      
      MPoint bbMin = fGeometry.bbox.min();
      MPoint bbMax = fGeometry.bbox.max();
      
      // If BBox has zero size, make it default size
      if (bbMin.x == bbMax.x && bbMin.y == bbMax.y && bbMin.z == bbMax.z)
      {
         float3 m_value;
         plug.setAttribute(s_boundingBoxMin);
         GetPointPlugValue(plug, m_value);
         fGeometry.BBmin = MPoint(m_value[0], m_value[1], m_value[2]);

         plug.setAttribute(s_boundingBoxMax);
         GetPointPlugValue(plug, m_value);
         fGeometry.BBmax = MPoint(m_value[0], m_value[1], m_value[2]);

         fGeometry.bbox = MBoundingBox(fGeometry.BBmin, fGeometry.BBmax);
      }
      
      fGeometry.updateView = true;
   }

   if (fGeometry.mode == 0 && fGeometry.mode != tmpMode)
   {
      // Try first to load bounding box size
      if (!LoadBoundingBox())
      {
         MPoint bbMin = fGeometry.bbox.min();
         MPoint bbMax = fGeometry.bbox.max();
         
         // If BBox has zero size, make it default size
         if (bbMin.x == bbMax.x && bbMin.y == bbMax.y && bbMin.z == bbMax.z)
         {
            float3 m_value;
            plug.setAttribute(s_boundingBoxMin);
            GetPointPlugValue(plug, m_value);
            fGeometry.BBmin = MPoint(m_value[0], m_value[1], m_value[2]);

            plug.setAttribute(s_boundingBoxMax);
            GetPointPlugValue(plug, m_value);
            fGeometry.BBmax = MPoint(m_value[0], m_value[1], m_value[2]);

            fGeometry.bbox = MBoundingBox(fGeometry.BBmin, fGeometry.BBmax);
         }
      }
      fGeometry.updateView = true;
   }
   else if (fGeometry.mode != 0 && fGeometry.mode != tmpMode)
   {
      MStatus load = GetPointsFromAss();
      //if we cant load the geom, we force bounding box
      if (load != MS::kSuccess)
         fGeometry.mode = 0;
         
      MPoint bbMin = fGeometry.bbox.min();
      MPoint bbMax = fGeometry.bbox.max();
      
      // If BBox has zero size, make it default size and mode = 0
      if (bbMin.x == bbMax.x && bbMin.y == bbMax.y && bbMin.z == bbMax.z)
      {
         float3 m_value;
         plug.setAttribute(s_boundingBoxMin);
         GetPointPlugValue(plug, m_value);
         fGeometry.BBmin = MPoint(m_value[0], m_value[1], m_value[2]);

         plug.setAttribute(s_boundingBoxMax);
         GetPointPlugValue(plug, m_value);
         fGeometry.BBmax = MPoint(m_value[0], m_value[1], m_value[2]);

         fGeometry.bbox = MBoundingBox(fGeometry.BBmin, fGeometry.BBmax);
         fGeometry.mode = 0;
      }
      
      fGeometry.updateView = true;
   }

   return &fGeometry;
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
   // Initialize GL function table first time through
   static MGLFunctionTable *gGLFT = NULL;
   if (gGLFT == NULL)
      gGLFT = MHardwareRenderer::theRenderer()->glFunctionTable();
      
   MDrawData data = request.drawData();
   CArnoldStandInGeom * geom = (CArnoldStandInGeom*) data.geometry();
   view.beginGL();

   if (geom->updateView)
   {
      if (geom->dList == 0)
         geom->dList = gGLFT->glGenLists(1);

      MBoundingBox m_bbox = geom->bbox;
      float minPt[4];
      float maxPt[4];
      m_bbox.min().get(minPt);
      m_bbox.max().get(maxPt);
      float bottomLeftFront[3] =
      { minPt[0], minPt[1], minPt[2] };
      float topLeftFront[3] =
      { minPt[0], maxPt[1], minPt[2] };
      float bottomRightFront[3] =
      { maxPt[0], minPt[1], minPt[2] };
      float topRightFront[3] =
      { maxPt[0], maxPt[1], minPt[2] };
      float bottomLeftBack[3] =
      { minPt[0], minPt[1], maxPt[2] };
      float topLeftBack[3] =
      { minPt[0], maxPt[1], maxPt[2] };
      float bottomRightBack[3] =
      { maxPt[0], minPt[1], maxPt[2] };
      float topRightBack[3] =
      { maxPt[0], maxPt[1], maxPt[2] };

      switch (geom->mode)
      {
      case 0:
         gGLFT->glNewList(geom->dList, MGL_COMPILE);
         gGLFT->glBegin(MGL_LINE_STRIP);

         gGLFT->glVertex3fv(bottomLeftFront);
         gGLFT->glVertex3fv(bottomLeftBack);
         gGLFT->glVertex3fv(topLeftBack);
         gGLFT->glVertex3fv(topLeftFront);
         gGLFT->glVertex3fv(bottomLeftFront);
         gGLFT->glVertex3fv(bottomRightFront);
         gGLFT->glVertex3fv(bottomRightBack);
         gGLFT->glVertex3fv(topRightBack);
         gGLFT->glVertex3fv(topRightFront);
         gGLFT->glVertex3fv(bottomRightFront);
         gGLFT->glEnd();

         gGLFT->glBegin(MGL_LINES);
         gGLFT->glVertex3fv(bottomLeftBack);
         gGLFT->glVertex3fv(bottomRightBack);

         gGLFT->glVertex3fv(topLeftBack);
         gGLFT->glVertex3fv(topRightBack);

         gGLFT->glVertex3fv(topLeftFront);
         gGLFT->glVertex3fv(topRightFront);
         gGLFT->glEnd();
         gGLFT->glEndList();
         break;

      case 1:
         gGLFT->glNewList(geom->dList, MGL_COMPILE);
         gGLFT->glPushAttrib(MGL_CURRENT_BIT);
         gGLFT->glEnable(MGL_POLYGON_OFFSET_FILL);

         for (unsigned int i = 0; i < geom->faceList.size(); ++i)
         {
            for (unsigned int j = 0; j < geom->faceList[i].size(); ++j)
            {
               gGLFT->glBegin(MGL_POLYGON);
               for (unsigned int k = 0; k < geom->faceList[i][j].size(); ++k)
               {
                  gGLFT->glColor4f(0.5, 0.5, 0.5, 1);
                  gGLFT->glVertex3f(geom->faceList[i][j][k].x, geom->faceList[i][j][k].y, geom->faceList[i][j][k].z);
               }
               gGLFT->glEnd();
            }
         }

         gGLFT->glPopAttrib();
         for (unsigned int i = 0; i < geom->faceList.size(); ++i)
         {
            for (unsigned int j = 0; j < geom->faceList[i].size(); ++j)
            {
               gGLFT->glBegin(MGL_LINE_STRIP);
               for (unsigned int k = 0; k < geom->faceList[i][j].size(); ++k)
               {
                  gGLFT->glVertex3f(geom->faceList[i][j][k].x, geom->faceList[i][j][k].y, geom->faceList[i][j][k].z);
               }
               gGLFT->glEnd();
            }
         }
         gGLFT->glEndList();
         break;

      case 2:
         gGLFT->glNewList(geom->dList, MGL_COMPILE);
         for (unsigned int i = 0; i < geom->faceList.size(); ++i)
         {
            for (unsigned int j = 0; j < geom->faceList[i].size(); ++j)
            {
               gGLFT->glBegin(MGL_LINE_STRIP);
               for (unsigned int k = 0; k < geom->faceList[i][j].size(); ++k)
               {
                  gGLFT->glVertex3f(geom->faceList[i][j][k].x, geom->faceList[i][j][k].y, geom->faceList[i][j][k].z);
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

         for (unsigned int i = 0; i < geom->faceList.size(); ++i)
         {
            for (unsigned int j = 0; j < geom->faceList[i].size(); ++j)
            {
               for (unsigned int k = 0; k < geom->faceList[i][j].size(); ++k)
               {
                  gGLFT->glVertex3f(geom->faceList[i][j][k].x, geom->faceList[i][j][k].y, geom->faceList[i][j][k].z);
               }
            }
         }
         gGLFT->glEnd();
         gGLFT->glEndList();
         gGLFT->glDisable(MGL_POINT_SMOOTH);
         gGLFT->glPopAttrib();
         break;
      }
      geom->faceList.clear();
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

