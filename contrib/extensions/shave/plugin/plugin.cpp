#include "ShaveAndHaircut.h"

#include "extension/Extension.h"

extern "C"
{

DLLEXPORT void initializeExtension(CExtension& extension)
{
   MStatus status;

   extension.Requires("shaveNode");
   extension.LoadArnoldPlugin("shave_shaders");
   status = extension.RegisterTranslator("shaveHair",
         "shave",
         CShaveTranslator::creator,
         CShaveTranslator::NodeInitializer);
}

DLLEXPORT void deinitializeExtension(CExtension& extension)
{
}

}

