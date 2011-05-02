#ifndef CAMERAS_H
#define CAMERAS_H

#include "scene/NodeTranslator.h"

class DLLEXPORT CCameraTranslator
   :   public CDagTranslator
{
public:
   virtual AtNode* Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      m_atNode = CDagTranslator::Init(dagPath, scene, outputAttr);
      m_motion = scene->IsCameraMotionBlurEnabled();
      m_fnCamera.setObject(dagPath);
      return m_atNode;
   }
   
   // FIXME: this method shouldn't be required.
   virtual bool RequiresMotionData()
   {
      return m_motion;
   }

protected:
   double GetDeviceAspect();
   void SetFilmTransform(AtNode* camera, double factorX=0, double factorY=0, double width=0, bool persp=true);
   void ExportImagePlane(AtUInt step);
   void ExportDOF(AtNode* camera);
   void ExportCameraData(AtNode* camera);
   void ExportCameraMBData(AtNode* camera, AtUInt step);
   static void MakeDefaultAttributes(CExtensionAttrHelper &helper);
   static void MakeDOFAttributes(CExtensionAttrHelper &helper);

protected:
   bool m_motion;
   MFnCamera m_fnCamera;
};


class DLLEXPORT CPerspCameraTranslator
      :   public CCameraTranslator
{
public:
   virtual void Export(AtNode* camera);
   virtual void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new CPerspCameraTranslator();
   }
   const char* GetArnoldNodeType();

protected:
   // return FOV
   float ExportFilmback(AtNode* camera);
};


class DLLEXPORT COrthoCameraTranslator
      :   public CCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera, AtUInt step);
    static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new COrthoCameraTranslator();
   }
   const char* GetArnoldNodeType();

protected:
   void ExportFilmback(AtNode* camera);
};

class DLLEXPORT CFishEyeCameraTranslator
      :   public CCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new CFishEyeCameraTranslator();
   }
   const char* GetArnoldNodeType();

protected:
   // return FOV
   float ExportFilmback(AtNode* camera);
};

class DLLEXPORT CCylCameraTranslator
      :   public CCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new CCylCameraTranslator();
   }
   const char* GetArnoldNodeType();

protected:
   // return FOV
   void ExportFilmback(AtNode* camera, float fovs[]);
};
#endif // CAMERAS_H
