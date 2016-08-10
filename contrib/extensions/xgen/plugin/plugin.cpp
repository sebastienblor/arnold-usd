#include "XGenTranslator.h"
#include "extension/Extension.h"

extern "C"
{
#ifdef ENABLE_XGEN
	
	DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;

        extension.Requires ( "xgenToolkit" );
        status = extension.RegisterTranslator ( "xgmDescription",
                                                "",
                                                CXgDescriptionTranslator::creator,
                                                CXgDescriptionTranslator::NodeInitializer );
    }

    DLLEXPORT void deinitializeExtension ( CExtension& extension )
    {
    }
#endif
}
