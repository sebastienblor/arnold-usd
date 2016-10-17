#pragma once

#include "PxMayaNode.h"
#include "PxArnoldNode.h"
#include "PxTranslator.h"

#include <string>
#include <list>
#include <set>
#include <map>


// Set of Maya node proxies
typedef unordered_set<CPxMayaNode> MayaNodesSet;

// Arnold node to Maya node multimap
/// key: arnold node name. value: maya node name
typedef std::multimap<CPxArnoldNode, CPxMayaNode> ArnoldNodeToMayaNodeMap;
// Set of Translator proxies
typedef unordered_set<CPxTranslator> TranslatorsSet;
// Maya node to Translator map. FIXME, to be merged with the following typedef and use unordered_map
// once we understand why there are fails when the StereoRigCamera is loaded before the Camera in this iterator
typedef std::map<CPxMayaNode, TranslatorsSet> MayaNodeToTranslatorsOldMap;
// Maya node to Translator unordered_map
typedef unordered_map<CPxMayaNode, TranslatorsSet> MayaNodeToTranslatorsMap;
// Storing the default translator value
typedef unordered_map<std::string, MString> DefaultTranslatorMap;
