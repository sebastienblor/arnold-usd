#pragma once

#include "translators/shape/ShapeTranslator.h"

class BifrostTranslator : public CShapeTranslator
{
public:
    AtNode* CreateArnoldNodes();
    virtual void Export( AtNode *shape );
    void ExportMotion( AtNode *shape );

    void RequestUpdate();

    static void* creator() { return new BifrostTranslator(); }

    static void NodeInitializer( CAbTranslator context );

private:
    void ExportSurface(MFnDagNode& dagNode, AtNode *shape);
    void ExportPolymesh(MFnDagNode& dagNode, AtNode *shape);

    void ExportBifrostShader();
    void ExportDisplacement();
};
