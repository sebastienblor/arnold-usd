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
                             const MString &providerName,
                             const MString &providerFile,
                             const MString &arnoldNodeName,
                             TCreatorFunction creatorFunction,
                             TNodeInitFunction nodeInitFunction)
{
   name = translatorName;
   provider = providerName;
   if (name.numChars() == 0) name = provider;
   file = providerFile;
   arnold = arnoldNodeName;
   creator = creatorFunction;
   initialize = nodeInitFunction;
}
CPxTranslator::CPxTranslator(const MString &translatorName,
                             const MString &providerName,
                             const MString &providerFile,
                             const AtNodeEntry* arnoldNodeEntry,
                             TCreatorFunction creatorFunction,
                             TNodeInitFunction nodeInitFunction)
{
   name = translatorName;
   provider = providerName;
   if (name.numChars() == 0) name = provider;
   file = providerFile;
   arnold = AiNodeEntryGetName(arnoldNodeEntry);
   creator = creatorFunction;
   initialize = nodeInitFunction;
}

void CPxTranslator::Set(const CPxTranslator& other)
{
   name = other.name;
   provider = other.provider;
   file = other.file;
   arnold = other.arnold;
   creator = other.creator;
   initialize = other.initialize;
}

void CPxTranslator::ReadMetaData(const AtNodeEntry* arnoldNodeEntry)
{
   if (NULL == arnoldNodeEntry) return;
   const char* node = AiNodeEntryGetName(arnoldNodeEntry);
   const char* ext = provider.asChar();
   // If no explicit translator was specified, choose a default one using Arnold node type and metadata
   if (NULL == creator)
   {
      const char* arnoldNodeTypeName;
      if (!AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.translator", &arnoldNodeTypeName))
         arnoldNodeTypeName = AiNodeEntryGetTypeName(arnoldNodeEntry);
      if (strcmp(arnoldNodeTypeName, "camera") == 0)
      {
         // TODO : define a non virtual CameraTranslator
         creator = CPerspCameraTranslator::creator;
         initialize = CPerspCameraTranslator::NodeInitializer;
      }
      else if (strcmp(arnoldNodeTypeName,"light") == 0)
      {
         // TODO : define a non virtual CLightTranslator
         creator = CPointLightTranslator::creator;
         initialize = CPointLightTranslator::NodeInitializer;
      }
      else if (strcmp(arnoldNodeTypeName,"shader") == 0)
      {
         // TODO : define a non virtual CNodeTranslator
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

}
