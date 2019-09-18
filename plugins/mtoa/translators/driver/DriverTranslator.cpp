#include "DriverTranslator.h"
#include "utils/Universe.h"
#include "session/ArnoldSession.h"
#include <assert.h>
#include "translators/NodeTranslatorImpl.h"
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
      if ((mode == 0 && m_impl->m_session->IsBatch()) || (mode == 1 && !m_impl->m_session->IsBatch()))
         return NULL;
   }

   const char* driverType = m_impl->m_abstract.arnold.asChar();
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

   AtNode* created = AddArnoldNode(driverType/*, driverType*/);

   // we used to set this as the driver's name (using tags)
   // so until we're sure there wasn't a good reason for it I'm keeping this behaviour
   std::string name = AiNodeGetName(created);
   name += "@";
   name += driverType;
   AiNodeSetStr(created, "name", name.c_str());

   m_baseName = name;
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
      if (AiNodeEntryLookUpParameter(entry, "progressive") != NULL)
         AiNodeSetBool(shader, "progressive", m_impl->m_session->IsProgressive());
   } else
   {
      // not for display drivers, at least not for now


      int colorSpaceVal = FindMayaPlug("colorManagement").asInt();

      int cmEnabled = 0;
      MGlobal::executeCommand("colorManagementPrefs -q -cmEnabled", cmEnabled);

      if(cmEnabled)
      {
         if (colorSpaceVal == 0)
         {
            AiNodeSetStr(shader, "color_space", "");
         } else if (colorSpaceVal == 1)
         {
            MString viewTransform;
            MGlobal::executeCommand("colorManagementPrefs -q -viewTransformName", viewTransform);
            AiNodeSetStr(shader, "color_space", viewTransform.asChar());      
         } else 
         {
            int cmOutputEnabled = 0;
            MGlobal::executeCommand("colorManagementPrefs -q -outputTransformEnabled", cmOutputEnabled);

            if (cmOutputEnabled)
            {
               MString colorSpace;
               MGlobal::executeCommand("colorManagementPrefs -q -outputTransformName", colorSpace);
               AiNodeSetStr(shader, "color_space", colorSpace.asChar());  
            } else
            {
               AiNodeSetStr(shader, "color_space", "");
            }
         }
      }
      else
      {
         AiNodeSetStr(shader, "color_space", "");
      }

   }
    
}

void CDriverTranslator::NodeInitializer(CAbTranslator context)
{
   MString maya = context.maya;
   MString arnold = context.arnold;
   MString provider = context.provider;
   const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(arnold.asChar());

   CExtensionAttrHelper helper(maya, nodeEntry, "");
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

// No callbacks currently, can this maya node be deleted ?
void CDriverTranslator::AddUpdateCallbacks()
{
}

AtNode *CDriverTranslator::GetChildDriver(const std::string &token)
{
   AtNode *parentDriver = GetArnoldNode();
   AtNode *childDriver = GetArnoldNode(token.c_str());
   if(childDriver == NULL)
   {
      const char* driverType = m_impl->m_abstract.arnold.asChar();
      childDriver = AddArnoldNode(driverType, token.c_str());
   }

   const AtNodeEntry *nodeEntry = AiNodeGetNodeEntry(childDriver);

   static AtString s_nameStr("name");
   static AtString s_filenameStr("filename");
   // Now need to copy all parameters
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      AtString paramName(AiParamGetName(paramEntry));
      if (paramName == s_filenameStr || paramName == s_nameStr)
         continue;

      switch(AiParamGetType(paramEntry))
      {
         case AI_TYPE_BYTE:
            AiNodeSetByte(childDriver, paramName, AiNodeGetByte(parentDriver, paramName));
            break;
         case AI_TYPE_ENUM:
         case AI_TYPE_INT:
            AiNodeSetInt(childDriver, paramName, AiNodeGetInt(parentDriver, paramName));
            break;
         case AI_TYPE_UINT:
            AiNodeSetUInt(childDriver, paramName, AiNodeGetUInt(parentDriver, paramName));
            break;
         case AI_TYPE_BOOLEAN:
            AiNodeSetBool(childDriver, paramName, AiNodeGetBool(parentDriver, paramName));
            break;
         case AI_TYPE_FLOAT:
            AiNodeSetFlt(childDriver, paramName, AiNodeGetFlt(parentDriver, paramName));
            break;
         {
         case AI_TYPE_RGB:
            AtRGB col = AiNodeGetRGB(parentDriver, paramName);
            AiNodeSetRGB(childDriver, paramName, col.r, col.g, col.b);
            break;
         }
         {
         case AI_TYPE_RGBA:
            AtRGBA col = AiNodeGetRGBA(parentDriver, paramName);
            AiNodeSetRGBA(childDriver, paramName, col.r, col.g, col.b, col.a);
            break;
         }
         {
         case AI_TYPE_VECTOR:
            AtVector vec = AiNodeGetVec(parentDriver, paramName);
            AiNodeSetVec(childDriver, paramName, vec.x, vec.y, vec.z);
            break;
         }
         {
         case AI_TYPE_VECTOR2:
            AtVector2 vec = AiNodeGetVec2(parentDriver, paramName);
            AiNodeSetVec2(childDriver, paramName, vec.x, vec.y);
            break;
         }
         case AI_TYPE_STRING:
            AiNodeSetStr(childDriver, paramName, AiNodeGetStr(parentDriver, paramName));
            break;
         case AI_TYPE_MATRIX:
            AiNodeSetMatrix(childDriver, paramName, AiNodeGetMatrix(parentDriver, paramName));
            break;
         case AI_TYPE_POINTER:
         case AI_TYPE_NODE:
            AiNodeSetPtr(childDriver, paramName, AiNodeGetPtr(parentDriver, paramName));
            break;         
         case AI_TYPE_ARRAY:
            AiNodeSetArray(childDriver, paramName, AiArrayCopy(AiNodeGetArray(parentDriver, paramName)));
            break;
         default:
            break;



      }     
   }
   AiParamIteratorDestroy(nodeParam);
   return childDriver;
}