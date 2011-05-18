#include "ArnoldShapeTranslator.h"

// computes and sets the visibility mask as well as other shape attributes related to ray visibility
// (self_shadows, opaque)
void CArnoldShapeTranslator::ProcessRenderFlags(AtNode* node)
{
   AiNodeSetInt(node, "visibility", ComputeVisibility());

   MPlug plug;
   plug = GetFnNode().findPlug("selfShadows");
   if (!plug.isNull()) AiNodeSetBool(node, "self_shadows", plug.asBool());

   plug = GetFnNode().findPlug("opaque");
   if (!plug.isNull()) AiNodeSetBool(node, "opaque", plug.asBool());

   plug = GetFnNode().findPlug("receiveShadows");
   if (!plug.isNull()) AiNodeSetBool(node, "receive_shadows", plug.asBool());

   // Subsurface Scattering
   plug = GetFnNode().findPlug("sss_use_gi");
   if (!plug.isNull()) AiNodeSetBool(node, "sss_use_gi", plug.asBool());

   plug = GetFnNode().findPlug("sss_max_samples");
   if (!plug.isNull()) AiNodeSetInt(node, "sss_max_samples", plug.asInt());

   plug = GetFnNode().findPlug("sss_sample_spacing");
   if (!plug.isNull()) AiNodeSetFlt(node, "sss_sample_spacing", plug.asFloat());

}

// create attributes common to arnold shape nodes
//
void CArnoldShapeTranslator::MakeCommonAttributes(CBaseAttrHelper& helper)
{
   helper.MakeInput("sss_use_gi");
   helper.MakeInput("sss_max_samples");
   helper.MakeInput("sss_sample_spacing");

   helper.MakeInput("self_shadows");
   helper.MakeInput("opaque");

   MakeArnoldVisibilityFlags(helper);
}

