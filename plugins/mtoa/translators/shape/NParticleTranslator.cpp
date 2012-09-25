
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

#if MAYA_API_VERSION < 201200

   MString mayaNodeType = context.maya;
   CExtensionAttrHelper helper(mayaNodeType, "points");

   // node attributes
   CShapeTranslator::MakeCommonAttributes(helper);

   CAttrData data;

   data.defaultValue.BOOL = false;
   data.name = "aiExportParticleIDs";
   data.shortName = "ai_export_particle_ids";
   helper.MakeInputBoolean(data);

   data.defaultValue.STR = "";
   data.name = "aiExportAttributes";
   data.shortName = "ai_export_attributes";
   helper.MakeInputString(data);

   MStringArray  enumNames;
   enumNames.append("points");
   enumNames.append("spheres");
   enumNames.append("quads");
   //data.defaultValue.ENUM = 0; /// how do you do a default in an enum?
   data.name = "aiRenderPointsAs";
   data.shortName = "ai_render_points_as";
   data.enums= enumNames;
   helper.MakeInputEnum(data);

   data.defaultValue.FLT = 0;
   data.name = "aiMinParticleRadius";
   data.shortName = "ai_min_particle_radius";
   helper.MakeInputFloat(data);

   data.defaultValue.FLT = 1.0;
   data.name = "aiRadiusMultiplier";
   data.shortName = "ai_radius_multiplier";
   helper.MakeInputFloat(data);

   data.defaultValue.FLT = 1000000;
   data.name = "aiMaxParticleRadius";
   data.shortName = "ai_max_particle_radius";
   helper.MakeInputFloat(data);

   data.defaultValue.FLT = 0;
   data.name = "aiMinPixelWidth";
   data.shortName = "ai_min_pixel_width";
   helper.MakeInputFloat(data);

   data.defaultValue.BOOL = false;
   data.name = "aiDeleteDeadParticles";
   data.shortName = "ai_delete_dead_particles";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "aiInterpolateBlur";
   data.shortName = "ai_interpolate_blur";
   helper.MakeInputBoolean(data);

#endif

}

bool CNParticleTranslator::IsCached()
{
   MPlug plug = m_fnParticleSystem.findPlug("playFromCache");
   return !plug.isNull() && plug.asBool();
}

