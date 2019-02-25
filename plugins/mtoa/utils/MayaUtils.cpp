#include "MayaUtils.h"
#include <maya/MGlobal.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MFnDependencyNode.h>
#include <ai_constants.h>

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
                     const bool* strictAOVs,
                     const MString& eye)
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
   if (eye.numChars())
      cmd+= " Eye="+eye+"";
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

MString replaceInString(const MString& source, const MString& origString, const MString& toReplace)
{
   if (origString == toReplace)
      return source;
   MString ret = source;
   int index = ret.indexW(origString);
   for (;index != -1; index = ret.indexW(origString))
      ret = ret.substringW(0, index) + toReplace + ret.substringW(index + origString.numChars(), ret.numChars());
   return ret;
}

MString resolveFilePathForSequences(const MString& sourceFileName, const int frameNumber)
{

    MString newFilename = "";
    int start, end  = 0;
    char frameExt[64];

    start = sourceFileName.index('#');
    end = sourceFileName.rindex('#');

    if(start >= 0)
    {
        sprintf(frameExt, "%0*d", end - start + 1, frameNumber);
        newFilename = sourceFileName.substring(0,start-1) + frameExt + sourceFileName.substring(end+1,sourceFileName.length());
    }
    else
    {
        newFilename = sourceFileName;
    }

    return newFilename.expandEnvironmentVariablesAndTilde();

}


bool IsFloatAttrDefault(MPlug plug, float value)
{
   if (plug.isNull())
      return true;

   MPlugArray connections;
   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
      return false; 
   float plugValue = plug.asFloat();
   if (std::abs(plugValue - value) > AI_EPSILON)
      return false; 

   return true;
}
bool IsBoolAttrDefault(MPlug plug, bool value)
{
   if (plug.isNull())
      return true;

   MPlugArray connections;
   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
      return false; 
   bool plugValue = plug.asBool();
   
   return (plugValue == value);
}

bool IsVec2AttrDefault(MPlug plug, float valueX, float valueY)
{
   if (plug.isNull())
      return true;

   MPlugArray connections;
   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
      return false; 

   return (IsFloatAttrDefault(plug.child(0), valueX) &&
           IsFloatAttrDefault(plug.child(1), valueY));

}

bool IsRGBAttrDefault(MPlug plug, float valueR, float valueG, float valueB)
{
   if (plug.isNull())
      return true;

   MPlugArray connections;
   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
      return false; 

   return (IsFloatAttrDefault(plug.child(0), valueR) &&
           IsFloatAttrDefault(plug.child(1), valueG) &&
           IsFloatAttrDefault(plug.child(1), valueB));

}
