#include "ArnoldNodeLinkSanitizer.h"

#include <ai.h>

#define LOG_ME 0

CShaderLinkSanitizer::CShaderLinkSanitizer(AtUniverse* in_universe, const char* node_name_prefix)
{
    m_universe = in_universe;
    m_node_prefix = node_name_prefix;

}

//////////////////////////////////////////////////
// INPUT CONNECTIONS
//////////////////////////////////////////////////

// Loop all the shaders, find all the parameters with at least one input connection, and store them in the m_parameters vector.
// Return true if at least one parameter with input connections was found.
//
bool CShaderLinkSanitizer::FindShadersWithInputComponents()
{
#if LOG_ME
    fprintf(stderr, "-------------------- %s --------------------\n", __func__);
#endif
    m_parameters.clear();
    AtNodeIterator* it = AiUniverseGetNodeIterator(m_universe, AI_NODE_SHADER);
    while (!AiNodeIteratorFinished(it))
    {
        AtNode* node = AiNodeIteratorGetNext(it);
        const AtNodeEntry* node_entry = AiNodeGetNodeEntry(node);
        string node_name(AiNodeGetName(node));

        AtParamIterator *param_it = AiNodeEntryGetParamIterator(node_entry);
        while (!AiParamIteratorFinished(param_it))
        {
            const AtParamEntry* param_entry = AiParamIteratorGetNext(param_it);
            string param_name(AiParamGetName(param_entry));

            // is the parameter or any of its components linked ?
            if (!AiNodeIsLinked(node, param_name.c_str()))
                continue;

            uint8_t param_type = AiParamGetType(param_entry);
            int param_nb_components = GetTypeNbComponents(param_type);
            if (param_nb_components < 3 || param_nb_components > 4) // no vec2
                continue;

            for (int component_index = 0; component_index < param_nb_components; component_index++)
            {
                string parameter_component = GetParameterComponent(param_name, param_type, component_index);
                if (parameter_component.empty())
                    continue;
                if (AiNodeIsLinked(node, parameter_component.c_str()))
                {
                    Parameter p = { node, param_name, param_type, param_nb_components };
                    m_parameters.push_back(p);
#if LOG_ME
                    fprintf(stderr, "%s: %s.%s has input component connections\n", __func__, node_name.c_str(), p.m_param_name.c_str());
#endif
                    break; // don't check the other components, we are just collecting the entire parameter
                }
            }

        }
    }
    AiNodeIteratorDestroy(it);
    return m_parameters.size() > 0;
}

// Fix all the input connections for the parameters in the m_parameters vector.
// Replace the link to those paramaters by a float_to_rgbx shader.
//
void CShaderLinkSanitizer::FixShadersWithInputComponents()
{
#if LOG_ME
    fprintf(stderr, "-------------------- %s --------------------\n", __func__);
#endif
    int output_component = -1;
    float param_value[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    for (auto& p : m_parameters)
    {
        string node_name(AiNodeGetName(p.m_node));
        AtNode* adapter = CreateNode(p.m_param_nb_components == 3 ? AtString("float_to_rgb") : AtString("float_to_rgba"));
        // first, get the self value and assign it to float_to_rgbx
        GetParameterValue(p.m_node, p.m_param_name, param_value);
        for (int component_index = 0; component_index < p.m_param_nb_components; component_index++)
            AiNodeSetFlt(adapter, m_rgba_from_int[component_index].c_str(), param_value[component_index]);

        // gather the connected shaders
        for (int component_index = 0; component_index < p.m_param_nb_components; component_index++)
        {
            // link inputs:
            string parameter_component = GetParameterComponent(p.m_param_name, p.m_param_type, component_index);
            if (AiNodeIsLinked(p.m_node, parameter_component.c_str()))
            {
                AtNode* input_node = AiNodeGetLink(p.m_node, parameter_component.c_str(), &output_component);
                string input_node_name(AiNodeGetName(input_node));
#if LOG_ME
                fprintf(stderr, "%s: %s.%s is linked by %s (component = %d)\n", __func__, node_name.c_str(), parameter_component.c_str(), input_node_name.c_str(), output_component);
#endif
                // type of the input node:
                int input_node_entry_type = AiNodeEntryGetOutputType(AiNodeGetNodeEntry(input_node));
                string input_node_component = GetParameterComponent("", input_node_entry_type, output_component);
#if LOG_ME
                if (output_component == -1)
                    fprintf(stderr, "%s: will link %s to %s.%s\n", __func__, input_node_name.c_str(),
                        node_name.c_str(), parameter_component.c_str());
                else
                    fprintf(stderr, "%s: will link %s.%s to %s.%s\n", __func__, input_node_name.c_str(), input_node_component.c_str(),
                        node_name.c_str(), parameter_component.c_str());
#endif
                if (output_component == -1)
                    AiNodeLink(input_node, m_rgba_from_int[component_index].c_str(), adapter);
                else
                    AiNodeLinkOutput(input_node, input_node_component.c_str(), adapter, m_rgba_from_int[component_index].c_str());
            }
        }
        // finally, link float_to_rgbx to p
        AiNodeLink(adapter, p.m_param_name.c_str(), p.m_node);
    }
}

//////////////////////////////////////////////////
// OUTPUT CONNECTIONS
//////////////////////////////////////////////////

// Loop all the shaders, find all the parameters with at least one output connection, and store them in the m_parameters vector.
// Return true if at least one parameter with output connections was found.
//
bool CShaderLinkSanitizer::FindShadersWithOutputComponents()
{
#if LOG_ME
    fprintf(stderr, "-------------------- %s --------------------\n", __func__);
#endif
    m_parameters.clear();
    int output_component = -1;

    AtNodeIterator* it = AiUniverseGetNodeIterator(m_universe, AI_NODE_SHADER);
    while (!AiNodeIteratorFinished(it))
    {
        AtNode* node = AiNodeIteratorGetNext(it);
        const AtNodeEntry* node_entry = AiNodeGetNodeEntry(node);

        AtParamIterator *param_it = AiNodeEntryGetParamIterator(node_entry);
        while (!AiParamIteratorFinished(param_it))
        {
            const AtParamEntry* param_entry = AiParamIteratorGetNext(param_it);
            string param_name(AiParamGetName(param_entry));
            std::cout << " Param Name is " << AiNodeGetName(node) << " .  " << param_name << std::endl;
            AtNode* input_node = AiNodeGetLink(node, param_name.c_str(), &output_component);
            std::cout << " Input Node Name is " << AiNodeGetName(input_node) << std::endl;

            if (!input_node || AiParamGetType(param_entry) != AI_TYPE_FLOAT )
            {
                continue;
            }
            // if (input_node && output_component == -1 )
            // {
            //     // This means it's a component'ed connection.
                
            //     if (AiParamGetType(param_entry) == AI_TYPE_FLOAT)
            //     {
            //         output_component = 4;
            //     }
            //     string node_name(AiNodeGetName(node));
            //     fprintf(stderr, "%s: %s.%s is linked by %s (component = %d)\n", __func__, node_name.c_str(), param_name.c_str(), AiNodeGetName(input_node), output_component);
            //     // fprintf (stderr, "\n \n \n >>>>  " , AiParamGetTypeName(AiParamGetType(param_entry)) , " <<<<<<< \n \n \n " )  ;
            // }

            // we have an input component toward node.param_name
#if LOG_ME
            string node_name(AiNodeGetName(node));
            fprintf(stderr, "%s: %s.%s is linked by %s (component = %d)\n", __func__, node_name.c_str(), param_name.c_str(), AiNodeGetName(input_node), output_component);
#endif
            uint8_t param_type = AiParamGetType(param_entry);
            int param_nb_components = GetTypeNbComponents(param_type);
            Parameter p = { node, param_name, param_type, param_nb_components }; // all info about THIS parameter (not the one linked by this)
            m_parameters.push_back(p);
        }

    }
    AiNodeIteratorDestroy(it);
    return m_parameters.size() > 0;
}

// Fix all the output connections for the parameters in the m_parameters vector.
// Replace the link to those paramaters by a rgbx_to_float shader.
//
void CShaderLinkSanitizer::FixShadersWithOutputComponents()
{
#if LOG_ME
    fprintf(stderr, "-------------------- %s --------------------\n", __func__);
#endif
    // To keep the list of the rgbx_to_float created for nodes and their components.
    // Say that xxx.r links both to yyy.p and zzz.q. Since we'll create xxx->rgbx_to_float(r), we will link
    // rgbx_to_float(r) to both yyy.p and zzz.q
    map < pair< AtNode*, int> , AtNode* > rgbx_mode_map; 
    int output_component = -1;

    for (auto& p : m_parameters)
    {
        AtNode* input_node = AiNodeGetLink(p.m_node, p.m_param_name.c_str(), &output_component);
        
        AtNode* adapter = NULL;
        pair< AtNode*, int> pr(input_node, output_component);
        // has this output connection of input_node already been filtered by an rgbx_to_float ? Is so, 
        // just reuse it for this parameter too (zzz.q in the example above)
        if (rgbx_mode_map.find(pr) != rgbx_mode_map.end())
            adapter = rgbx_mode_map[pr];
        else
        {
            int input_node_entry_type = AiNodeEntryGetOutputType(AiNodeGetNodeEntry(input_node));
            int input_node_nb_components = GetTypeNbComponents(input_node_entry_type);
            if (input_node_nb_components < 3 || input_node_nb_components > 4) // no vec2
                continue;

            adapter = CreateNode(input_node_nb_components == 3 ? AtString("rgb_to_float") : AtString("rgba_to_float")); // rgb_to_float or rgba_to_float
            if (output_component == -1)
            {
                AiNodeSetStr(adapter, "mode", "average");
            }
            else
            {
                AiNodeSetStr(adapter, "mode", m_rgba_from_int[output_component].c_str()); // setting rgbx_to_float.mode enum
            }
            
            rgbx_mode_map[pr] = adapter;
        }
#if LOG_ME
        string node_name(AiNodeGetName(p.m_node));
        string input_node_name(AiNodeGetName(input_node));
        string adapter_node_name(AiNodeGetName(adapter));
        fprintf(stderr, "%s: 1. linking %s to %s.input\n", __func__, input_node_name.c_str(), adapter_node_name.c_str());
        fprintf(stderr, "%s: 2. linking %s to %s.%s\n", __func__, adapter_node_name.c_str(), node_name.c_str(), p.m_param_name.c_str());
#endif
        AiNodeLink(input_node, "input", adapter);
        AiNodeLink(adapter, p.m_param_name.c_str(), p.m_node);
    }
}

// Sanitize input componets Ex.:
// xxx   -> yyy.r ==> xxx->float_to_rgbx(r)->yyy
// xxx.r -> yyy.r ==> xxx.r->float_to_rgbx(r)->yyy
//
void CShaderLinkSanitizer::SanitizeInputComponents()
{
    if (FindShadersWithInputComponents())
        FixShadersWithInputComponents();
}

// Sanitize output componets (ex. xxx.r -> yyy).
// In order to work, input components must have been previously senitized,
// that is there is no connection such as xxx.r->yyy.r
void CShaderLinkSanitizer::SanitizeOutputComponents()
{
    if (FindShadersWithOutputComponents())
        FixShadersWithOutputComponents();
}

// Do all.
//
void CShaderLinkSanitizer::Sanitize()
{
    // Order matters !
    SanitizeInputComponents();
    SanitizeOutputComponents();
}

// helpers:
int CShaderLinkSanitizer::GetTypeNbComponents(uint8_t in_type)
{
    switch (in_type)
    {
    case AI_TYPE_BOOLEAN: case AI_TYPE_BYTE: case AI_TYPE_FLOAT: case AI_TYPE_INT: case AI_TYPE_UINT:
    case AI_TYPE_STRING: case AI_TYPE_ENUM: case AI_TYPE_NODE: case AI_TYPE_ARRAY: case AI_TYPE_POINTER:
        return 1;
    case AI_TYPE_VECTOR2:
        return 2;
    case AI_TYPE_VECTOR: case AI_TYPE_RGB:
        return 3;
    case AI_TYPE_RGBA:
        return 4;
    case AI_TYPE_MATRIX:
        return 16;
    case AI_TYPE_NONE: default:
        return 0;
    }
}

string CShaderLinkSanitizer::GetParameterComponent(const string in_param_name, const uint8_t in_type, const uint8_t in_index)
{
    string out_param_component_name;
    string component_name;

    if (in_type == AI_TYPE_VECTOR2 || in_type == AI_TYPE_VECTOR)
    {
        if (in_index == 0)      component_name = "x";
        else if (in_index == 1) component_name = "y";
        else if (in_index == 2) component_name = "z";
    }
    else if (in_type == AI_TYPE_RGB || in_type == AI_TYPE_RGBA)
    {
        if (in_index == 0)      component_name = "r";
        else if (in_index == 1) component_name = "g";
        else if (in_index == 2) component_name = "b";
        else if (in_index == 3) component_name = "a";
        else if (in_index == 4) component_name ="average";
    }
    else
        return "";

    if (!in_param_name.empty())
    {
        out_param_component_name = in_param_name + "." + component_name;
        return out_param_component_name;
    }
    return component_name;
}

AtNode* CShaderLinkSanitizer::CreateNode(const AtString in_node_entry_name)
{
    string node_name = m_node_prefix +to_string(m_created_node_index++);
    return AiNode(m_universe, in_node_entry_name.c_str(), string (node_name+in_node_entry_name.c_str()).c_str() );
}

void CShaderLinkSanitizer::GetParameterValue(AtNode* in_node, string in_param_name, float* out_value)
{
    const AtNodeEntry* node_entry = AiNodeGetNodeEntry(in_node);
    const AtParamEntry* param_entry = AiNodeEntryLookUpParameter(node_entry, in_param_name.c_str());
    uint8_t param_type = AiParamGetType(param_entry);
    if (param_type == AI_TYPE_VECTOR)
    {
        AtVector value = AiNodeGetVec(in_node, in_param_name.c_str());
        out_value[0] = value.x;
        out_value[1] = value.y;
        out_value[2] = value.z;
    }
    else if (param_type == AI_TYPE_RGB)
    {
        AtRGB value = AiNodeGetRGB(in_node, in_param_name.c_str());
        out_value[0] = value.r;
        out_value[1] = value.g;
        out_value[2] = value.b;
    }
    else if (param_type == AI_TYPE_RGBA)
    {
        AtRGBA value = AiNodeGetRGBA(in_node, in_param_name.c_str());
        out_value[0] = value.r;
        out_value[1] = value.g;
        out_value[2] = value.b;
        out_value[3] = value.a;
    }
}