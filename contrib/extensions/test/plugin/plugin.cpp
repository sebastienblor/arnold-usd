#include "test.h"
#include "extension/Extension.h"

extern "C"
{

DLLEXPORT void initializeExtension(CExtension &plugin)
{
   plugin.RegisterTranslator("lambert",
                             0x524c414d,
                             "test",
                             CTestTranslatorCmd::creator);
}

DLLEXPORT void deinitializeExtension(CExtension &plugin)
{
}

}
