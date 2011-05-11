#include "ShaveAndHaircut.h"

#include "extension/Extension.h"

extern "C"
{

DLLEXPORT void initializeExtension(CExtension& plugin)
{
   plugin.LoadArnoldPlugin("shave_shaders");
   plugin.RegisterTranslator("shaveHair",
                             0x1029b7,
                             "shave",
                             CShaveTranslator::creator,
                             CShaveTranslator::NodeInitializer);
}

DLLEXPORT void deinitializeExtension(CExtension& plugin)
{
}
   // plugin.DeloadArnoldPlugin("shave_shaders");
}

