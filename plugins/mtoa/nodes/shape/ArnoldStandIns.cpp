
#include "ArnoldStandIns.h"
#include "nodes/ArnoldNodeIDs.h"
#include "nodes/options/ArnoldOptionsNode.h"
#include "translators/DagTranslator.h"
#include "utils/Universe.h"
#include "scene/MayaScene.h"
#include "utils/MayaUtils.h"

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
MObject CArnoldStandInShape::s_overrideNodes;
MObject CArnoldStandInShape::s_selectedItems;
//MObject CArnoldStandInShape::s_deferStandinLoad;
//MObject CArnoldStandInShape::s_scale;
MObject CArnoldStandInShape::s_boundingBoxMin;
MObject CArnoldStandInShape::s_boundingBoxMax;
MObject CArnoldStandInShape::s_drawOverride;
MObject CArnoldStandInShape::s_namespaceName;
   
enum StandinDrawingMode{
   DM_BOUNDING_BOX,
   DM_PER_OBJECT_BOUNDING_BOX,
   DM_POLYWIRE,
   DM_WIREFRAME,
   DM_POINT_CLOUD,
   DM_SHADED_POLYWIRE,
   DM_SHADED
};

CArnoldStandInGeom::CArnoldStandInGeom()
{
   dso  = "";
   data = "";
   mode = 0;
   geomLoaded = "";
   //scale = 1.0f;
   BBmin = MPoint(-1.0f, -1.0f, -1.0f);
   BBmax = MPoint(1.0f, 1.0f, 1.0f);
   bbox = MBoundingBox(BBmin, BBmax);
   IsGeomLoaded = false;
   updateView = true;
   updateBBox = true;
   useSubFrame = false;
   useFrameExtension = false;
   dList = 0;
   drawOverride = 0;
}

CArnoldStandInGeom::~CArnoldStandInGeom()
{
   Clear();
}

void CArnoldStandInGeom::Clear()
{
   for (geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
      delete it->second;
   m_geometryList.clear();

   for (instanceListIterType it = m_instanceList.begin();
        it != m_instanceList.end(); ++it)
      delete (*it);
   m_instanceList.clear();
}

void CArnoldStandInGeom::Draw(int DrawMode)
{
   for (geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
   {
      if (it->second->Visible())
         it->second->Draw(DrawMode);
   }

   for (instanceListIterType it = m_instanceList.begin();
        it != m_instanceList.end(); ++it)
      (*it)->Draw(DrawMode);
}

size_t CArnoldStandInGeom::PointCount() const
{
    size_t totalPoints = 0;
    for (geometryListIterType it = m_geometryList.begin();
         it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible())
        {
            totalPoints += it->second->PointCount();
        }
    }

    for (instanceListIterType it = m_instanceList.begin();
         it != m_instanceList.end(); ++it)
    {
        if ((*it)->GetGeometry().Visible())
        {
            totalPoints += (*it)->GetGeometry().PointCount();
        }
    }
    return totalPoints;
}

size_t CArnoldStandInGeom::SharedVertexCount() const
{
    size_t totalPoints = 0;
    for (geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible())
            totalPoints += it->second->SharedVertexCount();
    }

    for (instanceListIterType it = m_instanceList.begin();
        it != m_instanceList.end(); ++it)
        totalPoints += (*it)->GetGeometry().SharedVertexCount();
    return totalPoints;
}

size_t CArnoldStandInGeom::WireIndexCount() const
{
    size_t total = 0;
    for (geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible())
        {
            total += it->second->WireIndexCount();
        }
    }

    for (instanceListIterType it = m_instanceList.begin();
        it != m_instanceList.end(); ++it)
    {
        if ((*it)->GetGeometry().Visible())
        {
            total += (*it)->GetGeometry().WireIndexCount();
        }
    }
    return total;
}

size_t CArnoldStandInGeom::TriangleIndexCount(bool sharedVertices) const
{
    size_t total = 0;
    for (geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible())
        {
            total += it->second->TriangleIndexCount(sharedVertices);
        }
    }

    for (instanceListIterType it = m_instanceList.begin();
        it != m_instanceList.end(); ++it)
    {
        if ((*it)->GetGeometry().Visible())
        {
            total += (*it)->GetGeometry().TriangleIndexCount(sharedVertices);
        }
    }
    return total;
}

size_t CArnoldStandInGeom::VisibleGeometryCount() const
{
    size_t total = 0;
    for (geometryListIterType it = m_geometryList.begin();
        it != m_geometryList.end(); ++it)
    {
        if (it->second->Visible())
            total++;
    }

    for (instanceListIterType it = m_instanceList.begin();
        it != m_instanceList.end(); ++it)
    {
        if ((*it)->GetGeometry().Visible())
            total++;
    }
    return total;
}

CArnoldStandInShape::CArnoldStandInShape() : m_attrChangeId(0), m_refreshAvoided(false)
{
}

CArnoldStandInShape::~CArnoldStandInShape()
{
   if (m_attrChangeId != 0)
      MMessage::removeCallback(m_attrChangeId);
}

/* override */
void CArnoldStandInShape::postConstructor()
{
   // This call allows the shape to have shading groups assigned
   setRenderable(true);

   MObject me = thisMObject();
   m_attrChangeId = MNodeMessage::addAttributeChangedCallback(me, AttrChangedCallback, this);
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

   m_refreshAvoided = false;   

   MString assfile = geom->filename;
   MString dsoData = geom->data;
   bool AiUniverseCreated = false;
   AtUniverse *universe = NULL;
   if (assfile != "")
   {  
      MPlug selPlug(thisMObject(), s_selectedItems);
      MString selectedItems;
      selPlug.getValue(selectedItems);
      MStringArray selectedItemsList;
      
      if (selectedItems.length() > 0)
         selectedItems.split(',', selectedItemsList);
      
      
      bool processRead = false;
      bool isSo = false;
      bool isAss = false;
      
      // This will load correct platform library file independently of current extension
      unsigned int nchars = assfile.numChars();
      if ((nchars > 3) && (assfile.substringW(nchars - 3, nchars-1).toLowerCase() == ".so"))
      {
         assfile = assfile.substringW(0, nchars - 4) + LIBEXT;
         isSo = true;
      }
      else if ((nchars > 4) && (assfile.substringW(nchars - 4, nchars-1).toLowerCase() == ".dll"))
      {
         assfile = assfile.substringW(0, nchars - 5) + LIBEXT;
         isSo = true;
      }
      else if ((nchars > 6) && (assfile.substringW(nchars - 6, nchars-1).toLowerCase() == ".dylib"))
      {
         assfile = assfile.substringW(0, nchars - 7) + LIBEXT;
         isSo = true;
      }
      else if ((nchars > 4) && (assfile.substringW(nchars - 4, nchars-1).toLowerCase() == ".ass"))
         isAss = true;
      else if ((nchars > 7) && (assfile.substringW(nchars - 7, nchars-1).toLowerCase() == ".ass.gz"))
         isAss = true;

      if (isAss)
      {
         if (!AiUniverseIsActive())
         {
            AiUniverseCreated = true;
            AiBegin();
         }      

         universe = AiUniverse();
      }
      else
      {
         //if (AiUniverseIsActive())
         {
            m_refreshAvoided = true;
            return MS::kSuccess;
         }                 
      }
	   
      AtNode* options = AiUniverseGetOptions(universe);
      AiNodeSetBool(options, "skip_license_check", true);
      AiNodeSetBool(options, "enable_dependency_graph", false);

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
      AiNodeSetStr(options, "procedural_searchpath", proceduralPath.asChar());      

      AtNode* procedural = 0;
      
      if (isAss)
      {
         if (fGeometry.drawOverride != 3)
         {
            AiASSLoad(universe, assfile.asChar(), AI_NODE_ALL);
            processRead = true;
         }
         else
         {
            geom->IsGeomLoaded = false;
            
            if (universe) AiUniverseDestroy(universe);
            if (AiUniverseCreated) AiEnd();
            return MS::kSuccess;
         }
      }
      else
      {
         if (universe) AiUniverseDestroy(universe);
         if (AiUniverseCreated) AiEnd();        
         return MS::kSuccess;

         // FIXME: for now we're not trying to display anything for non-ass files

        /*
         procedural = AiNode(universe, "procedural", AtString(), NULL);
         AiNodeSetStr(procedural, "filename", assfile.asChar());
//         AiNodeSetBool(procedural, "load_at_init", true);
//         if (fGeometry.drawOverride == 3) 
//            AiNodeSetBool(procedural, "load_at_init", false); 

         AiNodeSetMatrix(procedural, "matrix", AiM4Identity());
         
         if (fGeometry.drawOverride != 3)
         {
            // If it is a lib file
            if (isSo)
            {
               if (AiNodeDeclare(procedural, "used_for_maya_display", "constant BOOL"))
                  AiNodeSetBool(procedural, "used_for_maya_display", true);
               AiNodeSetStr(procedural, "data", dsoData.asChar());
               CNodeTranslator::ExportUserAttributes(procedural, thisMObject());
            }

            if (AiRender(AI_RENDER_MODE_FREE) == AI_SUCCESS)
               processRead = true;
         }
         else
         {
            geom->IsGeomLoaded = false;
            if (universe) AiUniverseDestroy(universe);
            if (AiUniverseCreated) AiEnd();            

            return MS::kSuccess;
         }*/
      }

      if (processRead)
      {
         geom->geomLoaded = geom->filename;
         //clear current geo
         geom->bbox.clear();
         
         geom->Clear();

         // iterate all shape in file twice
         // first load all the shapes
         // then resolve all the instances

         static const AtString polymesh_str("polymesh");
         static const AtString points_str("points");
         static const AtString procedural_str("procedural");
         static const AtString box_str("box");
         static const AtString ginstance_str("ginstance");

         AtNodeIterator* iter = AiUniverseGetNodeIterator(universe, AI_NODE_SHAPE);

         while (!AiNodeIteratorFinished(iter))
         {
            AtNode* node = AiNodeIteratorGetNext(iter);
            if (node == procedural)
               continue;
            if (node)
            {  
               MString nodeName = MString(AiNodeGetName(node));
               CArnoldStandInGeometry* g = 0;
               if (AiNodeIs(node, polymesh_str))
                  g = new CArnoldPolymeshGeometry(node);
               else if (AiNodeIs(node, points_str))
                  g = new CArnoldPointsGeometry(node);
               else if(AiNodeIs(node, procedural_str))
                  g = new CArnoldProceduralGeometry(node);
               else if(AiNodeIs(node, box_str))
                  g = new CArnoldBoxGeometry(node);
               else
                  continue;
               if (g->Invalid())
               {
                  delete g;
                  continue;
               }
               if (g->Visible())
                  geom->bbox.expand(g->GetBBox());  

               for (unsigned int s = 0; s < selectedItemsList.length(); ++s)
               {
                  if (selectedItemsList[s] == nodeName)
                  {
                     g->SetSelected(true);
                  }


               }
               geom->m_geometryList.insert(std::make_pair(std::string(AiNodeGetName(node)), g));
            }
         }

         AiNodeIteratorDestroy(iter);

         iter = AiUniverseGetNodeIterator(universe, AI_NODE_SHAPE);

         while (!AiNodeIteratorFinished(iter))
         {
            AtNode* node = AiNodeIteratorGetNext(iter);
            if (node == procedural)
               continue;
            if (node)
            {
               if (AiNodeGetByte(node, "visibility") == 0)
                  continue;
               AtMatrix total_matrix = AiM4Identity();
               bool inherit_xform = true;
               bool isInstance = false;
               while(AiNodeIs(node, ginstance_str))
               {                  
                  isInstance = true;
                  AtMatrix current_matrix = AiNodeGetMatrix(node, "matrix");
                  if (inherit_xform)
                  {
                     total_matrix = AiM4Mult(total_matrix, current_matrix);
                  }
                  inherit_xform = AiNodeGetBool(node, "inherit_xform");
                  node = (AtNode*)AiNodeGetPtr(node, "node");
               }
               if (!isInstance)
                  continue;
               if (AiNodeIs(node, polymesh_str) || AiNodeIs(node, points_str) || AiNodeIs(node, procedural_str))
               {
                  std::string nodeName(AiNodeGetName(node));
                  CArnoldStandInGeom::geometryListIterType iter = geom->m_geometryList.find(nodeName);
                  if (iter != geom->m_geometryList.end())
                  {
                     CArnoldStandInGInstance* gi = new CArnoldStandInGInstance(iter->second, total_matrix, inherit_xform);
                     geom->m_instanceList.push_back(gi);
                     geom->bbox.expand(gi->GetBBox());
                  }
               }               
            }
         }

         AiNodeIteratorDestroy(iter);
         geom->IsGeomLoaded = true;
         geom->updateView = true;
         status = MS::kSuccess;
      }
      else
      {
         geom->IsGeomLoaded = false;
         status = MS::kFailure;
      }

      if (universe) AiUniverseDestroy(universe);
      if (AiUniverseCreated) AiEnd();
      


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
   /*else if (plug == s_scale)
   {
      datahandle.set(fGeometry.scale);
      isOk = true;
   }*/
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
      isOk = MPxSurfaceShape::getInternalValueInContext(plug, datahandle, context);
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
   /*else if (plug == s_scale)
   {
      isOk = true;
   }*/
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
   CArnoldStandInShape* nonConstThis = const_cast<CArnoldStandInShape*> (this);
   CArnoldStandInGeom* geom = nonConstThis->geometry();
   // default value
   geom->bbox = MBoundingBox(MPoint(-1.f, -1.f, -1.f), MPoint(1.f, 1.f, 1.f));

   MString path_val = geom->filename;

#define STANDIN_USE_METADATA

#ifdef STANDIN_USE_METADATA
   AtMetadataStore *mds = AiMetadataStore();
   AtString boundsStr;
   
   if (AiMetadataStoreLoadFromASS(mds, path_val.asChar()) && 
       AiMetadataStoreGetStr(mds, AtString("bounds"), &boundsStr))
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
            geom->bbox = MBoundingBox(min, max);
         } 
         
         AiMetadataStoreDestroy(mds);   
         return true;
      }
   }
   AiMetadataStoreDestroy(mds);
#else
   // Manually parsing the ass file to extract the bounds.
   
   // First check if this ass file has metadata
   std::ifstream assfile(path_val.asChar());
   std::string assline;
   if (assfile.is_open())
   {  
      while(true)
      {    
         std::getline(assfile, assline);

         // we're assuming the metadatas are stored at the top of the ass file
         if (assline.length() > 0 && assline[0] != '#')
            break;

         if (assline.substr(0, 11) == "### bounds:")
         {
            assline = assline.substr(10);
            char *str = new char[assline.length() + 1];
            strcpy(str, assline.c_str());
            strtok(str, " ");
            double xmin = convertToFloat(strtok(NULL, " "));
            double ymin = convertToFloat(strtok(NULL, " "));
            double zmin = convertToFloat(strtok(NULL, " "));
            double xmax = convertToFloat(strtok(NULL, " "));
            double ymax = convertToFloat(strtok(NULL, " "));
            double zmax = convertToFloat(strtok(NULL, " "));
            
            if (xmin <= xmax && ymin <= ymax && zmin <= zmax)
            {
               MPoint min(xmin, ymin, zmin);
               MPoint max(xmax, ymax, zmax);
               geom->bbox = MBoundingBox(min, max);
            } 
            
            delete []str;
            return true;
         }
      }
      assfile.close();
   }
#endif


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
         geom->bbox = MBoundingBox(min, max);
      } 
      
      delete []str;
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
   MPoint bbMin = geom->BBmin;
   MPoint bbMax = geom->BBmax;

   float minCoords[4];
   float maxCoords[4];

   bbMin.get(minCoords);
   bbMax.get(maxCoords);

   /*if(geom->deferStandinLoad)
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
   */
   return MBoundingBox (minCoords, maxCoords);

}

MSelectionMask CArnoldStandInShape::getShapeSelectionMask() const
//
// Description
//     This method is overriden to support interactive object selection in Viewport 2.0
//
// Returns
//
//    The selection mask of the shape
//
{
	MSelectionMask::SelectionType selType = MSelectionMask::kSelectMeshes;
    return MSelectionMask( selType );
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

   // Why did we need to do that here ? this is invoked in translator's initialize
   //CDagTranslator::MakeArnoldVisibilityFlags(s_attributes);

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

   s_mode = eAttr.create("mode", "mode", 0);
   eAttr.addField("Bounding Box", DM_BOUNDING_BOX);
   eAttr.addField("Per Object Bounding Box", DM_PER_OBJECT_BOUNDING_BOX);
   eAttr.addField("Polywire", DM_POLYWIRE);
   eAttr.addField("Wireframe", DM_WIREFRAME);
   eAttr.addField("Point Cloud", DM_POINT_CLOUD);
   eAttr.addField("Shaded Polywire", DM_SHADED_POLYWIRE);
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

   s_namespaceName = tAttr.create("aiNamespace", "ai_namespace", MFnData::kString);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   addAttribute(s_namespaceName);

   s_selectedItems = tAttr.create("selectedItems", "selected_items", MFnData::kString);
   nAttr.setHidden(true);
   nAttr.setStorable(false);
   addAttribute(s_selectedItems);

   // atributes that are used only by translation
   CAttrData data;
   
   data.defaultValue.BOOL() = false;
   data.name = "overrideCastsShadows";
   data.shortName = "overrideCastsShadows";
   s_attributes.MakeInputBoolean(data);
   
   //The 'castShadows' attribute is defined in CDagTranslator::MakeMayaVisibilityFlags
   
   data.defaultValue.BOOL() = false;
   data.name = "overrideReceiveShadows";
   data.shortName = "overrideReceiveShadows";
   s_attributes.MakeInputBoolean(data);
   
   //The 'receiveShadows' attribute is defined in CDagTranslator::MakeMayaVisibilityFlags
   
   data.defaultValue.BOOL() = false;
   data.name = "overridePrimaryVisibility";
   data.shortName = "overridePrimaryVisibility";
   s_attributes.MakeInputBoolean(data);
   
   //The 'primaryVisibility' attribute is defined in CDagTranslator::MakeMayaVisibilityFlags
   
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
   data.name = "overrideVisibleInDiffuseReflection";
   data.shortName = "overrideVisibleInDiffuseReflection";
   s_attributes.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "overrideVisibleInSpecularReflection";
   data.shortName = "overrideVisibleInSpecularReflection";
   s_attributes.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "overrideVisibleInDiffuseTransmission";
   data.shortName = "overrideVisibleInDiffuseTransmission";
   s_attributes.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "overrideVisibleInSpecularTransmission";
   data.shortName = "overrideVisibleInSpecularTransmission";
   s_attributes.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "overrideVisibleInVolume";
   data.shortName = "overrideVisibleInVolume";
   s_attributes.MakeInputBoolean(data);
   
   data.defaultValue.BOOL() = false;
   data.name = "overrideMatte";
   data.shortName = "overrideMatte";
   s_attributes.MakeInputBoolean(data);

   data.name = "operators";
   data.shortName = "operators";
   data.type = AI_TYPE_NODE;   
   data.isArray = true;
   s_attributes.MakeInput(data);

   //The 'matte' attribute is defined in CShapeTranslator::MakeCommonAttributes

   return MStatus::kSuccess;
}

//
// This function gets the draw mode from the shape
//
int CArnoldStandInShape::drawMode()
{
	int mode;
    MPlug plug(thisMObject(), s_mode);
    plug.getValue(mode);
    return mode;
}

/*
//
// This function returns true if loading the standin should be deferred.
//
bool CArnoldStandInShape::deferStandinLoad()
{
    MPlug plug(thisMObject(), s_deferStandinLoad);
    plug.getValue(fGeometry.deferStandinLoad);
    return fGeometry.deferStandinLoad;
}
*/
//
// This function gets the values of all the attributes and
// assigns them to the fGeometry. Calling MPlug::getValue
// will ensure that the values are up-to-date.
//
CArnoldStandInGeom* CArnoldStandInShape::geometry()
{
   int tmpMode = fGeometry.mode;
   int tmpDrawOverride = fGeometry.drawOverride;

   MString tmpFilename = fGeometry.filename;
   MString tmpDso = fGeometry.dso;
   MString tmpData = fGeometry.data;
   //bool tmpDeferStandinLoad =  fGeometry.deferStandinLoad;
   //float tmpScale =  fGeometry.scale;
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

   //plug.setAttribute(s_deferStandinLoad);
   //plug.getValue(fGeometry.deferStandinLoad);
   
   //plug.setAttribute(s_scale);
   //plug.getValue(fGeometry.scale);
   
   plug.setAttribute(s_drawOverride); 
   plug.getValue(fGeometry.drawOverride);
   
   float3 f3_value; 
 	plug.setAttribute(s_boundingBoxMin); 
 	GetPointPlugValue(plug, f3_value); 
 	fGeometry.BBmin = MPoint(f3_value[0], f3_value[1], f3_value[2]); 

 	plug.setAttribute(s_boundingBoxMax); 
 	GetPointPlugValue(plug, f3_value); 
 	fGeometry.BBmax = MPoint(f3_value[0], f3_value[1], f3_value[2]); 

 	if (fGeometry.drawOverride == 0) 
 	{ 
      MObject ArnoldRenderOptionsNode = CMayaScene::GetSceneArnoldRenderOptionsNode(); 
      if (!ArnoldRenderOptionsNode.isNull()) 
         fGeometry.drawOverride = MFnDependencyNode(ArnoldRenderOptionsNode).findPlug("standin_draw_override", true).asShort(); 
 	} 
 	else 
      fGeometry.drawOverride -= 1;

   float framestep = fGeometry.frame + fGeometry.frameOffset;

   // If changed framestep, useFrameExtension or dso
   if (tmpFrameStep != framestep || tmpUseFrameExtension != fGeometry.useFrameExtension || tmpDso != fGeometry.dso)
   {
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

      start = fGeometry.dso.index('#');
      end = fGeometry.dso.rindex('#');

      if(start >= 0)
      {
         if(fGeometry.dso.substring(start-1,start-1) == "_")
            newDso = fGeometry.dso.substring(0,start-2) + ".#" + fGeometry.dso.substring(end+1,fGeometry.dso.length());
         else
            newDso = fGeometry.dso.substring(0,start-1) + "#" + fGeometry.dso.substring(end+1,fGeometry.dso.length());

         fGeometry.dso.substring(start,end).split('.',pattern);
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
         newDso = fGeometry.dso;
      }

      if (subFrames || fGeometry.useSubFrame || (subFramePadding != 0))
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

      resolved = MRenderUtil::exactFileTextureName(newDso, fGeometry.useFrameExtension,
            frameNumber, fGeometry.filename);

      if (!resolved)
      {
         frameNumber = frameExtWithHash;
         resolved = MRenderUtil::exactFileTextureName(newDso, fGeometry.useFrameExtension,
            frameNumber, fGeometry.filename);
      }

      if (!resolved)
      {
         // If file has something after frame number, MRenderUtil::exactFileTextureName has problems to
         //  find the file.
         if (start >= 0)
         {
            MString baseName = fGeometry.dso.substring(0,start-1) + frameExt + fGeometry.dso.substring(end+1,fGeometry.dso.length());
            resolved = MRenderUtil::exactFileTextureName(baseName, false, frameNumber, fGeometry.filename);
         }
      }

      if (!resolved)
      {
         fGeometry.filename = fGeometry.dso;
      }
   }
   
   // Check if something has changed that requires us to reload the .ass (or at least the bounding box)
   if (fGeometry.drawOverride != 3 && (fGeometry.filename != tmpFilename || fGeometry.data != tmpData || fGeometry.mode != tmpMode || fGeometry.drawOverride != tmpDrawOverride))
   {
      // if mode == 0 (bounding box), we first try to load the bounding box from the metadatas.
      // If we can't, we have to load the .ass file and compute it ourselves
      if (fGeometry.mode != 0 || !LoadBoundingBox())
      {
         MStatus load = GetPointsFromAss();
         //if we cant load the geom, we force bounding box
         if (load != MS::kSuccess && fGeometry.mode != 0)
         {
            plug.setAttribute(s_mode);
            plug.setValue(0);
         }
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
         // empty geometry, so set the mode to 0
         if (fGeometry.mode != 0)
         {
            plug.setAttribute(s_mode);
            plug.setValue(0);
         }
      }
      else
      {
         float3 m_value;
         m_value[0] = (float)bbMin.x;
         m_value[1] = (float)bbMin.y;
         m_value[2] = (float)bbMin.z;
         plug.setAttribute(s_boundingBoxMin);
         SetPointPlugValue(plug, m_value);
         fGeometry.BBmin = MPoint(m_value[0], m_value[1], m_value[2]);

         m_value[0] = (float)bbMax.x;
         m_value[1] = (float)bbMax.y;
         m_value[2] = (float)bbMax.z;
         plug.setAttribute(s_boundingBoxMax);
         SetPointPlugValue(plug, m_value);
         fGeometry.BBmax = MPoint(m_value[0], m_value[1], m_value[2]);
      }
      fGeometry.updateView = true;
   }

   return &fGeometry;
}

void CArnoldStandInShape::UpdateSelectedItems()
{
   MPlug selPlug(thisMObject(), s_selectedItems);
   MString selectedItems;
   selPlug.getValue(selectedItems);
   MStringArray selectedItemsList;
   
   if (selectedItems.length() > 0)
      selectedItems.split(',', selectedItemsList);
   
   CArnoldStandInShape* nonConstThis = const_cast<CArnoldStandInShape*> (this);
   CArnoldStandInGeom* geom = nonConstThis->geometry();
   for (CArnoldStandInGeom::geometryListIterType it = geom->m_geometryList.begin(); it != geom->m_geometryList.end(); ++it)
   {
      CArnoldStandInGeometry* g = it->second;
      MString nodeName(it->first.c_str());

      bool selected = false;

      if (g)
      {
         for (unsigned int i = 0; i < selectedItemsList.length(); ++i)
         {
            if (selectedItemsList[i] == nodeName)
            {
               selected = true;
               break;
            }
         }
         g->SetSelected(selected);
      }
      
   }
}
// FIXME : please remove all these hacks regarding the "override" attributes 
// once we no longer case about pre-2.0.2 compatibility
void CArnoldStandInShape::AttrChangedCallback(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void* clientData)
{

   CArnoldStandInShape *node = static_cast<CArnoldStandInShape*>(clientData);
   if (!node)
      return; 

   MFnAttribute fnAttr(plug.attribute());
   MString attrName = fnAttr.name();

   if (plug == s_selectedItems)
   {
      node->UpdateSelectedItems();
   }


   static MString overridePrefix("override");
   if (overridePrefix != attrName.substringW(0, 7))
      return;

   MObject this_object = node->thisMObject();
   MStatus status;
   MFnDependencyNode dNode(this_object, &status);
   if (!status)
      return;

#define STANDIN_VISIBILITY_ATTR_COUNT 7
   static const char* overrideVisibilityAttributes[STANDIN_VISIBILITY_ATTR_COUNT] = { "overridePrimaryVisibility", 
                                                "overrideVisibleInDiffuseReflection", 
                                                "overrideVisibleInSpecularReflection",
                                                "overrideVisibleInDiffuseTransmission",
                                                "overrideVisibleInSpecularTransmission",
                                                "overrideVisibleInVolume",
                                                "overrideCastsShadows"};

   static const MStringArray OverrideVisibilityAttributesList(overrideVisibilityAttributes, STANDIN_VISIBILITY_ATTR_COUNT);
   
   static const char* visibilityAttributes[STANDIN_VISIBILITY_ATTR_COUNT] = { "primaryVisibility", 
                                                "aiVisibleInDiffuseReflection", 
                                                "aiVisibleInSpecularReflection",
                                                "aiVisibleInDiffuseTransmission",
                                                "aiVisibleInSpecularTransmission",
                                                "aiVisibleInVolume",
                                                "castsShadows"};

   static const MStringArray VisibilityAttributesList(visibilityAttributes, STANDIN_VISIBILITY_ATTR_COUNT);

   for (int i = 0; i < STANDIN_VISIBILITY_ATTR_COUNT; ++i)
   {
      if (attrName != OverrideVisibilityAttributesList[i])
         continue;
      if (!plug.asBool())
      {
         // this will surely invoke this attrChanged callback again, but since the value 
         // will be OFF there should be any problem
         plug.setValue(true);

         // need to set the other attribute to true, meaning that nothing will be overridden in the standin
         MPlug basePlug = dNode.findPlug(VisibilityAttributesList[i], true, &status);
         if (status)
            basePlug.setValue(true);
      }
      return;
   }
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
   // Are we displaying meshes?
   if (!info.objectDisplayStatus(M3dView::kDisplayMeshes))
      return;

   // Do we enable display of standins?
   int drawOverride = 0;
   MStatus status;
   MFnDependencyNode dNode(info.multiPath().node(), &status);
   if (status)
   {
      MPlug plug = dNode.findPlug("standInDrawOverride", true, &status);
      if (!plug.isNull() && status)
      {
         const int localDrawOverride = plug.asShort();
         if (localDrawOverride == 0) // use global settings
         {
            MObject ArnoldRenderOptionsNode = CArnoldOptionsNode::getOptionsNode();
            if (!ArnoldRenderOptionsNode.isNull())
               drawOverride = MFnDependencyNode(ArnoldRenderOptionsNode).findPlug("standin_draw_override", true).asShort();
         }
         else
            drawOverride = localDrawOverride - 1;
      }
   }

   if (drawOverride == 2) // draw is disabled
      return;

   // The draw data is used to pass geometry through the
   // draw queue. The data should hold all the information
   // needed to draw the shape.
   //
   MDrawData data;
   MDrawRequest request = info.getPrototype(*this);
   CArnoldStandInShape* shapeNode = reinterpret_cast<CArnoldStandInShape*>(surfaceShape());
   CArnoldStandInGeom* geom = shapeNode->geometry();
   geom->drawOverride = drawOverride;
   getDrawData(geom, data);
   request.setDrawData(data);

   // Use mode status to determine how to display object
   // why was there a switch if everything executed the same code??
   getDrawRequestsWireFrame(request, info);
   queue.add(request);
}

void CArnoldStandInShapeUI::draw(const MDrawRequest & request, M3dView & view) const
{
   MDrawData data = request.drawData();
   CArnoldStandInGeom * geom = (CArnoldStandInGeom*) data.geometry();
   view.beginGL();
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

   if (geom->updateView || geom->updateBBox)
   {
      if (geom->dList == 0)
         geom->dList = glGenLists(2);

      // Only show scaled BBox in this case
      /*
      if(geom->deferStandinLoad)
      {
         float minPt[4];
         float maxPt[4];
         geom->BBmin.get(minPt);
         geom->BBmax.get(maxPt);
         
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
               
         glNewList(geom->dList+1, GL_COMPILE);
         glBegin(GL_LINE_STRIP);
         glVertex3fv(sbottomLeftFront);
         glVertex3fv(sbottomLeftBack);
         glVertex3fv(stopLeftBack);
         glVertex3fv(stopLeftFront);
         glVertex3fv(sbottomLeftFront);
         glVertex3fv(sbottomRightFront);
         glVertex3fv(sbottomRightBack);
         glVertex3fv(stopRightBack);
         glVertex3fv(stopRightFront);
         glVertex3fv(sbottomRightFront);
         glEnd();
         
         glBegin(GL_LINES);
         glVertex3fv(sbottomLeftBack);
         glVertex3fv(sbottomRightBack);

         glVertex3fv(stopLeftBack);
         glVertex3fv(stopRightBack);

         glVertex3fv(stopLeftFront);
         glVertex3fv(stopRightFront);
         glEnd();
         glEndList();
      }*/
      geom->updateBBox = false;
   }
   
   if (geom->updateView)
   {
      float minPt[4];
      float maxPt[4];
      geom->BBmin.get(minPt);
      geom->BBmax.get(maxPt);
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

      glMatrixMode(GL_MODELVIEW);

      switch (geom->mode)
      {
      case DM_BOUNDING_BOX:
         glNewList(geom->dList, GL_COMPILE);
         glBegin(GL_LINE_STRIP);

         glVertex3fv(bottomLeftFront);
         glVertex3fv(bottomLeftBack);
         glVertex3fv(topLeftBack);
         glVertex3fv(topLeftFront);
         glVertex3fv(bottomLeftFront);
         glVertex3fv(bottomRightFront);
         glVertex3fv(bottomRightBack);
         glVertex3fv(topRightBack);
         glVertex3fv(topRightFront);
         glVertex3fv(bottomRightFront);
         glEnd();

         glBegin(GL_LINES);
         glVertex3fv(bottomLeftBack);
         glVertex3fv(bottomRightBack);

         glVertex3fv(topLeftBack);
         glVertex3fv(topRightBack);

         glVertex3fv(topLeftFront);
         glVertex3fv(topRightFront);
         glEnd();
         glEndList();
         break;
      case DM_PER_OBJECT_BOUNDING_BOX:
         glNewList(geom->dList, GL_COMPILE);
         geom->Draw(GM_BOUNDING_BOX);
         glEndList();
         break;
      case DM_POLYWIRE: // filled polygon
         glNewList(geom->dList, GL_COMPILE);
         glPushAttrib(GL_CURRENT_BIT);
         glEnable(GL_POLYGON_OFFSET_FILL);         
         glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
         geom->Draw(GM_POLYGONS);         
         glPopAttrib();
         geom->Draw(GM_WIREFRAME);         
         glEndList();
         break;

      case DM_WIREFRAME: // wireframe
         glNewList(geom->dList, GL_COMPILE);
         geom->Draw(GM_WIREFRAME);
         glEndList();         
         break;

      case DM_POINT_CLOUD: // points
         glPushAttrib(GL_CURRENT_BIT);
         glEnable(GL_POINT_SMOOTH);
         // Make round points, not square points and not working
         glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
         glEnable(GL_BLEND);
         glDepthMask(GL_TRUE);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glNewList(geom->dList, GL_COMPILE);
         geom->Draw(GM_POINTS);
         glEndList();
         
         glDisable(GL_POINT_SMOOTH);
         glPopAttrib();
         break;
      case DM_SHADED_POLYWIRE: // shaded polywire
         {
            glNewList(geom->dList, GL_COMPILE);
            const bool enableLighting = view.displayStyle() == M3dView::kGouraudShaded;
            if (enableLighting)
            {
               glEnable(GL_LIGHTING);
               glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
               glEnable(GL_COLOR_MATERIAL);
            }
            glPushAttrib(GL_CURRENT_BIT);
            glEnable(GL_POLYGON_OFFSET_FILL);                  
            glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
            geom->Draw(GM_NORMAL_AND_POLYGONS);
            glPopAttrib();
            if (enableLighting)
            {
               glDisable(GL_LIGHTING);
               glDisable(GL_COLOR_MATERIAL);
            }
            geom->Draw(GM_WIREFRAME);
            glEndList();
         }
         break;
      case DM_SHADED: // shaded
         {
            glNewList(geom->dList, GL_COMPILE);
            const bool enableLighting = view.displayStyle() == M3dView::kGouraudShaded;
            if (enableLighting)
            {
               glEnable(GL_LIGHTING);
               glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
               glEnable(GL_COLOR_MATERIAL);
            }
            glPushAttrib(GL_ALL_ATTRIB_BITS);         
            glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
            geom->Draw(GM_NORMAL_AND_POLYGONS);
            glPopAttrib();
            if (enableLighting)
            {
               glDisable(GL_LIGHTING);
               glDisable(GL_COLOR_MATERIAL);
            }
            glEndList();
         }
         break;
      }
      geom->Clear();
      geom->updateView = false;
   }

   if (geom->drawOverride == 1 || geom->drawOverride == 3 )
   {
      MBoundingBox m_bbox = geom->bbox;
      float minPt[4];
      float maxPt[4];
      geom->BBmin.get(minPt);
      geom->BBmax.get(maxPt);
      const float bottomLeftFront[3] =
      { minPt[0], minPt[1], minPt[2] };
      const float topLeftFront[3] =
      { minPt[0], maxPt[1], minPt[2] };
      const float bottomRightFront[3] =
      { maxPt[0], minPt[1], minPt[2] };
      const float topRightFront[3] =
      { maxPt[0], maxPt[1], minPt[2] };
      const float bottomLeftBack[3] =
      { minPt[0], minPt[1], maxPt[2] };
      const float topLeftBack[3] =
      { minPt[0], maxPt[1], maxPt[2] };
      const float bottomRightBack[3] =
      { maxPt[0], minPt[1], maxPt[2] };
      const float topRightBack[3] =
      { maxPt[0], maxPt[1], maxPt[2] };

      glBegin(GL_LINE_STRIP);
      glVertex3fv(bottomLeftFront);
      glVertex3fv(bottomLeftBack);
      glVertex3fv(topLeftBack);
      glVertex3fv(topLeftFront);
      glVertex3fv(bottomLeftFront);
      glVertex3fv(bottomRightFront);
      glVertex3fv(bottomRightBack);
      glVertex3fv(topRightBack);
      glVertex3fv(topRightFront);
      glVertex3fv(bottomRightFront);
      glEnd();

      glBegin(GL_LINES);
      glVertex3fv(bottomLeftBack);
      glVertex3fv(bottomRightBack);

      glVertex3fv(topLeftBack);
      glVertex3fv(topRightBack);

      glVertex3fv(topLeftFront);
      glVertex3fv(topRightFront);
      glEnd();
   }
   else if (geom->dList != 0)
   {      
      glCallList(geom->dList);
      // Draw scaled BBox
      //if(geom->deferStandinLoad)
      //   glCallList(geom->dList+1);
   }
   glPopAttrib();
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
{
//
// Select function. Gets called when the bbox for the object is selected.
// This function just selects the object without doing any intersection tests.
//
// Arguments:
//
//     selectInfo           - the selection state information
//     selectionList        - the list of selected items to add to
//     worldSpaceSelectPts  -
//
   MSelectionMask priorityMask(MSelectionMask::kSelectMeshes);
   MSelectionList item;
   item.add(selectInfo.selectPath());
   MPoint xformedPt;

   selectInfo.addSelection(item, xformedPt, selectionList, worldSpaceSelectPts, priorityMask, false);
   return true;

}

