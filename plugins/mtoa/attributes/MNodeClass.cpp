#include "MNodeClass.h"
#include "AttrHelper.h"

#include <maya/MGlobal.h>
#include <maya/MDGMessage.h>
#include <maya/MItDependencyNodes.h>

#if MAYA_API_VERSION < 201200

// MNodeClass

MCallbackIdArray MNodeClass::s_callbackIDs;
ExtensionAttrMap MNodeClass::s_attrs;
ExtensionAttrDataMap MNodeClass::s_attrData;

// FIXME: this method does not work
MStatus MNodeClass::addExtensionAttribute(MObject &attr) const
{
   // ensure we have a callback for this node
   AddNodeCallback(m_nodeClassName);
   // save MObjects for later
   s_attrs[m_nodeClassName.asChar()].push_back(attr);
   // we don't really know if it succeeded, but since we're trying to be MNodeClass compatible...
   return MS::kSuccess;
}

MStatus MNodeClass::addExtensionAttribute(CAttrData &data) const
{
   // ensure we have a callback for this node
   AddNodeCallback(m_nodeClassName);
   std::vector<CAttrData>& attrData = s_attrData[m_nodeClassName.asChar()];
   for (unsigned int i=0; i < attrData.size(); ++i)
   {
      if (data.name == attrData[i].name)
      {
         // skip dupes without raising a warning: probably just two translators
         // that use the same attributes
         // TODO: compare CAttrData values and raise warning when they differ
         return MS::kSuccess;
      }
   }
   // save data for later
   s_attrData[m_nodeClassName.asChar()].push_back(data);
   // we don't really know if it succeeded, but since we're trying to be MNodeClass compatible...
   return MS::kSuccess;
}

/// Add attributes to nodes that already exist in the scene
///
void MNodeClass::InitializeExistingNodes()
{
   MFnDependencyNode fnNode;
   MItDependencyNodes nodeIt;
   MStatus status;
   for (; !nodeIt.isDone(); nodeIt.next())
   {
      MObject node = nodeIt.item();
      if (node.isNull())
         continue;
      fnNode.setObject(node);
      // don't use [] access to map because that creates a new entry
      ExtensionAttrDataMap::iterator it = s_attrData.find(fnNode.typeName().asChar());
      if (it != s_attrData.end())
      {
         std::vector<CAttrData>& attrData = it->second;
         CDynamicAttrHelper helper = CDynamicAttrHelper(node);
         for (unsigned int i=0; i < attrData.size(); ++i)
         {
            CAttrData data = attrData[i];
            // only add the attribute if it does not yet exist
            fnNode.findPlug(data.name, false, &status);
            if (status == MS::kInvalidParameter)
               helper.MakeInput(data);
         }
      }
   }
}

void MNodeClass::NodeCreatedCallback(MObject &node, void *clientData)
{
   MFnDependencyNode fnNode(node);
   std::vector<CAttrData>& attrData = s_attrData[fnNode.typeName().asChar()];
   CDynamicAttrHelper helper = CDynamicAttrHelper(node);
   for (unsigned int i=0; i < attrData.size(); ++i)
   {
      CAttrData data = attrData[i];
      helper.MakeInput(data);
   }
}

MStatus MNodeClass::AddNodeCallback(const MString &nodeClassName) const
{
   // don't add if we already have a callback
   if (s_attrData.count(nodeClassName.asChar()))
      return MS::kSuccess;

   if (!MFnPlugin::isNodeRegistered(nodeClassName))
   {
      /*
      if (strlen(providedByPlugin) != 0)
      {
         // can't add the callback if the node type is unknown
         // make the callback when the plugin is loaded
         CMayaPluginData pluginData;
         pluginData.mayaNode = mayaNode;
         pluginData.nodeClassInitializer = nodeClassInitializer;
         s_mayaPluginData[providedByPlugin].push_back(pluginData);
      }
      else*/
      if (strcmp(nodeClassName.asChar(), "<driver>" ) != 0)
      {
         return MS::kFailure;
      }
      else if (strcmp(nodeClassName.asChar(), "<filter>") != 0)
      {
         return MS::kFailure;
      }
      else
      {
         AiMsgWarning("[mtoa] Cannot add a callback on %s: the node type does not exist.", nodeClassName.asChar());
         AiMsgWarning("[mtoa] If the node is provided by a plugin, load the plugin before the extension that defines a translator for that node");
         return MS::kFailure;
      }
   }
   MStatus status;
   // add a callback for creating arnold attributes
   MCallbackId id = MDGMessage::addNodeAddedCallback(MNodeClass::NodeCreatedCallback, nodeClassName, NULL, &status);
   s_callbackIDs.append(id);
   CHECK_MSTATUS(status);

   return status;
}

void MNodeClass::CreateCallbacks()
{
   /*
   MStatus status;
   // create callbacks
   s_pluginLoadedCallbackId = MSceneMessage::addStringArrayCallback(MSceneMessage::kAfterPluginLoad, 
      CExtension::MayaPluginLoadedCallback, 
      NULL, 
      &status);
   CHECK_MSTATUS(status);
   */
}

void MNodeClass::RemoveCallbacks()
{
   // delete callbacks
   const MStatus status = MMessage::removeCallbacks(s_callbackIDs);
   CHECK_MSTATUS(status);
   if (status == MS::kSuccess)
      s_callbackIDs.clear();
}

#endif
