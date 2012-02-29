#include "platform/Platform.h"
#include "AOV.h"
#include "scene/MayaScene.h"

#include <maya/MPlug.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MGlobal.h>
#include <maya/MFnRenderLayer.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MRenderUtil.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>

#include <ai_universe.h>

#include <assert.h>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAOV::CAOV()
   : m_name(""),
     m_type(0),
     m_enabled(true),
     m_prefix(""),
     m_imageformat(""),
     m_object(MObject::kNullObj)
{
}

CAOV::CAOV(const CAOV &rhs)
   : m_name(rhs.m_name),
     m_type(rhs.m_type),
     m_enabled(rhs.m_enabled),
     m_prefix(rhs.m_prefix),
     m_imageformat(rhs.m_imageformat),
     m_object(rhs.m_object),
     m_drivers(rhs.m_drivers),
     m_filters(rhs.m_filters),
     m_extensions(rhs.m_extensions)
{
}

CAOV::~CAOV()
{
}

CAOV& CAOV::operator=(const CAOV &rhs)
{
   if (this != &rhs)
   {
      m_name = rhs.m_name;
      m_type = rhs.m_type;
      m_prefix = rhs.m_prefix;
      m_imageformat = rhs.m_imageformat;
      m_object = rhs.m_object;
      m_drivers = rhs.m_drivers;
      m_filters = rhs.m_filters;
      m_extensions = rhs.m_extensions;
   }
   return *this;
}

void CAOV::NormalizePath(MString &path) const
{
#ifdef _WIN32
   std::string str(path.asChar());

   size_t p0 = 0;
   size_t p1 = str.find('\\', p0);

   while (p1 != std::string::npos)
   {
      str[p1] = '/';
      p0 = p1;
      p1 = str.find('\\', p0);
   }

   path = str.c_str();
#endif
}

void CAOV::Strip(MString &mstr) const
{
   std::string str = mstr.asChar();

   size_t p0 = str.find_first_not_of(" \t\n\r");
   if (p0 != std::string::npos)
   {
      size_t p1 = str.find_last_not_of(" \t\n\r");
      mstr = str.substr(p0, p1-p0+1).c_str();
   }
   else
   {
      mstr = "";
   }
}

bool CAOV::FromMaya(MObject &AOVNode)
{
   if (AOVNode.isNull())
   {
      return false;
   }
   m_object = AOVNode;
   MFnDependencyNode fnNode;
   fnNode.setObject(AOVNode);
   m_name = fnNode.findPlug("name").asString();
   Strip(m_name);

   if (m_name.length() == 0)
   {
      return false;
   }

   m_type = fnNode.findPlug("type").asInt();
   m_enabled = fnNode.findPlug("enabled").asBool();
   m_prefix = fnNode.findPlug("prefix").asString();
   Strip(m_prefix);
   NormalizePath(m_prefix);

   m_imageformat = fnNode.findPlug("imageFormat", true).asString();

   return true;
}

void CAOV::SetImageFilename(unsigned int driverNum, const MString &filename) const
{
   AtNode* driver = m_drivers[driverNum];
   if (driver == NULL)
   {
      AiMsgError("[mtoa] [aov] Cannot set filename for \"%s\": the driver has not been created", GetName().asChar());
      return;
   }
   if (AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(driver), "filename") == NULL)
      return;

   AiNodeSetStr(driver, "filename", filename.asChar());

   if (filename != "")
   {
      // create the output directory
      int result;
      std::string outDir = filename.asChar();
      size_t p0 = outDir.find_last_of("\\/");
      if (p0 != std::string::npos)
      {
         outDir = outDir.substr(0, p0);
         // FIXME: this should be handled by MCommonRenderSettingsData.getImageName
         MGlobal::executeCommand("sysFile -makeDir \"" + MString(outDir.c_str()) + "\"", result);
      }
   }
}

void CAOV::SetupOutputs(MStringArray &result, AtNode *defaultDriver, AtNode *defaultFilter)
{
   assert(AiUniverseIsActive());

   AiMsgDebug("[mtoa] [aov %s] Setting AOV output: filter and driver.",
         m_name.asChar());

   MFnDependencyNode fnNode;
   fnNode.setObject(m_object);
   MPlugArray conn;
   MPlug outputsPlug = fnNode.findPlug("outputs", true);
   for (unsigned int i=0; i<outputsPlug.numElements(); ++i)
   {
      // Filter
      MPlug filterPlug = outputsPlug[i].child(1);
      filterPlug.connectedTo(conn, true, false);

      MString filterName;

      // FIXME: AiNodeDestroy previous node in IPR mode or track these translators
      if (!conn.length())//filterType == "<Use Globals>" || filterType == "")
      {
         // use default filter
         filterName = AiNodeGetName(defaultFilter);
         AiMsgDebug("[mtoa] [aov %s] Uses default filter %s(%s).",
            m_name.asChar(), filterName.asChar(), AiNodeEntryGetName(AiNodeGetNodeEntry(defaultFilter)));
      }
      else
      {
         AtNode* filter = CMayaScene::GetArnoldSession()->ExportNode(conn[0]);
         MString nodeTypeName = AiNodeEntryGetName(AiNodeGetNodeEntry(filter));
         filterName = nodeTypeName + "_" + m_name;
         AiNodeSetStr(filter, "name", filterName.asChar());
         AiMsgDebug("[mtoa] [aov %s] Created new filter %s(%s).",
               m_name.asChar(), AiNodeGetName(filter), AiNodeEntryGetName(AiNodeGetNodeEntry(filter)));
         m_filters.push_back(filter);
      }

      // Driver
      MPlug driverPlug = outputsPlug[i].child(0);
      driverPlug.connectedTo(conn, true, false);
      AtNode* driver;
      // FIXME: AiNodeDestroy previous node in IPR mode or track these translators
      if (!conn.length())//driverType == "<Use Globals>" || driverType == "")
      {
         // create a copy of the default driver for this AOV (this is a deep copy)
         driver = AiNodeClone(defaultDriver);
      }
      else
      {
         driver = CMayaScene::GetArnoldSession()->ExportNode(conn[0]);
      }

      MString nodeTypeName = AiNodeEntryGetName(AiNodeGetNodeEntry(driver));
      MString driverName = nodeTypeName + "_" + m_name;
      AiNodeSetStr(driver, "name", driverName.asChar());
      m_drivers.push_back(driver);
      const char* ext;
      if (AiMetaDataGetStr(AiNodeGetNodeEntry(driver), NULL, "maya.translator", &ext))
         m_extensions.append(ext);
      else
         m_extensions.append("");
      char str[1024];
      sprintf(str, "%s %s %s %s", m_name.asChar(), AiParamGetTypeName(m_type), filterName.asChar(), driverName.asChar());
      AiMsgDebug("[mtoa] [aov %s] prefix '%s' output line: %s", m_name.asChar(), m_prefix.asChar(), str);

      result.append(MString(str));
   }
}



