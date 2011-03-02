#ifndef ARNOLDNODEIDS_H
#define ARNOLDNODEIDS_H

// Arnolds internal ids are 0x00115C00 - 0x00115DFF

// Arnold Nodes in Maya
#define ARNOLD_NODEID_RENDER_OPTIONS            0x00115C00
#define ARNOLD_NODEID_STAND_INS                 0x00115C01
#define ARNOLD_NODEID_SPHERE_LOCATOR            0x00115C02
// Arnold Lights and light filters
#define ARNOLD_NODEID_BARNDOOR                  0x00115C80
#define ARNOLD_NODEID_GOBO                      0x00115C81
#define ARNOLD_NODEID_LIGHT_BLOCKER             0x00115C82
#define ARNOLD_NODEID_LIGHT_DECAY               0x00115C83
#define ARNOLD_NODEID_SKY                       0x00115C84
#define ARNOLD_NODEID_SKYDOME_LIGHT             0x00115C85
// Arnold Shaders
#define ARNOLD_NODEID_CUSTOM                    0x00115D00
#define ARNOLD_NODEID_STANDARD                  0x00115D01
#define ARNOLD_NODEID_DISPLACEMENT              0x00115D02
#define ARNOLD_NODEID_UTILITY                   0x00115D03
#define ARNOLD_NODEID_RAY_SWITCH                0x00115D04
#define ARNOLD_NODEID_MESH_INFO                 0x00115D05
#define ARNOLD_NODEID_AOV                       0x00115D06
#define ARNOLD_NODEID_AMBIENT_OCCLUSION         0x00115D07
#define ARNOLD_NODEID_WIREFRAME                 0x00115D08
#define ARNOLD_NODEID_HAIR                      0x00115D09
#define ARNOLD_NODEID_FOG                       0x00115D0A
#define ARNOLD_NODEID_VOLUME_SCATTERING         0x00115D0B
// For automatic node generation
#define ARNOLD_NODEID_AUTOGEN                   0x00115E00

// Maya builtins ids
#define MAYA_NODEID_SURFACE_SHADER              0x52535348
#define MAYA_NODEID_LAMBERT                     0x524c414d
#define MAYA_NODEID_FILE                        0x52544654
#define MAYA_NODEID_PLACE2D_TEXTURE             0x52504c32
#define MAYA_NODEID_BUMP2D                      0x5242554d
#define MAYA_NODEID_BUMP3D                      0x52425533
#define MAYA_NODEID_SAMPLER_INFO                0x5253494e
#define MAYA_NODEID_PLUS_MINUS_AVERAGE          0x52504d41
#define MAYA_NODEID_REMAP_VALUE                 0x524d564c
#define MAYA_NODEID_REMAP_COLOR                 0x524d434c
#define MAYA_NODEID_PROJECTION                  0x5250524a
#define MAYA_NODEID_RAMP                        0x52545241
#define MAYA_NODEID_LAYERED_TEXTURE             0x4c595254

#define MAYA_NODEID_DIRECTIONAL_LIGHT           0x4449524c
#define MAYA_NODEID_SPOT_LIGHT                  0x5350544c
#define MAYA_NODEID_AMBIENT_LIGHT               0x414d424c
#define MAYA_NODEID_AREA_LIGHT                  0x41524c54
#define MAYA_NODEID_POINT_LIGHT                 0x504f4954

#define MAYA_NODEID_MESH                        0x444d5348
#define MAYA_NODEID_NURBS_SURFACE               0x4e535246

#define MAYA_NODEID_CAMERA                      0x4443414d
#define MAYA_NODEID_HAIR                        0x48535953

#endif // ARNOLDNODEIDS_H