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

   bool IsFinite() const { return false; }
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

   bool IsFinite() const { return false; }
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
   CMeshLightTranslator() : m_numVertices(0)
   {
   }

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

   virtual void Delete();
   virtual void ExportMotion(AtNode* light, unsigned int step);

protected:
   MPlug GetPlug(const MObject& obj, const MString &attrName, MStatus* ReturnStatus = NULL) const
   {
      MFnDependencyNode fnNode(obj);
      return fnNode.findPlug(attrName, ReturnStatus);
   }

   virtual AtNode* ExportSimpleMesh(const MObject& meshObject);
   virtual MObject GetMeshObject() const;

   int m_numVertices;
};

// Translator for new mesh light representation where
// the light is a seperate node, with a connection to the
// mesh representing the light shape
class CMeshLightNewTranslator : public CMeshLightTranslator
{
public:
   CMeshLightNewTranslator() : CMeshLightTranslator() {}
   virtual MObject GetMeshObject() const;

   static void* creator()
   {
      return new CMeshLightNewTranslator();
   }

protected:
   virtual void AddUpdateCallbacks();
   static void MeshDirtyCallback(MObject& node, MPlug& plug, void* clientData);
};
