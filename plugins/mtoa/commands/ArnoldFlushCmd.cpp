#include "ArnoldFlushCmd.h"

#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>

#include "session/SessionManager.h"

#include <ai_universe.h>
#include <ai_nodes.h>
#include <ai_texture.h>

#include "utils/MakeTx.h"
#include "utils/MtoaLog.h"
#include "utils/ConstantStrings.h"

MSyntax CArnoldFlushCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("t", "textures", MSyntax::kBoolean);
   syntax.addFlag("s", "skydome", MSyntax::kBoolean);
   syntax.addFlag("st", "selected_textures", MSyntax::kBoolean);
   syntax.addFlag("q", "quads", MSyntax::kBoolean);
   syntax.addFlag("fa", "flushall", MSyntax::kBoolean);

   syntax.setObjectType(MSyntax::kStringObjects);
   return syntax;
}
static void FlushInvalidateConnectedTextures(AtNode *node)
{
   if(node == NULL) return;

   if(AiNodeIs(node, str::image))
   {
      // this is an image node
      MString filename = AiNodeGetStr(node, str::filename).c_str();
      MStringArray expandedFilenames = expandFilename(filename);
      for (unsigned int i = 0; i < expandedFilenames.length(); ++i)
      {
         if (MtoaTranslationInfo())
            MtoaDebugLog("[mtoa.flush] Flushing texture "+ expandedFilenames[i]);
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
   
   // Initialize command syntax and get flags
   MSyntax syntax = newSyntax();
   // we must use an MArgParser because MArgList is not python compatible, and we
   // use this result to set syntax.useSelectionAsDefault() prior to creating the MArgDatabase.
   MStatus status;
   MArgParser args(syntax, argList, &status);
   bool exportSelected = args.isFlagSet("selected_textures");
   MArgDatabase argDB(syntax, argList, &status);


   if (args.isFlagSet("textures"))
      AiUniverseCacheFlush(nullptr, AI_CACHE_TEXTURE);
   
   if (args.isFlagSet("skydome"))
   {
      AiUniverseCacheFlush(nullptr, AI_CACHE_BACKGROUND);
      auto activeSessions = CSessionManager::GetActiveSessions();
      for (auto sessionIter : activeSessions)
      {
         CArnoldSession *session = sessionIter.second;
         if (session == nullptr)
            continue;
         AtUniverse *universe = session->GetUniverse();
         // also invalidate the textures connected to the skydome's color (#2541)
         AtNodeIterator* nodeIter = AiUniverseGetNodeIterator(universe, AI_NODE_LIGHT);
         while (!AiNodeIteratorFinished(nodeIter))
         {
            AtNode *node = AiNodeIteratorGetNext(nodeIter);
            if (!AiNodeIs(node, str::skydome_light)) continue;

            if (!AiNodeIsLinked(node, "color")) continue;
            AtNode *link = AiNodeGetLink(node, "color");
            FlushInvalidateConnectedTextures(link);
         }
         AiNodeIteratorDestroy(nodeIter);
      }
   }
   
   // Note that we flush the cache on the default universe, as this will
   // currently do it for all active universes
   if (args.isFlagSet("quads"))
      AiUniverseCacheFlush(nullptr, AI_CACHE_QUAD);
   
   if (args.isFlagSet("flushall"))
      AiUniverseCacheFlush(nullptr, AI_CACHE_ALL);
   
   // FIXME : we're not looking at the right universe for the "selected" node
   if (exportSelected)
   {
      MSelectionList activeList;
      MStringArray sListStrings;
      argDB.getObjects(sListStrings);
      const unsigned int sListStringsLength = sListStrings.length();
      if (sListStringsLength > 0)
      {
         for (unsigned int i = 0; i < sListStringsLength; ++i)
            activeList.add(sListStrings[i]);
      }
      else
         MGlobal::getActiveSelectionList(activeList);
   
      for (unsigned int i = 0; i < activeList.length(); ++i)
      {
         MObject depNode;
         activeList.getDependNode(i, depNode);

         if (depNode.hasFn(MFn::kTransform))
         {
            // from Transform to Shape
            MDagPath dagPath;
            activeList.getDagPath(i, dagPath);
            depNode = dagPath.child(0);
         }
         MFnDependencyNode nodeFn( depNode );

         // note that this considers the node has the same name in maya and arnold
         // based on the same code as ARV "shade with selected"
         AtNode *selected = AiNodeLookUpByName(nullptr, AtString(nodeFn.name().asChar()));

         if (selected)
         {
            if (AiNodeIs(selected, str::image))
            {
               MString filename = AiNodeGetStr(selected, str::filename).c_str();
               MStringArray expandedFilenames = expandFilename(filename);
               for (unsigned int i = 0; i < expandedFilenames.length(); ++i)
               {
                  if (MtoaTranslationInfo())
                     MtoaDebugLog("[mtoa.flush] Flushing texture " + expandedFilenames[i]);

                  AiTextureInvalidate(expandedFilenames[i].asChar());
               }
            }
         }
      }
      
   }

   return MS::kSuccess;
}

