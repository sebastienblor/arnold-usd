#ifndef LIGHTS_H
#define LIGHTS_H

#include "NodeTranslator.h"

class DLLEXPORT CLightTranslator
   :   public CDagTranslator
{
public:

   virtual AtNode* Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="")
   {
      CDagTranslator::Init(dagPath, scene, outputAttr);
      m_motion = scene->IsLightMotionBlurEnabled();
      return m_atNode;
   }
   bool RequiresMotionData()
   {
      return m_motion;
   }
   static void NodeInitializer(MString nodeClassName);
protected:
   virtual void Export(AtNode* light, bool mayaAttrs=true);
   virtual void ExportMotion(AtNode* light, AtUInt step);
   virtual void ExportLightFilters(AtNode* light, const MObjectArray &filters);
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
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("ambient_light");
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
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("distant_light");
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
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("point_light");
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
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("spot_light");
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
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("quad_light");
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
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("skydome_light");
   }
};
#endif // LIGHTS_H
