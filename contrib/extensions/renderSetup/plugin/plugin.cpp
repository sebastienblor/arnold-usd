#include "extension/Extension.h"
#include <maya/MTypes.h> 

extern "C"
{

DLLEXPORT void initializeExtension(CExtension& extension)
{
    extension.LoadArnoldPlugin("renderSetup_shaders");
    extension.RegisterPluginNodesAndTranslators("renderSetup");
}

DLLEXPORT void deinitializeExtension(CExtension& extension)
{
}

}
