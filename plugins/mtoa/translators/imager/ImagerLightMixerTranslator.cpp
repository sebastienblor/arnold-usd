#include "ImagerLightMixerTranslator.h"
#include "ImagerTranslator.h"
#include "utils/Universe.h"
#include "translators/NodeTranslatorImpl.h"
#include "translators/DagTranslator.h"
#include <maya/MPlugArray.h>
#include <maya/MTimerMessage.h>
#include <maya/MEventMessage.h>
#include "render/RenderSession.h"
#include "scene/MayaScene.h"

// #include <ai_array.h>
// #include <ai_universe.h>
// #include <ai_params.h>

AtNode* CImagerLightMixer::CreateArnoldNodes()
{
   return AddArnoldNode(m_impl->m_abstract.arnold.asChar());
}

void CImagerLightMixer::Export(AtNode *shader)
{
   MStatus status;
   MPlug plug;
   MString mayaAttrName;
   const AtNodeEntry* entry = AiNodeGetNodeEntry(shader);
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(entry);

   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      std::string paramNameStr(paramName);

      if (paramNameStr == "name" || paramNameStr == "input" )
      {
         continue;
      }
      else if (paramNameStr != "layer_selection" && paramNameStr != "output_name" && paramNameStr != "enable")
      {
         AtArray *interpArray = AiNodeGetArray(shader, paramName);
         unsigned int numElems = AiArrayGetNumElements(interpArray);
         if (numElems == 0)
         {
            ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
            continue;
         }

         CBaseAttrHelper helper(entry);
         MFnDependencyNode fnNode(GetMayaObject());
         if (helper.IsHidden(paramName))
         {
            if (MtoaTranslationInfo())
               MtoaDebugLog("[mtoa.translator]  "+GetTranslatorName()+": Parameter "+
                           paramName+" is hidden on Arnold node "+MString(AiNodeGetName(shader))+
                           "("+MString(AiNodeEntryGetName(entry))+").");
         }

         // check paramName
         mayaAttrName = helper.GetMayaAttrName(paramName);
         plug = FindMayaPlug(mayaAttrName, &status);
         if ((MStatus::kSuccess != status) || plug.isNull())
         {
            // check aiParamName
            helper.SetPrefix("ai_");
            MString mayaAttrAiName = helper.GetMayaAttrName(paramName);
            plug = FindMayaPlug(mayaAttrAiName, &status);
         }
         if ( numElems == plug.numElements() )
         {

            for (unsigned int i = 0; i < numElems; i++)
            {
               if ( AiArrayGetType(interpArray) == AI_TYPE_BOOLEAN )
               {
                  AiArraySetBool(interpArray, i, plug[i].asBool());
               }
               else if (AiArrayGetType(interpArray) == AI_TYPE_FLOAT)
               {
                  AiArraySetFlt(interpArray, i, plug[i].asFloat());
               }
               else if (AiArrayGetType(interpArray) == AI_TYPE_STRING)
               {
                  AiArraySetStr(interpArray, i, plug[i].asString().asChar());
               }
               else if (AiArrayGetType(interpArray) == AI_TYPE_RGB)
               {
                  AtRGB rgb = AtRGB(plug[i].child(0).asFloat(), plug[i].child(1).asFloat(), plug[i].child(2).asFloat());
                  AiArraySetRGB(interpArray, i, rgb);
               }
            }
         }
         else
         {
            ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
         }
      }
      else
      {
         ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
      }
   }
   AiParamIteratorDestroy(nodeParam);

}
void CImagerLightMixer::NodeInitializer(CAbTranslator context)
{   
}

