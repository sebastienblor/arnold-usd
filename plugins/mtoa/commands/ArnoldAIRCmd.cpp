#include "ArnoldAIRCmd.h"

void* CArnoldAIRCmd::creator()
{
	return new CArnoldAIRCmd();
}

MSyntax CArnoldAIRCmd::newSyntax()
{
	MSyntax syntax;

	return syntax;
}

MStatus CArnoldAIRCmd::doIt(const MArgList& argList)
{
	return MS::kSuccess;
}
