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

   CExtensionAttrHelper helper(context.maya, "alembic");
   CShapeTranslator::MakeCommonAttributes(helper);

   CAttrData data;
   data.defaultValue.BOOL() = false;
   data.name = "aiPullUserParams";
   data.shortName = "apup";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "aiInfo";
   data.shortName = "aiin";
   helper.MakeInputBoolean(data);

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

}

AtNode* CGpuCacheTranslator::CreateArnoldNodes()
{
   if (!s_alembicSupported) return NULL;

   return AddArnoldNode("alembic");
}

void CGpuCacheTranslator::Export( AtNode *shape )
{
   if (s_alembicSupported == false || shape == NULL)
      return;

   ExportMatrix(shape);
   ProcessRenderFlags(shape);

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
   AiNodeSetFlt(shape, "frame", curTime.value());

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

   AiNodeSetBool(shape, "pull_user_params", FindMayaPlug( "aiPullUserParams" ).asBool());
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
}


void CGpuCacheTranslator::ExportShaders()
{
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
   if (!IsTransformPlug(plug))
      SetUpdateMode(AI_RECREATE_NODE);

   CShapeTranslator::NodeChanged(node, plug);
}
   

