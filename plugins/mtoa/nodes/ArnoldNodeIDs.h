#pragma once

// We have 5 blocks of 512 IDs each

// 0x00116400 - 0x001165FF
// 0x00116200 - 0x001163FF
// 0x00116000 - 0x001161FF
// 0x00115E00 - 0x00115FFF
// Arnolds internal ids are 0x00115C00 - 0x00115DFF

// Arnold Nodes in Maya
#define ARNOLD_NODEID_RENDER_OPTIONS            0x00115C00
#define ARNOLD_NODEID_STAND_INS                 0x00115C01
#define ARNOLD_NODEID_SPHERE_LOCATOR            0x00115C02
#define ARNOLD_NODEID_DRIVER                    0x00115C03
#define ARNOLD_NODEID_FILTER                    0x00115C04

// Arnold Lights and light filters
#define ARNOLD_NODEID_BARNDOOR                  0x00115C80
#define ARNOLD_NODEID_GOBO                      0x00115C81
#define ARNOLD_NODEID_LIGHT_BLOCKER             0x00115C82
#define ARNOLD_NODEID_LIGHT_DECAY               0x00115C83
#define ARNOLD_NODEID_SKY                       0x00115C84
#define ARNOLD_NODEID_SKYDOME_LIGHT             0x00115C85
#define ARNOLD_NODEID_AREA_LIGHT                0x00115C86
#define ARNOLD_NODEID_PHOTOMETRIC_LIGHT         0x00115C87
// Arnold Shaders
#define ARNOLD_NODEID_CUSTOM                    0x00115D00
#define ARNOLD_NODEID_STANDARD                  0x00115D01
#define ARNOLD_NODEID_DISPLACEMENT              0x00115D02
#define ARNOLD_NODEID_UTILITY                   0x00115D03
#define ARNOLD_NODEID_RAY_SWITCH                0x00115D04
#define ARNOLD_NODEID_MESH_INFO                 0x00115D05
#define ARNOLD_NODEID_AOV                       0x00115D06  // FIXME: move to non-shader ID section above?
#define ARNOLD_NODEID_AMBIENT_OCCLUSION         0x00115D07
#define ARNOLD_NODEID_WIREFRAME                 0x00115D08
#define ARNOLD_NODEID_HAIR                      0x00115D09
#define ARNOLD_NODEID_FOG                       0x00115D0A
#define ARNOLD_NODEID_VOLUME_SCATTERING         0x00115D0B
#define ARNOLD_NODEID_BUMP2D                    0x00115D0C
#define ARNOLD_NODEID_NOISE                     0x00115D0D
#define ARNOLD_NODEID_BUMP3D                    0x00115D0E
#define ARNOLD_NODEID_MOTION_VECTOR             0x00115D0F
#define ARNOLD_NODEID_WRITE_COLOR               0x00115D10
#define ARNOLD_NODEID_WRITE_FLOAT               0x00115D11
#define ARNOLD_NODEID_USER_DATA_FLOAT           0x00115D12
#define ARNOLD_NODEID_USER_DATA_VECTOR          0x00115D13
#define ARNOLD_NODEID_USER_DATA_COLOR           0x00115D14
#define ARNOLD_NODEID_USER_DAT_ASTRING          0x00115D15
#define ARNOLD_NODEID_SKIN                      0x00115D16
#define ARNOLD_NODEID_IMAGE                     0x00115D17
#define ARNOLD_NODEID_USER_DATA_BOOL            0x00115D18
#define ANROLD_NODEID_SHADOW_CATCHER            0x00115D19
#define ARNOLD_NODEID_MAYA_HAIR                 0x00115D1A
#define ARNOLD_NODEID_MESHLIGHT_MATERIAL        0x00115D1B
#define ARNOLD_NODEID_USER_DATA_PNT2            0x00115D1C
#define ARNOLD_NODEID_USER_DATA_INT             0x00115D1D
#define ARNOLD_NODEID_PHYSICAL_SKY              0x00115D1E

// For automatic node generation
#define ARNOLD_NODEID_AUTOGEN                   0x00115E00
