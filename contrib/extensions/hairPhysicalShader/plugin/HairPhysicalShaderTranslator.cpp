#include "HairPhysicalShaderTranslator.h"
#include "extension/Extension.h"
#include <cassert>

#include <maya/MFnNumericData.h>

void CHairPhysicalShaderTranslator::NodeInitializer(CAbTranslator context)
{
    CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "hair");

    // Diffuse
    helper.MakeInput("kd_ind");
/*
    // UVs
    {
        CAttrData data;
        helper.GetAttrData("uparam", data);
        data.stringDefault = "uparamcoord";
        helper.MakeInput(data);
    }
    {
        CAttrData data;
        helper.GetAttrData("vparam", data);
        data.stringDefault = "vparamcoord";
        helper.MakeInput(data);
    }

    // AOVs
    helper.MakeInput("aov_direct_diffuse");
    helper.MakeInput("aov_direct_specular");
    helper.MakeInput("aov_direct_transmission");
    helper.MakeInput("aov_indirect_diffuse");*/
}

AtNode* CHairPhysicalShaderTranslator::CreateArnoldNodes()
{
    // Create the root aiHair shader node
    AtNode* shader = AddArnoldNode("hair");

    // Create the parameter adapter nodes
    AddAdapterNode(shader, "rootcolor",           "HairPhysicalShaderDiffuseColorAdapter");
    AddAdapterNode(shader, "tipcolor",            "HairPhysicalShaderDiffuseColorAdapter");
    AddAdapterNode(shader, "spec",                "HairPhysicalShaderSpecularWeightAdapter");
    AddAdapterNode(shader, "gloss",               "HairPhysicalShaderSpecularGlossAdapter");
    AddAdapterNode(shader, "spec2",               "HairPhysicalShaderSpecularWeightAdapter");
    AddAdapterNode(shader, "gloss2",              "HairPhysicalShaderSpecularGlossAdapter");
    AddAdapterNode(shader, "transmission",        "HairPhysicalShaderTransmissionWeightAdapter");
    AddAdapterNode(shader, "transmission_spread", "HairPhysicalShaderTransmissionSpreadAdapter");
    AddAdapterNode(shader, "opacity",             "MayaReverse");

    // Other AOV output nodes
    return ProcessAOVOutput(shader);
}

void CHairPhysicalShaderTranslator::Export(AtNode* shader)
{
    // Diffuse Component (D)
    //

    AtNode* rootcolor = GetAdapterNode("rootcolor");
        ProcessParameter(rootcolor, "input1",AI_TYPE_RGB,   "rootColorD");
        ProcessParameter(rootcolor, "input2.x", AI_TYPE_FLOAT, "intensityD");
        ProcessParameter(rootcolor, "input2.y", AI_TYPE_FLOAT, "intensityD");
        ProcessParameter(rootcolor, "input2.z", AI_TYPE_FLOAT, "intensityD");

    AtNode* tipcolor = GetAdapterNode("tipcolor");
        ProcessParameter(tipcolor, "input1",     AI_TYPE_RGB,   "tipColorD");
        ProcessParameter(tipcolor, "input2.x", AI_TYPE_FLOAT, "intensityD");
        ProcessParameter(tipcolor, "input2.y", AI_TYPE_FLOAT, "intensityD");
        ProcessParameter(tipcolor, "input2.z", AI_TYPE_FLOAT, "intensityD");

    // // Fully Kajiya-Kay diffuse. No isotropic.
    AiNodeSetFlt(shader, "ambdiff", 1.0f);


    // // Primary Specular Component (R)
    // //
    AtNode* spec = GetAdapterNode("spec");
    ProcessParameter(spec, "input1.r",         AI_TYPE_FLOAT, "intensityR");
    ProcessParameter(spec, "input1.g",         AI_TYPE_FLOAT, "intensityR");
    ProcessParameter(spec, "input1.b",         AI_TYPE_FLOAT, "intensityR");
    ProcessParameter(spec, "input2.r", AI_TYPE_FLOAT, "longitudinalWidthR");
    ProcessParameter(spec, "input2.g", AI_TYPE_FLOAT, "longitudinalWidthR");
    ProcessParameter(spec, "input2.b", AI_TYPE_FLOAT, "longitudinalWidthR");

    AtNode* gloss = GetAdapterNode("gloss");
    ProcessParameter(gloss, "input1.r", AI_TYPE_FLOAT, "longitudinalWidthR");
    ProcessParameter(gloss, "input1.g", AI_TYPE_FLOAT, "longitudinalWidthR");
    ProcessParameter(gloss, "input1.b", AI_TYPE_FLOAT, "longitudinalWidthR");
    ProcessParameter(gloss, "input2.r", AI_TYPE_FLOAT, "longitudinalWidthR");
    ProcessParameter(gloss, "input2.g", AI_TYPE_FLOAT, "longitudinalWidthR");
    ProcessParameter(gloss, "input2.b", AI_TYPE_FLOAT, "longitudinalWidthR");

    // // Primary specular color
    ProcessParameter(shader, "spec_color", AI_TYPE_RGB, "colorR");

    // // Angular shift in degrees
    ProcessParameter(shader, "spec_shift", AI_TYPE_FLOAT, "longitudinalShiftR");


    // // Secondary Specular Component (TRT)
    // //
    AtNode* spec2 = GetAdapterNode("spec2");
    ProcessParameter(spec2, "input1.r", AI_TYPE_FLOAT, "intensityTRT");
    ProcessParameter(spec2, "input1.g", AI_TYPE_FLOAT, "intensityTRT");
    ProcessParameter(spec2, "input1.b", AI_TYPE_FLOAT, "intensityTRT");
    ProcessParameter(spec2, "input2.r", AI_TYPE_FLOAT, "longitudinalWidthTRT");
    ProcessParameter(spec2, "input2.g", AI_TYPE_FLOAT, "longitudinalWidthTRT");
    ProcessParameter(spec2, "input2.b", AI_TYPE_FLOAT, "longitudinalWidthTRT");

    AtNode* gloss2 = GetAdapterNode("gloss2");
    ProcessParameter(gloss2, "input1.r", AI_TYPE_FLOAT, "longitudinalWidthTRT");
    ProcessParameter(gloss2, "input1.g", AI_TYPE_FLOAT, "longitudinalWidthTRT");
    ProcessParameter(gloss2, "input1.b", AI_TYPE_FLOAT, "longitudinalWidthTRT");
    ProcessParameter(gloss2, "input2.r", AI_TYPE_FLOAT, "longitudinalWidthTRT");
    ProcessParameter(gloss2, "input2.g", AI_TYPE_FLOAT, "longitudinalWidthTRT");
    ProcessParameter(gloss2, "input2.b", AI_TYPE_FLOAT, "longitudinalWidthTRT");

    // // Secondary specular color
    ProcessParameter(shader, "spec2_color", AI_TYPE_RGB, "colorTRT");

    // // Angular shift in degrees
    ProcessParameter(shader, "spec2_shift", AI_TYPE_FLOAT, "longitudinalShiftTRT");


    // // Transmission Component (TT)
    // //
    AtNode* transmission = GetAdapterNode("transmission");
    AtNode* transmission2 = GetAdapterNode("transmission_mult2");
    AtNode* transmission3 = GetAdapterNode("transmission_mult3");
    ProcessParameter(transmission, "input1.r", AI_TYPE_FLOAT, "longitudinalWidthTT");
    ProcessParameter(transmission, "input1.g", AI_TYPE_FLOAT, "longitudinalWidthTT");
    ProcessParameter(transmission, "input1.b", AI_TYPE_FLOAT, "longitudinalWidthTT");
    ProcessParameter(transmission2, "input1.r",AI_TYPE_FLOAT, "azimuthalWidthTT");
    ProcessParameter(transmission2, "input1.g",AI_TYPE_FLOAT, "azimuthalWidthTT");
    ProcessParameter(transmission2, "input1.b",AI_TYPE_FLOAT, "azimuthalWidthTT");
    ProcessParameter(transmission3, "input1.r",AI_TYPE_FLOAT, "intensityTT");
    ProcessParameter(transmission3, "input1.g",AI_TYPE_FLOAT, "intensityTT");
    ProcessParameter(transmission3, "input1.b",AI_TYPE_FLOAT, "intensityTT");

    AtNode* transmission_spread = GetAdapterNode("transmission_spread");
    AtNode* transmission_spread2 = GetAdapterNode("transmission_spread_mult2");
    ProcessParameter(transmission_spread, "input1.r", AI_TYPE_FLOAT, "longitudinalWidthTT");
    ProcessParameter(transmission_spread, "input1.g", AI_TYPE_FLOAT, "longitudinalWidthTT");
    ProcessParameter(transmission_spread, "input1.b", AI_TYPE_FLOAT, "longitudinalWidthTT");
    ProcessParameter(transmission_spread2, "input1.r",AI_TYPE_FLOAT, "azimuthalWidthTT");
    ProcessParameter(transmission_spread2, "input1.g",AI_TYPE_FLOAT, "azimuthalWidthTT");
    ProcessParameter(transmission_spread2, "input1.b",AI_TYPE_FLOAT, "azimuthalWidthTT");

    // Transmission color
    ProcessParameter(shader, "transmission_color", AI_TYPE_RGB, "colorTT");


    // Opacity
    //
    AtNode* opacity = GetAdapterNode("opacity");
    ProcessParameter(opacity, "input", AI_TYPE_RGB, "transparency");


    // Arnold-specific attributes
    //

    // Indirect Diffuse
    ProcessParameter(shader, "kd_ind", AI_TYPE_FLOAT, "ai_kd_ind");

    // UVs

    // AOVs

    
    // Unsupported attributes
    //
    if (!IsBlack("ambientColor"))
        AiMsgWarning("[hairPhysicalShader] Ambient Color (.ambientColor) is not supported.");
    if (!IsBlack("incandescence"))
        AiMsgWarning("[hairPhysicalShader] Incandescence (.incandescence) is not supported.");
    if (!IsZero("intensityG"))
        AiMsgWarning("[hairPhysicalShader] Glint (.intensityG) is not supported.");
}

AtNode* CHairPhysicalShaderTranslator::AddAdapterNode(AtNode* shader, const char* input, const char* type)
{
    // Create a parameter adapter node of the specific type
    
    AtNode* adapter = NULL;
    if ( strcmp(type ,"HairPhysicalShaderDiffuseColorAdapter") == 0 )
    {
        adapter = AddArnoldNode("multiply", input);
        AiNodeSetRGB(adapter, "input1", 0.207f, 0.138f, 0.069f);
    }
    
    else if ( strcmp(type ,"HairPhysicalShaderSpecularWeightAdapter") == 0 )
    {
        AtNode* mult1 = AddArnoldNode("multiply", input);
        adapter = AddArnoldNode("multiply", (std::string(input) +std::string("_2")).c_str() )  ;
        AiNodeSetRGB(mult1, "input1", 0.55f, 0.55f,0.55f);
        AiNodeSetRGB(mult1, "input2", 3.5f, 3.5f,3.5f );
        AiNodeLink(mult1, "input1", adapter);
        AiNodeSetRGB(adapter, "input2", 0.1f, 0.1f,0.1f);
    }
    else if ( strcmp(type ,"HairPhysicalShaderTransmissionSpreadAdapter") == 0 )
    {
        AtNode* mult1 = AddArnoldNode("multiply", input);
        AtNode* mult2 = AddArnoldNode("multiply", (std::string(input) +std::string("_mult2")).c_str());
        AtNode* add = AddArnoldNode("add", (std::string(input) +std::string("_add")).c_str());
        adapter = AddArnoldNode("divide", (std::string(input) +std::string("_divide")).c_str());

        AiNodeSetRGB(mult1, "input1", 10.0f, 10.0f,10.0f);
        AiNodeSetRGB(mult1, "input2", 5.0f, 5.0f,5.0f);
        AiNodeSetRGB(mult2, "input1", 10.0f, 10.0f,10.0f);
        AiNodeSetRGB(mult2, "input2", 2.0f, 2.0f,2.0f);
        AiNodeSetRGB(adapter, "input2", 40.0f, 40.0f,40.0f);
        AiNodeLink(mult1, "input1", add);
        AiNodeLink(mult2, "input2", add);
        AiNodeLink(add, "input1", adapter);
    }
    else if ( strcmp(type ,"HairPhysicalShaderTransmissionWeightAdapter") == 0 )
    {
        AtNode* mult1 = AddArnoldNode("multiply", input);
        AtNode* mult2 = AddArnoldNode("multiply", (std::string(input) +std::string("_mult2")).c_str());
        AtNode* add = AddArnoldNode("add", (std::string(input) +std::string("_add")).c_str());
        AtNode* divide = AddArnoldNode("divide", (std::string(input) +std::string("_divide")).c_str());
        AtNode* mult3 = AddArnoldNode("multiply", (std::string(input) +std::string("_mult3")).c_str());
        adapter = AddArnoldNode("multiply", (std::string(input) +std::string("_mult4")).c_str());

        AiNodeSetRGB(mult1, "input1", 10.0f, 10.0f,10.0f);
        AiNodeSetRGB(mult1, "input2", 5.0f, 5.0f,5.0f);
        AiNodeSetRGB(mult2, "input1", 10.0f, 10.0f,10.0f);
        AiNodeSetRGB(mult2, "input2", 2.0f, 2.0f,2.0f);
        AiNodeSetRGB(mult3, "input1", 0.15f, 0.15f,0.150f);
        AiNodeSetRGB(adapter, "input2", 0.1f, 0.1f,0.1f);
        AiNodeSetRGB(divide, "input2", 40.0f, 40.0f,40.0f);
        AiNodeLink(mult1, "input1", add);
        AiNodeLink(mult2, "input2", add);
        AiNodeLink(add, "input1", divide);
        AiNodeLink(divide, "input2", mult3);
        AiNodeLink(mult3, "input1" , adapter);
    }
    else if ( strcmp(type ,"HairPhysicalShaderSpecularGlossAdapter") == 0 )
    {
        AtNode* mult = AddArnoldNode("multiply", type);
        AtNode* divide = AddArnoldNode("divide", (std::string(input) +std::string("_divide")).c_str());
        adapter = AddArnoldNode("min", (std::string(input) +std::string("_min")).c_str() /*tag*/);
        AiNodeSetRGB(mult, "input1", 3.5f, 3.5f,3.50f);
        AiNodeSetRGB(mult, "input2", 3.5f, 3.5f,3.50f);
        AiNodeSetRGB(divide, "input1", 3000.0f, 3000.0f,3000.0f);
        AiNodeSetRGB(adapter, "input2", 5000.0f, 5000.0f,5000.0f);
        AiNodeLink(mult, "input2", divide);
        AiNodeLink(divide, "input1", adapter);
    }
    
    
    assert(adapter);

    // Link the adapter node to the input of the shader node
    assert(!AiNodeIsLinked(shader, input));
    AiNodeLink(adapter, input, shader);
    assert(AiNodeGetLink(shader, input) == adapter);

    return adapter;
}

AtNode* CHairPhysicalShaderTranslator::GetAdapterNode(const char* input)
{
    // Get the adapter node from the node map
    AtNode* adapter = GetArnoldNode(input /*tag*/);
    assert(adapter);
    return adapter;
}


bool CHairPhysicalShaderTranslator::IsBlack(const char* plugName)
{
    // Find the plug
    MPlug plug = FindMayaPlug(plugName);
    if (!plug.isNull())
    {
        // Connected ?
        if (plug.isDestination() || plug.numConnectedChildren() > 0)
            return false;

        // Black(0,0,0) ?
        float rgb[3] = {0.0f, 0.0f, 0.0f};
        MFnNumericData data(plug.asMObject());
        if (data.getData3Float(rgb[0], rgb[1], rgb[2]) &&
            (rgb[0] != 0.0f || rgb[1] != 0.0f || rgb[2] != 0.0f))
        {
            return false;
        }
    }
    return true;
}

bool CHairPhysicalShaderTranslator::IsZero(const char* plugName)
{
    // Find the plug
    MPlug plug = FindMayaPlug(plugName);
    if (!plug.isNull())
    {
        // Connected ?
        if (plug.isDestination())
            return false;

        // Zero 0 ?
        if (plug.asFloat() != 0.0f)
            return false;
    }
    return true;
}

