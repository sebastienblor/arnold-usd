#include "OperatorTranslator.h"
#include "utils/Universe.h"
#include "translators/NodeTranslatorImpl.h"
#include "translators/DagTranslator.h"
#include <maya/MPlugArray.h>

AtNode* COperatorTranslator::CreateArnoldNodes()
{
   return AddArnoldNode(m_impl->m_abstract.arnold.asChar());   
}

void COperatorTranslator::Export(AtNode *shader)
{
   MStatus status;
   const AtNodeEntry* entry = AiNodeGetNodeEntry(shader);
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(entry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      std::string paramNameStr(paramName);

      if (paramNameStr != "name" && paramNameStr != "inputs")
         ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
   }
   AiParamIteratorDestroy(nodeParam);

   // Deal with inputs now
   MPlug inputs = FindMayaPlug("inputs");
   unsigned nelems = inputs.numElements();
   MPlug elemPlug;
   AtArray* array = AiArrayAllocate(nelems, 1, AI_TYPE_NODE);
      
   for (unsigned int i = 0; i < nelems; ++i)
   {
      elemPlug = inputs[i];       
      
      MPlugArray connections;
      elemPlug.connectedTo(connections, true, false);
      AtNode* linkedNode = NULL;
      if (connections.length() > 0)
      {
         linkedNode = ExportConnectedNode(connections[0]);
      }
      if (linkedNode)
      {
         AiArraySetPtr(array, i, (void*)linkedNode);
      }
   }

   AtArray *prevArray = AiNodeGetArray(shader, "inputs");
   bool needUpdate = true;
   if (prevArray)
   {
      unsigned prevArrayElems = AiArrayGetNumElements(prevArray);
      if (prevArrayElems == nelems)
      {
         needUpdate = false;
         for (unsigned i = 0; i < nelems; ++i)
         {
            if (AiArrayGetPtr(array, i) != AiArrayGetPtr(prevArray, i))
            {
               needUpdate = true;
               break;
            }
         }
      }
   }

   if (needUpdate) // only set the parameter if inputs have actually changed
      AiNodeSetArray(shader, "inputs", array);

   static AtString set_parameter_str("set_parameter");
   if (AiNodeIs(shader, set_parameter_str ))
      ExportAssignedShaders(shader);      


   // MPlug outPlug = FindMayaPlug("out");
   // MPlugArray outConn;
   // outPlug.connectedTo(outConn, false, true);
   // outPlug = FindMayaPlug("message");
   // MPlugArray messageConn;
   // outPlug.connectedTo(messageConn, false, true);
   // for (unsigned int i = 0; i < messageConn.length(); ++i)
   //    outConn.append(messageConn[i]);

   //bool globalOp = false;
   MStringArray procList;

   // for (unsigned int i = 0; i < outConn.length(); ++i)
   // {
   //    MStatus retStat;
   //    MObject targetObj(outConn[i].node());
   //    MFnDependencyNode target(targetObj, &retStat);
   //    if (retStat != MS::kSuccess)
   //       continue;

   //    MString plugName = outConn[i].name();

   //    // TODO : need to do the loop recursively until a procedural is found. 
   //    // We're currently not supporting sub-graphs connected to procedurals
   //    plugName = plugName.substring(plugName.rindex('.'), plugName.length()-1);
   //    if (plugName.length() >= 10 && plugName.substringW(0, 9) == MString(".operators"))
   //    {
   //       MDagPath dagPath;      
   //       if (MDagPath::getAPathTo(targetObj, dagPath) == MS::kSuccess)
   //       {
   //          MString dagName(CDagTranslator::GetArnoldNaming(dagPath));
   //          if (dagName.length() > 0)
   //          {
   //             procList.append(dagName);
   //          }
   //       }
   //    } 
   // }
   procList = WalkOutputs(GetMayaObject());

   if (procList.length() > 0)
   {
      MPlug selectionPlug = FindMayaPlug("selection");
      if (!selectionPlug.isNull())
      {
         MString selection = selectionPlug.asString();
         MString finalSelection;
         //if (globalOp)
            //finalSelection = selection;

         for (unsigned int i = 0; i < procList.length(); ++i)
         {
            if (finalSelection.length() > 0)
               finalSelection += MString(" or ");

            finalSelection += procList[i] + MString("*") + selection;
         }
         AiNodeSetStr(shader, "selection", AtString(finalSelection.asChar()));
      }
      
   }
   // add standin name
}
void COperatorTranslator::NodeInitializer(CAbTranslator context)
{   
}


MStringArray COperatorTranslator::WalkOutputs(MObject obj)
{
   MStatus status(MStatus::kSuccess);

   MFnDependencyNode fnNode(obj);

   MPlug outPlug = fnNode.findPlug("out", &status);;
   MPlugArray outConn;
   outPlug.connectedTo(outConn, false, true);
   outPlug = fnNode.findPlug("message", &status);
   MPlugArray messageConn;
   outPlug.connectedTo(messageConn, false, true);
   for (unsigned int i = 0; i < messageConn.length(); ++i)
      outConn.append(messageConn[i]);

   //bool globalOp = false;
   MStringArray procList;

   for (unsigned int i = 0; i < outConn.length(); ++i)
   {
      MStatus retStat;
      MObject targetObj(outConn[i].node());
      MFnDependencyNode target(targetObj, &retStat);
      if (retStat != MS::kSuccess)
         continue;

      MString plugName = outConn[i].name();

      // TODO : need to do the loop recursively until a procedural is found. 
      // We're currently not supporting sub-graphs connected to procedurals
      plugName = plugName.substring(plugName.rindex('.'), plugName.length()-1);
      if (plugName.length() >= 10 && plugName.substringW(0, 9) == MString(".operators"))
      {
         MDagPath dagPath;      
         if (MDagPath::getAPathTo(targetObj, dagPath) == MS::kSuccess)
         {
            MString dagName(CDagTranslator::GetArnoldNaming(dagPath));
            if (dagName.length() > 0)
            {
               procList.append(dagName);
            }
         }
      } else {
         MStringArray child_procList = WalkOutputs(targetObj);
         for (unsigned int c = 0; c < child_procList.length(); ++c)
            procList.append(child_procList[c]);
      }
   }

   return procList;
}

// special case for set_parameter operators, we want to export the nodes which are eventually referenced.
// We could derive operator translator classes to handle this, but for now it's still quite simple
void COperatorTranslator::ExportAssignedShaders(AtNode *shader)
{
   AtArray *assignments = AiNodeGetArray(shader, "assignment");
   if (assignments == NULL)
      return;
   unsigned int numAssignments = AiArrayGetNumElements(assignments);
   for (unsigned int i = 0; i < numAssignments; ++i)
   {
      AtString assignmentStr = AiArrayGetStr(assignments, i);
      MString assignment(assignmentStr.c_str());
      MStringArray assignmentSplit;
      assignment.split('=', assignmentSplit);
      if (assignmentSplit.length() <= 1)
         return;

      MString attrName = assignmentSplit[0].substringW(0, 5);
      if (attrName != MString("shader"))
         return; // here we only care about shader assignments

      std::string attrValue = assignmentSplit[1].asChar(); // this is meant to be the name of the arnold shader
      while (attrValue.length() > 0 && (attrValue[0] == ' ' || attrValue[0] == '\'' || attrValue[0] == '"'))
         attrValue.erase(attrValue.begin());

      while (attrValue.length() > 0 && (attrValue[attrValue.length() - 1] == ' ' || attrValue[attrValue.length() - 1] == '\'' || attrValue[attrValue.length() - 1] == '"'))
         attrValue.erase(attrValue.end() -1);

      MSelectionList sel;
      sel.add(MString(attrValue.c_str()));
      if (sel.length() == 0)
         return; // the referenced shader wasn't found in the maya scene
      
      MObject shaderNode;
      sel.getDependNode(0,shaderNode);
      if (shaderNode.isNull())
         return;
      MPlug dummyPlug = MFnDependencyNode(shaderNode).findPlug("message");
      if (dummyPlug.isNull())
         return;
      
      ExportConnectedNode(dummyPlug); // do export the shader
   }
}