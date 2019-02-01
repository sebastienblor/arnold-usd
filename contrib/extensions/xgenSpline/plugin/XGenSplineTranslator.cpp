#include "XGenSplineTranslator.h"
#include "../common/XgSplineArnoldExpand.h"
#include "extension/Extension.h"
#include "utils/time.h"

#include <maya/MFnDagNode.h>
#include <maya/MFnPluginData.h>
#include <maya/MPxData.h>
#include <maya/MTime.h>

#include <sstream>
#include <string>
#include <vector>

void CXgSplineDescriptionTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "procedural");
   CShapeTranslator::MakeCommonAttributes(helper);

   CAttrData data;

   data.defaultValue.FLT() = 0.0f;
   data.name = "aiMinPixelWidth";
   data.shortName = "ai_min_pixel_width";
   helper.MakeInputFloat(data);

   MStringArray curveTypeEnum;
   curveTypeEnum.append("Ribbon");
   curveTypeEnum.append("Thick");
   data.defaultValue.INT() = 0;
   data.name = "aiMode";
   data.shortName = "ai_mode";
   data.enums= curveTypeEnum;
   helper.MakeInputEnum(data);
}

AtNode* CXgSplineDescriptionTranslator::CreateArnoldNodes()
{
   m_expandedProcedurals.clear();
   return AddArnoldNode("xgenProcedural");
}


void CXgSplineDescriptionTranslator::Delete()
{
   // If the procedural has been expanded at export,
   // we need to delete all the created nodes here 
   for (size_t i = 0; i < m_expandedProcedurals.size(); i++)
   {
      XgArnoldInternal::XgSplineProcedural* expandedProcedural = m_expandedProcedurals[i];
      if (expandedProcedural)
      {
         int numNodes = expandedProcedural->NumNodes();
         for (int i = 0; i < numNodes; ++i)
         {
            AtNode *node = expandedProcedural->GetNode(i);
            if (node == NULL) continue; 
            AiNodeDestroy(node);
         }
         //expandedProcedural->Cleanup();

         delete expandedProcedural;
      }
   }
   m_expandedProcedurals.clear();
   CShapeTranslator::Delete();
}

void CXgSplineDescriptionTranslator::Export(AtNode* procedural)
{  
   MStatus status;

   MFnDagNode fnDagNode(m_dagPath);

   // Set matrix for step 0
   ExportMatrix(procedural);

   // Export render flags
   ProcessRenderFlags(procedural);
   ExportLightLinking(procedural);

   // Export shaders
   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), 0);
   if (!shadingGroupPlug.isNull())
   {
      AtNode* shader = ExportConnectedNode(shadingGroupPlug);
      if (shader)
      {
         AiNodeSetPtr(procedural, "shader", shader);
      }
   }

   // Set procedural parameters
   {
      AiNodeSetBool(procedural, "load_at_init", true);
      AiNodeSetVec(procedural, "min", -1.0f, -1.0f, -1.0f);
      AiNodeSetVec(procedural, "max",  1.0f,  1.0f,  1.0f);
   }

   // Export the sample frames
   AiNodeDeclare(procedural, "sampleTimes", "constant ARRAY FLOAT");
   if (IsMotionBlurEnabled(MTOA_MBLUR_DEFORM))
   {
      // Motion blur is enabled. Output the motion frames.
      unsigned int motionFramesCount;
      const double *motionFrames = GetMotionFrames(motionFramesCount);
        
      AtArray* sampleTimes = AiArrayAllocate(motionFramesCount, 1, AI_TYPE_FLOAT);
      for (unsigned int i = 0; i < motionFramesCount; i++)
         AiArraySetFlt(sampleTimes, i, float(motionFrames[i]));
   
      AiNodeSetArray(procedural, "sampleTimes", sampleTimes);
   }
   else
   {
      // No motion blur. Output the current frame.
      const double sampleFrame = GetExportFrame();

      AtArray* sampleTimes = AiArrayAllocate(1, 1, AI_TYPE_FLOAT);
      AiArraySetFlt(sampleTimes, 0, float(sampleFrame));
      AiNodeSetArray(procedural, "sampleTimes", sampleTimes);
   }

   // Export frame-per-second
   {
      const float fps = float(MTime(1.0, MTime::kSeconds).asUnits(MTime::uiUnit()));
      AiNodeDeclare(procedural, "fps", "constant FLOAT");
      AiNodeSetFlt(procedural, "fps", fps);
   }

   // Export the spline data (opaque)
   ExportSplineData(procedural, 0);

   // aiMinPixelWidth
   {
      AiNodeDeclare(procedural, "ai_min_pixel_width", "constant FLOAT");
      AiNodeSetFlt(procedural, "ai_min_pixel_width", fnDagNode.findPlug("ai_min_pixel_width", true).asFloat());
   }

   // aiMode
   {
      AiNodeDeclare(procedural, "ai_mode", "constant INT");
      int aiMode = fnDagNode.findPlug("ai_mode", true).asInt();

#ifdef XGEN_ARNOLD_ORIENTATIONS
      MPlug faceCamAttr = fnDagNode.findPlug("faceCamera", true);
      if (aiMode == 0 && (!faceCamAttr.isNull()) && (!faceCamAttr.asBool()))
         aiMode = 2; // oriented
#endif

      AiNodeSetInt(procedural, "ai_mode", aiMode);
   }
   
}

void CXgSplineDescriptionTranslator::ExportMotion(AtNode* procedural)
{
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(procedural);

   // Same for object deformation, early out if it's not set.
   if (!IsMotionBlurEnabled(MTOA_MBLUR_DEFORM)) return;

   ExportSplineData(procedural, GetMotionStep());
}

extern void CXgSplineDescriptionTranslator_ExportSplineData(MDagPath& dagPath, AtNode* procedural, unsigned int step);
void CXgSplineDescriptionTranslator::ExportSplineData(AtNode* procedural, unsigned int step)
{
   CXgSplineDescriptionTranslator_ExportSplineData(m_dagPath, procedural, step);
}

void CXgSplineDescriptionTranslator::PostExport(AtNode *node)
{
   // For now we're only expanding the procedurals during export if we are on an interactive render
   // (see ticket #2599). This way the arnold render doesn't have to gather XGen data, and IPR
   // can be updated while tweaking the XGen attributes
   if (GetSessionOptions().GetSessionMode() == MTOA_SESSION_ASS)
      return;

   ExpandProcedural();
}


// For now we're only expanding the procedurals during export if we are on an interactive render
// (see ticket #2599). This way the arnold render doesn't have to gather XGen data, and IPR
// can be updated while tweaking the XGen attributes
void CXgSplineDescriptionTranslator::ExpandProcedural()
{
   if (!m_expandedProcedurals.empty())
      return;

   // setting this global variable to the current translator so that it is known by the callback.
   AtNode *node = GetArnoldNode();
   m_expandedProcedurals.push_back(new XgArnoldInternal::XgSplineProcedural());
   m_expandedProcedurals.back()->Init( node, false ); // "false" means that we don't want the created nodes to set the procedural parent

   // FIXME verify if we need to do something about the procedural matrix ?

   // in theory we could simply delete the procedural node, but I'm afraid of the consequences it may
   // have if GetArnoldNode returns NULL. So for safety we're just disabling this node for now
   AiNodeSetDisabled(node, true);
   
}
// this forces the refresh during IPR. All nodes have to be deleted
// which will happen in the Delete() function
void CXgSplineDescriptionTranslator::RequestUpdate()
{
   SetUpdateMode(AI_RECREATE_NODE);
   CShapeTranslator::RequestUpdate();
}
