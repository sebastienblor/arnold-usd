#include "OptionsTranslator.h"
#include "render/RenderSession.h"
#include "render/RenderOptions.h"
#include "utils/MayaUtils.h"

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

/// For each active AOV add a CAOV class to m_aovs
void COptionsTranslator::ProcessAOVs()
{
   AOVMode aovMode = AOVMode(FindMayaPlug("aovMode").asInt());
   m_aovsEnabled = aovMode == AOV_MODE_ENABLED ||
         (m_session->IsBatch() && aovMode == AOV_MODE_BATCH_ONLY);

   bool foundBeauty = false;
   MPlugArray conns;
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
   if (!m_aovsEnabled)
      AiMsgDebug("[mtoa] [aovs] disabled");
}

/// Set the filenames for all output drivers
void COptionsTranslator::ExportAOVs()
{
   ProcessAOVs();

   MString displayAOV = FindMayaPlug("displayAOV").asString();
   // for backward compatibility
   if ((displayAOV == "RGBA") || (displayAOV == "RGB"))
      displayAOV = "beauty";

   AtNode* defaultFilter = CreateDefaultFilter();

   bool singleLayerDisplay = false;
   CAOVOutput displayOutput;
   displayOutput.driver = CreateDisplayDriver(displayOutput.prefix, singleLayerDisplay);
   displayOutput.filter = defaultFilter;
   displayOutput.mergeAOVs = false;  // FIXME: get a proper value

   // loop through AOVs
   for (AOVSet::iterator it=m_aovs.begin(); it!=m_aovs.end(); ++it)
   {
      MString name = it->GetName();
      CAOVOutputArray aovData;
      aovData.type = it->GetDataType();

      AiMsgDebug("[mtoa] [aov %s] Setting AOV output: filter and driver.", name.asChar());

      GetDriversAndFilters(*it, aovData.outputs);
      if (displayOutput.driver != NULL && (!singleLayerDisplay || name == displayAOV))
         aovData.outputs.push_back(displayOutput);

      aovData.tokens = MString("RenderPass=") + name;

      if (name == "beauty")
      {
         // add default driver
         CAOVOutput output;
         output.driver = ExportDriver(FindMayaPlug("driver"), output.prefix, output.mergeAOVs, output.singleLayer);
         output.filter = defaultFilter;
         aovData.outputs.push_back(output);

         // RGBA/RGB AOVs are a special case because the AOV name and the data type are linked.
         // We provide the term "beauty" to encapsulate these under one term. The data type of the beauty
         // pass determines whether we use the name "RGBA" or "RGB".
         name = (aovData.type == AI_TYPE_RGBA) ? "RGBA" : "RGB";
      }
      aovData.name = name;
      m_aovData.push_back(aovData);
   }
}

/// Set the filenames for all output drivers
void COptionsTranslator::SetImageFilenames(MStringArray &outputs)
{
   MDagPath camera = m_session->GetExportCamera();
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
   sceneFileName = sceneFileName.substringW(0, sceneFileName.rindexW('.')-1);

   // camera name
   MFnDagNode camDagTransform(camera.transform());
   MString nameCamera = camDagTransform.name();

   MCommonRenderSettingsData::MpathType pathType;
   MCommonRenderSettingsData defaultRenderGlobalsData;
   MRenderUtil::getCommonRenderSettings(defaultRenderGlobalsData);
   if (m_session->IsBatch())
   {
      pathType = defaultRenderGlobalsData.kFullPathImage;
   }
   else
   {
      pathType = defaultRenderGlobalsData.kFullPathTmp;
   }

   // loop through aovs
   unsigned int nAOVs = m_aovData.size();
   for (unsigned int i = 0; i < nAOVs; ++i)
   {
      CAOVOutputArray& aovData = m_aovData[i];

      // loop through outputs
      unsigned int nOutputs = aovData.outputs.size();
      for (unsigned int j=0; j < nOutputs; ++j)
      {
         CAOVOutput& output = aovData.outputs[j];
         const AtNodeEntry* driverEntry = AiNodeGetNodeEntry(output.driver);

         // handle drivers with filename parameters
         if (AiNodeEntryLookUpParameter(driverEntry, "filename") != NULL)
         {
            const char* ext = "";
            AiMetaDataGetStr(driverEntry, NULL, "maya.translator", &ext);

            MString tokens = aovData.tokens;
            MString path = output.prefix;
            if (path == "")
               // No override provided, use globals default
               path = defaultRenderGlobalsData.name;

            bool strictAOVs = !(m_aovsEnabled && m_aovsInUse && !output.mergeAOVs);

            MString filename = getFileName( pathType,
                                            fileFrameNumber,
                                            sceneFileName,
                                            nameCamera,
                                            ext,
                                            renderLayer,
                                            tokens,
                                            true,
                                            "images",
                                            path,
                                            NULL,
                                            &strictAOVs);

            MString nodeTypeName = AiNodeEntryGetName(driverEntry);
            std::map<std::string, AtNode*>::iterator it;
            it = m_multiDriverMap.find(filename.asChar());
            if (it == m_multiDriverMap.end())
            {
               // The filename has not been encountered yet.

               // The same AtNode* driver may appear in m_aovData several times.  This happens because
               // ExportNode() caches the results of previous exports to avoid creating duplicates.
               // When a single aiAOVDriver node produces multiple files with unique names (via tokens)
               // AND that node appears elsewhere in our list of output drivers then we have to clone the node.

               bool found = false;
               for (it = m_multiDriverMap.begin(); it != m_multiDriverMap.end(); ++it)
               {
                  if (it->second == output.driver)
                  {
                     found = true;
                     break;
                  }
               }
               MString driverName = AiNodeGetName(output.driver);
               driverName += "." + aovData.name;

               if (found)
                  output.driver = AiNodeClone(output.driver);

               AiNodeSetStr(output.driver, "name", driverName.asChar());
               m_multiDriverMap[filename.asChar()] = output.driver;
            }
            else
            {
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
         }
         // output statement
         char str[1024];
         sprintf(str, "%s %s %s %s", aovData.name.asChar(), AiParamGetTypeName(aovData.type),
                 AiNodeGetName(output.filter), AiNodeGetName(output.driver));
         AiMsgDebug("[mtoa] [aov %s] output line: %s", aovData.name.asChar(), str);

         outputs.append(MString(str));
      }
   }
   m_multiDriverMap.clear();
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

AtNode* COptionsTranslator::ExportDriver(const MPlug& driverPlug, MString& prefix, bool& mergeAOVs, bool& singleLayer)
{
   MPlugArray conn;
   driverPlug.connectedTo(conn, true, false);

   if (!conn.length())//driverType == "<Use Globals>" || driverType == "")
      return NULL;

   // this generates a unique node every export
   AtNode* driver = ExportNode(conn[0]);
   if (driver == NULL)
      return NULL;

   const AtNodeEntry* entry = AiNodeGetNodeEntry(driver);

   bool displayDriver = false;
   if ((AiNodeEntryLookUpParameter(entry, "gamma") != NULL) &&
        AiMetaDataGetBool(entry, NULL, "display_driver", &displayDriver) &&
        displayDriver)
   {
      AiNodeSetFlt(driver, "gamma", FindMayaPlug("display_gamma").asFloat());
   }

   MFnDependencyNode fnNode(conn[0].node());
   singleLayer = false;
   AiMetaDataGetBool(entry, NULL, "single_layer_driver", &singleLayer);
   if (!singleLayer)
      mergeAOVs = fnNode.findPlug("mergeAOVs").asBool();
   else
      mergeAOVs = false;
   prefix = fnNode.findPlug("prefix").asString();
   return driver;
}

AtNode* COptionsTranslator::ExportFilter(const MPlug& filterPlug)
{
   MPlugArray conn;
   filterPlug.connectedTo(conn, true, false);

   if (!conn.length())//filterType == "<Use Globals>" || filterType == "")
      return NULL;

   AtNode* filter = ExportNode(conn[0]);
   if (filter == NULL)
      return NULL;

   return filter;
}

unsigned int COptionsTranslator::GetDriversAndFilters(const CAOV& aov,
                                                      std::vector<CAOVOutput>& outputs)
{
   MFnDependencyNode fnNode;
   MObject aovNode = aov.GetNode();
   if (aovNode.isNull())
      return 0;
   fnNode.setObject(aovNode);
   MString name = aov.GetName();
   MPlugArray conn;
   MPlug outputsPlug = fnNode.findPlug("outputs", true);
   for (unsigned int i=0; i<outputsPlug.numElements(); ++i)
   {
      // Filter
      CAOVOutput output;
      output.filter = ExportFilter(outputsPlug[i].child(1));
      if (output.filter == NULL)
         continue;

      // Driver
      output.driver = ExportDriver(outputsPlug[i].child(0), output.prefix, output.mergeAOVs, output.singleLayer);
      if (output.driver == NULL)
         continue;

      outputs.push_back(output);
   }
   return outputs.size();
}

AtNode * COptionsTranslator::CreateDefaultFilter()
{
   // set the output driver
   AtNode* filter = ExportFilter(FindMayaPlug("filter"));
   if (filter == NULL)
      AiMsgError("[mtoa] default filter is NULL");
   return filter;
}

AtNode * COptionsTranslator::CreateDisplayDriver(MString& prefix, bool& singleLayer)
{
   // Don't create it if we're in batch mode.
   if (m_session->IsBatch()) return NULL;

   AtNode* driver = AiNode("renderview_display");

   const AtNodeEntry* entry = AiNodeGetNodeEntry(driver);
   MString nodeTypeName = AiNodeEntryGetName(entry);
   MString driverName = nodeTypeName + "@display";
   AiNodeSetStr(driver, "name", driverName.asChar());

   AiNodeSetFlt(driver, "gamma", FindMayaPlug("display_gamma").asFloat());

   singleLayer = false;
   AiMetaDataGetBool(entry, NULL, "single_layer_driver", &singleLayer);

   prefix = "";
   return driver;
}

void COptionsTranslator::SetCamera(AtNode *options)
{
   MDagPath cameraNode = m_session->GetExportCamera();
   if (!cameraNode.isValid())
      return;

   cameraNode.extendToShape();
   // FIXME: do this more explicitly: at this point the node should be exported, this is just retrieving the arnold node
   AtNode* camera = ExportDagPath(cameraNode);
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
      AiArraySetStr(outputs, i, outputStrings[i].asChar());
   AiNodeSetArray(options, "outputs", outputs);
}

void COptionsTranslator::Export(AtNode *options)
{
   assert(AiUniverseIsActive());

   MStringArray outputStrings;

   ExportAOVs(); // file outputs are skipped during IPR, so no need to call this in Update


   SetCamera(options);

   MStatus status;

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
               ProcessParameter(options, "AA_sample_clamp", AI_TYPE_FLOAT);
            }
            if (FindMayaPlug("use_sample_clamp_AOVs").asBool())
            {
               ProcessParameter(options, "use_sample_clamp_AOVs", AI_TYPE_BOOLEAN);
            }
         }
         else if (strcmp(paramName, "AA_seed") == 0)
         {
            // FIXME: this is supposed to use a connection to AA_seed attribute
            if (!FindMayaPlug("lock_sampling_noise").asBool())
            {
               AiNodeSetInt(options, "AA_seed", (int)GetExportFrame());
            }
         }
         else if (strcmp(paramName, "sss_bssrdf_samples") == 0)
         {
            if (FindMayaPlug("enable_raytraced_SSS").asBool())
               ProcessParameter(options, "sss_bssrdf_samples", AI_TYPE_INT);
            else
               AiNodeSetInt(options, "sss_bssrdf_samples", 0);
         }
         else if (strcmp(paramName, "bucket_scanning") == 0)
         {
            ProcessParameter(options, "bucket_scanning", AI_TYPE_INT, "bucketScanning");
         }
         else
         {
            // Process parameter automatically
            // FIXME: we can't use the default method since the options names don't
            // follow the standard "toMayaStyle" behavior when no metadata is present
            // (see CBaseAttrHelper::GetMayaAttrName that is used by CNodeTranslator)
            const char* attrName;
            MPlug plug;
            if (AiMetaDataGetStr(optionsEntry, paramName, "maya.name", &attrName))
            {
               plug = FindMayaPlug(attrName);
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
            else
            {
               // AiMsgDebug("[mtoa] [translator %s] Arnold options parameter %s is not exposed on Maya %s(%s)",
               //      GetTranslatorName().asChar(), paramName, GetMayaNodeName().asChar(), GetMayaNodeTypeName().asChar());
            }
         }
      }
   }
   AiParamIteratorDestroy(nodeParam);

   // BACKGROUND SHADER
   //
   MPlugArray conns;
   MPlug pBG = FindMayaPlug("background");
   pBG.connectedTo(conns, true, false);
   if (conns.length() == 1)
   {
      AiNodeSetPtr(options, "background", ExportNode(conns[0]));
   }

   // ATMOSPHERE SHADER
   //
   MSelectionList list;
   MPlug        shader;

   int atmosphere = FindMayaPlug("atmosphere").asInt();
   switch (atmosphere)
   {
   case 0:
      break;

   case 1:  // Fog
      list.add("defaultFog.outColor");
      if (list.length() > 0)
      {
         list.getPlug(0, shader);
         AiNodeSetPtr(options, "atmosphere", ExportNode(shader));
      }
      break;

   case 2:  // Volume Scattering
      list.add("defaultVolumeScattering.outColor");
      if (list.length() > 0)
      {
         list.getPlug(0, shader);
         AiNodeSetPtr(options, "atmosphere", ExportNode(shader));
      }
      break;
      
   case 3:
      shader = FindMayaPlug("atmosphereShader");
      shader.connectedTo(conns, true, false);
      if (conns.length())
         AiNodeSetPtr(options, "atmosphere", ExportNode(conns[0]));
      break;
   }

   // frame number
   AiNodeDeclare(options, "frame", "constant FLOAT");
   AiNodeSetFlt(options, "frame", (AtFloat)GetExportFrame());
}

void COptionsTranslator::Update(AtNode *options)
{
   // set the camera
   SetCamera(options);
}
