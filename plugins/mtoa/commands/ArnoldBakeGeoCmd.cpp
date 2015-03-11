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

   while (!AiNodeIteratorFinished(node_itr))
   {
      AtNode *node = AiNodeIteratorGetNext(node_itr);
      if (AiNodeIs(node, "polymesh") )
      {
         // MGlobal::displayInfo(MString(AiNodeGetName(node)));
         std::stringstream vBuf, uvBuf, nBuf, fBuf;
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
         while (AiShaderGlobalsGetTriangle(sg, 0, localPos))
         {
            AiShaderGlobalsGetVertexUVs(sg, uv);
            AiShaderGlobalsGetVertexNormals(sg, 0, localNormal);

            fBuf <<"f "<<vert_index<<" "<<vert_index+1<<" "<<vert_index+2<<"\n";
            vert_index+=3;
            
            // Please God forgive me for duplicating the vertices for each triangle
            for (int j = 0; j < 3; ++j)
            {
               // convert local vertices to world              
               AiM4PointByMatrixMult(&worldPos[j], localToWorld, &localPos[j]); 
               AiM4VectorByMatrixMult(&worldNormal[j], localToWorld, &localNormal[j]); 
               
               vBuf <<"v "<<(float)worldPos[j].x<< " "<<(float)worldPos[j].y<<" "<<(float)worldPos[j].z<<"\n";
               nBuf <<"vn "<<(float)worldNormal[j].x<< " "<<(float)worldNormal[j].y<<" "<<(float)worldNormal[j].z<<"\n";
               uvBuf <<"vt "<<(float)uv[j].x<< " "<<(float)uv[j].y<<"\n";

            }
            sg->fi = ++index;
         }

         os <<vBuf.str()<<"\n"<<fBuf.str()<<"\n"<<nBuf.str()<<"\n"<<uvBuf.str()<<"\n";
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
