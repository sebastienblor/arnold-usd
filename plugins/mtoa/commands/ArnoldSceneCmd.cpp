#include "utils/Version.h"
#include "ArnoldSceneCmd.h"
#include "scene/MayaScene.h"
#include "utils/MakeTx.h"
#include "../common/UnorderedContainer.h"
#include "translators/NodeTranslator.h"
#include "translators/DagTranslator.h"

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

MSyntax CArnoldSceneCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("m", "mode", MSyntax::kString);
   syntax.addFlag("l", "list",  MSyntax::kString);
   syntax.addFlag("q", "query"); // returns true is there's currently an active arnold scene

   syntax.setObjectType(MSyntax::kStringObjects);
   return syntax;
}



MStatus CArnoldSceneCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MArgDatabase args(syntax(), argList);

   MStringArray result;

   bool listAllNewNodes = false;
   bool listRootNodes = false;
   bool listAllNodes = false;

   if (args.isFlagSet("query"))
   {
      setResult(CMayaScene::IsActive());
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
   if (listAllNewNodes && AiUniverseIsActive())
   {
      // if requested, loop over previously existing arnold nodes in the scene, so
      // that we can find out the newly created nodes after export
      AtNodeIterator* nodeIter = AiUniverseGetNodeIterator(AI_NODE_ALL);
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
      if (!CMayaScene::IsActive())
         CMayaScene::Begin(MTOA_SESSION_ASS);

      // export with an empty selection list so that options, etc...
      // are properly initialized
      MSelectionList list;
      CMayaScene::Export(&list);
   }
   else if (mode == "destroy")
   {
      if (CMayaScene::IsActive())
         CMayaScene::End();

   }
   else if (mode == "convert_scene")
   {
      if (!CMayaScene::IsActive())
         CMayaScene::Begin(MTOA_SESSION_ASS);

      CMayaScene::Export();
   }
   // Ok this is tricky. "convert_selected" is going to call CMayaScene::Export(&sList) which will trigger the options export.
   // But we need another mode that only exports each of the selected nodes independantly. We're calling this new mode "convert_selected_only".
   // Yes, it sucks.... but we don't want to break existing workflows using "convert_selected"
   else if (mode == "convert_selected" || mode == "convert_selected_only" ) 
   {
      if (!CMayaScene::IsActive())
         CMayaScene::Begin(MTOA_SESSION_ASS);
      MSelectionList sList;
      MStringArray sListStrings;
      args.getObjects(sListStrings);   

      const unsigned int sListStringsLength = sListStrings.length();


      sList.clear();
      if (sListStringsLength > 0)
      {
         for (unsigned int i = 0; i < sListStringsLength; ++i)
         {
            sList.add(sListStrings[i]);
         }
      }
      else
         MGlobal::getActiveSelectionList(sList);

      MSelectionList sel(sList);

      if (mode == "convert_selected")
         CMayaScene::Export(&sList); // do NOT call this function for "convert_selected_only", because we don't want to export the options

      CArnoldSession *session = CMayaScene::GetArnoldSession();

      // Even though we called Export(&sList) we must ensure that each of the selected nodes was properly exported.
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
               MPlug shaderPlug = MFnDependencyNode(objNode).findPlug("message", true);
               if (!shaderPlug.isNull())
               {
                 tr = session->ExportNode(shaderPlug, 
                                false, 0);
               }
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
   if (listAllNewNodes || listAllNodes)
   {
      AtNodeIterator* nodeIter = AiUniverseGetNodeIterator(AI_NODE_ALL);
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
