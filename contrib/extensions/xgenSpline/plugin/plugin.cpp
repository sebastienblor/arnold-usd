#include "XGenSplineTranslator.h"
#include "XGenHairMappingTranslator.h"

#include "extension/Extension.h"

extern "C"
{
    
    DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;

        //extension.Requires ( "xgenToolkit" ); -> this is now handled in the .mtd file #3923
//        extension.LoadArnoldPlugin("xgenSpline_shaders");

        status = extension.RegisterTranslator ( "xgmSplineDescription",
                                                "",
                                                CXgSplineDescriptionTranslator::creator,
                                                CXgSplineDescriptionTranslator::NodeInitializer );

        status = extension.RegisterTranslator ( "xgmHairMapping",
                                                "",
                                                CXgHairMappingTranslator::creator );
    }

    DLLEXPORT void deinitializeExtension ( CExtension& extension )
    {
    }

}
