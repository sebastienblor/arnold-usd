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
#define ARNOLD_NODEID_AREA_LIGHT                0x00115C86
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
#define ARNOLD_NODEID_WRITECOLOR                0x00115D10
#define ARNOLD_NODEID_WRITEFLOAT                0x00115D11
#define ARNOLD_NODEID_USERDATAFLOAT             0x00115D12
#define ARNOLD_NODEID_USERDATAVECTOR            0x00115D13
#define ARNOLD_NODEID_USERDATACOLOR             0x00115D14
#define ARNOLD_NODEID_USERDATASTRING            0x00115D15
// For automatic node generation
#define ARNOLD_NODEID_AUTOGEN                   0x00115E00

#endif // ARNOLDNODEIDS_H
