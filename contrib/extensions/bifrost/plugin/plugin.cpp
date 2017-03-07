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

#if MAYA_API_VERSION < 201650
        extension.Requires ( "BifrostMain" );
#else
        extension.Requires ( "bifrostvisplugin" );
#endif
        extension.LoadArnoldPlugin("bifrost_shaders");

        status = extension.RegisterTranslator ( "bifrostShape", "",
                                                BifrostTranslator::creator,
                                                BifrostTranslator::NodeInitializer );

        status = extension.RegisterTranslator ( "bifrostAeroMaterial", "",
                                                CBfAeroMaterialTranslator::creator,
                                                CBfAeroMaterialTranslator::NodeInitializer );

        status = extension.RegisterTranslator ( "bifrostFoamMaterial", "",
                                                CBfFoamMaterialTranslator::creator,
                                                CBfFoamMaterialTranslator::NodeInitializer );
    }

	DLLEXPORT void deinitializeExtension ( CExtension& extension )
	{
	}

#endif
}
