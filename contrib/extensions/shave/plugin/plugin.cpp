#include "ShaveAndHaircut.h"

#include "api/Extension.h"

extern "C"
{

DLLEXPORT void initializePlugin(CExtension& plugin)
{
   plugin.RegisterDagTranslator("shaveHair",
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

