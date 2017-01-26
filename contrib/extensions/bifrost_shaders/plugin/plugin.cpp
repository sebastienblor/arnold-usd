#include "extension/Extension.h"

#include "BifrostMayaShadersTranslator.h"

extern "C"
{
	DLLEXPORT void initializeExtension ( CExtension& extension )
	{
		MStatus status;

        extension.Requires ( "bifrostshaders" );

        extension.LoadArnoldPlugin("new_bifrost_shaders");

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
}
