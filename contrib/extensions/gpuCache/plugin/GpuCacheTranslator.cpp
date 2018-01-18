#include "GpuCacheTranslator.h"

#include <maya/MRenderLineArray.h>
#include <maya/MRenderLine.h>
#include <maya/MDagPathArray.h>
#include <maya/MPointArray.h>
#include <maya/MHairSystem.h>
#include <maya/MFnPfxGeometry.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnNurbsCurve.h>

#include <algorithm>
#include <string>
static bool s_alembicSupported = false;

void CGpuCacheTranslator::NodeInitializer(CAbTranslator context)
{
   s_alembicSupported = (AiNodeEntryLookUp("alembic") != NULL);

   CExtensionAttrHelper helper(context.maya, "procedural");
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

   AiNodeSetFlt(shape, "shutter_start", AiNodeGetFlt(shape, "motion_start"));
   AiNodeSetFlt(shape, "shutter_end", AiNodeGetFlt(shape, "motion_end"));
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
   

