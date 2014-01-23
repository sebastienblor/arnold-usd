#pragma once

#include <maya/MPxCommand.h>
#include <maya/MString.h>
#include <maya/MFragmentManager.h>
#include <maya/MArgList.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>

class CArnoldVP2Command : public MPxCommand{
public:
    static void* creator();

    virtual MStatus doIt(const MArgList& args);
};

void* CArnoldVP2Command::creator()
{
    return new CArnoldVP2Command();
}

#include <fstream>
#include <string>

MStatus CArnoldVP2Command::doIt(const MArgList& args)
{
    MStringArray sArray;
    /*MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
    if (theRenderer == 0)
        return MS::kFailure;
    MHWRender::MFragmentManager* fmg = theRenderer->getFragmentManager();
    if (fmg == 0)
        return MS::kFailure;
    fmg->fragmentList(sArray);
    for (unsigned int i = 0; i < sArray.length(); ++i)
    {
        std::string fragmentName = sArray[i].asChar();
        std::fstream fs((std::string("/work/mayaFragments/") + fragmentName).c_str(), std::ios::out);
        MString fragmentString;
        fmg->getFragmentXML(fragmentName.c_str(), fragmentString);
        fs << fragmentString.asChar();
    }*/
    return MS::kSuccess;
}
