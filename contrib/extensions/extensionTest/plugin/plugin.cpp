#include "translators.h"
#include "extension/Extension.h"
//#include "testNode.h"
extern "C"
{

DLLEXPORT void initializeExtension(CExtension &plugin)
{

   plugin.RegisterTranslator("mesh",
    "pouet",
    MtoaTestProcTranslator::creator,
    CShapeTranslator::NodeInitializer);
   plugin.RegisterTranslator ( "aiMtoaTestShader",
                                                "",
                                                MtoaTestShaderTranslator::creator );
}

DLLEXPORT void deinitializeExtension(CExtension &plugin)
{
}

}
