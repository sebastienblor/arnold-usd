#include "ShapeTranslator.h"

// computes and sets the visibility mask as well as other shape attributes related to ray visibility
// (self_shadows, opaque)
void CShapeTranslator::ProcessRenderFlags(AtNode* node)
{
   AiNodeSetInt(node, "visibility", ComputeVisibility());

   MPlug plug;
   plug = GetFnNode().findPlug("aiSelfShadows");
   if (!plug.isNull()) AiNodeSetBool(node, "self_shadows", plug.asBool());

   plug = GetFnNode().findPlug("aiOpaque");
   if (!plug.isNull()) AiNodeSetBool(node, "opaque", plug.asBool());

   plug = GetFnNode().findPlug("receiveShadows");
   if (!plug.isNull()) AiNodeSetBool(node, "receive_shadows", plug.asBool());

   // Subsurface Scattering
   plug = GetFnNode().findPlug("aiSssUseGi");
   if (!plug.isNull()) AiNodeSetBool(node, "sss_use_gi", plug.asBool());

   plug = GetFnNode().findPlug("aiSssMaxSamples");
   if (!plug.isNull()) AiNodeSetInt(node, "sss_max_samples", plug.asInt());

   plug = GetFnNode().findPlug("aiSssSampleSpacing");
   if (!plug.isNull()) AiNodeSetFlt(node, "sss_sample_spacing", plug.asFloat());

}

// create attributes common to arnold shape nodes
//
void CShapeTranslator::MakeCommonAttributes(CBaseAttrHelper& helper)
{
   helper.MakeInput("sss_use_gi");
   helper.MakeInput("sss_max_samples");
   helper.MakeInput("sss_sample_spacing");

   helper.MakeInput("self_shadows");
   helper.MakeInput("opaque");

   MakeArnoldVisibilityFlags(helper);
}

