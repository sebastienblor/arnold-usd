#include <sstream>
#include <stdio.h>

#include "BifShapeTranslator.h"

#include <maya/MAnimControl.h>
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
#include "BifrostUtils.h"


#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif


// 2 GB, e.g. 2^31 elements of single bytes, matching limits of Arnold 5.4.0.0 and earlier
// (newer versions have a fix allowing the full 2^32 elements, or 4 GB per array key)
static const size_t ARNOLD_MAX_BYTE_ARRAY_SIZE = size_t(2) * 1024UL * 1024UL * 1024UL;
// 32 GB, e.g. 2^31 elements of 16-byte RGBAs, matching the max size of any array key possible in Arnold
static const size_t ARNOLD_MAX_PARAMETER_BYTES = size_t(2) * 1024UL * 1024UL * 1024UL * sizeof(AtRGBA);

static const size_t MAX_PATH_LENGTH = 4096;

static std::string GenerateBobPath(const std::string& exportFile, const AtNode* node, double frame)
{
   std::ostringstream bobPathStream;

   // Drop the extension
   std::string exportPrefix;
   size_t pos = exportFile.rfind('.');
   size_t dirBasePos = exportFile.rfind('/');
   if (pos == std::string::npos || pos < dirBasePos)
      exportPrefix = exportFile;
   else
      exportPrefix = exportFile.substr(0, pos);
   bobPathStream << exportPrefix << "-";

   std::string nodePart(AiNodeGetName(node));
   // Sanitize node name so it's suitable to be an on-disk filename
   for (size_t i = 0; i < nodePart.size(); ++i)
   {
      if (nodePart[i] == '\\' || nodePart[i] == '/' || nodePart[i] == '|' || nodePart[i] == ':')
         nodePart[i] = '_';
   }
   // Skip any leading underscores, they're not helpful
   nodePart = nodePart.substr(nodePart.find_first_not_of('_'), std::string::npos);
   bobPathStream << nodePart;

   // Generate a frame spec from a frame, of the form ..._<[-]int>_<4 digit frac>
   double wholePart;
   double fracPart = std::modf(frame, &wholePart);
   int intSpec = static_cast<int>(wholePart);
   int fracSpec = static_cast<int>(std::abs(fracPart) * 1000.0);
   bobPathStream << "_" << intSpec << '_' << fracSpec;

   bobPathStream << ".bob";
   bobPathStream.flush();

   std::string path = bobPathStream.str();
   // Convert to native path for Windows
#ifdef _WIN32
   for (size_t i = 0; i < path.size(); ++i)
   {
      if (path[i] == '/')
         path[i] = '\\';
   }
#endif

   // Create all parent directories as needed so the final file can be written
   bool success = true;
   for (size_t delimiterPos = path.find_first_of("/\\"); delimiterPos != std::string::npos; delimiterPos = path.find_first_of("/\\", delimiterPos + 1))
   {
      std::string subpath = path.substr(0, delimiterPos);
      if (subpath.empty())
         continue;
#ifdef _WIN32
      int err = _mkdir(subpath.c_str());
      if (err != 0 && errno != EEXIST)
      {
         AiMsgWarning("[mtoa.bifrost_graph] %s: could not create directories for path %s (subpath %s, error=%d)",
                      AiNodeGetName(node),
                      path.c_str(),
                      subpath.c_str(),
                      err);
         success = false;
         break;
      }
#else
      int err = mkdir(subpath.c_str(), 0755);
      if (err != 0 && errno != EEXIST && errno != EACCES)
      {
         AiMsgWarning("[mtoa.bifrost_graph] %s: could not create directories for path %s (subpath %s, error=%d)",
                      AiNodeGetName(node),
                      path.c_str(),
                      subpath.c_str(),
                      err);
         success = false;
         break;
      }
#endif
   }
   return success ? path : std::string();
}

// Return a base path in maya workspace folder, under bifrost.
// Later on, GenerateBobPath will compute a final .bob filename
// for every step

static MString GetBaseBobPath()
{
   MString filename;
   MString sceneFilename;
   MGlobal::executeCommand("file -q -sn", sceneFilename);

   MStatus status = MGlobal::executeCommand(MString("workspace -q -rd;"), filename);

   // If no workspace was found, let's just return the name of the scene,
   // the bob file will then be saved next to the maya scene
   if (status != MS::kSuccess)
      return sceneFilename;

   filename += "/bifrost/";

   // extract the basename of the maya scene
   MFileObject fileObj;
   fileObj.setRawFullName(sceneFilename);

   filename += fileObj.rawName();
   return filename;
}

void CBifShapeTranslator::NodeInitializer(CAbTranslator context)
{
   if (!BifrostUtils::LoadBifrostProcedural())
      AiMsgWarning("Bifrost procedural could not be found: %s", BifrostUtils::GetBifrostProceduralPath().asChar());

   MNodeClass bifrostClass(MString("bifrostGraphShape"));
   if (bifrostClass.typeId() == MTypeId(MFn::kInvalid))
      return;

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
   if (!BifrostUtils::LoadBifrostProcedural())
     AiMsgError("Bifrost procedural could not be found: %s", BifrostUtils::GetBifrostProceduralPath().asChar());

   return AddArnoldNode("bifrost_graph");
}

void CBifShapeTranslator::Export( AtNode *shape )
{

   unsigned int step = GetMotionStep();
   double frame = MAnimControl::currentTime().as(MTime::uiUnit());
   bool velocityOnly = FindMayaPlug("aiMotionBlurMode").asInt() == 1;

   MPlug vpRenderPlug = FindMayaPlug("viewportRenderSelect");
   if (!vpRenderPlug.isNull())
   {
      int vpRenderValue = vpRenderPlug.asInt();
      if (vpRenderValue == 0)
         vpRenderPlug.setValue(1);
      else if (vpRenderValue == 2)
         vpRenderPlug.setValue(3);
   }   

   // export BifShape parameters
   MPlug filenamePlug = FindMayaPlug("aiFilename");
   if (!filenamePlug.isNull() && !filenamePlug.isDefaultValue())
   {
      MString filename = filenamePlug.asString();
      AiNodeSetStr(shape, "filename", filename.asChar());
   }

   // export the Bifrost graph results if no file is supplied
   MPlug dataStreamPlug     = FindMayaPlug("outputBifrostDataStream"); // Preferred plug
   MPlug serialisedDataPlug = FindMayaPlug("outputSerializedData");    // Legacy plug
   if (!dataStreamPlug.isNull() && BifrostUtils::GetArnoldBifrostABIRev() >= 2)
   {
      AiMsgDebug("[mtoa.bifrost_graph] %s: exporting plug outputBifrostDataStream for motion step %u (frame %f)",
                 AiNodeGetName(shape),
                 step,
                 frame);
      MDataHandle dataStreamHandle;
      dataStreamPlug.getValue(dataStreamHandle);
      MFnDoubleArrayData arrData(dataStreamHandle.data());
      MDoubleArray streamData = arrData.array();

      unsigned int nEle = streamData.length();
      size_t nBytes = static_cast<size_t>(nEle) * sizeof(double);

      AtArray *inputsArray = AiArray(1, 1, AI_TYPE_STRING, "input0");
      AiNodeSetArray(shape, "input_names", inputsArray);

      unsigned int numMotionSteps = IsMotionBlurEnabled(MTOA_MBLUR_DEFORM) && !velocityOnly ? GetNumMotionSteps() : 1;

      MString baseBobPath = GetBaseBobPath();
      if (GetSessionMode() != MTOA_SESSION_ASS || baseBobPath.length() == 0)
      {
         // Pass pointers (handles) over for speed, as there's no danger of
         // pointers being written to disk

         // arnold_bifrost turns the handle stream into an actual pointer handle
         // for us to pass back to it.  It takes care of lifetime issues with
         // the smart pointers the handle refers to.
         void* handle = BifrostUtils::s_arnoldBifrostPrepareHandle(&streamData[0], nBytes);

         // Send serialized data directly to arnold_bifrost, maximizing size per
         // key by masquerading as a larger-sized data type if necessary
         AtArray *interpsArray = AiArray(1, 1, AI_TYPE_STRING, "handle");
         AtArray *dataArray = AiArrayAllocate(1, numMotionSteps, AI_TYPE_POINTER);
         for (unsigned int i = 0; i < numMotionSteps; ++i)
            AiArraySetPtr(dataArray, i, i == step ? handle : NULL);

         AiNodeDeclare(shape, "bifrost:input0", "constant ARRAY POINTER");
         AiNodeSetArray(shape, "bifrost:input0", dataArray);
         AiNodeSetArray(shape, "input_interpretations", interpsArray);
      }
      else
      {
         AiMsgDebug("[mtoa.bifrost_graph] %s: detected Arnold file export, will use Bifrost serialized data",
                    AiNodeGetName(shape));

         // Convert the handle data stream to fully-serialized Bifrost data
         size_t serialisedNBytes = 0;
         uint8_t* serialisedData = BifrostUtils::s_arnoldBifrostSerializeData(&streamData[0], nBytes, serialisedNBytes);

         // Write fully-serialized data to disk (or inline into AtArray if it fits)
         if (serialisedNBytes >= ARNOLD_MAX_PARAMETER_BYTES)
         {
            // Write the input to a file and have the proc it later
            std::string bobFilePath = GenerateBobPath(baseBobPath.asChar(), shape, frame);
            if (!bobFilePath.empty())
            {
                AtArray *interpsArray = AiArray(1, 1, AI_TYPE_STRING, "file");
                AtArray *filenames_array = AiArrayAllocate(1, numMotionSteps, AI_TYPE_STRING);

                AiArraySetStr(filenames_array, step, AtString(bobFilePath.c_str()));

                AiMsgInfo("[mtoa.bifrost_graph] %s: serialized Bifrost data too large, serializing to disk as: %s",
                          AiNodeGetName(shape),
                          bobFilePath.c_str());

                FILE *fp = fopen(bobFilePath.c_str(), "wb");
                uint8_t* data = serialisedData;
                size_t numBytesLeft = serialisedNBytes;
                while (numBytesLeft > 0)
                {
                   size_t bytesToWrite = 2 * 1024 * 1024;
                   if (bytesToWrite > numBytesLeft)
                      bytesToWrite = numBytesLeft;
                   fwrite(data, sizeof(uint8_t), bytesToWrite, fp);
                   numBytesLeft -= bytesToWrite;
                   data += bytesToWrite;
                }
                fclose(fp);

                AiNodeDeclare(shape, "bifrost:input0", "constant ARRAY STRING");
                AiNodeSetArray(shape, "bifrost:input0", filenames_array);
                AiNodeSetArray(shape, "input_interpretations", interpsArray);
            }
            else
               AiMsgWarning("[mtoa.bifrost_graph] %s: could not write Bifrost serialized data, maya project area inaccessible", AiNodeGetName(shape));
         }
         else if (serialisedNBytes > 0)
         {
            // When serializing data, Arnold will B85 encode data if it takes up more
            // than 32 bytes of memory.  We want to use raw bytes rather than printing
            // any float data if B85 encoding won't kick in, otherwise we may not
            // get byte-for-byte transfer from printed floats.
            uint8_t arrayType = (serialisedNBytes <= 32) ? AI_TYPE_BYTE : AI_TYPE_RGBA;
            uint64_t arrayTypeSize = arrayType == AI_TYPE_BYTE ? sizeof(uint8_t) : sizeof(AtRGBA);
            const char* arrayDecl = arrayType == AI_TYPE_BYTE ? "constant ARRAY BYTE" : "constant ARRAY RGBA";
            uint64_t extraElem = (serialisedNBytes % arrayTypeSize > 0) ? 1 : 0;

            // Send serialized data directly to arnold_bifrost, maximizing size per
            // key by masquerading as a larger-sized data type if necessary
            AtArray *interpsArray = AiArray(1, 1, AI_TYPE_STRING, "serialized");
            AtArray *dataArray = AiArrayAllocate(static_cast<uint32_t>(serialisedNBytes / arrayTypeSize + extraElem), numMotionSteps, arrayType);

            uint8_t* data = serialisedData;
            uint8_t *dataList = static_cast<uint8_t*>(AiArrayMapKey(dataArray, step));
            memcpy(dataList, data, serialisedNBytes);
            size_t keySize = AiArrayGetKeySize(dataArray);
            if (serialisedNBytes < keySize)
               memset(dataList + serialisedNBytes, 0, keySize - serialisedNBytes);
            AiArrayUnmap(dataArray);

            AiNodeDeclare(shape, "bifrost:input0", arrayDecl);
            AiNodeSetArray(shape, "bifrost:input0", dataArray);
            AiNodeSetArray(shape, "input_interpretations", interpsArray);
         }

         if (serialisedData != NULL)
            AiFree(serialisedData);
      }

      dataStreamPlug.destructHandle(dataStreamHandle);
   }
   else if (!serialisedDataPlug.isNull())
   {
      // Legacy plug for backwards compatibility.  This plug can only output
      // up to 4 GB of data maximum, see ARNOLD-147 for details.

      AiMsgInfo("[mtoa.bifrost_graph] %s: exporting plug outputSerializedData for motion step %u (frame %f)",
                AiNodeGetName(shape),
                step,
                frame);
      MDataHandle serialisedDataHandle;
      serialisedDataPlug.getValue(serialisedDataHandle);
      MFnDoubleArrayData arrData(serialisedDataHandle.data());
      MDoubleArray serialisedData = arrData.array();

      unsigned int nEle = serialisedData.length();
      size_t nBytes = static_cast<size_t>(nEle) * sizeof(double);

      if (BifrostUtils::GetArnoldBifrostABIRev() < 1 && nBytes >= ARNOLD_MAX_BYTE_ARRAY_SIZE)
      {
         // Early versions of arnold_bifrost only take an array of BYTEs, and
         // we can only have 2GB in size, so we have to bail as we're over the
         // limit and can't write the data to a temp file (or it won't get
         // cleaned up and will spam the filesystem).  Better to issue an error
         // and let the user break up their Bifrost outputs if possible.
         AiMsgError("[mtoa.bifrost_graph] %s: this version of arnold_bifrost does not support more than 2GB of data per bif shape, "
                    "please break up your outputs into more or smaller objects, or upgrade your Bifrost distribution.",
                    AiNodeGetName(shape));
         serialisedDataPlug.destructHandle(serialisedDataHandle);
         return;
      }

      AtArray *inputsArray = AiArray(1, 1, AI_TYPE_STRING, "input0");
      AiNodeSetArray(shape, "input_names", inputsArray);

      unsigned int numMotionSteps = IsMotionBlurEnabled(MTOA_MBLUR_DEFORM) && !velocityOnly ? GetNumMotionSteps() : 1;

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
         uint8_t arrayType = (BifrostUtils::GetArnoldBifrostABIRev() < 1 || nBytes <= 32) ? AI_TYPE_BYTE : AI_TYPE_RGBA;
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

      serialisedDataPlug.destructHandle(serialisedDataHandle);
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
      materialReferencesPlug.destructHandle(matDataHandle);
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

   if (!BifrostUtils::LoadBifrostProcedural())
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
         bool found_shader = namespaceEndsWith(connections[0].name().asChar(), "lambert1") || 
                             namespaceEndsWith(connections[0].name().asChar(), "standardSurface1");;
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
   return CProceduralTranslator::RequiresMotionData();
}

void CBifShapeTranslator::ExportMotion(AtNode *shape)
{
   if (BifrostUtils::LoadBifrostProcedural() == false || shape == NULL)
      return;

   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(shape);

   if (!IsMotionBlurEnabled(MTOA_MBLUR_DEFORM)) return;

   MPlug filenamePlug = FindMayaPlug("aiFilename");
   if (!filenamePlug.isNull() && !filenamePlug.isDefaultValue())
   {
      // Export already done in first exported step
      return;
   }

   unsigned int step = GetMotionStep();
   double frame = MAnimControl::currentTime().as(MTime::uiUnit());
   bool velocityOnly = FindMayaPlug("aiMotionBlurMode").asInt() == 1;
   unsigned int numMotionSteps = velocityOnly ? 1 : GetNumMotionSteps();
   if (step >= numMotionSteps)
      return;

   MString baseBobPath = GetBaseBobPath();

   MPlug dataStreamPlug     = FindMayaPlug("outputBifrostDataStream"); // Preferred plug
   MPlug serialisedDataPlug = FindMayaPlug("outputSerializedData");    // Legacy plug

   if (dataStreamPlug.isNull() && serialisedDataPlug.isNull())
      return;

   if (!dataStreamPlug.isNull())
   {
      AiMsgDebug("[mtoa.bifrost_graph] %s: exporting plug outputBifrostDataStream for motion step %u (frame %f)",
                 AiNodeGetName(shape),
                 step,
                 frame);
      MDataHandle dataStreamHandle;
      dataStreamPlug.getValue(dataStreamHandle);
      MFnDoubleArrayData arrData(dataStreamHandle.data());
      MDoubleArray streamData = arrData.array();
      size_t streamSize = streamData.length() * sizeof(double);

      AtArray* dataArray = AiNodeGetArray(shape, "bifrost:input0");

      if (GetSessionMode() != MTOA_SESSION_ASS || baseBobPath.length() == 0)
      {
         // Pass pointers (handles) over for speed, as there's no danger of
         // pointers being written to disk

         // arnold_bifrost turns the handle stream into an actual pointer handle
         // for us to pass back to it.  It takes care of lifetime issues with
         // the smart pointers the handle refers to.
         void* handle = BifrostUtils::s_arnoldBifrostPrepareHandle(&streamData[0], streamSize);

         // This is the simplest case: just set the handle for the current motion step
         AiArraySetPtr(dataArray, step, handle);
      }
      else
      {
         // Convert the handle data stream to fully-serialized Bifrost data
         size_t serialisedSize = 0;
         uint8_t* serialisedData = BifrostUtils::s_arnoldBifrostSerializeData(&streamData[0], streamSize, serialisedSize);
         if (serialisedData == NULL || serialisedSize == 0)
         {
             // We already get a warning from arnold_bifrost about a failed/empty serialization, no need to issue another
             dataStreamPlug.destructHandle(dataStreamHandle);
             return;
         }

         if (AiArrayGetType(dataArray) != AI_TYPE_STRING && serialisedSize >= ARNOLD_MAX_PARAMETER_BYTES)
         {
            // Previous motion samples were directly serialized, but this sample is over
            // the limit for parameter size, we need to stream out all already-written
            // steps to files as well.
            unsigned int frameSteps = 1;
            const double *frameTimes = GetMotionFrames(frameSteps);
            size_t keySize = AiArrayGetKeySize(dataArray);
            AtArray *newDataArray = AiArrayAllocate(1, numMotionSteps, AI_TYPE_STRING);
            for (unsigned int key = 0; key < numMotionSteps; ++key)
            {
               if (key == step)
                  continue;

               // Rewrite the data with the frame time of the other motion key (if available)
               double frameTime = (key < frameSteps && frameTimes != NULL) ? frameTimes[key] : frame;
               std::string bobFilePath = GenerateBobPath(baseBobPath.asChar(), shape, frameTime);
               if (!bobFilePath.empty())
               {
                  AiArraySetStr(newDataArray, key, AtString(bobFilePath.c_str()));
                  FILE *fp = fopen(bobFilePath.c_str(), "wb");
                  fwrite(AiArrayMapKey(dataArray, key), sizeof(uint8_t), keySize, fp);
                  fclose(fp);
                  AiArrayUnmap(dataArray);
               }
               else
               {
                  AiMsgWarning("[mtoa.bifrost_graph] %s: could not write Bifrost serialized data, maya project area inaccessible", AiNodeGetName(shape));
                  AiFree(serialisedData);
                  dataStreamPlug.destructHandle(dataStreamHandle);
                  return;
               }
            }

            AiNodeResetParameter(shape, "bifrost:input0");
            AiNodeDeclare(shape, "bifrost:input0", "constant ARRAY STRING");
            AiNodeSetArray(shape, "bifrost:input0", newDataArray);
            dataArray = newDataArray;

            // Reset the interpretation to temp file
            AtArray *interpsArray = AiNodeGetArray(shape, "input_interpretations");
            AiArraySetStr(interpsArray, 0, "file");
         }

         // If the serialized data was too large or we streamed out to a temp file,
         // we also stream this sample out to a temp file (regardless of its size).
         if (AiArrayGetType(dataArray) == AI_TYPE_STRING)
         {
            FILE *fp = NULL;
            std::string bobFilePath = GenerateBobPath(baseBobPath.asChar(), shape, frame);
            if (!bobFilePath.empty())
            {
               AiArraySetStr(dataArray, step, AtString(bobFilePath.c_str()));
               fp = fopen(bobFilePath.c_str(), "wb");
               fwrite(&serialisedData[0], sizeof(uint8_t), serialisedSize, fp);
               fclose(fp);
            }
            else
               AiMsgWarning("[mtoa.bifrost_graph] %s: could not write Bifrost serialized data, maya project area inaccessible", AiNodeGetName(shape));

            AiFree(serialisedData);
            dataStreamPlug.destructHandle(dataStreamHandle);
            return;
         }

         // Data is serialized to an array, so take care to resize the keys if this
         // one is bigger than the previous keys already filled.
         size_t keySize = AiArrayGetKeySize(dataArray);
         uint8_t nKeys = AiArrayGetNumKeys(dataArray);
         if (nKeys <= step)
         {
            AiMsgWarning("[mtoa.bifrost_graph] %s: attempted motion key export when key %u is not available",
                         AiNodeGetName(shape),
                         step);
            AiFree(serialisedData);
            dataStreamPlug.destructHandle(dataStreamHandle);
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

            uint8_t arrayType = serialisedSize <= 32 ? AI_TYPE_BYTE : AI_TYPE_RGBA;
            size_t arrayTypeSize = arrayType == AI_TYPE_BYTE ? sizeof(uint8_t) : sizeof(AtRGBA);
            const char* arrayDecl = arrayType == AI_TYPE_BYTE ? "constant ARRAY BYTE" : "constant ARRAY RGBA";
            size_t extraElem = (serialisedSize % arrayTypeSize > 0) ? 1 : 0;

            AtArray* newDataArray = AiArrayAllocate(serialisedSize / arrayTypeSize + extraElem, numMotionSteps, arrayType);
            size_t newKeySize = AiArrayGetKeySize(newDataArray);
            uint8_t* existingData = static_cast<uint8_t*>(AiArrayMap(dataArray));
            uint8_t* newData = static_cast<uint8_t*>(AiArrayMap(newDataArray));
            for (unsigned int key = 0; key < numMotionSteps; ++key)
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
         uint8_t* data = serialisedData;
         uint8_t *dataList = static_cast<uint8_t*>(AiArrayMapKey(dataArray, step));
         memcpy(dataList, data, serialisedSize);
         // Zero any remaining bytes to be nice to the Bifrost deserialisation parser
         if (serialisedSize < keySize)
            memset(dataList + serialisedSize, 0, keySize - serialisedSize);
         AiArrayUnmap(dataArray);

         AiFree(serialisedData);
      }
      dataStreamPlug.destructHandle(dataStreamHandle);
   }
   else
   {
      // Legacy plug for backwards compatibility.  This plug can only output
      // up to 4 GB of data maximum, see ARNOLD-147 for details.

      AiMsgDebug("[mtoa.bifrost_graph] %s: exporting plug outputSerializedData for motion step %u (frame %f)",
                 AiNodeGetName(shape),
                 step,
                 frame);
      MDataHandle serialisedDataHandle;
      serialisedDataPlug.getValue(serialisedDataHandle);
      MFnDoubleArrayData arrData(serialisedDataHandle.data());
      MDoubleArray serialisedData = arrData.array();
      size_t serialisedSize = serialisedData.length() * sizeof(double);

      AtArray* dataArray = AiNodeGetArray(shape, "bifrost:input0");
      if (BifrostUtils::GetArnoldBifrostABIRev() < 1 && serialisedSize >= ARNOLD_MAX_BYTE_ARRAY_SIZE)
      {
         // arnold_bifrost only takes an array of BYTEs, and we can only have 2GB
         // in size, so we have to bail as we're over the limit and can't write
         // the data to a temp file (or it won't get cleaned up and will spam
         // the filesystem).  Better to issue an error and let the user break
         // up their Bifrost outputs if possible.
         AiMsgError("[mtoa.bifrost_graph] %s: this version of arnold_bifrost does not support more than 2GB of data per bif shape, "
                    "please break up your outputs into more or smaller objects, or upgrade your Bifrost distribution.",
                    AiNodeGetName(shape));
         serialisedDataPlug.destructHandle(serialisedDataHandle);
         return;
      }

      if (AiArrayGetType(dataArray) != AI_TYPE_STRING && serialisedSize >= ARNOLD_MAX_PARAMETER_BYTES)
      {
         // Previous motion samples were directly serialized, but this sample is over
         // the limit for parameter size, we need to stream out all already-written
         // steps to temp files
         size_t keySize = AiArrayGetKeySize(dataArray);
         AtArray *newDataArray = AiArrayAllocate(1, numMotionSteps, AI_TYPE_STRING);
         for (unsigned int key = 0; key < numMotionSteps; ++key)
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
         serialisedDataPlug.destructHandle(serialisedDataHandle);
         return;
      }

      // Data is serialized to an array, so take care to resize the keys if this
      // one is bigger than the previous keys already filled.
      size_t keySize = AiArrayGetKeySize(dataArray);
      uint8_t nKeys = AiArrayGetNumKeys(dataArray);
      if (nKeys <= step)
      {
         AiMsgWarning("[mtoa.bifrost_graph] %s: attempted motion key export when key %u is not available",
                      AiNodeGetName(shape),
                      step);
         serialisedDataPlug.destructHandle(serialisedDataHandle);
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

         uint8_t arrayType = (BifrostUtils::GetArnoldBifrostABIRev() < 1 || serialisedSize <= 32) ? AI_TYPE_BYTE : AI_TYPE_RGBA;
         size_t arrayTypeSize = arrayType == AI_TYPE_BYTE ? sizeof(uint8_t) : sizeof(AtRGBA);
         const char* arrayDecl = arrayType == AI_TYPE_BYTE ? "constant ARRAY BYTE" : "constant ARRAY RGBA";
         size_t extraElem = (serialisedSize % arrayTypeSize > 0) ? 1 : 0;

         AtArray* newDataArray = AiArrayAllocate(serialisedSize / arrayTypeSize + extraElem, numMotionSteps, arrayType);
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
      serialisedDataPlug.destructHandle(serialisedDataHandle);
   }
}


void CBifShapeTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if (plugName == "viewportRenderSelect")
      return; // we don't want to update IPR when this attribute changes

   CProceduralTranslator::NodeChanged(node, plug);
}
