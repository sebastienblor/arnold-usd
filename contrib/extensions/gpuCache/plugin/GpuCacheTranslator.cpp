#include "GpuCacheTranslator.h"

#include <maya/MRenderLineArray.h>
#include <maya/MRenderLine.h>
#include <maya/MDagPathArray.h>
#include <maya/MPointArray.h>
#include <maya/MHairSystem.h>
#include <maya/MFnPfxGeometry.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MAnimControl.h>
#include <maya/MSelectionList.h>
#include <maya/MDGModifier.h>
#include <maya/MDGMessage.h>
#include <maya/MEventMessage.h>

#include <algorithm>
#include <string>
static bool s_alembicSupported = false;

const char* NodeTypes[] =
{
   "polymesh",
   "curves",
   "nurbs",
   "points",
   NULL
};

void CGpuCacheTranslator::NodeInitializer(CAbTranslator context)
{
   s_alembicSupported = (AiNodeEntryLookUp("alembic") != NULL);
   if (!s_alembicSupported)
      return;

   CExtensionAttrHelper helper(context.maya, "alembic");
   CShapeTranslator::MakeCommonAttributes(helper);

   helper.MakeInput("make_instance");
   helper.MakeInput("exclude_xform");
   helper.MakeInput("flip_v");

   helper.MakeInput("visibility_ignore");
   helper.MakeInput("expand_hidden");

   helper.MakeInput("velocity_ignore");
   helper.MakeInput("velocity_scale");

   helper.MakeInput("radius_attribute");
   helper.MakeInput("radius_default");
   helper.MakeInput("radius_scale");

   helper.MakeInput("namespace");
   helper.MakeInput("nameprefix");

   CAttrData data;
   data.defaultValue.BOOL() = false;
   data.name = "aiOverrideFrame";
   data.shortName = "aiOverrideFrame";
   helper.MakeInputBoolean(data);

   data.defaultValue.FLT() = 0.0f;
   data.name = "aiFrame";
   data.shortName = "aiFrame";
   helper.MakeInputFloat(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiPullUserParams";
   data.shortName = "apup";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiInfo";
   data.shortName = "aiin";
   helper.MakeInputBoolean(data);

   //// operators

   data.name = "operators";
   data.shortName = "operators";
   data.type = AI_TYPE_NODE;
   data.isArray = true;
   helper.MakeInput(data);

   //// userattrs

   std::vector<CAttrData> children(3);

   children[0].name = "attrName";
   children[0].shortName = "attr_name";
   children[0].type = AI_TYPE_STRING;

   children[1].name = "attrValue";
   children[1].shortName = "attr_value";
   children[1].type = AI_TYPE_STRING;

   MStringArray  enumTypes;

   for (unsigned int i = 0; NodeTypes[i] != NULL; i++)
   {
      enumTypes.append ( NodeTypes[i] );
   }

   children[2].defaultValue.INT() = 0;
   children[2].enums= enumTypes;
   children[2].name = "attrNodeType";
   children[2].shortName = "attr_node_type";
   children[2].type = AI_TYPE_ENUM;

   data.name = "aiNodeAttrs";
   data.shortName = "aina";
   data.isArray = true;

   helper.MakeInputCompound(data, children);

   //// aiOverrides

   std::vector<CAttrData> ovrchildren(4);

   ovrchildren[0].name = "abcPath";
   ovrchildren[0].shortName = "abc_path";
   ovrchildren[0].type = AI_TYPE_STRING;

   ovrchildren[1].name = "abcShader";
   ovrchildren[1].shortName = "abc_shader";
   ovrchildren[1].type = AI_TYPE_NODE;

   ovrchildren[2].name = "abcDisplacement";
   ovrchildren[2].shortName = "abc_displacement";
   ovrchildren[2].type = AI_TYPE_NODE;

   ovrchildren[3].name = "abcOverrides";
   ovrchildren[3].shortName = "abc_overrides";
   ovrchildren[3].isArray = true;
   ovrchildren[3].type = AI_TYPE_STRING;

   data.name = "aiOverrides";
   data.shortName = "aiovr";
   data.isArray = true;

   helper.MakeInputCompound(data, ovrchildren);

}

AtNode* CGpuCacheTranslator::CreateArnoldNodes()
{
   if (!s_alembicSupported) return NULL;

   return AddArnoldNode("alembic");
}

// Callback is called whenever a gpuCache node is created
void CGpuCacheTranslator::timeChangedCallback(void* clientData)
{

   CGpuCacheTranslator * translator = static_cast< CGpuCacheTranslator* >(clientData);
   if (translator != NULL)
   {
      translator->SetUpdateMode(AI_RECREATE_NODE);
      translator->RequestUpdate();
   }

   // AtNode *node =  translator->GetArnoldNode();

   // MTime curTime = MAnimControl::currentTime();
   // AiNodeSetFlt(node, "frame", float(curTime.value()));
}

void CGpuCacheTranslator::AddUpdateCallbacks()
{

   MStatus status;
   MCallbackId id;

   MObject object = GetMayaObject();
   id = MEventMessage::addEventCallback("timeChanged", 
                                        CGpuCacheTranslator::timeChangedCallback,
                                        this,
                                        &status);

   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   CShapeTranslator::AddUpdateCallbacks();
}

void CGpuCacheTranslator::Export( AtNode *shape )
{
   if (s_alembicSupported == false || shape == NULL)
      return;

   // ExportMatrix(shape);
   // ProcessRenderFlags(shape);

   // export gpuCache parameters   
   MPlug filenamePlug = FindMayaPlug("cacheFileName");
   if (!filenamePlug.isNull())
   {
      MString filename = filenamePlug.asString();
      AiNodeSetStr(shape, "filename", filename.asChar());
   }

   MPlug geomPlug = FindMayaPlug("cacheGeomPath");
   if (!geomPlug.isNull())
   {
      std::string geomPath = geomPlug.asString().asChar();
      std::replace( geomPath.begin(), geomPath.end(), '|', '/'); // Maya converts '/' into '|' 
      AiNodeSetStr(shape, "objectpath", geomPath.c_str());
   } 

   if (RequiresShaderExport())
      ExportShaders();

   MTime curTime = MAnimControl::currentTime();

   if (FindMayaPlug("aiOverrideFrame").asBool())
      AiNodeSetFlt(shape, "frame", float(FindMayaPlug("aiFrame").asFloat()));
   else
      AiNodeSetFlt(shape, "frame", float(curTime.value()));

   float fps = 24.0f;
   MTime::Unit unit = curTime.unit();
   if (unit!=MTime::kInvalid)
   {
     MTime time(1.0, MTime::kSeconds);
     fps = static_cast<float>(time.as(unit));
   }
   if (fps <= 0.f )
   {
     fps = 24.0f;
   }

   AiNodeSetFlt(shape, "fps", fps);
   AiNodeSetFlt(shape, "shutter_start", AiNodeGetFlt(shape, "motion_start"));
   AiNodeSetFlt(shape, "shutter_end", AiNodeGetFlt(shape, "motion_end"));

   AiNodeSetBool(shape, "make_instance", FindMayaPlug( "aiMakeInstance" ).asBool());
   AiNodeSetBool(shape, "exclude_xform", FindMayaPlug("aiExcludeXform").asBool());
   AiNodeSetBool(shape, "flip_v", FindMayaPlug("aiFlipV").asBool());

   AiNodeSetBool(shape, "visibility_ignore", FindMayaPlug("aiVisibilityIgnore").asBool());
   AiNodeSetBool(shape, "expand_hidden", FindMayaPlug("aiExpandHidden").asBool());

   AiNodeSetBool(shape, "velocity_ignore", FindMayaPlug("aiVelocityIgnore").asBool());
   AiNodeSetFlt(shape, "velocity_scale", FindMayaPlug("aiVelocityScale").asFloat());

   AiNodeSetStr(shape, "radius_attribute", FindMayaPlug("aiRadiusAttribute").asString().asChar());
   AiNodeSetFlt(shape, "radius_default", FindMayaPlug("aiRadiusDefault").asFloat());
   AiNodeSetFlt(shape, "radius_scale", FindMayaPlug("aiRadiusScale").asFloat());

   AiNodeSetBool(shape, "pull_user_params", FindMayaPlug( "aiPullUserParams" ).asBool());

   MString namePrefix = FindMayaPlug("aiNameprefix").asString();
   if (namePrefix.length() > 0)
      AiNodeSetStr(shape, "nameprefix", namePrefix.asChar());
   else
      AiNodeResetParameter(shape, "nameprefix");

   // now the user attributes
   MPlug arrayPlug = FindMayaPlug("aiNodeAttrs");

   for (unsigned int i = 0; i < arrayPlug.numElements (); i++)
   {
      MPlug namePlug = arrayPlug[i].child(0);
      MPlug valuePlug = arrayPlug[i].child(1);
      MPlug typePlug = arrayPlug[i].child(2);

      const char* node_type = NodeTypes[typePlug.asInt()];

      std::string attribute_name, attribute_set;
      attribute_name = std::string(node_type) + ":" + std::string(namePlug.asString().asChar());

      attribute_set = attribute_name + " " + std::string(valuePlug.asString().asChar());

      // get attr_type

      const AtNodeEntry* entry = AiNodeEntryLookUp(node_type);

      const AtParamEntry* param = AiNodeEntryLookUpParameter(entry, namePlug.asString().asChar());

      if (param != NULL)
      {

         int arnold_type = AiParamGetType(param);

         std::string declStr = "constant ";
         declStr += arnold_type == AI_TYPE_ENUM ? "STRING" : AiParamGetTypeName(arnold_type);
         if (arnold_type == AI_TYPE_ARRAY)
         {
            int array_type = AiArrayGetType(AiParamGetDefault(param)->ARRAY());
            declStr += " ";
            declStr += AiParamGetTypeName(array_type);
         }

         AiNodeDeclare( shape, attribute_name.c_str(), declStr.c_str() );
         AiNodeSetAttributes(shape, attribute_set.c_str());
      }
   }

   ExportProcedural(shape);
}


void CGpuCacheTranslator::ExportShaders()
{
   if (s_alembicSupported == false)
      return;

   AtNode *node = GetArnoldNode();
   if (node == NULL)
      return;

   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroupPlug.isNull())
   {
      AtNode *shader = ExportConnectedNode(shadingGroupPlug);
      if (shader != NULL)
      {
         AiNodeSetPtr(node, "shader", shader);
      }
      else
      {         
         AiNodeSetPtr(node, "shader", NULL);
      }
   }

   MPlug OverridesPlug = FindMayaPlug("aiOverrides");
   unsigned nelems = OverridesPlug.numElements();
   MPlug elemPlug;
   for (unsigned int i = 0; i < nelems; ++i)
   {
      elemPlug = OverridesPlug[i];

      MPlug shaderPlug = elemPlug.child(1);
      MPlugArray shaderConnections;
      shaderPlug.connectedTo(shaderConnections, true, false);
      if (shaderConnections.length() > 0)
         ExportConnectedNode(shaderConnections[0]);

      MPlug dispPlug = elemPlug.child(2);
      MPlugArray dispConnections;
      dispPlug.connectedTo(dispConnections, true, false);
      if (dispConnections.length() > 0)
         ExportConnectedNode(dispConnections[0]);

      // TODO export and create operators from overrides, 
      // currently we make them in the interactive session
   }
}

void CGpuCacheTranslator::ExportMotion(AtNode *shape)
{
   if (s_alembicSupported == false || shape == NULL)
      return;
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;


   // Set transform matrix
   ExportMatrix(shape);

}

void CGpuCacheTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   if (s_alembicSupported == false)
      return;
   
   // Check if
   if (!IsTransformPlug(plug))
      SetUpdateMode(AI_RECREATE_NODE);
   CShapeTranslator::NodeChanged(node, plug);
}
   

