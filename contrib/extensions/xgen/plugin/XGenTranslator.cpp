#include "extension/Extension.h"
#include "utils/time.h"
#include "scene/MayaScene.h"

#include <maya/MFileObject.h>
#include <maya/MTime.h>
#include <maya/MGlobal.h>

#include "XGenTranslator.h"

#include "session/SessionOptions.h"


#include <string>
#include <vector>


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
   //AiMsgInfo("[CXgDescriptionTranslator] CreateArnoldNodes()");
   return AddArnoldNode("procedural");
}

void CXgDescriptionTranslator::Export(AtNode* instance)
{
   //AiMsgInfo("[CXgDescriptionTranslator] Exporting %s", GetMayaNodeName().asChar());
   Update(instance);
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

void CXgDescriptionTranslator::Update(AtNode* procedural)
{
   //AiMsgInfo("[CXgDescriptionTranslator] Update()");

   // Export the transform matrix
   //ExportMatrix(instance, 0);

   // Get the visibiliy and render flags set.
   //ProcessRenderFlags(instance);

   // Build the path to the procedural dso
   //string path = string(getenv("MTOA_EXTENSIONS_PATH"));
   //unsigned int pluginPathLength = path.length();
   //string basepath = path.substr(0,pluginPathLength-11);
   //string(getenv("MTOA_EXTENSIONS_PATH")) + string("/procedurals/libXgArnoldProcedural.so");
   static std::string strDSO = std::string(getenv("MTOA_PATH")) + std::string("/procedurals/xgen_procedural.so");
   //static string strDSO = basepath + string("/procedurals/xgen_procedural.so");
   //static string strDSO = string("C:/solidangle/mtoadeploy/2014/procedurals/xgen_procedural.dll");

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
      if(CMayaScene::GetArnoldSession() && CMayaScene::GetArnoldSession()->IsBatch())
      {
         int exists = 0;
            MGlobal::executeCommand("objExists defaultArnoldRenderOptions.mtoaOrigFileName", exists);
         if (exists == 1)
            MGlobal::executeCommand("getAttr \"defaultArnoldRenderOptions.mtoaOrigFileName\"", mstrCurrentScene);
      }

      MFileObject fo;
      fo.setRawFullName( mstrCurrentScene );

      strScenePath = fo.resolvedPath().asChar();
      strSceneFile = fo.resolvedName().asChar();
      strSceneName = strSceneFile.substr( 0, strSceneFile.size()-3 );
   }

#ifdef DEBUG_MTOA
   printf("strDSO=%s\n",strDSO.c_str() );
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
         info.strPalette = palDagPath.fullPathName().asChar();
         info.strPalette = info.strPalette.substr( 1 );
#ifdef DEBUG_MTOA
         printf("strPalette=%s\n",info.strPalette.c_str() );
#endif

         MDagPath descDagPath = m_dagPath;
         descDagPath.pop();
         info.strDescription = descDagPath.fullPathName().asChar();
         info.strDescription = info.strDescription.substr( 1 + info.strPalette.size() + 1 );
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

            strChild = strChild.substr( 1+ info.strPalette.size() + 1 + info.strDescription.size() + 1 );

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

               //  use render globals moblur settings
               if (info.moblur == 0)
               {
                  if(CMayaScene::GetArnoldSession() && CMayaScene::GetArnoldSession()->IsMotionBlurEnabled(MTOA_MBLUR_OBJECT))
                  {
                     info.motionBlurSteps = CMayaScene::GetArnoldSession()->GetMotionFrames().size();
                     info.moblurFactor = float(CMayaScene::GetArnoldSession()->GetMotionByFrame());
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
      info.bCameraOrtho = false;
      info.setCameraPos( -48.4233f, 29.8617f, -21.2033f );
      info.fCameraFOV = 54.432224f;
      info.setCameraInvMat( -0.397148f,0.446873f,0.80161f,0.f,5.55112e-17f,0.873446f,-0.48692f,0.f,0.917755f,0.193379f,0.346887f,0.f,0.228188f,-0.343197f,60.712f,1.f );
      info.fCamRatio = 1.0f;
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
      int pos;
      while( (pos = strGeomFile.find(":")) != std::string::npos)
         strGeomFile.replace(pos, 1, "__ns__");
         
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
         rootShader = ExportShaders( shape );

         ExportMatrix(shape, 0);
      }
      // For other patches we reuse the shaders and create new procedural
      else
      {
         shape = AiNode("procedural");

         AiNodeDeclare( shape, "xgen_shader", "constant ARRAY NODE" );
         AiNodeSetArray(shape, "xgen_shader", AiArray(1, 1, AI_TYPE_NODE, rootShader));

         /*AtNode* otherInstance = AiNode("ginstance");
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
      if((CMayaScene::GetArnoldSession() && CMayaScene::GetArnoldSession()->IsBatch()) || (info.renderMode == 3))
      {
         batchModeOk = true;
      }

      // check if we don't have an alembic
      if (!info.hasAlembicFile) // we don't have the alembic
      {
         AiMsgError("[XGEN]: CAN'T MOTION BLUR,  alembic file-> %s  has not been exported", strGeomFile.c_str());
         info.moblur = 2; // turning off xgen motion blur
         info.renderMode = 1; // set to live mode  for good measure
         batchModeOk = false;
      }
      
      if (batchModeOk)
      {
         AiMsgInfo("[XGEN]: All batch mode tests passed! proceeding with Xgeneration... using alembic patch cache");
         SetEnv("MI_MAYA_BATCH", "1");  // this is apparently the magic that forces xgen to batch mode vs live
      }
      else
      {
         AiMsgWarning("[XGEN]: Batch mode tests failed! proceeding with Xgeneration... using live scene data");
      }
      
      // if motion blur is enabled
      if (info.moblur != 2 && info.motionBlurSteps > 1 && info.moblurFactor > 0.0f)
      {

         if (info.moblur == 0) // use render globals
         {
            if(CMayaScene::GetArnoldSession())
            {
               AiNodeDeclare( shape, "time_samples", "constant ARRAY FLOAT");
               AtArray* samples = AiArrayAllocate( info.motionBlurSteps, 1, AI_TYPE_FLOAT );
               
               std::vector<double> steps = CMayaScene::GetArnoldSession()->GetMotionFrames();
            
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
            else
            {
               AiMsgWarning("[XGEN]: Motion blur sample settings cannot be aquired from Arnold Render Globals");
               mbSamplesString += std::string("0.0 ");
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
		 strData += " -file " + info.strScene + "__" + info.strPalette + ".xgen";
         strData += " -palette " + info.strPalette;
         strData += " -geom " + strGeomFile;
         strData += " -patch " + strPatch;
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
         AiNodeSetBool( shape, "load_at_init", true );
         AiNodeSetStr( shape, "dso", strDSO.c_str() );
         AiNodeSetStr( shape, "data", strData.c_str() );
         AiNodeSetPnt( shape, "min", info.fBoundingBox[0], info.fBoundingBox[1], info.fBoundingBox[2] );
         AiNodeSetPnt( shape, "max", info.fBoundingBox[3], info.fBoundingBox[4], info.fBoundingBox[5] );

         AiNodeDeclare( shape, "irRenderCam", "constant STRING" );
         AiNodeDeclare( shape, "irRenderCamFOV", "constant STRING" );
         AiNodeDeclare( shape, "irRenderCamXform", "constant STRING" );
         AiNodeDeclare( shape, "irRenderCamRatio", "constant STRING" );

         sprintf(buf,"%s,%f,%f,%f", info.bCameraOrtho? "true":"false", info.fCameraPos[0], info.fCameraPos[1], info.fCameraPos[2] );
         AiNodeSetStr( shape, "irRenderCam", buf );

         sprintf(buf,"%f", info.fCameraFOV );
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
       

      }
   }
}

void CXgDescriptionTranslator::ExportMotion(AtNode* shape, unsigned int step)
{
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(shape, step);
}

void CXgDescriptionTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "procedural");
   CShapeTranslator::MakeCommonAttributes(helper);
   CShapeTranslator::MakeMayaVisibilityFlags(helper);

   CAttrData data;
   
   // render mode  1 = live  3 = batch
   data.defaultValue.INT = 1;
   data.name = "renderMode";
   data.shortName = "render_mode";
   helper.MakeInputInt ( data );

    data.defaultValue.INT = 0;
    data.name = "motionBlurOverride";
    data.shortName = "motion_blur_override";
    helper.MakeInputInt ( data );

	MStringArray  enumNames;
    enumNames.append ( "Start On Frame" );
    enumNames.append ( "Center On Frame" );
    enumNames.append ( "End On Frame" );
	enumNames.append ( "Use RenderGlobals" );
    data.defaultValue.INT = 3;
    data.name = "motionBlurMode";
    data.shortName = "motion_blur_mode";
    data.enums= enumNames;
    helper.MakeInputEnum ( data );

	data.defaultValue.INT = 3;
    data.name = "motionBlurSteps";
    data.shortName = "motion_blur_steps";
    helper.MakeInputInt ( data );

    data.defaultValue.FLT = 1.0;
    data.name = "motionBlurFactor";
    data.shortName = "motion_blur_factor";
    helper.MakeInputFloat ( data );

    data.defaultValue.FLT = 1.0;
    data.name = "motionBlurMult";
    data.shortName = "motion_blur_mult";
    helper.MakeInputFloat ( data );

	data.defaultValue.FLT = 0.0;
	data.name = "aiMinPixelWidth";
	data.shortName = "ai_min_pixel_width";
	helper.MakeInputFloat ( data );

	MStringArray  curveTypeEnum;
    curveTypeEnum.append ( "Ribbon" );
    curveTypeEnum.append ( "Thick" );
    data.defaultValue.INT = 0;
    data.name = "aiMode";
    data.shortName = "ai_mode";
    data.enums= curveTypeEnum;
    helper.MakeInputEnum ( data );

   data.defaultValue.BOOL = false;
   data.name = "aiUseAuxRenderPatch";
   data.shortName = "ai_use_aux_render_patch";
   helper.MakeInputBoolean ( data );
    
   data.defaultValue.STR = "";
   data.name = "aiAuxRenderPatch";
   data.shortName = "ai_batch_render_patch";
   helper.MakeInputString ( data );
}

AtNode* CXgDescriptionTranslator::ExportShaders(AtNode* instance)
{
   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), 0);
   if (!shadingGroupPlug.isNull())
   {
      AtNode *rootShader = ExportNode(shadingGroupPlug);
      if (rootShader != NULL)
      {
         AiNodeDeclare( instance, "xgen_shader", "constant ARRAY NODE" );
         AiNodeSetArray(instance, "xgen_shader", AiArray(1, 1, AI_TYPE_NODE, rootShader));
         return rootShader;
      }
   }

   return NULL;
}
