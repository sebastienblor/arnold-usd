#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning( disable : 4244 )
#pragma warning( disable : 4305 )

#elif defined(_LINUX) || defined(_DARWIN)
#include <dlfcn.h>
#endif

#include <algorithm>
#include <string>


#include "UsdProxyShapeTranslator.h"
#include "translators/NodeTranslatorImpl.h"

#include <maya/MRenderLineArray.h>
#include <maya/MRenderLine.h>
#include <maya/MDagPathArray.h>
#include <maya/MPointArray.h>
#include <maya/MHairSystem.h>
#include <maya/MFnPfxGeometry.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MAnimControl.h>
#include <maya/MSelectionList.h>
#include <maya/MDGModifier.h>
#include <maya/MDGMessage.h>
#include <maya/MEventMessage.h>
#include <maya/MGlobal.h>

CUsdProxyShapeTranslator::CUsdProxyShapeTranslator() : CProceduralTranslator(),
                                                       m_cacheId(0) 
{}

CUsdProxyShapeTranslator::~CUsdProxyShapeTranslator()
{   
   ClearListener();
}

void CUsdProxyShapeTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "usd");
   CShapeTranslator::MakeCommonAttributes(helper);

   helper.MakeInput("namespace");
   //// operators
   CAttrData data;
   data.name = "operators";
   data.shortName = "operators";
   data.type = AI_TYPE_NODE;
   data.isArray = true;
   helper.MakeInput(data);

   data.name = "overrides";
   data.shortName = "overrides";
   data.isArray = true;
   data.type = AI_TYPE_STRING;
   helper.MakeInput(data);   

   data.type = AI_TYPE_BOOLEAN;
   data.isArray = false;
   data.defaultValue.BOOL() = false;
   data.name = "overrideReceiveShadows";
   data.shortName = "overrideReceiveShadows";
   helper.MakeInputBoolean(data);
   
   //The 'receiveShadows' attribute is defined in CDagTranslator::MakeMayaVisibilityFlags
   
   data.defaultValue.BOOL() = false;
   data.name = "overrideDoubleSided";
   data.shortName = "overrideDoubleSided";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "overrideSelfShadows";
   data.shortName = "overrideSelfShadows";
   helper.MakeInputBoolean(data);
   
   //The 'self_shadows' attribute is defined in CShapeTranslator::MakeCommonAttributes

   data.defaultValue.BOOL() = false;
   data.name = "overrideOpaque";
   data.shortName = "overrideOpaque";
   helper.MakeInputBoolean(data);

   //The 'opaque' attribute is defined in CShapeTranslator::MakeCommonAttributes
   data.defaultValue.BOOL() = false;
   data.name = "overrideMatte";
   data.shortName = "overrideMatte";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = true;
   data.name = "shareUsdStage";
   data.shortName = "shareUsdStage";
   helper.MakeInputBoolean(data);

}
AtNode* CUsdProxyShapeTranslator::CreateArnoldNodes()
{
   // If mode is export to ass, just export to usd
#ifdef ENABLE_USD
   // When exporting to .ass, we always want to export to the builtin core procedural.
   // This way, it can be ported to other DCCs, or be kicked in standalone
   if (IsFileExport() || !FindMayaPlug("shareUsdStage").asBool()) {
      return AddArnoldNode("usd");   
   }
      

   if (AiNodeEntryLookUp("usd_cache") == nullptr)
   {
      static MString s_mtoaExtPath;
      if (s_mtoaExtPath.length() == 0) 
          MGlobal::executeCommand("getenv MTOA_EXTENSIONS_PATH", s_mtoaExtPath);
      
      // FIXME replace by USD version
      if (s_mtoaExtPath.length() > 0)
      {
         // Find if we're in python2 or python3
         const char* envVar = getenv("MAYA_PYTHON_VERSION");
         MString envVarStr = (envVar) ? MString(envVar) : MString("3");
         
         MString usdVersion;
         MGlobal::executePythonCommand("from pxr import Usd; Usd.GetVersion()", usdVersion);
         usdVersion.substitute(MString("("), MString(""));
         usdVersion.substitute(MString(")"), MString(""));
         usdVersion.substitute(MString(" "), MString(""));
         
         MStringArray splitUsdVersion;
         usdVersion.split(',', splitUsdVersion);
         MString fullVersion;
         
         for (unsigned int i = 0; i < splitUsdVersion.length(); ++i)
         {
            MString str = splitUsdVersion[i];
            if (str.length() == 0 || str == "0")
               continue;
            if (str.length() == 1)
               str = MString("0") + str;
            if (str.length() == 2)
               fullVersion += str;
         }
         
         if (envVarStr == MString("2"))
            fullVersion += MString("_python2");

         MString usdFolder = MString("/usd/") + fullVersion;
         

         MString usdCachePath = s_mtoaExtPath + usdFolder;
         AiLoadPlugins(usdCachePath.asChar());
         if (AiNodeEntryLookUp("usd_cache"))   
            return AddArnoldNode("usd_cache");
      }
   } else 
      return AddArnoldNode("usd_cache");
#endif
   return AddArnoldNode("usd");
}

// Callback is called whenever a gpuCache node is created
void CUsdProxyShapeTranslator::timeChangedCallback(void* clientData)
{

   CUsdProxyShapeTranslator * translator = static_cast< CUsdProxyShapeTranslator* >(clientData);
   if (translator != NULL)
   {
      // translator->SetUpdateMode(AI_RECREATE_NODE);
      translator->RequestUpdate();
   }

}

void CUsdProxyShapeTranslator::AddUpdateCallbacks()
{

   MStatus status;
   MCallbackId id;

   MObject object = GetMayaObject();
   id = MEventMessage::addEventCallback("timeChanged", 
                                        CUsdProxyShapeTranslator::timeChangedCallback,
                                        this,
                                        &status);

   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   CShapeTranslator::AddUpdateCallbacks();
}

void CUsdProxyShapeTranslator::Export( AtNode *shape )
{
   if (shape == NULL)
      return;

   // ExportMatrix(shape);
   // ProcessRenderFlags(shape);

   // export gpuCache parameters   
   MPlug filenamePlug = FindMayaPlug("filePath");
   if (!filenamePlug.isNull())
   {
      MString filename = filenamePlug.asString().expandEnvironmentVariablesAndTilde();
      GetSessionOptions().FormatProceduralPath(filename);
      AiNodeSetStr(shape, "filename", filename.asChar());

   }

   bool isUsdCache = false;
#ifdef ENABLE_USD
   isUsdCache = AiNodeIs(shape, AtString("usd_cache"));
#endif

   if (isUsdCache)
   {
      MPlug cacheIdPlug = FindMayaPlug("outStageCacheId");
      if (!cacheIdPlug.isNull())
      {
         int cacheId = cacheIdPlug.asInt();
         std::string cacheStr;
         AiNodeSetInt(shape, "cache_id", cacheId);
         
         // Run a python command that will ping us every time there is a change on this USD stage.
         // This will be needed to update the IPR
         if (IsInteractiveSession())
         {
            if (cacheId != m_cacheId)
            {
               ClearListener();
            }
            m_cacheId = cacheId;

            MString cmd("import usdProxyShapeTranslator;usdProxyShapeTranslator.UsdArnoldListener(");
            cmd += cacheId;
            cmd += ",'";
            cmd += GetMayaNodeName();
            cmd += "','";
            cmd += m_impl->m_session->GetSessionName();
            cmd += "')";
            MGlobal::executePythonCommand(cmd);
         }
      }
   } else {
      MPlug overridesPlug = FindMayaPlug("overrides");
      if (!overridesPlug.isNull() )
         ProcessParameter(shape, AtString("overrides"), AI_TYPE_ARRAY);
   }

   MPlug geomPlug = FindMayaPlug("primPath");
   if (!geomPlug.isNull())
   {
      std::string geomPath = geomPlug.asString().asChar();
      std::replace( geomPath.begin(), geomPath.end(), '|', '/'); // Maya converts '/' into '|' 
      AiNodeSetStr(shape, "object_path", geomPath.c_str());
   } 

   if (RequiresShaderExport())
      ExportShaders();

   MTime curTime = MAnimControl::currentTime();

   MTime::Unit unit = curTime.unit();

   MTime time(FindMayaPlug("time").asFloat(), MTime::kSeconds);
   AiNodeSetFlt(shape, "frame", float(time.as(unit)));
  
   ExportProcedural(shape);
}


void CUsdProxyShapeTranslator::ExportShaders()
{
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

void CUsdProxyShapeTranslator::ExportMotion(AtNode *shape)
{
   if (shape == NULL)
      return;
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(shape);

}

void CUsdProxyShapeTranslator::ClearListener()
{
   if (m_cacheId)
   {
      MString cmd("import usdProxyShapeTranslator;usdProxyShapeTranslator.UsdArnoldUnregisterListener(");
      cmd += m_cacheId;
      cmd += ")";
      MGlobal::executePythonCommand(cmd);
      m_cacheId = 0;
   }
}

void CUsdProxyShapeTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   m_attrChanged = true; // this flag tells me that I've been through a NodeChanged call
   MString plugName = plug.partialName(false, false, false, false, false, true);

   if (plugName == "selectedItems" || plugName == "selected_items" || 
      plugName == "MinBoundingBox0" || plugName == "MinBoundingBox1" || plugName == "MinBoundingBox2" || 
      plugName == "MaxBoundingBox0" || plugName == "MaxBoundingBox1" || plugName == "MaxBoundingBox2") return;

   // if the cache ID is being modified, we want to delete and re-generate a usd_cache node
   if (m_cacheId != 0 && plugName ==  "outStageCacheId")
   {
      ClearListener();
      SetUpdateMode(AI_RECREATE_NODE);
   }
   if (plugName == "shareUsdStage") {
      ClearListener();
      SetUpdateMode(AI_RECREATE_NODE);  
   }
   if (plugName == "overrides") {
      SetUpdateMode(AI_RECREATE_NODE);
   }



   // we're calling directly the shape translator function, as we don't want to make it a AI_RECREATE_NODE
   CShapeTranslator::NodeChanged(node, plug);  
}

