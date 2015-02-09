#include "ArnoldRenderToTextureCmd.h"
#include "../scene/MayaScene.h"
#include "../extension/ExtensionsManager.h"
#include "../translators/shader/ShaderTranslator.h"
#include <maya/MStatus.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MFnSet.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnAttribute.h>
#include <maya/MStringArray.h>
#include <maya/MGLobal.h>
#include <maya/MArgDatabase.h>
#include <maya/MItDependencyNodes.h>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <ostream>
#include <ai.h>
#include <fstream>
#include <istream>
#include <streambuf>

CArnoldRenderToTextureCmd::CArnoldRenderToTextureCmd(){}

CArnoldRenderToTextureCmd::~CArnoldRenderToTextureCmd(){}

MSyntax CArnoldRenderToTextureCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("f", "folder", MSyntax::kString);
   syntax.addFlag("s", "shader", MSyntax::kString);
   syntax.addFlag("r", "resolution", MSyntax::kUnsigned);
   syntax.addFlag("as", "aa_samples", MSyntax::kUnsigned);
   syntax.addFlag("af", "filter", MSyntax::kString);
   syntax.addFlag("afw", "filter_width", MSyntax::kDouble);


   
   syntax.setObjectType(MSyntax::kStringObjects);
   return syntax;
}

void* CArnoldRenderToTextureCmd::creator()
{
   return new CArnoldRenderToTextureCmd();
}

struct matrixAsFloats {
   float elems[16];
};

MStatus CArnoldRenderToTextureCmd::doIt(const MArgList& argList)
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
      MGlobal::displayError("[mtoa] Render to Texture : No geometry selected");
      return MS::kSuccess;
   }


   MString folderName = "";
   if (argDB.isFlagSet("folder"))
   {
      argDB.getFlagArgument("folder", 0, folderName);
   }
     
   if (folderName == "")
   { 
      MGlobal::displayError("[mtoa] Render to Texture : no output directory specified");
      return MS::kSuccess;
   }   

   CMayaScene::End();
   // Cannot export while a render is active
   if (AiUniverseIsActive())
   {
      MGlobal::displayError("[mtoa] Render to Texture : a render is already in progress");
      return MS::kSuccess;
   }

   
   AiBegin();
   CMayaScene::Begin(MTOA_SESSION_BATCH);
   CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();
   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   renderSession->SetForceTranslateShadingEngines(true);   
   arnoldSession->SetExportFilterMask(AI_NODE_ALL);

   CMayaScene::Export(); 
   AtNode *shaderNode = 0;

   if (argDB.isFlagSet("shader"))
   {
      MString shaderName = "";
      argDB.getFlagArgument("shader", 0, shaderName);
      if (shaderName != "")
      {

         MSelectionList sel;
         sel.add(shaderName);
         if (sel.length() > 0)
         {
            MObject mat;
            sel.getDependNode(0, mat);         
            MFnDependencyNode depNodeMat(mat);
            
            MStatus outColorStatus = MS::kFailure;
            MPlug outColorPlug = depNodeMat.findPlug("outColor", true, &outColorStatus);
            if (outColorStatus != MS::kSuccess || outColorPlug.name() == "")
            {
               outColorPlug = depNodeMat.findPlug("output", true, &outColorStatus);
               
            }
            if (outColorStatus != MS::kSuccess || outColorPlug.name() == "") 
            {
               std::string errLog = "[mtoa] Render to Texture : Output attribute not found in ";
               errLog += shaderName.asChar();
               MGlobal::displayError(errLog.c_str());
            } else
            {
               CNodeTranslator *translator = arnoldSession->ExportNode(outColorPlug, NULL, 0, false, &outColorStatus);
               if (translator != 0)
               {
                  shaderNode = translator->GetArnoldRootNode();
               }
               else 
               {
                  std::string errLog = "[mtoa] Render to Texture : Mtoa Translator not found for ";
                  errLog += shaderName.asChar();
                  MGlobal::displayError(errLog.c_str());
               }
            } 
            
         } else
         {
            std::string errLog = "[mtoa] Render to Texture : Shader ";
            errLog += shaderName.asChar();
            errLog += " not found in the scene";
            MGlobal::displayError(errLog.c_str());
         }
      }
   }
   AtNode* options_node = AiUniverseGetOptions();
   AiNodeSetBool(options_node, "preserve_scene_data", true);

   int resolution = 512; // default value
   if (argDB.isFlagSet("resolution"))
      argDB.getFlagArgument("resolution", 0, resolution);
   else 
      MGlobal::displayWarning("[mtoa] Render to Texture : No resolution specified, using 512x512 by default");

   renderSession->SetResolution(resolution, resolution); 
   // I guess that setting the renderSession's resolution
   // is enough, but let's make sure....
   AiNodeSetInt(options_node, "xres", resolution);
   AiNodeSetInt(options_node, "yres", resolution);

   if (argDB.isFlagSet("aa_samples"))
   {
      int aa_samples = 1;
      argDB.getFlagArgument("aa_samples", 0, aa_samples);
      AiNodeSetInt(options_node, "AA_samples", aa_samples);
   }

   // create a filter
   MString filterType = "gaussian";
   if (argDB.isFlagSet("filter"))
   {
      argDB.getFlagArgument("filter", 0, filterType);
      filterType += "_filter";
   }
   AtNode *filterNode = AiNode(filterType.asChar());
   if (filterNode == 0)
   {
      MString errLog = "[mtoa] Render to Texture : Unknown filter type ";
      errLog += filterType;
      MGlobal::displayError(errLog.asChar());
      filterNode = AiNode("gaussian_filter");
   }
   AiNodeSetStr(filterNode, "name", "defaultArnoldFilter@cameraMapperFilter");

   int filterWidth = 2;
   if (argDB.isFlagSet("filter_width")) argDB.getFlagArgument("filter_width", 0, filterWidth);
   
   AiNodeSetInt(filterNode, "width", filterWidth);


   // create a driver that will write the output texture
   AtNode *driver = AiNode("driver_exr");
   AiNodeSetStr(driver, "name", "defaultArnoldDriver@cameraMapperOutput");
   

   AtArray *outputs = AiArray(1, 1, AI_TYPE_STRING,
      "RGBA RGBA defaultArnoldFilter@cameraMapperFilter defaultArnoldDriver@cameraMapperOutput" );
   // assign it to the render options
   AiNodeSetArray(options_node, "outputs", outputs);

   // Dirty hack... this is initializing all the polymeshes triangles
   // which is necessary for CameraUvMapper to work correctly
   AiRender(AI_RENDER_MODE_FREE);
   AiRenderAbort();

   for (unsigned int i = 0; i < selected.length(); ++i)  
   {
      MDagPath dagPath;
      if (selected.getDagPath(i, dagPath) == MS::kFailure) continue;

      // if the dagPath is a transform node, we actually want to consider its shape name
      if (dagPath.hasFn(MFn::kTransform) && dagPath.childCount() > 0) {
         dagPath.extendToShape();
      }
      MFnDagNode dagNode(dagPath);
      MString meshName = dagNode.partialPathName();
      

      // if export selection -> export current polymesh

      AtNode*input_object = AiNodeLookUpByName(meshName.asChar());
      if (input_object == 0)
      {
         std::string errorLog = "[mtoa] Render to Texture : Object ";
         errorLog += meshName.asChar();
         errorLog += " not exported to Arnold world";
         MGlobal::displayError(errorLog.c_str());
         continue;
      }

      std::string logMsg = "[mtoa] Render to Texture : Rendering polymesh ";
      logMsg += meshName.asChar();
      MGlobal::displayInfo(MString(logMsg.c_str()));

      MString filename = folderName + "/"+meshName+".exr";
      AiNodeSetStr(driver, "filename", filename.asChar());

      // dirty hack.... triggering the polymesh initialization by invoking
      // a render in free mode. Then we ask for a triangle information
      // so that subdivision and displacement is computed.
      // Then abort the free render and start real rendering.
      // Now prepare the real rendering.
      // Create the CameraUvMapper camera and assign it in the render options
      AtNode *camera = AiNode("cameraUvMapper");
      if (camera == 0) 
      {
         AiEnd();
         argDB.getFlagArgument("folder", 0, folderName);
         MGlobal::displayError("[mtoa] Render to Texture : Couldn't create a CameraUvMapper node");
         return MS::kSuccess;
      }

      AiNodeSetStr(camera, "name", "cameraUvBaker");
      AiNodeSetPtr(options_node, "camera", camera);
      AiNodeSetStr(camera, "polymesh", meshName.asChar());
      
      // specific shader to be assigned to geometry
      if(shaderNode)
      {
         AtNode *polymeshNode = AiNodeLookUpByName(meshName.asChar());
         if (polymeshNode)
            AiNodeSetPtr(polymeshNode, "shader", shaderNode);
      }
      

      // let's go !
      AiRender();
      std::string endLog = "[mtoa] Render to Texture : Rendered to ";
      endLog += filename.asChar();
      MGlobal::displayInfo(MString(endLog.c_str()));

      AiNodeDestroy(camera);
   }
   AiEnd();

   return MS::kSuccess;
}

