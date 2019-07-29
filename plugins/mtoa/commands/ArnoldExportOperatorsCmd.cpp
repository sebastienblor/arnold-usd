#include "utils/Version.h"
#include "ArnoldExportOperatorsCmd.h"
#include "scene/MayaScene.h"
#include "translators/NodeTranslator.h"

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

MSyntax CArnoldExportOperatorsCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("f", "filename", MSyntax::kString);
   syntax.addFlag("shd", "shaders",  MSyntax::kBoolean);
   syntax.addFlag("sel", "selection",  MSyntax::kBoolean);   
   syntax.setObjectType(MSyntax::kStringObjects);
   return syntax;
}

MStatus CArnoldExportOperatorsCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MSyntax syntax = newSyntax();

   // we must use an MArgParser because MArgList is not python compatible, and we
   // use this result to set syntax.useSelectionAsDefault() prior to creating the MArgDatabase.
   MArgParser args(syntax, argList, &status);
   MArgDatabase argDB(syntax, argList, &status);

   if (!argDB.isFlagSet("filename"))
   {
      AiMsgError("[mtoa] Export Operator Graph : No filename specified");
      return MS::kFailure;
   }

   MSelectionList selected; // list of selected objects to be exported
   bool exportSelected = false;
   if (argDB.isFlagSet("selection")) 
      argDB.getFlagArgument("selection", 0, exportSelected);

   
   if (exportSelected)
   {      
      MStringArray sListStrings;
      argDB.getObjects(sListStrings);
      const unsigned int sListStringsLength = sListStrings.length();
      
      if (sListStringsLength > 0)
      {
         for (unsigned int i = 0; i < sListStringsLength; ++i)
            selected.add(sListStrings[i]);
      }
      else
         MGlobal::getActiveSelectionList(selected);

      // do we want to export the whole scene if nothing is selected ?
      if (selected.length() == 0)
      {
         AiMsgError("[mtoa] Nothing selected");
         return MS::kFailure;
      }
   }

   CMayaScene::End();
   // Cannot export while a render is active
   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] Cannot Export Operator Graph while rendering");
      return MS::kFailure;
   }

   MString filename = "";
   argDB.getFlagArgument("filename", 0, filename);

   bool exportShaders = false;
   if (argDB.isFlagSet("shaders")) 
      argDB.getFlagArgument("shaders", 0, exportShaders);

   CMayaScene::Begin(MTOA_SESSION_ASS);
   CArnoldSession* arnoldSession = CMayaScene::GetArnoldSession();
   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   arnoldSession->SetExportFilterMask(AI_NODE_ALL);
   std::vector<AtNode *> targets;

   if (!exportSelected)
   {
      // I want to get the target operator in the options
      MSelectionList list;
      MObject node;
      list.add("defaultArnoldRenderOptions");
      if (list.length() > 0)
      {
         list.getDependNode(0, node);
         MFnDependencyNode fnArnoldRenderOptions(node);
         MPlug operatorPlug = fnArnoldRenderOptions.findPlug("operator", true);
         if (!operatorPlug.isNull())
         {
            MPlugArray conns;
            operatorPlug.connectedTo(conns, true, false);
            if (conns.length() > 0)
            {

               CNodeTranslator *opTranslator = arnoldSession->ExportNode(conns[0]);
               if (opTranslator)
                  targets.push_back(opTranslator->GetArnoldNode());
            }
         }
      }
   } else
   {
      // Export the selected operators. Also consider selections to procedurals
      for (unsigned int i = 0; i < selected.length(); ++i)
      {
         MStatus listStatus;
         MDagPath dag;
         MObject objNode;
         CNodeTranslator *tr = NULL;
         if (selected.getDagPath(i, dag) == MS::kSuccess)
         {
            dag.extendToShape();
            MStatus tmpStatus;
            MFnDependencyNode fnShape(dag.node(), &tmpStatus);
            if (tmpStatus == MS::kSuccess)
            {
               MPlug ops = fnShape.findPlug("operators");
               if (!ops.isNull())
               {
                  std::vector<AtNode *> procOps;
                  // This procedural has an operators attribute, let's export its operators
                  unsigned nelems = ops.numElements();
                  MPlug elemPlug;
                  for (unsigned int i = 0; i < nelems; ++i)
                  {
                     elemPlug = ops[i];
                     MPlugArray connections;
                     elemPlug.connectedTo(connections, true, false);
                     if (connections.length() > 0)
                     {
                        MObject opNode = connections[0].node();
                        MString name = CNodeTranslator::GetArnoldNaming(opNode);
                        AtNode *op = AiNodeLookUpByName(name.asChar());

                        if (op)
                           procOps.push_back(op);
                        else
                        {
                           CNodeTranslator *tr = arnoldSession->ExportNode(connections[0]);
                           op = (tr) ? tr->GetArnoldNode() : NULL;
                           if (op)
                              procOps.push_back(op); // append the operator node to the list 
                        }

                     }
                  }
                  if (procOps.size() == 1)
                     targets.insert(targets.begin(), procOps[0]);
                  else if (procOps.size() > 1)
                  {
                     // need to insert a merge op
                     MString mergeOpName = fnShape.name();
                     mergeOpName += "@input_merge_op";
                     AtNode *mergeOp = AiNode(NULL, "merge", mergeOpName.asChar());
                     AtArray* opArray = AiArrayAllocate(procOps.size(), 1, AI_TYPE_NODE);
                     for (unsigned int i = 0; i < procOps.size(); ++i)
                        AiArraySetPtr(opArray, i, (void*)procOps[i]);
                     AiNodeSetArray(mergeOp, "inputs", opArray);
                     targets.insert(targets.begin(), mergeOp); // add the merge as target operator
                  }
               }
            }
         } else if (selected.getDependNode(i, objNode) == MS::kSuccess)
         {
            // Check if the node is an operator
            MFnDependencyNode fnNode(objNode);
            MPlug opPlug = fnNode.findPlug("message", true);

            MString name = CNodeTranslator::GetArnoldNaming(objNode);
            if (!AiNodeLookUpByName(name.asChar()))
            {
               CNodeTranslator *tr = arnoldSession->ExportNode(opPlug, false, 0);
               AtNode *opNode = (tr) ? tr->GetArnoldNode() : NULL;
               if (opNode && AI_NODE_OPERATOR == AiNodeEntryGetType(AiNodeGetNodeEntry(opNode)))
                  targets.push_back(opNode);
            }
         }
      }
   }
   // Now, if I have multiple targets, we need to choose one. 
   // Let's pick the first for now
   if (!targets.empty())
   {
      if (AiNodeDeclare(targets[0], "is_target", "constant BOOL"))
         AiNodeSetBool(targets[0], "is_target", true);  
    
   }
   AiASSWrite(NULL, filename.asChar(), (exportShaders) ? AI_NODE_OPERATOR | AI_NODE_SHADER : AI_NODE_OPERATOR);


   CMayaScene::End();
   return MS::kSuccess;
}


