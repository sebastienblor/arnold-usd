#pragma once

#include "extension/ExtensionsManager.h"
#include "extension/AbMayaNode.h"

#include <maya/MPxNode.h>

class CArnoldUserDataBoolNode : public MPxNode{
public:

    static void* creator();
    static MStatus initialize();

    static MTypeId id;

    CAbMayaNode m_abstract;
    static CAbMayaNode s_abstract;

    static CStaticAttrHelper s_attributes;

    static MObject s_OUT_Value;
    static MObject s_boolAttrName;
    static MObject s_defaultValue;
    
};
