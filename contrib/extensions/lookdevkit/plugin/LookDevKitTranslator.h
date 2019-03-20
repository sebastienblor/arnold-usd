#pragma once

#include "translators/shader/ShaderTranslator.h"


class CLookDevKitTranslator : public CShaderTranslator
{
public:
    virtual AtNode* CreateArnoldNodes();
    virtual void Export(AtNode* shader);

    static void* creator()
    {
        return new CLookDevKitTranslator();
    }

protected:
	virtual void NodeChanged(MObject& node, MPlug& plug);
	
private:
	void ExportRGBAChannels(AtNode *shader, const char *arnoldParam, const char *rgbParam, const char *alphaParam);
};
