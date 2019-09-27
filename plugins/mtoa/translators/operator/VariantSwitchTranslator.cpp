#include "VariantSwitchTranslator.h"
#include "utils/Universe.h"
#include "translators/NodeTranslatorImpl.h"
#include "translators/DagTranslator.h"
#include <maya/MPlugArray.h>


void CVariantSwitchTranslator::Export(AtNode *shader)
{
   MStatus status;

   // loop the variant entries
   MPlug index = FindMayaPlug("index");
   AiNodeSetInt(shader, "index", index.asInt());

   // loop the variant entries
   MPlug variants = FindMayaPlug("variants");
   unsigned nvars = variants.numElements();
   MPlug varPlug;
   AtArray* varArray = AiArrayAllocate(nvars, 1, AI_TYPE_NODE);

   // should we just export the merge node from the current index?

   for (unsigned int i = 0; i < nvars; ++i)
   {
      varPlug = variants[i];
      // get variant name
      MPlug name = varPlug.child(0, &status);
      MString name_str = name.asString();

      MPlug inputs = varPlug.child(1, &status);
      // get connections

      MString mergeName = GetMayaNodeName() + "_" + name_str + i;

      AtNode* mergeNode = GetArnoldNode(mergeName.asChar());
      if (mergeNode == NULL)
         mergeNode = AddArnoldNode("merge", mergeName.asChar());

      // Deal with inputs now
      unsigned nelems = inputs.numElements();
      if (nelems > 0)
      {
         MPlug elemPlug;
         AtArray* array = AiArrayAllocate(nelems, 1, AI_TYPE_NODE);

         for (unsigned int e = 0; e < nelems; ++e)
         {
            elemPlug = inputs[e];
            MPlugArray connections;
            elemPlug.connectedTo(connections, true, false);
            AtNode* linkedNode = NULL;
            if (connections.length() > 0)
            {
               linkedNode = ExportConnectedNode(connections[0]);
            }
            if (linkedNode)
            {
               AiArraySetPtr(array, e, (void*)linkedNode);
            }
         }

         AiNodeSetArray(mergeNode, "inputs", array);
      }

      AiArraySetPtr(varArray, i, (void*)mergeNode);
   }

   // if (needUpdate) // only set the parameter if inputs have actually changed
   AiNodeSetArray(shader, "inputs", varArray);
}

void CVariantSwitchTranslator::NodeInitializer(CAbTranslator context)
{
}
