#include "LookSwitchTranslator.h"
#include "utils/Universe.h"
#include "translators/NodeTranslatorImpl.h"
#include "translators/DagTranslator.h"
#include <maya/MPlugArray.h>

static const AtString s_inputs("inputs");
static const AtString s_inputs_enable("inputs_enable");

void CLookSwitchTranslator::Export(AtNode *shader)
{
   MStatus status;

   // loop the look entries
   MPlug index = FindMayaPlug("index");
   unsigned int active_input = (unsigned int)index.asInt();
   AiNodeSetInt(shader, "index", active_input);

   // loop the look entries
   MPlug looks = FindMayaPlug("looks");
   unsigned nvars = looks.numElements();
   MPlug varPlug;
   AtArray* varArray = AiArrayAllocate(nvars, 1, AI_TYPE_NODE);

   // should we just export the merge node from the current index?

   for (unsigned int i = 0; i < nvars; ++i)
   {
      varPlug = looks[i];
      // get look name
      MPlug name = varPlug.child(0, &status);
      MString name_str = name.asString();

      MPlug inputs = varPlug.child(1, &status);
      // get connections

      MString mergeName = name_str;

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

         AtArray *prevArray = AiNodeGetArray(mergeNode, s_inputs);
         bool mergeNeedUpdate = true;
         if (prevArray)
         {
            unsigned prevArrayElems = AiArrayGetNumElements(prevArray);
            if (prevArrayElems == nelems)
            {
               mergeNeedUpdate = false;
               for (unsigned i = 0; i < nelems; ++i)
               {
                  if (AiArrayGetPtr(array, i) != AiArrayGetPtr(prevArray, i))
                  {
                     mergeNeedUpdate = true;
                     break;
                  }
               }
            }
         }
         if (mergeNeedUpdate)
            AiNodeSetArray(mergeNode, s_inputs, array);
      }

      AiArraySetPtr(varArray, i, (void*)mergeNode);
   }

   AiNodeSetArray(shader, s_inputs, varArray);

   AtArray* enable = AiArrayAllocate(nvars, 1, AI_TYPE_BOOLEAN);
   for (size_t i = 0; i < nvars; ++i)
      AiArraySetBool(enable, i, false);

   if (nvars > 0)
      AiArraySetBool(enable, active_input, true);

   // Define and set the user parameter used by the cooker to enable/bypass inputs
   if (!AiNodeLookUpUserParameter(shader, s_inputs_enable))
      AiNodeDeclare(shader, s_inputs_enable, "constant ARRAY BOOL");

   AiNodeSetArray(shader, s_inputs_enable, enable);
}

void CLookSwitchTranslator::NodeInitializer(CAbTranslator context)
{
}
