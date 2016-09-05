#pragma once
#include "CameraTranslator.h"

class CStandardCameraTranslator : public CCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CStandardCameraTranslator();
   }
   AtNode* CreateArnoldNodes();
   
protected:
   bool IsOrtho();
   
   void ExportOrtho(AtNode* camera);
   void ExportPersp(AtNode* camera);
   void ExportMotionOrtho(AtNode* camera, unsigned int step);
   void ExportMotionPersp(AtNode* camera, unsigned int step);
   void ExportFilmbackOrtho(AtNode* camera);
   float ExportFilmbackPersp(AtNode* camera);
};      

class CFishEyeCameraTranslator
      :   public CCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera);
   static void NodeInitializer(CAbTranslator context);
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
   void ExportMotion(AtNode* camera);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CCylCameraTranslator();
   }
   AtNode* CreateArnoldNodes();

protected:
   // return FOV
   void ExportFilmback(AtNode* camera, float fovs[]);
};

class CSphericalCameraTranslator
      :   public CCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CSphericalCameraTranslator();
   }
   AtNode* CreateArnoldNodes();

protected:
   // return FOV
   void ExportFilmback(AtNode* camera);
};
