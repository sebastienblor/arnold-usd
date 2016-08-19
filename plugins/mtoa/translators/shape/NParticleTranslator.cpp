#include "translators/NodeTranslatorImpl.h"
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

CNParticleTranslator::CNParticleTranslator() :
  CParticleTranslator()
{
  // Just for debug info, translator creates whatever arnold nodes are required
  // through the CreateArnoldNodes method
  m_impl->m_abstract.arnold = "points";
}


void CNParticleTranslator::NodeInitializer(CAbTranslator context)
{

}

bool CNParticleTranslator::IsCached()
{
   MPlug plug = m_fnParticleSystem.findPlug("playFromCache");
   return !plug.isNull() && plug.asBool();
}

bool CNParticleTranslator::IsNParticle()
{
   return true;
}

