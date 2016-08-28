#pragma once

#include "translators/shape/ShapeTranslator.h"

class CBfDescriptionTranslator : public CShapeTranslator
{
public:

   enum RenderType {
      CBIFROST_AERO,
      CBIFROST_LIQUID,
      CBIFROST_ISOSURFACE_PARTICLES,
      CBIFROST_FOAM
   };


   AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* shape);
   void ExportMotion(AtNode*);

   void UpdateFoam(AtNode *shape);
   void UpdateAero(AtNode *shape);
   void UpdateLiquid(AtNode *shape);

   static void* creator()
   {
      return new CBfDescriptionTranslator();
   }
   static void NodeInitializer(CAbTranslator context);


private:

   virtual void ExportBifrostShader();
   
   RenderType  m_render_type;
   std::string m_object;
   std::string m_file;

};
