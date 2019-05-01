#include <sstream>

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

#include "extension/Extension.h"
#include "extension/ExtensionsManager.h"

// #define BIFDATA_HACK 1


/*
 * We reproduce this class because there is no other way to get to the Amino
 * array of objects currently.  When an MPlug is provided on the bifShape, we
 * will switch to using that instead to get serialized data to hand over to
 * arnold_bifrost.
 *
 * This must match the layout of the real BifData in maya-bifrostboard-plugin.
 */
#ifdef BIFDATA_HACK

#include <maya/MPxGeometryData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

#include <bifrost_object/bifrost_object_i.h>
#include <geometry/bifrost_serialization.h>

#include <stdOpLib/Cpp/AminoArray.h>


const MTypeId BifDataId(0x00081122);
const MString BifDataTypeName( "bifData" );

class BifData : public MPxData
{
public:
    enum class BifDataModel { NotSet, BifrostExp, };

   //////////////////////////////////////////////////////////////////
   //
   // Overrides from MPxData
   //
   //////////////////////////////////////////////////////////////////

   BifData() : fDataModel(BifDataModel::NotSet) { }
   ~BifData() override { }

   MStatus        readASCII( const MArgList& argList, unsigned& idx ) override { return MS::kSuccess; }
   MStatus        readBinary( istream& in, unsigned length ) override { return MS::kSuccess; }
   MStatus        writeASCII( ostream& out ) override { return MS::kSuccess; }
   MStatus        writeBinary( ostream& out ) override { return MS::kSuccess; }

   MTypeId        typeId() const override { return BifDataId; }
   MString        name() const override { return BifDataTypeName; }

   bool empty() const 
    { 
        switch (fDataModel)
        {
        case BifDataModel::BifrostExp:
            return fBifrostExpObjects.empty();
        default:
            return true;
        }
    }

    Amino::Array<Amino::Ptr<const BifrostExp::Object> >& bifrostObjects()
    {
        fDataModel = BifDataModel::BifrostExp;
        return fBifrostExpObjects;
    }

private:
    BifDataModel fDataModel;
    Amino::Array<Amino::Ptr<const BifrostExp::Object> > fBifrostExpObjects;
};

#endif // BIFDATA_HACK

static bool s_loadedProcedural = false;

static MString s_bifrostProcedural = "arnold_bifrost";

static bool LoadBifrostProcedural()
{
   if (s_loadedProcedural)
      return true;

   if (AiNodeEntryLookUp("bifrost_graph") != NULL)
   {
      s_loadedProcedural = true;
      return true;
   }

   MString bifrostModuleLocation;
   MString cmd("import bifrost_utils; bifrost_utils.get_arnold_bifrost_path()");
   bifrostModuleLocation = MGlobal::executePythonCommandStringResult(cmd);

   AiMsgInfo("[bifrost] %s", bifrostModuleLocation.asChar());

   if (bifrostModuleLocation.length() > 0)
   {
      CExtension *extension = CExtensionsManager::GetExtensionByName("bifShapeTranslator");
      if (extension)
      {
         MFileObject fo;
         fo.setRawFullName(bifrostModuleLocation);
         if (fo.exists())
         {
            extension->LoadArnoldPlugin(s_bifrostProcedural, bifrostModuleLocation);
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
      AiMsgError("Bifrost procedural could not be found");

   CExtensionAttrHelper helper(context.maya, "bifrost_graph");
   CShapeTranslator::MakeCommonAttributes(helper);

   // bifrost_board procedural parameters
   helper.MakeInput("filename");
   helper.MakeInput("compound");
   CAttrData fastBifHackData;
   fastBifHackData.defaultValue.BOOL() = false;
   fastBifHackData.name = "aiFastBifHack";
   fastBifHackData.shortName = "aiFastBifHack";
   helper.MakeInputBoolean(fastBifHackData);

   // procedural namespace parameter
   helper.MakeInput("namespace");

   //// operators attribute for adding operators to custom procedurals
   CAttrData data;
   data.name = "operators";
   data.shortName = "operators";
   data.type = AI_TYPE_NODE;
   data.isArray = true;
   helper.MakeInput(data);
}

AtNode* CBifShapeTranslator::CreateArnoldNodes()
{
   if (!LoadBifrostProcedural())
      AiMsgError("Bifrost procedural could not be found");

   return AddArnoldNode("bifrost_graph");
}

void CBifShapeTranslator::Export( AtNode *shape )
{
   MStatus status = MS::kSuccess;
#ifdef BIFDATA_HACK
   MPlug viewport = FindMayaPlug("viewport");
   MDataHandle handle;
   viewport.getValue(handle);
   MFnPluginData fnData(handle.data());
   BifData* data = static_cast<BifData*>(fnData.data(&status));
   if (data && !data->empty())
   {
      bool fastHack = false;
      MPlug fastHackPlug = FindMayaPlug("aiFastBifHack");
      if (!fastHackPlug.isNull())
         fastHack = fastHackPlug.asBool();
      
      if (fastHack)
      {
         // Pass pointers to bifrost objects directly
         AiMsgWarning("[bifShapeTranslator] %s: using fast hack (passing pointers -- veeerrry dangerous!)", AiNodeGetName(shape));

         AtArray *inputsArray = AiArrayAllocate(static_cast<uint32_t>(data->bifrostObjects().size()), 1, AI_TYPE_STRING);
         // We can get away with one entry, because they're all the same and arnold_bifrost figures this out
         AtArray *interpsArray = AiArray(1, 1, AI_TYPE_STRING, "pointer");

         uint32_t i = 0;
         for (auto iter = data->bifrostObjects().begin(); iter != data->bifrostObjects().end(); ++iter, ++i)
         {
            std::ostringstream inputNamer;
            inputNamer << "input" << i;
            inputNamer.flush();
            std::string inputName = inputNamer.str();
            std::string bifrostInputName = std::string("bifrost:") + inputName;
            AiNodeDeclare(shape, bifrostInputName.c_str(), "constant POINTER");
            // INTENTIONAL LEAK: Dangle a reference to the object and keep it around, otherwise we crash!
            Amino::Ptr<BifrostExp::Object> *mutObj = new Amino::Ptr<BifrostExp::Object>(data->bifrostObjects()[i].toMutable());
            AiNodeSetPtr(shape, bifrostInputName.c_str(), mutObj->get());
            AiArraySetStr(inputsArray, i, AtString(inputName.c_str()));
         }
         AiNodeSetArray(shape, "input_names", inputsArray);
         AiNodeSetArray(shape, "input_interpretations", interpsArray);
      }
      else
      {
         // Serialize to disk, tell the procedural about it
         // NOTE: this temp file workflow sucks, we need real temp files (_tmpnam and friends, perhaps)
#ifdef _WIN32
         std::string filename = "C:\\temp\\bifrost-";
#else
         std::string filename = "/tmp/bifrost-";
#endif
         filename += AiNodeGetName(shape);
         filename += ".bob";
         if (!BifrostExp::IO::serialize(data->bifrostObjects(), Amino::String(filename.c_str())))
         {
            AiMsgError("[bifShapeTranslator] Could not serialized bifrost objects to %s", filename.c_str());
            return;
         }
         AtArray *inputsArray = AiArray(1, 1, AI_TYPE_STRING, "input0");
         AtArray *interpsArray = AiArray(1, 1, AI_TYPE_STRING, "file");
         AiNodeDeclare(shape, "bifrost:input0", "constant STRING");
         AiNodeSetStr(shape, "bifrost:input0", filename.c_str());
         AiNodeSetArray(shape, "input_names", inputsArray);
         AiNodeSetArray(shape, "input_interpretations", interpsArray);
      }
   }
   else
#endif // BIFDATA_HACK
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

         // ProcessArrayParameter(shape, "graph_contents", serialisedDataPlug, AI_TYPE_BYTE);

         MDataHandle handle;
         serialisedDataPlug.getValue(handle);
         MFnDoubleArrayData ArrData(handle.data());
         MDoubleArray serialisedData = ArrData.array();

         // double dVal ;
         unsigned int nEle = serialisedData.length();
         AiMsgInfo("[mtoa.bifrost_graph] : Setting graph_contents, size: %i", nEle);
         AtArray *dataArray = AiArrayAllocate(nEle, 1, AI_TYPE_BYTE);
         for (unsigned int idx=0; idx < nEle; ++idx)
         {
            AiArraySetByte(dataArray, idx, (AtByte)serialisedData[idx]);
         }
         AiNodeSetArray(shape, "graph_contents", dataArray);
      }

      MPlug geomPlug = FindMayaPlug("aiCompound");
      if (!geomPlug.isNull())
      {
         MString geomPath = geomPlug.asString();
         AiNodeSetStr(shape, "compound", geomPath.asChar());
      }
   }

   if (RequiresShaderExport())
      ExportShaders();

   ExportProcedural(shape);
}


void CBifShapeTranslator::ExportShaders()
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

