#pragma once

#include "LightTranslator.h"

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

class CPhotometricLightTranslator : public CLightTranslator
{
public:
   void Export(AtNode* light);
   static void NodeInitializer(CAbTranslator context);
   static void* creator()
   {
      return new CPhotometricLightTranslator();
   }
   AtNode* CreateArnoldNodes()
   {
      return AddArnoldNode("photometric_light");
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
      AddArnoldNode("polymesh", "mesh");
      AddArnoldNode("meshLightMaterial", "shader");
      return AddArnoldNode("mesh_light");
   }
   
   virtual void ExportMotion(AtNode* light, unsigned int step);
protected:
   virtual AtNode* ExportSimpleMesh(const MObject& meshObject);
   virtual MObject GetMeshObject() const;

   int m_numVertices;
};
