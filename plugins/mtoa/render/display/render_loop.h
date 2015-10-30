/*
 * Common header file for macros, data types, prototypes and global vars
 * 
 * --------------------------------------------------------------------------
 * --------------------------------------------------------------------------
 */

#pragma once


#include <ai.h>

#include <math.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <conio.h>
#include <io.h>
#else
#include <unistd.h>
#endif

/******************************************************************************
   Various macros
******************************************************************************/

#define K_MAX_FILENAME     1024
#define K_MAX_FILES        32
#define K_MAX_PATHS        32
#define K_MAX_IGNORES      16
#define K_SUCCESS          0

#ifdef _WIN32
#define K_ERROR            256
#else
#define K_ERROR            (!K_SUCCESS)
#endif

#define K_PLUGIN_PATH      "ARNOLD_PLUGIN_PATH"
#define K_STDIN_STRING     "-"

#ifdef _WIN32
#define K_PATH_SEPARATOR   ';'
#else
#define K_PATH_SEPARATOR   ':'
#endif

#define K_FILTER_NAME    "kick_filter"
#define K_DRIVER_NAME    "kick_driver"
#define K_DEFAULT_FILTER "gaussian_filter"
#define K_MAX_DRIVERS    32

#define K_MOVEMENT_NONE  0
#define K_MOVEMENT_MAYA  1
#define K_MOVEMENT_QUAKE 2
#define K_MOVEMENT_EXTERNAL 3


#define K_DISPLAY_NAME "driver_kick"

#ifdef _WIN32
#define isatty(h) _isatty(h)
#define fileno(s) _fileno(s)
#endif

enum EInfoMode
{
   K_INFO_NONE,
   K_INFO_NODES,
   K_INFO_NODE,
   K_INFO_LIST_OUTPUT_DRIVERS,
   K_INFO_SHADE_TREE,
   K_INFO_LICENSE
};

/******************************************************************************
   Global variables  
******************************************************************************/

// This was taken from kick driver code
// here we can surely have a better way to store these global variables
// and most of them are probably useless now,
// but I'd rather doing all the "code-cleaning" changes progressively

extern int    K_argc;
extern char   **K_argv;
extern int    K_isatty;
extern int    K_ignore_stdin;
extern char   K_input_filename[K_MAX_FILES][K_MAX_FILENAME];
extern int    K_input_files;
extern char   K_lib_path[K_MAX_PATHS][K_MAX_FILENAME];
extern int    K_lib_paths;
extern int    K_fformat;
extern char   K_output_filename[K_MAX_FILENAME];
extern char   K_driver_filename[K_MAX_DRIVERS][K_MAX_FILENAME];
extern const struct AtNodeEntry *K_driver_type;
extern int    K_flat_all;
extern char   K_camera_filename[K_MAX_FILENAME];
extern int    K_render_window;
extern std::string K_title;
extern std::string K_command_line;
extern int    K_verbose;
extern int    K_nwarnings;
extern int    K_write_log;
extern int    K_log_flags;
extern char   K_log_filename[K_MAX_FILENAME];
extern int    K_progressive;
extern int    K_wait_for_changes;
extern int 	  K_allow_interruption;
extern int 	  K_enable_aovs;

/*
 * we appear to need 'volatile' for the empty while() loop after we've
 * finished the last AA pass, otherwise 'kick' doesn't realize that these
 * values have possibly changeed (by another thread)
 */
extern volatile int K_aborted;
extern volatile int K_restartLoop;
extern volatile AtUInt64  K_render_timestamp;

extern int    K_repeat;
extern int    K_resave;
extern int    K_open_procedurals;
extern char   K_resave_filename[K_MAX_FILENAME];
extern bool   K_shutter;
extern float  K_shutter_start;
extern float  K_shutter_end;
extern float  K_fov;
extern float  K_camera_exposure;
extern char   K_ignore_list[K_MAX_IGNORES][256];
extern int    K_ignore_list_size;
extern int    K_turn;
extern const char* K_filename;
extern int    K_set_filename;
extern char   K_filter_type[64];
extern float  K_filter_width;
extern EInfoMode K_info_mode;
extern int    K_info_sort;
extern const char*  K_info_data;
extern int    K_wait_keypress;
extern int    K_binary_ass;

extern char   K_RLM_server_name[HOST_NAME_MAX];
extern unsigned int K_RLM_server_port;


extern int      K_AA_samples;



/******************************************************************************
   Public prototypes
******************************************************************************/

void K_InitGlobalVars();

char *mystrdup(const char *str);
void initRenderWindow(void *kwin_ptr);
void closeRenderWindow(void *kwin_ptr);

unsigned int kickWindowRender(void *kwin_ptr);
void initializeKickWindow(void *kwin_ptr);

