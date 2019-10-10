/*
 * Arnold API header file
 * Copyright (c) 2019 Autodesk
 */

#pragma once

#include "ai.h"

#ifdef __cplusplus
#define AXFTOA_EXTERN_C extern "C"
#else
#define AXFTOA_EXTERN_C extern
#endif

# ifdef _WIN32
// Public functions need a special declaration in Win32
#  ifdef _AXFTOADLL
#   define AXFTOA_API AXFTOA_EXTERN_C __declspec(dllexport)
#  else
#   define AXFTOA_API AXFTOA_EXTERN_C __declspec(dllimport)
#  endif
# else
#  define AXFTOA_API  AXFTOA_EXTERN_C __attribute__(( visibility("default") ))
# endif

//
// Export a given axf file to arnold in a single call
//

// open and close axf file, export the default material to Arnold and write out required textures
AXFTOA_API AtNode *AxFtoAGetShader(AtUniverse* universe, const char *axf_path, const char *tex_path,
                        float uv_unit_size = 1.0f);

//
// Session
//

// restarts the session, resets the export cache
AXFTOA_API void AxFtoASessionStart();

// ends the session and releases resources (but not arnold nodes)
AXFTOA_API void AxFtoASessionEnd();

// Set and Get the working color space (will invalidate the cache)
// If not set, it will use linear sRGB
AXFTOA_API void AxFtoASessionSetColorSpace(const char *color_space);
AXFTOA_API const char *AxFtoASessionGetColorSpace();

// clear session errors
AXFTOA_API void AxFtoASessionClearErrors();

// check for errors
AXFTOA_API bool AxFtoASessionHasErrors();

// set verbosity
AXFTOA_API void AxFtoASessionSetVerbosity(int v);

// get verbosity
AXFTOA_API int AxFtoASessionGetVerbosity();

//
// AxF File
//

struct AxFtoAFile;

// Open an AxF shader file
AXFTOA_API AxFtoAFile* AxFtoAFileOpen(const char *axf_path);

// Close and release an AxF shader file
AXFTOA_API void AxFtoAFileClose(AxFtoAFile *file);

// enumerate materials in axf file
AXFTOA_API int AxFtoAFileGetNumMaterials(AxFtoAFile *file);

struct AxFtoAMaterial;

// get specific material index
AXFTOA_API AxFtoAMaterial* AxFtoAFileGetMaterial(AxFtoAFile *file, int idx);


//
// AxF Materials
//

// get user readable material name
AXFTOA_API const char *AxFtoAMaterialGetName(AxFtoAMaterial* material);

// Set and Get the working color space (will invalidate the cache)
// If not set, it will use linear sRGB
AXFTOA_API void AxFtoAMaterialSetColorSpace(AxFtoAMaterial *material, const char *color_space);
AXFTOA_API const char *AxFtoAMaterialGetColorSpace(AxFtoAMaterial *material);

// These methods can be called with material set to NULL to set default values
// for the following operations or to get data for all previous operations

// Set and Get universe (will use the default otherwise)
AXFTOA_API void AxFtoAMaterialSetUniverse(AxFtoAMaterial* e, AtUniverse* universe);
AXFTOA_API AtUniverse* AxFtoAMaterialGetUniverse(AxFtoAMaterial* e);

// Set and Get texture folder (will use the default otherwise)
AXFTOA_API void AxFtoAMaterialSetTextureFolder(AxFtoAMaterial* e, const char *path);
AXFTOA_API const char *AxFtoAMaterialGetTextureFolder(AxFtoAMaterial* e);

// Set and get UV unit size (will use the default otherwise)
AXFTOA_API void AxFtoAMaterialSetUVUnitSize(AxFtoAMaterial* e, float uv_unit_size);
AXFTOA_API float AxFtoAMaterialGetUVUnitSize(AxFtoAMaterial* e);

// Set and get prefixes (will use the default otherwise)
AXFTOA_API void AxFtoAMaterialSetNodeNamePrefix(AxFtoAMaterial*, const char*);
AXFTOA_API void AxFtoAMaterialSetTextureNamePrefix(AxFtoAMaterial*, const char*);

// Get number of arnold nodes
AXFTOA_API int AxFtoAMaterialGetNumNodes(AxFtoAMaterial*);
// Get arnold node at index
AXFTOA_API AtNode *AxFtoAMaterialGetNode(AxFtoAMaterial*, int idx);
// Get Arnold shading tree root node
AXFTOA_API AtNode *AxFtoAMaterialGetRootNode(AxFtoAMaterial*);

// Get number of output textures (only includes the ones updated in this conversion)
AXFTOA_API int AxFtoAMaterialGetNumTextures(AxFtoAMaterial*);
// get texture path at index
AXFTOA_API const char *AxFtoAMaterialGetTexturePath(AxFtoAMaterial*, int idx);

// Write texture files (only will write out missing textures)
AXFTOA_API int AxFtoAMaterialWriteTextures(AxFtoAMaterial*);
