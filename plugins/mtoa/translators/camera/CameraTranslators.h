#ifndef CAMERATRANSLATORS_H
#define CAMERATRANSLATORS_H

#include "ArnoldCameraTranslator.h"


class CPerspCameraTranslator
      :   public CArnoldCameraTranslator
{
public:
   virtual void Export(AtNode* camera);
   virtual void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MString nodeClassName);
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
      :   public CArnoldCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera, AtUInt step);
    static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new COrthoCameraTranslator();
   }
   AtNode* CreateArnoldNodes();

protected:
   void ExportFilmback(AtNode* camera);
};

class CFishEyeCameraTranslator
      :   public CArnoldCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MString nodeClassName);
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
      :   public CArnoldCameraTranslator
{
public:
   void Export(AtNode* camera);
   void ExportMotion(AtNode* camera, AtUInt step);
   static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new CCylCameraTranslator();
   }
   AtNode* CreateArnoldNodes();

protected:
   // return FOV
   void ExportFilmback(AtNode* camera, float fovs[]);
};
#endif // CAMERATRANSLATORS_H
