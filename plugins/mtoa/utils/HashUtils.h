#pragma once

#include <ai.h>

inline unsigned int getHash(AtNode* node)
{
   return (unsigned int)AiNodeGetStr(node, AtString("name")).hash();
}
