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

#include <ai_universe.h>

#include <assert.h>
#include <string>


CAOV::CAOV()
   : m_name(""),
     m_type(0),
     m_enabled(true),
     m_prefix(""),
     m_imageformat(""),
     m_object(MObject::kNullObj),
     m_driver(NULL),
     m_filter(NULL)
{
}

CAOV::CAOV(const CAOV &rhs)
   : m_name(rhs.m_name),
     m_type(rhs.m_type),
     m_enabled(rhs.m_enabled),
     m_prefix(rhs.m_prefix),
     m_imageformat(rhs.m_imageformat),
     m_object(rhs.m_object),
     m_driver(rhs.m_driver),
     m_filter(rhs.m_filter)
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
      m_driver = rhs.m_driver;
      m_filter = rhs.m_filter;
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

void CAOV::SetImageFilename(const MString &filename) const
{
   if (m_driver == NULL)
   {
      AiMsgError("[mtoa] [aov] Cannot set filename for \"%s\": the driver has not been created", GetName().asChar());
      return;
   }
   AiNodeSetStr(m_driver, "filename", filename.asChar());

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

MString CAOV::SetupOutput(AtNode *defaultDriver, AtNode *defaultFilter)
{
   assert(AiUniverseIsActive());

   AiMsgDebug("[mtoa] [aov %s] Setting AOV output: filter and driver.",
         m_name.asChar());

   MFnDependencyNode fnNode;
   fnNode.setObject(m_object);

   // Filter
   MString filterType = fnNode.findPlug("filterType", true).asString();
   MString filterName;

   // FIXME: AiNodeDestroy previous node in IPR mode or track these translators
   if (filterType == "<Use Globals>" || filterType == "")
   {
      // use default filter
      filterName = AiNodeGetName(defaultFilter);
      AiMsgDebug("[mtoa] [aov %s] Uses default filter %s(%s).",
         m_name.asChar(), filterName.asChar(), AiNodeEntryGetName(defaultFilter->base_node));
   }
   else
   {
      m_filter = CMayaScene::GetArnoldSession()->ExportFilter(m_object, filterType);
      MString nodeTypeName = AiNodeEntryGetName(m_filter->base_node);
      filterName = nodeTypeName + "_" + m_name;
      AiNodeSetStr(m_filter, "name", filterName.asChar());
      AiMsgDebug("[mtoa] [aov %s] Created new filter %s(%s).",
            m_name.asChar(), AiNodeGetName(m_filter), AiNodeEntryGetName(m_filter->base_node));
   }

   // Driver
   MString driverType = fnNode.findPlug("imageFormat", true).asString();

   // FIXME: AiNodeDestroy previous node in IPR mode or track these translators
   if (driverType == "<Use Globals>" || driverType == "")
   {
      // create a copy of the default driver for this AOV (this is a deep copy)
      m_driver = AiNodeClone(defaultDriver);
   }
   else
   {
      m_driver = CMayaScene::GetArnoldSession()->ExportDriver(m_object, driverType);
   }

   MString nodeTypeName = AiNodeEntryGetName(m_driver->base_node);
   MString driverName = nodeTypeName + "_" + m_name;
   AiNodeSetStr(m_driver, "name", driverName.asChar());

   char str[1024];
   sprintf(str, "%s %s %s %s", m_name.asChar(), AiParamGetTypeName(m_type), filterName.asChar(), driverName.asChar());
   AiMsgDebug("[mtoa] [aov %s] prefix '%s' output line: %s", m_name.asChar(), m_prefix.asChar(), str);

   return MString(str);
}



