#include "LookDevKitTranslator.h"
#include "extension/Extension.h"
#include <cassert>

#include <maya/MFnNumericData.h>

AtNode* CLookDevKitTranslator::CreateArnoldNodes()
{
    MString nodeType = MFnDependencyNode(GetMayaObject()).typeName();
    MString prefix = nodeType.substringW(0, 0);
    nodeType = nodeType.substringW(1, nodeType.length() - 1);
    nodeType = "ldk" + prefix.toUpperCase() + nodeType;
    
    AtNode *node = AddArnoldNode(nodeType.asChar());
    return node;
}

void CLookDevKitTranslator::Export(AtNode* shader)
{
    CNodeTranslator::Export(shader);
}


