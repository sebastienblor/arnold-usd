
#include "MayaScene.h"

#include <ai_cameras.h>
#include <ai_constants.h>

#include <maya/MFnCamera.h>

void CMayaScene::ExportCamera(const MDagPath& dagPath)
{

   MPoint     point;
   MVector    vector;
   MFnCamera  fnCamera(dagPath);
   MFnDagNode fnDagNode(dagPath);

   AtNode* camera = AiNode("persp_camera");

   AiNodeSetStr(camera, "name", fnDagNode.name().asChar());

   point = fnCamera.eyePoint(MSpace::kWorld);
   AiNodeSetPnt(camera, "position", point.x, point.y, point.z);

   vector = fnCamera.viewDirection(MSpace::kWorld);
   point  = point + vector;
   AiNodeSetPnt(camera, "look_at", point.x, point.y, point.z);

   vector = fnCamera.upDirection(MSpace::kWorld);
   AiNodeSetVec(camera, "up", vector.x, vector.y, vector.z);

   AiNodeSetFlt(camera, "fov", static_cast<float>(AI_RTOD * fnCamera.horizontalFieldOfView()));
   AiNodeSetStr(camera, "handedness", "right");

}  // ExportCamera()
