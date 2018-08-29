#include "translators/NodeTranslator.h"

class CSynColorTranslator
   :   public CNodeTranslator
{
public:
	
   virtual void Export(AtNode* camera);

   static void NodeInitializer(CAbTranslator context);

   AtNode* CreateArnoldNodes();
   virtual void NodeChanged(MObject& node, MPlug& plug);

   static void* creator()
   {
      return new CSynColorTranslator();
   }
private:
	bool m_renderingSpaceChanged;
};
