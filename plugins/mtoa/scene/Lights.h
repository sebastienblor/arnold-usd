#ifndef LIGHTS_H
#define LIGHTS_H

#include "NodeTranslator.h"

class CLightTranslator
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
   void Update(AtNode* light, bool mayaAttrs=true);
   void ExportMotion(AtNode* light, AtUInt step);
   void ExportLightFilters(AtNode* light, const MObjectArray &filters);
   virtual void Delete();

protected:
   bool m_motion;
};

class CAmbientLightTranslator : public CLightTranslator
{
public:
   AtNode* Export();
   void Update(AtNode* light);
   static void* creator()
   {
      return new CAmbientLightTranslator();
   }
   bool RequiresMotionData()
   {
      return false;
   }
};

class CDirectionalLightTranslator : public CLightTranslator
{
public:
   AtNode* Export();
   void Update(AtNode* light);
   static void* creator()
   {
      return new CDirectionalLightTranslator();
   }
};

class CPointLightTranslator : public CLightTranslator
{
public:
   AtNode* Export();
   void Update(AtNode* light);
   static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new CPointLightTranslator();
   }
};

class CSpotLightTranslator : public CLightTranslator
{
public:
   AtNode* Export();
   void Update(AtNode* light);
   static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new CSpotLightTranslator();
   }
};

class CAreaLightTranslator : public CLightTranslator
{
public:
   AtNode* Export();
   void Update(AtNode* light);
   static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new CAreaLightTranslator();
   }
};

class CSkyDomeLightTranslator : public CLightTranslator
{
public:
   AtNode* Export();
   void Update(AtNode* light);
   static void* creator()
   {
      return new CSkyDomeLightTranslator();
   }
};
#endif // LIGHTS_H
