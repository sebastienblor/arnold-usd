
#include "ShaderTranslator.h"
#include "translators/DagTranslator.h"

class CArnoldAxfShaderTranslator : public CShaderTranslator{
public:
   static void* creator(){return new CArnoldAxfShaderTranslator();}
   virtual void Export(AtNode* shader) {};
   AtNode* CreateArnoldNodes();
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug);
};