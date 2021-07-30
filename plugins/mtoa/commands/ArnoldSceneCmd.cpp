#include "utils/Version.h"
#include "ArnoldSceneCmd.h"
#include "utils/MakeTx.h"
#include "../common/UnorderedContainer.h"
#include "translators/NodeTranslator.h"
#include "translators/DagTranslator.h"
#include "session/SessionManager.h"

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
   syntax.addFlag("q", "query"); // returns true is there's currently an active arnold scene

   syntax.setObjectType(MSyntax::kStringObjects);
   return syntax;
}

static inline CArnoldSession *InitArnoldSceneSession()
{
   CArnoldSession *session = CSessionManager::FindActiveSession(s_arnoldSceneSessionId);
   if (session == nullptr)
   {
      // here we want to use the default (implicit) universe.
      // The goal of this command is for users to convert a scene in memory
      // and then access it through the arnold API. For this to work, we need the
      // nodes to be in the default universe
      session = new CArnoldSession(true, true); 
      CSessionManager::AddActiveSession(s_arnoldSceneSessionId, session);
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
   CArnoldSession *session = CSessionManager::FindActiveSession(s_arnoldSceneSessionId);
   if (args.isFlagSet("query"))
   {
      setResult(session != nullptr);
      return MS::kSuccess;
   }

   MString listValue = (args.isFlagSet("list")) ? args.flagArgumentString("list", 0) : "";
   MString mode = (args.isFlagSet("mode")) ? args.flagArgumentString("mode", 0) : "create";

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
      InitArnoldSceneSession()->Export(&list);
   }
   else if (mode == "destroy")
   {
      if (session)
      {
         // This will delete all the AtNodes that were created in the default universe
         CSessionManager::DeleteActiveSession(s_arnoldSceneSessionId);
      }
   }
   else if (mode == "convert_scene")
   {
      InitArnoldSceneSession()->Export();
   }
   else if (mode == "convert_selected") 
   {
      CArnoldSession *session = InitArnoldSceneSession();
      
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
      CArnoldSession *session = CSessionManager::FindActiveSession(s_arnoldSceneSessionId);
      if (session == nullptr)
         return MS::kFailure;
      
      MSelectionList sel = ComputeSelectionList(args);
      std::vector<CNodeTranslator *> translators;
      GetTranslatorsList(sel, session, translators);
      for (auto tr : translators)
      {
         tr->RequestUpdate();
      }
   } else if (mode == "destroy_selected") 
   {
      CArnoldSession *session = CSessionManager::FindActiveSession(s_arnoldSceneSessionId);
      if (session == nullptr)
         return MS::kFailure;

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
