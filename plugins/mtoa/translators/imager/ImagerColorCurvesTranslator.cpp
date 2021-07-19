#include "ImagerColorCurvesTranslator.h"
#include "ImagerTranslator.h"
#include "utils/Universe.h"
#include "translators/NodeTranslatorImpl.h"
#include "translators/DagTranslator.h"
#include <maya/MPlugArray.h>
#include <maya/MTimerMessage.h>
#include <maya/MEventMessage.h>
#include "render/RenderSession.h"
#include "scene/MayaScene.h"


AtNode* CImagerColorCurvesTranslator::CreateArnoldNodes()
{
   return AddArnoldNode(m_impl->m_abstract.arnold.asChar());
}

void CImagerColorCurvesTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper("aiImagerColorCurves");
   
   CAttrData data;
   data.name = "rampRGB";
   data.shortName = "aiRampRGB";
   helper.MakeInputCurveRamp(data);

   data.name = "rampR";
   data.shortName = "aiRampR";
   helper.MakeInputCurveRamp(data);

   data.name = "rampG";
   data.shortName = "aiRampG";
   helper.MakeInputCurveRamp(data);

   data.name = "rampB";
   data.shortName = "aiRampB";
   helper.MakeInputCurveRamp(data);
}

void CImagerColorCurvesTranslator::ExportRampCurve(AtNode *shader, const std::string attr_name)
{
   MFnDependencyNode fnNode(GetMayaObject());
   MPlug plug;
   
   MString ramp_plug_name = MString("ramp")+MString(attr_name.c_str());
   MString ramp_plug_position = ramp_plug_name + MString("_Position");
   MString ramp_plug_value = ramp_plug_name + MString("_FloatValue");
   MString ramp_plug_interp = ramp_plug_name + MString("_Interp");

   std::string arnold_ramp_position_attr = attr_name + "_position";
   std::string arnold_ramp_value_attr = attr_name + "_value";
   std::string arnold_ramp_interpolation_attr = attr_name + "_interpolation";
   
   plug = FindMayaPlug(ramp_plug_name);
   MObject opos = fnNode.attribute(ramp_plug_position);
   ProcessArrayParameter(shader, arnold_ramp_position_attr.c_str(), plug, AI_TYPE_FLOAT, &opos);
   MObject ocol = fnNode.attribute(ramp_plug_value);
   ProcessArrayParameter(shader, arnold_ramp_value_attr.c_str(), plug, AI_TYPE_FLOAT, &ocol);
   MObject oint = fnNode.attribute(ramp_plug_interp);
   ProcessArrayParameter(shader, arnold_ramp_interpolation_attr.c_str(), plug, AI_TYPE_INT, &oint);

   AtArray *interpArray = AiNodeGetArray(shader, arnold_ramp_interpolation_attr.c_str());
   unsigned int numElems = (interpArray) ? AiArrayGetNumElements(interpArray) : 0;
   for (unsigned int i = 0; i < numElems; ++i)
   {
      switch (AiArrayGetInt(interpArray, i))
      {
         default: // 0: None and 1: Linear are already ok
            break;
         case 2: // smooth
            AiArraySetInt(interpArray, i, 6); // Smooth is 6 in ramp_rgb
            break;
         case 3: // spline
            AiArraySetInt(interpArray, i, 2); // Converting "Spline" as "Catmull-Rom" (2)
            break;
      }
   } 

}

void CImagerColorCurvesTranslator::Export(AtNode *shader)
{

   ProcessParameter(shader, "enable", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "layer_selection", AI_TYPE_STRING);
   ProcessParameter(shader, "working_color_space", AI_TYPE_STRING);
   ExportRampCurve(shader,std::string("RGB"));
   ExportRampCurve(shader,std::string("R"));
   ExportRampCurve(shader,std::string("G"));
   ExportRampCurve(shader,std::string("B"));











//    MStatus status;
//    MPlug plug;
//    MString mayaAttrName;
//    const AtNodeEntry* entry = AiNodeGetNodeEntry(shader);
//    AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(entry);

//    while (!AiParamIteratorFinished(nodeParam))
//    {
//       const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
//       const char* paramName = AiParamGetName(paramEntry);
//       std::string paramNameStr(paramName);

//       if (paramNameStr == "name" || paramNameStr == "input" )
//       {
//          continue;
//       }
//       else if (paramNameStr != "layer_selection" && paramNameStr != "output_name" && paramNameStr != "enable")
//       {
//          AtArray *interpArray = AiNodeGetArray(shader, paramName);
//          unsigned int numElems = AiArrayGetNumElements(interpArray);
//          if (numElems == 0)
//          {
//             ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
//             continue;
//          }

//          CBaseAttrHelper helper(entry);
//          MFnDependencyNode fnNode(GetMayaObject());
//          if (helper.IsHidden(paramName))
//          {
//             if (MtoaTranslationInfo())
//                MtoaDebugLog("[mtoa.translator]  "+GetTranslatorName()+": Parameter "+
//                            paramName+" is hidden on Arnold node "+MString(AiNodeGetName(shader))+
//                            "("+MString(AiNodeEntryGetName(entry))+").");
//          }

//          // check paramName
//          mayaAttrName = helper.GetMayaAttrName(paramName);
//          plug = FindMayaPlug(mayaAttrName, &status);
//          if ((MStatus::kSuccess != status) || plug.isNull())
//          {
//             // check aiParamName
//             helper.SetPrefix("ai_");
//             MString mayaAttrAiName = helper.GetMayaAttrName(paramName);
//             plug = FindMayaPlug(mayaAttrAiName, &status);
//          }
//          if ( numElems == plug.numElements() )
//          {
//             for (unsigned int i = 0; i < numElems; i++)
//             {
//                if ( AiArrayGetType(interpArray) == AI_TYPE_BOOLEAN )
//                {
//                   AiArraySetBool(interpArray, i, plug[i].asBool());
//                }
//                else if (AiArrayGetType(interpArray) == AI_TYPE_FLOAT)
//                {
//                   AiArraySetFlt(interpArray, i, plug[i].asFloat());
//                }
//                else if (AiArrayGetType(interpArray) == AI_TYPE_STRING)
//                {
//                   AiArraySetStr(interpArray, i, plug[i].asString().asChar());
//                }
//                else if (AiArrayGetType(interpArray) == AI_TYPE_RGB)
//                {
//                   AtRGB rgb = AtRGB(plug[i].child(0).asFloat(), plug[i].child(1).asFloat(), plug[i].child(2).asFloat());
//                   AiArraySetRGB(interpArray, i, rgb);
//                }
//             }
//          }
//          else
//          {
//             ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
//          }
//       }
//       else
//       {
//          ProcessParameter(shader, paramName, AiParamGetType(paramEntry));
//       }
//    }
//    AiParamIteratorDestroy(nodeParam);


//    // // Always export residual layer (#4474)
//    bool export_residual = true;
//    AtArray *name_array = AiNodeGetArray(shader, "layer_name");
//    unsigned int numElems = AiArrayGetNumElements(name_array);

//    // Searching from the back because there's a higer chance for this 
//    // to be the last item in the list
//    for (unsigned int i = numElems ; i-- > 0;)
//    {
//       if (std::string(AiArrayGetStr(name_array, i)) == "<residual_lights>")
//       {
//          export_residual = false;
//          break;
//       }
//    }

//    // Using the numElems from one array for everything
//    // There should never be a case where these arrays are not the same
   
//    if (export_residual)
//    {   
//       AiArrayResize(name_array, numElems+1, AiArrayGetNumKeys(name_array));
//       AiArraySetStr(name_array, numElems, "<residual_lights>");

//       AtArray* enable_array = AiNodeGetArray(shader, "layer_enable");
//       AiArrayResize(enable_array, numElems+1, AiArrayGetNumKeys(enable_array));
//       AiArraySetBool(enable_array, numElems, 1);
      
//       AtArray* solo_array = AiNodeGetArray(shader, "layer_solo");
//       AiArrayResize(solo_array, numElems+1, AiArrayGetNumKeys(solo_array));
//       AiArraySetBool(solo_array, numElems, 0);
      
//       AtArray* intensity_array = AiNodeGetArray(shader, "layer_intensity");
//       AiArrayResize(intensity_array, numElems+1, AiArrayGetNumKeys(intensity_array));
//       AiArraySetFlt(intensity_array, numElems, 1.0f);
      
//       AtArray* exposure_array = AiNodeGetArray(shader, "layer_exposure");
//       AiArrayResize(exposure_array, numElems+1, AiArrayGetNumKeys(exposure_array));
//       AiArraySetFlt(exposure_array, numElems, 0.0f);
//       AtArray* tint_array = AiNodeGetArray(shader, "layer_tint");
//       AiArrayResize(tint_array, numElems+1, AiArrayGetNumKeys(tint_array));
//       AtRGB rgb = AtRGB(1.0f,1.0f,1.0f);
//       AiArraySetRGB(tint_array, numElems, rgb);
//    }
}
