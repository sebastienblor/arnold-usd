#ifndef CAMERA_TRANSLATORS_H
#define CAMERA_TRANSLATORS_H

#include "CameraTranslator.h"


class CPerspCameraTranslator
      :   public CCameraTranslator
{
public:
   virtual void Export(AtNode* camera);
   virtual void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MString nodeClassName, CNodeInitContext context);
   static void* creator()
   {
      return new CPerspCameraTranslator();
   }
   AtNode* CreateArnoldNodes();

protected:
   // return FOV
   float ExportFilmback(AtNode* camera);
};


class COrthoCameraTranslator
      :   public CCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MString nodeClassName, CNodeInitContext context);
   static void* creator()
   {
      return new COrthoCameraTranslator();
   }
   AtNode* CreateArnoldNodes();

protected:
   void ExportFilmback(AtNode* camera);
};

class CFishEyeCameraTranslator
      :   public CCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MString nodeClassName, CNodeInitContext context);
   static void* creator()
   {
      return new CFishEyeCameraTranslator();
   }
   AtNode* CreateArnoldNodes();

protected:
   // return FOV
   float ExportFilmback(AtNode* camera);
};

class CCylCameraTranslator
      :   public CCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MString nodeClassName, CNodeInitContext context);
   static void* creator()
   {
      return new CCylCameraTranslator();
   }
   AtNode* CreateArnoldNodes();

protected:
   // return FOV
   void ExportFilmback(AtNode* camera, float fovs[]);
};
#endif // CAMERA_TRANSLATORS_H
