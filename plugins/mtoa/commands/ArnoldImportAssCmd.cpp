#include "utils/Version.h"
#include "ArnoldImportAssCmd.h"
#include "scene/MayaScene.h"
#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>
#include <ai_bbox.h>

#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <maya/MRenderUtil.h>
#include <maya/MFileIO.h>
#include <maya/MFileObject.h>
#include <maya/MFnRenderLayer.h>
#include <maya/MAnimControl.h>
#include <maya/MBoundingBox.h>

#include <math.h>

// These functions were copied from AttrHelper.cpp
MString ArnoldToMayaStyle(MString s)
{
   MString name;
   bool capitalize = false;
   MString c;
   for (unsigned int i=0; i < s.numChars(); i++)
   {
      c = s.substringW(i, i);
      if (c == "_")
      {
         capitalize = true;
      }
      else if (capitalize)
      {
         c = c.toUpperCase();
         name += c;
         capitalize = false;
      }
      else if (c.length() > 0)
      {
         // always go to lower case
         // this avoids ugly things like GI_diffuse_samples --> GIDiffuseSamples
         // and instead produces the slightly nicer giDiffuseSamples
         // TODO : but then ai_remapColor will yield aiRemapcolor
         // name += c.toLowerCase();
         name = name + c;
      }
   }
   return name;
}

static MString ArnoldToMayaAttrName(const AtNodeEntry *nodeEntry, const char* paramName) 
{
   AtString attrName;
   if (AiMetaDataGetStr(nodeEntry, paramName, "maya.name", &attrName))
   {
      MString attrNameStr(attrName.c_str());
      return attrNameStr;
   }
   else
   {
      return ArnoldToMayaStyle(MString(paramName));
   }
}

MSyntax CArnoldImportAssCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("f", "filename", MSyntax::kString);
   return syntax;
}

MStatus CArnoldImportAssCmd::doIt(const MArgList& argList)
{
   MStatus status;

   // Initialize command syntax and get flags
   MSyntax syntax = newSyntax();
   // we must use an MArgParser because MArgList is not python compatible, and we
   // use this result to set syntax.useSelectionAsDefault() prior to creating the MArgDatabase.
   MArgParser args(syntax, argList, &status);
   MString filename;
   if (args.isFlagSet("filename"))
      args.getFlagArgument("filename", 0, filename);
   
   bool universeCreated = false;

   if (!AiUniverseIsActive())
   {
      universeCreated = true;
      AiBegin();
   }  
   AtUniverse *universe = AiUniverse();
   AiASSLoad(filename.asChar(), AI_NODE_ALL, universe);

   int foundUnsupported = 0;

   MStringArray connectCmds;

   // First Loop to create the imported nodes
   AtNodeIterator* iter = AiUniverseGetNodeIterator(AI_NODE_ALL, universe);         
   while (!AiNodeIteratorFinished(iter))
   {
      AtNode* node = AiNodeIteratorGetNext(iter);
      if (node == NULL)
         continue;

      std::string nodeName = AiNodeGetName(node);
      if (nodeName == "options" || nodeName == "ai_default_reflection_shader" || nodeName == "root")
         continue;

      const AtNodeEntry *nodeEntry = AiNodeGetNodeEntry(node);
      if (AiNodeEntryGetType(nodeEntry) != AI_NODE_OPERATOR)
      {
         foundUnsupported++;
         continue;
      }
      
      std::string nodeEntryName = AiNodeEntryGetName(nodeEntry);
      MString mayaTypeName = ArnoldToMayaStyle(MString("ai_")+MString(nodeEntryName.c_str()));

      AtString mayaNodeNameMtd;
      if (AiMetaDataGetStr(nodeEntry, NULL, "maya.name", &mayaNodeNameMtd))
         mayaTypeName = MString(mayaNodeNameMtd.c_str());

      MString mayaName;
      MString createCmd = MString("createNode \"") + mayaTypeName + MString("\" -name \"") + MString(nodeName.c_str()) + MString("\";");
      MGlobal::executeCommand(createCmd, mayaName);

      // loop over this arnold node parameters
      AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
      while (!AiParamIteratorFinished(nodeParam))
      {
         const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
         const char* paramName = AiParamGetName(paramEntry);
         std::string paramNameStr(paramName);
         if (paramNameStr == "name" /* || paramNameStr == "inputs"*/)
            continue;
         
         MString mayaAttrName = ArnoldToMayaAttrName(nodeEntry, paramName);

         MString mayaFullAttr = mayaName + MString(".") + mayaAttrName;

         MString attrValue(" ");
         
         switch(AiParamGetType(paramEntry))
         {
            case AI_TYPE_BYTE:
               attrValue += (int)AiNodeGetByte(node, paramName);
               MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
            break;
            case AI_TYPE_ENUM:
            case AI_TYPE_INT:
               attrValue += (int)AiNodeGetInt(node, paramName);
               MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
            break;
            case AI_TYPE_UINT:
               attrValue += (int)AiNodeGetUInt(node, paramName);
               MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
            break;
            {
            case AI_TYPE_BOOLEAN:
               attrValue += (AiNodeGetBool(node, paramName)) ? MString("1") : MString("0");
               MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
            break;
            }
            case AI_TYPE_FLOAT:
               attrValue += (float)AiNodeGetFlt(node, paramName);
               MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
            break;
            {
            case AI_TYPE_RGB:
               AtRGB col = AiNodeGetRGB(node, paramName);
               attrValue += "-type float3 ";
               attrValue += (float) col.r;
               attrValue += MString(" ");
               attrValue += (float) col.g;
               attrValue += MString(" ");
               attrValue += (float) col.b;
               MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
            break;
            }
            {
            case AI_TYPE_RGBA:
               AtRGBA col = AiNodeGetRGBA(node, paramName);
               attrValue += "-type float3 ";
               attrValue += (float) col.r;
               attrValue += MString(" ");
               attrValue += (float) col.g;
               attrValue += MString(" ");
               attrValue += (float) col.b;
               MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
            break;
            }
            {
            case AI_TYPE_VECTOR:
               AtVector col = AiNodeGetVec(node, paramName);
               attrValue += "-type float3 ";
               attrValue += (float) col.x;
               attrValue += MString(" ");
               attrValue += (float) col.y;
               attrValue += MString(" ");
               attrValue += (float) col.z;
               MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
            break;
            }
            {
            case AI_TYPE_VECTOR2:
               AtVector2 col = AiNodeGetVec2(node, paramName);
               attrValue += "-type float2 ";
               attrValue += (float) col.x;
               attrValue += MString(" ");
               attrValue += (float) col.y;
               MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
            break;
            }
            {
            case AI_TYPE_STRING:
               AtString str = AiNodeGetStr(node, paramName);
               attrValue += "-type \"string\" \"";
               attrValue += str.c_str();
               attrValue += "\"";
               MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
            break;
            }
            {
            case AI_TYPE_NODE:
            case AI_TYPE_POINTER:
            // FIXME we're assuming that the created maya node has the same name than the arnold one.
            // But if a node already exists in the scene with this name, it will fail. We need to do this 
            // in 2 steps, and store a map for Maya-to-Arnold names.
               AtNode *ptr = (AtNode*)AiNodeGetPtr(node, paramName);
               if (ptr)
               {
                  MString connectCmd("connectAttr -f ");
                  connectCmd += MString(AiNodeGetName(ptr)) + MString(".out ");
                  connectCmd += mayaFullAttr + attrValue;
                  connectCmds.append(connectCmd);
               }
            break;
            }
            {
            case AI_TYPE_ARRAY:
               AtArray *arr = AiNodeGetArray(node, paramName);
               if (arr)
               {
                  unsigned int arrElems = AiArrayGetNumElements(arr);
                  for (unsigned int i = 0; i < arrElems; ++i)
                  {
                     attrValue = MString("[");
                     attrValue += int(i);
                     attrValue += MString("] ");
                     switch(AiArrayGetType(arr))
                     {
                        case AI_TYPE_BYTE:
                           attrValue += (int)AiArrayGetByte(arr, i);
                           MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
                        break;
                        case AI_TYPE_ENUM:
                        case AI_TYPE_INT:
                           attrValue += (int)AiArrayGetInt(arr, i);
                           MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
                        break;
                        case AI_TYPE_UINT:
                           attrValue += (int)AiArrayGetUInt(arr, i);
                           MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
                        break;
                        case AI_TYPE_BOOLEAN:
                           attrValue += (AiArrayGetBool(arr, i)) ? MString("1") : MString("0");
                           MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
                        break;
                        case AI_TYPE_FLOAT:
                           attrValue += (float)AiArrayGetFlt(arr, i);
                           MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
                        break;
                        {
                        case AI_TYPE_RGB:
                           AtRGB col = AiArrayGetRGB(arr, i);
                           attrValue += "-type float3 ";
                           attrValue += (float) col.r;
                           attrValue += MString(" ");
                           attrValue += (float) col.g;
                           attrValue += MString(" ");
                           attrValue += (float) col.b;
                           MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
                        break;
                        }
                        {
                        case AI_TYPE_RGBA:
                           AtRGBA col = AiArrayGetRGBA(arr, i);
                           attrValue += "-type float3 ";
                           attrValue += (float) col.r;
                           attrValue += MString(" ");
                           attrValue += (float) col.g;
                           attrValue += MString(" ");
                           attrValue += (float) col.b;
                           MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
                        break;
                        }
                        {
                        case AI_TYPE_VECTOR:
                           AtVector vec = AiArrayGetVec(arr, i);
                           attrValue += "-type float3 ";
                           attrValue += (float) vec.x;
                           attrValue += MString(" ");
                           attrValue += (float) vec.y;
                           attrValue += MString(" ");
                           attrValue += (float) vec.z;
                           MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
                        break;
                        }
                        {
                        case AI_TYPE_VECTOR2:
                           AtVector2 vec = AiArrayGetVec2(arr, i);
                           attrValue += "-type float2 ";
                           attrValue += (float) vec.x;
                           attrValue += MString(" ");
                           attrValue += (float) vec.y;
                           MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
                        break;
                        }
                        {
                        case AI_TYPE_STRING:
                           AtString str = AiArrayGetStr(arr, i);
                           attrValue += "-type \"string\" \"";
                           attrValue += str.c_str();
                           attrValue += "\"";
                           MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
                        break;
                        }
                        {
                        case AI_TYPE_NODE:
                        case AI_TYPE_POINTER:  
                           AtNode *ptr = (AtNode*)AiArrayGetPtr(arr, i);
                           MString connectCmd("connectAttr -f ");
                           connectCmd += MString(AiNodeGetName(ptr)) + MString(".out ");
                           connectCmd += mayaFullAttr + attrValue;
                           connectCmds.append(connectCmd);
                        break;
                        }
                        case AI_TYPE_MATRIX:
                        case AI_TYPE_CLOSURE:
                        case AI_TYPE_USHORT:
                        case AI_TYPE_HALF:
                        default:
                        break;

                     }
                  }
               }
            break;
            }
            case AI_TYPE_MATRIX:
            case AI_TYPE_CLOSURE:
            case AI_TYPE_USHORT:
            case AI_TYPE_HALF:
            default:
            break;
         }
      }
      AiParamIteratorDestroy(nodeParam);
   }
   AiNodeIteratorDestroy(iter);

   for (unsigned int i = 0; i < connectCmds.length(); ++i)
      MGlobal::executeCommand(connectCmds[i]);

   if (foundUnsupported > 0)
      AiMsgError("[mtoa.import] Only operators are currently supported for import");

   AiUniverseDestroy(universe);

   if (universeCreated)
      AiEnd();
    return status;
}
