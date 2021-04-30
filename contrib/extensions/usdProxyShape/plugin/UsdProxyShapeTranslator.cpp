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

#ifdef ENABLE_USD
#include <pxr/usd/ar/resolver.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/stageCache.h>
#include <pxr/usd/usd/stageCacheContext.h>
#include <pxr/usd/usdGeom/camera.h>
#include <pxr/usd/usdSkel/bakeSkinning.h>
#include "pxr/usd/sdf/api.h"
#include "pxr/usd/sdf/data.h"
#include "pxr/usd/sdf/declareHandles.h"
#include "pxr/usd/sdf/identity.h"
#include "pxr/usd/sdf/layerOffset.h"
#include "pxr/usd/sdf/namespaceEdit.h"
#include "pxr/usd/sdf/path.h"
#include "pxr/usd/sdf/proxyTypes.h"
#include "pxr/usd/sdf/spec.h"
#include "pxr/usd/sdf/types.h"

#include "UsdProxyShapeListener.h"
#include <pxr/usd/usdUtils/stageCache.h>

PXR_NAMESPACE_USING_DIRECTIVE

class CUsdProxyShapeTranslatorImpl
{
public:
   CUsdProxyShapeTranslatorImpl(CUsdProxyShapeTranslator &tr) : m_translator(tr), m_stage(nullptr) {}
   void InitStage(UsdStageRefPtr stage)
   {
      m_stage = stage;
      m_listener.SetStage(stage);
      if (stage == nullptr)
         return;
      
      m_listener.SetStageContentsChangedCallback(
         [this](const UsdNotice::StageContentsChanged& notice) {
                return OnStageContentsChanged(notice);
            });
   }
private:
   CUsdProxyShapeTranslator &m_translator;
   UsdStageRefPtr m_stage;
   CUsdMtoAListener m_listener;

   /*
   void OnStageObjectsChanged(const UsdNotice::ObjectsChanged& notice)
   {
      m_translator.StageChanged();
      UsdNotice::ObjectsChanged::PathRange range = notice.GetResyncedPaths();
      for (UsdNotice::ObjectsChanged::PathRange::iterator it = range.begin(); it != range.end(); ++it)
         std::cerr<<it->GetAsString()<<std::endl;
   }*/

   void OnStageContentsChanged(const UsdNotice::StageContentsChanged& notice)
   {
      m_translator.StageChanged();      
   }
};

CUsdProxyShapeTranslator::CUsdProxyShapeTranslator() : CProceduralTranslator() 
{
   m_impl = new CUsdProxyShapeTranslatorImpl(*this);
}
CUsdProxyShapeTranslator::~CUsdProxyShapeTranslator()
{
   if (m_impl)
      delete m_impl;
}
#else
CUsdProxyShapeTranslator::CUsdProxyShapeTranslator() : CProceduralTranslator() 
{
   m_impl = nullptr;
}
CUsdProxyShapeTranslator::~CUsdProxyShapeTranslator()
{
}
#endif
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

}
AtNode* CUsdProxyShapeTranslator::CreateArnoldNodes()
{
   // If mode is export to ass, just export to usd
#ifdef ENABLE_USD
   // When exporting to .ass, we always want to export to the builtin core procedural.
   // This way, it can be ported to other DCCs, or be kicked in standalone
   if (GetSessionMode() == MTOA_SESSION_ASS)
      return AddArnoldNode("usd");   

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
         MString usdFolder = (envVarStr == MString("2")) ? MString("/usd_python2") : MString("/usd");

         /* When we'll need to get the usd version, we can call the 
            following python code
            
            from pxr import Usd
            return Usd.GetVersion()
         */
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

#ifdef ENABLE_USD
   if (AiNodeIs(shape, AtString("usd_cache")))
   {
      MPlug cacheIdPlug = FindMayaPlug("outStageCacheId");
      if (!cacheIdPlug.isNull())
      {
         int cacheId = cacheIdPlug.asInt();
         std::string cacheStr;
         AiNodeSetInt(shape, "cache_id", cacheId);
         UsdStageCache &stageCache = UsdUtilsStageCache::Get();
         UsdStageCache::Id id = UsdStageCache::Id::FromLongInt(cacheId);
         m_impl->InitStage((id.IsValid()) ? stageCache.Find(id) : nullptr);

      }
   }
#endif

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

void CUsdProxyShapeTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   m_attrChanged = true; // this flag tells me that I've been through a NodeChanged call
   MString plugName = plug.partialName(false, false, false, false, false, true);

   if (plugName == "selectedItems" || plugName == "selected_items" || 
      plugName == "MinBoundingBox0" || plugName == "MinBoundingBox1" || plugName == "MinBoundingBox2" || 
      plugName == "MaxBoundingBox0" || plugName == "MaxBoundingBox1" || plugName == "MaxBoundingBox2") return;

   // we're calling directly the shape translator function, as we don't want to make it a AI_RECREATE_NODE
   CShapeTranslator::NodeChanged(node, plug);  
}

void CUsdProxyShapeTranslator::StageChanged()
{
   CProceduralTranslator::RequestUpdate();
}
