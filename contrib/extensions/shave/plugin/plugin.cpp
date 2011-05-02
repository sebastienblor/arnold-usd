#include "ShaveAndHaircut.h"

#include "scene/Extension.h"

extern "C"
{

DLLEXPORT void initializePlugin(CExtension& plugin)
{
   plugin.RegisterTranslator("shaveHair",
                                0x1029b7,
                                "default",
                                CShaveTranslator::creator,
                                CShaveTranslator::NodeInitializer,
                                "shaveNode");
}

DLLEXPORT void uninitializePlugin(CExtension& plugin)
{
}

}

