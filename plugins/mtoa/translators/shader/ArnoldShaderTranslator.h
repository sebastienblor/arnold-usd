#ifndef ARNOLDSHADERTRANSLATOR_H
#define ARNOLDSHADERTRANSLATOR_H

#include "translators/NodeTranslator.h"



//--------------- CArnoldShaderTranslator ------------------------------------------

/// A Translator class which can automatically export simple Maya nodes.

/// To perform an automatic export, the translator does the following:
///  -# gets the Arnold node entry that corresponds to the Maya node being export from m_arnoldNodeName
///  -# loops through each parameter on the Arnold node entry
///  -# processes the equivalent attribute on the Maya node
///

class DLLEXPORT CArnoldShaderTranslator
   :  public CNodeTranslator
{
public:
   AtNode* Init(MDagPath& dagPath, CMayaScene* scene, MString outputAttr="");
   static void* creator()
   {
      return new CArnoldShaderTranslator();
   }
   AtNode* CreateArnoldNodes();
   void Export(AtNode* atNode);
};


#endif // ARNOLDSHADERTRANSLATOR_H
