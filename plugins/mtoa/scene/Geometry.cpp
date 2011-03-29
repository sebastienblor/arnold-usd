
#include "Geometry.h"
#include "render/RenderSession.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

#include <maya/MDagPath.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MIntArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MItMeshVertex.h>
#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>

#include <vector>

namespace
{
   void SetKeyData(AtArray* arr, AtUInt step, const std::vector<float>& data, AtUInt size)
   {
      AtUInt index = 0;

      switch(arr->type)
      {
         case AI_TYPE_POINT:
         {
            AtPoint pnt;
            for(AtUInt J = 0; (J < size); ++J)
            {
               AiV3Create(pnt, data[index++], data[index++], data[index++]);
               AiArraySetPnt(arr, J + (size * step), pnt);
            }
         }
         break;

         case AI_TYPE_VECTOR:
         {
            AtVector vec;
            for(AtUInt J = 0; (J < size); ++J)
            {
               AiV3Create(vec, data[index++], data[index++], data[index++]);
               AiArraySetVec(arr, J + (size * step), vec);
            }
         }
      }
   }
}

MObject CGeoTranslator::GetNodeShadingGroup(MObject dagNode, int instanceNum)
{
   MPlugArray        connections;
   MFnDependencyNode fnDGNode(dagNode);

   MPlug plug(dagNode, fnDGNode.attribute("instObjGroups"));

   plug.elementByLogicalIndex(instanceNum).connectedTo(connections, false, true);

   for (unsigned int k=0; k<connections.length(); ++k)
   {
      MObject shadingGroup(connections[k].node());
      if (shadingGroup.apiType() == MFn::kShadingEngine )
      {
         return shadingGroup;
      }
   }
   return MObject::kNullObj;
}

MObject CGeoTranslator::GetNodeShader(MObject dagNode, int instanceNum)
{
   MPlugArray        connections;
   MObject shadingGroup = GetNodeShadingGroup(dagNode, instanceNum);
   MFnDependencyNode fnDGNode(shadingGroup);
   MPlug shaderPlug = fnDGNode.findPlug("surfaceShader");
   shaderPlug.connectedTo(connections, true, false);

   return connections[0].node();
}

bool CGeoTranslator::GetVerticesWorld(MObject &dagNode, MFnMesh &fnMesh, std::vector<float> &vertices)
{
   if (fnMesh.numVertices() > 0)
   {
      vertices.resize(fnMesh.numVertices() * 3);

      // A mesh has no transform, we must trace it back in the DAG
      MDagPath dp;
      MDagPath::getAPathTo(dagNode, dp);
      MFnMesh pointFn(dp);
      MFloatPointArray pointsArray;
      pointFn.getPoints(pointsArray, MSpace::kWorld);
      for (int J = 0; ( J < fnMesh.numVertices() ); ++J)
      {
         vertices[J * 3 + 0] = pointsArray[J].x;
         vertices[J * 3 + 1] = pointsArray[J].y;
         vertices[J * 3 + 2] = pointsArray[J].z;
      }
      return true;
   }
   return false;
}

bool CGeoTranslator::GetVertices(MFnMesh &fnMesh, std::vector<float> &vertices)
{
   int nverts = fnMesh.numVertices();
   if (nverts > 0)
   {
      vertices.resize(nverts * 3);

      MFloatPointArray pointsArray;
      fnMesh.getPoints(pointsArray, MSpace::kObject);

      for (int J = 0; ( J < nverts ); ++J)
      {
         vertices[J * 3 + 0] = pointsArray[J].x;
         vertices[J * 3 + 1] = pointsArray[J].y;
         vertices[J * 3 + 2] = pointsArray[J].z;
      }
      return true;
   }
   return false;
}

bool CGeoTranslator::GetNormals(MFnMesh &fnMesh, std::vector<float> &normals)
{
   int nnorms = fnMesh.numNormals();
   if (m_fnNode.findPlug("smoothShading").asBool() &&
         !m_fnNode.findPlug("subdiv_type").asBool() &&
         nnorms > 0)
   {
      normals.resize(nnorms * 3);

      MFloatVectorArray normalArray;
      fnMesh.getNormals(normalArray, MSpace::kObject);

      for (int J = 0; (J < nnorms); ++J)
      {
         normals[J * 3 + 0] = normalArray[J].x;
         normals[J * 3 + 1] = normalArray[J].y;
         normals[J * 3 + 2] = normalArray[J].z;
      }
      return true;
   }
   return false;
}

bool CGeoTranslator::GetTangents(MFnMesh &fnMesh, std::vector<float> &tangents, std::vector<float> &bitangents)
{
   MStatus stat;
   MPlug pExportTangents = fnMesh.findPlug("export_tangents", false, &stat);

   if (stat != MStatus::kSuccess)
      return false;

   if (pExportTangents.asBool())
   {
      MObject meshObject = fnMesh.object();
      MItMeshVertex itVertex(meshObject);
      MIntArray iarray;
      MVector ttmp, btmp, tangent, bitangent;
      float scale = 1.0f;
      unsigned int i = 0;

      int nverts = fnMesh.numVertices();

      tangents.resize(nverts * 3);
      bitangents.resize(nverts * 3);

      while (!itVertex.isDone())
      {
         iarray.clear();
         itVertex.getConnectedFaces(iarray);

         i = itVertex.index() * 3;

         tangent = MVector::zero;
         bitangent = MVector::zero;

         if (iarray.length() > 0)
         {
            scale = 1.0f / float(iarray.length());

            for (unsigned int j=0; j<iarray.length(); ++j)
            {
               fnMesh.getFaceVertexTangent(iarray[j], itVertex.index(), ttmp, MSpace::kObject);
               fnMesh.getFaceVertexBinormal(iarray[j], itVertex.index(), btmp, MSpace::kObject);
               tangent += ttmp;
               bitangent += btmp;
            }

            tangent *= scale;
            bitangent *= scale;
         }

         tangents[i] = (float) tangent.x;
         tangents[i+1] = (float) tangent.y;
         tangents[i+2] = (float) tangent.z;

         bitangents[i] = (float) bitangent.x;
         bitangents[i+1] = (float) bitangent.y;
         bitangents[i+2] = (float) bitangent.z;

         itVertex.next();
      }
      return true;
   }
   return false;
}

bool CGeoTranslator::GetRefObj(MFnMesh &fnMesh, std::vector<float> &refVertices, std::vector<float> &refNormals)
{
   MStatus stat;

   //Check if there is a referenceObject plug
   MPlug pReferenceObject = fnMesh.findPlug("referenceObject", false, &stat);

   if (stat != MStatus::kSuccess)
      return false;

   MPlugArray connectedPlugs;
   MObject referenceObject;

   //Check if anything is connected to .referenceObject plug
   pReferenceObject.connectedTo(connectedPlugs, true, true, &stat);
   if (stat != MStatus::kSuccess || 1 != connectedPlugs.length() ) {
      return false;
   }
   else
   {
      //checking if a script is not going rogue : the object must be a mesh !
      referenceObject = connectedPlugs[0].node();
      if (referenceObject.hasFn(MFn::kMesh) != 1)	return false;
   }

   MFnMesh referenceObjectMesh(referenceObject);

   //Check if the numbers of vertices is the same as the current object
   if (referenceObjectMesh.numVertices() != m_fnMesh.numVertices())
   {
   return false;
   }

   //Get vertices of the reference object in world space
   GetVerticesWorld(referenceObject ,referenceObjectMesh, refVertices);

   //Get normals of the reference object
   GetNormals(referenceObjectMesh, refNormals);

   return true;
}

bool CGeoTranslator::GetUVs(MFnMesh &fnMesh, std::vector<float> &uvs)
{
   // Get all UVs
   if (fnMesh.numUVs() > 0)
   {
      uvs.resize(fnMesh.numUVs() * 2);

      MFloatArray uArray, vArray;
      fnMesh.getUVs(uArray, vArray);

      for (int J = 0; (J < fnMesh.numUVs()); ++J)
      {
         uvs[J * 2 + 0] = uArray[J];
         uvs[J * 2 + 1] = vArray[J];
      }
      return true;
   }
   return false;
}

bool CGeoTranslator::GetVertexColors(MFnMesh &fnMesh, std::map<std::string, std::vector<float> > &vcolors)
{
   bool exportColors = false;

   if (fnMesh.numColorSets() > 0)
   {
      MPlug plug = fnMesh.findPlug("export_colors");
      if (!plug.isNull())
      {
         exportColors = plug.asBool();
      }
   }
   if (exportColors)
   {
      MStringArray names;
      MIntArray faces;
      unsigned int i = 0;
      float scale = 1.0f;
      int dim = 4;
      MColor col;

      fnMesh.getColorSetNames(names);
      for (unsigned int j=0; j<names.length(); ++j)
      {
         std::vector<float> &colors = vcolors[names[j].asChar()];
         colors.resize(fnMesh.numVertices() * dim, 0.0f);

         MObject meshObject = fnMesh.object();
         MItMeshVertex itVertex(meshObject);

         while (!itVertex.isDone())
         {
            faces.clear();
            itVertex.getConnectedFaces(faces);

            if (faces.length() > 0)
            {
               scale = 1.0f / float(faces.length());
               i = itVertex.index() * dim;

               for (unsigned int k=0; k<faces.length(); ++k)
               {
                  itVertex.getColor(col, faces[k], &names[j]);
                  for (int l=0; l<dim; ++l)
                  {
                     colors[i+l] += col[l];
                  }
               }
               for (int l=0; l<dim; ++l)
               {
                  colors[i+l] *= scale;
               }
            }

            itVertex.next();
         }
      }
   }
   return exportColors;
}

void CGeoTranslator::GetComponentIDs(MFnMesh &fnMesh,
      std::vector<AtUInt> &nsides,
      std::vector<AtLong> &vidxs,
      std::vector<AtLong> &nidxs,
      std::vector<AtLong> &uvidxs,
      bool exportNormals,
      bool exportUVs
      )
{
   int uv_id;
   // Traverse all polygons to export vidxs, uvindxs and nsides
   nsides.reserve(fnMesh.numPolygons());
   for (int p(0); p < fnMesh.numPolygons(); ++p)
   {
      // Num points/sides to the poly.
      nsides.push_back( fnMesh.polygonVertexCount(p) );
      // Vertex indicies.
      MIntArray p_vidxs;
      fnMesh.getPolygonVertices( p, p_vidxs);
      for( uint v(0); v < p_vidxs.length(); ++v)
      {
         vidxs.push_back(p_vidxs[v]);
         // UVs
         if (exportUVs)
         {
            m_fnMesh.getPolygonUVid(p, v, uv_id);
            uvidxs.push_back(uv_id);
         }
      }
   }

   // Normals.
   if (exportNormals)
   {
      MIntArray vertex_counts, normal_ids;
      m_fnMesh.getNormalIds(vertex_counts, normal_ids);
      for( uint n(0); n < normal_ids.length(); ++n ) nidxs.push_back( normal_ids[n] );
   }
}

void CGeoTranslator::ExportShaders()
{
   ExportMeshShaders(m_atNode, m_fnMesh);
}

void CGeoTranslator::ExportMeshShaders(AtNode* polymesh, MFnMesh &fnMesh)
{
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   std::vector<AtNode*> meshShaders;

   MObject shadingGroup = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroup.isNull())
   {
      MPlugArray        connections;
      MFnDependencyNode fnDGNode(shadingGroup);
      MPlug shaderPlug = fnDGNode.findPlug("surfaceShader");
      shaderPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         // shader assigned to node
         AtNode* shader = m_scene->ExportShader(connections[0].node());

         AiNodeSetPtr(polymesh, "shader", shader);
         meshShaders.push_back(shader);
      }
      else
         AiMsgWarning("[mtoa] shadingGroup %s has no surfaceShader input.", fnDGNode.name().asChar());
   }
   else
   {
      MIntArray indices;
      // per-face assignment
      MObjectArray shadingGroups;

      // indices are used later when exporting shidxs
      fnMesh.getConnectedShaders(instanceNum, shadingGroups, indices);

      for (int J = 0; (J < (int) shadingGroups.length()); J++)
      {
         MPlugArray        connections;
         MFnDependencyNode fnDGNode(shadingGroups[J]);
         MPlug             shaderPlug(shadingGroups[J], fnDGNode.attribute("surfaceShader"));

         shaderPlug.connectedTo(connections, true, false);
         // FIXME: there should be a check if connections.length() > 0
         // this is not a simple fix because it will shift all the indices,
         // but as it is now, it will crash if nothing is connected to "surfaceShader"
         meshShaders.push_back(m_scene->ExportShader(connections[0].node()));
      }

      AiNodeSetArray(polymesh, "shader", AiArrayConvert((AtInt)meshShaders.size(), 1, AI_TYPE_NODE, &meshShaders[0], TRUE));

      // export face to shader indices
      // first convert from MIntArray to AtUInt vector
      std::vector<AtUInt> shidxs;
      for(AtUInt i = 0; i < indices.length(); i++)
         shidxs.push_back(indices[i]);
      AiNodeSetArray(polymesh, "shidxs", AiArrayConvert((AtInt)shidxs.size(), 1, AI_TYPE_UINT, &(shidxs[0]), TRUE));
   }

   //
   // DISPLACEMENT
   //
   // currently does not work for per-face assignment
   if (!shadingGroup.isNull())
   {
      MPlugArray        connections;
      MFnDependencyNode fnDGNode(shadingGroup);
      MPlug shaderPlug = fnDGNode.findPlug("displacementShader");
      shaderPlug.connectedTo(connections, true, false);

      // are there any connections to displacementShader?
      if (connections.length() > 0)
      {
         m_displaced = true;
         MFnDependencyNode dispNode(connections[0].node());

         // Note that disp_height has no actual influence on the scale of the displacement if it is vector based
         // it only influences the computation of the displacement bounds
         AiNodeSetFlt(polymesh, "disp_height", dispNode.findPlug("disp_height").asFloat());
         AiNodeSetFlt(polymesh, "disp_zero_value", dispNode.findPlug("disp_zero_value").asFloat());
         AiNodeSetBool(polymesh, "autobump", dispNode.findPlug("autobump").asBool());

         connections.clear();
         dispNode.findPlug("disp_map").connectedTo(connections, true, false);

         if (connections.length() > 0)
         {
            MString attrName = connections[0].partialName(false, false, false, false, false, true);
            AtNode* dispImage(m_scene->ExportShader(connections[0].node(), attrName));

            MPlug pVectorDisp = dispNode.findPlug("vector_displacement", false);
            if (!pVectorDisp.isNull() && pVectorDisp.asBool())
            {
               AtNode* tangentToObject = AiNode("TangentToObjectSpace");
               m_scene->ProcessShaderParameter(dispNode, "vector_displacement_scale", tangentToObject, "scale", AI_TYPE_VECTOR);
               AiNodeLink(dispImage, "map", tangentToObject);

               AiNodeSetPtr(polymesh, "disp_map", tangentToObject);
            }
            else
            {
               AiNodeSetPtr(polymesh, "disp_map", dispImage);
            }
         }
      }
   }
}

void CGeoTranslator::ExportMeshGeoData(AtNode* polymesh, AtUInt step)
{
   //
   // GEOMETRY
   //
   std::vector<float> vertices, normals, tangents, bitangents;

   // Get all vertices
   GetVertices(m_fnMesh, vertices);

   // Get all normals
   bool exportNormals = GetNormals(m_fnMesh, normals);

   // Get all tangents, bitangents
   bool exportTangents = GetTangents(m_fnMesh, tangents, bitangents);

   if (step == 0)
   {
      std::vector<float> uvs;
      std::vector<AtUInt> nsides;
      std::vector<AtLong> vidxs, nidxs, uvidxs;
      std::map<std::string, std::vector<float> > vcolors;
      std::vector<float> refVertices, refNormals;

      // Get UVs
      bool exportUVs = GetUVs(m_fnMesh, uvs);

      //Get reference objects
      bool exportReferenceObjects = GetRefObj(m_fnMesh, refVertices, refNormals);

      // Get Component IDs
      GetComponentIDs(m_fnMesh, nsides, vidxs, nidxs, uvidxs, exportNormals, exportUVs);
      // Get Vertex Colors
      bool exportColors = GetVertexColors(m_fnMesh, vcolors);

      // Declare user parameters for tangents
      if (exportTangents)
      {
         AiNodeDeclare(polymesh, "tangent", "varying VECTOR");
         AiNodeDeclare(polymesh, "bitangent", "varying VECTOR");
      }

      if (exportReferenceObjects)
      {
          AiNodeDeclare(polymesh, "Pref", "varying POINT");
          AiNodeDeclare(polymesh, "Nref", "varying VECTOR");
      }

      // Declare user parameters for color sets
      if (exportColors)
      {
         std::map<std::string, std::vector<float> >::iterator it = vcolors.begin();
         while (it != vcolors.end())
         {
            AiNodeDeclare(polymesh, it->first.c_str(), "varying RGBA");
            ++it;
         }
      }

      if (!m_motionDeform)
      {
         // No deformation motion blur, so we create normal arrays
         AiNodeSetArray(polymesh, "vlist", AiArrayConvert(m_fnMesh.numVertices() * 3, 1, AI_TYPE_FLOAT, &(vertices[0]), TRUE));

         if (exportNormals && (m_fnMesh.numNormals() > 0))
            AiNodeSetArray(polymesh, "nlist", AiArrayConvert(m_fnMesh.numNormals() * 3, 1, AI_TYPE_FLOAT, &(normals[0]), TRUE));

         if (exportTangents)
         {
            AiNodeSetArray(polymesh, "tangent", AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_VECTOR, &(tangents[0]), TRUE));
            AiNodeSetArray(polymesh, "bitangent", AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_VECTOR, &(bitangents[0]), TRUE));
         }
      }
      else
      {
         // Deformation motion blur. We need to create keyable arrays for vlist and nlist
         AtArray* vlist_array = AiArrayAllocate(m_fnMesh.numVertices(), m_scene->GetNumMotionSteps(), AI_TYPE_POINT);
         SetKeyData(vlist_array, step, vertices, m_fnMesh.numVertices());
         AiNodeSetArray(polymesh, "vlist", vlist_array);

         if (exportNormals)
         {
            AtArray* nlist_array = AiArrayAllocate(m_fnMesh.numNormals(), m_scene->GetNumMotionSteps(), AI_TYPE_VECTOR);
            SetKeyData(nlist_array, step, normals, m_fnMesh.numNormals());
            AiNodeSetArray(polymesh, "nlist", nlist_array);
         }

         if (exportTangents)
         {
            AtArray* tangent_array = AiArrayAllocate(m_fnMesh.numVertices(), m_scene->GetNumMotionSteps(), AI_TYPE_VECTOR);
            SetKeyData(tangent_array, step, tangents, m_fnMesh.numVertices());
            AiNodeSetArray(polymesh, "tangent", tangent_array);

            AtArray* bitangent_array = AiArrayAllocate(m_fnMesh.numVertices(), m_scene->GetNumMotionSteps(), AI_TYPE_VECTOR);
            SetKeyData(bitangent_array, step, bitangents, m_fnMesh.numVertices());
            AiNodeSetArray(polymesh, "bitangent", bitangent_array);
         }
      }

      AiNodeSetArray(polymesh, "nsides", AiArrayConvert((AtInt)nsides.size(), 1, AI_TYPE_UINT, &(nsides[0]), TRUE));

      // Passing vidxs directly put Arnold in trouble
      //AiNodeSetArray(polymesh, "vidxs", AiArrayConvert(vidxs.size(), 1, AI_TYPE_UINT, &(vidxs[0]), TRUE));
      AtArray *vidxsTmp = AiArrayAllocate((AtInt)vidxs.size(), 1, AI_TYPE_UINT);
      for(AtUInt i = 0; (i < vidxs.size()); i++)
         AiArraySetUInt(vidxsTmp, i, vidxs[i]);
      AiNodeSetArray(polymesh, "vidxs", vidxsTmp);

      if (exportNormals)
      {
         // Same goes here
         //AiNodeSetArray(polymesh, "nidxs", AiArrayConvert(nidxs.size(), 1, AI_TYPE_UINT, &(nidxs[0]), TRUE));
         AtArray *nidxsTmp = AiArrayAllocate((AtInt)nidxs.size(), 1, AI_TYPE_UINT);
         for(AtUInt i = 0; (i < nidxs.size()); i++)
            AiArraySetUInt(nidxsTmp, i, nidxs[i]);
         AiNodeSetArray(polymesh, "nidxs", nidxsTmp);
      }

      if (exportReferenceObjects)
      {
           AiNodeSetArray(polymesh, "Pref", AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_POINT, &(refVertices[0]), TRUE));
         AiNodeSetArray(polymesh, "Nref", AiArrayConvert(m_fnMesh.numNormals(), 1, AI_TYPE_VECTOR, &(refNormals[0]), TRUE));

      }

      if (exportUVs)
      {
         AiNodeSetArray(polymesh, "uvlist", AiArrayConvert(m_fnMesh.numUVs() * 2, 1, AI_TYPE_FLOAT, &(uvs[0]), TRUE));
         // Same problem here
         //AiNodeSetArray(polymesh, "uvidxs", AiArrayConvert(uvidxs.size(), 1, AI_TYPE_UINT, &(uvidxs[0]), TRUE));
         AtArray *uvidxsTmp = AiArrayAllocate((AtInt)uvidxs.size(), 1, AI_TYPE_UINT);
         for(AtUInt i = 0; (i < uvidxs.size()); i++)
            AiArraySetUInt(uvidxsTmp, i, uvidxs[i]);
         AiNodeSetArray(polymesh, "uvidxs", uvidxsTmp);
      }

      if (exportColors)
      {
         std::map<std::string, std::vector<float> >::iterator it = vcolors.begin();
         while (it != vcolors.end())
         {
            AiNodeSetArray(polymesh, it->first.c_str(), AiArrayConvert(m_fnMesh.numVertices(), 1, AI_TYPE_RGBA, &(it->second[0]), TRUE));
            ++it;
         }
      }
   } // step == 0
   else
   {
      // Export motion blur keys information (for deformation)

      // vertices
      AtArray* vlist_array = AiNodeGetArray(polymesh, "vlist");
      SetKeyData(vlist_array, step, vertices, m_fnMesh.numVertices());

      // normals
      if (exportNormals)
      {
         AtArray* nlist_array = AiNodeGetArray(polymesh, "nlist");
         SetKeyData(nlist_array, step, normals, m_fnMesh.numNormals());
      }

      // tangents
      if (exportTangents)
      {
         AtArray* tangent_array = AiNodeGetArray(polymesh, "tangent");
         SetKeyData(tangent_array, step, tangents, m_fnMesh.numVertices());

         AtArray* bitangent_array = AiNodeGetArray(polymesh, "bitangent");
         SetKeyData(bitangent_array, step, bitangents, m_fnMesh.numVertices());
      }
   }
}

void CGeoTranslator::IsGeoDeforming()
{
    bool history = false;
    bool pnts = false;

    MPlug inMeshPlug = m_fnMesh.findPlug("inMesh");
    MPlugArray conn;
    inMeshPlug.connectedTo(conn, true, false);
    if(conn.length())
    {
        history = true;
    }
 
    inMeshPlug = m_fnMesh.findPlug("pnts");
    inMeshPlug.connectedTo(conn, true, false);
    if(conn.length())
    {
        pnts = true;
    }

    if(!history && !pnts && !m_displaced)
        m_motionDeform = false;
}

void CGeoTranslator::ExportMeshParameters(AtNode* polymesh)
{
   // Check if custom attributes have been created, ignore them otherwise
   MStatus status;
   m_fnNode.findPlug("subdiv_type", &status);
   bool customAttributes = (status == MS::kSuccess);

   AiNodeSetBool(polymesh, "smoothing", m_fnNode.findPlug("smoothShading").asBool());
   AiNodeSetBool(polymesh, "receive_shadows", m_fnNode.findPlug("receiveShadows").asBool());

   if (m_fnNode.findPlug("doubleSided").asBool())
      AiNodeSetInt(polymesh, "sidedness", 65535);
   else
   {
      AiNodeSetBool(polymesh, "invert_normals", m_fnNode.findPlug("opposite").asBool());
      AiNodeSetInt(polymesh, "sidedness", 0);
   }

   // Visibility options
   AtInt visibility = ComputeVisibility(true);
   AiNodeSetInt(polymesh, "visibility", visibility);

   if (customAttributes)
   {
      AiNodeSetBool(polymesh, "self_shadows", m_fnNode.findPlug("self_shadows").asBool());
      AiNodeSetBool(polymesh, "opaque", m_fnNode.findPlug("opaque").asBool());

      // Subdivision surfaces
      //
      bool subdivision = (m_fnNode.findPlug("subdiv_type").asInt() != 0);

      if (subdivision)
      {
         AiNodeSetStr(polymesh, "subdiv_type", "catclark");
         AiNodeSetInt(polymesh, "subdiv_iterations", m_fnNode.findPlug("subdiv_iterations").asInt());
         AiNodeSetInt(polymesh, "subdiv_adaptive_metric", m_fnNode.findPlug("subdiv_adaptive_metric").asInt());
         AiNodeSetFlt(polymesh, "subdiv_pixel_error", m_fnNode.findPlug("subdiv_pixel_error").asFloat());

         MString cameraName = m_fnNode.findPlug("subdiv_dicing_camera").asString();
         AtNode* camera = ((cameraName != "") && (cameraName != "Default")) ? AiNodeLookUpByName(cameraName.asChar()) : NULL;
         AiNodeSetPtr(polymesh, "subdiv_dicing_camera", camera);

         AiNodeSetInt(polymesh, "subdiv_uv_smoothing", m_fnNode.findPlug("subdiv_uv_smoothing").asInt());
      }

      // Subsurface Scattering
      //
      AiNodeSetInt(polymesh, "sss_max_samples", m_fnNode.findPlug("sss_max_samples").asInt());
      AiNodeSetFlt(polymesh, "sss_sample_spacing", m_fnNode.findPlug("sss_sample_spacing").asFloat());
      AiNodeSetBool(polymesh, "sss_use_gi", m_fnNode.findPlug("sss_use_gi").asBool());
   }
}

AtNode* CGeoTranslator::ExportMesh(AtNode* polymesh, bool update)
{
   AiNodeSetStr(polymesh, "name", m_dagPath.fullPathName().asChar());

   ExportMatrix(polymesh, 0);
   ExportMeshParameters(polymesh);
   ExportMeshShaders(polymesh, m_fnMesh);
   IsGeoDeforming();
   if (!update)
      ExportMeshGeoData(polymesh, 0);
   return polymesh;
}

AtNode* CGeoTranslator::ExportInstance(AtNode *instance, const MDagPath& masterInstance)
{
   AtNode* masterNode = AiNodeLookUpByName(masterInstance.fullPathName().asChar());

   // FIXME: we should not be here if we are not instanced, why the call to isInstanced? (chad)
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   AiNodeSetStr(instance, "name", m_dagPath.fullPathName().asChar());

   ExportMatrix(instance, 0);

   AiNodeSetPtr(instance, "node", masterNode);
   AiNodeSetBool(instance, "inherit_xform", false);

   //
   // SHADERS
   //
   MFnMesh           meshNode(m_dagPath.node());
   MObjectArray      shaders, shadersMaster;
   MIntArray         indices, indicesMaster;

   meshNode.getConnectedShaders(instanceNum, shaders, indices);
   // FIXME: it is incorrect to assume that instance 0 is the master as it may be hidden (chad)
   meshNode.getConnectedShaders(0, shadersMaster, indicesMaster);

   // As arnold does not support different shaders per face
   // on ginstances
   // we keep the master's per face assignment only
   // if it's completely the same
   bool equalShaderArrays = ((shaders.length() == shadersMaster.length()) && (indices.length() == indicesMaster.length()));

   // compare face arrays
   for(AtUInt j=0; (equalShaderArrays && (j < indices.length())); j++)
   {
      if(indices[j] != indicesMaster[j])
      {
         equalShaderArrays = false;
      }
   }
   // compare shader (actually SG) arrays
   for(AtUInt i=0; (equalShaderArrays && (i < shaders.length())); i++)
   {
      if (shaders[i] != shadersMaster[i])
      {
         equalShaderArrays = false;
      }
   }

   if ( (shaders.length() > 0) && (shadersMaster.length() > 0) )
   {
      MPlugArray        connections;
      MFnDependencyNode fnDGNode(shaders[0]);
      MPlug             shaderPlug(shaders[0], fnDGNode.attribute("surfaceShader"));
      MPlug             shaderPlugMaster(shadersMaster[0], fnDGNode.attribute("surfaceShader"));

      shaderPlug.connectedTo(connections, true, false);

      if ((shaderPlug != shaderPlugMaster) || (!equalShaderArrays))
      {
         AtNode* shader = m_scene->ExportShader(connections[0].node());
         AiNodeSetPtr(instance, "shader", shader);
      }
   }
   return instance;
}

void CGeoTranslator::Update(AtNode *anode)
{
   if (m_isMasterDag)
   {
      ExportMesh(anode, true);
   }
   else
   {
      ExportInstance(anode, m_masterDag);
   }
}

void CGeoTranslator::ExportMotion(AtNode* anode, AtUInt step)
{
   // ran into a strange bug where the object must be reset to
   // avoid a crash.  even calling .hasObj() was enough to avoid it
   m_fnMesh.setObject(m_object);
   if (m_isMasterDag)
   {
      ExportMatrix(anode, step);
      if (m_motionDeform)
      {
         ExportMeshGeoData(anode, step);
      }
   }
   else
   {
      ExportMatrix(anode, step);
   }
}

void CGeoTranslator::UpdateMotion(AtNode* anode, AtUInt step)
{
   ExportMatrix(anode, step);
}

void CGeoTranslator::AddCallbacks()
{
   AddShaderAssignmentCallbacks( m_object );
   CDagTranslator::AddCallbacks();
}

void CGeoTranslator::AddShaderAssignmentCallbacks(MObject & dagNode )
{
   MStatus status;
   MCallbackId id = MNodeMessage::addAttributeChangedCallback( dagNode, ShaderAssignmentCallback, this, &status );
   if ( MS::kSuccess == status ) ManageCallback( id );
}

void CGeoTranslator::ShaderAssignmentCallback( MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void*clientData )
{
   // Shading assignments are done with the instObjGroups attr, so we only
   // need to update when that is the attr that changes.
   if ( (msg & MNodeMessage::kConnectionMade) && (plug.partialName() == "iog") )
   {
      CGeoTranslator * translator = static_cast< CGeoTranslator* >(clientData);
      if ( translator != NULL )
      {
         // Interupt the render.
         CRenderSession* renderSession = CRenderSession::GetInstance();
         renderSession->InterruptRender();
         // Export the new shaders.
         translator->ExportShaders();
         // Update Arnold without passing a translator, this just forces a redraw.
         CMayaScene::UpdateIPR();
      }
   }
}


// CNurbsTranslator
//

void CNurbsSurfaceTranslator::GetTessellationOptions(MTesselationParams & params,
                                              MFnNurbsSurface & surface )
{
   // Reference for this code is from the devkit:
   // /devkit/obsolete/games/MDtApi/MDtShape.cpp
   // It is similar, this is tidier and more condenced.

   // Get the tesselation attributes off the node
   const int modeU                  = surface.findPlug( "modeU" ).asInt();
   const int numberU                = surface.findPlug( "numberU" ).asInt();
   const int modeV                  = surface.findPlug( "modeV" ).asInt();
   const int numberV                = surface.findPlug( "numberV" ).asInt();
   const bool smoothEdge            = surface.findPlug( "smoothEdge" ).asBool();
   const bool useChordHeightRatio   = surface.findPlug( "useChordHeightRatio" ).asBool();
   const bool edgeSwap              = surface.findPlug( "edgeSwap" ).asBool();
   const bool useMinScreen          = surface.findPlug( "useMinScreen" ).asBool();
   const double chordHeightRatio    = surface.findPlug( "chordHeightRatio" ).asDouble();
   const double minScreen           = surface.findPlug( "minScreen" ).asDouble();

   // I don't actually know why these aren't used. I don't see where they'd be set
   // on MTesselationParams either.
   //const bool useChordHeight      = surface.findPlug( "useChordHeight" ).asBool();
   //const double chordHeight       = surface.findPlug( "chordHeight" ).asDouble();

   switch ( modeU )
   {
      case 1:             // Per Surf # of Isoparms in 3D
         params.setUIsoparmType( MTesselationParams::kSurface3DEquiSpaced);
         break;
      case 2:             // Per Surf # of Isoparms
         params.setUIsoparmType( MTesselationParams::kSurfaceEquiSpaced);
         break;
      case 3:             // Per Span # of Isoparms
         params.setUIsoparmType( MTesselationParams::kSpanEquiSpaced);
         break;
      case 4:             // Best Guess Based on Screen Size, there is a comment that 4 uses mode 2 internally
         params.setUIsoparmType( MTesselationParams::kSurfaceEquiSpaced);
         break;
   }

   switch ( modeV )
   {
      case 1:             // Per Surf # of Isoparms in 3D
         params.setVIsoparmType( MTesselationParams::kSurface3DEquiSpaced);
         break;
      case 2:             // Per Surf # of Isoparms
         params.setVIsoparmType( MTesselationParams::kSurfaceEquiSpaced);
         break;
      case 3:             // Per Span # of Isoparms
         params.setVIsoparmType( MTesselationParams::kSpanEquiSpaced);
         break;
      case 4:             // Best Guess Based on Screen Size, there is a comment that 4 uses mode 2 internally
         params.setVIsoparmType( MTesselationParams::kSurfaceEquiSpaced);
         break;
   }

   params.setUNumber(numberU);
   params.setVNumber(numberV);
   params.setSubdivisionFlag(MTesselationParams::kUseChordHeightRatio, useChordHeightRatio);
   params.setChordHeightRatio(chordHeightRatio );
   params.setSubdivisionFlag(MTesselationParams::kUseMinScreenSize,useMinScreen);
   params.setMinScreenSize(minScreen, minScreen);

   params.setSubdivisionFlag(MTesselationParams::kUseEdgeSmooth, smoothEdge);
   params.setSubdivisionFlag(MTesselationParams::kUseTriangleEdgeSwapping, edgeSwap);
}

bool CNurbsSurfaceTranslator::Tessellate(MDagPath & dagPath)
{
   MStatus status;
   MFnNurbsSurface surface(dagPath, &status);
   if (!status)
   {
      AiMsgError("[mtoa] ERROR: Could not attache to NURBS surface for tessallation: %s",
                 status.errorString().asChar());
      return false;
   }

   MFnMeshData meshData;
   // This is a member variable. We have to keep hold of it or Maya will release it.
   m_data_mobj = meshData.create();

   MTesselationParams params(MTesselationParams::kGeneralFormat, MTesselationParams::kTriangles);
   GetTessellationOptions( params, surface );
   MObject mesh_mobj = surface.tesselate(params,
                                         m_data_mobj,
                                         &status);
   if (!status)
   {
      AiMsgError("[mtoa] ERROR: Could not tessallate NURBS surface: %s",
                 status.errorString().asChar());
      return false;
   }

   // set the MFnMesh to the newly created node
   m_fnMesh.setObject(mesh_mobj);
   return true;
}

AtNode* CNurbsSurfaceTranslator::Export()
{
   AtNode* anode = NULL;
   m_isMasterDag = IsMasterInstance(m_masterDag);
   if (m_isMasterDag)
   {
      // Early return if we can't tessalate. anode will be NULL.
      if (!Tessellate(m_dagPath)) return anode;

      anode = AiNode("polymesh");
      ExportMesh(anode, false);
      return anode;
   }
   else
   {
      anode = AiNode("ginstance");
      ExportInstance(anode, m_masterDag);
      return anode;
   }
}

void CNurbsSurfaceTranslator::ExportMotion(AtNode* anode, AtUInt step)
{
   // Re-tessalate the nurbs surface, but only if it's needed.
   if (m_motion && m_motionDeform && m_isMasterDag)
   {
      // TODO: Figure out how to get the same topology for
      // each tessellation.
      if (!Tessellate(m_dagPath)) return;
   }

   CGeoTranslator::ExportMotion(anode, step);
}

// TODO: implement this check for nurbs.
void CNurbsSurfaceTranslator::IsGeoDeforming()
{
}

 // --------- CMeshTranslator -------------//
unsigned int CMeshTranslator::GetNumMeshGroups()
{
   MObject node = m_dagPath.node();
   MFnDependencyNode fnDGNode(node);
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   MPlug plug = fnDGNode.findPlug("instObjGroups");

   if (plug.elementByLogicalIndex(instanceNum).isConnected())
   {
      return 1;
   }
   else
   {
      MFnMesh      mesh(node);
      MObjectArray shaders;
      MIntArray    indices;

      mesh.getConnectedShaders(instanceNum, shaders, indices);

      return shaders.length();
   }
}

AtNode* CMeshTranslator::Export()
{

   AtNode* anode = NULL;
   if (GetNumMeshGroups() == 0)
   {
      AiMsgError("[mtoa] ERROR: Mesh not exported. It has 0 groups.");
      return anode;
   }
   m_isMasterDag = IsMasterInstance(m_masterDag);
   if (m_isMasterDag)
   {
      m_fnMesh.setObject(m_dagPath.node());
      anode = AiNode("polymesh");
      ExportMesh(anode, false);
      return anode;
   }
   else
   {
      anode = AiNode("ginstance");
      ExportInstance(anode, m_masterDag);
      return anode;
   }
}


