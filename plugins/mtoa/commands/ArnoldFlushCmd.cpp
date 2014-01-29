#include "ArnoldFlushCmd.h"

#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>

#include <ai_universe.h>

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

MStatus CArnoldFlushCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MArgDatabase args(syntax(), argList);

   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   if (renderSession != NULL)
      renderSession->InterruptRender();
   
   if (args.isFlagSet("textures"))
   {
      if (!AiUniverseIsActive())
      {
         AiBegin();
         AiUniverseCacheFlush(AI_CACHE_TEXTURE);
         AiEnd();
      }
      else
         AiUniverseCacheFlush(AI_CACHE_TEXTURE);
   }
   
   if (args.isFlagSet("skydome"))
   {
      AiUniverseCacheFlush(AI_CACHE_BACKGROUND);
   }
   
   if (args.isFlagSet("quads"))
   {
      AiUniverseCacheFlush(AI_CACHE_QUAD);
   }
   
   if (args.isFlagSet("flushall"))
   {
      if (!AiUniverseIsActive())
      {
         AiBegin();
         AiUniverseCacheFlush(AI_CACHE_ALL);
         AiEnd();
      }
      else
         AiUniverseCacheFlush(AI_CACHE_ALL);
   }
   
   return MS::kSuccess;
}

