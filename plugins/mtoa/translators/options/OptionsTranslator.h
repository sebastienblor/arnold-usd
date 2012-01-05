#ifndef OPTIONSTRANSLATOR_H
#define OPTIONSTRANSLATOR_H

#include "translators/NodeTranslator.h"

class DLLEXPORT COptionsTranslator : public CNodeTranslator
{
public:
   static void* creator(){return new COptionsTranslator();}
   virtual void Export(AtNode* options);
   virtual void Update(AtNode *options);
   AtNode* CreateArnoldNodes();
   virtual bool DependsOnExportCamera() {return true;}

   void SetCamera(AtNode *options, MDagPath& cameraNode);
   bool IsActiveAOV(CAOV &aov) const
   {
      if (m_aovs.count(aov))
         return true;
      else
         return false;
   }

   AOVSet GetActiveAOVs() const
   {
      return m_aovs;
   }

protected:
   COptionsTranslator()  :
      CNodeTranslator(),
      m_driver(NULL)
   {
      // Just for debug info, translator creates whatever arnold nodes are required
      // through the CreateArnoldNodes method
      m_abstract.arnold = "options";
   }
   void ProcessAOVs();
   MString SetImageFilenames(MDagPath &camera);
   AtNode * CreateFileOutput(MStringArray &outputs, AtNode *defaultFilter);
   AtNode * CreateOutputFilter();
   AtNode * CreateRenderViewOutput(MStringArray &outputs, AtNode *defaultFilter);
   void SetupRenderOutput(AtNode* options);

protected:
   AtNode* m_driver;
   AOVSet m_aovs;
};

#endif // OPTIONSTRANSLATOR_H
