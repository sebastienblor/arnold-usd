#include "MayaUtils.h"
#include <maya/MGlobal.h>
#include <maya/MFnDependencyNode.h>

MString getFileName(MCommonRenderSettingsData::MpathType pathType,
                     double frameNumber,
                     const MString& sceneFileName,
                     const MString& cameraName,
                     const MString& fileFormat,
                     const MObject& layer,
                     const MString& customTokenString,
                     bool createDirectory,
                     const MString& fileRuleType,
                     const MString& path,
                     const bool* isSequence,
                     const bool* strictAOVs)
{
   MString cmd = "import mtoa.utils;mtoa.utils.getFileName(";
   cmd += pathType;
   // tokens
   cmd += ", '" + customTokenString + " ";
   MString renderLayer = MFnDependencyNode(layer).name();
   if (renderLayer.numChars())
      cmd += "RenderLayer=" + renderLayer + " ";
   if (cameraName.numChars())
      cmd += "Camera=" + cameraName + " ";
   if (fileFormat.numChars())
      cmd += "Extension=" + fileFormat + " ";
   if (sceneFileName.numChars())
      cmd += "Scene=\"" + sceneFileName + "\"";
   cmd += "', ";
   cmd += MString("frame=") + frameNumber + ", ";
   if (path.numChars())
      cmd += "path=r'" + path + "', ";
   if (fileRuleType.numChars())
      cmd += "fileType='" + fileRuleType + "', ";
   if (isSequence != NULL)
      cmd += MString("isSequence=") + (*isSequence ? "True" : "False")  + ", ";
   if (strictAOVs != NULL)
      cmd += MString("strictAOVs=") + (*strictAOVs ? "True" : "False")  + ", ";
   cmd += MString("createDirectory=") + (createDirectory ? "True" : "False")  + ");";

   return MGlobal::executePythonCommandStringResult(cmd);
}

MStringArray getSourceImagesPath()
{
   MStringArray res;
   MGlobal::executePythonCommand("import mtoa.utils;mtoa.utils.getSourceImagesDir()", res);
   return res;
}

MString getProjectFolderPath()
{
   MString res;
   MGlobal::executePythonCommand("import maya.cmds as cmds;cmds.workspace(query=True, rootDirectory=True)", res);
   return res;
}
