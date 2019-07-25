#include <sstream>
#include <stdio.h>

#include "BifShapeTranslator.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MProfiler.h>
#include <maya/MSelectionContext.h>
#include <maya/MPlug.h>
#include <maya/MDataHandle.h>
#include <maya/MFnPluginData.h>
#include <maya/MFileObject.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnStringArrayData.h>
#include <maya/MSelectionList.h>

#include "extension/Extension.h"
#include "extension/ExtensionsManager.h"

static bool s_loadedProcedural = false;

#ifdef _WIN32
static MString s_bifrostProcedural = "arnold_bifrost";
#else
static MString s_bifrostProcedural = "libarnold_bifrost";
#endif

static MString s_bifrostProceduralPath = "";

// WARNING: The same function is duplicated in the "old" bifrost extension.
// If you change this one, don't forget to change the other
static MString GetArnoldBifrostPath()
{
   MString str;
   char archStr[64];
   char majorStr[64];
   char minorStr[64];
   char fixStr[64];
   // Get the current Arnold version
   MString version = AiGetVersion(archStr, majorStr, minorStr, fixStr);
   // convert each token to integer for comparison
   int arch = MString(archStr).asInt();
   int major = MString(majorStr).asInt();
   int minor = MString(minorStr).asInt();
   int fix = MString(fixStr).asInt();
   // build a dummy unique version value
   int versionVal = fix + 100 * minor + 10000 * major + 1000000 * arch;

   // Get the path to the bifrost module
   MString cmd("import maya.cmds as cmds; cmds.getModulePath(moduleName='Bifrost')");
   MString bifrostPath = MGlobal::executePythonCommandStringResult(cmd);
   
   if (bifrostPath.length() == 0)
      return MString(); // didn't find it, Bifrost is probably not loaded

   DIR *dir;
   struct dirent *ent;
   int topVersionVal = -1;
   MString topVersionPath = "";

   // Loop over all directories in the bifrost module path, 
   // and look for the arnold-xxxxx ones
   if ((dir = opendir (bifrostPath.asChar())) != NULL) 
   {
      while ((ent = readdir (dir)) != NULL) 
      {
         MString dirName(ent->d_name);
         // Skip the ones that don't start with "arnold-"
         if (dirName.length() < 7 || dirName.substringW(0, 6) != MString("arnold-"))
            continue;

         // Get the version number this bifrost procedural was built against
         MString folderVersion = dirName.substringW(7, dirName.length() -1);
         
         // We want to get the most recent bifrost procedural that is compatible with 
         // the current arnold version
         MStringArray splitVersion;
         folderVersion.split('.', splitVersion);
         if (splitVersion.length() < 2 || !splitVersion[0].isInt() || !splitVersion[1].isInt())
            continue; // something wrong with the folder format, we need at least the 2 first integers

         int currentArch = splitVersion[0].asInt();
         // we need a procedural that matches the current architectural version of arnold
         if (currentArch < arch)
            continue; 

         int currentMajor = splitVersion[1].asInt();
         int currentVersionVal = 10000 * currentMajor + 1000000 * currentArch;

         // The following tests are in case we end up stripping the
         // minor and fix versions from the folders
         if (splitVersion.length() >= 3 && splitVersion[2].isInt())
            currentVersionVal += 100 * splitVersion[2].asInt();

         if (splitVersion.length() >= 4 && splitVersion[3].isInt())
            currentVersionVal += splitVersion[3].asInt();
         
         if (currentVersionVal > versionVal)
            continue; // the arnold version is older than the older package, ignore it

         if (currentVersionVal < topVersionVal)
            continue; // nah, we already have a better candidate;

         // this is the best candidate so far
         topVersionVal = currentVersionVal;
         topVersionPath = dirName;         
      }
      closedir (dir);
   }  
   str = bifrostPath + MString("/") + topVersionPath;
   return str;
}

static bool LoadBifrostProcedural()
{
   if (s_loadedProcedural)
      return true;

   if (AiNodeEntryLookUp("bifrost_graph") != NULL)
   {
      s_loadedProcedural = true;
      return true;
   }

   MString s_bifrostProceduralPath;
   s_bifrostProceduralPath = GetArnoldBifrostPath();

   AiMsgInfo("[bifrost] %s", s_bifrostProceduralPath.asChar());

   if (s_bifrostProceduralPath.length() > 0)
   {
      CExtension *extension = CExtensionsManager::GetExtensionByName("bifShapeTranslator");
      if (extension)
      {
         MFileObject fo;
         fo.setRawFullName(s_bifrostProceduralPath);
         if (fo.exists())
         {
            extension->LoadArnoldPlugin(s_bifrostProcedural, s_bifrostProceduralPath);
            s_loadedProcedural = true;
            return true;
         }
      }
   }
   return false;
}


void CBifShapeTranslator::NodeInitializer(CAbTranslator context)
{

   if (!LoadBifrostProcedural())
      AiMsgWarning("Bifrost procedural could not be found: %s", s_bifrostProceduralPath.asChar());

   CExtensionAttrHelper helper(context.maya, "procedural");
//   Arnold-76 : no longer create the common shape attributes
//   CShapeTranslator::MakeCommonAttributes(helper);
   MakeArnoldVisibilityFlags(helper);

   CAttrData data;

   // bifrost_board procedural parameters
   data.defaultValue.STR() = AtString("");
   data.name = "aiFilename";
   data.shortName = "aiFilename";
   helper.MakeInputString ( data );

   data.defaultValue.STR() = AtString("");
   data.name = "aiCompound";
   data.shortName = "aiCompound";
   helper.MakeInputString ( data );

   // motion blur controls
   data.defaultValue.FLT() = 1.0f;
   data.name = "aiVelocityScale";
   data.shortName = "aiVelocityScale";
   helper.MakeInputFloat ( data );

   MStringArray  enumNames;
   enumNames.append ( "Auto" );
   enumNames.append ( "Velocity Only" );
   data.defaultValue.INT() = 0;
   data.name = "aiMotionBlurMode";
   data.shortName = "aiMotionBlurMode";
   data.enums= enumNames;
   helper.MakeInputEnum ( data );

   // procedural namespace parameter
   helper.MakeInput("namespace");

   //// operators attribute for adding operators to custom procedurals
   data.name = "operators";
   data.shortName = "operators";
   data.type = AI_TYPE_NODE;
   data.isArray = true;
   helper.MakeInput(data);

}

AtNode* CBifShapeTranslator::CreateArnoldNodes()
{
   if (!LoadBifrostProcedural())
      AiMsgError("Bifrost procedural could not be found: %s", s_bifrostProceduralPath.asChar());

   return AddArnoldNode("bifrost_graph");
}


void CBifShapeTranslator::GetSerializedData(MDoubleArray& array)
{
   MPlug serialisedDataPlug = FindMayaPlug("outputSerializedData");
   if (!serialisedDataPlug.isNull() )
   {
      AiMsgInfo("[mtoa.bifrost_graph] : Exporting plug outputSerializedData");
      MDataHandle handle;
      serialisedDataPlug.getValue(handle);
      MFnDoubleArrayData ArrData(handle.data());
      array = ArrData.array();
   }
}

void CBifShapeTranslator::Export( AtNode *shape )
{
   unsigned int step = GetMotionStep();
   // export BifShape parameters
   MPlug filenamePlug = FindMayaPlug("aiFilename");
   if (!filenamePlug.isNull() && !filenamePlug.isDefaultValue())
   {
      MString filename = filenamePlug.asString();
      AiNodeSetStr(shape, "filename", filename.asChar());
   }

   // export the Bifrost graph data if not file is supplied
   MPlug serialisedDataPlug = FindMayaPlug("outputSerializedData");
   if (!serialisedDataPlug.isNull() && filenamePlug.isDefaultValue())
   {
      MDoubleArray serialisedData;
      GetSerializedData(serialisedData);
      unsigned int nEle = serialisedData.length();
      unsigned int nBytes = nEle * sizeof(double);
      AtArray *inputsArray = AiArray(1, 1, AI_TYPE_STRING, "input0");

#ifdef DEBUG_DUMP_TO_FILE
      // Write all inputs to file and have the proc read them back in
#ifdef _WIN32
      std::string filename = "C:\\temp\\bifrost-";
#else
      std::string filename = "/tmp/bifrost-";
#endif
      filename += AiNodeGetName(shape);
      filename += ".bob";

      // write the data to file
      const char *fname = filename.c_str();
      FILE * fp = fopen(fname,"wb");
      AtArray *interpsArray = AiArray(1, 1, AI_TYPE_STRING, "file");
      AiNodeDeclare(shape, "bifrost:input0", "constant STRING");
      AiNodeSetStr(shape, "bifrost:input0", filename.c_str());

      fwrite( &serialisedData[0], sizeof(uint8_t), nEle * sizeof(double), fp );

      fclose(fp);
      AiNodeSetArray(shape, "input_names", inputsArray);
      AiNodeSetArray(shape, "input_interpretations", interpsArray);
#else

      // Send serialized data directly to arnold_bifrost
      AtArray *interpsArray = AiArray(1, 1, AI_TYPE_STRING, "serialized");
      AtArray *dataArray = 0;

      if (!IsMotionBlurEnabled(MTOA_MBLUR_DEFORM))
      {
         dataArray = AiArrayConvert(nBytes, 1, AI_TYPE_BYTE, &serialisedData[0]);
      }
      else
      {
         dataArray = AiArrayAllocate(nBytes, GetNumMotionSteps(), AI_TYPE_BYTE);
         uint8_t* data = reinterpret_cast<uint8_t*>(&serialisedData[0]);
         uint8_t *dataList = static_cast<uint8_t*>(AiArrayMapKey(dataArray, step));
         memcpy(dataList, data, nBytes);
         AiArrayUnmap(dataArray);
      }
      if (AiArrayGetNumElements(dataArray) > 0)
      {
         AiNodeDeclare(shape, "bifrost:input0", "constant ARRAY BYTE");
         AiNodeSetArray(shape, "bifrost:input0", dataArray);
         AiNodeSetArray(shape, "input_names", inputsArray);
         AiNodeSetArray(shape, "input_interpretations", interpsArray);
      }
#endif
   }

   MPlug geomPlug = FindMayaPlug("aiCompound");
   if (!geomPlug.isNull())
   {
      MString geomPath = geomPlug.asString();
      if (geomPath.length() > 0)
         AiNodeSetStr(shape, "compound", geomPath.asChar());
   }

   // Export all material references done inside the bifrost graph
   MPlug materialReferencesPlug = FindMayaPlug("outputMaterialReferences");
   if (!materialReferencesPlug.isNull())
   {
      MDataHandle matDataHandle;
      materialReferencesPlug.getValue(matDataHandle);
      MFnStringArrayData data1(matDataHandle.data());
      MStringArray shaders = data1.array();
      if (shaders.length() > 0)
      {
         AtArray* materialReferences = AiArrayAllocate( shaders.length(), 1, AI_TYPE_STRING );
         AtArray* shaderReferences = AiArrayAllocate( shaders.length(), 1, AI_TYPE_NODE );
         for (uint32_t i = 0; i < shaders.length(); i++)
         {
            MSelectionList selection;
            selection.add(shaders[i]);
            MDagPath shPath;
            MObject shNode;
            selection.getDagPath(0, shPath);
            selection.getDependNode(0, shNode);
            MPlug dummyPlug = MFnDependencyNode(shNode).findPlug("message", true);
            if (dummyPlug.isNull())
               continue;

            AtNode* shaderNode = ExportConnectedNode(dummyPlug); // do export the shader
            AiArraySetStr(materialReferences, i, shaders[i].asChar());
            AiArraySetPtr(shaderReferences, i, shaderNode);
         }
         AiNodeSetArray(shape, "material_references", materialReferences);
         AiNodeSetArray(shape, "shader_references", shaderReferences);
      }
   }

   MPlug velocityScalePlug = FindMayaPlug("aiVelocityScale");
   if (!velocityScalePlug.isNull())
   {
      float velocityScale = velocityScalePlug.asFloat();
      AiNodeSetFlt(shape, "velocity_scale", velocityScale);
   }

   if (RequiresShaderExport())
      ExportShaders(shape);

   ExportProcedural(shape);
   // ARNOLD-76 : we don't want to set the common shape parameters (self_shadows / opaque / etc...)
   // otherwise it prevents bifrost from setting each children its own parameters.
   // By removing CShapeTranslator::MakeCommonAttributes(helper) in NodeInitialize we're preventing most of them
   // to be exported since they don't exist in Maya. But receiveShadows is a maya native attribute, so we can't
   // prevent it from being exported. The best way to deal with this now is simply to reset the attribute here
   AiNodeResetParameter(shape, "receive_shadows");
}


static bool namespaceEndsWith(const std::string& str, const std::string& compareStr)
{
    // Skip any components after the name
    std::string nameStr = str.substr(0, str.rfind('.'));
    if (nameStr.length() < compareStr.length())
        return false;
    if (nameStr.length() > compareStr.length() && nameStr[nameStr.length() - compareStr.length() - 1] != '|')
        return false; // We only want to match the last part of the namespace
    return nameStr.substr(nameStr.length() - compareStr.length()) == compareStr;
}


void CBifShapeTranslator::ExportShaders(AtNode *shape)
{
   if (!LoadBifrostProcedural())
      return;

   AtNode *node = GetArnoldNode();
   if (node == NULL)
      return;

   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroupPlug.isNull())
   {
      MFnDependencyNode shadingEngineNode(shadingGroupPlug.node());
      MPlug surfaceShaderPlug = shadingEngineNode.findPlug("surfaceShader", true);
      MPlugArray connections;
      surfaceShaderPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         // Skip any default shader assigned, we don't want this overriding the proc child node shaders
         bool found_shader = namespaceEndsWith(connections[0].name().asChar(), "lambert1");
         bool found_engine = namespaceEndsWith(shadingGroupPlug.name().asChar(), "initialShadingGroup");
         if ( found_shader && found_engine )
         {
            return;
         }
      }

      AtNode *shader = ExportConnectedNode(shadingGroupPlug);
      if (shader != NULL)
      {
         AiNodeSetPtr(node, "shader", shader);
      }
      else
      {
         AiNodeSetPtr(node, "shader", NULL);
      }
   }
}
bool CBifShapeTranslator::RequiresMotionData()
{
   if (FindMayaPlug("aiMotionBlurMode").asInt() == 1)
      return false;

   return CProceduralTranslator::RequiresMotionData();
}

void CBifShapeTranslator::ExportMotion(AtNode *shape)
{
   if (LoadBifrostProcedural() == false || shape == NULL)
      return;
      // Check if motionblur is enabled and early out if it's not.

   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(shape);

   if (!IsMotionBlurEnabled(MTOA_MBLUR_DEFORM)) return;

   int step = GetMotionStep();
   MDoubleArray serialisedData;
   GetSerializedData(serialisedData);

   AtArray* dataArray = AiNodeGetArray(shape, "bifrost:input0");
   size_t serialisedSize = serialisedData.length() * sizeof(double);
   uint8_t nKeys = AiArrayGetNumKeys(dataArray);
   if (nKeys <= step)
   {
      AiMsgWarning("[mtoa.bifrost_graph] attempted motion key export when key is not available");
      return;
   }
   size_t keySize = AiArrayGetKeySize(dataArray);
   if (serialisedSize > keySize)
   {
      // Allocate a new array and copy contents over (as AiArrayResize() should
      // not be used when we care about the existing contents)
      AtArray* newDataArray = AiArrayAllocate(serialisedSize, GetNumMotionSteps(), AI_TYPE_BYTE);
      uint8_t* existingData = static_cast<uint8_t*>(AiArrayMap(dataArray));
      uint8_t* newData = static_cast<uint8_t*>(AiArrayMap(newDataArray));
      for (int key = 0; key < step; ++key)
      {
         memcpy(&newData[key * serialisedSize], &existingData[key * keySize], keySize);
         // Zero any remaining bytes to be nice to the Bifrost deserialisation parser
         memset(&newData[key * serialisedSize + keySize], 0, serialisedSize - keySize);
      }
      AiArrayUnmap(dataArray);
      AiArrayUnmap(newDataArray);
      AiNodeSetArray(shape, "bifrost:input0", newDataArray);
      // Get the new array and key size for this motion sample's copy below
      dataArray = newDataArray;
      keySize = serialisedSize;
   }
   uint8_t* data = reinterpret_cast<uint8_t*>(&serialisedData[0]);
   uint8_t *dataList = static_cast<uint8_t*>(AiArrayMapKey(dataArray, step));
   memcpy(dataList, data, serialisedSize);
   // Zero any remaining bytes to be nice to the Bifrost deserialisation parser
   if (serialisedSize < keySize)
      memset(dataList + serialisedSize, 0, keySize - serialisedSize);
   AiArrayUnmap(dataArray);
}
