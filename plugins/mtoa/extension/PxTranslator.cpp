#include "PxTranslator.h"

#include <ai_metadata.h>

#include "nodes/ShaderUtils.h"
#include "nodes/shaders/surface/ArnoldCustomShader.h"
#include "nodes/shaders/light/ArnoldSkyDomeLightShader.h"
#include "scene/Lights.h"
#include "scene/Geometry.h"
#include "scene/Cameras.h"

// A Maya node proxy
CPxTranslator::CPxTranslator(const MString &translatorName,
                             const MString &arnoldNodeName,
                             const MString &providerName,
                             const MString &providerFile,
                             TCreatorFunction creatorFunction,
                             TNodeInitFunction nodeInitFunction)
{
   name = translatorName;
   arnold = arnoldNodeName;
   provider = providerName;
   // if (name.numChars() == 0) name = provider;
   file = providerFile;
   creator = creatorFunction;
   initialize = nodeInitFunction;
}

CPxTranslator::CPxTranslator(const MString &translatorName,
                             const AtNodeEntry* arnoldNodeEntry,
                             const MString &providerName,
                             const MString &providerFile,
                             TCreatorFunction creatorFunction,
                             TNodeInitFunction nodeInitFunction)
{
   name = translatorName;
   arnold = AiNodeEntryGetName(arnoldNodeEntry);
   provider = providerName;
   // if (name.numChars() == 0) name = provider;
   file = providerFile;
   creator = creatorFunction;
   initialize = nodeInitFunction;
}

MStatus CPxTranslator::ReadMetaData()
{
   const AtNodeEntry* arnoldNodeEntry = NULL;
   arnoldNodeEntry = AiNodeEntryLookUp(arnold.asChar());
   if (NULL == arnoldNodeEntry)
   {
      AiMsgError("[%s] Arnold node %s does not exist", provider.asChar(), arnold.asChar());
      return MStatus::kInvalidParameter;
   }

   const char* node = arnold.asChar();
   const char* ext = provider.asChar();

   // If no name was specified, use metadata, or by default the extension name
   if (name.numChars() == 0)
   {
      const char* translatorName;
      if (AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.translator", &translatorName))
      {
         name = MString(translatorName);
      }
      else
      {
         name = provider;
      }
   }
   // If no explicit translator was specified, choose a default one using Arnold node type and metadata
   if (NULL == creator)
   {
      const char* arnoldNodeTypeName;
      arnoldNodeTypeName = AiNodeEntryGetTypeName(arnoldNodeEntry);
      if (strcmp(arnoldNodeTypeName, "camera") == 0)
      {
         // TODO : define a non virtual CameraTranslator?
         creator = CPerspCameraTranslator::creator;
         initialize = CPerspCameraTranslator::NodeInitializer;
      }
      else if (strcmp(arnoldNodeTypeName,"light") == 0)
      {
         // TODO : define a non virtual CLightTranslator?
         creator = CPointLightTranslator::creator;
         initialize = CPointLightTranslator::NodeInitializer;
      }
      else if (strcmp(arnoldNodeTypeName,"shader") == 0)
      {
         // TODO : define a non virtual CShaderTranslator
         creator = CAutoTranslator::creator;
         // initialize = CAutoTranslator::NodeInitializer;
      }
      else if (strcmp(arnoldNodeTypeName,"shape") == 0)
      {
         // TODO : define a non virtual CShapeTranslator or Geo
         creator = CMeshTranslator::creator;
         initialize = CMeshTranslator::NodeInitializer;
      }
      // No default strategy to create the rest
   }

   return MStatus::kSuccess;
}
