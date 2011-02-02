#include "Metadata.h"

#include <ai_msg.h>

#include <maya/MGlobal.h>

#include <cstring>

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
      return result ? true: false ;
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


AtBoolean  MAiMetaDataGetStr (const AtNodeEntry *entry, const char *param, const char *name, char *value)
{
   if (HasMetadata(entry, param, name))
   {
      MString result;
      MString command = FormatCommand(entry, param, name);
      MStatus stat = MGlobal::executePythonCommand(command, result);
      strcpy(value, result.asChar());
      return true;
   }
   else
   {
      const char *sptr;
      if (AiMetaDataGetStr(entry, param, name, &sptr))
      {
         // succeeded. copy in the value
         strcpy(value, sptr);
         return true;
      }
      return false;
   }
}

AtParamValue MAiParamGetDefault(const AtNodeEntry *entry, const AtParamEntry* paramEntry)
{
   const char* param = AiParamGetName(paramEntry);
   const AtParamValue* real = AiParamGetDefault(paramEntry);
   AtParamValue value;
   memcpy(&value, real, sizeof(AtParamValue));

   if (HasMetadata(entry, param, "default"))
   {
      AtInt type = AiParamGetType(paramEntry);
      MString command = FormatCommand(entry, param, "default");
      bool isArray = false;
      if (type == AI_TYPE_ARRAY)
      {
         type = value.ARRAY->type;
         isArray = true;
      }
      switch (type)
      {
         case AI_TYPE_INT:
         {
            int result;
            MStatus stat = MGlobal::executePythonCommand(command, result);
            if (isArray)
               AiArraySetInt(value.ARRAY, 0, (AtInt)result);
            else
               value.INT = (AtInt)result;
            break;
         }
         case AI_TYPE_UINT:
         {
            int result;
            MStatus stat = MGlobal::executePythonCommand(command, result);
            if (isArray)
               AiArraySetUInt(value.ARRAY, 0, (AtUInt)result);
            else
               value.UINT = (AtUInt)result;
            break;
         }
         case AI_TYPE_BOOLEAN:
         {
            int result;
            MStatus stat = MGlobal::executePythonCommand(command, result);
            if (isArray)
               AiArraySetBool(value.ARRAY, 0, (AtBoolean)(true ? result : false));
            else
               value.BOOL = (AtBoolean)(true ? result : false);
            break;
         }
         case AI_TYPE_FLOAT:
         {
            double result;
            MStatus stat = MGlobal::executePythonCommand(command, result);
            if (isArray)
               AiArraySetFlt(value.ARRAY, 0, (AtFloat)result);
            else
               value.FLT = (AtFloat)result;
            break;
         }
         default:
         {
            AiMsgError("[mtoa] plain text defaults only supports basic numeric types");
         }
      }
   }
   return value;
}
