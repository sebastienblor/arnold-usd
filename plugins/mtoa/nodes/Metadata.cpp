#include "Metadata.h"

#include <maya/MGlobal.h>

bool HasMetadata(const AtNodeEntry *entry, const char *param, const char *name)
{
   MString command = "import mtoa.metadata as meta;meta.";
   if (param != NULL)
   {
      // parameter metadata
      command += MString("hasParamData(");
      command += MString("'") + AiNodeEntryGetName(entry) + "'";
      command += MString(", '") + param + "'";
      command += MString(", '") + name + "')";
   }
   else
   {
      // node metadata
      command += MString("hasNodeData(");
      command += MString("'") + AiNodeEntryGetName(entry) + "'";
      command += MString(", '") + name + "')";
   }
   int result;
   MStatus stat = MGlobal::executePythonCommand(command, result);
   if (stat == MS::kSuccess)
      return (bool)result;
   else
      return false;
}

MString FormatCommand(const AtNodeEntry *entry, const char *param, const char *name)
{
   MString command = "import mtoa.metadata as meta;meta.";
   if (param != NULL)
   {
      // parameter metadata
      command += MString("getParamData(");
      command += MString("'") + AiNodeEntryGetName(entry) + "'";
      command += MString(", '") + param + "'";
      command += MString(", '") + name + "')";
   }
   else
   {
      // node metadata
      command += MString("getNodeData(");
      command += MString("'") + AiNodeEntryGetName(entry) + "'";
      command += MString(", '") + name + "')";
   }
   return command;
}

AtBoolean  MAiMetaDataGetBool (const AtNodeEntry *entry, const char *param, const char *name, AtBoolean *value)
{
   if (HasMetadata(entry, param, name))
   {
      int result;
      MString command = FormatCommand(entry, param, name);
      MStatus stat = MGlobal::executePythonCommand(command, result);
      *value = (AtBoolean)result;
      return true;
   }
   else
      return AiMetaDataGetBool(entry, param, name, value);
}

AtBoolean  MAiMetaDataGetInt (const AtNodeEntry *entry, const char *param, const char *name, AtInt *value)
{
   if (HasMetadata(entry, param, name))
   {
      int result;
      MString command = FormatCommand(entry, param, name);
      MStatus stat = MGlobal::executePythonCommand(command, result);
      *value = (AtInt)result;
      return true;
   }
   else
      return AiMetaDataGetInt(entry, param, name, value);
}

AtBoolean  MAiMetaDataGetFlt (const AtNodeEntry *entry, const char *param, const char *name, AtFloat *value)
{
   if (HasMetadata(entry, param, name))
   {
      double result;
      MString command = FormatCommand(entry, param, name);
      MStatus stat = MGlobal::executePythonCommand(command, result);
      *value = (AtFloat)result;
      return true;
   }
   else
      return AiMetaDataGetFlt(entry, param, name, value);
}
//AtBoolean  MAiMetaDataGetPnt (const AtNodeEntry *entry, const char *param, const char *name, AtPoint *value);
//AtBoolean  MAiMetaDataGetVec (const AtNodeEntry *entry, const char *param, const char *name, AtVector *value);
//AtBoolean  MAiMetaDataGetPnt2 (const AtNodeEntry *entry, const char *param, const char *name, AtPoint2 *value);
//AtBoolean  MAiMetaDataGetRGB (const AtNodeEntry *entry, const char *param, const char *name, AtColor *value);
AtBoolean  MAiMetaDataGetStr (const AtNodeEntry *entry, const char *param, const char *name, const char **value)
{
   if (HasMetadata(entry, param, name))
   {
      MString result;
      MString command = FormatCommand(entry, param, name);
      MStatus stat = MGlobal::executePythonCommand(command, result);
      *value = result.asChar();
      return true;
   }
   else
      return AiMetaDataGetStr(entry, param, name, value);
}
