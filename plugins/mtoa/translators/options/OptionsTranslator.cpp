#include "OptionsTranslator.h"
#include "render/RenderSession.h"
#include "render/RenderOptions.h"

#include <ai_universe.h>
#include <ai_msg.h>

#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MPlugArray.h>

AtNode* COptionsTranslator::CreateArnoldNodes()
{
   return AiUniverseGetOptions();
}

void COptionsTranslator::Export(AtNode *options)
{
   MStatus status;
   MPlug plug;
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(options->base_node);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      if (strcmp(paramName, "name") != 0)
      {
         // Special cases
         if (strcmp(paramName, "threads") == 0)
         {
            AiNodeSetInt(options, "threads", FindMayaObjectPlug("threads_autodetect").asBool() ? 0 : FindMayaObjectPlug("threads").asInt());
         }
         else if (strcmp(paramName, "AA_sample_clamp") == 0)
         {
            if (FindMayaObjectPlug("use_sample_clamp").asBool())
            {
               ProcessParameter(options, "AA_sample_clamp", AI_TYPE_FLOAT);
            }
         }
         else if (strcmp(paramName, "AA_seed") == 0)
         {
            // FIXME: this is supposed to use a connection to AA_seed attribute
            if (!FindMayaObjectPlug("lock_sampling_noise").asBool())
            {
               AiNodeSetInt(options, "AA_seed", (AtInt)GetExportFrame());
            }
         }
         else
         {
            // Process parameter automatically
            ProcessParameter(options, paramName, AiParamGetType(paramEntry));
         }
      }
   }

   MObject background;
   MPlugArray conns;
   MPlug pBG = FindMayaObjectPlug("background");
   pBG.connectedTo(conns, true, false);
   if (conns.length() == 1)
   {
      background = conns[0].node();
   }
   else
   {
      background = MObject::kNullObj;
   }

   // BACKGROUND SHADER
   //
   if (!background.isNull())
   {
      AiNodeSetPtr(options, "background", ExportNode(background));
   }

   // ATMOSPHERE SHADER
   //
   MSelectionList list;
   MObject        node;

   AtInt atmosphere = FindMayaObjectPlug("atmosphere").asInt();
   switch (atmosphere)
   {
   case 0:
      break;

   case 1:  // Fog
      list.add("defaultFog");
      if (list.length() > 0)
      {
         list.getDependNode(0, node);
         AiNodeSetPtr(options, "atmosphere", ExportNode(node));
      }
      break;

   case 2:  // Volume Scattering
      list.add("defaultVolumeScattering");
      if (list.length() > 0)
      {
         list.getDependNode(0, node);
         AiNodeSetPtr(options, "atmosphere", ExportNode(node));
      }
      break;
   }

}
