#pragma once

#include <maya/MPxCommand.h>
#include <maya/MString.h>
#include <maya/MFragmentManager.h>
#include <maya/MArgList.h>

class CArnoldVP2Command : public MPxCommand{
public:
	static void* creator();

	virtual MStatus doIt(const MArgList& args);
};

void* CArnoldVP2Command::creator()
{
	return new CArnoldVP2Command();
}

MStatus CArnoldVP2Command::doIt(const MArgList& args)
{
	return MS::kSuccess;
}
