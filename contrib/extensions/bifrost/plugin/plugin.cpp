#include "BifrostTranslator.h"
#include "BifrostFoamMaterialTranslator.h"
#include "BifrostAeroMaterialTranslator.h"
#include "BifrostHydroTranslator.h"
#include "BifrostPointTranslator.h"
#include "BifrostAeroTranslator.h"

#include "extension/Extension.h"
#include <maya/MTypes.h> 
#include <iostream>

extern "C"
{
#ifdef ENABLE_BIFROST
	DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;

#if MAYA_API_VERSION < 201650
        extension.Requires ( "BifrostMain" );
#else
        extension.Requires ( "bifrostvisplugin" );
#endif
        extension.LoadArnoldPlugin("bifrost_shaders");
        
        status = extension.RegisterTranslator ( "bifrostShape",
												"",
                                                BifrostShapeTranslator::creator,
                                                BifrostShapeTranslator::NodeInitializer );

        status = extension.RegisterTranslator ( "bifrostFoamMaterial",
                                                "",
                                                CBfFoamMaterialTranslator::creator);

        status = extension.RegisterTranslator ( "bifrostAeroMaterial",
                                                "",
                                                CBfAeroMaterialTranslator::creator,
                                                CBfAeroMaterialTranslator::NodeInitializer );

        status = extension.RegisterTranslator ( "bifrostHydro",
                                                "",
                                                BifrostHydroTranslator::creator,
                                                BifrostHydroTranslator::NodeInitializer );

        status = extension.RegisterTranslator ( "bifrostPoint",
                                                "",
                                                BifrostPointTranslator::creator,
                                                BifrostPointTranslator::NodeInitializer );

        status = extension.RegisterTranslator ( "bifrostAero",
                                                "",
                                                BifrostAeroTranslator::creator,
                                                BifrostAeroTranslator::NodeInitializer );
    }

    DLLEXPORT void deinitializeExtension ( CExtension& extension )
    {
    }
#endif
}
