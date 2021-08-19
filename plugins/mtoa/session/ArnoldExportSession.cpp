#include "ArnoldExportSession.h"
#include <maya/MGlobal.h>
#include <maya/MFileObject.h>
#include <maya/MBoundingBox.h>
#include "utils/MayaUtils.h"
#include "utils/MtoaLog.h"


namespace
{
   MString VerifyFileName(MString fileName, bool compressed)
   {
      unsigned int len = fileName.length();
      if ((len > 5) && ((fileName.substring(len - 4, len - 1).toLowerCase() == ".usd") ||
                        (fileName.substring(len - 5, len - 1).toLowerCase() == ".usda")||
                        (fileName.substring(len - 5, len - 1).toLowerCase() == ".usdc")))
         return fileName;

      if (!compressed)
      {
         if ((len < 4) || (fileName.substring(len - 4, len - 1).toLowerCase() != ".ass"))
            fileName += ".ass";
      }
      else
      {
         if ((len < 7) || (fileName.substring(len - 7, len - 1).toLowerCase() != ".ass.gz"))
         {
            if ((len < 4) || (fileName.substring(len - 4, len - 1).toLowerCase() == ".ass"))
               fileName += ".gz";
            else if ((len < 3) || (fileName.substring(len - 3, len - 1).toLowerCase() == ".gz"))
               fileName = fileName.substring(0, len - 4) + ".ass.gz";
            else
               fileName += ".ass.gz";
         }
      }

      return fileName;
   }
}

AtBBox CArnoldExportSession::GetBoundingBox()
{
   // We're no longer asking Arnold to dump the bounding box, because it required to start 
   // a render in "free" mode, so that subdivision and displacement were applied.
   // This gave us an exact bounding box, and it used to be necessary when load_at_init = false.
   // Now that load_at_init was removed, this bounding box information is just used for viewport display,
   // so we don't need it to be perfectly accurate, and we don't want its computation to be expensive.
   // So I'm now changing this, so that bounding boxes are computed by what Maya returns us.

   ObjectToTranslatorMap::const_iterator it = m_translators.begin();
   ObjectToTranslatorMap::const_iterator itEnd = m_translators.end();

   MBoundingBox globalBox = MBoundingBox(); // creates an empty bounding box 
   for (; it != itEnd; ++it)
   {
      CNodeTranslator *translator = it->second;
      if (translator == NULL) continue;

      AtNode *node = translator->GetArnoldNode();
      if (node == NULL) continue;

      if (AiNodeEntryGetType(AiNodeGetNodeEntry(node)) != AI_NODE_SHAPE) continue; // only consider shapes

      CDagTranslator *dagTranslator = static_cast<CDagTranslator*>(translator);
      const MDagPath &dagPath = dagTranslator->GetMayaDagPath();
      if (!dagPath.isValid()) continue;

      MStatus status;
      MFnDagNode fnNode(dagPath, &status);
      if (status != MS::kSuccess) continue;

      MBoundingBox box = fnNode.boundingBox (&status);
      if (status != MS::kSuccess) continue;

      box.transformUsing(dagPath.inclusiveMatrix());

      globalBox.expand(box);
   }
   AtVector boxmin((float)globalBox.min()[0], (float)globalBox.min()[1], (float)globalBox.min()[2]);
   AtVector boxmax((float)globalBox.max()[0], (float)globalBox.max()[1], (float)globalBox.max()[2]);
   return AtBBox(boxmin, boxmax);
   
   return AI_BBOX_ZERO;
}

MString CArnoldExportSession::GetExportFilename(const MString& customName,
                                                 const MCommonRenderSettingsData& renderGlobals,
                                                 double frameNumber,
                                                 const MString &sceneName,
                                                 const MString &cameraName,
                                                 const MString &fileFormat,
                                                 const MObject layer,
                                                 const bool createDirectory,
                                                 const bool isSequence,
                                                 const bool subFrames,
                                                 const bool isBatch,
                                                 MStatus *ReturnStatus) const
{
   MStatus status;
   // Current Maya file and directory
   MFileObject sceneFile;
   sceneFile.overrideResolvedFullName(sceneName);
   MString sceneDir = sceneFile.resolvedPath();
   MString sceneFileName = sceneFile.resolvedName();
   // Strip Maya scene extension if present
   unsigned int nscn = sceneFileName.numChars();
   if (nscn > 3)
   {
      MString ext = sceneFileName.substringW(nscn-3, nscn-1);
      if (ext == ".ma" || ext == ".mb")
      {
         sceneFileName = sceneFileName.substringW(0, nscn-4);
      }
   }
   
   MString path = "";
   if (customName.numChars())
      path = customName;
   else
   {
      // no custom filename provided, let's use the default one from the options
      MGlobal::executeCommand("getAttr \"defaultArnoldRenderOptions.output_ass_filename\"", path); 
      path.expandEnvironmentVariablesAndTilde();
   }


   MString assFileName = getFileName(MCommonRenderSettingsData::kFullPathImage,
                                      frameNumber,
                                      sceneFileName,
                                      cameraName,
                                      fileFormat,
                                      layer, "",
                                      createDirectory,
                                      "ASS",
                                      path,
                                      &isSequence);

   if (NULL != ReturnStatus) *ReturnStatus = status;
   return assFileName;
}

void CArnoldExportSession::WriteScene(MString customFileName, const bool compressed, bool writeBox, bool append)
{
   MString fileName;
   AtNode *options = AiUniverseGetOptions(GetUniverse());

   // if no custom fileName is given, use the default one in the environment variable
   if (customFileName.length() > 0)
   {
      fileName = VerifyFileName(customFileName.asChar(), compressed);
   }
   else
   {
      MGlobal::executeCommand("getAttr \"defaultArnoldRenderOptions.output_ass_filename\"", fileName);  
      fileName = VerifyFileName(fileName.expandEnvironmentVariablesAndTilde(), compressed);
   }

   if (fileName.length() == 0)
   {
      AiMsgError("[mtoa] File name must be set before exporting .ass file");
   }
   else
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa] Exporting Maya scene to file \""+ fileName +"\"");

      // Now save the metadata
      AtMetadataStore *mds = AiMetadataStore();

      // we're still adding this as an option because it could be expensive on big scenes
      AtBBox bBox = AI_BBOX_ZERO;
      if (writeBox)
      {
         bBox = GetBoundingBox();
         MString boundsStr;
         boundsStr += bBox.min.x;
         boundsStr += " ";
         boundsStr += bBox.min.y;
         boundsStr += " ";
         boundsStr += bBox.min.z;
         boundsStr += " ";
         boundsStr += bBox.max.x;
         boundsStr += " ";
         boundsStr += bBox.max.y;
         boundsStr += " ";
         boundsStr += bBox.max.z;
         AiMetadataStoreSetStr(mds, AtString("bounds"), boundsStr.asChar());
      }

      if (AiNodeLookUpUserParameter(options, "frame"))
         AiMetadataStoreSetFlt(mds, AtString("frame"), AiNodeGetFlt(options, "frame"));

      if (AiNodeLookUpUserParameter(options, "fps"))
         AiMetadataStoreSetFlt(mds, AtString("fps"), AiNodeGetFlt(options, "fps"));

      if (AiNodeLookUpUserParameter(options, "render_layer"))
         AiMetadataStoreSetStr(mds, AtString("render_layer"), AiNodeGetStr(options, "render_layer"));

      MString currentUser;
      MGlobal::executePythonCommand("import getpass; getpass.getuser();", currentUser);
      if (currentUser.length() > 0)
         AiMetadataStoreSetStr(mds, AtString("user"), currentUser.asChar());



      MString sceneFileName;
      MGlobal::executeCommand("file -q -sn", sceneFileName);
      if(sceneFileName.length() > 0)
         AiMetadataStoreSetStr(mds, AtString("scene"), sceneFileName.asChar());

      // FIXME : problem this is actually double filtering files
      // (Once at export to AiUniverse and once at file write from it)
      AtParamValueMap* params = AiParamValueMap();
      
      AiParamValueMapSetInt(params, AtString("mask"), m_sessionOptions.outputAssMask());
      AiParamValueMapSetBool(params, AtString("open_procs"), m_sessionOptions.expandProcedurals());
      AiParamValueMapSetBool(params, AtString("binary"), m_sessionOptions.useBinaryEncoding());
      AiParamValueMapSetFlt(params, AtString("frame"), AiNodeGetFlt(options, "frame"));
      if (append)
         AiParamValueMapSetBool(params, AtString("append"), true);

      AiSceneWrite(m_universe, fileName.asChar(), params, mds);
      AiMetadataStoreDestroy(mds);
      AiParamValueMapDestroy(params);

   }
}

void CArnoldExportSession::Export(MSelectionList* selected)
{
   MGlobal::executeCommand("prepareRender -setup");
   MGlobal::executeCommand("prepareRender -invokePreRender");
   m_sessionOptions.SetSupportStereoCameras(true);
   SetBatch(true);
   if (selected)
      m_checkRenderLayer = false;

   CArnoldSession::Export(selected);

   MGlobal::executeCommand("prepareRender -invokePostRender");
   MGlobal::executeCommand("prepareRender -restore");    
}