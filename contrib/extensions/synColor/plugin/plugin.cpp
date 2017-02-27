#include "extension/Extension.h"

#include "SynColorTranslator.h"

extern "C"
{
    DLLEXPORT void initializeExtension(CExtension& extension)
    {
        extension.LoadArnoldPlugin("synColor_shaders");
        extension.RegisterPluginNodesAndTranslators("synColor");
        extension.RegisterTranslator("colorManagementGlobals",
                                     "",
                                     CSynColorTranslator::creator,
                                     CSynColorTranslator::NodeInitializer);
    }

    DLLEXPORT void deinitializeExtension(CExtension& extension)
    {
    }
}
