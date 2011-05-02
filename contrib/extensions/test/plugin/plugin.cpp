#include "test.h"
#include "scene/Extension.h"

extern "C"
{

DLLEXPORT void initializePlugin(CExtension &plugin)
{
   plugin.RegisterTranslator("lambert",
                                   0x524c414d,
                                   "test",
                                   CTestTranslatorCmd::creator);
}

DLLEXPORT void uninitializePlugin(CExtension &plugin)
{
}

}
