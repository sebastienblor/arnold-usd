#include "BifrostTranslator.h"
#include "BifrostFoamMaterialTranslator.h"
#include "BifrostAeroMaterialTranslator.h"

#include "extension/Extension.h"
#include <maya/MTypes.h> 

#include <maya/MDGMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MFileIO.h>
#include <maya/MGlobal.h>

extern "C"
{
   
#ifdef ENABLE_BIFROST
    
    DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;

#if MAYA_API_VERSION < 201650
        extension.Requires ( "BifrostMain" );
#else
        extension.Requires ( "bifrostvisplugin" );
#endif
//        extension.LoadArnoldPlugin("bifrost_shaders");
        
        status = extension.RegisterTranslator ( "bifrostShape",
                                                "",
                                                CBfDescriptionTranslator::creator,
                                                CBfDescriptionTranslator::NodeInitializer );


        status = extension.RegisterTranslator ( "bifrostFoamMaterial",
                                                "",
                                                CBfFoamMaterialTranslator::creator);

        status = extension.RegisterTranslator ( "bifrostAeroMaterial",
                                                "",
                                                CBfAeroMaterialTranslator::creator,
                                                CBfAeroMaterialTranslator::NodeInitializer );

        
       

    }

    DLLEXPORT void deinitializeExtension ( CExtension& extension )
    {
        CBfDescriptionTranslator::ClearCallbacks();
    }
#endif
}

