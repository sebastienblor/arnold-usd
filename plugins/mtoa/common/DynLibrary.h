#pragma once

//#include <ai_types.h>

/******************************************************************************
   Macros
******************************************************************************/

#ifdef _WIN32
#define AI_LIB_EXTENSION { "dll", NULL }
#endif

#ifdef _LINUX
#define AI_LIB_EXTENSION { "so", NULL }
#endif

#ifdef _DARWIN
#define AI_LIB_EXTENSION { "so", "dylib", NULL }
#endif

/******************************************************************************
   Dynamic lib functions
******************************************************************************/

/*
 * Load a dynamic library (dll/so) in memory
 *
 * Compiling a shared library, details depend up the platform:
 *  Visual C++:  cl -I%ARNOLDROOT%/include -LD sphere.c
 *  Borland C++: bcc32 -I%ARNOLDROOT%/include -tWD sphere.c
 *  Linux:       g++ -I$ARNOLDROOT/include -shared sphere.c -o sphere.so
 *  BSD/OS:      g++ -I$ARNOLDROOT/include -shared sphere.c -o sphere.so
 */
void *LibraryLoad(const char *filename);

/*
 * Unload a dynamic library (dll/so) from memory
 */
void LibraryUnload(void *handle);

/*
 * Get a symbol pointer (usually a function) from a previously
 * loaded library
 */
void *LibrarySymbol(void *handle, const char *name);

/*
 * Return a string with the last error message occurred during
 * a AiLibraryLoad/Unload/Symbol call.
 */
char *LibraryLastError();
