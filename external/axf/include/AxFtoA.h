/*
 * Arnold API header file
 * Copyright (c) 2019 Autodesk
 */

#pragma once

#include "ai.h"

//
// Export a given axf file to arnold in a single call
//

// open and close axf file, export the default material to Arnold and write out required textures
AtNode *AxFtoAGetShader(AtUniverse* universe, const char *axf_path, const char *tex_path,
                        float uv_unit_size = 1.0f);

//
// Session
//

// resets the export cache (no need to close the session)
void AxFtoASessionStart();

//
// AxF File
//

struct AxFtoAFile;

// Open an AxF shader file
AxFtoAFile* AxFtoAFileOpen(const char *axf_path);

// Close an AxF shader file
void AxFtoAFileClose(AxFtoAFile *file);

// enumerate materials in axf file
int AxFtoAFileGetNumMaterials(AxFtoAFile *file);

struct AxFtoAMaterial;

// get specific material index
AxFtoAMaterial* AxFtoAFileGetMaterial(AxFtoAFile *file);


//
// AxF Materials
//

// get user readable material name
const char *AxFtoAMaterialGetName(AxFtoAMaterial* material);

struct AxFtoAExports;

// Create an Arnold exported shading tree
AxFtoAExports* AxFtoACreateExport(AxFtoAMaterial* material);


//
// Exports are Arnold nodes and textures from an AxF file
//
// These methods can be called with export set to NULL to set default values
// for the following operations or to get data for all previous operations
//

// Set and Get universe (will use the default otherwise)
void AxFtoAExportsSetUniverse(AxFtoAExports* e, AtUniverse* universe);
AtUniverse* AxFtoAExportsGetUniverse(AxFtoAExports* e);

// Set and Get color space (will use the default otherwise)
void AxFtoAExportsSetColorSpace(AxFtoAExports* e, const char *color_space);
const char *AxFtoAExportsGetColorSpace(AxFtoAExports* e);

// Set and Get texture folder (will use the default otherwise)
void AxFtoAExportsSetTextureFolder(AxFtoAExports* e, const char *path);
const char *AxFtoAExportsGetTextureFolder(AxFtoAExports* e);

// Set and get UV unit size (will use the default otherwise)
void AxFtoAExportsSetUVUnitSize(AxFtoAExports* e, float uv_unit_size);
float AxFtoAExportsGetUVUnitSize(AxFtoAExports* e);

// Set and get prefixes (will use the default otherwise)
void AxFtoAExportsSetNodeNamePrefix(AxFtoAExports*, const char*);
void AxFtoAExportsSetTextureNamePrefix(AxFtoAExports*, const char*);

// Get number of arnold nodes
int AxFtoAExportsGetNumNodes(AxFtoAExports*);
// Get arnold node at index
AtNode *AxFtoAExportsGetNode(AxFtoAExports*, int idx);
// Get Arnold shading tree root node
AtNode *AxFtoAExportsGetRootNode(AxFtoAExports*);

// Get number of textures
int AxFtoAExportsGetNumTextures(AxFtoAExports*);
// get texture path at index
const char *AxFtoAExportsGetTexturePath(AxFtoAExports*, int idx);

// Write texture files
int AxFtoAExportsWriteTextures(AxFtoAExports*);
