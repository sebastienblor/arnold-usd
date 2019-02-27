#include "translators.h"
#include <maya/MFnDependencyNode.h>

AtNode* CApplyRelIntOverrideTranslator::CreateArnoldNodes()
{
    AtNode* multiply = AddArnoldNode("multiply", "multiply");
    AtNode* add = AddArnoldNode("add", "offset");
    AtNode* float_to_int = AddArnoldNode("float_to_int", "float_to_int");
    AiNodeLink(multiply, "input2", add);
    AiNodeLink(add, "input", float_to_int);
    return add;
}

void CApplyRelIntOverrideTranslator::Export( AtNode* shader)
{
    MFnDependencyNode mnode(this->GetMayaObject());
    AtNode* multiply = GetArnoldNode("multiply");
    AtNode* add = GetArnoldNode("offset");
    AtNode* float_to_int = shader;
    const float orig = mnode.findPlug("original", true).asFloat();
    if (mnode.findPlug("enabled", true).asBool())
    {
        const float mult = mnode.findPlug("multiply", true).asFloat();
        const float offset = mnode.findPlug("offset", true).asFloat();
        AiNodeSetRGB(multiply, "input1", orig,0,0);
        AiNodeSetRGB(multiply, "input2", mult,0,0);
        AiNodeSetRGB(add, "input1", offset,0,0);
    }
    else
    {
        AiNodeSetRGB(add, "input1", orig,0,0);
        AiNodeSetRGB(multiply, "input2", 0,0,0);
        AiNodeSetRGB(multiply, "input1", 0,0,0);
    }
}


AtNode* CApplyRelFloatOverrideTranslator::CreateArnoldNodes()
{
    AtNode* multiply = AddArnoldNode("multiply", "multiply");
    AtNode* add = AddArnoldNode("add", "offset");
    AiNodeLink(multiply, "input2", add);
    return add;
}

void CApplyRelFloatOverrideTranslator::Export( AtNode* shader)
{
    MFnDependencyNode mnode(this->GetMayaObject());
    AtNode* multiply = GetArnoldNode("multiply");
    AtNode* add = shader;
    const float orig = mnode.findPlug("original", true).asFloat();
    if (mnode.findPlug("enabled", true).asBool())
    {
        const float mult = mnode.findPlug("multiply", true).asFloat();
        const float offset = mnode.findPlug("offset", true).asFloat();
        AiNodeSetRGB(multiply, "input1", orig,orig,orig);
        AiNodeSetRGB(multiply, "input2", mult,mult,mult);
        AiNodeSetRGB(add, "input1", offset,offset,offset);
    }
    else
    {
        AiNodeSetRGB(add, "input1", orig,orig,orig);
        AiNodeSetRGB(multiply, "input2", 0,0,0);
        AiNodeSetRGB(multiply, "input1", 0,0,0);
    }
}

AtNode* CApplyRelFloat2OverrideTranslator::CreateArnoldNodes()
{
    AtNode* multiply = AddArnoldNode("multiply", "multiply");
    AtNode* add = AddArnoldNode("add", "offset");
    AiNodeLink(multiply, "input2", add);
    return add;
}

void CApplyRelFloat2OverrideTranslator::Export( AtNode* shader)
{
    MFnDependencyNode mnode(this->GetMayaObject());
    AtNode* multiply = GetArnoldNode("multiply");
    AtNode* add = shader;
    const float2& orig = mnode.findPlug("original", true).asMDataHandle().asFloat2();
    if (mnode.findPlug("enabled", true).asBool())
    {
        const float2& mult = mnode.findPlug("multiply", true).asMDataHandle().asFloat2();
        const float2& offset = mnode.findPlug("offset", true).asMDataHandle().asFloat2();
        AiNodeSetRGB(multiply, "input1", orig[0],orig[1],0);
        AiNodeSetRGB(multiply, "input2", mult[0],mult[1],0);
        AiNodeSetRGB(add, "input1", offset[0],offset[1],0);
    }
    else
    {
        AiNodeSetRGB(add, "input1", orig[0],orig[1],0);
        AiNodeSetRGB(multiply, "input2", 0,0,0);
        AiNodeSetRGB(multiply, "input1", 0,0,0);
    }
}

AtNode* CApplyRelFloat3OverrideTranslator::CreateArnoldNodes()
{
    AtNode* multiply = AddArnoldNode("multiply", "multiply");
    AtNode* add = AddArnoldNode("add", "offset");
    AiNodeLink(multiply, "input2", add);
    return add;
}

void CApplyRelFloat3OverrideTranslator::Export( AtNode* shader)
{
    MFnDependencyNode mnode(this->GetMayaObject());
    AtNode* multiply = GetArnoldNode("multiply");
    AtNode* add = shader;
    const float3& orig = mnode.findPlug("original", true).asMDataHandle().asFloat3();
    if (mnode.findPlug("enabled", true).asBool())
    {
        const float3& mult = mnode.findPlug("multiply", true).asMDataHandle().asFloat3();
        const float3& offset = mnode.findPlug("offset", true).asMDataHandle().asFloat3();
        AiNodeSetRGB(multiply, "input1", orig[0],orig[1],orig[2]);
        AiNodeSetRGB(multiply, "input2", mult[0],mult[1],mult[2]);
        AiNodeSetRGB(add, "input1", offset[0],offset[1],offset[2]);
    }
    else
    {
        AiNodeSetRGB(add, "input1", orig[0],orig[2],orig[3]);
        AiNodeSetRGB(multiply, "input2", 0,0,0);
        AiNodeSetRGB(multiply, "input1", 0,0,0);
    }
}

AtNode* CApplyAbsIntOverrideTranslator::CreateArnoldNodes()
{
    return AddArnoldNode("float_to_int");
}

void CApplyAbsIntOverrideTranslator::Export( AtNode* shader)
{
    MFnDependencyNode mnode(this->GetMayaObject());
    if (mnode.findPlug("enabled", true).asBool())
    {
        AiNodeSetFlt(shader, "input", mnode.findPlug("value", true).asFloat());
    }
    else
    {
        AiNodeSetFlt(shader, "input", mnode.findPlug("original", true).asFloat());
    }
}

AtNode* CApplyAbsFloatOverrideTranslator::CreateArnoldNodes()
{
    return AddArnoldNode("flat");
}

void CApplyAbsFloatOverrideTranslator::Export( AtNode* shader)
{
    MFnDependencyNode mnode(this->GetMayaObject());
    if (mnode.findPlug("enabled", true).asBool())
    {
        AiNodeSetRGB(shader, "color", mnode.findPlug("value", true).asFloat(),0,0);
    }
    else
    {
        AiNodeSetRGB(shader, "color", mnode.findPlug("original", true).asFloat(),0,0);
    }
}

AtNode* CApplyAbsFloat2OverrideTranslator::CreateArnoldNodes()
{
    return AddArnoldNode("flat");
}

void CApplyAbsFloat2OverrideTranslator::Export( AtNode* shader)
{
    MFnDependencyNode mnode(this->GetMayaObject());
    if (mnode.findPlug("enabled", true).asBool())
    {
        const float2& v = mnode.findPlug("value", true).asMDataHandle().asFloat2();
        AiNodeSetRGB(shader, "color",v[0] ,v[1],0);
    }
    else
    {
        const float2& v = mnode.findPlug("original", true).asMDataHandle().asFloat2();
        AiNodeSetRGB(shader, "color", v[0],v[1],0);
    }
}

AtNode* CApplyAbsFloat3OverrideTranslator::CreateArnoldNodes()
{
    return AddArnoldNode("flat");
}

void CApplyAbsFloat3OverrideTranslator::Export( AtNode* shader)
{
    MFnDependencyNode mnode(this->GetMayaObject());
    
    if (mnode.findPlug("enabled", true).asBool())
    {
        const float3& v = mnode.findPlug("value", true).asMDataHandle().asFloat3();
        AiNodeSetRGB(shader, "color",v[0] ,v[1],v[2]);
    }
    else
    {
        const float3& v = mnode.findPlug("original", true).asMDataHandle().asFloat3();
        AiNodeSetRGB(shader, "color", v[0],v[1],v[2]);
    }
}