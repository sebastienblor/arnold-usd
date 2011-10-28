#ifndef SHADER_TRANSLATOR_H
#define SHADER_TRANSLATOR_H

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
   AtNode* Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="");
   static void* creator()
   {
      return new CShaderTranslator();
   }
   virtual AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* atNode);
   virtual bool ResolveOutputPlug(const MPlug& outputPlug, MPlug &resolvedOutputPlug);
};


#endif // SHADER_TRANSLATOR_H
