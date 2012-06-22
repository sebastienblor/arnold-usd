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
   if (aovMode == AOV_MODE_ENABLED ||
         (m_session->IsBatch() && aovMode == AOV_MODE_BATCH_ONLY))
   {
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
               if (aov.IsEnabled())
               {
                  m_aovs.insert(aov);
               }
            }
            else
            {
               MGlobal::displayWarning("[mtoa] Could not setup AOV attribute " + MFnDependencyNode(oAOV).name());
            }
         }
      }
   }
   else
      AiMsgDebug("[mtoa] [aovs] disabled");
}

///// Set the filenames for all output drivers
//MString COptionsTranslator::SetImageFilenames(CAOV* aov)
//{
//   MString imageFilename;
//   MString cameraFolderName;
//   MObject renderLayer = MFnRenderLayer::currentLayer();
//   double fileFrameNumber = MAnimControl::currentTime().value();
//
//   // file name
//   MFileObject fileObj;
//   fileObj.setRawFullName(MFileIO::currentFile());
//   MString sceneFileName = fileObj.resolvedName();
//   sceneFileName = sceneFileName.substringW(0, sceneFileName.rindexW('.')-1);
//
//   MFnDagNode camDagTransform(m_session->GetExportCamera().transform());
//   MString nameCamera = camDagTransform.name();
//
//   // Notes on MCommonRenderSettingsData::getImageName:
//   //   - sceneFileName is only used if defaultRenderGlobals.imageFilePrefix is not set
//   //   - a "<RenderPass>/" token is added before the file name if any pass nodes are
//   //     connected to a render layer AND <RenderPass> does not appear in defaultRenderGlobals.imageFilePrefix
//   // because getImageName ignores the sceneFileName arg when defaultRenderGlobals.imageFilePrefix is non-empty,
//   // we can only achieve the proper addition of the <RenderPass> token by creating a dummy render pass node.
//   // TODO: write a complete replacement for MCommonRenderSettingsData::getImageName
//   MCommonRenderSettingsData::MpathType pathType;
//   MCommonRenderSettingsData defaultRenderGlobalsData;
//   MRenderUtil::getCommonRenderSettings(defaultRenderGlobalsData);
//   if (m_session->IsBatch())
//   {
//      pathType = defaultRenderGlobalsData.kFullPathImage;
//   }
//   else
//   {
//      pathType = defaultRenderGlobalsData.kFullPathTmp;
//   }
//
//   // FIXME: only add RenderPass tokens if custom AOVs are enabled
//   if (defaultRenderGlobalsData.name == "")
//   {
//      // setup the default RenderPass token
//      sceneFileName = "<RenderPass>/" + sceneFileName;
//      // FIXME: hard-wiring convention here:
//      // need a a complete replacement for MCommonRenderSettingsData::getImageName to avoid this
//      // (see mtoa.utils.expandFileTokens for the beginning of one)
//      sceneFileName += "_<RenderPass>";
//   }
//
//   MStringArray extensions;
//   aov->GetImageFormats(extensions);
//   MString tokens = MString("RenderPass=") + aov->GetName();
//   for (unsigned int i=0; i<extensions.length(); ++i)
//   {
//      MString filename = defaultRenderGlobalsData.getImageName(pathType, fileFrameNumber,
//                                                               sceneFileName, nameCamera,
//                                                               extensions[i], renderLayer,
//                                                               tokens, 1);
//      // FIXME: the driver is not getting its filename set
//      aov->SetImageFilename(filename);
//   }
//}

/// Set the filenames for all output drivers
MString COptionsTranslator::SetImageFilenames(MDagPath &camera)
{
   if (!camera.isValid())
   {
      return "";
   }

   MString imageFilename;
   MString cameraFolderName;
   MObject renderLayer = MFnRenderLayer::currentLayer();
   double fileFrameNumber = GetExportFrame();

   // file name
   MFileObject fileObj;
   fileObj.setRawFullName(MFileIO::currentFile());
   MString sceneFileName = fileObj.resolvedName();
   sceneFileName = sceneFileName.substringW(0, sceneFileName.rindexW('.')-1);

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

   MString path = defaultRenderGlobalsData.name;
   imageFilename = getFileName(pathType,
                                fileFrameNumber,
                                sceneFileName,
                                nameCamera,
                                "",
                                renderLayer,
                                (m_aovs.size() > 0) ? "RenderPass=beauty" : "",
                                true,
                                "images",
                                path);

   if (m_driver != NULL && AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(m_driver), "filename") != NULL)
      AiNodeSetStr(m_driver, "filename", imageFilename.asChar());

   for (AOVSet::iterator it=m_aovs.begin(); it!=m_aovs.end(); ++it)
   {
      MString tokens = MString("RenderPass=") + it->GetName();
      const MStringArray extensions = it->GetImageFormats();
      for (unsigned int i=0; i<extensions.length(); ++i)
      {
         MString filename = getFileName(pathType,
                                         fileFrameNumber,
                                         sceneFileName,
                                         nameCamera,
                                         extensions[i],
                                         renderLayer,
                                         tokens,
                                         true,
                                         "images",
                                         path);
         // FIXME: the driver is not getting its filename set
         it->SetImageFilename(i, filename);
      }
   }
   return imageFilename;
}

AtNode * COptionsTranslator::CreateFileOutput(MStringArray &outputs, AtNode *defaultFilter)
{
   // Don't install the file driver when in IPR mode.
   if (m_session->GetSessionMode() == MTOA_SESSION_IPR) return NULL;

   ProcessAOVs();

   // set the output driver
   MPlug driverPlug = FindMayaPlug("driver");
   MPlugArray conn;
   driverPlug.connectedTo(conn, true, false);
   if (conn.length())
      m_driver = ExportNode(conn[0], false);
   if (m_driver != NULL)
   {
      AiNodeSetStr(m_driver, "name", AiNodeEntryGetName(AiNodeGetNodeEntry(m_driver)));
      char   str[1024];
      sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(defaultFilter), AiNodeGetName(m_driver));
      outputs.append(str);
   }
   else
      AiMsgError("[mtoa] image driver is NULL");

   // because sets are ordered, their contents are const. we must make a new set
   // and overwrite it.
   AOVSet newAOVs;
   for (AOVSet::iterator it=m_aovs.begin(); it!=m_aovs.end(); ++it)
   {
      CAOV aov = (*it);
      MStringArray currOutputs;
      aov.SetupOutputs(outputs, m_driver, defaultFilter);
      newAOVs.insert(aov);
   }
   m_aovs = newAOVs;
   return m_driver;
}

AtNode * COptionsTranslator::CreateOutputFilter()
{
   // set the output driver
   AtNode* filter = NULL;
   MPlug filterPlug = FindMayaPlug("filter");
   MPlugArray conn;
   filterPlug.connectedTo(conn, true, false);
   if (conn.length())
      filter = ExportNode(conn[0], false);
   if (filter != NULL)
   {
      AiNodeSetStr(filter, "name", AiNodeEntryGetName(AiNodeGetNodeEntry(filter)));
   }
   else
      AiMsgError("[mtoa] filter is NULL");

   return filter;
}

AtNode * COptionsTranslator::CreateRenderViewOutput(MStringArray &outputs, AtNode *defaultFilter)
{
   // Don't create it if we're in batch mode.
   if (m_session->IsBatch()) return NULL;

   AtNode* driver = AiNode("renderview_display");
   AiNodeSetStr(driver, "name", "renderview_display");

   AiNodeSetFlt(driver, "gamma", FindMayaPlug("display_gamma").asFloat());
   char   str[1024];
   AiMsgInfo("display AOV: %s", FindMayaPlug("displayAOV").asString().asChar());
   sprintf(str, "%s RGBA %s %s", FindMayaPlug("displayAOV").asString().asChar(),
           AiNodeGetName(defaultFilter), AiNodeGetName(driver));
   outputs.append(str);
   //sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(filter), AiNodeGetName(render_view));

   return driver;
}

/// Create and setup all filters and drivers and output statements
void COptionsTranslator::SetupRenderOutput(AtNode* options)
{
   MStringArray outputStrings;
   AtNode * filter = CreateOutputFilter();
   CreateRenderViewOutput(outputStrings, filter);
   CreateFileOutput(outputStrings, filter);

   // OUTPUT STRINGS
   int ndrivers = outputStrings.length();
   AtArray* outputs  = AiArrayAllocate(ndrivers, 1, AI_TYPE_STRING);

   for (unsigned int i=0; i < outputStrings.length(); ++i)
   {
      AiArraySetStr(outputs, i, outputStrings[i].asChar());
   }
   AiNodeSetArray(options, "outputs", outputs);
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
   SetImageFilenames(cameraNode);
}

void COptionsTranslator::Export(AtNode *options)
{
   assert(AiUniverseIsActive());

   SetupRenderOutput(options);
   // set the camera
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
   }

}

void COptionsTranslator::Update(AtNode *options)
{
   // set the camera
   SetCamera(options);
}
