#include "ImagerLightMixerTranslator.h"
#include "ImagerTranslator.h"
#include "utils/Universe.h"
#include "utils/ConstantStrings.h"
#include "translators/NodeTranslatorImpl.h"
#include "translators/DagTranslator.h"
#include <maya/MPlugArray.h>
#include <maya/MTimerMessage.h>
#include <maya/MEventMessage.h>


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
      const AtString paramNameStr(paramName);

      if (paramNameStr == str::name || paramNameStr == str::input )
      {
         continue;
      }
      else if (paramNameStr != str::layer_selection && paramNameStr != str::output_name && paramNameStr != str::enable)
      {
         AtArray *interpArray = AiNodeGetArray(shader, paramNameStr);
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
   


   // // Always export residual layer (#4474)
   bool export_residual = true;
   AtArray *name_array = AiNodeGetArray(shader, str::layer_name);
   unsigned int numElems = AiArrayGetNumElements(name_array);

   // Searching from the back because there's a higer chance for this 
   // to be the last item in the list
   for (unsigned int i = numElems ; i-- > 0;)
   {
      if (std::string(AiArrayGetStr(name_array, i)) == "<residual_lights>")
      {
         export_residual = false;
         break;
      }
   }

   // Using the numElems from one array for everything
   // There should never be a case where these arrays are not the same
   
   if (export_residual)
   {   
      AiArrayResize(name_array, numElems+1, AiArrayGetNumKeys(name_array));
      AiArraySetStr(name_array, numElems, AtString("<residual_lights>"));

      AtArray* enable_array = AiNodeGetArray(shader, str::layer_enable);
      AiArrayResize(enable_array, numElems+1, AiArrayGetNumKeys(enable_array));
      AiArraySetBool(enable_array, numElems, 1);
      
      AtArray* solo_array = AiNodeGetArray(shader, str::layer_solo);
      AiArrayResize(solo_array, numElems+1, AiArrayGetNumKeys(solo_array));
      AiArraySetBool(solo_array, numElems, 0);
      
      AtArray* intensity_array = AiNodeGetArray(shader, str::layer_intensity);
      AiArrayResize(intensity_array, numElems+1, AiArrayGetNumKeys(intensity_array));
      AiArraySetFlt(intensity_array, numElems, 1.0f);
      
      AtArray* exposure_array = AiNodeGetArray(shader, str::layer_exposure);
      AiArrayResize(exposure_array, numElems+1, AiArrayGetNumKeys(exposure_array));
      AiArraySetFlt(exposure_array, numElems, 0.0f);
      AtArray* tint_array = AiNodeGetArray(shader, str::layer_tint);
      AiArrayResize(tint_array, numElems+1, AiArrayGetNumKeys(tint_array));
      AtRGB rgb = AtRGB(1.0f,1.0f,1.0f);
      AiArraySetRGB(tint_array, numElems, rgb);
   }
}
