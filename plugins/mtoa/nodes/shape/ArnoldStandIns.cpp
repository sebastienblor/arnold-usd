#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif

#include "ArnoldStandIns.h"
#include "nodes/ArnoldNodeIDs.h"
#include "translators/DagTranslator.h"
#include "utils/Universe.h"
#include "utils/MayaUtils.h"
#include "utils/ConstantStrings.h"
#include "session/ArnoldSession.h"

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
#include <maya/MSelectInfo.h>
#include <maya/MDrawInfo.h>
#include <maya/MDrawRequest.h>
#include <maya/MDrawRequestQueue.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MDrawData.h>
#include <maya/MDistance.h>
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

MTypeId CArnoldStandInShape::id(ARNOLD_NODEID_STAND_INS);

CStaticAttrHelper CArnoldStandInShape::s_attributes(CArnoldStandInShape::addAttribute);

MObject CArnoldStandInShape::s_dso;
MObject CArnoldStandInShape::s_useFrameExtension;
MObject CArnoldStandInShape::s_frameNumber;
MObject CArnoldStandInShape::s_useSubFrame;
MObject CArnoldStandInShape::s_frameOffset;
MObject CArnoldStandInShape::s_data;
MObject CArnoldStandInShape::s_overrideNodes;

//MObject CArnoldStandInShape::s_deferStandinLoad;
//MObject CArnoldStandInShape::s_scale;
MObject CArnoldStandInShape::s_boundingBoxMin;
MObject CArnoldStandInShape::s_boundingBoxMax;
MObject CArnoldStandInShape::s_drawOverride;
MObject CArnoldStandInShape::s_namespaceName;
MObject CArnoldStandInShape::s_useAutoInstancing;
MObject CArnoldStandInShape::s_ignoreGroupNodes;
MObject CArnoldStandInShape::s_objectPath;
MObject CArnoldStandInShape::s_abcLayers;
MObject CArnoldStandInShape::s_abcFps;
MObject CArnoldStandInShape::s_abcCurvesBasis;
MObject CArnoldStandInShape::s_overrides;

CArnoldStandInData::CArnoldStandInData() : CArnoldProceduralData()
{
   dso  = "";
   //scale = 1.0f;
   MPoint BBmin = MPoint(-1.0f, -1.0f, -1.0f);
   MPoint BBmax = MPoint(1.0f, 1.0f, 1.0f);
   m_bbox = MBoundingBox(BBmin, BBmax);
   useSubFrame = false;
   useFrameExtension = false;
   dList = 0;
   drawOverride = 0;
   m_loadFile = true;
   m_updateFilename = true;
   m_hasOverrides = false;
}

CArnoldStandInData::~CArnoldStandInData()
{
   Clear();
}


CArnoldStandInShape::CArnoldStandInShape() : CArnoldBaseProcedural()
{
   m_data = new CArnoldStandInData();
}

// Load geometry from ass to display it in the viewport.
//
MStatus CArnoldStandInShape::LoadFile()
{
   MStatus status;
   CArnoldStandInData* geom = GetStandinData();
   if (geom == NULL)
      return MS::kFailure;
   
   if (!geom->m_loadFile && !geom->m_hasOverrides)
      return MS::kSuccess;

   geom->m_loadFile = false;
   
   MString assfile = geom->filename;
   float frameStep = geom->frame + geom->frameOffset;
   AtUniverse *universe = NULL;
   if (assfile != "" || geom->m_hasOverrides)
   {       
      bool processRead = false;
      bool isSo = false;
      bool isAss = false;
      bool isAbc = false;
      bool isUsd = false;

      // This will load correct platform library file independently of current extension
      unsigned int nchars = assfile.numChars();
      MStringArray splitStr;
      assfile.split('.', splitStr);
      MString ext("ass");

      if (splitStr.length() > 1)
         ext = splitStr[splitStr.length() -1].toLowerCase();
      else if (geom->m_hasOverrides)
         ext = "usd";
      
      if (ext == "so")
      {
         assfile = assfile.substringW(0, nchars - 4) + LIBEXT;
         isSo = true;
      }
      else if (ext == "dll")
      {
         assfile = assfile.substringW(0, nchars - 5) + LIBEXT;
         isSo = true;
      }
      else if (ext == "dylib")
      {
         assfile = assfile.substringW(0, nchars - 7) + LIBEXT;
         isSo = true;
      }
      else if (ext == "ass" || ext == "ass.gz")
         isAss = true;
      else if (ext == "abc")
         isAbc = true;
      else if (ext == "usd" || ext == "usda" || ext == "usdc")
         isUsd = true;      
      else if (geom->m_hasOverrides)
         isUsd = true;

      AtUniverse *proc_universe = AiUniverse();
      universe = AiUniverse();

      AtNode* options = AiUniverseGetOptions(universe);
      AiNodeSetBool(options, str::skip_license_check, true);
      AiNodeSetBool(options, str::enable_dependency_graph, false);

      MDistance dist(1.0, MDistance::uiUnit());
      AiNodeSetFlt(AiUniverseGetOptions(proc_universe), str::meters_per_unit, float(dist.asMeters()));
   

      // setup procedural search path
      MString proceduralPath = "";
      MSelectionList list;
      MObject node;
      list.add("defaultArnoldRenderOptions");
      if (list.length() > 0)
      {
         list.getDependNode(0, node);
         MFnDependencyNode fnArnoldRenderOptions(node, &status);
         if (status)
         {
            MPlug plug = fnArnoldRenderOptions.findPlug("procedural_searchpath", true);            
            if (!plug.isNull())
               proceduralPath = plug.asString();
         }
      }
      if (proceduralPath != "")
      {
#ifdef _WIN32   
         const MString pathsep = ";";
#else
         const MString pathsep = ":";
#endif
         proceduralPath += pathsep;
      }
      proceduralPath += getProjectFolderPath();
      AiNodeSetStr(options, str::procedural_searchpath, AtString(proceduralPath.asChar()));

      AtNode *proc = NULL;
      if (isAss)
      {
         proc = AiNode(proc_universe, str::procedural, AtString(name().asChar()));
      }
      if (isAbc)
      {
         proc = AiNode(proc_universe, str::alembic, AtString(name().asChar()));
         AiNodeSetFlt(proc, str::frame, frameStep);

         MPlug fpsPlug(thisMObject(), s_abcFps);
         AiNodeSetFlt(proc, str::fps, fpsPlug.asFloat());
         // AiNodeSetBool(proc, "make_instance", true); // test if this speeds things up
         MPlug objectPathPlug(thisMObject(), s_objectPath);
         AiNodeSetStr(proc, str::objectpath, AtString(objectPathPlug.asString().asChar()));
         // add the layers
         MPlug layerPlug(thisMObject(), s_abcLayers);
         MString layersString =  layerPlug.asString();

         if (layersString.length())
         {
           MStringArray layerList;
           layersString.split(';', layerList);
           AtArray* layersArray = AiArrayAllocate(layerList.length(), 1, AI_TYPE_STRING);
           for (unsigned int i=0; i < layerList.length(); i++)
              AiArraySetStr(layersArray, i, layerList[i].asChar());

           AiNodeSetArray(proc, str::layers, layersArray);
         }
      }
      else if (isUsd)
      {
         proc = AiNode(proc_universe, str::usd, AtString(name().asChar()));
         AiNodeSetFlt(proc, str::frame, frameStep);
         MPlug objectPathPlug(thisMObject(), s_objectPath);
         AiNodeSetStr(proc, str::object_path, AtString(objectPathPlug.asString().asChar()));
         if (geom->m_hasOverrides)
         {
            MPlug overridesPlug(thisMObject(), s_overrides);
            int numOverrides = overridesPlug.numElements();
            AtArray* overridesArray = AiArrayAllocate(numOverrides, 1, AI_TYPE_STRING);
            for (int o = 0; o < numOverrides; ++o)
               AiArraySetStr(overridesArray, o, overridesPlug[o].asString().asChar());
            
            AiNodeSetArray(proc, str::overrides, overridesArray);
         }
      } else
         proc = AiNode(proc_universe, str::procedural, AtString(name().asChar()));

      if (proc)
      {
        AiNodeSetStr(proc, str::filename, AtString(geom->filename.asChar()));
        processRead = true;

        AtProcViewportMode viewport_mode = AI_PROC_BOXES;

        switch (geom->m_mode)
        {
          case DM_BOUNDING_BOX:
          case DM_PER_OBJECT_BOUNDING_BOX:
             viewport_mode = AI_PROC_BOXES;
             break;
          case DM_POLYWIRE: // filled polygon
          case DM_WIREFRAME: // wireframe
          case DM_SHADED_POLYWIRE: // shaded polywire
          case DM_SHADED: // shaded
             viewport_mode = AI_PROC_POLYGONS;
             break;
          case DM_POINT_CLOUD: // points
             viewport_mode = AI_PROC_POINTS;
             break;
        }
        // get the proc geo in a new universe
        AiProceduralViewport(proc, universe, viewport_mode);
      }
      if (processRead)
         DrawUniverse(universe);
      else
         status = MS::kFailure;
            
      if (universe) AiUniverseDestroy(universe);
      if (proc_universe) AiUniverseDestroy(proc_universe);
   }
   else
   {
      status = MS::kFailure;
   }

   return status;
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

float convertToFloat(const char *number)
{
   if (!strcmp(number, "infinity"))
      return AI_BIG;
   else if (!strcmp(number, "-infinity"))
      return -AI_BIG;
   else
      return static_cast<float>(atof(number));
}


bool CArnoldStandInShape::LoadBoundingBox()
{
   CArnoldStandInData* geom = GetStandinData();
   if (geom == NULL)
      return false;
   
   // default value
   geom->m_bbox = MBoundingBox(MPoint(-1.f, -1.f, -1.f), MPoint(1.f, 1.f, 1.f));

   MString path_val = geom->filename;

   AtMetadataStore *mds = AiMetadataStore();
   AtString boundsStr;
   
   if (AiMetadataStoreLoadFromASS(mds, path_val.asChar()) && 
       AiMetadataStoreGetStr(mds, str::bounds, &boundsStr))
   {
      MString bounds(boundsStr.c_str());
      MStringArray boundsElems;
      if ((bounds.split(' ', boundsElems) == MS::kSuccess) && boundsElems.length() >= 6)
      {
         double xmin = convertToFloat(boundsElems[0].asChar());
         double ymin = convertToFloat(boundsElems[1].asChar());
         double zmin = convertToFloat(boundsElems[2].asChar());
         double xmax = convertToFloat(boundsElems[3].asChar());
         double ymax = convertToFloat(boundsElems[4].asChar());
         double zmax = convertToFloat(boundsElems[5].asChar());
         if (xmin <= xmax && ymin <= ymax && zmin <= zmax)
         {
            MPoint min(xmin, ymin, zmin);
            MPoint max(xmax, ymax, zmax);
            geom->m_bbox = MBoundingBox(min, max);
         } 
         
         AiMetadataStoreDestroy(mds);   
         return true;
      }
   }
   AiMetadataStoreDestroy(mds);

   // if the ass file doesn't have any metadata (old file),
   // then check the asstoc
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
      double xmin = convertToFloat(strtok(NULL, " "));
      double ymin = convertToFloat(strtok(NULL, " "));
      double zmin = convertToFloat(strtok(NULL, " "));
      double xmax = convertToFloat(strtok(NULL, " "));
      double ymax = convertToFloat(strtok(NULL, " "));
      double zmax = convertToFloat(strtok(NULL, " "));

      file.close();
      if (xmin <= xmax && ymin <= ymax && zmin <= zmax)
      {
         MPoint min(xmin, ymin, zmin);
         MPoint max(xmax, ymax, zmax);
         geom->m_bbox = MBoundingBox(min, max);
      } 
      
      delete []str;
      return true;
   }
   else
   {
      return false;
   }
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

   s_dso = tAttr.create("dso", "dso", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   tAttr.setUsedAsFilename(true);
   addAttribute(s_dso);

   // Need to register this attribute to appear in the filepath editor
   MString typeLabel;
   MGlobal::executeCommand("filePathEditor -query -typeLabel aiStandIn.dso", typeLabel);
   if (typeLabel != MString("Standin"))
      MGlobal::executeCommand("filePathEditor -registerType aiStandIn.dso -typeLabel \"Standin\"");

   CArnoldBaseProcedural::initializeCommonAttributes();

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

   // This "data" attribute is no longer used
   s_data = tAttr.create("data", "data", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   addAttribute(s_data);

   s_boundingBoxMin = nAttr.create("MinBoundingBox", "min", MFnNumericData::k3Float, -1.0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setAffectsAppearance(true);
   addAttribute(s_boundingBoxMin);

   s_boundingBoxMax = nAttr.create("MaxBoundingBox", "max", MFnNumericData::k3Float, 1.0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   nAttr.setAffectsAppearance(true);
   addAttribute(s_boundingBoxMax);

   s_drawOverride = eAttr.create("standInDrawOverride", "standin_draw_override");
   eAttr.addField("Use Global Settings", 0);
   eAttr.addField("Use Local Settings", 1);
   eAttr.addField("Bounding Box", 2);
   eAttr.addField("Disable Draw", 3);
   eAttr.addField("Disable Load", 4);
   eAttr.setDefault(0);
   addAttribute(s_drawOverride);
   
   s_overrideNodes = nAttr.create("overrideNodes", "override_nodes",
         MFnNumericData::kBoolean, 0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   addAttribute(s_overrideNodes);

   s_useAutoInstancing = nAttr.create("useAutoInstancing", "auto_instancing",
         MFnNumericData::kBoolean, 1);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   addAttribute(s_useAutoInstancing);

   s_namespaceName = tAttr.create("aiNamespace", "ai_namespace", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   addAttribute(s_namespaceName);

   s_overrides = tAttr.create("overrides", "overrides", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   tAttr.setArray(true);
   addAttribute(s_overrides);


   // atributes that are used only by translation
   CAttrData data;
   
   //The 'castShadows' attribute is defined in CDagTranslator::MakeMayaVisibilityFlags
   
   data.defaultValue.BOOL() = false;
   data.name = "overrideReceiveShadows";
   data.shortName = "overrideReceiveShadows";
   s_attributes.MakeInputBoolean(data);
   
   //The 'receiveShadows' attribute is defined in CDagTranslator::MakeMayaVisibilityFlags
   
   data.defaultValue.BOOL() = false;
   data.name = "overrideDoubleSided";
   data.shortName = "overrideDoubleSided";
   s_attributes.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "overrideSelfShadows";
   data.shortName = "overrideSelfShadows";
   s_attributes.MakeInputBoolean(data);
   
   //The 'self_shadows' attribute is defined in CShapeTranslator::MakeCommonAttributes

   data.defaultValue.BOOL() = false;
   data.name = "overrideOpaque";
   data.shortName = "overrideOpaque";
   s_attributes.MakeInputBoolean(data);

   //The 'opaque' attribute is defined in CShapeTranslator::MakeCommonAttributes
   data.defaultValue.BOOL() = false;
   data.name = "overrideMatte";
   data.shortName = "overrideMatte";
   s_attributes.MakeInputBoolean(data);

   data.name = "operators";
   data.shortName = "operators";
   data.type = AI_TYPE_NODE;   
   data.isArray = true;
   s_attributes.MakeInput(data);

   data.defaultValue.BOOL() = false;
   data.name = "ignoreGroupNodes";
   data.shortName = "ignore_group_nodes";
   data.isArray = false;
   s_attributes.MakeInputBoolean(data);

   //The 'matte' attribute is defined in CShapeTranslator::MakeCommonAttributes

   // USD and Alembic have object path parameter

   s_objectPath = tAttr.create("objectPath", "objectpath", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setInternal( true);
   tAttr.setStorable( true);
   addAttribute(s_objectPath);
   // Alembic attributes

   data.defaultValue.STR() = AtString("");
   data.name = "abcNamePrefix";
   data.shortName = "abc_nameprefix";
   s_attributes.MakeInputString(data);

   s_abcLayers = tAttr.create("abcLayers", "abc_layers", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setInternal( true);
   tAttr.setStorable( true);
   addAttribute(s_abcLayers);

   s_abcFps = nAttr.create("abcFPS", "abc_fps", MFnNumericData::kFloat, 24.0f);
   nAttr.setHidden(false);
   nAttr.setInternal( true);
   nAttr.setStorable( true);
   addAttribute(s_abcFps);

   data.defaultValue.STR() = AtString("width");
   data.name = "abcRadiusAttribute";
   data.shortName = "abc_radius_attribute";
   s_attributes.MakeInputString(data);

   data.defaultValue.FLT() = 0.01f;
   data.name = "abcRadiusDefault";
   data.shortName = "abc_radius_default";
   s_attributes.MakeInputFloat(data);

   data.defaultValue.FLT() = 1.0f;
   data.name = "abcRadiusScale";
   data.shortName = "abc_radius_scale";
   s_attributes.MakeInputFloat(data);

   data.defaultValue.BOOL() = false;
   data.name = "abcVelocityIgnore";
   data.shortName = "abc_velocity_ignore";
   s_attributes.MakeInputBoolean(data);

   data.defaultValue.FLT() = 1.0f;
   data.name = "abcVelocityScale";
   data.shortName = "abc_velocity_scale";
   s_attributes.MakeInputFloat(data);

   data.defaultValue.BOOL() = false;
   data.name = "abcVisibilityIgnore";
   data.shortName = "abc_visibility_ignore";
   s_attributes.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "abcMakeInstance";
   data.shortName = "abc_make_instance";
   s_attributes.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "abcPullUserParams";
   data.shortName = "abc_pull_user_params";
   s_attributes.MakeInputBoolean(data);

   data.defaultValue.BOOL() = true;
   data.name = "abcUseInstanceCache";
   data.shortName = "abc_use_instance_cache";
   s_attributes.MakeInputBoolean(data);

   s_abcCurvesBasis = eAttr.create("abcCurvesBasis", "abc_curves_basis");
   eAttr.addField("auto", 0);
   eAttr.addField("bezier", 1);
   eAttr.addField("b-spline", 2);
   eAttr.addField("catmull-rom", 3);
   eAttr.addField("linear", 4);
   eAttr.setDefault(0);
   addAttribute(s_abcCurvesBasis);

   return MStatus::kSuccess;
}

CArnoldStandInData *CArnoldStandInShape::GetStandinData()
{
   return static_cast<CArnoldStandInData*>(m_data);
}
//
// This function gets the values of all the attributes and
// assigns them to the m_data-> Calling MPlug::getValue
// will ensure that the values are up-to-date.
//
void CArnoldStandInShape::updateGeometry()   
{
   CArnoldStandInData *data = GetStandinData();
   //======== Update geom data

   MString tmpFilename = data->filename;
   
   MObject this_object = thisMObject();
   MPlug plug(this_object, s_dso);
   plug.getValue(data->dso);
   
   plug.setAttribute(s_useFrameExtension);
   plug.getValue(data->useFrameExtension);

   plug.setAttribute(s_frameNumber);
   plug.getValue(data->frame);

   plug.setAttribute(s_useSubFrame);
   plug.getValue(data->useSubFrame);

   plug.setAttribute(s_frameOffset);
   plug.getValue(data->frameOffset);

   plug.setAttribute(s_objectPath);
   plug.getValue(data->objectPath);

   plug.setAttribute(s_abcLayers);
   plug.getValue(data->abcLayers);

   plug.setAttribute(s_abcFps);
   plug.getValue(data->abcFps);
   
   plug.setAttribute(s_drawOverride); 
   plug.getValue(data->drawOverride);
   
   float3 f3_value; 
 	plug.setAttribute(s_boundingBoxMin); 
 	GetPointPlugValue(plug, f3_value); 
 	data->m_bbox.min() = MPoint(f3_value[0], f3_value[1], f3_value[2]); 

 	plug.setAttribute(s_boundingBoxMax); 
 	GetPointPlugValue(plug, f3_value); 
 	data->m_bbox.max() = MPoint(f3_value[0], f3_value[1], f3_value[2]); 

   MPlug overPlug(thisMObject(), s_overrides);
   data->m_hasOverrides = (overPlug.numElements() > 0);

 	if (data->drawOverride == 0) 
 	{ 
      MObject ArnoldRenderOptionsNode = CArnoldSession::GetDefaultArnoldRenderOptions(); 
      if (!ArnoldRenderOptionsNode.isNull()) 
         data->drawOverride = MFnDependencyNode(ArnoldRenderOptionsNode).findPlug("standin_draw_override", true).asShort(); 
 	} 
 	else 
      data->drawOverride -= 1;
   //====================================

   float framestep = data->frame + data->frameOffset;

   // If changed framestep, useFrameExtension or dso
   if (data->m_updateFilename)
   {
      data->m_updateFilename = false;
      MString frameNumber = "0";
         
      bool subFrames = ((framestep - floor(framestep)) >= 0.001);
      char frameExtWithHash[64];
      char frameExtWithDot[64];
      char frameExt[64];
         
      int start = 0;
      int end = 0;
      MStringArray pattern;
      MString newDso = "";
      int framePadding = 0;
      int subFramePadding = 0;
      bool resolved = false;
      MString a, b;

      start = data->dso.index('#');
      end = data->dso.rindex('#');

      if(start >= 0)
      {
         if(data->dso.substring(start-1,start-1) == "_")
            newDso = data->dso.substring(0,start-2) + ".#" + data->dso.substring(end+1,data->dso.length());
         else
            newDso = data->dso.substring(0,start-1) + "#" + data->dso.substring(end+1,data->dso.length());

         data->dso.substring(start,end).split('.',pattern);
         if(pattern.length() > 0)
         {
            framePadding = pattern[0].length();
            a = pattern[0];
         }
         if(pattern.length() > 1)
         {
            subFramePadding = pattern[1].length();
            b = pattern[1];
         }
      }
      else
      {
         newDso = data->dso;
      }

      if (subFrames || data->useSubFrame || (subFramePadding != 0))
      {
         int fullFrame = (int) floor(framestep);
         int subFrame = (int) floor((framestep - fullFrame) * 1000);
         sprintf(frameExtWithHash, "_%0*d.%0*d", framePadding, fullFrame, subFramePadding, subFrame);
         sprintf(frameExtWithDot, ".%0*d.%0*d", framePadding, fullFrame, subFramePadding, subFrame);
         sprintf(frameExt, "%0*d.%0*d", framePadding, fullFrame, subFramePadding, subFrame);
      }
      else
      {
         sprintf(frameExtWithHash, "_%0*d", framePadding, (int) framestep);
         sprintf(frameExtWithDot, ".%0*d", framePadding, (int) framestep);
         sprintf(frameExt, "%0*d", framePadding, (int) framestep);
      }
      frameNumber = frameExtWithDot;

      resolved = MRenderUtil::exactFileTextureName(newDso, data->useFrameExtension,
            frameNumber, data->filename);

      if (!resolved)
      {
         frameNumber = frameExtWithHash;
         resolved = MRenderUtil::exactFileTextureName(newDso, data->useFrameExtension,
            frameNumber, data->filename);
      }

      if (!resolved)
      {
         // If file has something after frame number, MRenderUtil::exactFileTextureName has problems to
         //  find the file.
         if (start >= 0)
         {
            MString baseName = data->dso.substring(0,start-1) + frameExt + data->dso.substring(end+1,data->dso.length());
            resolved = MRenderUtil::exactFileTextureName(baseName, false, frameNumber, data->filename);
         }
      }

      if (!resolved)
      {
         data->filename = data->dso;
      }
   }
   if (data->filename.length() == 0 && !data->m_hasOverrides)
      return; // early out for empty procedurals

   if (data->filename != tmpFilename)
      data->m_loadFile = true;

   // if draw is disabled
   if (data->drawOverride == 3)
      return;

   // if mode == 0 (bounding box), we first try to load the bounding box from the metadatas.
   // If we can't, we have to load the .ass file and compute it ourselves
   if (m_data->m_mode != 0 || !LoadBoundingBox()) {

      LoadFile();
   }

   MPoint bbMin = m_data->m_bbox.min();
   MPoint bbMax = m_data->m_bbox.max();
   // If BBox has zero size, make it default size
   if (bbMin.x == bbMax.x && bbMin.y == bbMax.y && bbMin.z == bbMax.z)
   {
      float3 m_value;
      plug.setAttribute(s_boundingBoxMin);
      GetPointPlugValue(plug, m_value);
      m_data->m_bbox.min() = MPoint(m_value[0], m_value[1], m_value[2]);

      plug.setAttribute(s_boundingBoxMax);
      GetPointPlugValue(plug, m_value);
      m_data->m_bbox.max() = MPoint(m_value[0], m_value[1], m_value[2]);
      
   }
   else
   {
      float3 m_value;
      m_value[0] = (float)bbMin.x;
      m_value[1] = (float)bbMin.y;
      m_value[2] = (float)bbMin.z;
      plug.setAttribute(s_boundingBoxMin);
      SetPointPlugValue(plug, m_value);
      m_data->m_bbox.min() = MPoint(m_value[0], m_value[1], m_value[2]);

      m_value[0] = (float)bbMax.x;
      m_value[1] = (float)bbMax.y;
      m_value[2] = (float)bbMax.z;
      plug.setAttribute(s_boundingBoxMax);
      SetPointPlugValue(plug, m_value);
      m_data->m_bbox.max() = MPoint(m_value[0], m_value[1], m_value[2]);
   }
}

void CArnoldStandInShape::NodeChanged(MPlug &plug)
{ 
   CArnoldStandInData *data = GetStandinData();
   if (data)
   {

      // Signal to VP2 that we require an update. By default, do it for any attribute
      //MHWRender::MRenderer::setGeometryDrawDirty(thisMObject());
      if (plug == s_useFrameExtension || plug == s_frameNumber || plug == s_useSubFrame || 
         plug == s_frameOffset || plug == s_dso)
      {
         data->m_updateFilename = true;
         data->m_isDirty = true;
         data->m_loadFile = true;
      } else if (plug == s_drawOverride || plug == s_abcFps || plug == s_abcFps)
      {
         data->m_isDirty = true;
         data->m_loadFile = true;
      } else 
      {
         MString plugName = plug.partialName(false, false, false, false, false, true);
         if (plugName == MString("mode") || plugName == "overrides")
         {
            data->m_isDirty = true;
            data->m_loadFile = true;
         }
      }
   }
   CArnoldBaseProcedural::NodeChanged(plug);

}
