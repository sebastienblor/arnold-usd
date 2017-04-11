#include "extension/Extension.h"
#include "translators/shader/ShaderTranslator.h"

extern "C"
{

DLLEXPORT void initializeExtension(CExtension& extension)
{
//   extension.LoadArnoldPlugin("lookdevkit_shaders");
   extension.RegisterPluginNodesAndTranslators("lookdevkit");
}

DLLEXPORT void deinitializeExtension(CExtension& extension)
{
}

}
