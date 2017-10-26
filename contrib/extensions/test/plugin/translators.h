#include "translators/shape/ShapeTranslator.h"
#include "translators/shader/ShaderTranslator.h"

// this is an example translator that overrides the translation of maya's lambert
// node to export a red arnold standard shader

class MtoaTestProcTranslator
   :  public CShapeTranslator
{
public:
   AtNode* CreateArnoldNodes();
   virtual void Export(AtNode*);
   virtual void ExportShaders();
   static void* creator(){
      return new MtoaTestProcTranslator();
   }
};

class MtoaTestShaderTranslator
   :  public CShaderTranslator
{
public:
   AtNode* CreateArnoldNodes();
   virtual void Export(AtNode*);
   static void* creator(){
      return new MtoaTestShaderTranslator();
   }
};

