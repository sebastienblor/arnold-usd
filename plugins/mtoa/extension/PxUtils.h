#pragma once

#include "PxMayaNode.h"
#include "PxArnoldNode.h"
#include "PxTranslator.h"

#include <string>
#include <list>
#include <set>
#include <map>


// Set of Maya node proxies
typedef AmSet<CPxMayaNode> MayaNodesSet;

// Arnold node to Maya node multimap
/// key: arnold node name. value: maya node name
typedef std::multimap<CPxArnoldNode, CPxMayaNode> ArnoldNodeToMayaNodeMap;
// Set of Translator proxies
typedef AmSet<CPxTranslator> TranslatorsSet;
// Maya node to Translator map
typedef AmMap<CPxMayaNode, TranslatorsSet> MayaNodeToTranslatorsMap;
// Storing the default translator value
typedef AmMap<std::string, MString> DefaultTranslatorMap;
