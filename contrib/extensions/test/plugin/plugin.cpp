#include "test.h"
#include "nodes/ArnoldNodeFactory.h"

extern "C"
{

DLLEXPORT void initializePlugin(CExtension &plugin)
{
   plugin.RegisterDependTranslator("lambert", 0x524c414d, CTestTranslatorCmd::creator);
}

DLLEXPORT void uninitializePlugin(CExtension &plugin)
{
}

}
