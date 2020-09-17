#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif

#include "BifShapeTranslator.h"
#include "../nodes/BifrostShapeNode.h"
#include "extension/Extension.h"

extern "C"
{
    DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;

        extension.Requires ( "bifrostGraph" ); // require the bifrostGraph plugin
        extension.RegisterTranslator("bifShape", // translate bifShape nodes
                                     "",
                                     CBifShapeTranslator::creator,
                                     CBifShapeTranslator::NodeInitializer);
        extension.RegisterTranslator("bifrostGraphShape", // translate bifrost graph nodes
                                     "",
                                     CBifShapeTranslator::creator,
                                     CBifShapeTranslator::NodeInitializer);
        extension.RegisterNode("arnoldBifrostShape",
                        CBifrostShapeNode::id,
                        CBifrostShapeNode::creator,
                        CBifrostShapeNode::initialize,
                        MPxNode::kLocatorNode, 
                        CBifrostShapeNode::s_classification);
    }

    DLLEXPORT void deinitializeExtension ( CExtension& extension )
    {
    }
}
