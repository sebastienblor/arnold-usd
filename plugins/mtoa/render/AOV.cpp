#include "AOV.h"
#include <maya/MPlug.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MGlobal.h>
#include <maya/MFnRenderLayer.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MRenderUtil.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDagNode.h>
#include <string>

CAOV::CAOV()
   : m_name(""), m_type(0), m_enabled(true), m_prefix(""), m_filename("")
{
}

CAOV::CAOV(const CAOV &rhs)
   : m_name(rhs.m_name), m_type(rhs.m_type), m_enabled(rhs.m_enabled), m_prefix(rhs.m_prefix), m_filename(rhs.m_filename)
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

bool CAOV::FromMaya(MPlug &pAOV)
{
   if (pAOV.isNull())
   {
      return false;
   }

   m_name = pAOV.child(AOV_NAME).asString();
   Strip(m_name);

   if (m_name.length() == 0)
   {
      return false;
   }

   m_type = pAOV.child(AOV_TYPE).asInt();
   m_enabled = pAOV.child(AOV_ENABLED).asBool();
   m_prefix = pAOV.child(AOV_PREFIX).asString();
   Strip(m_prefix);
   NormalizePath(m_prefix);

   m_filename = "";

   return true;
}


void CAOV::SetupOutput(AtArray *outputs, int i, AtNode *defaultDriver, AtNode *defaultFilter) const
{
   // create a copy of the current driver for this AOV (this is a deep copy)
   AtNode *driver = AiNodeClone(defaultDriver);

   MString driverName = AiNodeGetStr(defaultDriver, "name");
   driverName += "_" + m_name;
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
   // for now, filter is default one
   char str[1024];
   sprintf(str, "%s %s %s %s", m_name.asChar(), AiParamGetTypeName(m_type), AiNodeGetName(defaultFilter), driverName.asChar());

   AiArraySetStr(outputs, i, str);
}

