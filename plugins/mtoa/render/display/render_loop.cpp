
#include <stdio.h>
#include <iostream>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

extern void K_ProcessRenderError(int error);


#include "render_loop.h"
#include "renderview.h"

#include "render_gl_widget.h"

#include "scene/MayaScene.h"
#include "session/ArnoldSession.h"

#include <iostream>

/******************************************************************************
   Global variables
******************************************************************************/

int           K_argc;
char          **K_argv;
int           K_isatty;
int           K_ignore_stdin;
char          K_input_filename[K_MAX_FILES][K_MAX_FILENAME];
int           K_input_files;
char          K_lib_path[K_MAX_PATHS][K_MAX_FILENAME];
int           K_lib_paths;
char          K_output_filename[K_MAX_FILENAME];
char          K_driver_filename[K_MAX_DRIVERS][K_MAX_FILENAME];
const AtNodeEntry *K_driver_type;
AtNode       *K_driver_node;
char          K_filter_type[64];
float         K_filter_width;
int           K_flat_all;
char          K_camera_filename[K_MAX_FILENAME];
int           K_render_window;
std::string   K_title;
std::string   K_command_line;
int           K_verbose;
int           K_nwarnings;
int           K_write_log;
char          K_log_filename[K_MAX_FILENAME];
int           K_log_flags;
int           K_progressive;
volatile int  K_aborted;
int           K_exit_code;
volatile int  K_restartLoop;
int           K_repeat;
int           K_repeat_counter;
int           K_resave;
int           K_open_procedurals;
char          K_resave_filename[K_MAX_FILENAME];
int           K_AA_samples;
bool          K_shutter;
float         K_shutter_start;
float         K_shutter_end;
float         K_fov;
float         K_camera_exposure;
char          K_ignore_list[K_MAX_IGNORES][256];
int           K_ignore_list_size;
int           K_turn;
int           K_set_filename;
EInfoMode     K_info_mode;
int           K_info_sort;
const char   *K_info_data;
int           K_wait_keypress;
int           K_binary_ass;
int           K_wait_for_changes;
int           K_allow_interruption;
int           K_enable_aovs;
volatile AtUInt64 K_render_timestamp;
bool          K_refresh_requested;



// forward for render loop
int RenderLoop(int smin, int smax);



/******************************************************************************
   Helper functions
******************************************************************************/

/*
 * strdup replacement because free'ing a strdup() string crashes in
 * VS2005 - thanks microsoft
 */
char *mystrdup(const char *str)
{
   // this crashes: return strdup(str);

   if (str == NULL)
      return NULL;
   unsigned int len = (int) strlen(str) + 1;
   char *ptr = (char *) AiMalloc(len);
   if (ptr == NULL)
      return NULL;
   memcpy(ptr, str, len);
   return ptr;
}

/*
 * Creates the necessary AOVs to begin rendering
 */
AtArray *K_CreateAOVs(const char *filter_type, float filter_width, const char *filter_name, const char *driver_name)
{
   if (!driver_name)
      return AiArray(1, 1, AI_TYPE_STRING, "");

   AtNode *filter;

   if (filter_type[0] != '\0')
      filter = AiNode(filter_type);
   else
      filter = AiNode(K_DEFAULT_FILTER);

   if (filter == NULL)
   {
      printf("Unrecognized filter: %s  (defaulting to %s)\n", filter_type, K_DEFAULT_FILTER);
      filter = AiNode(K_DEFAULT_FILTER);
   }

   if (filter_width != 0.0f)      
   {
      if (AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(filter), "width"))
         AiNodeSetFlt(filter, "width", filter_width);
   }
   AiNodeSetStr(filter, "name", filter_name);

   char aov_decl[1024];
   sprintf(aov_decl, "RGBA RGBA  %s %s", filter_name, driver_name);

   return AiArray(1, 1, AI_TYPE_STRING, aov_decl);
}


/*
 * Initialize counters and other global variables
 */
void K_InitGlobalVars(void)
{
   K_ignore_stdin = false;
   K_input_files = 0;
   K_lib_paths = 0;
   K_output_filename[0] = 0;
   K_render_window = true;
   K_verbose = 1;
   K_nwarnings = 5;
   K_write_log = false;
   K_log_flags = -1;
   K_log_filename[0] = 0;
   K_progressive = true;
   K_aborted = false;
   K_exit_code = K_SUCCESS;
   K_repeat = 0;
   K_resave = false;
   K_open_procedurals = false;
   K_driver_type = NULL;
   K_filter_type[0] = 0;
   K_filter_width   = 0;
   K_shutter = false;
   K_shutter_start = -1;
   K_shutter_end = -1;
   K_fov = -1;
   K_camera_exposure = 0;
   K_ignore_list_size = 0;
   K_turn = 0;
   K_driver_node = 0;
   K_set_filename = 0;
   
   K_info_mode = K_INFO_NONE;
   K_info_sort = 0;
   K_info_data = NULL;
   K_wait_keypress = true;
   K_binary_ass = true;
   K_wait_for_changes = false;
   K_allow_interruption = true;
   K_render_timestamp = 0;
   K_refresh_requested = false;

   K_set_filename = 0;
   K_resave = 0;
   K_enable_aovs = true;

   for (int i = 0; i < K_MAX_IGNORES; i++)
      strcpy(K_ignore_list[i], "");

   for (int i = 0; i < K_MAX_FILES; i++)
      strcpy(K_input_filename[i], "");

   /*
    * Construct the command line string (K_command_line) and the title
    * string (K_title) for the display window
    */
   K_command_line = "";
   for (int arg_i = 0; arg_i < K_argc; ++arg_i)
   {
      K_command_line += K_argv[arg_i];
      if (arg_i < K_argc - 1)
         K_command_line += " ";
   }

   K_title  = "Arnold ";
   K_title += AiGetVersion(NULL,NULL,NULL,NULL);
   K_title += " | ";
   K_title += K_command_line;
}



extern int RenderLoop(int smin, int smax)
{

   bool k_outputs_set = false;
   int i, exit_code = K_SUCCESS;
   K_wait_for_changes = false;
   CArnoldSession *arnoldSession = CMayaScene::GetArnoldSession();

   bool continuous = arnoldSession->GetContinuousUpdates();
   
   /*
    * Progressive rendering loop
    */
   do 
   {
      /*
       * apply any existing node changes
       */
      
      while(K_wait_for_changes) {CRenderView::sleep(10);}

      /*
       * begin the render-loop
       */

      K_render_timestamp = CRenderView::time();
      bool render_timer = false;
      if (continuous)
      {
         // now that a render is starting, let's set continuous updates to false
         // so that incoming events don't stop this render
         // after 1/15 seconds, this will be set back by render_gl_widget::paintGL (calling CRenderView::checkSceneUpdates)
         arnoldSession->SetContinuousUpdates(false);
      }


      K_refresh_requested = false;
      
      for (i=(K_progressive) ? smin : smax; i<=smax && !K_aborted ; i++)
      {        
         int error;
         if (K_aborted)
            break;

         K_restartLoop = false;


/*
         Commented for now
         This code triggers a new rendering after the first loop finished
         in case some changes have been done in the scene

         As opposed to the timer system used in CRenderView::checkSceneUpdates
         this forces a full rendering of the first step before allowing a new one.
         Not very convincing in the few tests I did

         if (continuous && i != smin)
         {
            
            if (arnoldSession->HasObjectsToUpdate())
            {
               K_wait_for_changes = true;
               arnoldSession->SetContinuousUpdates(true);
               K_restartLoop = true;
               i = smax+1;
               break;
            }            
            arnoldSession->SetContinuousUpdates(true);
         }

         */

         
         if ((i==0) || (i>1 && i<smax) || (i==smax-1))
            continue;
            
         
         AiNodeSetInt(AiUniverseGetOptions(), "AA_samples", i);
   
         error = AiRender(0); //AI_RENDER_THREADED);
         switch (error)
         {

            case AI_INTERRUPT:
            {
               /*
                * restart rendering loop
                */
               
               K_restartLoop = true;
               i = smax+1;
               exit_code = error;
               break;
            }
            case AI_SUCCESS:
            {
               /*
                * if this was the last AA setting, then enter busy-loop
                * waiting for a key-press or something
                */

               if (i == smax)
               {
                  // setting back continuous updates to its original state
                  arnoldSession->SetContinuousUpdates(continuous);
                  while (K_aborted == false && K_restartLoop == false) {
                     CRenderView::sleep(1000); // don't want CPU pegged at 100% with useless work
                  }
                }
               break;
            }
            default:
            {
              
               /*
                * then there was some sort of rendering error or abort
                * signal -- terminate the render loop
                */
               K_aborted = 1;
               exit_code = error;
               break;
            }
         }
      }

   } while (!K_aborted);
   

   return exit_code;
}


unsigned int kickWindowRender(void *kwin_ptr)
{
  CRenderView *kwin = (CRenderView*)kwin_ptr;
  kwin->show();

  int smin = MIN(-3, K_AA_samples);
  int smax = K_AA_samples;

  RenderLoop(smin, smax);

  return 0;
}


