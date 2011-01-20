#include "test.h"
//#include "Plugins.h"
#include "nodes/ArnoldNodeFactory.h"

extern "C"
{
   
void initializePlugin(CExtension& plugin)
{
   cout << "initializing test extension" << endl;
   // Maya lambert
   plugin.RegisterDependTranslator("lambert", 0x524c414d, CTestTranslatorCmd::creator);
}

void uninitializePlugin(CExtension& plugin)
{
   //status = plugin.deregisterCommand("mtoa_export_camera");
}

}

