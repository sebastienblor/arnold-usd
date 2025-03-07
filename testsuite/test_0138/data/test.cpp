#include <ai.h>

#include <cstdio>
#include <cstring>
#include <iostream>

int main(int argc, char **argv)
{
    AiMsgSetConsoleFlags(nullptr, AI_LOG_ALL);
    AiBegin();
    AiSceneLoad(nullptr, "scene.ass", nullptr);
    AtParamValueMap* params = AiParamValueMap();
    AiParamValueMapSetStr(params, AtString("scope"), AtString("beautiful/scope"));
    AiSceneWrite(nullptr, "scene.usda", params);
    AiParamValueMapDestroy(params);
    AiEnd();

    AiBegin();
    bool success = true;
    AiSceneLoad(nullptr, "scene.usda", nullptr);
    
    if (!AiNodeLookUpByName(nullptr, AtString("/beautiful/scope/my/sphere/name"))) {
        AiMsgError("/beautiful/scope/my/sphere/name doesn't exist");
        success = false;
    }
    if (!AiNodeLookUpByName(nullptr, AtString("/beautiful/scope/my/cube/name"))) {
        AiMsgError("/beautiful/scope/my/cone/name doesn't exist");
        success = false;
    }
    if (!AiNodeLookUpByName(nullptr, AtString("/beautiful/scope/mtl/lambert1/lambert1"))) {
        AiMsgError("/beautiful/scope/lambert1 doesn't exist");
        success = false;
    }
    if (!AiNodeLookUpByName(nullptr, AtString("/beautiful/scope/persp/perspShape"))) {
        AiMsgError("/beautiful/scope/persp/perspShape doesn't exist");
        success = false;
    }
    // Filters shouldn't be affected by the scope
    if (!AiNodeLookUpByName(nullptr, AtString("defaultArnoldFilter@gaussian_filter"))) {
        AiMsgError("defaultArnoldFilter@gaussian_filter doesn't exist");
        success = false;
    }
    if (!AiNodeLookUpByName(nullptr, AtString("/beautiful/scope/directionalLight1/directionalLightShape1"))) {
        AiMsgError("/beautiful/scope/directionalLight1/directionalLightShape1 doesn't exist");
        success = false;
    }
        
    AiEnd();
    return (success) ? 0 : 1;
}
