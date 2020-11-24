#include "ImagerTranslator.h"
#include "utils/Universe.h"
#include "translators/NodeTranslatorImpl.h"
#include "translators/DagTranslator.h"
#include <maya/MPlugArray.h>
#include <maya/MTimerMessage.h>
#include <maya/MEventMessage.h>
#include "render/RenderSession.h"
#include "scene/MayaScene.h"

AtNode* CImagerTranslator::CreateArnoldNodes()
{
   m_idleCb = 0;
   return AddArnoldNode(m_impl->m_abstract.arnold.asChar());   
}

void CImagerTranslator::Export(AtNode *shader)
{
   MStatus status;
   const AtNodeEntry* entry = AiNodeGetNodeEntry(shader);
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(entry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      std::string paramNameStr(paramName);

      if (paramNameStr != "name" && paramNameStr != "input" ) //&& paramNameStr != "layer_selection")
         ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
   }
   AiParamIteratorDestroy(nodeParam);
}
void CImagerTranslator::NodeInitializer(CAbTranslator context)
{   
}

static void NodeChangedIdleCallback(void *data)
{
   CImagerTranslator * translator = static_cast< CImagerTranslator* >(data);
   if (translator)
      translator->ProcessImagerChanges();
}
void CImagerTranslator::ProcessImagerChanges()
{
   if(m_idleCb)
   {
      MMessage::removeCallback(m_idleCb);
      m_idleCb = 0;   
   }
   static AtString request_imager_update("request_imager_update");
   Export(GetArnoldNode());

   CRenderSession *renderSession = CMayaScene::GetRenderSession();
   if (renderSession)
      renderSession->SetRenderViewOption("Update Imagers", "1");

   AiRenderSetHintBool(request_imager_update, true);
}

void CImagerTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MStatus status;
   if (m_idleCb)
      return;
   m_idleCb = MEventMessage::addEventCallback("idle",
                                             NodeChangedIdleCallback,
                                             this,
                                             &status);
   if (status != MStatus::kSuccess)
      m_idleCb = 0;
}
