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

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif


static bool s_loadedProcedural = false;
static int  s_arnoldBifrostABIRev = 0;

#ifdef _WIN32
static MString s_bifrostProcedural = "arnold_bifrost";
#else
static MString s_bifrostProcedural = "libarnold_bifrost";
#endif

static MString s_bifrostProceduralPath = "";

// 2 GB, e.g. 2^31 elements of single bytes, matching limits of Arnold 5.4.0.0 and earlier
// (newer versions have a fix allowing the full 2^32 elements, or 4 GB per array key)
static const size_t ARNOLD_MAX_BYTE_ARRAY_SIZE = size_t(2) * 1024UL * 1024UL * 1024UL;
// 32 GB, e.g. 2^31 elements of 16-byte RGBAs, same size as 2^32 elements of
// 8-byte doubles which is what Bifrost gives on its output plug for BOB data
static const size_t ARNOLD_MAX_PARAMETER_BYTES = size_t(2) * 1024UL * 1024UL * 1024UL * sizeof(AtRGBA);

static const size_t MAX_PATH_LENGTH = 4096;

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

static void GetArnoldBifrostABIRevision(const char* dsoPath, const char* dsoName)
{
    typedef int (*ArnoldBifrostABIRevision)();
#ifdef _WIN32
    HMODULE handle = GetModuleHandleA(dsoName);
    if (handle != NULL)
    {
       ArnoldBifrostABIRevision revFunc = (ArnoldBifrostABIRevision)GetProcAddress(handle, "ArnoldBifrostABIRevision");
       if (revFunc != NULL)
          s_arnoldBifrostABIRev = revFunc();
    }
#else
    std::string fullPath = std::string(dsoPath) + "/" + std::string(dsoName) + ".so";
    void *handle = dlopen(fullPath.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (handle != NULL)
    {
       ArnoldBifrostABIRevision revFunc = (ArnoldBifrostABIRevision)dlsym(handle, "ArnoldBifrostABIRevision");
       if (revFunc != NULL)
          s_arnoldBifrostABIRev = revFunc();
    }
#endif
}

static bool LoadBifrostProcedural()
{
   if (s_loadedProcedural)
      return true;

   MString s_bifrostProceduralPath;
   s_bifrostProceduralPath = GetArnoldBifrostPath();

   if (AiNodeEntryLookUp("bifrost_graph") != NULL)
   {
      GetArnoldBifrostABIRevision(s_bifrostProceduralPath.asChar(), s_bifrostProcedural.asChar());
      s_loadedProcedural = true;
      return true;
   }

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
            GetArnoldBifrostABIRevision(s_bifrostProceduralPath.asChar(), s_bifrostProcedural.asChar());
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
      size_t nBytes = static_cast<size_t>(nEle) * sizeof(double);

      if (s_arnoldBifrostABIRev < 1 && nBytes >= ARNOLD_MAX_BYTE_ARRAY_SIZE)
      {
         // Early versions of arnold_bifrost only take an array of BYTEs, and
         // we can only have 2GB in size, so we have to bail as we're over the
         // limit and can't write the data to a temp file (or it won't get
         // cleaned up and will spam the filesystem).  Better to issue an error
         // and let the user break up their Bifrost outputs if possible.
         AiMsgError("[mtoa.bifrost_graph] %s: this version of arnold_bifrost does not support more than 2GB of data per bif shape, "
                    "please break up your outputs into more or smaller objects, or upgrade your Bifrost distribution.",
                    AiNodeGetName(shape));
         return;
      }

      AtArray *inputsArray = AiArray(1, 1, AI_TYPE_STRING, "input0");
      AiNodeSetArray(shape, "input_names", inputsArray);

      unsigned int numMotionSteps = IsMotionBlurEnabled(MTOA_MBLUR_DEFORM) ? GetNumMotionSteps() : 1;

      if (nBytes >= ARNOLD_MAX_PARAMETER_BYTES)
      {
         // Write the input to a temp file and have the proc read them back in
         // (the proc will delete the file when done)

         AtArray *interpsArray = AiArray(1, 1, AI_TYPE_STRING, "tfile");
         AtArray *filenames_array = AiArrayAllocate(1, numMotionSteps, AI_TYPE_STRING);

#ifdef _WIN32
         char *filename = new char[MAX_PATH_LENGTH];
         bool success = ::tmpnam_s(filename, MAX_PATH_LENGTH) == 0;
         FILE *fp = success ? fopen(filename, "wb") : NULL;
         AiArraySetStr(filenames_array, step, AtString(filename));
         delete[] filename;
#else
         char filename[] = "/tmp/bob-XXXXXX";
         int tmpFD = ::mkstemp(filename);
         bool success = tmpFD > -1;
         FILE *fp = success ? fdopen(tmpFD, "wb") : NULL;
         AiArraySetStr(filenames_array, step, AtString(filename));
#endif

         uint8_t* data = reinterpret_cast<uint8_t*>(&serialisedData[0]);
         size_t numBytesLeft = nBytes;
         unsigned int chunks = 0;
         while (numBytesLeft > 0)
         {
            size_t bytesToWrite = 2 * 1024 * 1024;
            if (bytesToWrite > numBytesLeft)
               bytesToWrite = numBytesLeft;
            fwrite(data, sizeof(uint8_t), bytesToWrite, fp);
            numBytesLeft -= bytesToWrite;
            data += bytesToWrite;
            ++chunks;
         }
         fclose(fp);

         AiNodeDeclare(shape, "bifrost:input0", "constant ARRAY STRING");
         AiNodeSetArray(shape, "bifrost:input0", filenames_array);
         AiNodeSetArray(shape, "input_interpretations", interpsArray);
      }
      else
      {
         // When serializing data, Arnold will B85 encode data if it takes up more
         // than 32 bytes of memory.  We want to use raw bytes rather than printing
         // any float data if B85 encoding won't kick in, otherwise we may not
         // get byte-for-byte transfer from printed floats.
         //
         // Also, prior to arnold_bifrost ABI rev 1, it only accepts BYTE arrays,
         // so we're forced to use that type regardless of encoding for early
         // procedural versions.  In newer versions, we can masquerade the data
         // as a larger-typed size to fit more bytes into each motion key.
         uint8_t arrayType = (s_arnoldBifrostABIRev < 1 || nBytes <= 32) ? AI_TYPE_BYTE : AI_TYPE_RGBA;
         uint64_t arrayTypeSize = arrayType == AI_TYPE_BYTE ? sizeof(uint8_t) : sizeof(AtRGBA);
         const char* arrayDecl = arrayType == AI_TYPE_BYTE ? "constant ARRAY BYTE" : "constant ARRAY RGBA";
         uint64_t extraElem = (nBytes % arrayTypeSize > 0) ? 1 : 0;

         // Send serialized data directly to arnold_bifrost, maximizing size per
         // key by masquerading as a larger-sized data type if necessary
         AtArray *interpsArray = AiArray(1, 1, AI_TYPE_STRING, "serialized");
         AtArray *dataArray = AiArrayAllocate(static_cast<uint32_t>(nBytes / arrayTypeSize + extraElem), numMotionSteps, arrayType);

         uint8_t* data = reinterpret_cast<uint8_t*>(&serialisedData[0]);
         uint8_t *dataList = static_cast<uint8_t*>(AiArrayMapKey(dataArray, step));
         memcpy(dataList, data, nBytes);
         size_t keySize = AiArrayGetKeySize(dataArray);
         if (nBytes < keySize)
            memset(dataList + nBytes, 0, keySize - nBytes);
         AiArrayUnmap(dataArray);

         AiNodeDeclare(shape, "bifrost:input0", arrayDecl);
         AiNodeSetArray(shape, "bifrost:input0", dataArray);
         AiNodeSetArray(shape, "input_interpretations", interpsArray);
      }
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

   unsigned int step = GetMotionStep();

   MDoubleArray serialisedData;
   GetSerializedData(serialisedData);
   size_t serialisedSize = serialisedData.length() * sizeof(double);

   AtArray* dataArray = AiNodeGetArray(shape, "bifrost:input0");
   if (s_arnoldBifrostABIRev < 1 && serialisedSize >= ARNOLD_MAX_BYTE_ARRAY_SIZE)
   {
      // arnold_bifrost only takes an array of BYTEs, and we can only have 2GB
      // in size, so we have to bail as we're over the limit and can't write
      // the data to a temp file (or it won't get cleaned up and will spam
      // the filesystem).  Better to issue an error and let the user break
      // up their Bifrost outputs if possible.
      AiMsgError("[mtoa.bifrost_graph] %s: this version of arnold_bifrost does not support more than 2GB of data per bif shape, "
                 "please break up your outputs into more or smaller objects, or upgrade your Bifrost distribution.",
                 AiNodeGetName(shape));
      return;
   }

   if (AiArrayGetType(dataArray) != AI_TYPE_STRING && serialisedSize >= ARNOLD_MAX_PARAMETER_BYTES)
   {
      // Previous motion samples were directly serialized, but this sample is over
      // the limit for parameter size, we need to stream out all already-written
      // steps to temp files
      size_t keySize = AiArrayGetKeySize(dataArray);
      AtArray *newDataArray = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_STRING);
      for (unsigned int key = 0; key < GetNumMotionSteps(); ++key)
      {
         if (key == step)
            continue;

#ifdef _WIN32
         char *filename = new char[MAX_PATH_LENGTH];
         bool success = ::tmpnam_s(filename, MAX_PATH_LENGTH) == 0;
         FILE *fp = success ? fopen(filename, "wb") : NULL;
         AiArraySetStr(newDataArray, key, AtString(filename));
         delete[] filename;
#else
         char filename[] = "/tmp/bob-XXXXXX";
         int tmpFD = ::mkstemp(filename);
         bool success = tmpFD > -1;
         FILE *fp = success ? fdopen(tmpFD, "wb") : NULL;
         AiArraySetStr(newDataArray, key, AtString(filename));
#endif

         fwrite(AiArrayMapKey(dataArray, key), sizeof(uint8_t), keySize, fp);
         fclose(fp);
         AiArrayUnmap(dataArray);
      }

      AiNodeResetParameter(shape, "bifrost:input0");
      AiNodeDeclare(shape, "bifrost:input0", "constant ARRAY STRING");
      AiNodeSetArray(shape, "bifrost:input0", newDataArray);
      dataArray = newDataArray;

      // Reset the interpretation to temp file
      AtArray *interpsArray = AiNodeGetArray(shape, "input_interpretations");
      AiArraySetStr(interpsArray, 0, "tfile");
   }

   // If the serialized data was too large or we streamed out to a temp file,
   // we also stream this sample out to a temp file (regardless of its size).
   if (AiArrayGetType(dataArray) == AI_TYPE_STRING)
   {
      FILE *fp = NULL;
      AtString current_filename = AiArrayGetStr(dataArray, step);
      if (!current_filename.empty())
      {
         fp = fopen(current_filename.c_str(), "wb");
      }
      else
      {
#ifdef _WIN32
         char* filename = new char[MAX_PATH_LENGTH];
         bool success = ::tmpnam_s(filename, MAX_PATH_LENGTH) == 0;
         fp = success ? fopen(filename, "wb") : NULL;
         AiArraySetStr(dataArray, step, AtString(filename));
         delete[] filename;
#else
         char filename[] = "/tmp/bob-XXXXXX";
         int tmpFD = ::mkstemp(filename);
         bool success = tmpFD > -1;
         fp = success ? fdopen(tmpFD, "wb") : NULL;
         AiArraySetStr(dataArray, step, AtString(filename));
#endif
      }

      fwrite(&serialisedData[0], sizeof(uint8_t), serialisedSize, fp);
      fclose(fp);
      return;
   }

   // Data is serialized to an array, so take care to resize the keys if this
   // one is bigger than the previous keys already filled.
   size_t keySize = AiArrayGetKeySize(dataArray);
   uint8_t nKeys = AiArrayGetNumKeys(dataArray);
   if (nKeys <= step)
   {
      AiMsgWarning("[mtoa.bifrost_graph] attempted motion key export when key is not available");
      return;
   }
   if (serialisedSize > keySize)
   {
      // Note: prior to arnold_bifrost ABI rev 1, it only accepts BYTE arrays,
      // but afterward is accepts any non-STRING data type, and interprets it
      // as raw bytes anyway.  This allows us to store more data per key in memory.
      // We use BYTE when there are 32 bytes or less of data so that we don't
      // print actual floats into an ASS file, otherwise we use RGBA so we can
      // maximize the size per key in B85-encoded format.

      uint8_t arrayType = (s_arnoldBifrostABIRev < 1 || serialisedSize <= 32) ? AI_TYPE_BYTE : AI_TYPE_RGBA;
      size_t arrayTypeSize = arrayType == AI_TYPE_BYTE ? sizeof(uint8_t) : sizeof(AtRGBA);
      const char* arrayDecl = arrayType == AI_TYPE_BYTE ? "constant ARRAY BYTE" : "constant ARRAY RGBA";
      size_t extraElem = (serialisedSize % arrayTypeSize > 0) ? 1 : 0;

      AtArray* newDataArray = AiArrayAllocate(serialisedSize / arrayTypeSize + extraElem, GetNumMotionSteps(), arrayType);
      size_t newKeySize = AiArrayGetKeySize(newDataArray);
      uint8_t* existingData = static_cast<uint8_t*>(AiArrayMap(dataArray));
      uint8_t* newData = static_cast<uint8_t*>(AiArrayMap(newDataArray));
      for (unsigned int key = 0; key < GetNumMotionSteps(); ++key)
      {
         if (key == step)
            continue;
         memcpy(&newData[key * newKeySize], &existingData[key * keySize], keySize);
         // Zero any remaining bytes to be nice to the Bifrost deserialisation parser
         memset(&newData[key * newKeySize + keySize], 0, newKeySize - keySize);
      }
      AiArrayUnmap(dataArray);
      AiArrayUnmap(newDataArray);

      AiNodeResetParameter(shape, "bifrost:input0");
      AiNodeDeclare(shape, "bifrost:input0", arrayDecl);
      AiNodeSetArray(shape, "bifrost:input0", newDataArray);
      // Get the new array and key size for this motion sample's copy below
      dataArray = newDataArray;
      keySize = newKeySize;
   }

   // Copy the data into the current motion key
   uint8_t* data = reinterpret_cast<uint8_t*>(&serialisedData[0]);
   uint8_t *dataList = static_cast<uint8_t*>(AiArrayMapKey(dataArray, step));
   memcpy(dataList, data, serialisedSize);
   // Zero any remaining bytes to be nice to the Bifrost deserialisation parser
   if (serialisedSize < keySize)
      memset(dataList + serialisedSize, 0, keySize - serialisedSize);
   AiArrayUnmap(dataArray);
}
