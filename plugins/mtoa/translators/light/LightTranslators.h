#ifndef LIGHT_TRANSLATORS_H
#define LIGHT_TRANSLATORS_H

#include "LightTranslator.h"

class CAmbientLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CAmbientLightTranslator();
   }
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("ambient_light");
   }
};

class CDirectionalLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CDirectionalLightTranslator();
   }
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("distant_light");
   }
};

class CPointLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CPointLightTranslator();
   }
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("point_light");
   }
};

class CSpotLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CSpotLightTranslator();
   }
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("spot_light");
   }
};

class CQuadLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CQuadLightTranslator();
   }
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("quad_light");
   }
};

class CCylinderLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CCylinderLightTranslator();
   }
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("cylinder_light");
   }
};

class CDiskLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CDiskLightTranslator();
   }
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("disk_light");
   }
};

class CSkyDomeLightTranslator : public CLightTranslator
{
public:
   void GetMatrix(AtMatrix& matrix);
   void Export(AtNode* light);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CSkyDomeLightTranslator();
   }
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("skydome_light");
   }
};

class CMeshLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CMeshLightTranslator();
   }
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("mesh_light");
   }
};

#endif // LIGHT_TRANSLATORS_H
