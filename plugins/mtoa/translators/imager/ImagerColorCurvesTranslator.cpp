#include "ImagerColorCurvesTranslator.h"
#include "ImagerTranslator.h"
#include "utils/Universe.h"
#include "translators/NodeTranslatorImpl.h"
#include "translators/DagTranslator.h"
#include <maya/MPlugArray.h>
#include <maya/MTimerMessage.h>
#include <maya/MEventMessage.h>


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
}
