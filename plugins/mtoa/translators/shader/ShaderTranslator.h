#ifndef SHADERTRANSLATOR_H
#define SHADERTRANSLATOR_H

#include "translators/NodeTranslator.h"

//--------------- CShaderTranslator ------------------------------------------

/// A Translator class which can automatically export simple Maya nodes.

/// To perform an automatic export, the translator does the following:
///  -# gets the Arnold node entry that corresponds to the Maya node being export from m_arnoldNodeName
///  -# loops through each parameter on the Arnold node entry
///  -# processes the equivalent attribute on the Maya node
///

class DLLEXPORT CShaderTranslator
   :  public CNodeTranslator
{
public:
   AtNode* Init(CMayaScene* scene, MDagPath& dagPath, MString outputAttr="");
   static void* creator()
   {
      return new CShaderTranslator();
   }
   AtNode* CreateArnoldNodes();
   void Export(AtNode* atNode);

protected:
   bool m_motion;
};


#endif // SHADERTRANSLATOR_H
