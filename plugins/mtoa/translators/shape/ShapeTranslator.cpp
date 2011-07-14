#include "ShapeTranslator.h"

// computes and sets the visibility mask as well as other shape attributes related to ray visibility
// (self_shadows, opaque)
void CShapeTranslator::ProcessRenderFlags(AtNode* node)
{
   AiNodeSetInt(node, "visibility", ComputeVisibility());

   MPlug plug;
   plug = FindMayaObjectPlug("aiSelfShadows");
   if (!plug.isNull()) AiNodeSetBool(node, "self_shadows", plug.asBool());

   plug = FindMayaObjectPlug("aiOpaque");
   if (!plug.isNull()) AiNodeSetBool(node, "opaque", plug.asBool());

   plug = FindMayaObjectPlug("receiveShadows");
   if (!plug.isNull()) AiNodeSetBool(node, "receive_shadows", plug.asBool());

   // Subsurface Scattering
   plug = FindMayaObjectPlug("aiSssUseGi");
   if (!plug.isNull()) AiNodeSetBool(node, "sss_use_gi", plug.asBool());

   plug = FindMayaObjectPlug("aiSssSampleSpacing");
   if (!plug.isNull()) AiNodeSetFlt(node, "sss_sample_spacing", plug.asFloat());

}

// create attributes common to arnold shape nodes
//
void CShapeTranslator::MakeCommonAttributes(CBaseAttrHelper& helper)
{
   helper.MakeInput("sss_use_gi");
   helper.MakeInput("sss_sample_distribution");
   helper.MakeInput("sss_sample_spacing");

   helper.MakeInput("self_shadows");
   helper.MakeInput("opaque");

   MakeArnoldVisibilityFlags(helper);
}

