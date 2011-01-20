#ifndef CAMERAS_H
#define CAMERAS_H

#include "scene/NodeTranslator.h"

struct CCameraData
{
   float fov;
   double apertureX;
   double apertureY;
   double lensSqueeze;
   double scale;
   double focalLength;
   double factorX;
   double factorY;
   double width;
   double deviceAspect;
   CCameraData() : fov(0),
                   apertureX(0),
                   apertureY(0),
                   lensSqueeze(0),
                   scale(0),
                   focalLength(0),
                   factorX(0),
                   factorY(0),
                   width(0),
                   deviceAspect(0)
   {};
};

class CCameraTranslator
   :   public CDagTranslator
{
public:
   void Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      CDagTranslator::Init(dagPath, scene, outputAttr);
      m_motion = m_scene->IsCameraMotionBlurEnabled();
      m_fnCamera.setObject(dagPath);
      m_dagPath = dagPath;
      m_fnNode.setObject(dagPath);
      m_scene = scene;
      m_outputAttr = outputAttr;
   }
   bool RequiresMotionData()
   {
      return m_motion;
   }
   AtNode* Export();
   void Update(AtNode* camera);
   void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MObject& node);
   static void* creator()
   {
      return new CCameraTranslator();
   }

protected:
   double GetDeviceAspect();
   void ExportOrthoFilmback(AtNode* camera);
   void ExportPerspFilmback(AtNode* camera);
   MVectorArray GetFilmTransform(double width=0, bool persp=true);
   void ExportImagePlane(AtUInt step);

   void ExportOrtho(AtNode* camera);
   void ExportOrthoMotion(AtNode* camera, AtInt step);
   void ExportPersp(AtNode* camera);
   void ExportPerspMotion(AtNode* camera, AtInt step);

   void ExportCameraData(AtNode* camera);
   void ExportCameraMBData(AtNode* camera, AtUInt step);


protected:
   bool m_motion;
   CCameraData m_cameraData;
   MFnCamera m_fnCamera;
};

#endif // CAMERAS_H
