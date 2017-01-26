#pragma once

#include "translators/shader/ShaderTranslator.h"
#include "translators/NodeTranslator.h"

class BifrostHydroTranslator : public CShaderTranslator
{
	public:

		virtual AtNode* CreateArnoldNodes();
		virtual void Export(AtNode* shader);

		static void* creator() { return new BifrostHydroTranslator(); }
		static void NodeInitializer(CAbTranslator context);
};

class BifrostPointTranslator : public CShaderTranslator
{
	public:

		virtual AtNode* CreateArnoldNodes();
		virtual void Export(AtNode* shader);

		static void* creator() { return new BifrostPointTranslator(); }
		static void NodeInitializer(CAbTranslator context);
};

class BifrostAeroTranslator : public CShaderTranslator
{
	public:

		virtual AtNode* CreateArnoldNodes();
		virtual void Export(AtNode* shader);

		static void* creator() { return new BifrostAeroTranslator(); }
		static void NodeInitializer(CAbTranslator context);
};
