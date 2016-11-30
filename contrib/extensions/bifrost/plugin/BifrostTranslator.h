#pragma once

#include "translators/shape/ShapeTranslator.h"
#include "translators/DagTranslator.h"
#include "translators/NodeTranslator.h"


class BifrostShapeTranslator : public CShapeTranslator
{
public:

	enum RenderType {
		NONE,
		AERO,
		LIQUID_POLYMESH,
		LIQUID_IMPLICIT,
		POINT
	};

	enum BIFType {
		VOXEL,
		PARTICLE
	};

	AtNode* CreateArnoldNodes();
	virtual void Export(AtNode* shape);
	virtual void Update(AtNode* shape);
	void ExportMotion(AtNode*);

	void UpdatePoint(AtNode *shape);
	void UpdateAero(AtNode *shape);
	void UpdateLiquidPolyMesh(AtNode *shape);
	void UpdateLiquidImplicit(AtNode *shape);

	static void* creator() {
        return new BifrostShapeTranslator();
	}

	static void NodeInitializer( CAbTranslator context );

private:

	virtual void ExportBifrostShader();
   
	RenderType c_renderType;
	BIFType c_bifType;
	std::string c_object;
	std::string c_file;
};
