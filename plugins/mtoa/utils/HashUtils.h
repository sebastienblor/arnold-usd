#pragma once

#include <ai.h>

inline size_t getHash(AtNode* node)
{
   return AiNodeGetStr(node, AtString("name")).hash();
}
