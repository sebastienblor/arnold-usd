#include "XGenTranslator.h"
#include "extension/Extension.h"

extern "C"
{
	DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;

        //extension.Requires ( "xgenToolkit" ); -> this is now handled in the mtd file #3923
        status = extension.RegisterTranslator ( "xgmDescription",
                                                "",
                                                CXgDescriptionTranslator::creator,
                                                CXgDescriptionTranslator::NodeInitializer );
    }

    DLLEXPORT void deinitializeExtension ( CExtension& extension )
    {
    }
}