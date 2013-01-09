#include "AutoCameraTranslator.h"

AtNode* CAutoCameraTranslator::CreateArnoldNodes()
{
   return AddArnoldNode(m_abstract.arnold.asChar());
}

void CAutoCameraTranslator::Export(AtNode* camera)
{
   CNodeTranslator::Export(camera);
   ExportCameraData(camera);
}

void CAutoCameraTranslator::ExportMotion(AtNode* camera, unsigned int step)
{
   ExportCameraMBData(camera, step);
}
