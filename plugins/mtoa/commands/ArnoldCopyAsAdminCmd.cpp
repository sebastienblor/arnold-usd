#include "ArnoldCopyAsAdminCmd.h"

#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>

#include <ai_universe.h>

#include "scene/MayaScene.h"
#ifdef _DARWIN
#include <security/security.h>
#endif

#ifdef _WIN32
#include "shellapi.h"
#endif


MSyntax CArnoldCopyAsAdminCmd::newSyntax()
{
   MSyntax syntax;

   syntax.addFlag("f", "file", MSyntax::kString);
   syntax.addFlag("o", "output", MSyntax::kString);

   return syntax;
}

MStatus CArnoldCopyAsAdminCmd::doIt(const MArgList& argList)
{
   //MStatus status;
   MArgDatabase args(syntax(), argList);
   
   MString file = args.flagArgumentString("file", 0);
   MString output = args.flagArgumentString("output", 0);

#ifdef _WIN32
   for (int index = file.indexW("/"); index != -1; index = file.indexW("/"))
      file = file.substringW(0, index-1) + "\\" + file.substringW(index + 1, file.numChars());
   for (int index = output.indexW("/"); index != -1; index = output.indexW("/"))
      output = output.substringW(0, index-1) + "\\" + output.substringW(index + 1, output.numChars());
      
   const char *src;
   const char *dst;

   src = file.asChar();
   dst = output.asChar();
   
   char parameters[512];
   sprintf(parameters, "/C copy /Y \"%s\" \"%s\"", src, dst);

   // we need LPCWSTR parameters
   wchar_t wVerb[16];
   std::mbstowcs(wVerb, "runas", strlen("runas")+1);
   wchar_t wFile[256];
   std::mbstowcs(wFile, "cmd", strlen("cmd")+1);
   wchar_t wParameters[512];
   std::mbstowcs(wParameters, parameters, strlen(parameters)+1);

   SHELLEXECUTEINFOW shExInfo = {0};
   shExInfo.cbSize = sizeof(shExInfo);
   shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
   shExInfo.hwnd = 0;
   shExInfo.lpVerb = wVerb;   // Operation to perform
   shExInfo.lpFile = wFile;   // Application to start    
   shExInfo.lpParameters = wParameters;   // Additional parameters
   shExInfo.lpDirectory = 0;
   shExInfo.nShow = SW_SHOW;
   shExInfo.hInstApp = 0;

   if (!ShellExecuteExW(&shExInfo)) 
   {
      LPTSTR msg;
      DWORD code = GetLastError();

      if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&msg,
            0, NULL))
      {
         // show error message
      }
      else
      {
         // show error message
      }
      return MS::kFailure;
   }

   WaitForSingleObject(shExInfo.hProcess, INFINITE);
   CloseHandle(shExInfo.hProcess);
   return MS::kSuccess;
#endif

#ifdef _DARWIN
   AuthorizationRef authorizationRef;
   OSStatus status;

   const char *src;
   const char *dst;
      
   src = file.asChar();
   dst = output.asChar();
   
   status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &authorizationRef);
   if (status != 0)
   {
      // show error message
      return MS::kFailure;
   }

   std::string tool = "/bin/cp";
   char* argsCmd[] = { (char *)src, (char *)dst, NULL };
   FILE* pipe = NULL;

   status = AuthorizationExecuteWithPrivileges(authorizationRef, tool.c_str(), kAuthorizationFlagDefaults, argsCmd, &pipe);
   if (status != 0)
   {
      // show error message
      return MS::kFailure;
   }

   return MS::kSuccess;
#endif

// just an empty return statement to avoid build errors
#ifdef _LINUX
   return MS::kFailure;
#endif
}
