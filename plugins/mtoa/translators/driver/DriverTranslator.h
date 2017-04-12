#pragma once

#include "translators/NodeTranslator.h"

//--------------- DriverTranslator ------------------------------------------

class DLLEXPORT CDriverTranslator
   :  public CNodeTranslator
{
public:
   static void* creator()
   {
      return new CDriverTranslator();
   }
   static void NodeInitializer(CAbTranslator context);
   AtNode* CreateArnoldNodes();
   void Export(AtNode* atNode);

   const char *GetBaseName() const
   {
      return m_baseName.c_str();
   }
   AtNode *GetChildDriver(const std::string &token);

protected:
   virtual void AddUpdateCallbacks();
   std::string m_baseName;
};
