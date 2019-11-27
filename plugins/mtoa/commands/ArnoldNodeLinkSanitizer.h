#pragma once

#include <iostream>
#include <ai_universe.h>
#include <string>
#include <utility>
#include <map>
#include <vector>


using namespace std;

typedef struct
{
    AtNode*     m_node;
    string      m_param_name;
    uint8_t     m_param_type;
    int         m_param_nb_components;
} Parameter;


class CShaderLinkSanitizer
{
public:
    CShaderLinkSanitizer(AtUniverse* in_universe , const char* node_prefix);
    ~CShaderLinkSanitizer(){};
    void SanitizeInputComponents();
    void SanitizeOutputComponents();
    void Sanitize();

private:
    AtUniverse* m_universe = NULL;
    int         m_created_node_index = 0;
    vector<Parameter> m_parameters;
    const char* m_node_prefix ;

    bool FindShadersWithInputComponents();
    void FixShadersWithInputComponents();
    bool FindShadersWithOutputComponents();
    void FixShadersWithOutputComponents();

    int     GetTypeNbComponents(uint8_t in_type);
    string  GetParameterComponent(const string in_param_name, const uint8_t in_type, const uint8_t in_index);
    AtNode* CreateNode(const AtString in_node_entry_name);
    void    GetParameterValue(AtNode* in_node, string in_param_name, float* out_value);

    const string m_rgba_from_int[4] = { "r", "g", "b", "a" };
};
