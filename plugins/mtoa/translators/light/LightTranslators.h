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
      m_flushCache = false; // initialize to false
      m_colorTexture = false;
      return AddArnoldNode("quad_light");
   }
protected:
   void NodeChanged(MObject& node, MPlug& plug);

private:
   bool m_colorTexture;
   bool m_flushCache;
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
      m_flushCache = false; // initialize to false
      return AddArnoldNode("skydome_light");

   }

   bool IsFinite() const { return false; }
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
private:
   bool m_flushCache; 
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
      AddArnoldNode("two_sided", "two_sided");
      AddArnoldNode("ray_switch_shader", "shader");
      AddArnoldNode("multiply", "multShader");
      return AddArnoldNode("mesh_light");
   }

   virtual void ExportMotion(AtNode* light);
protected:

   virtual void NodeChanged(MObject& node, MPlug& plug);
   
   MPlug GetPlug(const MObject& obj, const MString &attrName, MStatus* ReturnStatus = NULL) const
   {
      MFnDependencyNode fnNode(obj);
      return fnNode.findPlug(attrName, true, ReturnStatus);
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
   static void NodeInitializer(CAbTranslator context);
   
   /// Customize the callbacks invoked during IPR updates for DAG nodes
   virtual void AddUpdateCallbacks();

   static void* creator()
   {
      return new CMeshLightNewTranslator();
   }

};



