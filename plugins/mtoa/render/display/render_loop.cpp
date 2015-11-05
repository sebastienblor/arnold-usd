
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
#include <sstream>

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



extern int RenderLoop(CRenderView *kwin)
{
   int i, exit_code = K_SUCCESS;
   K_wait_for_changes = false;
   
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

      int smin = MIN(-3, K_AA_samples);
      int smax = K_AA_samples;

      K_render_timestamp = CRenderView::time();
      
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


         std::stringstream statusStr;
         statusStr <<"Rendering...  ";

         statusStr<<"Resolution: "<<AiNodeGetInt(AiUniverseGetOptions(), "xres")<<"x"<<AiNodeGetInt(AiUniverseGetOptions(), "yres");
   
         statusStr<<"  Sampling: "<<i;
         statusStr<<"/"<<AiNodeGetInt(AiUniverseGetOptions(), "GI_diffuse_samples");
         statusStr<<"/"<<AiNodeGetInt(AiUniverseGetOptions(), "GI_glossy_samples");
         statusStr<<"/"<<AiNodeGetInt(AiUniverseGetOptions(), "GI_refraction_samples");
         statusStr<<"/"<<AiNodeGetInt(AiUniverseGetOptions(), "sss_bssrdf_samples");
         statusStr<<"/"<<AiNodeGetInt(AiUniverseGetOptions(), "volume_indirect_samples");
        
         kwin->setStatus(statusStr.str());
        
         AtUInt64 loop_time = (i == smax) ? CRenderView::time() : 0;

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
                  kwin->restoreContinuous();
                  


                  AtUInt64 end_time = CRenderView::time();
                  int seconds_count = (end_time - loop_time) / 1000000;
                  int minutes_count = seconds_count / 60;
                  seconds_count -= minutes_count * 60;
                  int hours_count = minutes_count / 60;
                  minutes_count -= hours_count * 60;

                  std::stringstream statusEndStr;

                  if (hours_count <= 0) statusEndStr <<"00:";
                  else if (hours_count < 10) statusEndStr<<"0"<<hours_count<<":";
                  else statusEndStr <<hours_count<<":";

                  if (minutes_count <= 0) statusEndStr <<"00:";
                  else if (minutes_count < 10) statusEndStr<<"0"<<minutes_count<<":";
                  else statusEndStr <<minutes_count<<":";
                  
                  if (seconds_count <= 0) statusEndStr <<"00";
                  else if (seconds_count < 10) statusEndStr<<"0"<<seconds_count;
                  else statusEndStr <<seconds_count;
                  

                  statusEndStr<<"  Resolution: "<<AiNodeGetInt(AiUniverseGetOptions(), "xres")<<"x"<<AiNodeGetInt(AiUniverseGetOptions(), "yres");
   

                  statusEndStr <<"  Sampling: "<<i;
                  statusEndStr<<"/"<<AiNodeGetInt(AiUniverseGetOptions(), "GI_diffuse_samples");
                  statusEndStr<<"/"<<AiNodeGetInt(AiUniverseGetOptions(), "GI_glossy_samples");
                  statusEndStr<<"/"<<AiNodeGetInt(AiUniverseGetOptions(), "GI_refraction_samples");
                  statusEndStr<<"/"<<AiNodeGetInt(AiUniverseGetOptions(), "sss_bssrdf_samples");
                  statusEndStr<<"/"<<AiNodeGetInt(AiUniverseGetOptions(), "volume_indirect_samples");
                 
                  const float mem_used = (float)AiMsgUtilGetUsedMemory() / 1024.0f / 1024.0f;
                  statusEndStr<<"  Memory: "<<mem_used<<"MB";

                  kwin->setStatus(statusEndStr.str());
                  kwin->draw();

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
               kwin->restoreContinuous();
               break;
            }
         }
      }

   } while (!K_aborted);


   kwin->restoreContinuous();
   return exit_code;
}


unsigned int kickWindowRender(void *kwin_ptr)
{
  CRenderView *kwin = (CRenderView*)kwin_ptr;
  kwin->show();

  RenderLoop(kwin);

  return 0;
}


