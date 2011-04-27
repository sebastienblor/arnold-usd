#ifndef LIGHTS_H
#define LIGHTS_H

#include "NodeTranslator.h"

class DLLEXPORT CLightTranslator
   :   public CDagTranslator
{
public:

   void Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      CDagTranslator::Init(dagPath, scene, outputAttr);
      m_motion = scene->IsLightMotionBlurEnabled();
      m_dagPath = dagPath;
      m_fnNode.setObject(dagPath);
      m_scene = scene;
      m_outputAttr = outputAttr;
   }
   bool RequiresMotionData()
   {
      return m_motion;
   }
   static void NodeInitializer(MString nodeClassName);
protected:
   void Export(AtNode* light, bool mayaAttrs=true);
   void ExportMotion(AtNode* light, AtUInt step);
   void ExportLightFilters(AtNode* light, const MObjectArray &filters);
   virtual void Delete();

protected:
   bool m_motion;
};

class DLLEXPORT CAmbientLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void* creator()
   {
      return new CAmbientLightTranslator();
   }
   bool RequiresMotionData()
   {
      return false;
   }
   const char* GetArnoldNodeType()
   {
      return "ambient_light";
   }
};

class DLLEXPORT CDirectionalLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void* creator()
   {
      return new CDirectionalLightTranslator();
   }
   const char* GetArnoldNodeType()
   {
      return "distant_light";
   }
};

class DLLEXPORT CPointLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new CPointLightTranslator();
   }
   const char* GetArnoldNodeType()
   {
      return "point_light";
   }
};

class DLLEXPORT CSpotLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new CSpotLightTranslator();
   }
   const char* GetArnoldNodeType()
   {
      return "spot_light";
   }
};

class DLLEXPORT CAreaLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new CAreaLightTranslator();
   }
   const char* GetArnoldNodeType()
   {
      return "quad_light";
   }
};

class DLLEXPORT CSkyDomeLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void* creator()
   {
      return new CSkyDomeLightTranslator();
   }
   const char* GetArnoldNodeType()
   {
      return "skydome_light";
   }
};
#endif // LIGHTS_H
