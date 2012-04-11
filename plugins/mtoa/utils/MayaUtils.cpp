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
                     const bool* isSequence)
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
      cmd += "Scene=" + sceneFileName + "', ";
   cmd += MString("frame=") + frameNumber + ", ";
   if (path.numChars())
      cmd += "path='" + path + "', ";
   if (fileRuleType.numChars())
      cmd += "fileType='" + fileRuleType + "', ";
   cmd += MString("createDirectory=") + (createDirectory ? "True" : "False")  + ");";
   if (isSequence != NULL)
      cmd += MString("isSequence=") + (*isSequence ? "True" : "False")  + ", ";

   return MGlobal::executePythonCommandStringResult(cmd);
}
