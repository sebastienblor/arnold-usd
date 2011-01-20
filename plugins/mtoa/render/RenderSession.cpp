
#include "RenderSession.h"
#include "RenderOptions.h"
#include "OutputDriver.h"
#include "maya_scene/MayaScene.h"

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_plugins.h>
#include <ai_render.h>
#include <ai_threads.h>
#include <ai_universe.h>
#include <ai_ray.h>

#include <cstdio>

#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDagNode.h>
#include <maya/MComputation.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MRenderUtil.h> 
#include <maya/MStatus.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

extern AtNodeMethods* mtoa_driver_mtd;

static CRenderSession* s_renderSession = NULL;

// This is the code for the render thread. This thread is used only to run the AiRender() process outside of the main thread.
static unsigned int RenderThread(AtVoid* data)
{
   AiRender(AI_RENDER_MODE_CAMERA);

   return 0;
}

// Cheap singleton
CRenderSession* CRenderSession::GetInstance()
{
   if (!s_renderSession)
      s_renderSession = new CRenderSession();

   return s_renderSession;
}

CMayaScene* CRenderSession::GetMayaScene()
{
   return m_scene;
}

void CRenderSession::Init(ExportMode exportMode, bool preMel, bool preLayerMel, bool preFrameMel)
{

   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] ERROR: There can only be one RenderSession active.");
      return;
   }

   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   if (preMel)
   {
      ExecuteScript(renderGlobals.preMel);
   }
   if (preLayerMel)
   {
      ExecuteScript(renderGlobals.preRenderLayerMel);
   }
   if (preFrameMel)
   {
      ExecuteScript(renderGlobals.preRenderMel);
   }

   m_scene = new CMayaScene;          
   m_renderOptions.GetFromMaya(m_scene);


   AiBegin();

   m_renderOptions.SetupLog();

   MString resolvedPathList = m_renderOptions.pluginsPath().expandEnvironmentVariablesAndTilde();

   MStringArray pluginPaths;
#ifdef _WIN32
   resolvedPathList.split(';', pluginPaths);
#else
   resolvedPathList.split(':', pluginPaths);
#endif
   for (unsigned int i=0; i<pluginPaths.length(); ++i)
   {
      MString pluginPath = pluginPaths[i];
      if (pluginPath.length() > 0)
      {
         AiLoadPlugins(pluginPath.asChar());
      }
   }

   m_scene->ExportToArnold(exportMode);

}

void CRenderSession::End(bool postMel, bool postLayerMel, bool postFrameMel)
{
   AiEnd();

   MCommonRenderSettingsData renderGlobals;
   MRenderUtil::getCommonRenderSettings(renderGlobals);

   if (postFrameMel)
   {
      ExecuteScript(renderGlobals.postRenderMel);
   }
   if (postLayerMel)
   {
      ExecuteScript(renderGlobals.postRenderLayerMel);
   }
   if (postMel)
   {
      ExecuteScript(renderGlobals.postMel);
   }

   delete m_scene;
}

void CRenderSession::Reset(bool postMel, bool postLayerMel, bool postFrameMel, bool preMel, bool preLayerMel, bool preFrameMel)
{
   if (IsActive())
   {
      End(postMel, postLayerMel, postFrameMel);
   }

   Init(MTOA_EXPORT_ALL, preMel, preLayerMel, preFrameMel);
}

void CRenderSession::SetBatch(bool batch)
{
   m_renderOptions.SetBatch(batch);
}

void CRenderSession::SetWidth(int width)
{
   if (width != -1)
      m_renderOptions.SetWidth(width);
}

void CRenderSession::SetHeight(int height)
{
   if (height != -1)
      m_renderOptions.SetHeight(height);
}

void CRenderSession::SetCamera(MString cameraNode)
{
   if (cameraNode != "")
   {
      m_renderOptions.SetCameraName(cameraNode);
      m_renderOptions.UpdateImageFilename();
      AtNode* camera = AiNodeLookUpByName(cameraNode.asChar());

      if (!camera)
      {
         MSelectionList list;
         MObject        node;
         MFnDagNode     dagNode;
         
         list.add(cameraNode);
         list.getDependNode(0, node);
         
         for (AtUInt J = 0; (J < MFnDagNode(node).childCount()); ++J)
         if (MFnDagNode(MFnDagNode(node).child(J)).typeName() == "camera")
         {
            cameraNode = MFnDagNode(MFnDagNode(node).child(0)).partialPathName();
            break;
         }

         camera = AiNodeLookUpByName(cameraNode.asChar());
      }

      if (!camera)
      {
         AiMsgError("Cannot find camera node %s.", cameraNode.asChar());
         return;
      }

      AiNodeSetPtr(AiUniverseGetOptions(), "camera", camera);
      
      // check visibility for all image planes.
      MDagPath dagPath;
      MItDag   dagIterCameras(MItDag::kDepthFirst, MFn::kCamera);

      // for all cameras
      for (dagIterCameras.reset(); (!dagIterCameras.isDone()); dagIterCameras.next())
      {
         if (!dagIterCameras.getPath(dagPath))
         {
            AiMsgError("[mtoa] ERROR: Could not get path for DAG iterator.");
            return;
         }
         bool isRenderingCamera = false;
         MFnDagNode fnDagNode(dagPath);
         if (fnDagNode.name() == cameraNode)
         {
            isRenderingCamera = true;
         }

         // check all of it's imageplanes
         MPlugArray connectedPlugs;
         MPlug      imagePlanePlug;
         MPlug      imagePlaneNodePlug;
         imagePlanePlug = fnDagNode.findPlug("imagePlane");
         if (imagePlanePlug.numConnectedElements() > 0)
         {
            for(AtUInt ips = 0; (ips < imagePlanePlug.numElements()); ips++)
            {
               MStatus status;
               imagePlaneNodePlug = imagePlanePlug.elementByPhysicalIndex(ips);
               imagePlaneNodePlug.connectedTo(connectedPlugs, true, false, &status);
               MObject resNode = connectedPlugs[0].node(&status);
               if (status)
               {
                  // get the dependency node of the image plane 
                  MFnDependencyNode fnRes(resNode);
                  MString imagePlaneName(fnDagNode.partialPathName());
                  imagePlaneName += "_IP_"; 
                  imagePlaneName += ips; 
                  bool displayOnlyIfCurrent = fnRes.findPlug("displayOnlyIfCurrent", &status).asBool();
                  AtNode* imagePlane = AiNodeLookUpByName(imagePlaneName.asChar());
                  AtInt visibility = 0;

                  if ( (displayOnlyIfCurrent && isRenderingCamera) || (!displayOnlyIfCurrent) )
                  {
                     visibility = AI_RAY_CAMERA;  
                  }
                  if ( (displayOnlyIfCurrent && !isRenderingCamera) )
                  {
                     visibility = 0;  
                  }
                  AiNodeSetInt(imagePlane, "visibility", visibility);
               }
            }
         }
      }
   }
}

void CRenderSession::SetMultiCameraRender(bool multi)
{
   m_renderOptions.SetMultiCameraRender(multi);
}

void CRenderSession::DoRender()
{
   SetupRenderOutput();
   m_renderOptions.SetupRenderOptions();

   InitializeDisplayUpdateQueue();

   // set progressive start point on AA
   AtInt init_progressive_samples = m_renderOptions.isProgressive() ? -3 : m_renderOptions.NumAASamples() ;
   AtUInt prog_passes = m_renderOptions.isProgressive() ? ((-init_progressive_samples) + 2) : 1;

   MComputation comp;
   comp.beginComputation();
   bool aborted = false;

   for (AtUInt i = 0; (i < prog_passes); i++)
   {
      AtInt sampling = i + init_progressive_samples;
      if (i + 1 == prog_passes)
      {
        sampling = m_renderOptions.NumAASamples();
      }

      AiNodeSetInt(AiUniverseGetOptions(), "AA_samples", sampling);
      AtVoid* handler = AiThreadCreate(RenderThread, NULL, AI_PRIORITY_LOW);

      // Process messages sent by the render thread, and exit when rendering is finished or aborted
      ProcessDisplayUpdateQueue(comp);

      if (!aborted && comp.isInterruptRequested())
      {
         AiRenderAbort();
         aborted = true;
         AiThreadWait(handler);
         AiThreadClose(handler);
         break;
      }

      // Wait for the render thread to release everything and close it
      AiThreadWait(handler);
      AiThreadClose(handler);
   }

   comp.endComputation();
}

void CRenderSession::DoBatchRender()
{
   SetupRenderOutput();
   m_renderOptions.SetupRenderOptions();

   AiRender(AI_RENDER_MODE_CAMERA);
}

void CRenderSession::DoExport(MString customFileName, ExportMode exportMode)
{
   MString fileName;

   // if no custom fileName is given, use the default one in the environment variable
   if (customFileName.length() > 0)
      fileName = m_renderOptions.VerifyFileName(customFileName.asChar(), m_renderOptions.outputAssCompressed());
   else
      fileName = m_renderOptions.VerifyFileName(m_renderOptions.outputAssFile().expandEnvironmentVariablesAndTilde(), m_renderOptions.outputAssCompressed());


   if (fileName.length() == 0)
   {
      AiMsgError("[mtoa] File name must be set before exporting .ass file");
   }
   else
   {
      AiMsgInfo("[mtoa] Exporting Maya scene to file '%s'", fileName.asChar());

      if (exportMode==MTOA_EXPORT_ALL)
      {
         SetupRenderOutput();
         m_renderOptions.SetupRenderOptions();
      }   
      AiASSWrite(fileName.asChar(), m_renderOptions.outputAssMask(), false);
   }
}

void CRenderSession::SetupRenderOutput()
{
   AtNode* driver;
   AtNode* renderViewDriver;

   if (!m_renderOptions.BatchMode())
   {
      // render in the renderview
      AiNodeInstall(AI_NODE_DRIVER, AI_TYPE_NONE, "renderview_display",  NULL, (AtNodeMethods*) mtoa_driver_mtd, AI_VERSION);
      renderViewDriver = AiNode("renderview_display");
      AiNodeSetStr(renderViewDriver, "name", "renderview_display");
      AiNodeSetFlt(renderViewDriver, "gamma", m_renderOptions.outputGamma());
   }

   // set the output driver
   driver = AiNode(m_renderOptions.RenderDriver().asChar());
   AiNodeSetStr(driver, "filename", m_renderOptions.GetImageFilename().asChar());

   // Set the driver name depending on the camera name to avoid nodes with the same name on renders with multiple cameras
   MString driverCamName = m_renderOptions.RenderDriver() + "_" + m_renderOptions.GetCameraName();
   AiNodeSetStr(driver, "name", driverCamName.asChar());

   // set output driver parameters
   // Only set output parameters if they exist within that specific node
   if (AiNodeEntryLookUpParameter(driver->base_node, "compression"))
   {
      AiNodeSetInt(driver, "compression", m_renderOptions.arnoldRenderImageCompression());
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "half_precision"))
   {
      AiNodeSetBool(driver, "half_precision", m_renderOptions.arnoldRenderImageHalfPrecision());
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "output_padded"))
   {
      AiNodeSetBool(driver, "output_padded", m_renderOptions.arnoldRenderImageOutputPadded());
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "gamma"))
   {
      AiNodeSetFlt(driver, "gamma", m_renderOptions.arnoldRenderImageGamma());
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "quality"))
   {
      AiNodeSetInt(driver, "quality", m_renderOptions.arnoldRenderImageQuality());
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "format"))
   {
      AiNodeSetInt(driver, "format", m_renderOptions.arnoldRenderImageOutputFormat());
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "tiled"))
   {
      AiNodeSetBool(driver, "tiled", m_renderOptions.arnoldRenderImageTiled());
   }
   if (AiNodeEntryLookUpParameter(driver->base_node, "unpremult_alpha"))
   {
      AiNodeSetBool(driver, "unpremult_alpha", m_renderOptions.arnoldRenderImageUnpremultAlpha());
   }

   // OUTPUT FILTER (use for all image outputs)
   //
   AtNode* filter = AiNode(m_renderOptions.filterType().asChar());

   // Only set filter parameters if they exist within that specific node
   if (AiNodeEntryLookUpParameter(filter->base_node, "width"))
   {
      AiNodeSetFlt(filter, "width", m_renderOptions.filterWidth());
   }
   if (AiNodeEntryLookUpParameter(filter->base_node, "domain"))
   {
      AiNodeSetStr(filter, "domain", m_renderOptions.filterDomain().asChar());
   }
   if (AiNodeEntryLookUpParameter(filter->base_node, "scalar_mode"))
   {
      AiNodeSetBool(filter, "scalar_mode", m_renderOptions.filterScalarMode());
   }
   if (AiNodeEntryLookUpParameter(filter->base_node, "maximum"))
   {
      AiNodeSetFlt(filter, "maximum", m_renderOptions.filterMaximum());
   }
   if (AiNodeEntryLookUpParameter(filter->base_node, "minimum"))
   {
      AiNodeSetFlt(filter, "minimum", m_renderOptions.filterMinimum());
   }

   // OUTPUT STRINGS
   //
   AtChar   str[1024];
   int      ndrivers = m_renderOptions.BatchMode() ? 1 : 2;
   AtArray* outputs  = AiArrayAllocate(ndrivers+static_cast<AtInt>(m_renderOptions.NumAOVs()), 1, AI_TYPE_STRING);

   sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(filter), AiNodeGetName(driver));
   AiArraySetStr(outputs, 0, str);

   if (!m_renderOptions.BatchMode())
   {
      sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(filter), "renderview_display");
      AiArraySetStr(outputs, 1, str);
   }

   for (size_t i=0; i<m_renderOptions.NumAOVs(); ++i)
   {
      m_renderOptions.GetAOV(i).SetupOutput(outputs, ndrivers+static_cast<int>(i), driver, filter);
   }

   AiNodeSetArray(AiUniverseGetOptions(), "outputs", outputs);
}
