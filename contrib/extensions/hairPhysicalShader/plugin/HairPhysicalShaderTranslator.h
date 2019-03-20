#pragma once

#include "translators/shader/ShaderTranslator.h"


class CHairPhysicalShaderTranslator : public CShaderTranslator
{
public:
    virtual AtNode* CreateArnoldNodes();
    virtual void Export(AtNode* shader);

    static void NodeInitializer(CAbTranslator context);
    static void* creator()
    {
        return new CHairPhysicalShaderTranslator();
    }

private:
    
    // Export the Maya plug to the adapter node input
    void ExportAdapterParameter(AtNode* adapter, const char* input, const char* plugName);

    // Return true if the color attribute is black
    bool IsBlack(const char* plugName);

    // Return true if the float attribute is zero
    bool IsZero(const char* plugName);
};
