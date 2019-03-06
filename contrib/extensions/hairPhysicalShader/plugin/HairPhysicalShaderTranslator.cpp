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

    // Other AOV output nodes
    return ProcessAOVOutput(shader);
}

void CHairPhysicalShaderTranslator::Export(AtNode* shader)
{
    MPlugArray connections;

    // Diffuse Component (D)
    //
    ProcessParameter(shader, "rootcolor", AI_TYPE_RGB, "rootColorD");
    ProcessParameter(shader, "tipcolor", AI_TYPE_RGB, "tipColorD");

    // We need to multiply both rootColor and tipColor by "Diffuse weight" (intensityD).
    // If none of them is linked we can simply to the multiplication here. Otherwise we need to insert a multiply shader
    MPlug intensityD_plug = FindMayaPlug("intensityD");
    intensityD_plug.connectedTo(connections, true, false);
    AtNode *intensityD_link = (connections.length() > 0) ? ExportConnectedNode(connections[0]) : NULL;
    if (connections.length() > 0 || AiNodeGetLink(shader, "rootcolor"))
    {
        // either rootcolor or diffuse weight is linked, we need to insert a shader
        AtNode *adapter = GetArnoldNode("rootcolor");
        if (adapter == NULL)
            adapter = AddArnoldNode("multiply", "rootcolor");

        AiNodeLink(adapter, "rootcolor", shader);
        ProcessParameter(adapter, "input1", AI_TYPE_RGB, "rootColorD");
        if (intensityD_link)
            AiNodeLink(intensityD_link, "input2", adapter);
        else
        {
            float val = intensityD_plug.asFloat();
            AiNodeUnlink(adapter, "input2");
            AiNodeSetRGB(adapter, "input2", val, val, val);
        }
    } else
    {
        AtRGB col = AiNodeGetRGB(shader, "rootcolor");
        col *= intensityD_plug.asFloat();
        AiNodeUnlink(shader, "rootcolor");
        AiNodeSetRGB(shader, "rootcolor", col.r, col.g, col.b);
    }

    if (connections.length() > 0 || AiNodeGetLink(shader, "tipcolor"))
    {
        // either rootcolor or diffuse weight is linked, we need to insert a shader
        AtNode *adapter = GetArnoldNode("tipcolor");
        if (adapter == NULL)
            adapter = AddArnoldNode("multiply", "tipcolor");

        AiNodeLink(adapter, "tipcolor", shader);
        ProcessParameter(adapter, "input1", AI_TYPE_RGB, "tipColorD");
        if (intensityD_link)
            AiNodeLink(intensityD_link, "input2", adapter);
        else
        {
            float val = intensityD_plug.asFloat();
            AiNodeUnlink(adapter, "input2");
            AiNodeSetRGB(adapter, "input2", val, val, val);
        }
    } else
    {
        AtRGB col = AiNodeGetRGB(shader, "tipcolor");
        col *= intensityD_plug.asFloat();
        AiNodeUnlink(shader, "tipcolor");
        AiNodeSetRGB(shader, "tipcolor", col.r, col.g, col.b);
    }

    // // Fully Kajiya-Kay diffuse. No isotropic.
    AiNodeSetFlt(shader, "ambdiff", 1.0f);


    // // Primary Specular Component is (R)
    // // Secondary Specular is (TRT)

    for (int i = 0; i < 2; ++i)
    {
        std::string specAttr = (i == 0) ? "spec" : "spec2";
        std::string specGlossAttr = (i == 0) ? "spec_gloss" : "spec2_gloss";
        std::string specIntensityAttr = (i == 0) ? "intensityR" : "intensityTRT";
        std::string specWidthAttr = (i == 0) ? "longitudinalWidthR" : "longitudinalWidthTRT";        

        connections.clear();
        float specIntensity = 0.1f;
        MPlug intensityR_plug = FindMayaPlug(specIntensityAttr.c_str());
        specIntensity *= intensityR_plug.asFloat();
        intensityR_plug.connectedTo(connections, true, false);
        AtNode *intensityR_link = (connections.length() > 0) ? ExportConnectedNode(connections[0]) : NULL;
        MPlug longitudinalWidthR_plug = FindMayaPlug(specWidthAttr.c_str());
        specIntensity *= longitudinalWidthR_plug.asFloat();
        connections.clear();
        longitudinalWidthR_plug.connectedTo(connections, true, false);
        AtNode *longitudinalWidthR_link = (connections.length() > 0) ? ExportConnectedNode(connections[0]) : NULL;

        if (intensityR_link == NULL && longitudinalWidthR_link == NULL)
        {
            AiNodeUnlink(shader, specAttr.c_str());
            // no link anywhere
            AiNodeSetFlt(shader, specAttr.c_str(), specIntensity);
        } else
        {
            // need to link spec to a shader
            AtNode *adapter = GetArnoldNode(specAttr.c_str());
            if (adapter == NULL)
                adapter = AddArnoldNode("color_correct", specAttr.c_str());

            AiNodeLinkOutput(adapter, "r", shader, specAttr.c_str());
            AiNodeSetRGB(adapter, "multiply", 0.1f, 0.1f, 0.1f);
            AiNodeUnlink(adapter, "input");
            AiNodeUnlink(adapter, "input.r");

            if (intensityR_link)
                AiNodeLink(intensityR_link, "input.r", adapter);
            else
                AiNodeSetRGBA(adapter, "input", intensityR_plug.asFloat(), 0.f, 0.f, 1.f);

            AiNodeSetFlt(adapter, "contrast_pivot", 0.f);
            AiNodeUnlink(adapter, "contrast");
            if (longitudinalWidthR_link)
                AiNodeLink(longitudinalWidthR_link, "contrast", adapter);
            else
                AiNodeSetFlt(adapter, "contrast", longitudinalWidthR_plug.asFloat());
        }

        AiNodeUnlink(shader, specGlossAttr.c_str());    
        if (longitudinalWidthR_link)
        {
            AtNode *adapter = GetArnoldNode(specGlossAttr.c_str());
            if(adapter == NULL)
                adapter = AddArnoldNode("layer_rgba", specGlossAttr.c_str());

            AiNodeLinkOutput(adapter, "r", shader, specGlossAttr.c_str());
            AiNodeSetRGBA(adapter, "input1", 3000.f, 0.f, 0.f, 1.f);
            AiNodeSetRGBA(adapter, "input2", 1.f, 1.f, 1.f, 1.f);
            AiNodeLink(longitudinalWidthR_link, "input2.r", adapter);
            AiNodeSetStr(adapter, "operation2", "divide");
            AiNodeSetRGBA(adapter, "input3", 1.f, 1.f, 1.f, 1.f);
            AiNodeLink(longitudinalWidthR_link, "input3.r", adapter);
            AiNodeSetStr(adapter, "operation3", "divide");
            AiNodeSetRGBA(adapter, "input4", 5000.f, 0.f, 0.f, 1.f);
            AiNodeSetStr(adapter, "operation4", "min");
            AiNodeSetBool(adapter, "enable1", true);
            AiNodeSetBool(adapter, "enable2", true);
            AiNodeSetBool(adapter, "enable3", true);
            AiNodeSetBool(adapter, "enable4", true);        
            AiNodeSetBool(adapter, "enable5", false);
            AiNodeSetBool(adapter, "enable6", false);
            AiNodeSetBool(adapter, "enable7", false);
            AiNodeSetBool(adapter, "enable8", false);
        } else
        {
            float spec_gloss = longitudinalWidthR_plug.asFloat();
            spec_gloss = AiMin(3000.0f / (spec_gloss * spec_gloss), 5000.0f);
            AiNodeSetFlt(shader, specGlossAttr.c_str(), spec_gloss);
        }
    }

    // // Primary specular color
    ProcessParameter(shader, "spec_color", AI_TYPE_RGB, "colorR");
    // // Angular shift in degrees
    ProcessParameter(shader, "spec_shift", AI_TYPE_FLOAT, "longitudinalShiftR");

    // // Secondary specular color
    ProcessParameter(shader, "spec2_color", AI_TYPE_RGB, "colorTRT");

    // // Angular shift in degrees
    ProcessParameter(shader, "spec2_shift", AI_TYPE_FLOAT, "longitudinalShiftTRT");


    // // Transmission Component (TT)
    // //

    connections.clear();
    MPlug intensityTT_plug = FindMayaPlug("intensityTT");
    intensityTT_plug.connectedTo(connections, true, false);
    AtNode *intensityTT_link = (connections.length() > 0) ? ExportConnectedNode(connections[0]) : NULL;

    MPlug longitudinalWidthTT_plug = FindMayaPlug("longitudinalWidthTT");
    connections.clear();
    longitudinalWidthTT_plug.connectedTo(connections, true, false);
    if (connections.length() > 0)
        AiMsgWarning("[mtoa.translator] Parameters binding not supported in hairPhysical's Transmission U Scatter");

    MPlug azimuthalWidthTT_plug = FindMayaPlug("azimuthalWidthTT");
    connections.clear();
    azimuthalWidthTT_plug.connectedTo(connections, true, false);
    if (connections.length() > 0)
        AiMsgWarning("[mtoa.translator] Parameters binding not supported in hairPhysical's Transmission V Scatter");

    float spread = (longitudinalWidthTT_plug.asFloat() * 5.f + azimuthalWidthTT_plug.asFloat() * 2.f) / 40.f;

    AiNodeUnlink(shader, "transmission");
    if (intensityTT_link)
    {
        AtNode* adapter = GetArnoldNode("transmission");
        if (adapter == NULL)
            adapter = AddArnoldNode("multiply", "transmission");
        AiNodeLink(adapter, "transmission", shader);
        AiNodeLink(intensityTT_link, "input1", adapter);
        float transmissionVal = 0.1f * spread;
        AiNodeSetRGB(adapter, "input2", transmissionVal, transmissionVal, transmissionVal);

    } else
    {
        float transmissionVal = 0.1f * intensityTT_plug.asFloat() * spread;
        AiNodeSetFlt(shader, "transmission", transmissionVal);
    }
    AiNodeSetFlt(shader, "transmission_spread", spread);

    // Transmission color
    ProcessParameter(shader, "transmission_color", AI_TYPE_RGB, "colorTT");


    // Opacity
    //
    MPlug transparency_plug = FindMayaPlug("transparency");
    connections.clear();
    transparency_plug.connectedTo(connections, true, false);
    AiNodeUnlink(shader, "opacity");
    AtRGB opacity(1.f - transparency_plug.child(0).asFloat(),
                  1.f - transparency_plug.child(1).asFloat(), 
                  1.f - transparency_plug.child(2).asFloat());
    AiNodeSetRGB(shader, "opacity", opacity.r, opacity.g, opacity.b);
    bool hasTransparencyConnection = (connections.length() > 0);
    if (!hasTransparencyConnection)
    {
        for (unsigned int i = 0; i < 3; ++i)
        {
            connections.clear();
            transparency_plug.child(i).connectedTo(connections, true, false);   
            if (connections.length() > 0)
            {
                hasTransparencyConnection = true;
                break;
            }
        }
    }
    if (hasTransparencyConnection)
    {
        AtNode *adapter = GetArnoldNode("opacity");
        if (adapter == NULL)
            adapter = AddArnoldNode("complement", "opacity");
        AiNodeLink(adapter, "opacity", shader);
        ProcessParameter(adapter, "input", AI_TYPE_RGB, "transparency");
    }

    
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

