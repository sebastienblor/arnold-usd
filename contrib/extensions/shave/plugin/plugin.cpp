#include "ShaveAndHaircut.h"

#include "extension/Extension.h"

extern "C"
{

DLLEXPORT void initializeExtension(CExtension& plugin)
{
   plugin.RegisterTranslator("shaveHair",
                             0x1029b7,
                             "default",
                             CShaveTranslator::creator,
                             CShaveTranslator::NodeInitializer);
}

DLLEXPORT void deinitializeExtension(CExtension& plugin)
{
}

}

