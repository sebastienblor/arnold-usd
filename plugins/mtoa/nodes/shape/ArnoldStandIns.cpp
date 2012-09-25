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
MObject CArnoldStandInShape::s_deferStandinLoad;
MObject CArnoldStandInShape::s_scale;
MObject CArnoldStandInShape::s_boundingBoxMin;
MObject CArnoldStandInShape::s_boundingBoxMax;
   
enum StandinDrawingMode{
   DM_BOUNDING_BOX,
   DM_PER_OBJECT_BOUNDING_BOX,
   DM_POLYWIRE,
   DM_WIREFRAME,
   DM_POINT_CLOUD,
   DM_SHADED
};

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
   updateBBox = true;
   useSubFrame = false;
   useFrameExtension = false;
   dList = 0;
}

CArnoldStandInGeom::~CArnoldStandInGeom()
{
   for (std::vector<CArnoldStandInGeometry*>::iterator it = m_geometryList.begin();
           it != m_geometryList.end(); ++it)
      delete *it;
   m_geometryList.clear();
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
   
   // If we are in a batch render, it is not needed and it will cause the render crash. 
   if(CMayaScene::GetArnoldSession() && CMayaScene::GetArnoldSession()->IsBatch())
   {
      geom->bbox.clear();
      return MS::kSuccess;
   }
   

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
      
      // This will load correct platform library file independently of current extension
      unsigned int nchars = assfile.numChars();
      if (nchars > 3 && assfile.substringW(nchars-3, nchars).toLowerCase() == ".so")
      {
         assfile = assfile.substringW(0, nchars-4)+LIBEXT;
         isSo = true;
      }
      else if (nchars > 4 && assfile.substringW(nchars-4, nchars).toLowerCase() == ".dll")
      {
         assfile = assfile.substringW(0, nchars-5)+LIBEXT;
         isSo = true;
      }
      else if (nchars > 6 && assfile.substringW(nchars-6, nchars).toLowerCase() == ".dylib")
      {
         assfile = assfile.substringW(0, nchars-7)+LIBEXT;
         isSo = true;
      }

      // If it is a .ass or a .ass.gz file.
      if ((nchars > 4 && assfile.substring(nchars - 4, nchars).toLowerCase() == ".ass") ||
          (nchars > 7 && assfile.substring(nchars - 7, nchars).toLowerCase() == ".ass.gz"))
      {
         if (AiASSLoad(assfile.asChar()) == 0)
         {
            processRead = true;
         }
      }
      // If it is a .obj or a .ply file
      else if (nchars > 4 && (assfile.substring(nchars - 4, nchars).toLowerCase() == ".obj" || 
              assfile.substring(nchars - 4, nchars).toLowerCase() == ".ply"))
      {
         AtNode *options = AiUniverseGetOptions();
         AiNodeSetBool(options, "preserve_scene_data", true);
         AtNode * procedural = AiNode("procedural");
         AiNodeSetStr(procedural, "dso", assfile.asChar());
         AiNodeSetBool(procedural, "load_at_init", true);
         if (AiRender(AI_RENDER_MODE_FREE) == AI_SUCCESS)
         {
            processRead = true;
         }
      }
      // If it is a lib file
      else if (isSo)
      {
         AtNode *options = AiUniverseGetOptions();
         AiNodeSetBool(options, "preserve_scene_data", true);
         AtNode * procedural = AiNode("procedural");
         AiNodeSetStr(procedural, "dso", assfile.asChar());
         AiNodeSetStr(procedural, "data", dsoData.asChar());
         AiNodeSetBool(procedural, "load_at_init", true);
         if (AiRender(AI_RENDER_MODE_FREE) == AI_SUCCESS)
         {
            processRead = true;
         }
      }
      
      if (processRead)
      {
         geom->geomLoaded = geom->filename;
         //clear current geo
         geom->bbox.clear();
         
         for (std::vector<CArnoldStandInGeometry*>::iterator it = geom->m_geometryList.begin();
                 it != geom->m_geometryList.end(); ++it)
            delete *it;
         geom->m_geometryList.clear();

         AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_SHAPE);
         //iterate all shape in file

         while (!AiNodeIteratorFinished(iter))
         {
            AtNode *node = AiNodeIteratorGetNext(iter);
            if (node)
            {
               AtMatrix total_matrix;
               AiM4Identity(total_matrix);
               bool inherit_xform = true;
               AtArray* myArray;
               while(AiNodeIs(node, "ginstance"))
               {
                  AtMatrix current_matrix;
                  AiNodeGetMatrix(node, "matrix", current_matrix);
                  if (inherit_xform)
                  {
                     AiM4Mult(total_matrix, total_matrix, current_matrix);
                  }
                  inherit_xform = AiNodeGetBool(node, "inherit_xform");
                  node = (AtNode*)AiNodeGetPtr(node, "node");
               }
               if (AiNodeIs(node, "polymesh"))
               {
                  geom->m_geometryList.push_back(new CArnoldPolymeshGeometry(node, total_matrix, inherit_xform, geom->bbox));
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

MStatus CArnoldStandInShape::SetPointPlugValue(MPlug plug, float3   value)
{  
   plug.child(0).setDouble(value[0]);
   plug.child(1).setDouble(value[1]);
   plug.child(2).setDouble(value[2]);
   
   return MS::kSuccess;
}

bool CArnoldStandInShape::LoadBoundingBox()
{
   CArnoldStandInShape* nonConstThis = const_cast<CArnoldStandInShape*> (this);
   CArnoldStandInGeom* geom = nonConstThis->geometry();

   MString path_val = geom->filename;

   MString fileBase = "";
   
   if(path_val.rindexW(".ass.gz") != -1)
   {
      fileBase = path_val.substringW(0, path_val.rindexW(".ass.gz") - 1);
   }
   else
   {
      fileBase = path_val.substringW(0, path_val.rindex('.') - 1);
   }

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

   if(geom->deferStandinLoad)
   {
      // Calculate scaled BBox dimensions
      float halfSize[3] =
      {0.5f*(maxCoords[0] - minCoords[0]),
       0.5f*(maxCoords[1] - minCoords[1]),
       0.5f*(maxCoords[2] - minCoords[2])};
      float center[3] =
      {0.5f*(maxCoords[0] + minCoords[0]),
       0.5f*(maxCoords[1] + minCoords[1]),
       0.5f*(maxCoords[2] + minCoords[2])};

      minCoords[0] = -halfSize[0]*geom->scale + center[0];
      minCoords[1] = -halfSize[1]*geom->scale + center[1];
      minCoords[2] = -halfSize[2]*geom->scale + center[2];

      maxCoords[0] =  halfSize[0]*geom->scale + center[0];
      maxCoords[1] =  halfSize[1]*geom->scale + center[1];
      maxCoords[2] =  halfSize[2]*geom->scale + center[2];
   }
   
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
   eAttr.addField("Bounding Box", DM_BOUNDING_BOX);
   eAttr.addField("Per Object Bounding Box", DM_PER_OBJECT_BOUNDING_BOX);
   eAttr.addField("Polywire", DM_POLYWIRE);
   eAttr.addField("Wireframe", DM_WIREFRAME);
   eAttr.addField("Point Cloud", DM_POINT_CLOUD);
   eAttr.addField("Shaded", DM_SHADED);
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

   s_deferStandinLoad = nAttr.create("deferStandinLoad", "deferStandinLoad", MFnNumericData::kBoolean, 1);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   addAttribute(s_deferStandinLoad);

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
   
   // atributes that are used only by translation
   CAttrData data;

   data.defaultValue.BOOL = false;
   data.name = "overrideOpaque";
   data.shortName = "overrideOpaque";
   s_attributes.MakeInputBoolean(data);

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
   MString tmpData = fGeometry.data;
   bool tmpDeferStandinLoad =  fGeometry.deferStandinLoad;
   float tmpScale =  fGeometry.scale;
   bool tmpUseFrameExtension = fGeometry.useFrameExtension;
   float tmpFrameStep = fGeometry.frame + fGeometry.frameOffset;

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

   plug.setAttribute(s_deferStandinLoad);
   plug.getValue(fGeometry.deferStandinLoad);
   
   plug.setAttribute(s_scale);
   plug.getValue(fGeometry.scale);

   float framestep = fGeometry.frame + fGeometry.frameOffset;

   // Only find for frame extension files if this option is true
   if (fGeometry.useFrameExtension)
   {
      // If changed framestep, useFrameExtension or dso
      if (tmpFrameStep != framestep || tmpUseFrameExtension == false || tmpDso != fGeometry.dso)
      {
         MString frameNumber = "0";
         
         bool subFrames = ((framestep - floor(framestep)) >= 0.001);
         char frameExtWithHash[64];
         char frameExtWithDot[64];
         char frameExt[64];
         if (subFrames || fGeometry.useSubFrame)
         {
            int fullFrame = (int) floor(framestep);
            int subFrame = (int) floor((framestep - fullFrame) * 1000);
            sprintf(frameExtWithHash, "_%04d.%03d", fullFrame, subFrame);
            sprintf(frameExtWithDot, ".%04d.%03d", fullFrame, subFrame);
            sprintf(frameExt, "%04d.%03d", fullFrame, subFrame);
         }
         else
         {
            sprintf(frameExtWithHash, "_%04d", (int) framestep);
            sprintf(frameExtWithDot, ".%04d", (int) framestep);
            sprintf(frameExt, "%04d", (int) framestep);
         }
         frameNumber = frameExtWithDot;

         bool resolved = MRenderUtil::exactFileTextureName(fGeometry.dso, fGeometry.useFrameExtension,
               frameNumber, fGeometry.filename);

         if (!resolved)
         {
            frameNumber = frameExtWithHash;
            resolved = MRenderUtil::exactFileTextureName(fGeometry.dso, fGeometry.useFrameExtension,
               frameNumber, fGeometry.filename);
         }

         if (!resolved)
         {
            // If file has ".ass.gz" extension, MRenderUtil::exactFileTextureName has problems to
            //  find the file.
            int len = fGeometry.dso.length();
            if (len > 8 && fGeometry.dso.substring(len - 7, len - 1) == ".ass.gz")
            {
               MString baseName = fGeometry.dso.substring(0, len - 9) + frameExt + ".ass.gz";
               resolved = MRenderUtil::exactFileTextureName(baseName, false,
               frameNumber, fGeometry.filename);
            }
         }

         if (!resolved)
         {
            fGeometry.filename = fGeometry.dso;
         }
      }
   }
   else
   {
      fGeometry.filename = fGeometry.dso;
   }
   
   if (fGeometry.deferStandinLoad != tmpDeferStandinLoad || fGeometry.scale != tmpScale)
   {
      fGeometry.updateBBox = true;
   }

   if (fGeometry.filename != tmpFilename || fGeometry.data != tmpData)
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
      else
      {
         float3 m_value;
         m_value[0] = bbMin.x;
         m_value[1] = bbMin.y;
         m_value[2] = bbMin.z;
         plug.setAttribute(s_boundingBoxMin);
         SetPointPlugValue(plug, m_value);
         fGeometry.BBmin = MPoint(m_value[0], m_value[1], m_value[2]);

         m_value[0] = bbMax.x;
         m_value[1] = bbMax.y;
         m_value[2] = bbMax.z;
         plug.setAttribute(s_boundingBoxMax);
         SetPointPlugValue(plug, m_value);
         fGeometry.BBmax = MPoint(m_value[0], m_value[1], m_value[2]);
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
         else
         {
            float3 m_value;
            m_value[0] = bbMin.x;
            m_value[1] = bbMin.y;
            m_value[2] = bbMin.z;
            plug.setAttribute(s_boundingBoxMin);
            SetPointPlugValue(plug, m_value);
            fGeometry.BBmin = MPoint(m_value[0], m_value[1], m_value[2]);

            m_value[0] = bbMax.x;
            m_value[1] = bbMax.y;
            m_value[2] = bbMax.z;
            plug.setAttribute(s_boundingBoxMax);
            SetPointPlugValue(plug, m_value);
            fGeometry.BBmax = MPoint(m_value[0], m_value[1], m_value[2]);
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
      else
      {
         float3 m_value;
         m_value[0] = bbMin.x;
         m_value[1] = bbMin.y;
         m_value[2] = bbMin.z;
         plug.setAttribute(s_boundingBoxMin);
         SetPointPlugValue(plug, m_value);
         fGeometry.BBmin = MPoint(m_value[0], m_value[1], m_value[2]);

         m_value[0] = bbMax.x;
         m_value[1] = bbMax.y;
         m_value[2] = bbMax.z;
         plug.setAttribute(s_boundingBoxMax);
         SetPointPlugValue(plug, m_value);
         fGeometry.BBmax = MPoint(m_value[0], m_value[1], m_value[2]);
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
   // why was there a switch if everything executed the same code??
   getDrawRequestsWireFrame(request, info);
   queue.add(request);
}

void CArnoldStandInShapeUI::draw(const MDrawRequest & request, M3dView & view) const
{
   // Initialize GL function table first time through
   static MGLFunctionTable *gGLFT = NULL;
   if (gGLFT == NULL)
   {
      gGLFT = MHardwareRenderer::theRenderer()->glFunctionTable();
      CArnoldStandInGeometry::setGLFTable(gGLFT);
   }
      
   MDrawData data = request.drawData();
   CArnoldStandInGeom * geom = (CArnoldStandInGeom*) data.geometry();
   view.beginGL();
   gGLFT->glPushAttrib(MGL_ALL_ATTRIB_BITS);
   gGLFT->glEnable(MGL_DEPTH_TEST);
   gGLFT->glDepthFunc(MGL_LESS);

   if (geom->updateView || geom->updateBBox)
   {
      if (geom->dList == 0)
         geom->dList = gGLFT->glGenLists(2);

      // Only show scaled BBox in this case
      if(geom->deferStandinLoad)
      {
         MBoundingBox m_bbox = geom->bbox;
         float minPt[4];
         float maxPt[4];
         m_bbox.min().get(minPt);
         m_bbox.max().get(maxPt);
         
         // Calculate scaled BBox dimensions
         float halfSize[3] =
         {0.5f*(maxPt[0] - minPt[0]), 0.5f*(maxPt[1] - minPt[1]), 0.5f*(maxPt[2] - minPt[2])};
         float center[3] =
         {0.5f*(maxPt[0] + minPt[0]), 0.5f*(maxPt[1] + minPt[1]), 0.5f*(maxPt[2] + minPt[2])};
         float sbottomLeftFront[3] =
         {-halfSize[0]*geom->scale + center[0], -halfSize[1]*geom->scale + center[1], -halfSize[2]*geom->scale + center[2]};
         float stopLeftFront[3] =
         {-halfSize[0]*geom->scale + center[0],  halfSize[1]*geom->scale + center[1], -halfSize[2]*geom->scale + center[2]};
         float sbottomRightFront[3] =
         { halfSize[0]*geom->scale + center[0], -halfSize[1]*geom->scale + center[1], -halfSize[2]*geom->scale + center[2]};
         float stopRightFront[3] =
         { halfSize[0]*geom->scale + center[0],  halfSize[1]*geom->scale + center[1], -halfSize[2]*geom->scale + center[2]};
         float sbottomLeftBack[3] =
         {-halfSize[0]*geom->scale + center[0], -halfSize[1]*geom->scale + center[1],  halfSize[2]*geom->scale + center[2]};
         float stopLeftBack[3] =
         {-halfSize[0]*geom->scale + center[0],  halfSize[1]*geom->scale + center[1],  halfSize[2]*geom->scale + center[2]};
         float sbottomRightBack[3] =
         { halfSize[0]*geom->scale + center[0], -halfSize[1]*geom->scale + center[1],  halfSize[2]*geom->scale + center[2]};
         float stopRightBack[3] =
         { halfSize[0]*geom->scale + center[0],  halfSize[1]*geom->scale + center[1],  halfSize[2]*geom->scale + center[2]};
               
         gGLFT->glNewList(geom->dList+1, MGL_COMPILE);
         gGLFT->glBegin(MGL_LINE_STRIP);
         gGLFT->glVertex3fv(sbottomLeftFront);
         gGLFT->glVertex3fv(sbottomLeftBack);
         gGLFT->glVertex3fv(stopLeftBack);
         gGLFT->glVertex3fv(stopLeftFront);
         gGLFT->glVertex3fv(sbottomLeftFront);
         gGLFT->glVertex3fv(sbottomRightFront);
         gGLFT->glVertex3fv(sbottomRightBack);
         gGLFT->glVertex3fv(stopRightBack);
         gGLFT->glVertex3fv(stopRightFront);
         gGLFT->glVertex3fv(sbottomRightFront);
         gGLFT->glEnd();
         
         gGLFT->glBegin(MGL_LINES);
         gGLFT->glVertex3fv(sbottomLeftBack);
         gGLFT->glVertex3fv(sbottomRightBack);

         gGLFT->glVertex3fv(stopLeftBack);
         gGLFT->glVertex3fv(stopRightBack);

         gGLFT->glVertex3fv(stopLeftFront);
         gGLFT->glVertex3fv(stopRightFront);
         gGLFT->glEnd();
         gGLFT->glEndList();
      }
      geom->updateBBox = false;
   }
   
   if (geom->updateView)
   {
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
      case DM_BOUNDING_BOX:
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
      case DM_PER_OBJECT_BOUNDING_BOX:
         gGLFT->glNewList(geom->dList, MGL_COMPILE);
         for (std::vector<CArnoldStandInGeometry*>::iterator it = geom->m_geometryList.begin();
                 it != geom->m_geometryList.end(); ++it)
         {
            
            (*it)->DrawBoundingBox();
         }
         gGLFT->glEndList();
         break;
      case DM_POLYWIRE: // filled polygon
         gGLFT->glNewList(geom->dList, MGL_COMPILE);
         gGLFT->glPushAttrib(MGL_CURRENT_BIT);
         gGLFT->glEnable(MGL_POLYGON_OFFSET_FILL);
         
         gGLFT->glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
         
         for (std::vector<CArnoldStandInGeometry*>::iterator it = geom->m_geometryList.begin();
                 it != geom->m_geometryList.end(); ++it)
         {
            
            (*it)->DrawPolygons();
         }
         
         gGLFT->glPopAttrib();
         
         for (std::vector<CArnoldStandInGeometry*>::iterator it = geom->m_geometryList.begin();
                 it != geom->m_geometryList.end(); ++it)
         {
            (*it)->DrawWireframe();
         }
         
         gGLFT->glEndList();
         break;

      case DM_WIREFRAME: // wireframe
         gGLFT->glNewList(geom->dList, MGL_COMPILE);
         for (std::vector<CArnoldStandInGeometry*>::iterator it = geom->m_geometryList.begin();
                 it != geom->m_geometryList.end(); ++it)
         {
            (*it)->DrawWireframe();
         }
         gGLFT->glEndList();
         
         break;

      case DM_POINT_CLOUD: // points
         gGLFT->glPushAttrib(MGL_CURRENT_BIT);
         gGLFT->glEnable(MGL_POINT_SMOOTH);
         // Make round points, not square points and not working
         gGLFT->glHint(MGL_POINT_SMOOTH_HINT, MGL_NICEST);
         gGLFT->glEnable(MGL_BLEND);
         gGLFT->glDepthMask(MGL_TRUE);
         gGLFT->glBlendFunc(MGL_SRC_ALPHA, MGL_ONE_MINUS_SRC_ALPHA);
         gGLFT->glNewList(geom->dList, MGL_COMPILE);
         for (std::vector<CArnoldStandInGeometry*>::iterator it = geom->m_geometryList.begin();
                 it != geom->m_geometryList.end(); ++it)
         {
            (*it)->DrawPoints(); // it's a bit unnecessary to call glBegin and glEnd
            // per geometry here, but I am doing this for consistency reasons
         }
         gGLFT->glEndList();
         
         gGLFT->glDisable(MGL_POINT_SMOOTH);
         gGLFT->glPopAttrib();
         break;
      case DM_SHADED: // shaded
         gGLFT->glNewList(geom->dList, MGL_COMPILE);
         gGLFT->glPushAttrib(MGL_ALL_ATTRIB_BITS);
         gGLFT->glEnable(MGL_POLYGON_OFFSET_FILL);
         gGLFT->glEnable(MGL_LIGHTING);
         
         gGLFT->glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
         
         for (std::vector<CArnoldStandInGeometry*>::iterator it = geom->m_geometryList.begin();
                 it != geom->m_geometryList.end(); ++it)
         {
            
            (*it)->DrawNormalAndPolygons();
         }
         gGLFT->glPopAttrib();
         
         for (std::vector<CArnoldStandInGeometry*>::iterator it = geom->m_geometryList.begin();
                 it != geom->m_geometryList.end(); ++it)
         {
            (*it)->DrawWireframe();
         }         
         gGLFT->glEndList();         
         break;            
      }
      for (std::vector<CArnoldStandInGeometry*>::iterator it = geom->m_geometryList.begin();
              it != geom->m_geometryList.end(); ++it)
         delete *it;
      geom->m_geometryList.clear();
      geom->updateView = false;
   }
   
   gGLFT->glCallList(geom->dList);
   // Draw scaled BBox
   if(geom->deferStandinLoad)
   {
      gGLFT->glCallList(geom->dList+1);
   }
   gGLFT->glPopAttrib();
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

