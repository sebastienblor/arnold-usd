#pragma once

#include "extension/ExtensionsManager.h"
#include "extension/AbMayaNode.h"

#include <maya/MPxNode.h>

class CArnoldUserDataVectorNode : public MPxNode{
public:

    static void* creator();
    static MStatus initialize();

    static MTypeId id;

    CAbMayaNode m_abstract;
    static CAbMayaNode s_abstract;

    static CStaticAttrHelper s_attributes;

    static MObject s_OUT_Value;
    static MObject s_OUT_ValueX;
    static MObject s_OUT_ValueY;
    static MObject s_OUT_ValueZ;

    static MObject s_OUT_transparencyR;
    static MObject s_OUT_transparencyG;
    static MObject s_OUT_transparencyB;
    static MObject s_OUT_transparency;
    
    static MObject s_vectorAttrName;
    static MObject s_defaultValue;
    static MObject s_defaultValueX;
    static MObject s_defaultValueY;
    static MObject s_defaultValueZ;
};
