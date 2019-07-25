#include "BifShapeTranslator.h"
#include "extension/Extension.h"

extern "C"
{
    DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;

        extension.Requires ( "bifrostGraph" ); // require the bifrostGraph plugin
        extension.RegisterTranslator("bifShape", // translate bifShape nodes
                                     "",
                                     CBifShapeTranslator::creator,
                                     CBifShapeTranslator::NodeInitializer);
    }

    DLLEXPORT void deinitializeExtension ( CExtension& extension )
    {
    }
}
