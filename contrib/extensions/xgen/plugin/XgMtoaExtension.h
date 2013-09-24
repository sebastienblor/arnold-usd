//-
// ==================================================================
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//+

#ifndef __XGENMTOAEXTENSION_H__
#define __XGENMTOAEXTENSION_H__

#include <translators/shape/ShapeTranslator.h>

class CXgDescriptionTranslator : public CShapeTranslator
{
public:

   AtNode* CreateArnoldNodes();
   virtual void Export(AtNode* shape);
   virtual void Update(AtNode* shape);
   void ExportMotion(AtNode*, unsigned int);

   static void* creator()
   {
      return new CXgDescriptionTranslator();
   }
   static void NodeInitializer(CAbTranslator context);

private:

   AtNode* ExportShaders(AtNode* instance);
};

#endif

