
#include <ai_nodes.h>

#include <maya/MFloatPointArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MMatrix.h>

#include <vector>

void ProcessMesh(MObject mayaMesh, MObject dagNode, MMatrix tm)
{
   MFnMesh    fnMesh(mayaMesh);
   MFnDagNode fnDagNode(dagNode);
   bool       hasUVs = (fnMesh.numUVs() > 0);
   AtNode*    polymesh = AiNode("polymesh");

   AiNodeSetStr(polymesh, "name", fnDagNode.name().asChar());

   AtMatrix matrix;

   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) tm[I][J];
      }
   }

   AiNodeSetMatrix(polymesh, "matrix", matrix);

   AiNodeSetBool(polymesh, "smoothing", 1);

   //
   // GEOMETRY
   //

   // Get all vertices
   if (fnMesh.numVertices() > 0)
   {
      MFloatPointArray pointsArray;

      fnMesh.getPoints(pointsArray, MSpace::kObject);

      float* vertices = new float[fnMesh.numVertices() * 3];

      for (int J = 0; ( J < fnMesh.numVertices() ); ++J)
      {
         vertices[J * 3 + 0] = pointsArray[J].x;
         vertices[J * 3 + 1] = pointsArray[J].y;
         vertices[J * 3 + 2] = pointsArray[J].z;
      }

      AiNodeSetArray(polymesh, "vlist", AiArrayConvert(fnMesh.numVertices() * 3, 1, AI_TYPE_FLOAT, vertices, TRUE));

      delete[] vertices;
   }

   // Get all normals
   if (fnMesh.numNormals() > 0)
   {
      MFloatVectorArray normalArray;

      fnMesh.getNormals(normalArray, MSpace::kObject);

      float* normals = new float[fnMesh.numNormals() * 3];

      for (int J = 0; (J < fnMesh.numNormals()); ++J)
      {
         normals[J * 3 + 0] = normalArray[J].x;
         normals[J * 3 + 1] = normalArray[J].y;
         normals[J * 3 + 2] = normalArray[J].z;
      }

      AiNodeSetArray(polymesh, "nlist", AiArrayConvert(fnMesh.numNormals() * 3, 1, AI_TYPE_FLOAT, normals, TRUE));

      delete[] normals;
   }

   // Get all UVs
   if (hasUVs)
   {
      MFloatArray uArray, vArray;

      fnMesh.getUVs(uArray, vArray);

      float* uvs = new float[fnMesh.numUVs() * 2];

      for (int J = 0; (J < fnMesh.numUVs()); ++J)
      {
         uvs[J * 2 + 0] = uArray[J];
         uvs[J * 2 + 1] = vArray[J];
      }

      AiNodeSetArray(polymesh, "uvlist", AiArrayConvert(fnMesh.numUVs() * 2, 1, AI_TYPE_FLOAT, uvs, TRUE));

      delete[] uvs;
   }

   // Traverse all polygons to export vidxs, nidxs, uvindxs y nsides
   AtByte* nsides = new AtByte[fnMesh.numPolygons()];

   std::vector<AtLong> vidxs, nidxs, uvidxs;

   MItMeshPolygon itMeshPolygon(mayaMesh);
   unsigned int   polygonIndex = 0;

   for (; (!itMeshPolygon.isDone()); itMeshPolygon.next())
   {
      unsigned int vertexCount = itMeshPolygon.polygonVertexCount();

      nsides[polygonIndex] = vertexCount;

      for (size_t V = 0; (V < vertexCount); ++V)
      {
         vidxs.push_back(itMeshPolygon.vertexIndex(V));
         nidxs.push_back(itMeshPolygon.normalIndex(V));

         if (hasUVs)
         {
            int uvIndex;

            itMeshPolygon.getUVIndex(V, uvIndex);

            uvidxs.push_back(uvIndex);
         }
      }

      ++polygonIndex;
   }

   AiNodeSetArray(polymesh, "nsides", AiArrayConvert(fnMesh.numPolygons(), 1, AI_TYPE_BYTE, nsides, TRUE));
   AiNodeSetArray(polymesh, "vidxs", AiArrayConvert(vidxs.size(), 1, AI_TYPE_UINT, &(vidxs[0]), TRUE));
   AiNodeSetArray(polymesh, "nidxs", AiArrayConvert(nidxs.size(), 1, AI_TYPE_UINT, &(nidxs[0]), TRUE));

   if (hasUVs)
   {
      AiNodeSetArray(polymesh, "uvidxs", AiArrayConvert(uvidxs.size(), 1, AI_TYPE_UINT, &(uvidxs[0]), TRUE));
   }

   delete[] nsides;

}  // ProcessMesh()
