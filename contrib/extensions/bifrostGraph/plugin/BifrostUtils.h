#pragma once 
#include <maya/MString.h>
#include "platform/Platform.h"

#include <stdio.h>
#include <iostream>
#include <sstream>
#include "BifrostUtils.h"
#include "extension/Extension.h"
#include "extension/ExtensionsManager.h"
#include <ai.h>


#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include <maya/MFileObject.h>


// Pass the results of this to arnold_bifrost as a "handle" parameter, it takes care of the rest
typedef void*    (*ArnoldBifrostPrepareHandleFunc)(void *input_buffer, size_t input_length);
// Write the contents to wherever you like, but use AiFree() to clean up the return buffer
typedef uint8_t* (*ArnoldBifrostSerializeDataFunc)(void *input_buffer, size_t input_length, size_t& output_length);

class DLLEXPORT BifrostUtils
{
   public:
      static bool LoadBifrostProcedural();
      static void GetArnoldBifrostAPIExtensions(const char* dsoPath, const char* dsoName);
      static MString GetArnoldBifrostPath();
      static ArnoldBifrostPrepareHandleFunc s_arnoldBifrostPrepareHandle ;
      static ArnoldBifrostSerializeDataFunc s_arnoldBifrostSerializeData ;
      static MString GetBifrostProceduralPath();
      static int GetArnoldBifrostABIRev();
};




