#include "test.h"
#include "scene/Extension.h"

extern "C"
{

DLLEXPORT void initializePlugin(CExtension &plugin)
{
   plugin.RegisterDependTranslator("lambert",
                                   0x524c414d,
                                   "test",
                                   CTestTranslatorCmd::creator);
}

DLLEXPORT void uninitializePlugin(CExtension &plugin)
{
}

}
