#ifndef AUTO_DAG_TRANSLATOR_H
#define AUTO_DAG_TRANSLATOR_H

#include "translators/NodeTranslator.h"

/// A translator for auto-exporting DAG nodes.
/// This is the default translator for cameras and shapes.  It's like ShaderTranslator
/// but for DAG nodes.
class DLLEXPORT CAutoDagTranslator
   :  public CDagTranslator
{
public:
   static void* creator()
   {
      return new CAutoDagTranslator();
   }
   virtual AtNode* CreateArnoldNodes();
   virtual bool RequiresMotionData();
};

#endif // SHADER_TRANSLATOR_H
