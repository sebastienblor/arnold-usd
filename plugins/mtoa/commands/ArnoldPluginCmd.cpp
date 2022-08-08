#include "ArnoldPluginCmd.h"
#include "extension/ExtensionsManager.h"
#include "attributes/AttrHelper.h"
#include "utils/Universe.h"
#include "utils/BuildID.h"
#include "utils/ConstantStrings.h"

#include <curl/curl.h>

#include <maya/MArgDatabase.h>
#include <maya/MTypes.h>

MSyntax CArnoldPluginCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("lsp", "listPlugins", MSyntax::kString);
   syntax.addFlag("lst", "listTranslators", MSyntax::kString);
   syntax.addFlag("le", "loadExtension", MSyntax::kString);
   syntax.addFlag("ule", "unloadExtension", MSyntax::kString);
   syntax.addFlag("gad", "getAttrData", MSyntax::kString);

   syntax.addFlag("aov", "listAOVs", MSyntax::kNoArg);
   syntax.addFlag("nt", "nodeType", MSyntax::kString);
   
   syntax.addFlag("lnt", "listAOVNodeTypes", MSyntax::kNoArg);
   syntax.addFlag("las", "listAOVShaders", MSyntax::kNoArg);
   syntax.addFlag("lcs", "listCustomShapes", MSyntax::kNoArg);
   syntax.addFlag("lop", "listOperators", MSyntax::kNoArg);
   syntax.addFlag("lim", "listImagers", MSyntax::kNoArg);
   syntax.addFlag("sdt", "setDefaultTranslator", MSyntax::kString, MSyntax::kString);
   syntax.addFlag("gdt", "getDefaultTranslator", MSyntax::kString);
   syntax.addFlag("lnm", "listNodeMetadatas", MSyntax::kString);
   syntax.addFlag("gnm", "getNodeMetadata", MSyntax::kString, MSyntax::kString);
   syntax.addFlag("lmm", "listMatchMetadata", MSyntax::kString, MSyntax::kString);

   syntax.addFlag("llx", "listLoadedExtensions", MSyntax::kNoArg);
   syntax.addFlag("gev", "getExtensionApiVersion", MSyntax::kString);
   syntax.addFlag("gbi", "getBuildID", MSyntax::kNoArg);
   syntax.addFlag("gcv", "getClmVersion", MSyntax::kNoArg);
   syntax.addFlag("gbd", "getBuildDate", MSyntax::kNoArg);
   syntax.addFlag("glv", "getLatestVersion", MSyntax::kNoArg);


   return syntax;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

MStatus CArnoldPluginCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MArgDatabase args(syntax(), argList, &status);
   if (!status)
      return status;

   if (args.isFlagSet("listPlugins"))
   {
      MStringArray pluginNames;
      MString extPath = args.flagArgumentString("listPlugins", 0);
      CExtension* extension = CExtensionsManager::GetExtension(extPath);
      if (NULL != extension)
      {
         pluginNames = extension->GetOwnLoadedArnoldPlugins();
      }
      else
      {
         MGlobal::displayError(MString("Could not find extension ")+extPath);
      }
      // Can also request only those from a specific extension
      // with GetTranslatorNames(typeName, provider)
      setResult(pluginNames);
   }
   else if (args.isFlagSet("listTranslators"))
   {
      MString typeName = args.flagArgumentString("listTranslators", 0);
      // Can also request only those from a specific extension
      // with GetTranslatorNames(typeName, provider)
      setResult(CExtensionsManager::GetTranslatorNames(typeName));
   }
   else if (args.isFlagSet("loadExtension", 0))
   {
      MString extPath = args.flagArgumentString("loadExtension", 0);
      int basenameIndex = extPath.rindexW('/');
      CExtension *extension = nullptr;
      if (basenameIndex > 0)
      {
         // Eventually split the filename into a folder basename, and the actual
         // extension file to load #MTOA-847
         MString basename = extPath.substring(0, basenameIndex - 1);
         MString filename = extPath.substring(basenameIndex + 1, extPath.length() -1);
         extension = CExtensionsManager::LoadExtension(filename, basename);
      } else
      {
         extension = CExtensionsManager::LoadExtension(extPath);   
      }
      // If an extension was loaded, we need to register it, 
      // to ensure it will be found during translation
      if (extension)
      {
         MStatus regStatus = CExtensionsManager::RegisterExtension(extension); 
         if (regStatus == MS::kSuccess)
         {
            MGlobal::displayInfo(MString("Successfully registered extension ") + extPath);
         } else
         {
            MGlobal::displayError(MString("Could not register extension ") + extPath);
         }
      } else
      {
         MGlobal::displayError(MString("Could not load extension ") + extPath);
      }
   }
   else if (args.isFlagSet("unloadExtension", 0))
   {
      MString extPath = args.flagArgumentString("unloadExtension", 0);
      CExtension* extension = CExtensionsManager::GetExtension(extPath);
      if (NULL != extension)
      {
         CExtensionsManager::UnloadExtension(extension);
      }
      else
      {
         MGlobal::displayError(MString("Could not find extension ")+extPath);
      }
   }
   else if (args.isFlagSet("getAttrData", 0))
   {
      MString nodeName = args.flagArgumentString("getAttrData", 0);
      MStringArray result;
      const AtNodeEntry* nodeEntry = AiNodeEntryLookUp(AtString(nodeName.asChar()));
      if (nodeEntry == NULL)
      {
         MString err = "Unknown arnold node \"";
         err += nodeName + "\"";
         MGlobal::displayError( err );
         return MS::kInvalidParameter;
      }
      CBaseAttrHelper helper(nodeEntry);
      AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
      while (!AiParamIteratorFinished(nodeParam))
      {
         const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
         const char* paramName = AiParamGetName(paramEntry);
         if (!helper.IsHidden(paramName))
         {
            AtString paramNameStr(paramName);
            result.append(paramName);
            result.append(helper.GetMayaAttrName(paramName));
            AtString label("");
            AiMetaDataGetStr(nodeEntry, paramNameStr, str::label, &label);
            result.append(label.c_str());
            AtString desc("");
            AiMetaDataGetStr(nodeEntry, paramNameStr, str::desc, &desc);
            result.append(desc.c_str());
         }
      }
      AiParamIteratorDestroy(nodeParam);
      setResult(result);
   }
   else if (args.isFlagSet("listAOVs"))
   {
      MStringArray result;
      if (args.isFlagSet("nodeType"))
      {
         MString nodeType;
         args.getFlagArgument("nodeType", 0, nodeType);
         CExtensionsManager::GetNodeAOVs(nodeType, result);
      }
      else
         CExtensionsManager::GetAOVs(result);
      setResult(result);
   }
   else if (args.isFlagSet("listAOVNodeTypes"))
   {
      MStringArray result;
      CExtensionsManager::GetNodeTypesWithAOVs(result);
      setResult(result);
   }
   else if (args.isFlagSet("listAOVShaders"))
   {
      MStringArray result;
      CExtensionsManager::GetAOVShaders(result);
      setResult(result);
   }
   else if (args.isFlagSet("listCustomShapes"))
   {
      MStringArray result;
      CExtensionsManager::GetCustomShapes(result);
      setResult(result);
   }
   else if (args.isFlagSet("listOperators"))
   {
      MStringArray result;
      CExtensionsManager::GetOperators(result);
      setResult(result);
   }
   else if (args.isFlagSet("listImagers"))
   {
      MStringArray result;
      CExtensionsManager::GetImagers(result);
      setResult(result);
   }
   else if(args.isFlagSet("setDefaultTranslator"))
   {
      MString mayaTypeName, translatorName;
      args.getFlagArgument("setDefaultTranslator", 0, mayaTypeName);
      args.getFlagArgument("setDefaultTranslator", 1, translatorName);
      CExtensionsManager::SetDefaultTranslator(mayaTypeName, translatorName);
   }
   else if(args.isFlagSet("getDefaultTranslator"))
   {
      MString mayaTypeName;
      args.getFlagArgument("getDefaultTranslator", 0, mayaTypeName);
      setResult(CExtensionsManager::GetDefaultTranslator(mayaTypeName));
   }
   else if(args.isFlagSet("listLoadedExtensions"))
   {
      MStringArray arr = CExtensionsManager::ListLoadedExtensions();
      setResult(arr);
   }
   else if(args.isFlagSet("getExtensionApiVersion"))
   {
      MString extensionName;
      args.getFlagArgument("getExtensionApiVersion", 0, extensionName);
      CExtension* extension = CExtensionsManager::GetExtensionByName(extensionName);
      if (extension != 0)
         setResult(extension->GetApiVersion());
   }
   else if(args.isFlagSet("getBuildID"))
   {
      setResult(MString(BUILD_ID));
   }
   else if(args.isFlagSet("getBuildDate"))
   {
      setResult(MString(__DATE__) + MString (" ")+ MString(__TIME__));
   }
   else if(args.isFlagSet("getClmVersion"))
   {
#ifdef CLIC_V1
      setResult(MString("1"));
#endif
#ifdef CLIC_V2
      setResult(MString("2"));
#endif
   } else if(args.isFlagSet("listMatchMetadata"))
   {
      MString metadataName, metadataValue;
      args.getFlagArgument("listMatchMetadata", 0, metadataName);
      args.getFlagArgument("listMatchMetadata", 1, metadataValue);
      setResult(CExtensionsManager::FindMatchingMetadatas(metadataName, metadataValue));
   } else if (args.isFlagSet("listNodeMetadatas"))
   {
      MString nodeEntry;
      args.getFlagArgument("listNodeMetadatas", 0, nodeEntry);
      const ArnoldNodeMetadataStore *metadatas = CExtensionsManager::FindNodeMetadatas(nodeEntry, false);
      MStringArray resultArray;
      for (size_t i = 0; i < metadatas->size(); ++i)
      {
         resultArray.append(metadatas->at(i).name.c_str());
      }
      setResult(resultArray);
   } else if (args.isFlagSet("getNodeMetadata"))
   {
      MString nodeEntry;
      args.getFlagArgument("getNodeMetadata", 0, nodeEntry);
      MString metadataName;
      args.getFlagArgument("getNodeMetadata", 1, metadataName);
      AtString metadataNameStr(metadataName.asChar());
      const ArnoldNodeMetadataStore *metadatas = CExtensionsManager::FindNodeMetadatas(nodeEntry, false);
      MString paramValueStr;
      for (size_t i = 0; i < metadatas->size(); ++i) 
      {
         const AtMetaDataEntry &metadata = metadatas->at(i);
         if (metadata.name != metadataNameStr)
            continue;
         // found the metadata, now get its value
         const AtParamValue &paramValue = metadata.value;
         switch(metadata.type)
         {
            case AI_TYPE_BYTE:
               paramValueStr += (int)paramValue.BYTE();
            break;
            case AI_TYPE_INT:
               paramValueStr += paramValue.INT();
            break;
            case AI_TYPE_UINT:
               paramValueStr += (int)paramValue.UINT();
            break;
            case AI_TYPE_BOOLEAN:
               paramValueStr = (paramValue.BOOL()) ? MString("true") : MString("false");
            break;
            case AI_TYPE_USHORT:
            case AI_TYPE_HALF:
            case AI_TYPE_FLOAT:
               paramValueStr += paramValue.FLT();
            break;
            case AI_TYPE_RGB:
               paramValueStr += paramValue.RGB().r;
               paramValueStr += MString(",");
               paramValueStr += paramValue.RGB().g;
               paramValueStr += MString(",");
               paramValueStr += paramValue.RGB().b;
            break;
            case AI_TYPE_RGBA:
               paramValueStr += paramValue.RGBA().r;
               paramValueStr += MString(",");
               paramValueStr += paramValue.RGBA().g;
               paramValueStr += MString(",");
               paramValueStr += paramValue.RGBA().b;
               paramValueStr += MString(",");
               paramValueStr += paramValue.RGBA().a;
            break;
            case AI_TYPE_VECTOR:
               paramValueStr += paramValue.VEC().x;
               paramValueStr += MString(",");
               paramValueStr += paramValue.VEC().y;
               paramValueStr += MString(",");
               paramValueStr += paramValue.VEC().z;
            break;
            case AI_TYPE_VECTOR2:
               paramValueStr += paramValue.VEC2().x;
               paramValueStr += MString(",");
               paramValueStr += paramValue.VEC2().y;
            break;
            case AI_TYPE_ENUM:
            case AI_TYPE_STRING:
               paramValueStr = MString(paramValue.STR().c_str());
            break;
            default:
            break;
         }

         break;
      }
      setResult(paramValueStr);
   } else if (args.isFlagSet("getLatestVersion"))      
   {
         CURL *curl;
         CURLcode res;
         std::string readBuffer;

         curl = curl_easy_init();
         if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "https://arnoldrenderer.com/getversion/maya");
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            res = curl_easy_perform(curl);
            /* always cleanup */ 
            curl_easy_cleanup(curl);

            setResult(readBuffer.c_str());   
         }
   }

   // FIXME: error on unknown flag
   return MS::kSuccess;

}
