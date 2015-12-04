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

   m_aovs.clear();

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


   // loop through AOVs
   m_aovData.clear();

   for (AOVSet::iterator it=m_aovs.begin(); it!=m_aovs.end(); ++it)
   {
      MString name = it->GetName();
      CAOVOutputArray aovData;
      aovData.type = it->GetDataType();

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
      AiMsgDebug("[mtoa] [aov %s] Setting AOV output: filter and driver.", name.asChar());

      GetOutputArray(*it, aovData.outputs);

      // Add global outputs
      for (unsigned int i=0; i < globalOutputs.size(); ++i)
      {
         if (!globalOutputs[i].singleLayer || name == displayAOV)
            aovData.outputs.push_back(globalOutputs[i]);
      }


      aovData.tokens = MString("RenderPass=") + name;

      if (name == "beauty")
      {
         // add default driver
         CAOVOutput output;
         output.driver = ExportDriver(FindMayaPlug("driver"), output.prefix, output.mergeAOVs, output.singleLayer, output.raw);
         output.filter = ExportFilter(FindMayaPlug("filter"));
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
         // handle drivers with filename parameters
         if (AiNodeEntryLookUpParameter(driverEntry, "filename") != NULL)
         {
            const char* ext = "";
            AiMetaDataGetStr(driverEntry, NULL, "maya.translator", &ext);
            if(strcmp (ext,"deepexr") == 0)
               ext = "exr";
            
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
         if (output.raw)
         {
            sprintf(str, "%s", AiNodeGetName(output.driver));
         }
         else
         {
            sprintf(str, "%s %s %s %s", aovData.name.asChar(), AiParamGetTypeName(aovData.type),
                    AiNodeGetName(output.filter), AiNodeGetName(output.driver));
         }
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

AtNode* COptionsTranslator::ExportDriver(const MPlug& driverPlug, MString& prefix, bool& mergeAOVs, bool& singleLayer, bool& raw)
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

   MFnDependencyNode fnNode(conn[0].node());
   singleLayer = false;
   AiMetaDataGetBool(entry, NULL, "single_layer_driver", &singleLayer);
   if (!singleLayer)
      mergeAOVs = fnNode.findPlug("mergeAOVs").asBool();
   else
      mergeAOVs = false;
   raw = false;
   AiMetaDataGetBool(entry, NULL, "raw_driver", &raw);
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
   output.driver = ExportDriver(driverPlug,
                                output.prefix,
                                output.mergeAOVs,
                                output.singleLayer,
                                output.raw);
   if (output.driver == NULL)
      return false;
   return true;
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

   MStringArray outputStrings;

   // in renderView sessions, we'll call exportAOVs in Update().
   // This should probably be done in all other modes
   // but it might introduce issues
   if (GetSessionMode() != MTOA_SESSION_RENDERVIEW) ExportAOVs();
   
   AiNodeSetFlt(options, "texture_max_sharpen", 1.5f);
   
   AiNodeSetBool(options, "texture_per_file_stats", true);

   MStatus status;

   Update(options);

   // frame number
   AiNodeDeclare(options, "frame", "constant FLOAT");
   AiNodeSetFlt(options, "frame", (float)GetExportFrame());
   // render layer name
   MObject currentRenderLayerObj = MFnRenderLayer::currentLayer(&status);   
   if (status)
   {
      MFnRenderLayer currentRenderLayer(currentRenderLayerObj, &status);
      if (status)
      {
         AiNodeDeclare(options, "render_layer", "constant STRING");
         AiNodeSetStr(options, "render_layer", currentRenderLayer.name().asChar());
      }
   }
   AiNodeDeclare(options, "fps", "constant FLOAT");
   static const float fpsTable[] = { 0.f, 1.f / 3600.f, 1.f / 60.f, 1.f,
                                   1000.f, 15.f, 24.f, 25.f, 30.f, 48.f,
                                   50.f, 60.f, 2.f, 3.f, 4.f, 5.f, 6.f,
                                   8.f, 10.f, 12.f, 16.f, 20.f, 40.f, 75.f,
                                   80.f, 100.f, 120.f, 125.f, 150.f, 200.f,
                                   240.f, 250.f, 300.f, 375.f, 400.f, 500.f,
                                   600.f, 750.f, 1200.f, 1500.f, 2000.f, 3000.f,
                                   6000.f, 0.f };
   AiNodeSetFlt(options, "fps", fpsTable[MTime::uiUnit()]);   
}

void COptionsTranslator::Update(AtNode *options)
{
   // we should probably be able to change this in regular IPR too
   if (GetSessionMode() == MTOA_SESSION_RENDERVIEW) ExportAOVs();
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
            }
            if (FindMayaPlug("use_sample_clamp_AOVs").asBool())
            {
               CNodeTranslator::ProcessParameter(options, "use_sample_clamp_AOVs", AI_TYPE_BOOLEAN);
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
         else if (strcmp(paramName, "GI_sss_samples") == 0)
         {
            CNodeTranslator::ProcessParameter(options, "GI_sss_samples", AI_TYPE_INT);
         }
         else if (strcmp(paramName, "bucket_scanning") == 0)
         {
            CNodeTranslator::ProcessParameter(options, "bucket_scanning", AI_TYPE_INT, "bucketScanning");
         }
         else if (strcmp(paramName, "texture_autotile") == 0)
         {
            AiNodeSetInt(options, "texture_autotile", !FindMayaPlug("autotile").asBool() ? 0 : FindMayaPlug("texture_autotile").asInt());
         }
         else if (strcmp(paramName, "AA_samples") == 0)
         {
            const int AA_samples = FindMayaPlug("AA_samples").asInt();
            AiNodeSetInt(options, "AA_samples", AA_samples == 0 ? 1 : AA_samples);
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

   AddProjectFoldersToSearchPaths(options);
   
   // BACKGROUND SHADER
   //
   MPlugArray conns;
   MPlug pBG = FindMayaPlug("background");
   pBG.connectedTo(conns, true, false);
   if (conns.length() == 1)
   {
      AiNodeSetPtr(options, "background", ExportNode(conns[0]));
   }
   else
   {
      AiNodeSetPtr(options, "background", NULL);
   }
   if ((m_session->GetSessionMode() == MTOA_SESSION_BATCH) || (m_session->GetSessionMode() == MTOA_SESSION_ASS))
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

         //AiMsgInfo("Exporting overscan : %f %f %f %f", overscanL, overscanT, overscanB, overscanR);
      }
   }

   ExportAtmosphere(options);   
}

void COptionsTranslator::ExportAtmosphere(AtNode *options)
{
   MPlugArray conns;
   MPlug pBG = FindMayaPlug("atmosphere");
   pBG.connectedTo(conns, true, false);
   if (conns.length() == 1)
   {
      AiNodeSetPtr(options, "atmosphere", ExportNode(conns[0]));
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

/// Main entry point to export values to an arnold parameter from a maya plug, recursively following
/// connections in the dependency graph.
/// Calls ProcessParameterInputs for parameters that allow linking or ProcessConstantParameter
/// We need to override this function for the options node, because linking and unlinking
/// is not allowed, and calling AiNodeUnlink adds some unwanted messages to the log
AtNode* COptionsTranslator::ProcessParameter(AtNode* arnoldNode, const char* arnoldParamName,
                                          int arnoldParamType, const MPlug& plug)
{
   if (arnoldNode == NULL)
   {
      AiMsgError("[mtoa.translator]  %s: Cannot process parameter %s on null node.",
            GetTranslatorName().asChar(), arnoldParamName);
      return NULL;
   }
   if (plug.isNull())
   {
      AiMsgError("[mtoa.translator]  %s: Invalid Maya plug was passed as source for parameter %s on Arnold node %s(%s)",
            GetTranslatorName().asChar(), arnoldParamName,
            AiNodeGetName(arnoldNode), AiNodeEntryGetName(AiNodeGetNodeEntry(arnoldNode)));
      return NULL;
   }

   // It doesn't make sense to call this method when step is greater than 0
   if (GetMotionStep() > 0)
   {
      AiMsgWarning("[mtoa] [translator %s] %s.%s: ProcessParameter should not be used on motion steps greater than 0",
            GetTranslatorName().asChar(), AiNodeGetName(arnoldNode), arnoldParamName);
      return NULL;
   }

   return ProcessConstantParameter(arnoldNode, arnoldParamName, arnoldParamType, plug);
}
