#include "ArnoldOptionsTranslator.h"
#include "render/RenderSession.h"
#include "render/RenderOptions.h"

#include <ai_universe.h>
#include <ai_msg.h>

#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MPlugArray.h>

void CArnoldOptionsTranslator::SetupImageOptions(AtNode* options)
{
   const CRenderOptions* renderOptions = CRenderSession::GetInstance()->RenderOptions();
   if (renderOptions->useRenderRegion())
   {
      AiNodeSetInt(options, "region_min_x", renderOptions->minX());
      AiNodeSetInt(options, "region_min_y", renderOptions->height() - renderOptions->maxY() - 1);
      AiNodeSetInt(options, "region_max_x", renderOptions->maxX());
      AiNodeSetInt(options, "region_max_y", renderOptions->height() - renderOptions->minY() - 1);
   }

   AiNodeSetInt(options, "xres", renderOptions->width());
   AiNodeSetInt(options, "yres", renderOptions->height());
   AiNodeSetFlt(options, "aspect_ratio", renderOptions->pixelAspectRatio());
}

AtNode* CArnoldOptionsTranslator::CreateArnoldNodes()
{
   return AiUniverseGetOptions();
}

void CArnoldOptionsTranslator::Export(AtNode *options)
{
   SetupImageOptions(options);
   MTime currentTime;

   currentTime = MAnimControl::currentTime();

   MStatus status;
   MPlug plug;
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(options->base_node);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      if (strcmp(paramName, "name") != 0)
      {
         AtInt paramType = AiParamGetType(paramEntry);

         // attr name name remap
         const char* attrName;
         if (!AiMetaDataGetStr(options->base_node, paramName, "maya.name", &attrName))
            attrName = paramName;

         plug = GetFnNode().findPlug(attrName, &status);
         if (status == MS::kSuccess)
         {
            // Special cases
            if (strcmp(paramName, "threads") == 0)
            {
               AiNodeSetInt(options, "threads", GetFnNode().findPlug("threads_autodetect").asBool() ? 0 : GetFnNode().findPlug("threads").asInt());
            }
            else if (strcmp(paramName, "AA_sample_clamp") == 0)
            {
               if (GetFnNode().findPlug("use_sample_clamp").asBool())
               {
                  ProcessParameter(options, "AA_sample_clamp", AI_TYPE_FLOAT);
               }
            }
            else if (strcmp(paramName, "AA_seed") == 0)
            {
               // FIXME: this is supposed to use a connection to AA_seed attribute
               if (!GetFnNode().findPlug("lock_sampling_noise").asBool())
               {
                  AiNodeSetInt(options, "AA_seed", (AtInt)currentTime.value());
               }
            }
            // Process parameter automatically
            else
            {
               ProcessParameter(options, plug, paramName, paramType);
            }
         }
         else
         {
            AiMsgDebug("Attribute %s.%s requested by translator does not exist", GetFnNode().name().asChar(), attrName);
         }
      }
   }

   MObject background;
   MPlugArray conns;
   MPlug pBG = GetFnNode().findPlug("background");
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
      AiNodeSetPtr(options, "background", ExportShader(background));
   }

   // ATMOSPHERE SHADER
   //
   MSelectionList list;
   MObject        node;

   AtInt atmosphere = GetFnNode().findPlug("atmosphere").asInt();
   switch (atmosphere)
   {
   case 0:
      break;

   case 1:  // Fog
      list.add("defaultFog");
      if (list.length() > 0)
      {
         list.getDependNode(0, node);
         AiNodeSetPtr(options, "atmosphere", ExportShader(node));
      }
      break;

   case 2:  // Volume Scattering
      list.add("defaultVolumeScattering");
      if (list.length() > 0)
      {
         list.getDependNode(0, node);
         AiNodeSetPtr(options, "atmosphere", ExportShader(node));
      }
      break;
   }

}
