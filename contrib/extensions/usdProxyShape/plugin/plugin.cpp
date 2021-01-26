#include "UsdProxyShapeTranslator.h"
#include "extension/Extension.h"

extern "C"
{
    DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;
        //extension.Requires ( "mayaUsdPlugin" ); -> handled in the mtd file
        extension.RegisterTranslator("mayaUsdProxyShape",
                                    "usd",
                                    CUsdProxyShapeTranslator::creator,
                                    CUsdProxyShapeTranslator::NodeInitializer);
      
    }

    DLLEXPORT void deinitializeExtension ( CExtension& extension )
    {
    }
}
