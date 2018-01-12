#include "GpuCacheTranslator.h"

#include <maya/MRenderLineArray.h>
#include <maya/MRenderLine.h>
#include <maya/MDagPathArray.h>
#include <maya/MPointArray.h>
#include <maya/MHairSystem.h>
#include <maya/MFnPfxGeometry.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnNurbsCurve.h>

static bool s_alembicSupported = false;

void CGpuCacheTranslator::NodeInitializer(CAbTranslator context)
{
   s_alembicSupported = (AiNodeEntryLookUp("alembic") != NULL);

   CExtensionAttrHelper helper(context.maya, "procedural");
   CShapeTranslator::MakeCommonAttributes(helper);

   CAttrData data;
   data.defaultValue.BOOL() = true;
   data.name = "overrideLightLinking";
   data.shortName = "oll";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL() = true;
   data.name = "overrideShaders";
   data.shortName = "osh";
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
      MString geomPath = geomPlug.asString();
      AiNodeSetStr(shape, "objectPath", geomPath.asChar());
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
 