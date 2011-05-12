#include "ShaveAndHaircut.h"

#include "extension/Extension.h"

extern "C"
{

DLLEXPORT void initializeExtension(CExtension& extension)
{
   extension.LoadArnoldPlugin("shave_shaders");
   extension.RegisterTranslator("shaveHair",
                             0x1029b7,
                             "shave",
                             CShaveTranslator::creator,
                             CShaveTranslator::NodeInitializer);
}

DLLEXPORT void deinitializeExtension(CExtension& plugin)
{
   // plugin.DeloadArnoldPlugin("shave_shaders");
}

}

