#ifndef M_NODE_CLASS_H
#define M_NODE_CLASS_H

#include <ai_node_entry.h>

#define MNoVersionString
#define MNoPluginEntry
#include <maya/MMessage.h> // for MCallbackId
#include <maya/MCallbackIdArray.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnDependencyNode.h>

#include <string>
#include <map>
#include <vector>

// MNodeClass
//
/// Mimics the behavior of MNodeClass introduced in Maya 2012.

/// Used here for adding extension attributes on node creation

struct CAttrData;
typedef std::map<std::string, std::vector<MObject> > ExtensionAttrMap;
typedef std::map<std::string, std::vector<CAttrData> > ExtensionAttrDataMap;

class MNodeClass
{
public:
   MNodeClass (const MString &nodeClassName) :
      m_nodeClassName(nodeClassName)
   {};
   MStatus addExtensionAttribute(MObject &attr) const;
   MStatus addExtensionAttribute(CAttrData &data) const;
   static void InitializeExistingNodes();

private:
   static void NodeCreatedCallback(MObject &node, void *clientData);
   MStatus AddNodeCallback(const MString &nodeClassName) const;
   static void CreateCallbacks();
   static void RemoveCallbacks();


private:
   MString m_nodeClassName;
   static MCallbackIdArray s_callbackIDs;
   static ExtensionAttrMap s_attrs;
   static ExtensionAttrDataMap s_attrData;
};

#endif
