#include "BifrostTranslator.h"
#include "BifrostFoamMaterialTranslator.h"
#include "BifrostAeroMaterialTranslator.h"

#include "extension/Extension.h"

extern "C"
{
#ifdef ENABLE_BIFROST
	
	DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;

        extension.Requires ( "BifrostMain" );

        extension.LoadArnoldPlugin("bifrost_shaders");
        
        status = extension.RegisterTranslator ( "bifrostShape",
                                                "",
                                                CBfDescriptionTranslator::creator,
                                                CBfDescriptionTranslator::NodeInitializer );


        status = extension.RegisterTranslator ( "bifrostFoamMaterial",
                                                "",
                                                CBfFoamMaterialTranslator::creator);

        status = extension.RegisterTranslator ( "bifrostAeroMaterial",
                                                "",
                                                CBfAeroMaterialTranslator::creator,
                                                CBfAeroMaterialTranslator::NodeInitializer );

       

    }

    DLLEXPORT void deinitializeExtension ( CExtension& extension )
    {
    }
#endif
}
