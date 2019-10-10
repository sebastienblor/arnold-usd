#include "OperatorTranslator.h"
#include "utils/Universe.h"
#include "translators/NodeTranslatorImpl.h"
#include "translators/DagTranslator.h"
#include <maya/MPlugArray.h>

static AtString set_parameter_str("set_parameter");

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

   if (AiNodeIs(shader, set_parameter_str ))
      ExportAssignedShaders(shader);      

}
void COperatorTranslator::NodeInitializer(CAbTranslator context)
{   
}

void COperatorTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   SetUpdateMode(AI_RECREATE_NODE); // I need to re-generate the shaders, so that they include the matte at the root of the shading tree

   CNodeTranslator::NodeChanged(node, plug);
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
         continue;

      MString attrName = assignmentSplit[0].substringW(0, 8);

      MStringArray paramSplit;
      assignmentSplit[0].split(' ', paramSplit);
      if (paramSplit.length() > 1)
         attrName = paramSplit[1].substringW(0, 8);

      attrName.substitute(" ", "");

      if (attrName != MString("shader") && attrName != MString("disp_map"))
         continue; // here we only care about shader and disp_map assignments

      std::string attrValue = assignmentSplit[1].asChar(); // this is meant to be the name of the arnold shader
      while (attrValue.length() > 0 && (attrValue[0] == ' ' || attrValue[0] == '\'' || attrValue[0] == '"'))
         attrValue.erase(attrValue.begin());

      while (attrValue.length() > 0 && (attrValue[attrValue.length() - 1] == ' ' || attrValue[attrValue.length() - 1] == '\'' || attrValue[attrValue.length() - 1] == '"'))
         attrValue.erase(attrValue.end() -1);

      MSelectionList sel;
      sel.add(MString(attrValue.c_str()));
      if (sel.length() == 0)
         continue; // the referenced shader wasn't found in the maya scene

      MObject shaderNode;
      sel.getDependNode(0,shaderNode);
      if (shaderNode.isNull())
         continue;
      MPlug dummyPlug = MFnDependencyNode(shaderNode).findPlug("message", true);
      if (dummyPlug.isNull())
         continue;

      ExportConnectedNode(dummyPlug); // do export the shader
   }
}
