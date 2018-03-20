#include "OptionsTranslator.h"
#include "translators/DagTranslator.h"
#include "translators/NodeTranslatorImpl.h"
#include "translators/driver/DriverTranslator.h"
#include "translators/camera/ImagePlaneTranslator.h"

#include "utils/MayaUtils.h"
#include "utils/MtoaLog.h"

#include <ai_universe.h>
#include <ai_msg.h>

#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MPlugArray.h>
#include <maya/MFnRenderLayer.h>
#include <maya/MFileIO.h>
#include <maya/MFileObject.h>
#include <maya/MRenderUtil.h>

#include <assert.h>

AtNode* COptionsTranslator::CreateArnoldNodes()
{
   assert(AiUniverseIsActive());

   AtNode* options = AiUniverseGetOptions();
   return options;
}

// One of the active AOVs is changing during IPR session, so we need to re-export
void COptionsTranslator::AovChangedCallback(MNodeMessage::AttributeMessage msg,
                                                    MPlug& plug, MPlug& otherPlug,
                                                    void* clientData)
{
   COptionsTranslator * translator = static_cast< COptionsTranslator* >(clientData);
   if (translator != NULL)
      translator->RequestUpdate();
}

void COptionsTranslator::ClearAovCallbacks()
{
   for (size_t i = 0; i < m_aovCallbacks.size(); ++i)
      MNodeMessage::removeCallback(m_aovCallbacks[i]);

   m_aovCallbacks.clear();
}

/// For each active AOV add a CAOV class to m_aovs
void COptionsTranslator::ProcessAOVs()
{
   AOVMode aovMode = AOVMode(FindMayaPlug("aovMode").asInt());
   m_aovsEnabled = aovMode == AOV_MODE_ENABLED ||
         (GetSessionOptions().IsBatch() && aovMode == AOV_MODE_BATCH_ONLY);

   bool foundBeauty = false;
   MPlugArray conns;

   m_aovs.clear();

   ClearAovCallbacks();

   MPlug pAOVs = FindMayaPlug("aovs");
   for (unsigned int i = 0; i < pAOVs.evaluateNumElements(); ++i)
   {
      if (pAOVs[i].connectedTo(conns, true, false))
      {
         CAOV aov;
         MObject oAOV = conns[0].node();
         if (aov.FromMaya(oAOV))
         {

            if (aov.GetName() == "beauty")
            {
               m_aovs.insert(aov);
               foundBeauty = true;
            }
            else
            {
               // AOVs are considered "in-use" even if they are not enabled
               m_aovsInUse = true;
               if (m_aovsEnabled && aov.IsEnabled())
                  m_aovs.insert(aov);
            }

            // We want to be adverted when one of the AOV nodes changes (light groups, lpe, etc...)
            if (GetSessionOptions().IsInteractiveRender())
               m_aovCallbacks.push_back(MNodeMessage::addAttributeChangedCallback(oAOV, AovChangedCallback, this));
         }
         else
         {
            MGlobal::displayWarning("[mtoa] Could not setup AOV attribute " + MFnDependencyNode(oAOV).name());
         }
      }
   }
   if (!foundBeauty)
   {
      CAOV aov;
      // CAOV is enabled and RGBA by default
      aov.SetName("beauty");
      m_aovs.insert(aov);
   }

}

/// Set the filenames for all output drivers
void COptionsTranslator::ExportAOVs()
{
   ProcessAOVs();

   MString displayAOV = FindMayaPlug("displayAOV").asString();
   // for backward compatibility
   if ((displayAOV == "RGBA") || (displayAOV == "RGB"))
      displayAOV = "beauty";


   // loop through AOVs
   m_aovData.clear();

   for (AOVSet::iterator it=m_aovs.begin(); it!=m_aovs.end(); ++it)
   {
      MString name = it->GetName();

      CAOVOutputArray aovData;
      aovData.type = it->GetDataType();

      bool lightGroups = it->HasLightGroups();
      bool globalAov = it->HasGlobalAov();
      MString lightGroupsList = it->GetLightGroupsList();
      MStringArray lgList;
      if (lightGroupsList.length() > 0)
         lightGroupsList.split(' ', lgList);

      aovData.lpe =  it->GetLightPathExpression();
      MPlug shaderPlug = it->GetShaderPlug();
      MString camera = it->GetCamera();

      if (!shaderPlug.isNull())
      {
         // there is a shader assigned to this AOV (attribute "defaultValue", weird name...)
         m_impl->ExportConnectedNode(shaderPlug, true, &aovData.shaderTranslator);
      } else
         aovData.shaderTranslator = NULL;

      if (camera.length() > 0)
      {
         MSelectionList cameraSelList;
         cameraSelList.add(camera);

         MDagPath camObject;
         cameraSelList.getDagPath(0,camObject);

         if (camObject.isValid())
         {
            camObject.extendToShape();
            MFnDependencyNode fnNode(camObject.node());
            MPlug dummyPlug = fnNode.findPlug("matrix"); // I need a dummy plug from the camera node
            if (!dummyPlug.isNull())
            {
               m_impl->ExportConnectedNode(dummyPlug, true, &aovData.cameraTranslator);
            }
         }
          else
         {
            AiMsgError("[mtoa.aov] Camera %s not found", camera.asChar());
         }
         

         
      } else
         aovData.cameraTranslator = NULL;

      // Global drivers
      std::vector<CAOVOutput> globalOutputs;
      MPlug pFilter = FindMayaPlug("filter");
      MPlug pDisplays = FindMayaPlug("drivers");

      for (unsigned int i=0; i < pDisplays.numElements(); ++i)
      {
         CAOVOutput output;
         if (GetOutput(pDisplays[i], pFilter, output))
            globalOutputs.push_back(output);
      }
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] [aov "+name+"] Setting AOV output: filter and driver.");

      GetOutputArray(*it, aovData.outputs);
      
      // Keep a copy of the outputs before I eventually add the global ones (renderview).
      // If I want to output light groups for this AOV, I don't want the global ones 
      // to be copied as well
      std::vector<CAOVOutput> localAOVOutputs = aovData.outputs;      

      // Add global outputs
      for (unsigned int i=0; i < globalOutputs.size(); ++i)
      {
         if (!globalOutputs[i].singleLayer || name == displayAOV)
            aovData.outputs.push_back(globalOutputs[i]);
         
      }

      
      // This token is added to the arguments in command mtoa.utils.getFileName()
      aovData.tokens = MString("RenderPass=") + name;
      aovData.name = name;

      if (name == "beauty")
      {
         // add default driver
         CAOVOutput output;
         ExportDriver(FindMayaPlug("driver"), output);
         output.filter = ExportFilter(FindMayaPlug("filter"));

         // search if I already have the same output driver + filter
         bool foundOutput = false;
         for (size_t i = 0; i < aovData.outputs.size(); ++i)
         {
            if (aovData.outputs[i].driver == output.driver && aovData.outputs[i].filter == output.filter)
            {
               foundOutput = true;
               break; 
            }
         }
         if (!foundOutput)
         {
            aovData.outputs.push_back(output);
            localAOVOutputs.push_back(output);
         }

         globalAov = true;
         // RGBA/RGB AOVs are a special case because the AOV name and the data type are linked.
         // We provide the term "beauty" to encapsulate these under one term. The data type of the beauty
         // pass determines whether we use the name "RGBA" or "RGB".
         aovData.name = (aovData.type == AI_TYPE_RGBA) ? "RGBA" : "RGB";
      } //else

      std::vector<CAOVOutputArray> aovDataList;
      
      if (globalAov)
         aovDataList.push_back(aovData);
      
      // now get rid of the global outputs (renderview)
      aovData.outputs = localAOVOutputs;

      if (lightGroups)
      {
         // We can merge the light groups in a single AOV for batch sessions
         // AND if the output image is exr (it's saved as multi-layer exr)
         bool mergeLightGroups = (GetSessionMode() == MTOA_SESSION_BATCH || GetSessionMode() == MTOA_SESSION_ASS);
         if (mergeLightGroups)
         {
            for (size_t i = 0; i < aovData.outputs.size(); ++i)
            {
               AtNode *driver = aovData.outputs[i].driver;
               if(driver && !AiNodeIs(driver, AtString("driver_exr")))
               {
                  mergeLightGroups = false;
                  break;
               }
            }
         }
         if (mergeLightGroups)
         {
            CAOVOutputArray aovDataLg = aovData;

            aovDataLg.name += "_*";
            aovDataLg.aovSuffix = "_lgroups";

            aovDataList.push_back(aovDataLg);
         } else
         {
            // expand all light groups in the maya scene

            unordered_set<std::string> lightGroupsSet;
            
            MStatus status;
            MDagPath path;
            MItDag   dagIterLights(MItDag::kDepthFirst, MFn::kLight, &status);

            for (; (!dagIterLights.isDone()); dagIterLights.next())
            {
               if (dagIterLights.getPath(path))
               {
                  // Only check for lights being visible, not templated and in render layer
                  // FIXME: does a light need to be in layer to render actually in Maya?
                  MFnDependencyNode depFn(path.node());
                  MStatus stat;
                  MPlug lgroups = depFn.findPlug("aiAov");
                  if (!lgroups.isNull())
                  {
                     MString lgroupStr = lgroups.asString();
                     if (lgroupStr.length() > 0)
                        lightGroupsSet.insert(lgroupStr.asChar());
                  }
               }
            }

            MString           classification;
            MItDag            dagIterPlugin(MItDag::kDepthFirst, MFn::kPluginLocatorNode, &status);
            for (; (!dagIterPlugin.isDone()); dagIterPlugin.next())
            {

               MFnDependencyNode depFn(dagIterPlugin.currentItem());
               std::string classification(MFnDependencyNode::classification(depFn.typeName()).asChar());
               if (classification.find("rendernode/arnold/light") != std::string::npos)
               {
                  if (dagIterPlugin.getPath(path))
                  {
                     MFnDagNode node(path.node());
                     MStatus stat;
                     MPlug lgroups = depFn.findPlug("aiAov");
                     if (!lgroups.isNull())
                     {
                        MString lgroupStr = lgroups.asString();
                        if (lgroupStr.length() > 0)
                           lightGroupsSet.insert(lgroupStr.asChar());
                     }
                  }
               }
            }

            for (unordered_set<std::string>::iterator it = lightGroupsSet.begin(); it != lightGroupsSet.end(); ++it)
            {
               std::string lgName(*it);
               CAOVOutputArray aovDataLg = aovData;
               aovDataLg.name = aovData.name + MString("_") + MString(lgName.c_str());
               aovDataLg.aovSuffix = MString("_") + MString(lgName.c_str());
               aovDataList.push_back(aovDataLg);
            }

         }
      } else if (lgList.length() > 0)
      {
         // can't have both "light groups" and seperate light groups simultaneously
         for (unsigned int i = 0; i < lgList.length(); ++i)
         {
            CAOVOutputArray aovDataLg = aovData;
            aovDataLg.name = aovData.name + MString("_") + lgList[i];
            aovDataLg.aovSuffix = MString("_") + lgList[i];
            aovDataList.push_back(aovDataLg);
         }
      }
      if (!aovDataList.empty())
      {
         aovData = aovDataList.back();
         aovDataList.pop_back();

         for (size_t i = 0; i < aovDataList.size(); ++i)
            m_aovData.push_back(aovDataList[i]);

      }
      m_aovData.push_back(aovData);
   }
}

/// Set the filenames for all output drivers
void COptionsTranslator::SetImageFilenames(MStringArray &outputs)
{
   m_imageFilenames.clear();

   const CSessionOptions &options = GetSessionOptions();
   MDagPath camera = options.GetExportCamera();
   if (!camera.isValid())
   {
      return;
   }
   MObject renderLayer = MFnRenderLayer::currentLayer();
   double fileFrameNumber = GetExportFrame();

   // file name
   MFileObject fileObj;
   fileObj.setRawFullName(MFileIO::currentFile());
   MString sceneFileName = fileObj.resolvedName();
   const int idx = sceneFileName.rindexW('.');
   if(idx==0)
   {
      sceneFileName = "";
   }
   else if(idx>=1)
   {
      sceneFileName = sceneFileName.substringW(0, idx-1);
   }

   // camera name
   MFnDagNode camDagTransform(camera.transform());
   MString nameCamera = camDagTransform.name();

   std::set<std::string> lightPathExpressions;

   MCommonRenderSettingsData::MpathType pathType;
   MCommonRenderSettingsData defaultRenderGlobalsData;
   MRenderUtil::getCommonRenderSettings(defaultRenderGlobalsData);
   if (options.IsBatch())
   {
      pathType = defaultRenderGlobalsData.kFullPathImage;
   }
   else
   {
      pathType = defaultRenderGlobalsData.kFullPathTmp;
   }

   // we're only doing stereo rendering for Batch sessions (ass export / batch render) or sequence rendering
   bool stereo = (options.IsBatch() || options.GetSessionMode() == MTOA_SESSION_SEQUENCE) && camera.node().hasFn(MFn::kStereoCameraMaster);

   int numEyes = 1;
   // loop through aovs
   unsigned int nAOVs = m_aovData.size();
   std::vector<CAOVOutputArray> stereoAovData;
   MString rightCameraName = "";
   MString leftCameraName = "";
   MDagPath rightCameraDag = camera;
   MDagPath leftCameraDag = camera;

   if (stereo)
   {   
      int childCount = camDagTransform.childCount();      
      for (int i = childCount - 1; i >= 0; --i)
      {
         MFnDagNode childNode(camDagTransform.child(i));
         MDagPath camChildPath;
         if (childNode.getPath(camChildPath) != MS::kSuccess) continue;
         camChildPath.extendToShape();
         MString childName = CDagTranslator::GetArnoldNaming(camChildPath);
         std::string childNameStr = childName.asChar();

         // if there's a way to get the Right-Left cameras through the Maya API it would be way better...
         // for now we're going through the cameras names, but this could fail if manually renamed.
         if(rightCameraName.numChars() == 0 && childNameStr.find("Right") != std::string::npos)
         {
            rightCameraName = childName;
            rightCameraDag = camChildPath;
         }
         else if (leftCameraName.numChars() == 0 && childNameStr.find("Left") != std::string::npos)
         {
            leftCameraName = childName;
            leftCameraDag = camChildPath;
         }
      }
      if (rightCameraName.numChars() > 0 && leftCameraName.numChars() > 0) numEyes = 2;
      else  stereo = false;
   }

   for (int eye = 0; eye < numEyes; ++eye)
   {
      // we're not storing the stereo cameras in m_aovData
      // if this function is called several times, I'd be scared that we double 
      // the amount of AOVs multiple times.
      // Here this stereo treatment only happens for batch sessions, 
      // so the m_aovData list is cleared after export anyway...
      if (eye > 0) stereoAovData = m_aovData;  // copying the aovData list for the Right eye

      for (unsigned int i = 0; i < nAOVs; ++i)
      {
         CAOVOutputArray& aovData = (eye > 0) ? stereoAovData[i] : m_aovData[i];

         MString aovCamera = nameCamera;
         MDagPath aovCameraDag = camera;

         if (aovData.cameraTranslator)
         {
            // replace the camera name by the one specified in the AOV itself
            aovCameraDag = static_cast<CDagTranslator*>(aovData.cameraTranslator)->GetMayaDagPath();

            MFnDagNode aovCamDagTransform(aovCameraDag);

            aovCamera = aovCamDagTransform.name();
            if (GetSessionOptions().IsInteractiveRender() && (aovCamera != nameCamera))
            {
               AiMsgWarning("[mtoa.aov] Per-camera AOV %s is skipped during interactive renders", aovData.name.asChar());
               continue;
            }
         }
         
         // loop through outputs
         unsigned int nOutputs = aovData.outputs.size();

         for (unsigned int j=0; j < nOutputs; ++j)
         {
            CAOVOutput& output = aovData.outputs[j];
            if (output.driver == NULL)
            {
               AiMsgWarning("[mtoa] Output driver %d for AOV \"%s\" is null", j, aovData.name.asChar());
               continue;
            }
            if (output.filter == NULL)
            {
               AiMsgWarning("[mtoa] Output filter %d for AOV \"%s\" is null", j, aovData.name.asChar());
               continue;
            }
            const AtNodeEntry* driverEntry = AiNodeGetNodeEntry(output.driver);


            // is this driver an output file image (otherwise it could be a display driver)
            bool outputImageDriver = (AiNodeEntryLookUpParameter(driverEntry, "filename") != NULL);
            if (outputImageDriver)
            {
               
               AtString ext("");
               AiMetaDataGetStr(driverEntry, NULL, "maya.translator", &ext);
               if (ext == AtString("deepexr"))
                  ext = AtString("exr");
               else if (ext == AtString("jpeg"))
                  ext = AtString("jpg");
                              
               
               MString tokens = aovData.tokens;
               MString path = output.prefix;

               if (path == "")
                  // No override provided, use globals default
                  path = defaultRenderGlobalsData.name;

               bool strictAOVs = !(m_aovsEnabled && m_aovsInUse && !output.mergeAOVs);

            
               MString eyeToken = "";
               if (stereo)
               {
                  // Setting the <Eye> token for Stereo rendering
                  if (eye == 0)
                  {
                     eyeToken = "Left";
                     aovCamera = leftCameraName;
                     aovCameraDag = leftCameraDag;
                  } else
                  {
                     eyeToken = "Right";
                     aovCamera = rightCameraName;
                     aovCameraDag = rightCameraDag;
                  }
               }

               MString filename = getFileName( pathType,
                                               fileFrameNumber,
                                               sceneFileName,
                                               nameCamera, // always use the export camera, even if this AOV is using a different one
                                               ext.c_str(),
                                               renderLayer,
                                               tokens,
                                               true,
                                               "images",
                                               path,
                                               NULL,
                                               &strictAOVs,
                                               eyeToken);

               // Eventually add a suffix to the filename (for light groups)
               if ((!output.mergeAOVs) && aovData.aovSuffix.length() > 0)
               {
                  int dotPos = filename.rindexW('.');
                  if (dotPos > 0)
                     filename = filename.substringW(0, dotPos - 1) + aovData.aovSuffix + filename.substringW(dotPos, filename.length() -1);
               }

               MString nodeTypeName = AiNodeEntryGetName(driverEntry);
               unordered_map<std::string, AtNode*>::iterator it;

               it = m_multiDriverMap.find(filename.asChar());

               std::string driverNodeToken = "";

               if (it == m_multiDriverMap.end())
               {                  
                  // The filename has not been encountered yet.
                  m_imageFilenames.append(filename);

                  // The same AtNode* driver may appear in m_aovData several times.  This happens because
                  // ExportNode() caches the results of previous exports to avoid creating duplicates.
                  // When a single aiAOVDriver node produces multiple files with unique names (via tokens)
                  // AND that node appears elsewhere in our list of output drivers then we have to clone the node.

                  // This situation also happens for stereo rendering, we'll have a new filename but the same output.driver as before
                  bool found = false;
                  for (it = m_multiDriverMap.begin(); it != m_multiDriverMap.end(); ++it)
                  {
                     if (it->second == output.driver)
                     {
                        found = true;
                        break;
                     }
                  }
                  MString driverName = (output.driverTranslator) ? output.driverTranslator->GetBaseName() : AiNodeGetName(output.driver);

                  // for light group AOVs, replace '_*' by '_lgroups' in the driver name
                  std::string aovNameStr(aovData.name.asChar());
                  if ((!aovNameStr.empty()) && aovNameStr[aovNameStr.length() -1] == '*')
                  {
                     aovNameStr = aovNameStr.substr(0, aovNameStr.length() - 1);
                     aovNameStr += "lgroups";
                  }

                  if (found && stereo && eye  > 0)
                  {
                     // For Stereo we don't want to add the aov name to the driver's name (we could, but names would become confusing), 
                     // so we're just adding th suffix ".Right"
                     // we could also add it for the left eye (without testing eye > 0), but I'm trying to minimize the possible issues
                     
                     if (aovNameStr != "RGBA")
                     {
                        driverNodeToken = aovNameStr;
                        driverNodeToken += ".";
                     }
                     driverNodeToken += eyeToken.asChar();
                     
                  } else
                     driverNodeToken = aovNameStr;
                  
                  driverName += ".";
                  driverName += driverNodeToken.c_str();

                  if (found)
                  {
                     if (output.driverTranslator)
                        output.driver = output.driverTranslator->GetChildDriver(driverNodeToken);
                     else
                        output.driver = AiNodeClone(output.driver);

                  }

                  AiNodeSetStr(output.driver, "name", driverName.asChar());

                  m_multiDriverMap[filename.asChar()] = output.driver;

                  std::string typeAOV = AiParamGetTypeName(aovData.type);
                  if (typeAOV != "RGB" && typeAOV != "RGBA")
                     AiNodeSetStr(output.driver, "color_space", ""); // this is supposed to be interpreted by arnold as Raw (passthrough)

               }
               else
               {  
                  // for stereo, if AOVs are merged we'll have multiple times the same filename 
                  // but output.driver still contains the previous (Left) driver
                  if (eye > 0) output.driver = it->second;

                  // Found an existing driver with the same filename.
                  // Check that it's the same driver.
                  if (output.driver != it->second)
                  {
                     // NOTE: it could be possible to merge the output of multiple drivers of the same type, but if their settings differ
                     // it will be unclear to the user which node's settings should be used
                     AiMsgWarning("[mtoa] Two drivers produced the same output path. AOV merging is only supported using a single driver node: \"%s\", \"%s\"",
                                  AiNodeGetName(output.driver), AiNodeGetName(it->second));
                     // skip this output
                     continue;                     
                  }
               }

               if (strictAOVs && (path.indexW("<RenderPass>") > -1))
               {
                  AiMsgWarning("[mtoa] Driver \"%s\" set to merge AOVs, but path prefix includes <RenderPass> token. Resulting outputs will not be merged",
                               AiNodeGetName(output.driver));
               }

               AiNodeSetStr(output.driver, "filename", filename.asChar());
               // FIXME: isn't this already handled by getImageName?
               CreateFileDirectory(filename);

               if (eye == 0 && aovData.lpe.length() > 0)
               {
                  MString lpe = aovData.name + " " + aovData.lpe.asChar();
                  lightPathExpressions.insert(lpe.asChar());
               }
            } else // here outputImageDriver == NULL
            {
               // no image driver -> possibly a display driver
               // we want to skip specific-cameras AOVs
               if (stereo) // display driver in stereo rendering
               {
                  if (eye != 0)
                     continue; // nothing to do here, we just want one eye for display

                  aovCamera = leftCameraName; // the display camera will be the left one       
                  aovCameraDag = leftCameraDag;           
               }                   
            }

            // output statement
            char str[1024];
            if (output.raw)
            {
               sprintf(str, "%s", AiNodeGetName(output.driver));
            }
            else
            {
               if (aovCamera != nameCamera)
               {
                  MString aovCameraArnoldName = CDagTranslator::GetArnoldNaming(aovCameraDag);
                  sprintf(str, "%s %s %s %s %s", aovCameraArnoldName.asChar(),  aovData.name.asChar(), AiParamGetTypeName(aovData.type),
                          AiNodeGetName(output.filter), AiNodeGetName(output.driver));
               }
               else
                  sprintf(str, "%s %s %s %s", aovData.name.asChar(), AiParamGetTypeName(aovData.type),
                          AiNodeGetName(output.filter), AiNodeGetName(output.driver));     
            }

            bool foundAOV = false;
            for (unsigned int o = 0; o < outputs.length(); ++o)
            {
               if (outputs[o] == MString(str))
               {
                  foundAOV = true;
                  break;
               }
            }  

            // I don't want to dump twice the same line as it wouldn't make sense
            // it can happen if you have a RGBA AOV
            if (!foundAOV)
            {
               if (MtoaTranslationInfo())
                  MtoaDebugLog("[mtoa] [aov "+aovData.name+"] output line: "+MString(str));

               outputs.append(MString(str));
            }
         }
      }
   }
   m_multiDriverMap.clear();

   if (!lightPathExpressions.empty())
   {
      AtArray *lpeArray = AiArrayAllocate(lightPathExpressions.size(), 1, AI_TYPE_STRING);
      std::set<std::string>::iterator it = lightPathExpressions.begin();
      std::set<std::string>::iterator itEnd = lightPathExpressions.end();

      int lpeInd = 0;
      for (; it != itEnd; ++it, lpeInd++)
      {
         AtString lpeElem((*it).c_str());
         AiArraySetStr(lpeArray, lpeInd, lpeElem);
      }
      AiNodeSetArray(AiUniverseGetOptions(), "light_path_expressions", lpeArray);
   }
}

void COptionsTranslator::CreateFileDirectory(const MString &filename) const
{
   if (filename != "")
   {
      // create the output directory
      int result;
      std::string outDir = filename.asChar();
      size_t p0 = outDir.find_last_of("\\/");
      if (p0 != std::string::npos)
      {
         outDir = outDir.substr(0, p0);
         MGlobal::executeCommand("sysFile -makeDir \"" + MString(outDir.c_str()) + "\"", result);
      }
   }
}

AtNode* COptionsTranslator::ExportDriver(const MPlug& driverPlug, CAOVOutput &output)
{

   MPlugArray conn;
   driverPlug.connectedTo(conn, true, false);

   if (!conn.length())//driverType == "<Use Globals>" || driverType == "")
      return NULL;

   // this generates a unique node every export

   CNodeTranslator *translator = NULL;
   output.driver = m_impl->ExportConnectedNode(conn[0], true, &translator);
   
   if (output.driver == NULL)
      return NULL;

   output.driverTranslator = dynamic_cast<CDriverTranslator*>(translator);

   const AtNodeEntry* entry = AiNodeGetNodeEntry(output.driver);

   MFnDependencyNode fnNode(conn[0].node());
   output.singleLayer = false;
   AiMetaDataGetBool(entry, NULL, "single_layer_driver", &output.singleLayer);
   if (!output.singleLayer)
      output.mergeAOVs = fnNode.findPlug("mergeAOVs").asBool();
   else
      output.mergeAOVs = false;
   output.raw = false;
   AiMetaDataGetBool(entry, NULL, "raw_driver", &output.raw);
   output.prefix = fnNode.findPlug("prefix").asString();
   return output.driver;
}

AtNode* COptionsTranslator::ExportFilter(const MPlug& filterPlug)
{
   MPlugArray conn;
   filterPlug.connectedTo(conn, true, false);

   if (!conn.length())//filterType == "<Use Globals>" || filterType == "")
      return NULL;

   AtNode* filter = ExportConnectedNode(conn[0]);
   if (filter == NULL)
      return NULL;

   return filter;
}

unsigned int COptionsTranslator::GetOutputArray(const CAOV& aov,
                                                std::vector<CAOVOutput>& outputs)
{
   MFnDependencyNode fnNode;
   MObject aovNode = aov.GetNode();
   if (aovNode.isNull())
      return 0;
   fnNode.setObject(aovNode);
   MPlug outputsPlug = fnNode.findPlug("outputs", true);
   for (unsigned int i=0; i<outputsPlug.numElements(); ++i)
   {
      CAOVOutput output;
      if (GetOutput(outputsPlug[i].child(0), outputsPlug[i].child(1), output))
         outputs.push_back(output);
   }
   return outputs.size();
}

bool COptionsTranslator::GetOutput(const MPlug& driverPlug,
                                   const MPlug& filterPlug,
                                   CAOVOutput& output)
{
   // Filter
   output.filter = ExportFilter(filterPlug);
   if (output.filter == NULL)
      return false;

   // Driver
   ExportDriver(driverPlug, output);
   if (output.driver == NULL)
      return false;

   return true;
}

void COptionsTranslator::SetCamera(AtNode *options)
{
   MDagPath cameraNode = GetSessionOptions().GetExportCamera();
   if (!cameraNode.isValid())
      return;

   cameraNode.extendToShape();
   CNodeTranslator *cameraTranslator = GetTranslator(cameraNode);
   AtNode* camera = (cameraTranslator) ? cameraTranslator->GetArnoldNode() : NULL;
   if (camera == NULL)
   {
      AiMsgError("[mtoa] Setting camera %s failed", cameraNode.partialPathName().asChar());
      return;
   }
   AiNodeSetPtr(options, "camera", camera);
   MStringArray outputStrings;
   SetImageFilenames(outputStrings);

   
   // OUTPUT STRINGS
   unsigned int ndrivers = outputStrings.length();
   AtArray* outputs  = AiArrayAllocate(ndrivers, 1, AI_TYPE_STRING);

   for (unsigned int i=0; i < ndrivers; ++i)
   {
      AiArraySetStr(outputs, i, outputStrings[i].asChar());
   }
   AiNodeSetArray(options, "outputs", outputs);

}

void ParseOverscanSettings(const MString& s, float& overscan, bool& isPercent)
{
   MString ms = s;
   ms.toLowerCase();
   if (ms.rindex('%') == (int)(ms.length() - 1))
   {
      isPercent = true;
      ms = ms.substring(0, ms.length() - 2);
   }
   else if (ms.rindexW(MString("px")) == (int)(ms.length() - 2))
   {
      isPercent = false;
      ms = ms.substring(0, ms.length() - 3);
   }
   else
      isPercent = false;

   if (ms.isInt())
      overscan = (float)ms.asInt();
   else if (ms.isFloat())
      overscan = ms.asFloat();
   else if (ms.isDouble())
      overscan = (float)ms.asDouble();
   else
      overscan = 0.0f;

   if (isPercent)
      overscan = overscan / 100.0f;

   if (overscan < AI_EPSILON)
      overscan = 0.0f;
}


void COptionsTranslator::Export(AtNode *options)
{
   assert(AiUniverseIsActive());
   ExportAOVs();

   AiNodeSetFlt(options, "texture_max_sharpen", 1.5f);
   AiNodeSetBool(options, "texture_per_file_stats", true);

// for maya 2017 and above, autoTX replaced automip, so we're forcing it to be false
#if MAYA_API_VERSION >= 201600
   AiNodeSetBool(options, "texture_automip", false);
#endif

   // set the camera
   SetCamera(options);

   const AtNodeEntry* optionsEntry = AiNodeGetNodeEntry(options);
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(options));
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      if (strcmp(paramName, "name") != 0)
      {
         // Special cases
         if (strcmp(paramName, "threads") == 0)
         {
            AiNodeSetInt(options, "threads", FindMayaPlug("threads_autodetect").asBool() ? 0 : FindMayaPlug("threads").asInt());
         }
         else if (strcmp(paramName, "AA_sample_clamp") == 0)
         {
            if (FindMayaPlug("use_sample_clamp").asBool())
            {
               CNodeTranslator::ProcessParameter(options, "AA_sample_clamp", AI_TYPE_FLOAT);
            } else
               AiNodeResetParameter(options, "AA_sample_clamp");
            
         }
         else if (strcmp(paramName, "AA_sample_clamp_affects_aovs") == 0)
         {
            if (FindMayaPlug("use_sample_clamp").asBool())
            {
               CNodeTranslator::ProcessParameter(options, "AA_sample_clamp_affects_aovs", AI_TYPE_BOOLEAN, "use_sample_clamp_AOVs");
            } else
               AiNodeResetParameter(options, "AA_sample_clamp_affects_aovs");

         }
         else if (strcmp(paramName, "indirect_sample_clamp") == 0)
         {
            CNodeTranslator::ProcessParameter(options, "indirect_sample_clamp", AI_TYPE_FLOAT);
         }
         else if (strcmp(paramName, "AA_seed") == 0)
         {
            // FIXME: this is supposed to use a connection to AA_seed attribute
            if (!FindMayaPlug("lock_sampling_noise").asBool())
            {
               AiNodeSetInt(options, "AA_seed", (int)GetExportFrame());
            }
         }
         else if (strcmp(paramName, "GI_sss_samples") == 0)
         {
            CNodeTranslator::ProcessParameter(options, "GI_sss_samples", AI_TYPE_INT);
         }
         else if (strcmp(paramName, "indirect_specular_blur") == 0)
         {
            CNodeTranslator::ProcessParameter(options, "indirect_specular_blur", AI_TYPE_FLOAT, "indirectSpecularBlur");
         }
         else if (strcmp(paramName, "bucket_scanning") == 0)
         {
            int bucket_scanning = AiMin(FindMayaPlug("bucketScanning").asInt(), 4); // old scenes might have a bigger value
            AiNodeSetInt(options, "bucket_scanning", bucket_scanning);
         }
         else if (strcmp(paramName, "texture_autotile") == 0)
         {
            AiNodeSetInt(options, "texture_autotile", !FindMayaPlug("autotile").asBool() ? 0 : FindMayaPlug("texture_autotile").asInt());
         }
         else if (strcmp(paramName, "AA_samples") == 0)
         {
            const int AA_samples = FindMayaPlug("AA_samples").asInt();
            AiNodeSetInt(options, "AA_samples", AA_samples == 0 ? 1 : AA_samples);
         } else if (strcmp(paramName, "thread_priority") == 0)
         {
            //AiNodeSetInt(options, "thread_priority", 2 );
         } else if (strcmp(paramName, "GI_glossy_samples") == 0 || strcmp(paramName, "GI_refraction_samples") == 0 ||
            strcmp(paramName, "sss_bssrdf_samples") == 0 || strcmp(paramName, "volume_indirect_samples") == 0)
         {
            // deprecated parameters, don't do anything
         }
         else
         {
            // Process parameter automatically
            // FIXME: we can't use the default method since the options names don't
            // follow the standard "toMayaStyle" behavior when no metadata is present
            // (see CBaseAttrHelper::GetMayaAttrName that is used by CNodeTranslator)
            AtString attrName;
            MPlug plug;
            if (AiMetaDataGetStr(optionsEntry, paramName, "maya.name", &attrName))
            {
               plug = FindMayaPlug(attrName.c_str());
            }
            else
            {
               plug = FindMayaPlug(paramName);
            }
            // Don't print warnings, just debug for missing options attributes are there are a lot
            // that are not exposed in Maya
            if (!plug.isNull())
            {
               ProcessParameter(options, paramName, AiParamGetType(paramEntry), plug);
            }
         }
      }
   }
   AiParamIteratorDestroy(nodeParam);

   // Setting the reference time properly (used when ignore motion blur is turned on)
   float referenceTime = 0.f;
   if (FindMayaPlug("mb_en").asBool())
   {
      // if motion blur is enabled, check the motion's range type 
      int motionRange = FindMayaPlug("range_type").asInt();
      switch(motionRange)
      {
         case MTOA_MBLUR_TYPE_START:
            referenceTime = 0.f;
         break;
         default:
         case MTOA_MBLUR_TYPE_CENTER:
            referenceTime = 0.5f;
         break;
         case MTOA_MBLUR_TYPE_END:
            referenceTime = 1.f;
         break;
         {
         case MTOA_MBLUR_TYPE_CUSTOM:
            float motionStart = FindMayaPlug("motion_start").asFloat();
            float motionEnd = FindMayaPlug("motion_end").asFloat();
            referenceTime = AiClamp((-motionStart) / AiMax((motionEnd - motionStart), AI_EPSILON), 0.f, 1.f);
         break;
         }
      }
   }
   AiNodeSetFlt(options, "reference_time", referenceTime);

   AddProjectFoldersToSearchPaths(options);
   
   // BACKGROUND SHADER
   //
   MPlugArray conns;
   MPlug pBG = FindMayaPlug("background");
   pBG.connectedTo(conns, true, false);
   if (conns.length() == 1)
   {
      AiNodeSetPtr(options, "background", ExportConnectedNode(conns[0]));
   }
   else
   {
      AiNodeSetPtr(options, "background", NULL);
      // first we get the image planes connected to this camera
      
      m_impl->m_session->ExportImagePlane();

   }
   if ((GetSessionMode() == MTOA_SESSION_BATCH) || (GetSessionMode() == MTOA_SESSION_ASS))
   {
      MString overscanString = FindMayaPlug("outputOverscan").asString();
      if (overscanString != "")
      {
         float overscanL = 0.0f;
         float overscanR = 0.0f;
         float overscanT = 0.0f;
         float overscanB = 0.0f;
         bool overscanLP = false;
         bool overscanRP = false;
         bool overscanTP = false;
         bool overscanBP = false;
         
         MStringArray split;
         overscanString.split(' ', split);
         const unsigned int splitLength = split.length();
         if (splitLength == 1)
         {
            ParseOverscanSettings(split[0], overscanL, overscanLP);
            overscanR = overscanL;
            overscanT = overscanL;
            overscanB = overscanL;
            overscanRP = overscanLP;
            overscanTP = overscanLP;
            overscanBP = overscanLP;
         }
         else if (splitLength == 2)
         {
            ParseOverscanSettings(split[0], overscanT, overscanTP);
            overscanB = overscanT;
            overscanBP = overscanTP;
            ParseOverscanSettings(split[1], overscanL, overscanLP);
            overscanR = overscanL;
            overscanRP = overscanLP;
         }
         else if (splitLength == 3)
         {
            ParseOverscanSettings(split[0], overscanT, overscanTP);
            ParseOverscanSettings(split[1], overscanL, overscanLP);
            overscanR = overscanL;
            overscanRP = overscanLP;
            ParseOverscanSettings(split[2], overscanB, overscanBP);
         }
         else if (splitLength == 4)
         {
            ParseOverscanSettings(split[0], overscanT, overscanTP);
            ParseOverscanSettings(split[1], overscanR, overscanRP);
            ParseOverscanSettings(split[2], overscanB, overscanBP);
            ParseOverscanSettings(split[3], overscanL, overscanLP);
         }

         const int width = AiNodeGetInt(options, "xres");
         const int height = AiNodeGetInt(options, "yres");

         AiNodeSetInt(options, "region_min_x", overscanLP ? (int)ceilf(-(float)width * overscanL) : -(int)overscanL);
         AiNodeSetInt(options, "region_max_x", overscanRP ? width + (int)ceilf((float)width * overscanR) : width + (int)overscanR - 1);
         AiNodeSetInt(options, "region_min_y", overscanTP ? (int)ceilf(-(float)height * overscanT) : -(int)overscanT);
         AiNodeSetInt(options, "region_max_y", overscanBP ? height + (int)ceilf((float)height * overscanB) : height + (int)overscanB - 1);
         
      }
   }

   ExportAtmosphere(options);   

   conns.clear();
   MPlug pOP = FindMayaPlug("operator");
   pOP.connectedTo(conns, true, false);
   if (conns.length() == 1)
   {
      AiNodeSetPtr(options, "operator", ExportConnectedNode(conns[0]));
   }
   else
   {
      AiNodeSetPtr(options, "operator", NULL);
   }


   // subdivision dicing camera
   //
   pBG = FindMayaPlug("subdivDicingCamera");
   pBG.connectedTo(conns, true, false);
   if (conns.length() == 1)
   {
      AiNodeSetPtr(options, "subdiv_dicing_camera", ExportConnectedNode(conns[0]));
   }
   else
   {
      AiNodeSetPtr(options, "subdiv_dicing_camera", NULL);
   }

   // frame number. We're now updating it at every Update (#2319)
   if (AiNodeLookUpUserParameter(options, "frame") == NULL)
      AiNodeDeclare(options, "frame", "constant FLOAT");
   
   AiNodeSetFlt(options, "frame", (float)GetExportFrame());

   if (!IsExported())
   {
      // render layer name
      // We're only dealing with render layer at first export 
      // because when render layer is changed, everything should be re-exported.
      // ARV does so, but we should maybe port it to Maya RV
      MStatus status;

      MObject currentRenderLayerObj = MFnRenderLayer::currentLayer(&status);   
      if (status)
      {
         MFnRenderLayer currentRenderLayer(currentRenderLayerObj, &status);
         if (status)
         {
            if (AiNodeLookUpUserParameter(options, "render_layer") == NULL)
               AiNodeDeclare(options, "render_layer", "constant STRING");

            AiNodeSetStr(options, "render_layer", currentRenderLayer.name().asChar());
         }
      }
      if (AiNodeLookUpUserParameter(options, "fps") == NULL)
         AiNodeDeclare(options, "fps", "constant FLOAT");
   }

   // now updating fps at every update, whoe knows
   static const float fpsTable[] = { 0.f, 1.f / 3600.f, 1.f / 60.f, 1.f,
                                   1000.f, 15.f, 24.f, 25.f, 30.f, 48.f,
                                   50.f, 60.f, 2.f, 3.f, 4.f, 5.f, 6.f,
                                   8.f, 10.f, 12.f, 16.f, 20.f, 40.f, 75.f,
                                   80.f, 100.f, 120.f, 125.f, 150.f, 200.f,
                                   240.f, 250.f, 300.f, 375.f, 400.f, 500.f,
                                   600.f, 750.f, 1200.f, 1500.f, 2000.f, 3000.f,
                                   6000.f, 0.f };
   AiNodeSetFlt(options, "fps", fpsTable[MTime::uiUnit()]);

   // Export AOV shaders   
   MPlug aovShadersPlug = FindMayaPlug("aov_shaders");
   unordered_set<AtNode*> aovShaders;
   for (unsigned int i = 0; i < aovShadersPlug.numElements (); i++)
   {
      MPlug elementPlug = aovShadersPlug [i];
      MPlugArray conns;
      elementPlug.connectedTo(conns, true, false);

      if (conns.length() > 0)
      {
         AtNode *aovShaderNode = ExportConnectedNode(conns[0]);
         if (aovShaderNode)
            aovShaders.insert(aovShaderNode);
      }
   }

   // I also need to add the shaders the are assigned to specific AOVs 
   for (size_t i = 0; i < m_aovData.size(); ++i)
   {
      CAOVOutputArray &aovData = m_aovData[i];
      if (aovData.shaderTranslator == NULL)
         continue;
   
      // This AOV has a shader assigned to it. I want to check if this is an AOV shader or not (based on its metadata)
      // - If it's an AOV shader => add it to the "aov_shaders" list
      // - If it's not -> insert an MtoaAovWriteColor in between
      AtNode *shaderNode = aovData.shaderTranslator->GetArnoldNode();
      if (shaderNode == NULL)
         continue;

      const AtNodeEntry *shaderNodeEntry = AiNodeGetNodeEntry(shaderNode);
      bool isAovShader = false;
      if (shaderNodeEntry && AiMetaDataGetBool(shaderNodeEntry, NULL, "aov_shader", &isAovShader) &&isAovShader)
      {
         // aov shader -> insert it directly to the AOV shaders list
         aovShaders.insert(shaderNode);
      } else
      {
         // not an AOV shader, it cannot fill the aov. We need to create an "aov_write_" node
         // and insert it in the middle

         // first get the type of the AOV
         MString aovWriteType = GetAOVNodeType(aovData.type);
         std::string shaderTag = "aov_shader_" + std::string(aovData.name.asChar());
         AtNode *aovWriteNode = AddArnoldNode(aovWriteType.asChar(), shaderTag.c_str());
         std::string aovWriteName = AiNodeGetName(shaderNode);
         aovWriteName += "@aov_shader";
         AiNodeSetStr(aovWriteNode, "name", aovWriteName.c_str());
         if (aovWriteNode)
         {
            aovShaders.insert(aovWriteNode);
            AiNodeLink(shaderNode, "input", aovWriteNode);
            AiNodeSetStr(aovWriteNode, "aov_name", aovData.name.asChar());
         }
      }      
   }

   AiNodeResetParameter(options, "aov_shaders");
   if (!aovShaders.empty())
   {
      AtArray *aovShadersArray = AiArrayAllocate(aovShaders.size(), 1, AI_TYPE_NODE);
      int aovShaderIndex = 0;
      for (unordered_set<AtNode*>::iterator it = aovShaders.begin(); it != aovShaders.end(); ++it)
         AiArraySetPtr(aovShadersArray, aovShaderIndex++, *it);
      
      AiNodeSetArray(options, "aov_shaders", aovShadersArray);
   }

   if (GetSessionOptions().IsInteractiveRender())
      AiNodeSetBool(options, "enable_dependency_graph", true);

   MPlug gpuPlug = FindMayaPlug("gpu");
   if (!gpuPlug.isNull() && GetSessionMode() != MTOA_SESSION_SWATCH)
   {
      bool gpu = gpuPlug.asBool();
      if (gpu)
      {
         AiNodeSetStr(options, "render_device", "GPU");
         MPlug autoDevices = FindMayaPlug("auto_select_devices");
         if (autoDevices.isNull() || !autoDevices.asBool())
         {
            std::vector<unsigned int> devices;
            // Manual Device selection
            MPlug gpuDevices = FindMayaPlug("render_devices");
            if (!gpuDevices.isNull())
            {
               unsigned int numElements = gpuDevices.numElements();
               for (unsigned int i = 0; i < numElements; ++i)
               {
                  MPlug elemPlug = gpuDevices[i];
                  if (!elemPlug.isNull())
                  {
                     devices.push_back(elemPlug.asInt());
                  }
               }
            }
            if (!devices.empty())
            {
               AtArray* selectDevices = AiArrayConvert(devices.size(), 1, AI_TYPE_UINT, &devices[0]);
               AiDeviceSelect((gpu) ? AI_DEVICE_TYPE_GPU : AI_DEVICE_TYPE_CPU, selectDevices);
               AiArrayDestroy(selectDevices);
            }
         } else
         {
            // automatically select the GPU devices
            AiDeviceAutoSelect();
         }
      } else
      {
         AiNodeSetStr(options, "render_device", "CPU");
      }
   }
}

void COptionsTranslator::ExportAtmosphere(AtNode *options)
{
   MPlugArray conns;
   MPlug pBG = FindMayaPlug("atmosphere");
   pBG.connectedTo(conns, true, false);
   if (conns.length() == 1)
   {
      AiNodeSetPtr(options, "atmosphere", ExportConnectedNode(conns[0]));
   }
   else
   {
      AiNodeSetPtr(options, "atmosphere", NULL);
   }
}

void COptionsTranslator::AddProjectFoldersToSearchPaths(AtNode* options)
{
   MString texture_searchpath(AiNodeGetStr(options, "texture_searchpath"));
   MString procedural_searchpath(AiNodeGetStr(options, "procedural_searchpath"));
   MStringArray sourceImagesDirs = getSourceImagesPath();
   MString projectPath = getProjectFolderPath();
#ifdef _WIN32   
   const MString pathsep = ";";
#else
   const MString pathsep = ":";
#endif
   if (texture_searchpath != "")
      texture_searchpath += pathsep;
   for (unsigned int i = 0; i < sourceImagesDirs.length(); ++i)
   {
      texture_searchpath += sourceImagesDirs[i];
      if (i != (sourceImagesDirs.length() -1))
         texture_searchpath += pathsep;
   }
   if (procedural_searchpath != "")
      procedural_searchpath += pathsep;
   procedural_searchpath += projectPath;
   AiNodeSetStr(options, "texture_searchpath", texture_searchpath.asChar());
   AiNodeSetStr(options, "procedural_searchpath", procedural_searchpath.asChar());
}

void COptionsTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);

   // this extra parameter is set by ARV to store its settings in the maya scene
   // It is set at each scene save.
   // we don't want this to propagate any dirtiness signal.
   if (plugName == "ARV_options") return;

   if (plugName.length() == 0) return;

   if (plugName == "motion_blur_enable" || plugName == "mb_object_deform_enable" || plugName == "mb_camera_enable" || 
         plugName == "motion_steps" || plugName == "range_type" || plugName == "motion_frames" ||
         plugName == "motion_start" || plugName == "motion_end")
   {
      // Need to re-export all the nodes that Require Motion
      m_impl->m_session->RequestUpdateMotion();
   } else if (plugName.length() > 4 && plugName.substringW(0, 3) == "log_")
   {
      m_impl->m_session->RequestUpdateOptions();
   } 

   CNodeTranslator::NodeChanged(node, plug);
}
