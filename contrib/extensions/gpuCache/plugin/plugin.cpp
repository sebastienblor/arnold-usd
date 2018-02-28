#include "GpuCacheTranslator.h"
#include "extension/Extension.h"

extern "C"
{
    DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;

        extension.Requires ( "gpuCache" );
        extension.RegisterTranslator("gpuCache",
                                    "alembic",
                                    CGpuCacheTranslator::creator,
                                    CGpuCacheTranslator::NodeInitializer);
      
    }

    DLLEXPORT void deinitializeExtension ( CExtension& extension )
    {
    }
}
