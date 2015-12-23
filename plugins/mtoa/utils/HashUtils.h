#pragma once

#include <ai.h>

inline int getHash(AtNode* node)
{
   return AiNodeGetStr(node, AtString("name")).hash();
}
