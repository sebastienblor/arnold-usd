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
   : m_name(""), m_type(""), m_prefix(""), m_filename("")
{
}

CAOV::CAOV(const CAOV &rhs)
   : m_name(rhs.m_name), m_type(rhs.m_type), m_prefix(rhs.m_prefix), m_filename(rhs.m_filename)
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

void CAOV::ReturnType(MString &mtype) const
{
   switch (mtype.asInt())
   {
   case 0:
      mtype.set("FLOAT");
      break;
   case 1:
      mtype.set("RGB");
      break;
   case 2:
      mtype.set("RGBA");
      break;
   case 3:
      mtype.set("VECTOR");
      break;
   }
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

   m_name = pAOV.child(0).asString();
   Strip(m_name);

   if (m_name.length() == 0)
   {
      return false;
   }

   m_type = pAOV.child(1).asInt();
   ReturnType(m_type);


   m_prefix = pAOV.child(2).asString();
   Strip(m_prefix);
   NormalizePath(m_prefix);

   m_filename = "";

   return true;
}

void CAOV::UpdateImageFilename(const MString &cameraName, const MString &defaultExt, bool multiCam, bool batchMode) 
{
   MSelectionList list;
   MObject node;
   MString sceneFileName, nameCamera;
   MObject renderLayer = MFnRenderLayer::defaultRenderLayer();     
   MCommonRenderSettingsData renderGlobals;
   double fileFrameNumber;

   MString oldPrefix = MGlobal::executeCommandStringResult("getAttr defaultRenderGlobals.imageFilePrefix");
   NormalizePath(oldPrefix);

   // get the frame number
   MTime cT = MAnimControl::currentTime();
   fileFrameNumber = double(cT.value());
   MGlobal::executeCommand("basename((`file -q -sceneName -shortName`),(\".\" + (fileExtension((`file -q -sceneName -shortName`)))))", sceneFileName);

   if (m_prefix.length() != 0)
   {
      MGlobal::executeCommand("setAttr -type \"string\" defaultRenderGlobals.imageFilePrefix \"" + m_prefix + "\"");
   }
   else
   {
      MGlobal::executeCommand("setAttr -type \"string\" defaultRenderGlobals.imageFilePrefix \"" + oldPrefix + "." + m_name + "\"");
   }

   if (batchMode)
   {
      list.add(cameraName);
      list.getDependNode(0, node);

      MFnDagNode camDag(node);
      node = camDag.parent(0);
      camDag.setObject(node);
      nameCamera = camDag.name();

      if (multiCam)
      {
         nameCamera = sceneFileName + "/" + nameCamera;
      }

      if (oldPrefix.length() == 0)
      {
         MGlobal::executeCommand("setAttr -type \"string\" defaultRenderGlobals.imageFilePrefix \"" + sceneFileName + "." + m_name + "\"");
      }

      MRenderUtil::getCommonRenderSettings(renderGlobals);

      m_filename = renderGlobals.getImageName(renderGlobals.kFullPathImage,
                                              fileFrameNumber,
                                              sceneFileName,
                                              nameCamera,
                                              defaultExt,
                                              renderLayer,
                                              1);
   }
   else
   {
      if (oldPrefix.length() == 0)
      {
         MGlobal::executeCommand("setAttr -type \"string\" defaultRenderGlobals.imageFilePrefix \"" + sceneFileName + "." + m_name + "\"");
      }
   
      MRenderUtil::getCommonRenderSettings(renderGlobals);

      m_filename = renderGlobals.getImageName(renderGlobals.kFullPathTmp,
                                              fileFrameNumber,
                                              sceneFileName,
                                              nameCamera,
                                              defaultExt,
                                              renderLayer,
                                              1);
   }

   // restore old prefix
   MGlobal::executeCommand("setAttr -type \"string\" defaultRenderGlobals.imageFilePrefix \"" + oldPrefix + "\"");
}

void CAOV::SetupOutput(AtArray *outputs, int i, AtNode *defaultDriver, AtNode *defaultFilter) const
{
   AtNode *driver = AiNode(AiNodeEntryGetName(defaultDriver->base_node));

   MString driverName = AiNodeGetStr(defaultDriver, "name");
   driverName += "_" + m_name;
   AiNodeSetStr(driver, "name", driverName.asChar());

   if (m_filename != "")
   {
      int result;
      std::string outDir = m_filename.asChar();
      size_t p0 = outDir.find_last_of("\\/");
      if (p0 != std::string::npos)
      {
         outDir = outDir.substr(0, p0);
         MGlobal::executeCommand("sysFile -makeDir \"" + MString(outDir.c_str()) + "\"", result);
      }
   }
   AiNodeSetStr(driver, "filename", m_filename.asChar());

   // for now, copy defaultDriver attributes
   if (AiNodeEntryLookUpParameter(driver->base_node, "compression"))
   {
      AiNodeSetInt(driver, "compression", AiNodeGetInt(defaultDriver, "compression"));
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "half_precision"))
   {
      AiNodeSetBool(driver, "half_precision", AiNodeGetBool(defaultDriver, "half_precision"));
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "output_padded"))
   {
      AiNodeSetBool(driver, "output_padded", AiNodeGetBool(defaultDriver, "output_padded"));
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "gamma"))
   {
      AiNodeSetFlt(driver, "gamma", AiNodeGetFlt(defaultDriver, "gamma"));
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "quality"))
   {
      AiNodeSetInt(driver, "quality", AiNodeGetInt(defaultDriver, "quality"));
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "format"))
   {
      AiNodeSetInt(driver, "format", AiNodeGetInt(defaultDriver, "format"));
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "tiled"))
   {
      AiNodeSetBool(driver, "tiled", AiNodeGetBool(defaultDriver, "tiled"));
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "unpremult_alpha"))
   {
      AiNodeSetBool(driver, "unpremult_alpha", AiNodeGetBool(defaultDriver, "unpremult_alpha"));
   }

   // for now, filter is default one
   char str[1024];
   sprintf(str, "%s %s %s %s", m_name.asChar(), m_type.asChar(), AiNodeGetName(defaultFilter), driverName.asChar());

   AiArraySetStr(outputs, i, str);
}

