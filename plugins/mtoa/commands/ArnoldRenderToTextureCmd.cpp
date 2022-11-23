#include "ArnoldRenderToTextureCmd.h"
#include "../extension/ExtensionsManager.h"
#include "../translators/shader/ShaderTranslator.h"
#include "../utils/BuildID.h"
#include "../utils/ConstantStrings.h"
#include "utils/MtoAAdpPayloads.h"
#include "session/ArnoldSession.h"
#include "session/SessionManager.h"
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
#include <maya/MRenderUtil.h>


#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <ostream>
#include <ai.h>
#include <fstream>
#include <istream>
#include <streambuf>

static const std::string s_renderToTextureSessionId("renderToTexture");

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
   syntax.addFlag("uvs", "uv_set", MSyntax::kString);
   syntax.addFlag("ee", "extend_edges", MSyntax::kBoolean);
   syntax.addFlag("aov", "enable_aovs", MSyntax::kBoolean);
   syntax.addFlag("ust", "u_start", MSyntax::kDouble);
   syntax.addFlag("usc", "u_scale", MSyntax::kDouble);
   syntax.addFlag("vst", "v_start", MSyntax::kDouble);
   syntax.addFlag("vsc", "v_scale", MSyntax::kDouble);
   syntax.addFlag("seq", "sequence", MSyntax::kBoolean);
   syntax.addFlag("fst", "frame_start", MSyntax::kDouble);
   syntax.addFlag("fen", "frame_end", MSyntax::kDouble);
   syntax.addFlag("sst", "frame_step", MSyntax::kDouble);
   syntax.addFlag("sst", "frame_step", MSyntax::kDouble);
   syntax.addFlag("pad", "frame_padding", MSyntax::kUnsigned);
   syntax.addFlag("ur", "use_region", MSyntax::kBoolean);
   syntax.addFlag("mix", "region_min_x",  MSyntax::kLong);
   syntax.addFlag("miy", "region_min_y",  MSyntax::kLong);
   syntax.addFlag("max", "region_max_x",  MSyntax::kLong);
   syntax.addFlag("may", "region_max_y",  MSyntax::kLong);

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


   // only display progressBar in Interactive sessions
   // or in batch it might fail
   bool progressBar = (MGlobal::mayaState() == MGlobal::kInteractive);

   std::vector<double> frames;
   double currentFrame = 0.f;
   MString cmd = MString("currentTime -query");
   MGlobal::executeCommand(cmd, currentFrame);

   bool sequence = false;
   if (argDB.isFlagSet("sequence")) argDB.getFlagArgument("sequence", 0, sequence);
   int framePadding = 0;
   if (argDB.isFlagSet("frame_padding")) argDB.getFlagArgument("frame_padding", 0, framePadding);

   if (sequence)
   {
      double startFrame = currentFrame;
      if (argDB.isFlagSet("frame_start")) argDB.getFlagArgument("frame_start", 0, startFrame);
      double endFrame = currentFrame;
      if (argDB.isFlagSet("frame_end")) argDB.getFlagArgument("frame_end", 0, endFrame);

      double step = 1.;
      if (argDB.isFlagSet("frame_step")) argDB.getFlagArgument("frame_step", 0, step);

      endFrame = AiMax(endFrame, startFrame);
      for (double f = startFrame; f <= endFrame; f += step)
         frames.push_back(f);
   } else
   {
      frames.push_back(currentFrame);
   }

   for (size_t i = 0; i < frames.size(); ++i)
   {
      double frame = frames[i];

      CArnoldSession *session = new CArnoldSession();

      if (!CSessionManager::AddActiveSession(s_renderToTextureSessionId, session))
      {
         delete session;
         return MS::kFailure;
      }
      CSessionOptions &options = session->GetOptions();
      AtUniverse *universe = session->GetUniverse();
      options.SetForceTranslateShadingEngines(true);
      AtRenderSession *renderSession = session->GetRenderSession();


      double currentFrame = 0.f;
      MString cmd = MString("currentTime -query");
      MGlobal::executeCommand(cmd, currentFrame);

      if (currentFrame != frame)
      {
         MGlobal::viewFrame(frame);
         options.SetExportFrame(frame);
      }

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
         session->SetExportCamera(cameraPath);
      }

      session->Export();
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
                  CNodeTranslator *translator = session->ExportNode(outColorPlug, false, -1, &outColorStatus);
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
      AtNode* options_node = AiUniverseGetOptions(universe);
      AiNodeSetPtr(options_node, str::color_manager, NULL);

      int resolution = 512; // default value
      if (argDB.isFlagSet("resolution"))
         argDB.getFlagArgument("resolution", 0, resolution);
      else
         MGlobal::displayWarning("[mtoa] Render to Texture : No resolution specified, using 512x512 by default");

      options.SetResolution(resolution, resolution);
      // I guess that setting the renderSession's resolution
      // is enough, but let's make sure....
      AiNodeSetInt(options_node, str::xres, resolution);
      AiNodeSetInt(options_node, str::yres, resolution);

      bool useRegion = false;
      if (argDB.isFlagSet("use_region")) argDB.getFlagArgument("use_region", 0, useRegion);
      if (useRegion)
      {
         if (args.isFlagSet("region_min_x"))
         {
            int region_min_x = args.flagArgumentInt("region_min_x", 0);
            AiNodeSetInt(options_node, str::region_min_x, region_min_x);
         }
         if (args.isFlagSet("region_min_y"))
         {
            int region_min_y = args.flagArgumentInt("region_min_y", 0);
            AiNodeSetInt(options_node, str::region_min_y, region_min_y);
         }
         if (args.isFlagSet("region_max_x"))
         {
            int region_max_x = args.flagArgumentInt("region_max_x", 0);
            AiNodeSetInt(options_node, str::region_max_x, region_max_x);
         }
         if (args.isFlagSet("region_max_y"))
         {
            int region_max_y = args.flagArgumentInt("region_max_y", 0);
            AiNodeSetInt(options_node, str::region_max_y, region_max_y);
         }
      }

      if (argDB.isFlagSet("aa_samples"))
      {
         int aa_samples = 1;
         argDB.getFlagArgument("aa_samples", 0, aa_samples);
         AiNodeSetInt(options_node, str::AA_samples, aa_samples);
      }

      // create a filter
      MString filterType = "gaussian_filter";
      if (argDB.isFlagSet("filter"))
      {
         argDB.getFlagArgument("filter", 0, filterType);
         filterType += "_filter";
      }

      AtNode *filterNode = AiNode(universe, AtString(filterType.asChar()));
      if (filterNode == 0)
      {
         MString errLog = "[mtoa] Render to Texture : Unknown filter type ";
         errLog += filterType;
         MGlobal::displayWarning(errLog.asChar());
         filterNode = AiNode(universe, str::gaussian_filter);
      }

      static const AtString filterName("defaultArnoldFilter/cameraMapperFilter");
      AiNodeSetStr(filterNode, str::name, filterName);

      double filterWidth = 2.0;
      if (argDB.isFlagSet("filter_width")) argDB.getFlagArgument("filter_width", 0, filterWidth);
      AiNodeSetFlt(filterNode, str::width, (float)filterWidth);

      double normalOffset = 0.1;
      if (argDB.isFlagSet("normal_offset")) argDB.getFlagArgument("normal_offset", 0, normalOffset);

      double uStart = 0.0;
      if (argDB.isFlagSet("u_start")) argDB.getFlagArgument("u_start", 0, uStart);
      double uScale = 1.0;
      if (argDB.isFlagSet("u_scale")) argDB.getFlagArgument("u_scale", 0, uScale);
      double vStart = 0.0;
      if (argDB.isFlagSet("v_start")) argDB.getFlagArgument("v_start", 0, vStart);
      double vScale = 1.0;
      if (argDB.isFlagSet("v_scale")) argDB.getFlagArgument("v_scale", 0, vScale);
      bool extendEdges = false;
      if (argDB.isFlagSet("extend_edges")) argDB.getFlagArgument("extend_edges", 0, extendEdges);

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

      MString uvSet = "";
      if (argDB.isFlagSet("uv_set"))
         argDB.getFlagArgument("uv_set", 0, uvSet);

      std::vector<std::string> outputsList;

      std::vector<AtNode*> aovDrivers;
      std::vector<std::string> aovNames;

      outputsList.push_back("RGBA RGBA defaultArnoldFilter/cameraMapperFilter defaultArnoldDriver/cameraMapperOutput");

      // create a driver that will write the output texture
      static std::string uvMapperDriverName ("defaultArnoldDriver/cameraMapperOutput");
      AtNode *driver = AiNode(universe, str::driver_exr, AtString(uvMapperDriverName.c_str()));

      if (extendEdges)
         AiNodeSetBool(driver, str::tiled, false);

      AtArray *prevOutputs = AiNodeGetArray(options_node, str::outputs);
      if (enableAovs && prevOutputs && AiArrayGetNumElements(prevOutputs) > 1)
      {
         MString mainFilter;
         // AOVs have to be added to my outputs list
         for (unsigned int p = 0; p < AiArrayGetNumElements(prevOutputs); ++p)
         {
            AtString aovStr = AiArrayGetStr(prevOutputs, p);
            MString aovElem(aovStr.c_str());
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

            AiNodeSetStr(aovDriver, str::name, AtString(aovDriverName.c_str()));
            newAovElem += aovDriverName.c_str();

            aovDrivers.push_back(aovDriver);
            aovNames.push_back(aovName.asChar());

            outputsList.push_back(newAovElem.asChar());

         }
      }
      AtArray *outputs = AiArrayAllocate(outputsList.size(), 1, AI_TYPE_STRING);

      for (size_t aov = 0; aov < outputsList.size(); ++aov)
         AiArraySetStr(outputs, aov, AtString(outputsList[aov].c_str()));

      // assign it to the render options
      AiNodeSetArray(options_node, str::outputs, outputs);

      MString mayaVersion = MGlobal::mayaVersion();
      MString appString = MString("MtoA ") + MTOA_VERSION + " " + BUILD_ID + " Maya " + mayaVersion;
      AiSetAppString(appString.asChar());

      // We need to ensure that a render camera is set, otherwise subdivision might fail (#3264)
      AtNode *renderCam = (AtNode*)AiNodeGetPtr(options_node, str::camera);
      if (renderCam == NULL)
      {
         // Please don't tell anyone that I'm creating a dummy camera here,
         // it will be deleted at the end of this function anyway.
         renderCam = AiNode(universe, str::persp_camera, str::__mtoa_baking_cam);
         AiNodeSetPtr(options_node, str::camera, (void*)renderCam);
      }

      // Dirty hack... this is initializing all the polymeshes triangles
      // which is necessary for CameraUvMapper to work correctly
      AiRenderSetHintStr(renderSession, AI_ADP_RENDER_CONTEXT, AI_ADP_RENDER_CONTEXT_OTHER);
      AiRenderBegin(renderSession, AI_RENDER_MODE_FREE);
      while(AiRenderGetStatus(renderSession) != AI_RENDER_STATUS_PAUSED)
      {
         continue;
      }
      AiRenderEnd(renderSession);

      std::vector<AtNode*> nodes;
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
         MString meshName = options.GetArnoldNaming(dagPath);
         AtNode* node = AiNodeLookUpByName(universe, AtString(meshName.asChar()));

         if (node == NULL)
         {
            std::string errorLog = "[mtoa] Render to Texture : Object ";
            errorLog += meshName.asChar();
            errorLog += " not exported to Arnold world";
            MGlobal::displayError(errorLog.c_str());
            continue;
         }

         const AtNodeEntry *nodeEntry = AiNodeGetNodeEntry(node);
         const char *typeName = AiNodeEntryGetName(nodeEntry);

         if(AiNodeEntryGetDerivedType(nodeEntry) == AI_NODE_SHAPE_PROCEDURAL)
         {
            // this is a procedural node...
            // need to get all the children nodes

            // we loop over the entire Arnold Scene, and check which have this node as parent
            AtNodeIterator* nodeIter = AiUniverseGetNodeIterator(universe, AI_NODE_SHAPE);
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

               if (AiNodeIs(loopNode, str::polymesh) )
               {
                  nodes.push_back(loopNode);
               }
            }
            AiNodeIteratorDestroy(nodeIter);

         } else if (AiNodeIs(node, str::polymesh))
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

         std::string meshName = AiNodeGetName(mesh);
         std::string fullMeshName = meshName;

         if (AiNodeLookUpByName(universe, AtString(meshName.c_str())) == NULL)
         {
            // this name isn't enough to find the node in the scene.
            // We might need to set its full path name
            AtNode *parent = AiNodeGetParent(mesh);
            while(parent)
            {
               fullMeshName = std::string(AiNodeGetName(parent)) + std::string("^") + fullMeshName;
               parent = AiNodeGetParent(parent);
            }
         }

         // by how much should the progress increase for each node being rendered
         float progressNodeDelta = 100.f / (float)nodes.size();
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
               MProgressWindow::setProgress(int(i * progressNodeDelta));
            }
         }
         // specific shader to be assigned to geometry
         if (shaderNode)
         {
            AiNodeSetPtr(mesh, str::shader, shaderNode);
         }
         // get assigned shader name
         const char* shader_name = NULL;
         {
            AtNode* shader = (AtNode*)AiNodeGetPtr(mesh, str::shader);
            if (shader)
               shader_name = AiNodeGetName(shader);
         }

         std::string meshNameStr = meshName;
         if (AiNodeLookUpUserParameter(mesh, str::dcc_name))
         {
            // if the user data "dcc_name" exists, I want to use it for the output image filename
            meshNameStr = AiNodeGetStr(mesh, str::dcc_name);
         }
         MGlobal::displayInfo(MString("[mtoa] Render to Texture : Rendering polymesh ") + MString(meshNameStr.c_str()));

         std::replace( meshNameStr.begin(), meshNameStr.end(), ':', '_'); // replace all ':' to '_'
         std::replace( meshNameStr.begin(), meshNameStr.end(), '/', '_'); // replace all '/' to '_'
         std::replace( meshNameStr.begin(), meshNameStr.end(), '|', '_'); // replace all '|' to '_'

         std::string shaderNameStr;
         if (shader_name)
         {
            shaderNameStr = shader_name;
            std::replace( shaderNameStr.begin(), shaderNameStr.end(), ':', '_'); // replace all ':' to '_'
            std::replace( shaderNameStr.begin(), shaderNameStr.end(), '/', '_'); // replace all '/' to '_'
            std::replace( shaderNameStr.begin(), shaderNameStr.end(), '|', '_'); // replace all '|'to '_'
         }
         AtByte sidedness = AiNodeGetByte(mesh, str::sidedness);
         // remove camera sidedness since we're offsetting towards the normal's direction
         // this could remove some artefacts caused by the normal offset
         sidedness &= ~AI_RAY_CAMERA;
         AiNodeSetByte(mesh, str::sidedness, sidedness);
         AiNodeSetBool(mesh, str::opaque, true); // force opaque to true since a transparent material wouldn't work at all

         if (allUdims || udimsSet.size()>0)
         {

            AtArray* uv_list = AiNodeGetArray(mesh, str::uvlist);

            /// find all affected udims
            if (allUdims)
            {
               for (size_t j = 0; j < AiArrayGetNumElements(uv_list); ++j)
               {
                  // U coords can't be higher than 9, V coords can be up to 99 (#3453)
                  AtVector2 uv = AiArrayGetVec2(uv_list, j);
                  if (uv.x>AI_EPSILON && uv.y > AI_EPSILON&&
                     uv.x < 10 - AI_EPSILON && uv.y < 100 - AI_EPSILON)
                     udimsSet.insert(std::make_pair((int)floor(uv.x - AI_EPSILON), (int)floor(uv.y - AI_EPSILON))); 
               }
            }
            for (std::set<std::pair<int, int> >::iterator it = udimsSet.begin(); it != udimsSet.end(); it++)
            {
               const int& u_offset = it->first;
               const int& v_offset = it->second;

               std::ostringstream ss_filename;
               ss_filename << folderName.asChar() << "/" << shaderNameStr << "_" << meshNameStr << "_" << 1000 + u_offset + 1 + v_offset * 10;
               std::string baseFilename = ss_filename.str();

               if (sequence)
               {
                  char paddingFrame[64];
                  sprintf(paddingFrame, "%0*d", framePadding, (int) frame);
                  ss_filename << "."<<paddingFrame;
               }
               std::string filename = ss_filename.str() + ".exr";


               // comment for mayabatch
               std::cout << "[mtoa] Render to Texture : UDIM " << u_offset << ":" << v_offset << " Rendered to " << ss_filename.str() << "\n";
               AtNode *camera = AiNode(universe, str::uv_camera, str::cameraUvBaker);
               if (camera == 0)
               {
                  CSessionManager::DeleteActiveSession(s_renderToTextureSessionId);
                  MGlobal::displayError("[mtoa] Render to Texture : Couldn't create a uv_camera node");
                  return MS::kSuccess;
               }


               AiNodeSetPtr(camera, str::mesh, (void*) mesh);
               AiNodeSetStr(camera, str::uv_set, AtString(uvSet.asChar()));
               AiNodeSetFlt(camera, str::u_offset, (float)(-u_offset -uStart));
               AiNodeSetFlt(camera, str::v_offset, (float)(-v_offset -vStart));

               AiNodeSetFlt(camera, str::u_scale, (float)(1. / AiMax((float)uScale, AI_EPSILON)));
               AiNodeSetFlt(camera, str::v_scale, (float)(1. / AiMax((float)vScale, AI_EPSILON)));
               AiNodeSetBool(camera, str::extend_edges, extendEdges);
               AiNodeSetFlt(camera, str::offset, (float)normalOffset);
               // need to adjust the near plane to make sure it's not bigger than the offset
               AiNodeSetFlt(camera, str::near_clip, (float)AiMin(0.5*normalOffset, (double)AiNodeGetFlt(camera, str::near_clip)));
               AiNodeSetPtr(options_node, str::camera, camera);

               AiNodeSetStr(driver, str::filename, AtString(filename.c_str()));

               MString filenameStr(filename.c_str());

               for (size_t aov = 0; aov < aovDrivers.size(); ++aov)
               {
                  MString aovFilename = baseFilename.c_str();
                  aovFilename += ".";
                  aovFilename += aovNames[aov].c_str();
                  if (sequence)
                  {
                     char paddingFrame[64];
                     sprintf(paddingFrame, "%0*d", framePadding, (int) frame);
                     aovFilename += ".";
                     aovFilename +=paddingFrame;
                  }

                  aovFilename += ".exr";
                  AiNodeSetStr(aovDrivers[aov], str::filename, AtString(aovFilename.asChar()));
               }
               AiRenderSetHintStr(renderSession, AI_ADP_RENDER_CONTEXT, AI_ADP_RENDER_CONTEXT_OTHER);

               if (progressBar)
                  MRenderUtil::sendRenderProgressInfo(filenameStr, -1111); // magic number for start
               float lastProgress = -1;

               AiRenderBegin(renderSession);
               while(true)
               {
                  AtRenderStatus status = AiRenderGetStatus(renderSession);
                  if (status == AI_RENDER_STATUS_PAUSED || status == AI_RENDER_STATUS_FINISHED || status == AI_RENDER_STATUS_FAILED)
                     break;

                  if (progressBar)
                  {
                     float progress = 0;
                     AiRenderGetHintFlt(renderSession, str::total_progress, progress);
                     if (progress != lastProgress)
                     {
                        lastProgress = progress;
                        MRenderUtil::sendRenderProgressInfo(filenameStr, (int)progress);
                        // also increase the progressbar value
                        MProgressWindow::setProgress(int((i + AiMax(progress * 0.01f, 1.f)) * progressNodeDelta));
                     }
                  }
               }
               AiRenderEnd(renderSession);

               MRenderUtil::sendRenderProgressInfo(filenameStr, 100); // magic number for end
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
            AtNode *camera = AiNode(universe, str::uv_camera, str::cameraUvBaker);
            if (camera == 0)
            {
               CSessionManager::DeleteActiveSession(s_renderToTextureSessionId);
               MGlobal::displayError("[mtoa] Render to Texture : Couldn't create a uv_camera node");
               return MS::kSuccess;
            }

            MString filename = folderName + "/" + meshNameStr.c_str();
            MString baseFilename = filename;
            if (sequence)
            {
               char paddingFrame[64];
               sprintf(paddingFrame, "%0*d", framePadding, (int) frame);
               filename += ".";
               filename += paddingFrame;
            }

            filename += ".exr";

            AiNodeSetPtr(camera, str::mesh, (void*)mesh);
            AiNodeSetFlt(camera, str::offset, (float)normalOffset);
            // need to adjust the near plane to make sure it's not bigger than the offset
            AiNodeSetFlt(camera, str::near_clip, (float)AiMin(0.5*normalOffset, (double)AiNodeGetFlt(camera, str::near_clip)));

            AiNodeSetFlt(camera, str::u_offset, (float)(-uStart));
            AiNodeSetFlt(camera, str::v_offset, (float)(-vStart));
            AiNodeSetBool(camera, str::extend_edges, extendEdges);

            AiNodeSetFlt(camera, str::u_scale, (float)(1. / AiMax((float)uScale, AI_EPSILON)));
            AiNodeSetFlt(camera, str::v_scale, (float)(1. / AiMax((float)vScale, AI_EPSILON)));

            AiNodeSetPtr(options_node, str::camera, camera);
            AiNodeSetStr(camera, str::uv_set, AtString(uvSet.asChar()));
            AiNodeSetStr(driver, str::filename, AtString(filename.asChar()));

            for (size_t aov = 0; aov < aovDrivers.size(); ++aov)
            {
               MString aovFilename = baseFilename;
               aovFilename +=  ".";
               aovFilename += aovNames[aov].c_str();
               if (sequence)
               {
                  char paddingFrame[64];
                  sprintf(paddingFrame, "%0*d", framePadding, (int) frame);
                  aovFilename += ".";
                  aovFilename += paddingFrame;
               }

               aovFilename += ".exr";
               AiNodeSetStr(aovDrivers[aov], str::filename, AtString(aovFilename.asChar()));
            }
            AiRenderSetHintStr(renderSession, AI_ADP_RENDER_CONTEXT, AI_ADP_RENDER_CONTEXT_OTHER);
            // imagers
            COptionsTranslator* translator = session->GetOptionsTranslator();
            MPlug pImg = translator->FindMayaPlug("imagers");
            if (!pImg.isNull())
            {
               MPlugArray conns;
               unsigned numImagers = pImg.numElements();
               for (unsigned int imagerIdx = 0; imagerIdx < numImagers; imagerIdx++)
               {
                  MPlug imagerPlug = pImg[imagerIdx];
                  conns.clear();
                  bool hasConnection = imagerPlug.connectedTo(conns,
                                                              true  /* asDst */,
                                                              false /* asSrc */);
                  if (hasConnection && conns.length() == 1)
                  {
                     MPlug source = conns[0];
                     // get imager name from source
                     MString sourceName = source.name();
                     int dotPos = sourceName.rindexW('.') - 1;
                     MString mayaString = sourceName.substringW(0, dotPos);
                     AtString arnoldString(mayaString.asChar());
                     // set input for driver
                     AtNode* imager = AiNodeLookUpByName(universe, arnoldString);
                     AiNodeSetPtr(driver, str::input, imager);
                  }
               }
            }
            AiRenderBegin(renderSession);
            while(true)
            {
               AtRenderStatus status = AiRenderGetStatus(renderSession);
               if (status == AI_RENDER_STATUS_PAUSED || status == AI_RENDER_STATUS_FINISHED || status == AI_RENDER_STATUS_FAILED)
                  break;
            }
            MGlobal::displayInfo(MString("[mtoa] Render to Texture : Rendered to ") + filename);
            AiNodeDestroy(camera);
            AiRenderEnd(renderSession);
         }
      }
      MProgressWindow::endProgress();
      CSessionManager::DeleteActiveSession(s_renderToTextureSessionId);                     
   }

   return MS::kSuccess;
}

