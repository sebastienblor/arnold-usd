#include "ShaveAndHaircut.h"

#include "extension/Extension.h"

extern "C"
{

DLLEXPORT void initializePlugin(CExtension& plugin)
{
   plugin.RegisterTranslator("shaveHair",
                             0x1029b7,
                             "default",
                             CShaveTranslator::creator,
                             CShaveTranslator::NodeInitializer);
}

DLLEXPORT void uninitializePlugin(CExtension& plugin)
{
}

}

