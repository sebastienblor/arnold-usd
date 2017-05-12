#include "HairPhysicalShaderTranslator.h"

#include "extension/Extension.h"

extern "C"
{
    DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;

        extension.Requires ( "hairPhysicalShader" );
        //extension.LoadArnoldPlugin("hairPhysicalShader_shaders");

        status = extension.RegisterTranslator ( "hairPhysicalShader",
                                                "",
                                                CHairPhysicalShaderTranslator::creator,
                                                CHairPhysicalShaderTranslator::NodeInitializer );
    }

    DLLEXPORT void deinitializeExtension ( CExtension& extension )
    {
    }
}
