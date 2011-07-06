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

#include <string>

CAOV::CAOV()
   : m_name(""),
     m_type(0),
     m_enabled(true),
     m_prefix(""),
     m_filename(""),
     m_object(MObject::kNullObj)
{
}

CAOV::CAOV(const CAOV &rhs)
   : m_name(rhs.m_name),
     m_type(rhs.m_type),
     m_enabled(rhs.m_enabled),
     m_prefix(rhs.m_prefix),
     m_filename(rhs.m_filename),
     m_object(rhs.m_object)
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
      m_filename = rhs.m_filename;
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
   MFnDependencyNode fnNode = MFnDependencyNode(AOVNode);
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

   m_filename = "";

   return true;
}


void CAOV::SetupOutput(AtArray *outputs, int i, CMayaScene *scene, AtNode *defaultDriver, AtNode *defaultFilter) const
{
   AiMsgDebug("[mtoa] [aovs] setting up aov %s", m_name.asChar());

   MFnDependencyNode fnNode = MFnDependencyNode(m_object);

   // Filter
   AtNode* filter;
   MString filterType = fnNode.findPlug("filterType").asString();
   MString filterName;
   if (filterType == "<Use Globals>")
   {
      // use default filter
      filter = defaultFilter;
      filterName = AiNodeGetStr(defaultFilter, "name");
   }
   else
   {
      filter = scene->ExportFilter(m_object, filterType);
      MString nodeTypeName = AiNodeEntryGetName(filter->base_node);
      filterName = nodeTypeName + "_" + m_name;
      AiNodeSetStr(filter, "name", filterName.asChar());
   }

   // Driver
   AtNode *driver;
   MString driverType = fnNode.findPlug("imageFormat").asString();
   if (driverType == "<Use Globals>")
      // create a copy of the default driver for this AOV (this is a deep copy)
      driver = AiNodeClone(defaultDriver);
   else
      driver = scene->ExportDriver(m_object, driverType);

   MString nodeTypeName = AiNodeEntryGetName(driver->base_node);
   MString driverName = nodeTypeName + "_" + m_name;

   AiNodeSetStr(driver, "name", driverName.asChar());
   AiNodeSetStr(driver, "filename", m_filename.asChar());

   if (m_filename != "")
   {
      // create the output directory
      int result;
      std::string outDir = m_filename.asChar();
      size_t p0 = outDir.find_last_of("\\/");
      if (p0 != std::string::npos)
      {
         outDir = outDir.substr(0, p0);
         // FIXME: this should be handled by MCommonRenderSettingsData.getImageName
         MGlobal::executeCommand("sysFile -makeDir \"" + MString(outDir.c_str()) + "\"", result);
      }
   }

   char str[1024];
   sprintf(str, "%s %s %s %s", m_name.asChar(), AiParamGetTypeName(m_type), filterName.asChar(), driverName.asChar());
   AiArraySetStr(outputs, i, str);
}

