#include "utils/Version.h"
#include "ArnoldSceneCmd.h"
#include "utils/MakeTx.h"
#include "../common/UnorderedContainer.h"
#include "translators/NodeTranslator.h"
#include "translators/DagTranslator.h"
#include "session/SessionManager.h"
#include "session/ArnoldExportSession.h"
#include "common/DynLibrary.h"

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>
#include <ai_bbox.h>

#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <maya/MRenderUtil.h>
#include <maya/MFileIO.h>
#include <maya/MFileObject.h>
#include <maya/MFnRenderLayer.h>
#include <maya/MAnimControl.h>
#include <maya/MBoundingBox.h>


#include <math.h>
static const std::string s_arnoldSceneSessionId("arnoldScene");
MSyntax CArnoldSceneCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("m", "mode", MSyntax::kString);
   syntax.addFlag("l", "list",  MSyntax::kString);
   syntax.addFlag("uc", "usd_cache",  MSyntax::kLong);
   syntax.addFlag("uf", "usd_frame",  MSyntax::kDouble);
   syntax.addFlag("s", "session",  MSyntax::kString);
   syntax.addFlag("q", "query"); // returns true is there's currently an active arnold scene
   syntax.addFlag("o", "options",  MSyntax::kString);

   syntax.setObjectType(MSyntax::kStringObjects);
   return syntax;
}

// Create and register the ArnoldSession if it doesn't exist yet. If a specific aiOptions node is provided,
// use it instead of the default one in the SessionOptions
static inline CArnoldSession *InitArnoldSceneSession(const std::string &sessionId, const MString &optionsNode)
{
   CArnoldSession *session = CSessionManager::FindActiveSession(sessionId);
   if (session == nullptr)
   {
      // here we want to use the default (implicit) universe.
      // The goal of this command is for users to convert a scene in memory
      // and then access it through the arnold API. For this to work, we need the
      // nodes to be in the default universe
      session = new CArnoldSceneSession(); 
      if (optionsNode.length() > 0)
      {
         MSelectionList list;
         MObject node;
         list.add(optionsNode);
         if (list.length() > 0)
         {
            list.getDependNode(0, node);
            if (!node.isNull())
            {
               session->GetOptions().SetArnoldRenderOptions(node);
            }
         }
      }
      CSessionManager::AddActiveSession(sessionId, session);

   }
   return session;
}

static inline MSelectionList ComputeSelectionList(const MArgDatabase &args)
{
   MSelectionList sList;
   MStringArray sListStrings;
   args.getObjects(sListStrings);   
   const unsigned int sListStringsLength = sListStrings.length();
   
   if (sListStringsLength > 0)
   {
      for (unsigned int i = 0; i < sListStringsLength; ++i)
      {
         sList.add(sListStrings[i]);
      }
   }
   else
      MGlobal::getActiveSelectionList(sList);

   return sList;
}
typedef void (*WriteUsdStageCacheFunction)(int, bool, float);
// Write the current Arnold universe to USD, using a given USD cacheID.
// We want to dlopen the mayaUsdRegistry DSO, and invoke the function WriteUsdStageCache.
// This is required as it links against USD, as opposed to MtoA.
static void *s_pluginLib = nullptr; 

static bool WriteUsdFile(int cacheId, bool defaultTime, double time)
{
   // Get the path of this MtoA root install   
   const char *mtoa_var = getenv("MTOA_PATH");
   if (mtoa_var == nullptr)
      return false;

   std::string mtoa_path(mtoa_var);
   // Add the folder where mayaUsdRegistry is located
   mtoa_path += "usd/mayaUsdRegistry/";

   // We have one mayaUsdRegistry lib per version of USD this MtoA cut supports.
   // To find the proper one, we ask MayaUsd what is the "current" version of USD
   // and compute a folder name based on it
   MString usdVersion;
   MGlobal::executePythonCommand("from pxr import Usd; Usd.GetVersion()", usdVersion);
   // usdVersion will be e.g. (2, 1, 8), we need to convert this to "2108" for the folder name
   usdVersion.substitute(MString("("), MString(""));
   usdVersion.substitute(MString(")"), MString(""));
   usdVersion.substitute(MString(" "), MString(""));
   MStringArray splitUsdVersion;
   usdVersion.split(',', splitUsdVersion);
   MString fullVersion;
   for (unsigned int i = 0; i < splitUsdVersion.length(); ++i)
   {
      MString str = splitUsdVersion[i];
      if (str.length() == 0 || str == "0")
         continue;
      if (str.length() == 1)
         str = MString("0") + str;
      if (str.length() == 2)
         fullVersion += str;
   }
   mtoa_path += fullVersion.asChar();
   std::string filename = mtoa_path + "/mayaUsdRegistry";
   filename += LIBEXT;

   // Load the mayaUsdRegistry DSO
   if (s_pluginLib == nullptr)
      s_pluginLib = LibraryLoad(filename.c_str());
   if (s_pluginLib == nullptr)
   {
      AiMsgWarning("[mtoa] Error loading USD library: %s", LibraryLastError());
      return false;
   }

   // Check if we find the symbol for the WriteUsdStageCache function
   void* fn = LibrarySymbol(s_pluginLib, "WriteUsdStageCache");
   if (fn == NULL)
   {            
      AiMsgWarning("[mtoa] Error loading USD function: %s", LibraryLastError());
      LibraryUnload(s_pluginLib);
      return false;
   }

   // We are not unloading the plugin lib anymore because it can 
   // cause crashes in some cases (see MTOA-1365)
   
   // Invoke the DSO function
   const WriteUsdStageCacheFunction &writeFunc = (WriteUsdStageCacheFunction)(fn);
   (writeFunc)(cacheId, defaultTime, float(time));

   return true;
}
static inline void GetTranslatorsList(const MSelectionList &sel, CArnoldSession *session, std::vector<CNodeTranslator*> &translators)
{   
   translators.clear();
   translators.reserve(sel.length());
   for (unsigned int i = 0; i < sel.length(); ++i)
   {
      MStatus listStatus;
      MDagPath dag;
      MObject objNode;
      CNodeTranslator *tr = NULL;
      if (sel.getDagPath(i, dag) == MS::kSuccess)
      {
         dag.extendToShape();
         tr = session->GetActiveTranslator(CNodeAttrHandle(dag));
      } else if (sel.getDependNode(i, objNode) == MS::kSuccess)
      {
         tr = session->GetActiveTranslator(CNodeAttrHandle(objNode));
      }
      if (tr)
      {
         translators.push_back(tr);
      }
   }
}
MStatus CArnoldSceneCmd::doIt(const MArgList& argList)
{
   
   MStatus status;
   MArgDatabase args(syntax(), argList);

   MStringArray result;

   bool listAllNewNodes = false;
   bool listRootNodes = false;
   bool listAllNodes = false;
   // If the "session" argument is specified in the command, we will look for the expected arnold session ID.
   // Otherwise, we'll use the default one for arnoldScene command (s_arnoldSceneSessionId)
   std::string sessionId = args.isFlagSet("session") ? std::string(args.flagArgumentString("session", 0).asChar()) : s_arnoldSceneSessionId;
   CArnoldSession *session = CSessionManager::FindActiveSession(sessionId);
   if (args.isFlagSet("query"))
   {
      setResult(session != nullptr);
      return MS::kSuccess;
   }

   // If the usd_cache argument is set, it means that we're asked to save the current
   // Arnold universe to usd, using a specific USD cacheID
   if (args.isFlagSet("usd_cache"))
   {
      int cacheId = args.flagArgumentInt("usd_cache", 0);
      bool defaultTime = true;
      double time = 0.;
      // If no usd_frame argument is set, we want to export the "default" frame
      if (args.isFlagSet("usd_frame")) {
         defaultTime = false;
         time = args.flagArgumentDouble("usd_frame", 0);
      } else {
         MGlobal::executeCommand(MString("currentTime -query"), time); 
      }

      WriteUsdFile(cacheId, defaultTime, time);
      setResult(true);
      return MS::kSuccess;
   }

   MString listValue = (args.isFlagSet("list")) ? args.flagArgumentString("list", 0) : "";
   MString mode = (args.isFlagSet("mode")) ? args.flagArgumentString("mode", 0) : "create";

   MString optionsNode = (args.isFlagSet("options")) ? args.flagArgumentString("options", 0) : "";

   if (listValue == "new_nodes")
      listAllNewNodes = true;
   else if (listValue == "nodes" && mode == "convert_selected")
      listRootNodes = true;
   else if (listValue == "all_nodes")
      listAllNodes = true;


   unordered_set<std::string> previousObjects;
   if (listAllNewNodes && session != nullptr)
   {
      // if requested, loop over previously existing arnold nodes in the scene, so
      // that we can find out the newly created nodes after export
      AtNodeIterator* nodeIter = AiUniverseGetNodeIterator(session->GetUniverse(), AI_NODE_ALL);
      while (!AiNodeIteratorFinished(nodeIter))
      {
         AtNode *node = AiNodeIteratorGetNext(nodeIter);
         if (node == NULL) continue;
         std::string nodeName = AiNodeGetName(node);
         if (nodeName.empty()) continue;

         previousObjects.insert(nodeName);
      }
      AiNodeIteratorDestroy(nodeIter);
   }

   if (mode == "create")
   {
      // export with an empty selection list so that options, etc...
      // are properly initialized
      MSelectionList list;
      InitArnoldSceneSession(sessionId, optionsNode)->Export(&list);
   }
   else if (mode == "destroy")
   {
      if (session)
      {
         // This will delete all the AtNodes that were created in the default universe
         CSessionManager::DeleteActiveSession(sessionId);
      }
   }
   else if (mode == "convert_scene")
   {
      InitArnoldSceneSession(sessionId, optionsNode)->Export();
   }
   else if (mode == "convert_selected") 
   {
      CArnoldSession *session = InitArnoldSceneSession(sessionId, optionsNode);
      MSelectionList sel = ComputeSelectionList(args);

      session->Export(&sel);
      // Even though we called Export(&sel) we must ensure that each of the selected nodes was properly exported.
      // The problem is that this function is only exporting cameras, shapes, lights, etc... so it's not fully doing what we expect
      if (session)
      {
         unordered_set<std::string> newNodes;
         for (unsigned int i = 0; i < sel.length(); ++i)
         {
            MStatus listStatus;
            MDagPath dag;
            MObject objNode;
            CNodeTranslator *tr = NULL;
            if (sel.getDagPath(i, dag) == MS::kSuccess)
            {
               dag.extendToShape();
               tr = session->ExportDagPath(dag, false);
            } else if (sel.getDependNode(i, objNode) == MS::kSuccess)
            {
               tr = session->ExportNode(objNode, 
                                false, 0);

            }
            if (tr)
            {
               AtNode *trNode = tr->GetArnoldNode();
               if (trNode)
                  newNodes.insert(AiNodeGetName(trNode));
            }            
         }
         for (unordered_set<std::string>::iterator it = newNodes.begin(); it != newNodes.end(); ++it)
            result.append((*it).c_str());
      }
   }
   else if (mode == "update_selected") 
   {
      CArnoldSession *session = CSessionManager::FindActiveSession(sessionId);
      if (session == nullptr)
      {
         MGlobal::displayError(MString("[mtoa] Arnold Session not found : ") + MString(sessionId.c_str()));
         return MS::kSuccess;
      }
      MSelectionList sel = ComputeSelectionList(args);
      std::vector<CNodeTranslator *> translators;
      GetTranslatorsList(sel, session, translators);
      for (auto tr : translators)
      {
         tr->RequestUpdate();
      }
   } else if (mode == "destroy_selected") 
   {
      CArnoldSession *session = CSessionManager::FindActiveSession(sessionId);
      if (session == nullptr)
      {
         MGlobal::displayError(MString("[mtoa] Arnold Session not found : ") + MString(sessionId.c_str()));
         return MS::kSuccess;
      }

      MSelectionList sel = ComputeSelectionList(args);
      std::vector<CNodeTranslator *> translators;
      GetTranslatorsList(sel, session, translators);
      for (auto tr : translators)
      {
         session->EraseActiveTranslator(tr);      
      }
   }
   if (listAllNewNodes || listAllNodes)
   {
      AtNodeIterator* nodeIter = AiUniverseGetNodeIterator(session->GetUniverse(), AI_NODE_ALL);
      while (!AiNodeIteratorFinished(nodeIter))
      {
         AtNode *node = AiNodeIteratorGetNext(nodeIter);
         if (node == NULL || AiNodeIsDisabled(node)) continue;
         std::string nodeName = AiNodeGetName(node);
         if (nodeName.empty()) continue;
         // if this node wasn't already in the previous list, add it
         if (listAllNodes || previousObjects.find(nodeName) == previousObjects.end())
            result.append(nodeName.c_str());

      }
      AiNodeIteratorDestroy(nodeIter);
   } 
   setResult(result);
   return MS::kSuccess;
}
