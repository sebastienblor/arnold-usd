#include "../common/XgArnoldExpand.h"

#include "extension/Extension.h"
#include "utils/time.h"

#include <maya/MFileObject.h>
#include <maya/MTime.h>
#include <maya/MGlobal.h>
#include <maya/MFnCamera.h>
#include <maya/MMatrix.h>

#include "XGenTranslator.h"


static void SetEnv(const MString& env, const MString& val)
{
#ifdef WIN32
   MGlobal::executePythonCommand(MString("import os;os.environ['")+env+MString("']='")+val+MString("'"));
#else
   setenv(env.asChar(), val.asChar(), true);
#endif      
}

//#define DEBUG_MTOA 1

inline bool alembicExists(const std::string& name)
{
   struct stat buffer;
   return (stat (name.c_str(), &buffer) == 0);
}

AtNode* CXgDescriptionTranslator::CreateArnoldNodes()
{   
   m_expandedProcedural = NULL;
   //AiMsgInfo("[CXgDescriptionTranslator] CreateArnoldNodes()");
   return AddArnoldNode("xgen_procedural");
}

void CXgDescriptionTranslator::Delete()
{
   // If the procedural has been expanded at export,
   // we need to delete all the created nodes here 
   if (m_expandedProcedural)
   {
      int numNodes = m_expandedProcedural->NumNodes();
      for (int i = 0; i < numNodes; ++i)
      {
         AtNode *node = m_expandedProcedural->GetNode(i);
         if (node == NULL) continue; 
         AiNodeDestroy(node);
      }
      m_expandedProcedural->Cleanup();

      delete m_expandedProcedural;
      m_expandedProcedural = NULL;
      m_exportedSteps.clear();
   }
   CShapeTranslator::Delete();
}

struct DescInfo
{
   std::string strScene;
   std::string strPalette;
   std::string strDescription;
   std::vector<std::string> vecPatches;
   float fFrame;
   uint  renderMode;
   uint  moblur;
   uint  moblurmode;
   uint  motionBlurSteps;
   float moblurFactor;
   
   bool  hasAlembicFile;
   
   float aiMinPixelWidth;
   int aiMode;

   bool  bCameraOrtho;
   float fCameraPos[3];
   float fCameraFOV;
   float fCameraInvMat[16];
   float fCamRatio;
   float fBoundingBox[6];

   std::string auxRenderPatch;
   bool useAuxRenderPatch;

   bool multithreading;

   void setBoundingBox( float xmin, float ymin, float zmin, float xmax, float ymax, float zmax )
   {
      fBoundingBox[0] = xmin;
      fBoundingBox[1] = ymin;
      fBoundingBox[2] = zmin;

      fBoundingBox[3] = xmax;
      fBoundingBox[4] = ymax;
      fBoundingBox[5] = zmax;
   }

   void setCameraPos( float x, float y, float z )
   {
      fCameraPos[0] = x;
      fCameraPos[1] = y;
      fCameraPos[2] = z;
   }

   void setCameraInvMat(    float m00, float m01, float m02, float m03,
                     float m10, float m11, float m12, float m13,
                     float m20, float m21, float m22, float m23,
                     float m30, float m31, float m32, float m33 )
   {
      fCameraInvMat[ 0] = m00;
      fCameraInvMat[ 1] = m01;
      fCameraInvMat[ 2] = m02;
      fCameraInvMat[ 3] = m03;
      fCameraInvMat[ 4] = m10;
      fCameraInvMat[ 5] = m11;
      fCameraInvMat[ 6] = m12;
      fCameraInvMat[ 7] = m13;
      fCameraInvMat[ 8] = m20;
      fCameraInvMat[ 9] = m21;
      fCameraInvMat[10] = m22;
      fCameraInvMat[11] = m23;
      fCameraInvMat[12] = m30;
      fCameraInvMat[13] = m31;
      fCameraInvMat[14] = m32;
      fCameraInvMat[15] = m33;
   }
};

void CXgDescriptionTranslator::Export(AtNode* procedural)
{
   if (IsExported())
   {
      // Since we always use AI_RECREATE_NODE during IPR (see RequestUpdate)
      // we should never get here. Early out for safety
      return;
   }

   
   // Get strings based on the current scene name.
   std::string strScenePath; // The path to the directory containing the scene.
   std::string strSceneFile; // The filename of the scene with the extension.
   std::string strSceneName; // The filename of the scene without the extension.
   {
      MString mstrCurrentScene;
      MGlobal::executeCommand("file -q -sn", mstrCurrentScene);
#ifdef DEBUG_MTOA
      printf("mstrCurrentScene=%s\n",mstrCurrentScene.asChar() );
#endif

      // In Batch render, file name has a number added. Get the original name
      if(GetSessionOptions().IsBatch())
      {
         int exists = 0;
            MGlobal::executeCommand("objExists arnoldBatchNode.mtoaOrigFileName", exists);
         if (exists == 1)
            MGlobal::executeCommand("getAttr \"arnoldBatchNode.mtoaOrigFileName\"", mstrCurrentScene);
      }

      MFileObject fo;
      fo.setRawFullName( mstrCurrentScene );

      strScenePath = fo.resolvedPath().asChar();
      strSceneFile = fo.resolvedName().asChar();
      strSceneName = strSceneFile.substr( 0, strSceneFile.size()-3 );
   }

#ifdef DEBUG_MTOA
   printf("strScenePath=%s\n",strScenePath.c_str() );
   printf("strSceneFile=%s\n",strSceneFile.c_str() );
   printf("strSceneName=%s\n",strSceneName.c_str() );
#endif

   // Get current units
   std::string strUnitConvMat;
   float fUnitConvFactor = 1.f;
   {
      std::string strCurrentUnits;
      {
         MString mstrCurrentUnits;
         MGlobal::executeCommand("currentUnit -q -linear", mstrCurrentUnits);
         strCurrentUnits = mstrCurrentUnits.asChar();
      }


      static std::map<std::string, std::pair<std::string,float> > s_mapUnitsConv;
      if( s_mapUnitsConv.empty() )
      {
         s_mapUnitsConv["in"] = std::pair<std::string,float>( "2.54", 2.54f );
         s_mapUnitsConv["ft"] = std::pair<std::string,float>( "30.48", 30.48f );
         s_mapUnitsConv["yd"] = std::pair<std::string,float>( "91.44", 91.44f );
         s_mapUnitsConv["mi"] = std::pair<std::string,float>( "160934.4", 160934.4f );
         s_mapUnitsConv["mm"] = std::pair<std::string,float>( "0.1", 0.1f );
         s_mapUnitsConv["km"] = std::pair<std::string,float>( "100000.0", 100000.f );
         s_mapUnitsConv["m"]  = std::pair<std::string,float>( "100.0", 100.f );
         s_mapUnitsConv["dm"] = std::pair<std::string,float>( "10.0", 10.f );
      }

      std::string factor = "1";
      std::map<std::string, std::pair<std::string,float> >::const_iterator it = s_mapUnitsConv.find( strCurrentUnits );
      if( it!=s_mapUnitsConv.end() )
      {
         factor = it->second.first;
         fUnitConvFactor = it->second.second;
      }
      strUnitConvMat = " -world "+factor+";0;0;0;0;"+factor+";0;0;0;0;"+factor+";0;0;0;0;1";
   }

   // Extract description info from the current maya shape node.
   DescInfo info;
   {
      // Get Description and Palette from the dag paths.
      // The current dag path points to the desciption.
      // We get the parent to get the palette name.
      {
         MDagPath palDagPath = m_dagPath;
         palDagPath.pop();
         palDagPath.pop();
         info.strPalette = palDagPath.partialPathName().asChar();
         const std::string strPaletteFullPath = palDagPath.fullPathName().asChar();
#ifdef DEBUG_MTOA
         printf("strPalette=%s\n",info.strPalette.c_str() );
#endif

         MDagPath descDagPath = m_dagPath;
         descDagPath.pop();
         info.strDescription = descDagPath.fullPathName().asChar();
         info.strDescription = info.strDescription.substr( strPaletteFullPath.size() + 1 );
#ifdef DEBUG_MTOA
         printf("strDescription=%s\n",info.strDescription.c_str() );
#endif
         unsigned int c = descDagPath.childCount();
         for( unsigned int i=0; i<c; ++i )
         {
            MDagPath childDagPath;
            MDagPath::getAPathTo( descDagPath.child(i),childDagPath );

            std::string strChild = childDagPath.fullPathName().asChar();
            char buf[512];
            sprintf(buf,"nodeType %s;",strChild.c_str());
            MString nodeType = MGlobal::executeCommandStringResult(buf);

            strChild = strChild.substr( strPaletteFullPath.size() + 1 + info.strDescription.size() + 1 );

         // Get the data for the translation from the description shape
            if (nodeType == "xgmDescription") 
            {
               MFnDagNode  xgenDesc;
               xgenDesc.setObject(childDagPath.node());
               
               info.renderMode = xgenDesc.findPlug("renderMode").asInt();
               
               info.aiMinPixelWidth = xgenDesc.findPlug("aiMinPixelWidth").asFloat();
               info.aiMode = xgenDesc.findPlug("aiMode").asInt();
               info.renderMode = xgenDesc.findPlug("renderMode").asInt();
               info.moblur = xgenDesc.findPlug("motionBlurOverride").asInt();
               info.moblurmode = xgenDesc.findPlug("motionBlurMode").asInt();
               info.motionBlurSteps = 1;
               info.moblurFactor = 0.5;
               info.auxRenderPatch = xgenDesc.findPlug("aiAuxRenderPatch").asString().asChar();
               info.useAuxRenderPatch = xgenDesc.findPlug("aiUseAuxRenderPatch").asBool();
               info.multithreading = xgenDesc.findPlug("aiMultithreading").asBool();

               //  use render globals moblur settings
               if (info.moblur == 0)
               {
                  if(GetSessionOptions().IsMotionBlurEnabled(MTOA_MBLUR_OBJECT))
                  {
                     GetMotionFrames(info.motionBlurSteps);
                     info.moblurFactor = (float)GetMotionByFrame();
                  }
               }
               // use  xgen per  description moblur settings
               else if (info.moblur == 1)
               {
                  info.motionBlurSteps = xgenDesc.findPlug("motionBlurSteps").asInt();
                  info.moblurFactor = xgenDesc.findPlug("motionBlurFactor").asFloat();
               }
#ifdef DEBUG_MTOA
               printf("strChild=%s\n",strChild.c_str() );
#endif
            }
            // Look for patches
            else
            {
               // TODO XGEN: in "LIVE" mode, this only works to update the geo position for guide style xgen,
               //  groom xgen needs an openGL preview before geo translation is taken into account
               //  we want to look for the  xgmSubdPatch node so we can get the BBox values and force an update on it
               if (nodeType == "transform")  // lets find the subdivPatch  transform 
               {
                  uint shapeCount = childDagPath.childCount();
                  for (uint x = 0; x < shapeCount; ++x)
                  {
                     MDagPath xgenShape;
                     MDagPath::getAPathTo( childDagPath.child(x),xgenShape );
                     if (xgenShape.apiType() == MFn::kTransform) // we've found another transform, probably a guide?
                     {
                        continue;
                     }
                     std::string strShape = xgenShape.fullPathName().asChar();
                     char buf[512];
                     sprintf(buf,"nodeType %s;",strShape.c_str());
                     MString nodeTyp = MGlobal::executeCommandStringResult(buf);
                     
                     // we want to  skip this if its  a  groomable spline description
                     if (nodeTyp == "igmDescription")
                     {
                        continue;
                     }
                     
                     if (nodeTyp == "xgmSubdPatch")
                     {
               #ifdef DEBUG_MTOA
                        printf("found xgmSubdPatch!\n");
               #endif

                        MFnDagNode  xgenNode;
                        xgenNode.setObject(xgenShape.node());
                        float xmin,ymin,zmin;
                        float xmax,ymax,zmax;
                        float xlen,ylen,zlen;
                        xmin = xgenNode.findPlug ( "bboxCorner10" ).asFloat();
                        ymin = xgenNode.findPlug ( "bboxCorner11" ).asFloat();
                        zmin = xgenNode.findPlug ( "bboxCorner12" ).asFloat();
                        xmax = xgenNode.findPlug ( "bboxCorner20" ).asFloat();
                        ymax = xgenNode.findPlug ( "bboxCorner21" ).asFloat();
                        zmax = xgenNode.findPlug ( "bboxCorner22" ).asFloat();
                        xlen = xmax-xmin;
                        ylen = ymax-ymin;
                        zlen = zmax-zmin;
                        xmin -= xlen*5*fUnitConvFactor; /// really this is  xlen*.5*10*fUnitConvFactor 
                        ymin -= ylen*5*fUnitConvFactor;
                        zmin -= zlen*5*fUnitConvFactor;
                        xmax += xlen*5*fUnitConvFactor;
                        ymax += ylen*5*fUnitConvFactor;
                        zmax += zlen*5*fUnitConvFactor; 

                        MPlug geo = xgenNode.findPlug ( "geometry");
                        MPlugArray connections;
                        geo.connectedTo(connections, true, false);
                        if (connections.length() > 0)
                        {
                           MFnDagNode patch;
                           patch.setObject(connections[0].node());
                           patch.setObject(patch.parent(0));
                           std::string strChildSub = patch.name().asChar();
                           info.vecPatches.push_back( strChildSub );
                        }

                        //printf("bbox: %f, %f, %f, %f, %f, %f\n",xmin,ymin,zmin,xmax,ymax,zmax);

                        //info.setBoundingBox(xmin,ymin,zmin,xmax,ymax,zmax);
                        break; // we only need to find one subd patch, and want to skip  any guides in here.. 
                     }
                  }
               }  // end force update and bbox  subdiv patch
            }
         }
      }

      info.strScene = strScenePath + strSceneName;
      info.fFrame = (float)MAnimControl::currentTime().value();

      // Hardcoded values for now.
      //float s = 100000.f * fUnitConvFactor;
      //info.setBoundingBox( -s,-s,-s, s, s, s );
      MDagPath camera = GetSessionOptions().GetExportCamera();

      if (camera.isValid())
      {
         MStatus status;
         MFnDependencyNode fnNode(camera.node());
         MFnCamera fnCamera(camera.node());

         // info.bCameraOrtho
         MPlug plug = fnNode.findPlug("aiTranslator", status);
         if (status && !plug.isNull())
         {
            if (plug.asString() == MString("orthographic"))
               info.bCameraOrtho = true;
            else if(plug.asString() == MString("perspective"))
               info.bCameraOrtho = false;
            else
               info.bCameraOrtho = FindMayaPlug("orthographic").asBool();
         }
         else
            info.bCameraOrtho = FindMayaPlug("orthographic").asBool();

         // info.setCameraPos
         MMatrix tm = camera.inclusiveMatrix(&status);
         info.setCameraPos( (float)tm[3][0], (float)tm[3][1], (float)tm[3][2] );

         // info.fCameraFOV
         info.fCameraFOV = (float)fnCamera.horizontalFieldOfView(&status) * AI_RTOD;

         // info.setCameraInvMat
         // This is correct. Maya expects a mix of the inverted and not inverted matrix
         //  values, and also with translation values in a different place.
         MMatrix tmi = camera.inclusiveMatrixInverse(&status);
         info.setCameraInvMat((float)tm[0][0], (float)tm[1][0], (float)tm[2][0], (float)tm[0][3],
                              (float)tm[0][1], (float)tm[1][1], (float)tm[2][1], (float)tm[1][3],
                              (float)tm[0][2], (float)tm[1][2], (float)tm[2][2], (float)tm[2][3],
                              (float)tmi[3][0], (float)tmi[3][1], (float)tmi[3][2], (float)tm[3][3]);

         // info.fCamRatio
         info.fCamRatio = (float)fnCamera.aspectRatio(&status);
      }
   }

   char buf[512];

   AtNode* rootShader = NULL;

   // The geom cache file should contain all the patches the palette uses.
   // Xgen gives an error if a patch used in the palette isn't found: Caf(alembic/abc) error. No geometry named 'pPlane1' found in caf(abc) file(frame):
   std::string strGeomFile = info.strScene + "__" + info.strPalette + ".abc";
   
   if(info.useAuxRenderPatch )// use override patch file)
   {
      if (!info.auxRenderPatch.empty())
      {
         strGeomFile = info.auxRenderPatch;
      }
   }
   // lets check if the  .abc file exists
   info.hasAlembicFile = alembicExists(strGeomFile);
   
   if(!info.hasAlembicFile)
   {
      // Replace namespace ":" invalid caracters in paths for "__ns__"
      size_t pos = 2;
      while( (pos = strGeomFile.find(":", pos)) != std::string::npos)
      {
         strGeomFile.replace(pos, 1, "__ns__");
         pos += 6;
      }
        
      info.hasAlembicFile = alembicExists(strGeomFile);
   }

   for( unsigned int i=0; i<info.vecPatches.size(); ++i )
   {
      const std::string& strPatch = info.vecPatches[i];

      // Create a nested procedural node
      AtNode* shape;
      

      // First patch, we use current procedural, and create the shaders
      if( i==0 )
      {
         shape = procedural;
         //AiNodeSetPtr( instance, "node", shape );

         // Export shaders
         rootShader = ExportRootShader(shape);


         // Only exporting matrix for maya < 2017 (#2681)
#if MAYA_API_VERSION < 201700
         ExportMatrix(shape);
#endif
      }
      // For other patches we reuse the shaders and create new procedural
      else
      {
         MString nameKey = "proc";
         nameKey += i;
         // we store this procedural in our translator using a key based on its index.
         // This way it'll be properly cleared or re-used later in the IPR session
         shape = GetArnoldNode(nameKey.asChar());
         if (shape == NULL)
            shape = AddArnoldNode("xgen_procedural", nameKey.asChar());

         AiNodeDeclare( shape, "xgen_shader", "constant ARRAY NODE" );
         AiNodeSetArray(shape, "xgen_shader", AiArray(1, 1, AI_TYPE_NODE, rootShader));

         /*AtNode* otherInstance = GetArnoldNode(nameKey.asChar());
         if (otherInstance == NULL) otherInstance = AddArnoldNode("ginstance", nameKey.asChar());
         AiNodeSetStr(otherInstance, "name", NodeUniqueName(otherInstance, buf));
         AiNodeSetPtr( otherInstance, "node", shape );
         AiNodeSetPtr( otherInstance, "shader", rootShader );

         ExportMatrix(otherInstance, 0);
         ProcessRenderFlags(otherInstance);*/
      }

      AiNodeSetStr(shape, "name", NodeUniqueName(shape, buf));
      
      ProcessRenderFlags(shape);

      //AiNodeSetPtr( shape, "shader", rootShader );

   //  MOTION BLUR COMPUTATION STUFF 

   /// TODO THINK MORE: in the GUI, LIVE mode seems to rely on  ENV variable  MI_MAYA_BATCH
   ///             if it does not exist it tries to use whats "cached" in the xgen data blob inside maya.
   ///             We need to switch here for XGEN ui setting, existance of alembic file, and  maya UI vs batch render
   ///             If at all possible we want to use the alembic geo if it exists which will allow for motion blur if desired
   ///            Fallback order is as follows in the comments:



      std::string mbSamplesString;
      MFloatArray steps;
      bool batchModeOk = false;
      
      // if maya session is in batch or  xgen render mode is batch we want to 
      if(GetSessionOptions().IsBatch() || (info.renderMode == 3))
      {
         batchModeOk = true;
      }

      // check if we don't have an alembic
      if (!info.hasAlembicFile) // we don't have the alembic
      {
         // Only print the error if motion blur is enabled
         if (info.moblur != 2 && info.motionBlurSteps > 1 && info.moblurFactor > 0.0f)
         {
            AiMsgError("[xgen] Can't motion blur, alembic file '%s' has not been exported", strGeomFile.c_str());
         }

         info.moblur = 2; // turning off xgen motion blur
         info.renderMode = 1; // set to live mode  for good measure
         batchModeOk = false;
      }
      
      if (batchModeOk)
      {
         AiMsgInfo("[xgen] All batch mode tests passed! proceeding with Xgeneration... using alembic patch cache");
         SetEnv("MI_MAYA_BATCH", "1");  // this is apparently the magic that forces xgen to batch mode vs live
      }
      else
      {
         AiMsgWarning("[xgen] Batch mode tests failed! proceeding with Xgeneration... using live scene data");
      }
      
      // if motion blur is enabled
      if (info.moblur != 2 && info.motionBlurSteps > 1 && info.moblurFactor > 0.0f)
      {

         if (info.moblur == 0) // use render globals
         {

            AiNodeDeclare( shape, "time_samples", "constant ARRAY FLOAT");
            AtArray* samples = AiArrayAllocate( info.motionBlurSteps, 1, AI_TYPE_FLOAT );
            
            unsigned int motionFramesCount;
            const double *steps = GetMotionFrames(motionFramesCount);
            if (steps != NULL && motionFramesCount > 0)
            {
            
               for (uint sampCount = 0; sampCount < info.motionBlurSteps; sampCount ++)
               {
                  float sample = float(steps[sampCount] - GetExportFrame());

                  // If 0.0 used as start step. XGen will not refresh next mb changes until frame is changed
                  if(sampCount == 0 && sample == 0.0f)
                     sample = 0.0001f;
                  
                  sprintf(buf,"%f",sample);
                  mbSamplesString += std::string(buf) + " ";
                  
                  AiArraySetFlt(samples, sampCount, sample);
               }
               AiNodeSetArray(shape, "time_samples", samples);
            }
         }
         
         else // xgen blur on
         {
            MFloatArray steps;
            float stepSize = info.moblurFactor/(info.motionBlurSteps-1);
            
            AiNodeDeclare( shape, "time_samples", "constant ARRAY FLOAT");
            AtArray* samples = AiArrayAllocate( info.motionBlurSteps, 1, AI_TYPE_FLOAT );

            for (uint stepCount = 0; stepCount < info.motionBlurSteps; stepCount++)
            {
               if (info.moblurmode == 0)
                  steps.append(float(0.0001 + (stepSize*stepCount))); // If 0.0 used as start step. XGen will not refresh next mb changes
               else if (info.moblurmode == 1)
                  steps.append(float((0.0 - (info.moblurFactor/2.0)) + (stepSize*stepCount)));
               else
                  steps.append(float((0.0 - info.moblurFactor) + (stepSize*stepCount)));
            }

            for (uint sampCount = 0; sampCount < info.motionBlurSteps; sampCount ++)
            {
               float sample = steps[sampCount];
               
               sprintf(buf,"%f",sample);
               mbSamplesString += std::string(buf) + " ";
               
               AiArraySetFlt(samples, sampCount, sample);
            }
            AiNodeSetArray(shape, "time_samples", samples);
         }
      }
      // if motion blur is disabled
      else
      {
         mbSamplesString += std::string("0.0 ");
      }

      /// TODO XGEN:  make these args real  arnold arguments and make the procedural  build this string itself from the passed arguments 

      // Set the procedural arguments
      {
         // Build the data argument
         std::string strData;
         strData =  "-debug 1 -warning 1 -stats 1 ";
         sprintf(buf,"%f ",GetExportFrame());
         strData += " -frame "+ std::string(buf);// +" -shutter 0.0"; // Pedro's suggestion was to remove the shutter value, it seemed not to make a diff
         std::string filePallete = info.strPalette;
         std::string stringPatch = strPatch;
         
         size_t pos = 0;
         
         // In XGen file, namespace symbol ":" is replced by "__"
         while( (pos = filePallete.find(":", pos)) != std::string::npos)
         {
            filePallete.replace(pos, 1, "__");
            pos += 2;
         }
         
         // Namespace is a feature in Maya and .xgen file doesn't contain any
         // namespace information. When translating xgen nodes, we need to
         // extract the namespace from the node name and pass the namespace
         // to xgen by using -nameSpace flag.
         // i.e. Maya ns:name -> XGen -namespace ns: -palette name
         {
            // Namespace of xgen nodes. XGen only allows to use one namespace
            // for all nodes in the same collection.
            std::string ns;

            // Get the namespace from the palette node
            pos = info.strPalette.rfind(":");
            if (pos != std::string::npos)
               ns = info.strPalette.substr(0, pos);

            // Check the namespace of the patch node
            pos = stringPatch.rfind(":");
            if (pos != std::string::npos && ns != stringPatch.substr(0, pos))
               AiMsgWarning("[xgen] Patch %s has a different namespace than collection %s",
                    stringPatch.c_str(), info.strPalette.c_str());
               
            // Check the namespace of the description node
            pos = info.strDescription.rfind(":");
            if (pos != std::string::npos && ns != info.strDescription.substr(0, pos))
               AiMsgWarning("[xgen] Description %s has a different namespace than collection %s",
                    info.strDescription.c_str(), info.strPalette.c_str());

            // Add -nameSpace flag using the namespace from the palette
            if (!ns.empty())
            {
                strData += " -nameSpace " + ns;
            }
         }
         
         // Internally, XGen needs palette, description and patch without namespace
         pos = info.strPalette.rfind(":");
         if(pos != std::string::npos)
            info.strPalette.erase(0,pos + 1);
         
         pos = stringPatch.rfind(":");
         if(pos != std::string::npos)
            stringPatch.erase(0,pos + 1);
            
         pos = info.strDescription.rfind(":");
         if(pos != std::string::npos)
            info.strDescription.erase(0,pos + 1);
                 
         strData += " -file " + info.strScene + "__" + filePallete + ".xgen";
         strData += " -palette " + info.strPalette;
         
         // We only have to remove namespace character ':' if there is a patch cache file
         if(info.hasAlembicFile)
         {
            strData += " -geom " + strGeomFile;
         }
         else
         {
            strData += " -geom " + info.strScene + "__" + info.strPalette + ".abc";
         }
         
         strData += " -patch " + stringPatch;
         strData += " -description " + info.strDescription;
         MTime oneSec(1.0, MTime::kSeconds);
         float fps =  (float)oneSec.asUnits(MTime::uiUnit());
         sprintf(buf,"%f ",fps);
         strData += " -fps " + std::string(buf);
         strData += " -motionSamplesLookup "+ mbSamplesString;
         strData += " -motionSamplesPlacement "+ mbSamplesString;

         strData += strUnitConvMat;

#ifdef DEBUG_MTOA
         printf("strData=%s\n",strData.c_str() );
#endif
         // Set other arguments
         AiNodeSetStr( shape, "data", strData.c_str() );
         //AiNodeSetVec( shape, "min", info.fBoundingBox[0], info.fBoundingBox[1], info.fBoundingBox[2] );
         //AiNodeSetVec( shape, "max", info.fBoundingBox[3], info.fBoundingBox[4], info.fBoundingBox[5] );

         AiNodeDeclare( shape, "irRenderCam", "constant STRING" );
         AiNodeDeclare( shape, "irRenderCamFOV", "constant STRING" );
         AiNodeDeclare( shape, "irRenderCamXform", "constant STRING" );
         AiNodeDeclare( shape, "irRenderCamRatio", "constant STRING" );

         sprintf(buf,"%s,%f,%f,%f", info.bCameraOrtho? "true":"false", info.fCameraPos[0], info.fCameraPos[1], info.fCameraPos[2] );
         AiNodeSetStr( shape, "irRenderCam", buf );

         sprintf(buf,"%f,%f", info.fCameraFOV, info.fCameraFOV  / info.fCamRatio);
         AiNodeSetStr( shape, "irRenderCamFOV",buf );

         sprintf(buf,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
               info.fCameraInvMat[0],info.fCameraInvMat[1],info.fCameraInvMat[2],info.fCameraInvMat[3],
               info.fCameraInvMat[4],info.fCameraInvMat[5],info.fCameraInvMat[6],info.fCameraInvMat[7],
               info.fCameraInvMat[8],info.fCameraInvMat[9],info.fCameraInvMat[10],info.fCameraInvMat[11],
               info.fCameraInvMat[12],info.fCameraInvMat[13],info.fCameraInvMat[14],info.fCameraInvMat[15] );
         AiNodeSetStr( shape, "irRenderCamXform",buf );

         sprintf(buf,"%f", info.fCamRatio );
         AiNodeSetStr( shape, "irRenderCamRatio", buf );

         AiNodeDeclare( shape, "xgen_renderMethod", "constant STRING" );
         sprintf(buf,"%i",info.renderMode);
         AiNodeSetStr( shape, "xgen_renderMethod", buf );


         AiNodeDeclare( shape, "ai_mode", "constant INT");
         AiNodeSetInt(shape, "ai_mode", info.aiMode);

         AiNodeDeclare( shape, "ai_min_pixel_width", "constant FLOAT");
         AiNodeSetFlt(shape, "ai_min_pixel_width", info.aiMinPixelWidth);

         AiNodeDeclare( shape, "xgen_multithreading", "constant BOOL" );
         AiNodeSetBool ( shape, "xgen_multithreading", info.multithreading );
      }
      
      ExportLightLinking(shape);
   }

   m_exportedSteps.clear();


   // For now we're only expanding the procedurals during export if we are on an interactive render
   // (see ticket #2599). This way the arnold render doesn't have to gather XGen data, and IPR
   // can be updated while tweaking the XGen attributes
   if (GetSessionOptions().IsInteractiveRender())
   {
      // if there is no motion blur for this node, then we're good to expand the geometries
      bool hasMotion = IsMotionBlurEnabled() && RequiresMotionData();

      // If there is motion blur, we'll want to expand the procedural only
      // at the final motion step
      if (hasMotion)
      {
         m_exportedSteps.assign(GetNumMotionSteps(), false);
         m_exportedSteps[GetMotionStep()] = true;
      } else
         ExpandProcedural();
      
   }
}
void CXgDescriptionTranslator::ExportShaders()
{
   ExportRootShader(GetArnoldNode());
}

void CXgDescriptionTranslator::ExportMotion(AtNode* shape)
{
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

  // Only exporting matrix for maya < 2017 (#2681)
#if MAYA_API_VERSION < 201700
   ExportMatrix(shape);
#endif
   if (m_exportedSteps.empty())
      return;

   // If we're here, it means that we're on an interactive render
   // where the procedural has to be expanded during export

   m_exportedSteps[GetMotionStep()] = true; // motion step done

   // check if all motion steps have been exported
   for (size_t i = 0; i < m_exportedSteps.size(); ++i)
   {
      // a motion step is still missing. We'll expand later
      if (!m_exportedSteps[i])
         return;
   }

   // All motion steps have been exported, it's time to expand the procedural
   ExpandProcedural();
}

void CXgDescriptionTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "procedural");
   CShapeTranslator::MakeCommonAttributes(helper);
   CShapeTranslator::MakeMayaVisibilityFlags(helper);

   CAttrData data;
   
   // render mode  1 = live  3 = batch
   data.defaultValue.INT() = 1;
   data.name = "renderMode";
   data.shortName = "render_mode";
   helper.MakeInputInt ( data );

   data.defaultValue.INT() = 0;
   data.name = "motionBlurOverride";
   data.shortName = "motion_blur_override";
   helper.MakeInputInt ( data );

   MStringArray  enumNames;
   enumNames.append ( "Start On Frame" );
   enumNames.append ( "Center On Frame" );
   enumNames.append ( "End On Frame" );
   enumNames.append ( "Use RenderGlobals" );
   data.defaultValue.INT() = 3;
   data.name = "motionBlurMode";
   data.shortName = "motion_blur_mode";
   data.enums= enumNames;
   helper.MakeInputEnum ( data );

   data.defaultValue.INT() = 3;
   data.name = "motionBlurSteps";
   data.shortName = "motion_blur_steps";
   helper.MakeInputInt ( data );

   data.defaultValue.FLT() = 1.0;
   data.name = "motionBlurFactor";
   data.shortName = "motion_blur_factor";
   helper.MakeInputFloat ( data );

   data.defaultValue.FLT() = 1.0;
   data.name = "motionBlurMult";
   data.shortName = "motion_blur_mult";
   helper.MakeInputFloat ( data );

   data.defaultValue.FLT() = 0.0;
   data.name = "aiMinPixelWidth";
   data.shortName = "ai_min_pixel_width";
   helper.MakeInputFloat ( data );

   MStringArray  curveTypeEnum;
   curveTypeEnum.append ( "Ribbon" );
   curveTypeEnum.append ( "Thick" );
   data.defaultValue.INT() = 0;
   data.name = "aiMode";
   data.shortName = "ai_mode";
   data.enums= curveTypeEnum;
   helper.MakeInputEnum ( data );

   data.defaultValue.BOOL() = false;
   data.name = "aiUseAuxRenderPatch";
   data.shortName = "ai_use_aux_render_patch";
   helper.MakeInputBoolean ( data );
    
   data.defaultValue.STR() = AtString("");
   data.name = "aiAuxRenderPatch";
   data.shortName = "ai_batch_render_patch";
   helper.MakeInputString ( data );

   data.defaultValue.BOOL() = true;
   data.name = "aiMultithreading";
   data.shortName = "ai_multithreading";
   helper.MakeInputBoolean ( data );
}

AtNode* CXgDescriptionTranslator::ExportRootShader(AtNode* instance)
{
   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), 0);
   if (!shadingGroupPlug.isNull())
   {
      AtNode *rootShader = ExportConnectedNode(shadingGroupPlug);
      if (rootShader != NULL)
      {
         AiNodeDeclare( instance, "xgen_shader", "constant ARRAY NODE" );
         AiNodeSetArray(instance, "xgen_shader", AiArray(1, 1, AI_TYPE_NODE, rootShader));
         return rootShader;
      }
   }

   return NULL;
}

// this forces the refresh during IPR. All nodes have to be deleted
// which will happen in the Delete() function
void CXgDescriptionTranslator::RequestUpdate()
{
   SetUpdateMode(AI_RECREATE_NODE);
   CShapeTranslator::RequestUpdate();
}

// For now we're only expanding the procedurals during export if we are on an interactive render
// (see ticket #2599). This way the arnold render doesn't have to gather XGen data, and IPR
// can be updated while tweaking the XGen attributes
void CXgDescriptionTranslator::ExpandProcedural()
{
   if (m_expandedProcedural)
      return;

   s_bCleanDescriptionCache = true;
   AtNode *node = GetArnoldNode();
   m_expandedProcedural = new XGenArnold::ProceduralWrapper( new XGenArnold::Procedural(), false /* Won't do cleanup */ );
   m_expandedProcedural->Init( node );

#if MAYA_API_VERSION >= 201600
   MGlobal::executeCommand("xgmCache -clearPtexCache;");
#endif

   // FIXME verify if we need to do something about the procedural matrix ?

   // in theory we could simply delete the procedural node, but I'm afraid of the consequences it may
   // have if GetArnoldNode returns NULL. So for safety we're just disabling this node for now
   AiNodeSetDisabled(node, true);
   int i = 1;

   while(true)
   {
      MString nameKey = "proc";
      nameKey += i;
      AtNode * procNode = GetArnoldNode(nameKey.asChar());
      if (procNode == NULL)
         break;

      AiNodeSetDisabled(procNode, true);
      i++;
   }
}
