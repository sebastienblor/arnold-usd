#include "UsdPreviewSurfaceTranslator.h"
#include "extension/Extension.h"

extern "C"
{
    DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;
        extension.Requires ( "mayaUsdPlugin" );
        extension.RegisterTranslator("pxrUsdPreviewSurface",
                                    "standard_surface",
                                    CUsdPreviewSurfaceTranslator::creator);
      
    }

    DLLEXPORT void deinitializeExtension ( CExtension& extension )
    {
    }
}
