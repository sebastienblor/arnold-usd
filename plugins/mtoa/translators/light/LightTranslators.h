#ifndef LIGHTTRANSLATORS_H
#define LIGHTTRANSLATORS_H

#include "ArnoldLightTranslator.h"

class CAmbientLightTranslator : public CArnoldLightTranslator
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

class CDirectionalLightTranslator : public CArnoldLightTranslator
{
public:
   void Export(AtNode* light);
   static void NodeInitializer(MString nodeClassName);
   static void* creator()
   {
      return new CDirectionalLightTranslator();
   }
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("distant_light");
   }
};

class CPointLightTranslator : public CArnoldLightTranslator
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

class CSpotLightTranslator : public CArnoldLightTranslator
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

class CAreaLightTranslator : public CArnoldLightTranslator
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

class CSkyDomeLightTranslator : public CArnoldLightTranslator
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
#endif // LIGHTTRANSLATORS_H
