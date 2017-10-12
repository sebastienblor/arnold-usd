#include "translators.h"
#include <maya/MFnDependencyNode.h>

namespace
{
    template<typename T>
    inline void transfer(MFnDependencyNode mnode, const char* name, AtNode* node);

    template<> void transfer<bool>(MFnDependencyNode mnode, const char* name, AtNode* node){
        AiNodeSetBool(node, name, mnode.findPlug(name).asBool());
    }
    template<> void transfer<float>(MFnDependencyNode mnode, const char* name, AtNode* node){
        AiNodeSetFlt(node, name, mnode.findPlug(name).asFloat());
    }
    template<> void transfer<float2>(MFnDependencyNode mnode, const char* name, AtNode* node){
        const float2& v = mnode.findPlug(name).asMDataHandle().asFloat2();
        AiNodeSetVec2(node, name, v[0], v[1]);
    }
    template<> void transfer<float3>(MFnDependencyNode mnode, const char* name, AtNode* node){
        const float3& v = mnode.findPlug(name).asMDataHandle().asFloat3();
        AiNodeSetVec(node, name, v[0], v[1], v[2]);
    }
    template<> void transfer<int>(MFnDependencyNode mnode, const char* name, AtNode* node){
        AiNodeSetInt(node, name, mnode.findPlug(name).asInt());
    }
}

template<> AtNode* CApplyAbsOverrideTranslator<float>::CreateArnoldNodes()  { return AiNode("applyAbsFloatOverride");   }
template<> AtNode* CApplyAbsOverrideTranslator<float2>::CreateArnoldNodes() { return AiNode("applyAbs2FloatsOverride"); }
template<> AtNode* CApplyAbsOverrideTranslator<float3>::CreateArnoldNodes() { return AiNode("applyAbs3FloatsOverride"); }
template<> AtNode* CApplyAbsOverrideTranslator<int>::CreateArnoldNodes()    { return AiNode("applyAbsIntOverride");     }

template<> AtNode* CApplyRelOverrideTranslator<float>::CreateArnoldNodes()  { return AiNode("applyRelFloatOverride");   }
template<> AtNode* CApplyRelOverrideTranslator<float2>::CreateArnoldNodes() { return AiNode("applyRel2FloatsOverride"); }
template<> AtNode* CApplyRelOverrideTranslator<float3>::CreateArnoldNodes() { return AiNode("applyRel3FloatsOverride"); }
template<> AtNode* CApplyRelOverrideTranslator<int>::CreateArnoldNodes()    { return AiNode("applyRelIntOverride");     }

template<typename T>
void CApplyAbsOverrideTranslator<T>::Export(AtNode* node){
    MFnDependencyNode mnode(this->GetMayaObject());
    transfer<T>(mnode, "original", node);
    transfer<T>(mnode, "value", node);
    transfer<bool>(mnode, "enabled", node);
}

template<typename T>
void CApplyRelOverrideTranslator<T>::Export(AtNode* node){
    MFnDependencyNode mnode(this->GetMayaObject());
    transfer<T>(mnode, "original", node);
    transfer<T>(mnode, "multiply", node);
    transfer<T>(mnode, "offset",   node);
    transfer<bool>(mnode, "enabled", node);
}
