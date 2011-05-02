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

class DLLEXPORT CCameraTranslator
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

protected:
   double GetDeviceAspect();
   MVectorArray GetFilmTransform(double width=0, bool persp=true);
   void ExportImagePlane(AtUInt step);
   void ExportDOF(AtNode* camera);
   void ExportCameraData(AtNode* camera);
   void ExportCameraMBData(AtNode* camera, AtUInt step);
   static void MakeDOFAttributes(CExtensionAttrHelper &helper);

protected:
   bool m_motion;
   CCameraData m_cameraData;
   MFnCamera m_fnCamera;
};


class DLLEXPORT CPerspCameraTranslator
      :   public CCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new CPerspCameraTranslator();
   }
   const char* GetArnoldNodeType();

protected:
   void ExportFilmback(AtNode* camera);
};


class DLLEXPORT COrthoCameraTranslator
      :   public CCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera, AtUInt step);
   static void* creator()
   {
      return new COrthoCameraTranslator();
   }
   const char* GetArnoldNodeType();

protected:
   void ExportFilmback(AtNode* camera);
};
#endif // CAMERAS_H
