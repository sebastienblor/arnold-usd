#include "DriverTranslator.h"
#include "utils/Universe.h"

#include <assert.h>

AtNode* CDriverTranslator::CreateArnoldNodes()
{
   assert(AiUniverseIsActive());

   // for now don't assume this attribute exists, since this is also the filter translator
   MStatus status;
   MPlug plug = FindMayaPlug("outputMode", &status);
   if (status == MS::kSuccess)
   {
      int mode = plug.asInt();
      // ("GUI Only", 0);
      // ("Batch Only", 1);
      // ("GUI and Batch", 2);
      if ((mode == 0 && m_session->IsBatch()) || (mode == 1 && !m_session->IsBatch()))
         return NULL;
   }

   const char* driverType = GetArnoldNodeType().asChar();
   const AtNodeEntry* entry = AiNodeEntryLookUp(driverType);
   if (entry != NULL)
   {
      bool displayDriver = false;
      // don't export display drivers during batch
      if (AiMetaDataGetBool(entry, NULL, "display_driver", &displayDriver) && displayDriver)
      {
         if (GetSessionMode() == MTOA_SESSION_BATCH)
            return NULL;
      }
      // don't export non-display drivers during IPR
      else if (GetSessionMode() == MTOA_SESSION_IPR)
         return NULL;
   }

   AtNode* created = AddArnoldNode(driverType, driverType);
   return created;
}

void CDriverTranslator::Export(AtNode *shader)
{
   MStatus status;
   const AtNodeEntry* entry = AiNodeGetNodeEntry(shader);
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(entry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      if (strcmp(paramName, "name") != 0) ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
   }
   AiParamIteratorDestroy(nodeParam);

   bool displayDriver = false;
   if (AiMetaDataGetBool(entry, NULL, "display_driver", &displayDriver) && displayDriver)
   {
      MFnDependencyNode fnOpts(GetArnoldRenderOptions());
      if (AiNodeEntryLookUpParameter(entry, "gamma") != NULL)
         AiNodeSetFlt(shader, "gamma", fnOpts.findPlug("display_gamma").asFloat());
   }
}

void CDriverTranslator::NodeInitializer(CAbTranslator context)
{
   MString maya = context.maya;
   MString arnold = context.arnold;
   MString provider = context.provider;
   const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(arnold.asChar());

   CExtensionAttrHelper helper(maya, nodeEntry);
   // inputs
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      if (!helper.IsHidden(paramName))
      {
         helper.MakeInput(paramName);
      }
   }
   AiParamIteratorDestroy(nodeParam);

}

// No callbacks currently
void CDriverTranslator::AddUpdateCallbacks()
{
}

void CDriverTranslator::RemoveUpdateCallbacks()
{
}

