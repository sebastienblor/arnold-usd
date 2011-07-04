#ifndef MAYASCENE_H
#define MAYASCENE_H

#include "common/MObjectCompare.h"
#include "platform/Platform.h"
#include "render/RenderSession.h"
#include "translate/ExportSession.h"

#include <ai_nodes.h>

#include <maya/MStatus.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MMatrix.h>
#include <maya/MObjectArray.h>
#include <maya/MSelectionList.h>
#include <maya/MFnCamera.h>
#include <maya/MVectorArray.h>
#include <maya/MMessage.h>
#include <maya/MPlug.h>
#include <maya/MCommonRenderSettingsData.h>


#include <vector>
#include <set>
#include <map>
#include <string>



/// Translates the current state of all or part of an open Maya scene into the active Arnold universe.

/// In IPR mode, the resulting instance allows the scene to be quickly and incrementally retranslated
/// as changes occur to previously translated Maya objects.
///
/// Once CMayaScene::ExportToArnold() is called, the DAG hierarchy is traversed and CDagTranslators
/// are found and exported for all relevant Maya nodes.  Those translators in turn call
/// and CMayaScene::ExportShader() as they require, which triggers dependency graph evaluation and the
/// generation of CNodeTranslators.

class CMayaScene
{

public:

   CMayaScene();

   ~CMayaScene();

   // Currently there can be only one export and render session
   // but when it changes, CMayaScene will manage them

   /// Return the instance of the export session.
   static CExportSession* GetExportSession();

   /// Return the instance of the render session.
   static CRenderSession* GetRenderSession();

   static bool IsExportingMotion();

   MStatus ExportToArnold();
   static void SetupImageOptions(AtNode* options);
   void PrepareExport();

private:

   void ExportInstancerReplacement(const MDagPath& dagPath, AtUInt step);

   static DagFiltered FilteredStatus(CExportFilter filter, MDagPath dagPath);
   bool IsExportedPath(CExportFilter filter, MDagPath path);
   static bool IsInRenderLayer(MDagPath dagPath);
   static bool IsVisiblePath(MDagPath dagPath);
   static bool IsTemplatedPath(MDagPath dagPath);
   static bool IsVisible(MFnDagNode node);
   static bool IsTemplated(MFnDagNode node);

   static void IPRNewNodeCallback(MObject & node, void *);
   static void IPRIdleCallback(void *);
   static void UpdateIPR();

   static std::vector< CNodeTranslator * > s_translatorsToIPRUpdate;
   static MCallbackId s_IPRIdleCallbackId;
   static MCallbackId s_NewNodeCallbackId;
   
   // Currently there can be only one export and render session
   // but when it changes, CMayaScene will manage them
   static CRenderSession* s_renderSession;
   static CExportSession* s_exportSession;

   static double s_currentFrame;

};  // class CMayaScene

#endif // MAYASCENE_H
