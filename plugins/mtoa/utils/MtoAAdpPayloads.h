#pragma once 
#include "AiAdpPayload.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "platform/Platform.h"

namespace MtoAADPPayloads
{
DLLEXPORT    void ADPPostShaderUsed(const std::string shader_name);
DLLEXPORT    void ADPPostProductMetadata();
}