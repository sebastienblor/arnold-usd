#include "utils/Version.h"
#include "ArnoldExportImagersCmd.h"
#include "translators/NodeTranslator.h"
#include "session/ArnoldExportSession.h"
#include "session/SessionManager.h"
#include "utils/ConstantStrings.h"

#include <maya/MStatus.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MFnSet.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnAttribute.h>
#include <maya/MStringArray.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <maya/MPlugArray.h>

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>
#include <ai_bbox.h>
//#include <ai_materialx.h>
#include <math.h>
#include <algorithm>

static const std::string s_exportImagersSessionId("exportImagers");

MSyntax CArnoldExportImagersCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("f", "filename", MSyntax::kString);
   syntax.setObjectType(MSyntax::kStringObjects);
   return syntax;
}

MStatus CArnoldExportImagersCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MSyntax syntax = newSyntax();

   // we must use an MArgParser because MArgList is not python compatible, and we
   // use this result to set syntax.useSelectionAsDefault() prior to creating the MArgDatabase.
   MArgParser args(syntax, argList, &status);
   MArgDatabase argDB(syntax, argList, &status);

   if (!argDB.isFlagSet("filename"))
   {
      AiMsgError("[mtoa] Export Imagers : No filename specified");
      return MS::kFailure;
   }
   
   CArnoldExportSession *session = new CArnoldExportSession();

   if (!CSessionManager::AddActiveSession(s_exportImagersSessionId, session))
   {
      delete session;
      return MS::kFailure;
   }
   CSessionOptions &options = session->GetOptions();
   AtUniverse *universe = session->GetUniverse();
   MString filename = "";
   argDB.getFlagArgument("filename", 0, filename);

   AtNode *target = nullptr;

   // I want to get the target operator in the options
   MSelectionList list;
   MObject node;
   list.add("defaultArnoldRenderOptions");
   if (list.length() == 0)
      return MS::kFailure;

   list.getDependNode(0, node);
   MFnDependencyNode fnArnoldRenderOptions(node);
   MPlug imagersPlug = fnArnoldRenderOptions.findPlug("imagers", true);
   if (imagersPlug.isNull())
      return MS::kFailure;
   
   unsigned numImagers = imagersPlug.numElements();
   std::vector<AtNode*> imagersStack;
   imagersStack.reserve(numImagers);
   MPlugArray conns;
   // Process the stack of imagers that need to be rendered
   for (unsigned int i = 0; i < numImagers; ++i)
   {
      MPlug imagerPlug = imagersPlug[i];
      conns.clear();
      imagerPlug.connectedTo(conns, true, false);
      CNodeTranslator *imgTranslator = (conns.length() > 0) ?
         session->ExportNode(conns[0]) : nullptr;
      AtNode *imgNode = (imgTranslator) ? imgTranslator->GetArnoldNode() : nullptr;
      if (imgNode)
      {
         // When translating the stack to a graph, we need to start from last one, then 
         // successively connect the imagers as input of the previous, until the first one.
         // This will make it so that the last imager in the list is applied at the end by arnold
         imagersStack.insert(imagersStack.begin(), imgNode);
      }
   }

   // Process the imagers tree, by connecting them through the attribute "input"
   if (!imagersStack.empty())
   {  
      for (size_t i = 0; i < imagersStack.size() - 1; ++i)
      {
         AiNodeSetPtr(imagersStack[i], str::input, (void*)imagersStack[i+1]); 
      }
      // Ensure the last imager in the stack doesn't have any input from a previous render
      AiNodeResetParameter(imagersStack.back(), str::input);
   }

   
   AtParamValueMap* params = AiParamValueMap();
   AiParamValueMapSetInt(params, str::mask, AI_NODE_DRIVER);
   AiSceneWrite(universe, filename.asChar(), params);
   AiParamValueMapDestroy(params);

   CSessionManager::DeleteActiveSession(s_exportImagersSessionId);   
   return MS::kSuccess;
}


