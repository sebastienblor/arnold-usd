#include "utils/Version.h"
#include "ArnoldImportAssCmd.h"
#include "../common/UnorderedContainer.h"
#include "ArnoldNodeLinkSanitizer.h"


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

#ifdef ENABLE_AXFTOA
#include <AxFtoA.h>
#endif

#include <math.h>
#include "utils/Universe.h"
#include "utils/ConstantStrings.h"
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

static MString ArnoldToMayaAttrName(const AtNodeEntry *nodeEntry, const AtString &paramName) 
{   
   AtString attrName;
   if (AiMetaDataGetStr(nodeEntry, paramName, str::maya_name, &attrName))
   {
      MString attrNameStr(attrName.c_str());
      return attrNameStr;
   }

   // Note that we're first testing if maya.name is explicitely defined. In some case, the attribute can 
   // be hidden because we don't want it to be created in the maya node definition (eg standard_surface.normal)
   bool hidden = false;
   if (AiMetaDataGetBool(nodeEntry, paramName, str::maya_hide, &hidden) && hidden)
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

static bool ConnectMayaFromArnold(const MString &mayaFullAttr, const MString connected_output, AtNode *target, const unordered_map<std::string, std::string> &arnoldToMayaNames)
{
   if (target == NULL)
      return false; // shit happens...
   
   std::string targetArnoldNode = AiNodeGetName(target);
   unordered_map<std::string, std::string>::const_iterator it = arnoldToMayaNames.find(targetArnoldNode);
   if (it == arnoldToMayaNames.end())
      return false; // didn't find the connected node in my maya scene

   std::string targetMayaNode = it->second;
   MString fullTargetAttr;
   // If this is multiple outputs 
   if(connected_output.length() >0 ) 
   {
      fullTargetAttr = MString(targetMayaNode.c_str()) + MString(".") + connected_output;
   }
   else
   {
      int ind = 0;
      while(s_outAttrs[ind])
      {
         int exists = 0;
         MString attr = MString(targetMayaNode.c_str()) + MString(".") + MString(s_outAttrs[ind]);
         MString cmd = MString("objExists ") + attr;
         MGlobal::executeCommand(cmd, exists);
         if (exists) {
            fullTargetAttr = attr;
            break;
         }
         ind++;
      }
   }
   MString connectCmd = MString("connectAttr -f ") + fullTargetAttr + MString(" ") + mayaFullAttr;
   MGlobal::displayInfo(connectCmd);
   MGlobal::executeCommand(connectCmd);
   return true;
}

MStatus CArnoldImportAssCmd::convertAxfToArnold(const MString axfFileName, AtUniverse* universe)
{

   MString cwd;
   MGlobal::executeCommand("workspace -q -dir ", cwd) ;
   MString tex_path =  cwd ; 
   
   // unsigned int nchars = axfFileName.numChars();
#ifdef ENABLE_AXFTOA 
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
   AxFtoAMaterialWriteTextures(material);
   AxFtoAFileClose(axf_file);
   AxFtoASessionEnd();
   return MStatus::kSuccess ;
#else
   MGlobal::displayError("[mtoa] Axf shader not enabled in this build");
   return MStatus::kFailure;
#endif
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
std::string ConvertStringAttribute(const AtNode *node, const char *paramName)
{
   // We need to protect some characters as we'll end up running a setAttr
   // MEL command
   std::string str = std::string(AiNodeGetStr(node, AtString(paramName)));
   std::string::size_type i = str.find('\\');
   while (i != std::string::npos)
   {
       str.replace(i, 1, "\\\\");
       i = str.find('\\', i+2);
   }
   i = str.find("\n");
   while (i != std::string::npos)
   {
       str.replace(i, 1, "\\n");
       i = str.find("\n", i+2);
   }
   i = str.find("\"");
   while (i != std::string::npos)
   {
       str.replace(i, 1, "\\\"");
       i = str.find("\"", i+2);
   }
   return str;
}

std::map<uint8_t, std::vector<std::string>> param_components = 
{
   {AI_TYPE_RGB,     {"r", "g", "b"}},
   {AI_TYPE_RGBA,    {"r", "g", "b", "a"}},
   {AI_TYPE_VECTOR,  {"x", "y", "z"}},
   {AI_TYPE_VECTOR2, {"x", "y"}}
};

std::string getComponentName(uint8_t param_type, int component_idx)
{
   if (component_idx < 0)
      return "";

   auto find_param = param_components.find(param_type);
   if (find_param != param_components.end())
   {
      auto components = find_param->second;
      return components.at(component_idx);
   }

   return "";
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
      AtParamValueMap* params = AiParamValueMap();
      AiParamValueMapSetInt(params, AtString("mask"), mask);
      AiSceneLoad(universe, filename.asChar(), params);
      AiParamValueMapDestroy(params);
   }

   CShaderLinkSanitizer sanitizer(universe, "importArnold_");
   sanitizer.SanitizeOutputComponents();

   MString logStr("Importing file ");
   logStr += filename;

   MGlobal::displayInfo(logStr);

   MStringArray connectCmds;
   unordered_map<std::string, std::string>  arnoldToMayaNames;
   unordered_map<std::string, std::string>  arnoldToMayaShadingEngines;
   std::vector<const AtNode*> imagersList;
   unordered_set<const AtNode*>  referencedImagers;
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

      // Exception for osl shaders, the AtNodeEntry that we receive is different from the "builtin" one (#4334)
      // It doesn't have any of the metadatas we'll query below. Let's switch to using the good one.
      if (nodeEntryName == "osl")
         nodeEntry = AiNodeEntryLookUp(str::osl);
      
      MString mayaTypeName = ArnoldToMayaStyle(MString("ai_")+MString(nodeEntryName.c_str()));

      AtString mayaNodeNameMtd;
      if (AiMetaDataGetStr(nodeEntry, AtString(), str::maya_name, &mayaNodeNameMtd))
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
      
      bool isOsl = AiNodeIs(node, str::osl);
      if (isOsl)
      {
         // OSL is special, we need to set its attribute "code" first.
         // Then we need to go through the same code as when we decide
         // to compile the code, through the attribute editor. This will 
         // create all the maya node attributes corresponding to the ones 
         // declared in the OSL code. Only after this, we can iterate over the attributes
         // to convert them to Maya.
         std::string str = ConvertStringAttribute(node, "code");
         MString cmd = "setAttr " + mayaName + MString(".code ") ;
         cmd += "-type \"string\" \"";
         cmd += str.c_str();
         cmd += "\"";
         MGlobal::executeCommand(cmd);
         MString pythonCmd = MString("import mtoa.osl;mtoa.osl.OSLSceneModel(\"");
         pythonCmd += MString(str.c_str());
         pythonCmd += MString("\",\"");
         pythonCmd += mayaName;
         pythonCmd += "\")";
         MGlobal::executePythonCommand(pythonCmd);
      }
   }
   AiNodeIteratorDestroy(iter);

   // Now loop over the created nodes to set all attributes
   for (size_t i = 0; i < nodesToConvert.size(); ++i)
   {
      const AtNode *node = nodesToConvert[i];
      const AtNodeEntry *nodeEntry = AiNodeGetNodeEntry(node);

      std::string nodeName = AiNodeGetName(node);
      std::string mayaName = arnoldToMayaNames[nodeName];

      bool isOsl = AiNodeIs(node, str::osl);
      bool isImager = false;
      if (AiNodeEntryGetType(nodeEntry) == AI_NODE_DRIVER)
      {
         AtString subtype;
         if (AiMetaDataGetStr(nodeEntry, AtString(), str::subtype, &subtype) && subtype == str::imager)
         {            
            isImager = true;
            imagersList.push_back(node);
            const AtNode *input = (AtNode*) AiNodeGetPtr(node, str::input);
            // If an input imager is chained to this one, we mark it as being referenced.
            // This will allow us to identify the root imager, which isn't referenced anywhere 
            if (input != nullptr)
               referencedImagers.insert(input);
         }
      }
         
      // loop over this arnold node parameters
      AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
      while (!AiParamIteratorFinished(nodeParam))
      {
         const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
         AtString paramName = AiParamGetName(paramEntry);
         if (paramName == str::name)
            continue;

         // For OSL, we already converted the attribute "code", we must not do it again
         // (otherwise Arnold might recompile it under the hood)
         if (isOsl && paramName == str::code)
            continue;

         // For imagers, we don't want to import the attribute "input", but instead
         // order the imagers accordingly in the stack
         if (isImager && paramName == str::input)
            continue;

         MString mayaAttrName = ArnoldToMayaAttrName(nodeEntry, paramName);
         if (mayaAttrName.length() == 0)
            continue;

         MStringArray splitMayaArrayAttr;
         mayaAttrName.split('.', splitMayaArrayAttr);

         MString mayaFullAttr = MString(mayaName.c_str()) + MString(".") + mayaAttrName;
         MString attrValue(" ");
         MString mayaArrayAttr("");
         
         // First check if the attribute is linked
         if (AiNodeIsLinked(node, paramName))
         {
            int output_param, output_comp;
            AtNode* connected_node = AiNodeGetLinkOutput(node, paramName, output_param, output_comp);
            MString connected_attr;
            if (output_param >= 0)
            {
               auto node_entry = AiNodeGetNodeEntry(connected_node) ;
               auto param_entry = AiNodeEntryGetOutput(node_entry,output_param);
               MString connected_output(AiParamGetName(param_entry));
               connected_attr = MString(AiParamGetName(param_entry).c_str());
               // If the connection has a component
               if (output_comp > -1)
               {
                  const std::string output_component = getComponentName(AiNodeEntryGetOutputType(node_entry), output_comp);
                  connected_attr += MString(".")+ MString(output_component.c_str());
               }
            }
            ConnectMayaFromArnold(mayaFullAttr, connected_attr , connected_node, arnoldToMayaNames);
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
                  std::string str = ConvertStringAttribute(node, paramName.c_str());
                  attrValue += "-type \"string\" \"";
                  attrValue += str.c_str();
                  attrValue += "\"";
               break;
               }
               {
               case AI_TYPE_NODE:
               case AI_TYPE_POINTER:
                  setAttrValue = false; // we won't need to set the attribute value since we're connecting it
                  ConnectMayaFromArnold(mayaFullAttr, MString(), (AtNode*)AiNodeGetPtr(node, paramName), arnoldToMayaNames);
                  
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
                        attrValue += MString("]");
                        if (splitMayaArrayAttr.length() > 1)
                        {
                           mayaArrayAttr = MString(mayaName.c_str()) + MString(".") + splitMayaArrayAttr[0] + attrValue;
                           for (unsigned int i = 1; i < splitMayaArrayAttr.length(); ++i)
                              mayaArrayAttr += MString(".") + splitMayaArrayAttr[i];
                        } else 
                           mayaArrayAttr = mayaFullAttr + attrValue;

                        switch(AiArrayGetType(arr))
                        {
                           case AI_TYPE_BYTE:
                              mayaArrayAttr += MString(" ");
                              mayaArrayAttr += (int)AiArrayGetByte(arr, i);
                              MGlobal::executeCommand(MString("setAttr ") + mayaArrayAttr);
                           break;
                           case AI_TYPE_ENUM:
                           case AI_TYPE_INT:
                              mayaArrayAttr += MString(" ");
                              mayaArrayAttr += (int)AiArrayGetInt(arr, i);
                              MGlobal::executeCommand(MString("setAttr ") + mayaArrayAttr);
                           break;
                           case AI_TYPE_UINT:
                              mayaArrayAttr += MString(" ");
                              mayaArrayAttr += (int)AiArrayGetUInt(arr, i);
                              MGlobal::executeCommand(MString("setAttr ") + mayaArrayAttr);
                           break;
                           case AI_TYPE_BOOLEAN:
                              mayaArrayAttr += MString(" ");
                              mayaArrayAttr += (AiArrayGetBool(arr, i)) ? MString("1") : MString("0");
                              MGlobal::executeCommand(MString("setAttr ") + mayaArrayAttr);
                           break;
                           case AI_TYPE_FLOAT:
                              mayaArrayAttr += MString(" ");
                              mayaArrayAttr += (float)AiArrayGetFlt(arr, i);
                              MGlobal::executeCommand(MString("setAttr ") + mayaArrayAttr);
                           break;
                           {
                           case AI_TYPE_RGB:
                              AtRGB col = AiArrayGetRGB(arr, i);
                              mayaArrayAttr += " -type float3 ";
                              mayaArrayAttr += (float) col.r;
                              mayaArrayAttr += MString(" ");
                              mayaArrayAttr += (float) col.g;
                              mayaArrayAttr += MString(" ");
                              mayaArrayAttr += (float) col.b;
                              MGlobal::executeCommand(MString("setAttr ") + mayaArrayAttr);
                           break;
                           }
                           {
                           case AI_TYPE_RGBA:
                              AtRGBA col = AiArrayGetRGBA(arr, i);
                              mayaArrayAttr += " -type float3 ";
                              mayaArrayAttr += (float) col.r;
                              mayaArrayAttr += MString(" ");
                              mayaArrayAttr += (float) col.g;
                              mayaArrayAttr += MString(" ");
                              mayaArrayAttr += (float) col.b;
                              MGlobal::executeCommand(MString("setAttr ") + mayaArrayAttr);
                           break;
                           }
                           {
                           case AI_TYPE_VECTOR:
                              AtVector vec = AiArrayGetVec(arr, i);
                              mayaArrayAttr += " -type float3 ";
                              mayaArrayAttr += (float) vec.x;
                              mayaArrayAttr += MString(" ");
                              mayaArrayAttr += (float) vec.y;
                              mayaArrayAttr += MString(" ");
                              mayaArrayAttr += (float) vec.z;
                              MGlobal::executeCommand(MString("setAttr ") + mayaArrayAttr);
                           break;
                           }
                           {
                           case AI_TYPE_VECTOR2:
                              AtVector2 vec = AiArrayGetVec2(arr, i);
                              mayaArrayAttr += " -type float2 ";
                              mayaArrayAttr += (float) vec.x;
                              mayaArrayAttr += MString(" ");
                              mayaArrayAttr += (float) vec.y;
                              MGlobal::executeCommand(MString("setAttr ") + mayaArrayAttr);
                           break;
                           }
                           {
                           case AI_TYPE_STRING:
                              std::string str = std::string(AiArrayGetStr(arr, i));
                              mayaArrayAttr += " -type \"string\" \"";
                              // convert any '\' to '\\' in the string
                              std::string::size_type i = str.find('\\');
                              while (i != std::string::npos)
                              {
                                  str.replace(i, 1, "\\\\");
                                  i = str.find('\\', i+2);
                              }
                              mayaArrayAttr += str.c_str();
                              mayaArrayAttr += "\"";
                              MGlobal::executeCommand(MString("setAttr ") + mayaArrayAttr);
                           break;
                           }
                           {
                           case AI_TYPE_NODE:
                           case AI_TYPE_POINTER:
                              ConnectMayaFromArnold(mayaArrayAttr, MString(), (AtNode*)AiArrayGetPtr(arr, i), arnoldToMayaNames);
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
      if (AiNodeLookUpUserParameter(node, str::material_surface))
      {
         AtString shadingGroup = AiNodeGetStr(node, str::material_surface);
         std::string shg = GetShadingGroup(shadingGroup.c_str(), arnoldToMayaShadingEngines);
         MString connectCmd = MString("connectAttr -f ") + MString(mayaName.c_str()) + MString(".outColor ") + MString(shg.c_str()) + MString(".surfaceShader");
         // MGlobal::displayInfo(connectCmd);
         MGlobal::executeCommand(connectCmd);
      }
      if (AiNodeLookUpUserParameter(node, str::material_displacement))
      {
         AtString shadingGroup = AiNodeGetStr(node, str::material_displacement);
         std::string shg = GetShadingGroup(shadingGroup.c_str(), arnoldToMayaShadingEngines);
         MString connectCmd = MString("connectAttr -f ") + MString(mayaName.c_str()) + MString(".outColor ") + MString(shg.c_str()) + MString(".displacementShader");
         // MGlobal::displayInfo(connectCmd);
         MGlobal::executeCommand(connectCmd);
      }
   }
   
   if (!imagersList.empty())
   {
      // First, let's find the first imager, as being the first one that
      // isn't referenced anywhere
      const AtNode* targetImager = nullptr;
      for (const auto &imager : imagersList)
      {
         if (referencedImagers.find(imager) == referencedImagers.end())
         {
            targetImager = imager;
            break;
         }
      }

      // Now let's build the imagers stack as it will show up in the render settings
      std::vector<const AtNode *> imagersStack;
      while(targetImager != nullptr)
      {
         imagersStack.insert(imagersStack.begin(), targetImager);
         targetImager = (const AtNode*) AiNodeGetPtr(targetImager, AtString("input"));
      }
      
      for (unsigned int j = 0; j < imagersStack.size(); ++j)
      {
         MString mayaFullAttr = "defaultArnoldRenderOptions.imagers[";
         mayaFullAttr += j;
         mayaFullAttr += "]";
         ConnectMayaFromArnold(mayaFullAttr, MString("message"), (AtNode*)imagersStack[j], arnoldToMayaNames);
      }
   }
   AiUniverseDestroy(universe);

   
   return status;
}
