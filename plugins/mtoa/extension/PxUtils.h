#ifndef PXUTILS_H
#define PXUTILS_H

#include "PxMayaNode.h"
#include "PxArnoldNode.h"
#include "PxTranslator.h"

#include <string>
#include <list>
#include <set>
#include <map>

// Existing builtin maya nodes proxys corresponding to Arnold node types
// (To store default base Maya classes per type of Arnold node)
typedef std::set<CPxMayaNode> MayaNodesSet;
// Arnold node to Maya node map
/// key: maya node name. value: arnold node name and maya node id
typedef std::map<CPxMayaNode, CPxArnoldNode> MayaNodeToArnoldNodeMap;
/// key: arnold node name. value: maya node name
typedef std::map<CPxArnoldNode, CPxMayaNode> ArnoldNodeToMayaNodeMap;
// To store the data for nodes and translators this extension defined,
// as well as their relation
// TODO : vector to know load order (last)
typedef std::set<CPxTranslator> TranslatorsSet;
typedef std::map<CPxMayaNode, TranslatorsSet> MayaNodeToTranslatorsMap;

#endif // PXUTILS_H
