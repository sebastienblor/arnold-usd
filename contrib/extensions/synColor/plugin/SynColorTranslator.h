#include "translators/NodeTranslator.h"

class CSynColorTranslator
   :   public CNodeTranslator
{
public:

   virtual void Export(AtNode* camera);

   static void NodeInitializer(CAbTranslator context);

   AtNode* CreateArnoldNodes();

   static void* creator()
   {
      return new CSynColorTranslator();
   }
};
