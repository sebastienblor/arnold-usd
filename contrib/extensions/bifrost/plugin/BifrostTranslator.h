#pragma once

#include "translators/shape/ShapeTranslator.h"

class BifrostTranslator : public CShapeTranslator
{
public:
    AtNode* CreateArnoldNodes() override;
    void Export( AtNode *shape ) override;
    void ExportMotion( AtNode *shape ) override;

    void RequestUpdate() override;

    static void* creator() { return new BifrostTranslator(); }

    static void NodeInitializer( CAbTranslator context );

    static void ClearCallbacks();
private:
    // common
    void ExportShape(MFnDagNode& dagNode, AtNode *shape);

    // surface
    void ExportSurface(MFnDagNode& dagNode, AtNode *shape);
    void ExportPolymesh(MFnDagNode& dagNode, AtNode *shape);
    void ExportImplicit(MFnDagNode& dagNode, AtNode *shape);
    void ExportClipping(const MFnDagNode& dagNode, AtNode *shape);
    void ExportOceanPlane(const MFnDagNode& dagNode, AtNode *shape);

    // volume
    void ExportVolume(MFnDagNode& dagNode, AtNode *shape);

    // points
    void ExportPoints(MFnDagNode& dagNode, AtNode *shape);

    MMatrix getRelativeMatrix(const MPlug& src);

    void ExportBifrostShader();
    void ExportDisplacement();
};
