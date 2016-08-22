#pragma once
#include "NodeTranslator.h"
#include "common/MObjectCompare.h"
#include "platform/Platform.h"
#include "attributes/AttrHelper.h"
#include "session/ArnoldSession.h"
#include "extension/AbTranslator.h"
#include "render/AOV.h"

#include <ai_nodes.h>

#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include <maya/MMessage.h> // for MCallbackId
#include <maya/MCallbackIdArray.h>
#include <maya/MNodeMessage.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>

class CNodeTranslator;


class CNodeTranslatorImpl
{
public :
   CNodeTranslatorImpl(CNodeTranslator &translator) : 
      m_handle(CNodeAttrHandle()),
      m_updateMode(CNodeTranslator::AI_UPDATE_ONLY),
      m_holdUpdates(false),
      m_abstract(CAbTranslator()),
      m_session(NULL),
      m_atNode(NULL),
      m_overrideSets(),
      m_step(0),
      m_localAOVs(),
      m_upstreamAOVs(),
      m_shaders(NULL),
      m_isExported(false),
      m_tr(translator){}
   ~CNodeTranslatorImpl() {}

   AtNode* DoExport();
   AtNode* DoUpdate();
   void DoCreateArnoldNodes();
   AtNode* ProcessParameterInputs(AtNode* arnoldNode, const MPlug &plug,
                                                const char* arnoldParamName,
                                                int arnoldParamType);

   bool ProcessParameterComponentInputs(AtNode* arnoldNode, const MPlug &parentPlug,
                                                      const char* arnoldParamName,
                                                      int arnoldParamType);



   MPlug FindMayaOverridePlug(const MString &attrName, MStatus* ReturnStatus=NULL) const;
   MPlug FindMayaObjectPlug(const MString &attrName, MStatus* ReturnStatus=NULL) const;

   // Remove callbacks installed. 
   void RemoveUpdateCallbacks();
   void Init(CArnoldSession* session, const MObject& nodeObject, const MString& attrName="")
   {
      Init(session, CNodeAttrHandle(nodeObject, attrName));
   }
   void Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="")
   {
      Init(session, CNodeAttrHandle(dagPath, outputAttr));
   }
   void Init(CArnoldSession* session, const CNodeAttrHandle& object);


   // export the shading engine AOVS. This is mainly called when exporting MayaShadingEngine.
   // But the Shape translators that also create their own shader (when it doesn't exist in Maya)
   // call ExportRootShader, which call CreateShadingGroupShader, which call AddAOVDefaults
   void AddAOVDefaults(AtNode* shadingEngine, std::vector<AtNode*> &aovShaders);
   void TrackAOVs(AOVSet* aovs);
   
   MStatus ExportOverrideSets();
   MPlug GetOverridePlug(const MPlug &plug, MStatus* ReturnStatus=NULL) const;

   void WriteAOVUserAttributes(AtNode* atNode);
   virtual void SetShadersList(AtNodeSet *nodes);
   

   CNodeAttrHandle m_handle;
   CNodeTranslator::UpdateMode m_updateMode;
   bool m_holdUpdates; // for Arnold RenderView only
   CAbTranslator m_abstract;

   CArnoldSession* m_session;

   AtNode* m_atNode;
   std::map<std::string, AtNode*> m_atNodes;

   std::vector<CNodeTranslator*> m_overrideSets;

   unsigned int m_step;

   AOVSet m_localAOVs;
   AOVSet m_upstreamAOVs;
   AtNodeSet* m_shaders;

   // This stores callback IDs for the callbacks this
   // translator creates.
   MCallbackIdArray m_mayaCallbackIDs;
   bool m_isExported;



private:
   CNodeTranslator &m_tr;
};
