#include "ArnoldFlushCmd.h"

#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>

#include <ai_universe.h>
#include "utils/MakeTx.h"

#include "scene/MayaScene.h"

MSyntax CArnoldFlushCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("t", "textures", MSyntax::kBoolean);
   syntax.addFlag("s", "skydome", MSyntax::kBoolean);
   syntax.addFlag("q", "quads", MSyntax::kBoolean);
   syntax.addFlag("fa", "flushall", MSyntax::kBoolean);

   return syntax;
}
static void FlushInvalidateConnectedTextures(AtNode *node)
{
   if(node == NULL) return;

   if(AiNodeIs(node, "MayaFile") || AiNodeIs(node, "image"))
   {
      // this is an image node
      MString filename = AiNodeGetStr(node, "filename");
      MStringArray expandedFilenames = expandFilename(filename);
      for (size_t i = 0; i < expandedFilenames.length(); ++i)
         AiTextureInvalidate(expandedFilenames[i].asChar());
      
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

   if (!AiUniverseIsActive()) return MS::kSuccess;

   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   if (renderSession != NULL)
      renderSession->InterruptRender();
   
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
         if (!AiNodeIs(node, "skydome_light") ) continue;

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
   
   return MS::kSuccess;
}

