#pragma once

#include "translators/shape/ShapeTranslator.h"

class BifrostTranslator : public CShapeTranslator
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
		virtual void Export( AtNode *shape );
		void ExportMotion( AtNode *shape );

        void ExportLiquidAttributes( MFnDagNode& bifrostDesc, AtNode *shape );
		void ExportPoint(AtNode *shape);
		void ExportAero(AtNode *shape);
		void ExportLiquidPolyMesh(AtNode *shape);
		void ExportLiquidImplicit(AtNode *shape);

		void ExportMatrixWithSpaceScale(AtNode *shape, float spaceScale);

		void RequestUpdate();

		static void* creator() {
            return new BifrostTranslator();
		}

		static void NodeInitializer( CAbTranslator context );

	private:

		virtual void ExportBifrostShader();
   
		RenderType c_renderType;
		BIFType c_bifType;
		std::string c_object;
		std::string c_file;
};
