#include "BifrostTranslator.h"
#include "extension/Extension.h"

extern "C"
{
   #ifdef ENABLE_BIFROST
   DLLEXPORT void initializeExtension ( CExtension& extension )
   {
      MStatus status;

      extension.Requires ( "bifrostvisplugin" );

      status = extension.RegisterTranslator ( "bifrostShape", "",
                              BifrostTranslator::creator,
                              BifrostTranslator::NodeInitializer );

      
   }

   DLLEXPORT void deinitializeExtension ( CExtension& extension )
   {
      BifrostTranslator::Uninitialize();
   }

   #endif
}



