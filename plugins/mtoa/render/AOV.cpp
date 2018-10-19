#include "platform/Platform.h"
#include "translators/NodeTranslator.h"
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
     m_type(AI_TYPE_RGBA),
     m_enabled(true),
     m_prefix(""),
     m_imageformat(""),
     m_object(MObject::kNullObj),
     m_lightGroups(false),
     m_globalAov(true),
     m_denoise(false)
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
     m_extensions(rhs.m_extensions),
     m_lightPathExpression(rhs.m_lightPathExpression),
     m_lightGroupsList(rhs.m_lightGroupsList),
     m_lightGroups(rhs.m_lightGroups),
     m_globalAov(rhs.m_globalAov),
     m_shaderPlug(rhs.m_shaderPlug),
     m_camera(rhs.m_camera),
     m_denoise(rhs.m_denoise)
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
      m_lightPathExpression = rhs.m_lightPathExpression;
      m_lightGroups = rhs.m_lightGroups;
      m_lightGroupsList = rhs.m_lightGroupsList;
      m_globalAov = rhs.m_globalAov;
      m_shaderPlug = rhs.m_shaderPlug;
      m_camera = rhs.m_camera;
      m_denoise = rhs.m_denoise;
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
      return false;
   
   m_object = AOVNode;
   MFnDependencyNode fnNode;
   fnNode.setObject(AOVNode);

   // check if this AOV is Valid
   MString isValidCmd = MString("import mtoa.aovs;mtoa.aovs.isValidAOVNode(\"") + fnNode.name() + MString("\")");
   int isValidRes = 1;
   MGlobal::executePythonCommand(isValidCmd, isValidRes);

   if (isValidRes == 0)
      return false;

   m_name = fnNode.findPlug("name").asString();
   Strip(m_name);

   if (m_name.length() == 0)
   {
      return false;
   }
   if (m_name == "RGBA")
   {
      m_name = "beauty";
      m_type = AI_TYPE_RGBA;
      m_enabled = true;
   }
   else if (m_name == "RGB")
   {
      m_name = "beauty";
      m_type = AI_TYPE_RGB;
      m_enabled = true;
   }
   else
   {
      m_type = fnNode.findPlug("type").asInt();
      m_enabled = fnNode.findPlug("enabled").asBool();
   }
   m_prefix = fnNode.findPlug("prefix").asString();
   Strip(m_prefix);
   NormalizePath(m_prefix);

   m_imageformat = fnNode.findPlug("imageFormat", true).asString();
   m_denoise = fnNode.findPlug("denoise").asBool();

   m_lightGroups = fnNode.findPlug("lightGroups").asBool();
   m_lightPathExpression = fnNode.findPlug("lightPathExpression").asString();
   m_lightGroupsList = fnNode.findPlug("lightGroupsList").asString();
   m_globalAov = fnNode.findPlug("globalAov").asBool();

   m_shaderPlug = MPlug();
   MPlugArray connections;
   fnNode.findPlug("defaultValue").connectedTo(connections, true, false);

   if (connections.length() > 0)
      m_shaderPlug = connections[0];
   
   m_camera = fnNode.findPlug("camera").asString();

   return true;
}
