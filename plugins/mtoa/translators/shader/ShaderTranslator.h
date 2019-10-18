#pragma once

#include "translators/NodeTranslator.h"

#include <maya/MPlugArray.h>

/** \class CShaderTranslator
 A Translator class which can automatically export simple Maya shaders

 To perform an automatic export, the translator does the following:
  -# gets the Arnold node entry that corresponds to the Maya node being export from m_arnoldNodeName
  -# loops through each parameter on the Arnold node entry
  -# processes the equivalent attribute on the Maya node

 \see CNodeTranslator
*/

class DLLEXPORT CShaderTranslator
   :  public CNodeTranslator
{
friend class CShaderTranslatorImpl;

public:
   static void* creator()
   {
      return new CShaderTranslator();
   }
   virtual ~CShaderTranslator();
   //---- virtual functions derived from CNodeTranslator
   virtual AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* atNode);
   virtual void ExportMotion(AtNode *shader);
   virtual bool RequiresMotionData();
   
protected:
   virtual void NodeChanged(MObject& node, MPlug& plug); 
   //----

   /// This function exports the bump for current shader
   void ExportBump(AtNode* shader);

private:
   AtNode *GetMatteShader();

   /// Internal use only. Do Not override it
   virtual void CreateImplementation();
};
