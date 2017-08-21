#include "BifrostTranslator.h"

#include "extension/Extension.h"

#include <maya/MDGMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MFileObject.h>
#include <maya/MGlobal.h>

// TODO: remove this comment

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
      BifrostTranslator::ClearCallbacks();
   }

   #endif
}



