#include "utils/Version.h"
#include "ArnoldSceneCmd.h"
#include "scene/MayaScene.h"
#include "utils/MakeTx.h"

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

#include "../common/UnorderedContainer.h"

#include <math.h>

MSyntax CArnoldSceneCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("m", "mode", MSyntax::kString);
   syntax.addFlag("lo", "listObjects");
   syntax.setObjectType(MSyntax::kStringObjects);
   return syntax;
}



MStatus CArnoldSceneCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MArgDatabase args(syntax(), argList);

   MStringArray result;

   bool listObjects = args.isFlagSet("listObjects");

   unordered_set<std::string> previousObjects;
   if (listObjects && AiUniverseIsActive())
   {
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

   MString mode = (args.isFlagSet("mode")) ? args.flagArgumentString("mode", 0) : "create";

   if (mode == "create")
   {
      if (!CMayaScene::IsActive())
         CMayaScene::Begin(MTOA_SESSION_ASS);

      // export with an empty selection list so that options, etc...
      // are properly initialized
      MSelectionList list;
      CMayaScene::Export(&list);
      listObjects = false;
   }
   else if (mode == "destroy")
   {
      if (CMayaScene::IsActive())
         CMayaScene::End();

      listObjects = false;
   }
   else if (mode == "convert_scene")
   {
      if (!CMayaScene::IsActive())
         CMayaScene::Begin(MTOA_SESSION_ASS);

      CMayaScene::Export();
   }
   else if (mode == "convert_selected")
   {
      MSelectionList sList;
      MStringArray sListStrings;
      args.getObjects(sListStrings);   

      const unsigned int sListStringsLength = sListStrings.length();

      sList.clear();
      if (sListStringsLength > 0)
      {
         for (unsigned int i = 0; i < sListStringsLength; ++i)
            sList.add(sListStrings[i]);
      }
      else
         MGlobal::getActiveSelectionList(sList);
      CMayaScene::Export(&sList);

   }
   if (listObjects)
   {
      AtNodeIterator* nodeIter = AiUniverseGetNodeIterator(AI_NODE_ALL);
      while (!AiNodeIteratorFinished(nodeIter))
      {
         AtNode *node = AiNodeIteratorGetNext(nodeIter);
         if (node == NULL || AiNodeIsDisabled(node)) continue;
         std::string nodeName = AiNodeGetName(node);
         if (nodeName.empty()) continue;
         // if this node wasn't already in the previous list, add it
         if (previousObjects.find(nodeName) == previousObjects.end())
            result.append(nodeName.c_str());

      }
      AiNodeIteratorDestroy(nodeIter);
   }
   setResult(result);
   return MS::kSuccess;
}
