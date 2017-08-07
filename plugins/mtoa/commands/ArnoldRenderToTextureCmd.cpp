#include "ArnoldRenderToTextureCmd.h"
#include "../scene/MayaScene.h"
#include "../extension/ExtensionsManager.h"
#include "../translators/shader/ShaderTranslator.h"
#include "../utils/BuildID.h"
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
#include <maya/MItDependencyNodes.h>
#include <maya/MProgressWindow.h>

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
   syntax.addFlag("nof", "normal_offset", MSyntax::kDouble);
   syntax.addFlag("af", "filter", MSyntax::kString);
   syntax.addFlag("afw", "filter_width", MSyntax::kDouble);
   syntax.addFlag("aud", "all_udims", MSyntax::kBoolean);
   syntax.addFlag("ud", "udims", MSyntax::kString);
   syntax.addFlag("aov", "enable_aovs", MSyntax::kBoolean);

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

void splitString(const char* str, char delim, std::vector<std::string>& strings)
{
   std::istringstream f(str);
   std::string s;
   while (std::getline(f, s, delim))
   {
      strings.push_back(s);
   }
}

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

   // only display progressBar in Interactive sessions
   // or in batch it might fail
   bool progressBar = (MGlobal::mayaState() == MGlobal::kInteractive);


   AiBegin();
   CMayaScene::Begin(MTOA_SESSION_BATCH);
   CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();
   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   renderSession->SetForceTranslateShadingEngines(true);
   arnoldSession->SetExportFilterMask(AI_NODE_ALL);

   bool enableAovs = false;
   if (argDB.isFlagSet("enable_aovs")) argDB.getFlagArgument("enable_aovs", 0, enableAovs);

   // Set a default export camera
   // This is currently needed because the AOVs are set after we call SetExportCamera
   // and here we don't have any cam
   if (enableAovs)
   {
      MItDag dagIter(MItDag::kDepthFirst, MFn::kCamera);
      MDagPath cameraPath;
      dagIter.getPath(cameraPath);
      arnoldSession->SetExportCamera(cameraPath);
   }

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
            }
            else
            {
               CNodeTranslator *translator = arnoldSession->ExportNode(outColorPlug, NULL, 0, false, -1, &outColorStatus);
               if (translator != 0)
               {
                  shaderNode = translator->GetArnoldNode();
               }
               else
               {
                  std::string errLog = "[mtoa] Render to Texture : Mtoa Translator not found for ";
                  errLog += shaderName.asChar();
                  MGlobal::displayError(errLog.c_str());
               }
            }

         }
         else
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
   AiNodeSetPtr(options_node, "color_manager", NULL);

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
      MGlobal::displayWarning(errLog.asChar());
      filterNode = AiNode("gaussian_filter");
   }

   static std::string filterName("defaultArnoldFilter@cameraMapperFilter");
   AiNodeSetStr(filterNode, "name", filterName.c_str());

   double filterWidth = 2.0f;
   if (argDB.isFlagSet("filter_width")) argDB.getFlagArgument("filter_width", 0, filterWidth);
   AiNodeSetFlt(filterNode, "width", (float)filterWidth);

   double normalOffset = 0.1;
   if (argDB.isFlagSet("normal_offset")) argDB.getFlagArgument("normal_offset", 0, normalOffset);

   // handle udims
   bool allUdims = false;
   if (argDB.isFlagSet("all_udims")) argDB.getFlagArgument("all_udims", 0, allUdims);
   std::set<std::pair<int, int> > udimsSet;
   if (!allUdims)
   {
      MString udimsStr;
      if (argDB.isFlagSet("udims"))
      {
         argDB.getFlagArgument("udims", 0, udimsStr);
      }
      // udims flag format: 0:1,1:2,9:2
      std::vector<std::string> udimsVec;
      splitString(udimsStr.asChar(), ',', udimsVec);
      for (size_t i = 0; i < udimsVec.size(); ++i)
      {
         std::vector<std::string> udim;
         splitString(udimsVec[i].c_str(), ':', udim);
         if (udim.size() == 2)
         {
            int u, v;
            std::istringstream(udim[0]) >> u;
            std::istringstream(udim[1]) >> v;
            //if (u>=0&&v>=0&&u<10&&v<10)
            udimsSet.insert(std::make_pair(u, v));
         }
      }
   }

   std::vector<std::string> outputsList;

   std::vector<AtNode*> aovDrivers;
   std::vector<std::string> aovNames;

   outputsList.push_back("RGBA RGBA defaultArnoldFilter@cameraMapperFilter defaultArnoldDriver@cameraMapperOutput");

   // create a driver that will write the output texture
   AtNode *driver = AiNode("driver_exr");
   static std::string uvMapperDriverName ("defaultArnoldDriver@cameraMapperOutput");

   AiNodeSetStr(driver, "name", uvMapperDriverName.c_str());

   AtArray *prevOutputs = AiNodeGetArray(options_node, "outputs");
   if (enableAovs && prevOutputs && AiArrayGetNumElements(prevOutputs) > 1)
   {
      MString mainFilter;
      // AOVs have to be added to my outputs list      
      for (unsigned int p = 0; p < AiArrayGetNumElements(prevOutputs); ++p)
      {
         MString aovElem = AiArrayGetStr(prevOutputs, p);
         MStringArray aovElemSplit;
         aovElem.split(' ', aovElemSplit);
         if (aovElemSplit.length() <= 1)
            continue;

         MString aovName = aovElemSplit[0];
         MString aovFilter = aovElemSplit[2];
         
         if (aovName == "RGBA")
         {
            mainFilter = aovFilter;
            continue;
         }
         if (aovName == "Z")
            continue; // Z not supported

         MString newAovElem = aovName;
         newAovElem += " ";
         newAovElem += aovElemSplit[1];

         newAovElem += " ";

         if (aovFilter == mainFilter)
            newAovElem += filterName.c_str();  // need to set the main filter that was chosen here
         else
            newAovElem += aovFilter;  // keep the existing filter

         newAovElem += " ";

         AtNode *aovDriver = AiNodeClone(driver);
         std::string aovDriverName = uvMapperDriverName;
         aovDriverName += ".";
         aovDriverName += aovName.asChar();

         AiNodeSetStr(aovDriver, "name", aovDriverName.c_str());
         newAovElem += aovDriverName.c_str();

         aovDrivers.push_back(aovDriver);
         aovNames.push_back(aovName.asChar());

         outputsList.push_back(newAovElem.asChar());

      }
   }

   AtArray *outputs = 0;

   if (progressBar)
   {
      AtNode* render_view = AiNode("progress_driver");
      AiNodeSetStr(render_view, "name", "progress_display");

      AtNode* driverFilterNode = AiNode("box_filter");
      AiNodeSetStr(driverFilterNode, "name", "progress_driver_filter");

      outputsList.push_back("Z FLOAT progress_driver_filter progress_display");      
   }

   outputs = AiArrayAllocate(outputsList.size(), 1, AI_TYPE_STRING);

   for (size_t aov = 0; aov < outputsList.size(); ++aov)
      AiArraySetStr(outputs, aov, outputsList[aov].c_str());

   // assign it to the render options
   AiNodeSetArray(options_node, "outputs", outputs);

   MString mayaVersion = MGlobal::mayaVersion();     
   MString appString = MString("MtoA ") + MTOA_VERSION + " " + BUILD_ID + " Maya " + mayaVersion;
   AiSetAppString(appString.asChar());

   // Dirty hack... this is initializing all the polymeshes triangles
   // which is necessary for CameraUvMapper to work correctly
   AiRender(AI_RENDER_MODE_FREE);
   AiRenderAbort();

   std::vector<AtNode*> nodes;
   static const AtString polymesh_str("polymesh");
   // convert list of Maya selection to list of AtNodes selection
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

      AtNode* node = AiNodeLookUpByName(meshName.asChar());
      if (node == NULL)
      {
         const char *arnoldName = arnoldSession->GetArnoldObjectName(meshName.asChar());
         if ((arnoldName != NULL) && (arnoldName[0] != '\0'))
         {
            node = AiNodeLookUpByName(arnoldName);
         }
      }
      if (node == NULL)
      {
         std::string errorLog = "[mtoa] Render to Texture : Object ";
         errorLog += meshName.asChar();
         errorLog += " not exported to Arnold world";
         MGlobal::displayError(errorLog.c_str());
         continue;
      }

      const char *typeName = AiNodeEntryGetName(AiNodeGetNodeEntry(node));
      if(strcmp(typeName, "procedural") == 0) 
      {
         // this is a procedural node...
         // need to get all the children nodes

         // we loop over the entire Arnold Scene, and check which have this node as parent
         AtNodeIterator* nodeIter = AiUniverseGetNodeIterator(AI_NODE_SHAPE);
   
         while (!AiNodeIteratorFinished(nodeIter))
         {
            AtNode *loopNode = AiNodeIteratorGetNext(nodeIter);
            if (loopNode == NULL) continue;

            AtNode *rootNode = loopNode;
            AtNode *parentNode = AiNodeGetParent(rootNode);
            while(parentNode)
            {
               rootNode = parentNode;
               parentNode = AiNodeGetParent(rootNode);
            }
            // test if the root parent node is the one I'm treating
            if (rootNode != node) continue;
            
            if (AiNodeIs(loopNode, polymesh_str) )
            {
               nodes.push_back(loopNode);
            }
         }
         AiNodeIteratorDestroy(nodeIter);

      } else if (AiNodeIs(node, polymesh_str))
      {
         nodes.push_back(node);
      } else
      {         
         MString errLog = "[mtoa] Render to Texture : ";
         errLog += typeName;
         errLog += " nodes are not supported types";
         MGlobal::displayError(errLog);
      }
   }

   for (size_t i = 0; i < nodes.size(); ++i)
   {
      AtNode *mesh = nodes[i];
      const char *meshName = AiNodeGetName(mesh);

      if (progressBar)
      {
         if (i == 0)
         {
            MProgressWindow::reserve();
            MProgressWindow::setProgressRange(0, 100);
            MProgressWindow::setTitle("Rendering to Texture");
            MProgressWindow::setInterruptable(true);
         }
         if (MProgressWindow::isCancelled()) break;

         MString progressStatus = AiNodeGetName(mesh);
         progressStatus += " (";
         progressStatus += (unsigned int)(i + 1);
         progressStatus += "/";
         progressStatus += (unsigned int)nodes.size();
         progressStatus += ")";
         MProgressWindow::setProgressStatus(progressStatus);

         if (i == 0)
         {
            MProgressWindow::startProgress();
            // strange, but I need to change the value once so that it is displayed
            MProgressWindow::setProgress(1);
            MProgressWindow::setProgress(0);
         }
         else
         {
            MProgressWindow::setProgress(i * 100 / nodes.size());
         }
      }
      
      // specific shader to be assigned to geometry
      if (shaderNode)
      {
         AiNodeSetPtr(mesh, "shader", shaderNode);
      }
      // get assigned shader name
      const char* shader_name = NULL;
      {
         AtNode* shader = (AtNode*)AiNodeGetPtr(mesh, "shader");
         if (shader)
            shader_name = AiNodeGetName(shader);
      }

      MGlobal::displayInfo(MString("[mtoa] Render to Texture : Rendering polymesh ") + MString(meshName));

      std::string meshNameStr = meshName;
      std::replace( meshNameStr.begin(), meshNameStr.end(), ':', '_'); // replace all ':' to '_'
      std::replace( meshNameStr.begin(), meshNameStr.end(), '/', '_'); // replace all '/' to '_'

      std::string shaderNameStr;
      if (shader_name)
      {
         shaderNameStr = shader_name;
         std::replace( shaderNameStr.begin(), shaderNameStr.end(), ':', '_'); // replace all ':' to '_'
         std::replace( shaderNameStr.begin(), shaderNameStr.end(), '/', '_'); // replace all '/' to '_'
      }
      AtByte sidedness = AiNodeGetByte(mesh, "sidedness");
      // remove camera sidedness since we're offsetting towards the normal's direction
      // this could remove some artefacts caused by the normal offset
      sidedness &= ~AI_RAY_CAMERA; 
      AiNodeSetByte(mesh, "sidedness", sidedness);
      AiNodeSetBool(mesh, "opaque", true); // force opaque to true since a transparent material wouldn't work at all

      if (allUdims || udimsSet.size()>0)
      {

         AtArray* uv_list = AiNodeGetArray(mesh, "uvlist");

         /// find all affected udims
         if (allUdims)
         {
            for (size_t j = 0; j < AiArrayGetNumElements(uv_list); ++j)
            {
               AtVector2 uv = AiArrayGetVec2(uv_list, j);
               if (uv.x>AI_EPSILON && uv.y > AI_EPSILON&&
                  uv.x < 10 - AI_EPSILON && uv.y < 10 - AI_EPSILON)
                  udimsSet.insert(std::make_pair((int)floor(uv.x - AI_EPSILON), (int)floor(uv.y - AI_EPSILON)));
            }
         }
         for (std::set<std::pair<int, int> >::iterator it = udimsSet.begin(); it != udimsSet.end(); it++)
         {
            const int& u_offset = it->first;
            const int& v_offset = it->second;

            std::ostringstream ss_filename;
            ss_filename << folderName.asChar() << "/" << shaderNameStr << "_" << meshNameStr << "_" << 1000 + u_offset + 1 + v_offset * 10 << ".exr";

            // comment for mayabatch
            std::cout << "[mtoa] Render to Texture : UDIM " << u_offset << ":" << v_offset << " Rendered to " << ss_filename.str() << "\n";
            AtNode *camera = AiNode("MtoaCameraUvMapper");
            if (camera == 0)
            {
               AiEnd();
               MGlobal::displayError("[mtoa] Render to Texture : Couldn't create a CameraUvMapper node");
               return MS::kSuccess;
            }

            AiNodeSetStr(camera, "name", "cameraUvBaker");
            AiNodeSetStr(camera, "polymesh", meshName);
            AiNodeSetFlt(camera, "u_offset", -(float)u_offset);
            AiNodeSetFlt(camera, "v_offset", -(float)v_offset);
            AiNodeSetFlt(camera, "offset", (float)normalOffset);
            // need to adjust the near plane to make sure it's not bigger than the offset
            AiNodeSetFlt(camera, "near_plane", (float)AiMin(0.5*normalOffset, (double)AiNodeGetFlt(camera, "near_plane")));
            AiNodeSetPtr(options_node, "camera", camera);
            std::string filename = ss_filename.str();
            AiNodeSetStr(driver, "filename", filename.c_str());

            for (size_t aov = 0; aov < aovDrivers.size(); ++aov)
            {
               std::string aovFilename = filename.substr(0, filename.length() - 4) + "." + aovNames[aov] + ".exr";
               AiNodeSetStr(aovDrivers[aov], "filename", aovFilename.c_str());
            }

            AiRender();
            MGlobal::displayInfo(MString("[mtoa] Render to Texture : Rendered to ") + MString(filename.c_str()));

            AiNodeDestroy(camera);
         }
         if (allUdims)
         {
            udimsSet.clear();
         }
      }
      // render without udims
      else
      {
         AtNode *camera = AiNode("MtoaCameraUvMapper");
         if (camera == 0)
         {
            AiEnd();
            MGlobal::displayError("[mtoa] Render to Texture : Couldn't create a CameraUvMapper node");
            return MS::kSuccess;
         }
         
         MString filename = folderName + "/" + meshNameStr.c_str() + ".exr";

         AiNodeSetStr(camera, "name", "cameraUvBaker");
         AiNodeSetStr(camera, "polymesh", meshName);
         AiNodeSetFlt(camera, "offset", (float)normalOffset);
         // need to adjust the near plane to make sure it's not bigger than the offset
         AiNodeSetFlt(camera, "near_plane", (float)AiMin(0.5*normalOffset, (double)AiNodeGetFlt(camera, "near_plane")));
         AiNodeSetPtr(options_node, "camera", camera);
         AiNodeSetStr(driver, "filename", filename.asChar());

         for (size_t aov = 0; aov < aovDrivers.size(); ++aov)
         {
            MString aovFilename = filename.substring(0, filename.length() - 5);
            aovFilename +=  ".";
            aovFilename += aovNames[aov].c_str();
            aovFilename += ".exr";
            AiNodeSetStr(aovDrivers[aov], "filename", aovFilename.asChar());
         }

         AiRender();

         MGlobal::displayInfo(MString("[mtoa] Render to Texture : Rendered to ") + filename);

         AiNodeDestroy(camera);
      }
   }
   MProgressWindow::endProgress();

   AiEnd();

   return MS::kSuccess;
}

