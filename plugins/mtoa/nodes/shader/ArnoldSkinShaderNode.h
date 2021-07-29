#pragma once

#include "extension/ExtensionsManager.h"
#include "extension/AbMayaNode.h"

#include <maya/MPxNode.h>

class CArnoldSkinShaderNode : public MPxNode{
public:
    void postConstructor() override;

    static void* creator();
    static MStatus initialize();
   SchedulingType schedulingType()const override {return kSerial;}
    static const MTypeId id;

    CAbMayaNode m_abstract;
    static CAbMayaNode s_abstract;

    static CStaticAttrHelper* s_nodeHelper;

    static MObject s_OUT_color;
    static MObject s_OUT_transparencyR;
    static MObject s_OUT_transparencyG;
    static MObject s_OUT_transparencyB;
    static MObject s_OUT_transparency;
    static MObject s_normal_camera;
};
