#include "LightBlockerTranslator.h"

AtNode* CLightBlockerTranslator::Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr)
{
   CDagTranslator::Init(session, dagPath, outputAttr);
   return m_atNode;
}
   
bool CLightBlockerTranslator::RequiresMotionData()
{
   return false;
}
   
void* CLightBlockerTranslator::creator()
{
   return new CLightBlockerTranslator();
}
   
AtNode* CLightBlockerTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("light_blocker");
}

void CLightBlockerTranslator::Export(AtNode* blocker)
{
   AtMatrix matrix;
   GetMatrix(matrix);
   
   AiNodeSetMatrix(blocker, "geometry_matrix", matrix);
   
   ProcessParameter(blocker, "geometry_type", AI_TYPE_INT, "geometryType");
   ProcessParameter(blocker, "density", AI_TYPE_FLOAT, "density");
   ProcessParameter(blocker, "shader", AI_TYPE_RGB, "shader");
   ProcessParameter(blocker, "roundness", AI_TYPE_FLOAT, "roundness");
   ProcessParameter(blocker, "width_edge", AI_TYPE_FLOAT, "widthEdge");
   ProcessParameter(blocker, "height_edge", AI_TYPE_FLOAT, "heightEdge");
   ProcessParameter(blocker, "ramp", AI_TYPE_FLOAT, "ramp");
   ProcessParameter(blocker, "ramp_axis", AI_TYPE_INT, "rampAxis");

   if ((ComputeVisibility() & AI_RAY_CAMERA) == 0) AiNodeSetFlt(blocker, "density", 0.0f);
}
