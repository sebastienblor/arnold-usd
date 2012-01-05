
#include "NParticleTranslator.h"
#include "render/RenderSession.h"
#include "attributes/AttrHelper.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MString.h>
#include <maya/MFnMesh.h>
#include <maya/MVectorArray.h>
#include <maya/MStringArray.h>

#include <ai_msg.h>
#include <ai_nodes.h>


void CNParticleTranslator::NodeInitializer(CAbTranslator context)
{
}


