#include "BifrostTranslator.h"
#include "BifrostFoamMaterialTranslator.h"
#include "BifrostAeroMaterialTranslator.h"

#include "extension/Extension.h"
#include <maya/MTypes.h> 

extern "C"
{
#ifdef ENABLE_BIFROST
	
	DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;

#if MAYA_API_VERSION < 201650
        extension.Requires ( "BifrostMain" );
#endif
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
