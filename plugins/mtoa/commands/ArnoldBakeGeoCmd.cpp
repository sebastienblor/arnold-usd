#include "ArnoldBakeGeoCmd.h"
#include "../scene/MayaScene.h"
#include <ai.h>

#include <maya/MStatus.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MFnSet.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnAttribute.h>
#include <maya/MStringArray.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>

#include <cstring>
#include <iostream>
#include <sstream>
#include <ostream>
#include <fstream>
#include <istream>
#include <streambuf>
#include <unordered_map>

// hash function from http://www.cse.yorku.ca/~oz/hash.html
inline size_t
HashFunctionDJB2(const unsigned char *input, size_t size)
{
   size_t hash = 5381;
   int c;
   while (size--)
   {
      c = *input++;
      hash = ((hash << 5) + hash) + c; //hash * 33 + c
   }
   return hash;
}

CArnoldBakeGeoCmd::CArnoldBakeGeoCmd()
{   
}

CArnoldBakeGeoCmd::~CArnoldBakeGeoCmd()
{   
}

MSyntax CArnoldBakeGeoCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("f", "filename", MSyntax::kString);
   syntax.setObjectType(MSyntax::kStringObjects);
   return syntax;
}
void* CArnoldBakeGeoCmd::creator()
{
   return new CArnoldBakeGeoCmd();
}

struct matrixAsFloats {
   float elems[16];
};

MStatus CArnoldBakeGeoCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MSyntax syntax = newSyntax();

   // we must use an MArgParser because MArgList is not python compatible, and we
   // use this result to set syntax.useSelectionAsDefault() prior to creating the MArgDatabase.
   MArgParser args(syntax, argList, &status);
   MArgDatabase argDB(syntax, argList, &status);

   MSelectionList selected; // list of selected objects to be exported

   MStringArray sListStrings;
   argDB.getObjects(sListStrings);
   const unsigned int sListStringsLength = sListStrings.length();
   
   if (sListStringsLength > 0)
   {
      for (unsigned int i = 0; i < sListStringsLength; ++i)
         selected.add(sListStrings[i]);
   }
   else
      MGlobal::getActiveSelectionList(selected);

   // do we want to export the whole scene if nothing is selected ?
   if (selected.length() == 0)
   {
      AiMsgError("[mtoa] No geometry selected");
      return MS::kFailure;
   }
   
   if (!argDB.isFlagSet("filename"))
   {
      AiMsgError("[mtoa] Bake Geometry : No filename specified");
      return MS::kFailure;
   }

   CMayaScene::End();
   // Cannot export while a render is active
   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] Cannot bake geometry while rendering");
      return MS::kFailure;
   }

   MString filename = "";
   argDB.getFlagArgument("filename", 0, filename);

   std::filebuf fb;
   if (fb.open(filename.asChar(),ios::out) == NULL)
   {
      AiMsgError("[mtoa] Cannot create output file");
      return MS::kFailure;  
   }
   std::ostream os(&fb);
   os << "# Arnold Renderer - OBJ export\n";  // anything else we want to dump in the header ?

   AiBegin();
   CMayaScene::Begin(MTOA_SESSION_ASS);
   CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();
   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   renderSession->SetForceTranslateShadingEngines(true);   
   arnoldSession->SetExportFilterMask(AI_NODE_ALL);

   CMayaScene::Export(&selected);

   //  First, iterate over the geometries to get the matrices
   // Otherwise, as soon as AiRender is called, they are re-initialized
   AtNodeIterator* nodeIter = AiUniverseGetNodeIterator(AI_NODE_ALL);

#ifdef _DARWIN
   std::unordered_map<std::string, matrixAsFloats>  mtxMap;
#else
   std::tr1::unordered_map<std::string, matrixAsFloats>  mtxMap;
#endif
   
   while (!AiNodeIteratorFinished(nodeIter))
   {
      AtNode *node = AiNodeIteratorGetNext(nodeIter);
      if (AiNodeIs(node, "polymesh") )
      {
         AtMatrix localToWorld;
         AiNodeGetMatrix(node, "matrix", localToWorld);
         matrixAsFloats mtxFlt;
         int ind = 0;
         for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++, ind++)
               mtxFlt.elems[ind] = localToWorld[j][k];

         std::string name(AiNodeGetName(node));
         matrixAsFloats &objValue = mtxMap[name];
         objValue = mtxFlt;
      }
   }
   AiNodeIteratorDestroy(nodeIter);

   AiRender(AI_RENDER_MODE_FREE);
   nodeIter = AiUniverseGetNodeIterator(AI_NODE_ALL);
   AtShaderGlobals* sg = AiShaderGlobals();
   
   std::vector<AtPoint> vertices;
   std::vector<AtVector> normals;
   std::vector<AtPoint2> uvs;
   std::vector<unsigned int> vertexIds;
   
#ifdef _DARWIN
   std::unordered_map<size_t, unsigned int>  vertexMap;
   std::unordered_map<size_t, unsigned int>::iterator  vertexMapIter;
#else
   std::tr1::unordered_map<size_t, unsigned int>  vertexMap;
   std::tr1::unordered_map<size_t, unsigned int>::iterator  vertexMapIter;
#endif
   unsigned int vtxOffset = 1;  // OBJ expects vertex indices starting at 1

   while (!AiNodeIteratorFinished(nodeIter))
   {
      AtNode *node = AiNodeIteratorGetNext(nodeIter);
      if (AiNodeIs(node, "polymesh") )
      {
         os <<"o "<<AiNodeGetName(node)<<"\n";
        
         sg->Op = node;
         sg->fi = 0;
         AtPoint localPos[3], worldPos[3];
         AtVector localNormal[3], worldNormal[3];
         AtPoint2 uv[3];
         AtMatrix localToWorld;
         
         matrixAsFloats& mtxFlt = mtxMap[AiNodeGetName(node)];

         int mtxInd = 0;
         for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 4; ++k, ++mtxInd)
               localToWorld[j][k] = mtxFlt.elems[mtxInd];


         int triangleIndex = 0;
         bool validUvs = true;
         bool validNormals = true;
         
         // First try to get roughly the amount of vertices in this mesh
         // by recursively multiplying the previous value by 2
         // Let's start with a value of 64
         sg->fi = (vertices.capacity() == 0) ? 64 : vertices.capacity();
         while (AiShaderGlobalsGetTriangle(sg, 0, localPos))
         {
            //This mesh has more vertices than my vectors capacity
            // I keep multiplying this value by 2 until 
            // this function returns false (meaning that we're > vertexCount)
            sg->fi *= 2;
         }
         if (sg->fi > vertices.capacity())
         {
            vertices.reserve(sg->fi);
            normals.reserve(sg->fi);
            uvs.reserve(sg->fi);
            vertexIds.reserve(sg->fi);
         }

         // Let's start again for good now,
         // by reseting the triangle index
         sg->fi = 0;
         while (AiShaderGlobalsGetTriangle(sg, 0, localPos))
         {            
            for (int j = 0; j < 3; ++j)
            {
               // compute a hash key for this vertex position
               size_t positionHash = HashFunctionDJB2((const unsigned char *)&localPos[j], sizeof(AtPoint));

               vertexMapIter = vertexMap.find(positionHash);
               if (vertexMapIter != vertexMap.end())
               {
                  // this vertex already exists
                  vertexIds.push_back(vertexMapIter->second);
                  // no need to add the vertex / normals / uvs to the list
                  continue;
               }

               // this vertex hasn't been found yet
               unsigned int meshVtxId = (unsigned int)vertices.size() + vtxOffset;
               vertexMap[positionHash] = meshVtxId;
               vertexIds.push_back(meshVtxId);

               if (validUvs && !AiShaderGlobalsGetVertexUVs(sg, uv)) validUvs = false;
               if (validNormals && !AiShaderGlobalsGetVertexNormals(sg, 0, localNormal)) validNormals = false;


               // convert local vertices to world              
               AiM4PointByMatrixMult(&worldPos[j], localToWorld, &localPos[j]); 
               AiM4VectorByMatrixMult(&worldNormal[j], localToWorld, &localNormal[j]); 
               vertices.push_back(AiPoint((float)worldPos[j].x, (float)worldPos[j].y, (float)worldPos[j].z));

               if (validNormals)
               {
                  normals.push_back(AiVector((float)worldNormal[j].x, (float)worldNormal[j].y, (float)worldNormal[j].z));
               }
               if (validUvs)
               {
                  uvs.push_back(AiPoint2((float)uv[j].x, (float)uv[j].y));
               }
            }
            sg->fi = ++triangleIndex;
         }

         for (size_t i = 0; i < vertices.size(); ++i)
         {
            os<<"v "<<vertices[i].x<< " "<<vertices[i].y<<" "<<vertices[i].z<<"\n";
         }
         os<<"\n";

         if (validUvs)
         {
            for (size_t i = 0; i < uvs.size(); ++i)
            {
               os<<"vt "<<uvs[i].x<< " "<<uvs[i].y<<"\n";
            }
            os<<"\n";
         }

         if (validNormals)
         {
            for (size_t i = 0; i < normals.size(); ++i)
            {
               os<<"vn "<<normals[i].x<< " "<<normals[i].y<<" "<<normals[i].z<<"\n";
            }
         }

         // Write the faces
         int triangleVtxId = 0;
         for (int f = 0; f < triangleIndex; ++f)
         {
            os <<"f ";
            for (int v =0; v < 3; ++v, ++triangleVtxId)
            {
               int globalVertexIndex = vertexIds[triangleVtxId];
               os<<globalVertexIndex;
               if (validUvs)
               {
                  if (validNormals) os<<"/"<<globalVertexIndex<<"/"<<globalVertexIndex; // both UVs and normals
                  else os<<"/"<<globalVertexIndex;  // only UVs
               } else if (validNormals) os<<"//"<<globalVertexIndex; // only normals                  
               
               os<<" ";
            }
            os<<"\n";
         }
         os<<"\n";

         // incrementing the global vertex Offset
         vtxOffset += vertices.size();

         // clear the vectors / containers for the next mesh
         vertices.clear();
         uvs.clear();
         normals.clear();
         vertexMap.clear();
         vertexIds.clear();
      }
   }

   std::string outLog = "Exported geometry as ";
   outLog += filename.asChar();
   
   fb.close();
   MGlobal::displayInfo(MString(outLog.c_str()));

   AiNodeIteratorDestroy(nodeIter);
   AiShaderGlobalsDestroy(sg);
   AiRenderAbort();

   CMayaScene::End();
   AiEnd();

   return MS::kSuccess;
}
