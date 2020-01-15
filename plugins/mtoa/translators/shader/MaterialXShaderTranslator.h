
#include "ShaderTranslator.h"
#include "translators/DagTranslator.h"

class CArnoldMaterialXShaderTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CArnoldMaterialXShaderTranslator();}
   virtual void Export(AtNode* shader) ;
   AtNode* CreateArnoldNodes();
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
};