
#ifndef MAYAUSD_ARNOLD_REGISTRY_H
#define MAYAUSD_ARNOLD_REGISTRY_H

#include <unordered_map>
#include <string>

typedef std::unordered_map<std::string, std::string> ArnoldShaderMap;

PXR_NAMESPACE_OPEN_SCOPE


const std::unordered_map<std::string, std::string> &GetArnoldUsdToMayaNodes();
const std::unordered_map<std::string, std::string> &GetMayaToArnoldUsdNodes();
const ArnoldShaderMap &GetArnoldUsdToMayaAttrs(const std::string &shader);
const ArnoldShaderMap &GetMayaToArnoldUsdAttrs(const std::string &shader);

PXR_NAMESPACE_CLOSE_SCOPE

#endif