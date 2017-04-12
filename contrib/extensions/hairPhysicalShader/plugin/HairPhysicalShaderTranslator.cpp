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
        ProcessParameter(rootcolor, "colorD",     AI_TYPE_RGB,   "rootColorD");
        ProcessParameter(rootcolor, "intensityD", AI_TYPE_FLOAT, "intensityD");

    AtNode* tipcolor = GetAdapterNode("tipcolor");
        ProcessParameter(tipcolor, "colorD",     AI_TYPE_RGB,   "tipColorD");
        ProcessParameter(tipcolor, "intensityD", AI_TYPE_FLOAT, "intensityD");

    // Fully Kajiya-Kay diffuse. No isotropic.
    AiNodeSetFlt(shader, "ambdiff", 1.0f);


    // Primary Specular Component (R)
    //
    AtNode* spec = GetAdapterNode("spec");
        ProcessParameter(spec, "intensity",         AI_TYPE_FLOAT, "intensityR");
        ProcessParameter(spec, "longitudinalWidth", AI_TYPE_FLOAT, "longitudinalWidthR");

    AtNode* gloss = GetAdapterNode("gloss");
        ProcessParameter(gloss, "longitudinalWidth", AI_TYPE_FLOAT, "longitudinalWidthR");

    // Primary specular color
    ProcessParameter(shader, "spec_color", AI_TYPE_RGB, "colorR");

    // Angular shift in degrees
    ProcessParameter(shader, "spec_shift", AI_TYPE_FLOAT, "longitudinalShiftR");


    // Secondary Specular Component (TRT)
    //
    AtNode* spec2 = GetAdapterNode("spec2");
        ProcessParameter(spec2, "intensity",         AI_TYPE_FLOAT, "intensityTRT");
        ProcessParameter(spec2, "longitudinalWidth", AI_TYPE_FLOAT, "longitudinalWidthTRT");

    AtNode* gloss2 = GetAdapterNode("gloss2");
        ProcessParameter(gloss2, "longitudinalWidth", AI_TYPE_FLOAT, "longitudinalWidthTRT");

    // Secondary specular color
    ProcessParameter(shader, "spec2_color", AI_TYPE_RGB, "colorTRT");

    // Angular shift in degrees
    ProcessParameter(shader, "spec2_shift", AI_TYPE_FLOAT, "longitudinalShiftTRT");


    // Transmission Component (TT)
    //
    AtNode* transmission = GetAdapterNode("transmission");
        ProcessParameter(transmission, "intensityTT",         AI_TYPE_FLOAT, "intensityTT");
        ProcessParameter(transmission, "longitudinalWidthTT", AI_TYPE_FLOAT, "longitudinalWidthTT");
        ProcessParameter(transmission, "azimuthalWidthTT",    AI_TYPE_FLOAT, "azimuthalWidthTT");

    AtNode* transmission_spread = GetAdapterNode("transmission_spread");
        ProcessParameter(transmission_spread, "longitudinalWidthTT", AI_TYPE_FLOAT, "longitudinalWidthTT");
        ProcessParameter(transmission_spread, "azimuthalWidthTT",    AI_TYPE_FLOAT, "azimuthalWidthTT");

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
    AtNode* adapter = AddArnoldNode(type, input /*tag*/);
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

