#include "ArnoldBakeGeoCmd.h"
#include "../scene/MayaScene.h"
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
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <ostream>
#include <ai.h>
#include <fstream>
#include <istream>
#include <streambuf>



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
   os << "# Arnold Renderer - OBJ export\n";  // any else we want to dump in the header ?

   AiBegin();
   CMayaScene::Begin(MTOA_SESSION_ASS);
   CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();
   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   renderSession->SetForceTranslateShadingEngines(true);   
   arnoldSession->SetExportFilterMask(AI_NODE_ALL);

   CMayaScene::Export(&selected);

   //  First, iterate over the geometries to get the matrices
   // Otherwise, as soon as AiRender is called, they are re-initialized
   AtNodeIterator* node_itr = AiUniverseGetNodeIterator(AI_NODE_ALL);

   // not trusting the way AtMatrix are copied
   // so I'm using a float[16] here
   // but If I'm wrong we might use a std::map<AtMatrix, std:string> instead
   
   std::map<std::string, matrixAsFloats>  mtxMap;
   while (!AiNodeIteratorFinished(node_itr))
   {
      AtNode *node = AiNodeIteratorGetNext(node_itr);
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
   AiNodeIteratorDestroy(node_itr);

   AiRender(AI_RENDER_MODE_FREE);
   node_itr = AiUniverseGetNodeIterator(AI_NODE_ALL);
   AtShaderGlobals* sg = AiShaderGlobals();
   int vert_index = 1; // vertex indices must be global to whole obj, starting at 1

   std::vector<AtPoint> vertices;
   std::vector<AtVector> normals;
   std::vector<AtPoint2> uvs;
   
   while (!AiNodeIteratorFinished(node_itr))
   {
      AtNode *node = AiNodeIteratorGetNext(node_itr);
      if (AiNodeIs(node, "polymesh") )
      {
         // MGlobal::displayInfo(MString(AiNodeGetName(node)));
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

         //MGlobal::displayInfo(MString(matrix_str.str().c_str()));

         int index = 0;
         bool valid_uvs = true;
         bool valid_normals = true;
         
         while (AiShaderGlobalsGetTriangle(sg, 0, localPos))
         {
            if (valid_uvs && !AiShaderGlobalsGetVertexUVs(sg, uv)) valid_uvs = false;
            if (valid_normals && !AiShaderGlobalsGetVertexNormals(sg, 0, localNormal)) valid_normals = false;

            
            // Please God forgive me for duplicating the vertices for each triangle
            for (int j = 0; j < 3; ++j)
            {
               // convert local vertices to world              
               AiM4PointByMatrixMult(&worldPos[j], localToWorld, &localPos[j]); 
               AiM4VectorByMatrixMult(&worldNormal[j], localToWorld, &localNormal[j]); 
               vertices.push_back(AiPoint((float)worldPos[j].x, (float)worldPos[j].y, (float)worldPos[j].z));

               if (valid_normals)
               {
                  normals.push_back(AiVector((float)worldNormal[j].x, (float)worldNormal[j].y, (float)worldNormal[j].z));
               }
               if (valid_uvs)
               {
                  uvs.push_back(AiPoint2((float)uv[j].x, (float)uv[j].y));
               }

            }
            sg->fi = ++index;
         }

         for (size_t i = 0; i < vertices.size(); ++i)
         {
            os<<"v "<<vertices[i].x<< " "<<vertices[i].y<<" "<<vertices[i].z<<"\n";
         }
         os<<"\n";

         if (valid_uvs)
         {
            for (size_t i = 0; i < uvs.size(); ++i)
            {
               os<<"vt "<<uvs[i].x<< " "<<uvs[i].y<<"\n";
            }
            os<<"\n";
         }

         if (valid_normals)
         {
            for (size_t i = 0; i < normals.size(); ++i)
            {
               os<<"vn "<<normals[i].x<< " "<<normals[i].y<<" "<<normals[i].z<<"\n";
            }
         }

         // Write the faces
         for (int f = 0; f < index; ++f)
         {
            os <<"f ";
            for (int v =0; v < 3; ++v)
            {
               os<<vert_index;
               if (valid_uvs)
               {
                  if (valid_normals) os<<"/"<<vert_index<<"/"<<vert_index; // both UVs and normals
                  else os<<"/"<<vert_index;  // only UVs
               } else if (valid_normals) os<<"//"<<vert_index; // only normals                  
               
               os<<" ";
               vert_index++;
            }
            os<<"\n";
         }
         os<<"\n";

         vertices.clear();
         uvs.clear();
         normals.clear();
      }
   } 

   std::string outLog = "Exported geometry as ";
   outLog += filename.asChar();
   
   fb.close();
   MGlobal::displayInfo(MString(outLog.c_str()));

   AiNodeIteratorDestroy(node_itr);
   AiShaderGlobalsDestroy(sg);
   AiRenderAbort();

   CMayaScene::End();
   AiEnd();

   return MS::kSuccess;
}
