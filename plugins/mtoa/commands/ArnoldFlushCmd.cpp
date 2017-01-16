#include "ArnoldFlushCmd.h"

#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>

#include <ai_universe.h>
#include "utils/MakeTx.h"

#include "scene/MayaScene.h"


static const AtString MayaFile_str("MayaFile");
static const AtString image_str("image");
static const AtString skydome_light_str("skydome_light");
static const AtString MayaImagePlane_str("MayaImagePlane");


MSyntax CArnoldFlushCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("t", "textures", MSyntax::kBoolean);
   syntax.addFlag("s", "skydome", MSyntax::kBoolean);
   syntax.addFlag("st", "selected_textures", MSyntax::kBoolean);
   syntax.addFlag("q", "quads", MSyntax::kBoolean);
   syntax.addFlag("fa", "flushall", MSyntax::kBoolean);

   return syntax;
}
static void FlushInvalidateConnectedTextures(AtNode *node)
{
   if(node == NULL) return;

   if(AiNodeIs(node, MayaFile_str) || AiNodeIs(node, image_str))
   {
      // this is an image node
      MString filename = AiNodeGetStr(node, "filename").c_str();
      MStringArray expandedFilenames = expandFilename(filename);
      for (unsigned int i = 0; i < expandedFilenames.length(); ++i)
      {
         AiMsgDebug("[mtoa.flush] Flushing texture %s", expandedFilenames[i].asChar());
         AiTextureInvalidate(expandedFilenames[i].asChar());
      }
      
      return;
   }
   // loop over all attributes
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(node));
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      if (!AiNodeIsLinked(node, paramName)) continue; // only consider linked parameters

      FlushInvalidateConnectedTextures(AiNodeGetLink(node, paramName));      
   }
   AiParamIteratorDestroy(nodeParam);
}

MStatus CArnoldFlushCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MArgDatabase args(syntax(), argList);
  
   
   if (args.isFlagSet("textures"))
      AiUniverseCacheFlush(AI_CACHE_TEXTURE);
   
   if (args.isFlagSet("skydome"))
   {
      AiUniverseCacheFlush(AI_CACHE_BACKGROUND);

      // also invalidate the textures connected to the skydome's color (#2541)
      AtNodeIterator* nodeIter = AiUniverseGetNodeIterator(AI_NODE_LIGHT);
      while (!AiNodeIteratorFinished(nodeIter))
      {
         AtNode *node = AiNodeIteratorGetNext(nodeIter);
         if (!AiNodeIs(node, skydome_light_str)) continue;

         if (!AiNodeIsLinked(node, "color")) continue;
         AtNode *link = AiNodeGetLink(node, "color");
         FlushInvalidateConnectedTextures(link);
      }
      AiNodeIteratorDestroy(nodeIter);
   }
   
   if (args.isFlagSet("quads"))
      AiUniverseCacheFlush(AI_CACHE_QUAD);
   
   if (args.isFlagSet("flushall"))
      AiUniverseCacheFlush(AI_CACHE_ALL);
   
   if (args.isFlagSet("selected_textures"))
   {
      MSelectionList activeList;
      MGlobal::getActiveSelectionList(activeList);
      for (unsigned int i = 0; i < activeList.length(); ++i)
      {
         MObject depNode;
         activeList.getDependNode(i, depNode);
         if (depNode.hasFn(MFn::kTransform))
         {
            // from Transform to Shape
            MDagPath dagPath;
            activeList.getDagPath(0, dagPath);
            depNode = dagPath.child(0);
         }
         MFnDependencyNode nodeFn( depNode );

         // note that this considers the node has the same name in maya and arnold
         // based on the same code as ARV "shade with selected"
         AtNode *selected = AiNodeLookUpByName(nodeFn.name().asChar());

         if (selected)
         {
            if (AiNodeIs(selected, MayaFile_str) || AiNodeIs(selected, image_str) ||  AiNodeIs(selected, MayaImagePlane_str))
            {
               MString filename = AiNodeGetStr(selected, "filename").c_str();
               MStringArray expandedFilenames = expandFilename(filename);
               for (unsigned int i = 0; i < expandedFilenames.length(); ++i)
               {
                  AiMsgDebug("[mtoa.flush] Flushing texture %s", expandedFilenames[i].asChar());
                  AiTextureInvalidate(expandedFilenames[i].asChar());
               }
            }
         }
      }
      
   }
   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   if (renderSession != NULL && AiUniverseIsActive())
      renderSession->InterruptRender();

   return MS::kSuccess;
}

