
#include "RenderSession.h"
#include "RenderOptions.h"
#include "OutputDriver.h"
#include "maya_scene/MayaScene.h"
#include "maya_scene/NodeTranslator.h"

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_plugins.h>
#include <ai_render.h>
#include <ai_threads.h>
#include <ai_universe.h>
#include <ai_ray.h>


#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDagNode.h>
#include <maya/MComputation.h>
#include <maya/MEventMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MMessage.h> // for MCallbackId
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MRenderUtil.h>
#include <maya/MStatus.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

#include <maya/M3dView.h>
#include <maya/MRenderView.h>

#include <time.h>
#include <cstdio>

extern AtNodeMethods* mtoa_driver_mtd;

static CRenderSession* s_renderSession = NULL;

// This will update the render view if needed.
// It's called from a maya idle event callback.
// This means it's called a *lot*.
void CRenderSession::updateRenderViewCallback(void *)
{
   ProcessSomeOfDisplayUpdateQueue();
}

// This is the code for the render thread. This version is used for IPR
// to run the AiRender() process outside of the main thread.
// This is *static*.
unsigned int CRenderSession::RenderThread(AtVoid* data)
{
   CRenderOptions * render_options = static_cast< CRenderOptions * >( data );
   // set progressive start point on AA
   AtInt init_progressive_samples = render_options->isProgressive() ? -3 : render_options->NumAASamples();
   AtUInt prog_passes = render_options->isProgressive() ? ((-init_progressive_samples) + 2) : 1;

   // Get rid of any previous renders tiles that have not yet
   // been displayed.
   InitializeDisplayUpdateQueue();

   //const time_t start_time = time(0x0);

   AtULong ai_status(AI_SUCCESS);
   for (AtUInt i = 0; (i < prog_passes ); ++i)
   {
      AtInt sampling = i + init_progressive_samples;
      if (i + 1 == prog_passes)
      {
         sampling = render_options->NumAASamples();
      }

      AiNodeSetInt(AiUniverseGetOptions(), "AA_samples", sampling);
      // Begin a render!
      ai_status = AiRender(AI_RENDER_MODE_CAMERA);
      if ( ai_status != AI_SUCCESS ) break;
   }

   DisplayUpdateQueueRenderFinished();

//   // Update the render view with the render time of the render.
//   const time_t elapsed = time(0x0) - start_time;
//   char command_str[256];
//   sprintf( command_str,
//            "arnoldIpr -mode finishedIPR -elapsedTime \"%ld:%02ld\" ;",
//            elapsed / 60,
//            elapsed % 60 );
//   MGlobal::executeCommandOnIdle( command_str, false );

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

void CRenderSession::LoadPlugins()
{

   #ifdef _WIN32
      const char split_char(';');
   #else
      const char split_char(':');
   #endif

   const MString resolvedPathList = m_renderOptions.pluginsPath().expandEnvironmentVariablesAndTilde();

   MStringArray pluginPaths;
   resolvedPathList.split( split_char, pluginPaths);
   for (unsigned int i=0; i<pluginPaths.length(); ++i)
   {
      const MString pluginPath = pluginPaths[i];
      if (pluginPath.length() > 0)
      {
         AiLoadPlugins(pluginPath.asChar());
      }
   }
}

void CRenderSession::Init()
{
   m_scene = new CMayaScene;
   m_renderOptions.GetFromMaya(m_scene);
   m_renderOptions.SetupLog();
}

void CRenderSession::Translate(ExportMode exportMode)
{
   if (AiUniverseIsActive())
   {
      AiMsgError("[mtoa] ERROR: There can only be one RenderSession active.");
      return;
   }

   // Begin the Arnold universe.
   AiBegin();
   Init();
   LoadPlugins();
   
   m_scene->ExportToArnold(exportMode);
}


void CRenderSession::Finish()
{
   if (IsActive())
   {
      AiRenderAbort();
      InterruptRender();
      AiEnd();
   }

   // This will release the scene and therefore any
   // translators it has held.
   if ( m_scene != 0x0 )
   {
      delete m_scene;
      m_scene = 0x0;
   }
}

void CRenderSession::InterruptRender()
{
   if ( AiRendering() ) AiRenderInterrupt();

   // Stop the Idle update.
   ClearIdleRenderViewCallback();

   // Wait for the thread to clear.
   if (m_render_thread != 0x0 )
   {
      AiThreadWait(m_render_thread);
      AiThreadClose(m_render_thread);
   }
}

void CRenderSession::SetBatch(bool batch)
{
   m_renderOptions.SetBatch(batch);
}

void CRenderSession::SetResolution(const int width, const int height)
{
   if (width != -1) m_renderOptions.SetWidth(width);
   if (height != -1) m_renderOptions.SetHeight(height);
}

void CRenderSession::SetRegion( const AtUInt left, const AtUInt right,
                                const AtUInt bottom, const AtUInt top )
{
   m_renderOptions.SetRegion( left, right, bottom, top );
}

void CRenderSession::SetProgressive( const bool is_progressive )
{
   m_renderOptions.SetProgressive( is_progressive );
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

void CRenderSession::SetupRenderOutput()
{

   AtNode * render_view = CreateRenderViewOutput();
   AtNode * file_driver = CreateFileOutput();
   AtNode * filter = CreateOutputFilter();

   // OUTPUT STRINGS
   AtChar   str[1024];
   int ndrivers = 0;
   if ( render_view != 0x0 ) ++ndrivers;
   if ( file_driver != 0x0 ) ++ndrivers;

   AtArray* outputs  = AiArrayAllocate(ndrivers+m_renderOptions.NumAOVs(), 1, AI_TYPE_STRING);

   int driver_num(0);
   if (render_view != 0x0)
   {
      sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(filter), AiNodeGetName(render_view) );
      AiArraySetStr(outputs, driver_num++, str);
   }

   if ( file_driver != 0x0 )
   {
      sprintf(str, "RGBA RGBA %s %s", AiNodeGetName(filter), AiNodeGetName(file_driver));
      AiArraySetStr(outputs, driver_num++, str);

      for (size_t i=0; i<m_renderOptions.NumAOVs(); ++i)
      {
         m_renderOptions.GetAOV(i).SetupOutput(outputs, ndrivers+static_cast<int>(i), file_driver, filter);
      }
   }

   AiNodeSetArray(AiUniverseGetOptions(), "outputs", outputs);

}

AtNode * CRenderSession::CreateFileOutput()
{
   // Don't install the file driver when in IPR mode.
   if ( GetMayaScene()->GetExportMode() == MTOA_EXPORT_IPR ) return 0x0;

   AtNode* driver;
   // set the output driver
   MString driverCamName = m_renderOptions.RenderDriver() + "_" + m_renderOptions.GetCameraName();
   driver = AiNodeLookUpByName( driverCamName.asChar() );
   if ( driver == 0x0 )
   {
      driver = AiNode(m_renderOptions.RenderDriver().asChar());
      AiNodeSetStr(driver, "filename", m_renderOptions.GetImageFilename().asChar());

      // Set the driver name depending on the camera name to avoid nodes with
      // the same name on renders with multiple cameras
      AiNodeSetStr(driver, "name", driverCamName.asChar());
   }

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

   return driver;
}

AtNode * CRenderSession::CreateRenderViewOutput()
{
   // Don't create it if we're in batch mode.
   if (m_renderOptions.BatchMode()) return 0x0;

   AtNode * driver = AiNodeLookUpByName( "renderview_display" );
   if ( driver == 0x0 )
   {
      AiNodeInstall(AI_NODE_DRIVER,
                    AI_TYPE_NONE,
                    "renderview_display",
                    NULL,
                    (AtNodeMethods*) mtoa_driver_mtd,
                    AI_VERSION);
      driver = AiNode("renderview_display");
      AiNodeSetStr(driver, "name", "renderview_display");
   }

   AiNodeSetFlt(driver, "gamma", m_renderOptions.outputGamma());

   return driver;
}

AtNode * CRenderSession::CreateOutputFilter()
{
   // OUTPUT FILTER (use for all image outputs)
   AtNode* filter = AiNodeLookUpByName( m_renderOptions.filterType().asChar() );
   if (filter == 0x0) filter = AiNode(m_renderOptions.filterType().asChar());

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

   return filter;
}

void CRenderSession::DoInteractiveRender()
{
   MComputation comp;
   comp.beginComputation();

   SetupRenderOutput();
   m_renderOptions.SetupRenderOptions();
   PrepareRenderView();

   // Start the render thread.
   m_render_thread = AiThreadCreate(CRenderSession::RenderThread,
                                    &m_renderOptions,
                                    AI_PRIORITY_LOW);

   // This returns when the render is done or if someone
   // has hit escape.
   ProcessDisplayUpdateQueueWithInterupt( comp );

   // Stop and clean up after the render.
   InterruptRender();

   comp.endComputation();
}


void CRenderSession::DoBatchRender()
{
   SetupRenderOutput();
   m_renderOptions.SetupRenderOptions();

   AiRender(AI_RENDER_MODE_CAMERA);
}

void CRenderSession::DoExport(MString customFileName)
{
   MString fileName;

   // if no custom fileName is given, use the default one in the environment variable
   if (customFileName.length() > 0)
   {
      fileName = m_renderOptions.VerifyFileName(customFileName.asChar(),
                                                m_renderOptions.outputAssCompressed());
   }
   else
   {
      fileName = m_renderOptions.VerifyFileName(m_renderOptions.outputAssFile().expandEnvironmentVariablesAndTilde(),
                                                m_renderOptions.outputAssCompressed());
   }

   if (fileName.length() == 0)
   {
      AiMsgError("[mtoa] File name must be set before exporting .ass file");
   }
   else
   {
      AiMsgInfo("[mtoa] Exporting Maya scene to file '%s'", fileName.asChar());

      if (GetMayaScene()->GetExportMode()==MTOA_EXPORT_ALL)
      {
         SetupRenderOutput();
         m_renderOptions.SetupRenderOptions();
      }   
      AiASSWrite(fileName.asChar(), m_renderOptions.outputAssMask(), false);
   }
}

MStatus CRenderSession::PrepareRenderView( bool addIdleRenderViewUpdate )
{
   MStatus status( MS::kSuccess );
   
   // We need to set the current camera in renderView,
   // so the buttons render from the camera you want.
   MSelectionList list;
   MDagPath       cameraDagPath;
   list.add( m_renderOptions.GetCameraName() );
   list.getDagPath(0, cameraDagPath);
   MRenderView::setCurrentCamera(cameraDagPath);

   if (m_renderOptions.useRenderRegion())
   {
      status = MRenderView::startRegionRender(  m_renderOptions.width(),
                                                m_renderOptions.height(),
                                                m_renderOptions.minX(),
                                                m_renderOptions.maxX(),
                                                m_renderOptions.minY(),
                                                m_renderOptions.maxY(),
                                                !m_renderOptions.clearBeforeRender(),
                                                true);
   }
   else
   {
      status = MRenderView::startRender(  m_renderOptions.width(),
                                          m_renderOptions.height(),
                                          !m_renderOptions.clearBeforeRender(),
                                          true);
   }

   if (MStatus::kSuccess != status)
   {
      MGlobal::displayError("Render view is not able to render.");
      return status;
   }

   ClearIdleRenderViewCallback();
   if (addIdleRenderViewUpdate) AddIdleRenderViewCallback();

   return status;
}

void CRenderSession::DoIPRRender()
{
   if (!m_paused_ipr)
   {
      SetProgressive(true);
      SetBatch(false);
      SetupRenderOutput();
      PrepareRenderView(true); // Install callbacks.
      m_renderOptions.SetupRenderOptions();
      
      // Start the render thread.
      m_render_thread = AiThreadCreate(CRenderSession::RenderThread,
                                       &m_renderOptions,
                                       AI_PRIORITY_LOW);
   }
}

void CRenderSession::FinishedIPRTuning()
{
   // We not actually interrupting,
   // but this will clean up.
   InterruptRender();
}

void CRenderSession::PauseIPR()
{
   InterruptRender();
   ClearIdleRenderViewCallback();
   m_paused_ipr = true;
}

void CRenderSession::UnPauseIPR()
{
   m_paused_ipr = false;
   DoIPRRender();
}

AtUInt64 CRenderSession::GetUsedMemory()
{
   return AiMsgUtilGetUsedMemory() / 1024 / 1024;
}


void CRenderSession::AddIdleRenderViewCallback()
{
   if ( 0 == m_idle_cb )
   {
      MStatus status;
      m_idle_cb = MEventMessage::addEventCallback( "idle",
                                                   CRenderSession::updateRenderViewCallback,
                                                   0x0,
                                                   &status );
   }
}

void CRenderSession::ClearIdleRenderViewCallback()
{
   // Get the rest of the rendered image processed.
   ProcessDisplayUpdateQueue();

   if ( m_idle_cb > 0 )
   {
      MMessage::removeCallback( m_idle_cb );
      MRenderView::endRender();
      m_idle_cb = 0;
   }
}
