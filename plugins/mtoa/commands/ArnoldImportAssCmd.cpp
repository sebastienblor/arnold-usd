#include "utils/Version.h"
#include "ArnoldImportAssCmd.h"
#include "../common/UnorderedContainer.h"
#include "ArnoldNodeLinkSanitizer.h"


#include "scene/MayaScene.h"
#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
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

#include <AxFtoA.h>

#include <math.h>
#include "utils/Universe.h"
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

   // Note that we're first testing if maya.name is explicitely defined. In some case, the attribute can 
   // be hidden because we don't want it to be created in the maya node definition (eg standard_surface.normal)
   bool hidden = false;
   if (AiMetaDataGetBool(nodeEntry, paramName, "maya.hide", &hidden) && hidden)
      return MString("");
   
   return ArnoldToMayaStyle(MString(paramName));
}

MSyntax CArnoldImportAssCmd::newSyntax()
{
   MSyntax syntax;
   syntax.addFlag("f", "filename", MSyntax::kString);
   syntax.addFlag("m", "mask", MSyntax::kUnsigned);
   return syntax;
}

static const char* s_outAttrs[] = {"outColor", "outValue", "out", "message", NULL};

static bool ConnectMayaFromArnold(const MString &mayaFullAttr, AtNode *target, const unordered_map<std::string, std::string> &arnoldToMayaNames)
{
   if (target == NULL)
      return false; // shit happens...
   
   std::string targetArnoldNode = AiNodeGetName(target);
   unordered_map<std::string, std::string>::const_iterator it = arnoldToMayaNames.find(targetArnoldNode);
   if (it == arnoldToMayaNames.end())
      return false; // didn't find the connected node in my maya scene
   
   std::string targetMayaNode = it->second;

   int ind = 0;
   while(s_outAttrs[ind])
   {
      int exists = 0;
      MString fullTargetAttr = MString(targetMayaNode.c_str()) + MString(".") + MString(s_outAttrs[ind]);
      MString cmd = MString("objExists ") + fullTargetAttr;
      MGlobal::executeCommand(cmd, exists);  
      if (exists)
      {
         MString connectCmd = MString("connectAttr -f ") + fullTargetAttr + MString(" ") + mayaFullAttr;
         MGlobal::displayInfo(connectCmd);
         MGlobal::executeCommand(connectCmd);
         return true;
      }
      ind++;
   }
   return false;
}

MStatus CArnoldImportAssCmd::convertAxfToArnold(const MString axfFileName, AtUniverse* universe)
{

   MString cwd;
   MGlobal::executeCommand("workspace -q -dir ", cwd) ;
   MString tex_path =  cwd ; 
   
   // unsigned int nchars = axfFileName.numChars();
  
   AxFtoASessionStart();
   AxFtoASessionClearErrors();
   AxFtoASessionSetVerbosity(0);
   AxFtoAFile* axf_file = AxFtoAFileOpen(axfFileName.asChar());
   // TODO : Hard coding this to material index 0 in the file 
   AxFtoAMaterial *material = AxFtoAFileGetMaterial(axf_file, 0);
   if ( AxFtoASessionHasErrors()) 
   {
      return MStatus::kFailure; 
   }
   AxFtoAMaterialSetTextureFolder(material, tex_path.asChar());
   
   // TODO : Hard coding this to 1.0. Need a better way to expose this 
   AxFtoAMaterialSetUVUnitSize(material, 1.0f);
   // TODO : Hard coding this. Need a better way to expose this 
   AxFtoAMaterialSetColorSpace(material, "Rec709,E");
   
   AxFtoAMaterialSetUniverse(material, universe);
   AxFtoAMaterialSetTextureNamePrefix(material, "importAxf_");
   AxFtoAMaterialSetNodeNamePrefix(material, "importAxf_");
   AtNode* root_node = AxFtoAMaterialGetRootNode(material);
   AxFtoAMaterialWriteTextures(material);
   AxFtoAFileClose(axf_file);
   AxFtoASessionEnd();
   return MStatus::kSuccess ;
}

std::string GetShadingGroup(std::string name, unordered_map<std::string, std::string> &arnoldToMayaShadingEngines)
{
   unordered_map<std::string, std::string>::iterator it = arnoldToMayaShadingEngines.find(name);
   std::string sgName;
   if (it != arnoldToMayaShadingEngines.end())
      sgName = it->second;
   else
   {
      MString mayaName;
      // MString createCmd = MString("createNode \"shadingEngine\" -name \"") + MString(name.c_str()) + MString("\"");
      MString createCmd = MString("sets -name \"") + MString(name.c_str()) + MString("\" -renderable true -noSurfaceShader true -empty");
      
      MGlobal::executeCommand(createCmd, mayaName);
      sgName = mayaName.asChar();
      arnoldToMayaShadingEngines[name] = sgName;
   }
   return sgName;
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
   
   unsigned int mask = AI_NODE_ALL;
   // Specific objects mask to load
   if (args.isFlagSet("mask"))
      args.getFlagArgument("mask", 0, mask);

   bool universeCreated = false;

   if (!AiUniverseIsActive())
   {
      universeCreated = true;
      ArnoldUniverseBegin();
   }

   AtUniverse *universe = AiUniverse();


   unsigned int nchars = filename.numChars();
   if  (nchars > 4 && filename.substringW(nchars-4, nchars) == ".axf")
   {
      // This is an Axf File 
      if (convertAxfToArnold(filename, universe) == MStatus::kFailure)
      {
         return MStatus::kFailure;
      }
      
   }
   else if  (nchars > 5 && filename.substringW(nchars-5, nchars) == ".mtlx")
   {
      // This is a MaterialX file. Let's read it and convert it to arnold nodes
      AiMaterialxReadMaterials(universe, filename.asChar());
   }
   else
   {
      AiASSLoad(universe, filename.asChar(), mask);   
   }

   CShaderLinkSanitizer sanitizer(universe, "importAxf_");
   sanitizer.SanitizeOutputComponents();

   MString logStr("Importing file ");
   logStr += filename;

   MGlobal::displayInfo(logStr);

   MStringArray connectCmds;
   unordered_map<std::string, std::string>  arnoldToMayaNames;
   unordered_map<std::string, std::string>  arnoldToMayaShadingEngines;
   std::vector<AtNode *> nodesToConvert;

   // First Loop to create the imported nodes, and fill the map from arnold to maya nodes
   AtNodeIterator* iter = AiUniverseGetNodeIterator(universe, mask);
   while (!AiNodeIteratorFinished(iter))
   {
      AtNode* node = AiNodeIteratorGetNext(iter);
      if (node == NULL)
         continue;

      std::string nodeName = AiNodeGetName(node);
      if (nodeName == "options" || nodeName == "ai_default_reflection_shader" || nodeName == "root")
         continue;

      const AtNodeEntry *nodeEntry = AiNodeGetNodeEntry(node);
      unsigned int nodeType = AiNodeEntryGetType(nodeEntry);

      std::string nodeEntryName = AiNodeEntryGetName(nodeEntry);
      MString mayaTypeName = ArnoldToMayaStyle(MString("ai_")+MString(nodeEntryName.c_str()));

      AtString mayaNodeNameMtd;
      if (AiMetaDataGetStr(nodeEntry, NULL, "maya.name", &mayaNodeNameMtd))
         mayaTypeName = MString(mayaNodeNameMtd.c_str());

      MString mayaName;
      MString createCmd;
      if (nodeType == AI_NODE_SHADER)
         createCmd += MString("shadingNode \"") + mayaTypeName + MString("\" -name \"") + MString(nodeName.c_str()) + MString("\" -asShader");
      else
         createCmd = MString("createNode \"") + mayaTypeName + MString("\" -name \"") + MString(nodeName.c_str()) + MString("\"");

      MGlobal::executeCommand(createCmd, mayaName);
      arnoldToMayaNames[nodeName] = std::string(mayaName.asChar());
      nodesToConvert.push_back(node);
   }
   AiNodeIteratorDestroy(iter);

   // Now loop over the created nodes to set all attributes
   for (size_t i = 0; i < nodesToConvert.size(); ++i)
   {
      const AtNode *node = nodesToConvert[i];
      const AtNodeEntry *nodeEntry = AiNodeGetNodeEntry(node);

      std::string nodeName = AiNodeGetName(node);
      std::string mayaName = arnoldToMayaNames[nodeName];

      // loop over this arnold node parameters
      AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
      while (!AiParamIteratorFinished(nodeParam))
      {
         const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
         const char* paramName = AiParamGetName(paramEntry);
         std::string paramNameStr(paramName);
         if (paramNameStr == "name")
            continue;
         
         MString mayaAttrName = ArnoldToMayaAttrName(nodeEntry, paramName);
         if (mayaAttrName.length() == 0)
            continue;

         MString mayaFullAttr = MString(mayaName.c_str()) + MString(".") + mayaAttrName;
         MString attrValue(" ");
         
         // First check if the attribute is linked
         if (AiNodeIsLinked(node, paramName))
         {
            // FIXME: we should be checking for component links as well
            ConnectMayaFromArnold(mayaFullAttr, (AtNode*)AiNodeGetLink(node, paramName), arnoldToMayaNames);
         } else
         {
            bool setAttrValue = true;
            // Set the plain attribute value
            switch(AiParamGetType(paramEntry))
            {
               case AI_TYPE_BYTE:
                  attrValue += (int)AiNodeGetByte(node, paramName);
               break;
               case AI_TYPE_ENUM:
               case AI_TYPE_INT:
                  attrValue += (int)AiNodeGetInt(node, paramName);
               break;
               case AI_TYPE_UINT:
                  attrValue += (int)AiNodeGetUInt(node, paramName);
               break;
               {
               case AI_TYPE_BOOLEAN:
                  attrValue += (AiNodeGetBool(node, paramName)) ? MString("1") : MString("0");
               break;
               }
               case AI_TYPE_FLOAT:
                  attrValue += (float)AiNodeGetFlt(node, paramName);
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
               break;
               }
               {
               case AI_TYPE_VECTOR2:
                  AtVector2 col = AiNodeGetVec2(node, paramName);
                  attrValue += "-type float2 ";
                  attrValue += (float) col.x;
                  attrValue += MString(" ");
                  attrValue += (float) col.y;
               break;
               }
               {
               case AI_TYPE_STRING:
                  AtString str = AiNodeGetStr(node, paramName);
                  attrValue += "-type \"string\" \"";
                  attrValue += str.c_str();
                  attrValue += "\"";
               break;
               }
               {
               case AI_TYPE_NODE:
               case AI_TYPE_POINTER:
                  setAttrValue = false; // we won't need to set the attribute value since we're connecting it
                  ConnectMayaFromArnold(mayaFullAttr, (AtNode*)AiNodeGetPtr(node, paramName), arnoldToMayaNames);
                  
               break;
               }
               {
               case AI_TYPE_ARRAY:
                  setAttrValue = false; // value will be set for each array index here below
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
                              MString mayaArrayAttr = mayaFullAttr + attrValue;
                              ConnectMayaFromArnold(mayaArrayAttr, (AtNode*)AiArrayGetPtr(arr, i), arnoldToMayaNames);
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

            if (setAttrValue)
               MGlobal::executeCommand(MString("setAttr ") + mayaFullAttr + attrValue);
         }
      }
      AiParamIteratorDestroy(nodeParam);
      if (AiNodeLookUpUserParameter(node, "material_surface"))
      {
         AtString shadingGroup = AiNodeGetStr(node, "material_surface");
         std::string shg = GetShadingGroup(shadingGroup.c_str(), arnoldToMayaShadingEngines);
         MString connectCmd = MString("connectAttr -f ") + MString(mayaName.c_str()) + MString(".outColor ") + MString(shg.c_str()) + MString(".surfaceShader");
         // MGlobal::displayInfo(connectCmd);
         MGlobal::executeCommand(connectCmd);
      }
      if (AiNodeLookUpUserParameter(node, "material_displacement"))
      {
         AtString shadingGroup = AiNodeGetStr(node, "material_displacement");
         std::string shg = GetShadingGroup(shadingGroup.c_str(), arnoldToMayaShadingEngines);
         MString connectCmd = MString("connectAttr -f ") + MString(mayaName.c_str()) + MString(".outColor ") + MString(shg.c_str()) + MString(".displacementShader");
         // MGlobal::displayInfo(connectCmd);
         MGlobal::executeCommand(connectCmd);
      }
   }
   AiUniverseDestroy(universe);
   if (universeCreated)
      ArnoldUniverseEnd();

   return status;
}
