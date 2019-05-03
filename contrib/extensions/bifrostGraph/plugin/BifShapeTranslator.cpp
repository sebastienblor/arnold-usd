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

static MString s_bifrostProcedural = "arnold_bifrost";
static MString s_bifrostProceduralPath = "";

//#define DEBUG_DUMP_TO_FILE 1

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
   MString cmd("import bifrost_utils; bifrost_utils.get_arnold_bifrost_path()");
   s_bifrostProceduralPath = MGlobal::executePythonCommandStringResult(cmd);

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
   CShapeTranslator::MakeCommonAttributes(helper);

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

void CBifShapeTranslator::Export( AtNode *shape )
{

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
      AiMsgInfo("[mtoa.bifrost_graph] : Exporting plug outputSerializedData");

      MDataHandle handle;
      serialisedDataPlug.getValue(handle);
      MFnDoubleArrayData ArrData(handle.data());
      MDoubleArray serialisedData = ArrData.array();
      unsigned int nEle = serialisedData.length();

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

      AtArray *dataArray = AiArrayConvert(nEle * sizeof(double), 1, AI_TYPE_BYTE, &serialisedData[0]);
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
      AiNodeSetStr(shape, "compound", geomPath.asChar());
   }

   if (RequiresShaderExport())
      ExportShaders(shape);

   ExportProcedural(shape);
}


void CBifShapeTranslator::ExportShaders(AtNode *shape)
{
   if (!LoadBifrostProcedural())
      return;

   AtNode *node = GetArnoldNode();
   if (node == NULL)
      return;

   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   // Exporting all material references done inside the biforst graph
   MPlug materialReferencesPlug = FindMayaPlug("outputMaterialReferences");
   MDataHandle matDataHandle;
   materialReferencesPlug.getValue(matDataHandle);
   MFnStringArrayData data1(matDataHandle.data());
   MStringArray shaders =data1.array();
   if (shaders.length() > 0 )
   {
      AtArray* materialReferences = AiArrayAllocate( shaders.length(), 1, AI_TYPE_STRING );
      AtArray* shaderReferences = AiArrayAllocate( shaders.length(), 1, AI_TYPE_NODE );
      for (uint i = 0; i < shaders.length(); i++)
      {
            MSelectionList selection;
            selection.add(shaders[i]);
            MDagPath shPath;
            MObject shNode;
            selection.getDagPath(0,shPath);
            selection.getDependNode(0,shNode);
            MPlug dummyPlug = MFnDependencyNode(shNode).findPlug("message", true);
            if (dummyPlug.isNull())
               continue;
            
            AtNode* node = ExportConnectedNode(dummyPlug); // do export the shader
            AiArraySetStr(materialReferences, i , shaders[i].asChar());
            AiArraySetPtr(shaderReferences, i , node);
      }
      AiNodeSetArray(shape, "material_references", materialReferences);
      AiNodeSetArray(shape, "shader_references", shaderReferences);
     }


   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroupPlug.isNull())
   {
      MFnDependencyNode shadingEngineNode(shadingGroupPlug.node());
      MPlug surfaceShaderPlug = shadingEngineNode.findPlug("surfaceShader", true);
      MPlugArray connections;
      surfaceShaderPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         std::size_t found_shader = std::string(connections[0].name().asChar()).find(std::string("lambert1")); 
         std::size_t found_engine = std::string(shadingGroupPlug.name().asChar()).find(std::string("initialShadingGroup")); 
         if ( found_shader != std::string::npos && found_engine != std::string::npos )
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

void CBifShapeTranslator::ExportMotion(AtNode *shape)
{
   if (LoadBifrostProcedural() == false || shape == NULL)
      return;
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(shape);

}

