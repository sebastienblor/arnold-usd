#pragma once

#include "platform/Platform.h"
#include <ai.h>

DLLEXPORT void AiNodeDeclareConstant(AtNode* node, const char* name, unsigned int type);
DLLEXPORT void AiNodeDeclareConstantArray(AtNode* node, const char* name, unsigned int type);
DLLEXPORT void AiNodeDeclareUniform(AtNode* node, const char* name, unsigned int type);
DLLEXPORT void AiNodeDeclareVarying(AtNode* node, const char* name, unsigned int type);
