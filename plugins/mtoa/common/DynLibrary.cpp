
#include "DynLibrary.h"

#include "platform/Platform.h"

#include <ai_allocate.h>

#if defined(_WIN32)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#elif defined(_LINUX) || defined(_DARWIN)
#include <dlfcn.h>
#endif

#if defined(_WIN32)

void *LibraryLoad(const char *filename)
{
   // TODO: make it work or make an API! :)
   // Load the library with the flag DONT_RESOLVE_DLL_REFERENCES but it seems this is not the way to do
   // The library loads but we cannot do anything with it, when we try to call the initialize function, it crashes
   // LoadLibrary(filename) fails and says: "The specified module could not be found."
   // We have to make LoadLibrary() work, possible ?

   void* result;
   int len = strlen(filename);
   // Remove trailing .so if found (LoadLibrary will automatically add ".dll" to that name)
   if (filename && (len > 3) && !strcmp(filename + len - 3, ".so"))
   {
      char *name = (char*) AiMalloc(len);
      strncpy(name, filename, len - 3);
      name[len - 3] = '\0';
      result = LoadLibraryEx(filename, NULL, DONT_RESOLVE_DLL_REFERENCES);
      AiFree(name);
   }
   else
   {
      result = LoadLibraryEx(filename, NULL, DONT_RESOLVE_DLL_REFERENCES);
   }

   return (void *) result;
}

void LibraryUnload(void *handle)
{
   FreeLibrary((HMODULE) handle);
}

void *LibrarySymbol(void *handle, const char *name)
{
   return GetProcAddress((HMODULE) handle, name);
}

char *LibraryLastError()
{
   LPTSTR msg;
   DWORD code = GetLastError();

   if (FormatMessage(
         FORMAT_MESSAGE_ALLOCATE_BUFFER |
         FORMAT_MESSAGE_FROM_SYSTEM |
         FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL,
         code,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
         (LPTSTR)&msg,
         0, NULL ))
   {
      return (char *)msg;
   }
   else
   {
      return NULL;
   }
}

#elif defined(_LINUX) || defined(_DARWIN)

void *LibraryLoad(const char *filename)
{
   return dlopen(filename, RTLD_LAZY | RTLD_LOCAL);
}

void LibraryUnload(void *handle)
{
   dlclose(handle);
}

void *LibrarySymbol(void *handle, const char *name)
{
   return dlsym(handle, name);
}

char *LibraryLastError()
{
   return dlerror();
}

#else
#error "Building on unknown platform"
#endif
