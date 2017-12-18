#include "ArnoldLicenseCmd.h"

#include <ai_dotass.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>
#include <ai_bbox.h>
#include <ai_threads.h>
#include <ai_license.h>
#ifdef _WIN32
#include <Windows.h> // use < > for all system and library headers
#include <winuser.h>
#endif

#include <math.h>
#include <maya/MArgDatabase.h>
#include <maya/MStringArray.h>
#include <maya/MEventMessage.h>
#include <maya/MGlobal.h>
#include <maya/MMutexLock.h>
#include <maya/MFileObject.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>



/*
int RunProgram(const char* command, std::string& out)
{
#ifdef _WIN32
   STARTUPINFOW si;
   memset(&si, 0, sizeof(si));
   si.cb = sizeof(si);
   PROCESS_INFORMATION pi;
   SECURITY_ATTRIBUTES sa;
   sa.nLength = sizeof(SECURITY_ATTRIBUTES);
   sa.lpSecurityDescriptor = NULL;
   sa.bInheritHandle = true;
   // Create one-way pipe for child process STDOUT
   HANDLE childStdoutRd = NULL;
   HANDLE childStdoutWr = NULL;
   if (!CreatePipe(&childStdoutRd, &childStdoutWr, &sa, 0))
      return 1;
   // Ensure read handle to pipe for STDOUT is not inherited
   if (!SetHandleInformation(childStdoutRd, HANDLE_FLAG_INHERIT, 0))
      return 2;
   si.hStdError = childStdoutWr;
   si.hStdOutput = childStdoutWr;
   si.dwFlags |= STARTF_USESTDHANDLES;
   // create process
   wchar_t wcmd[2048];
   mbstowcs(wcmd, command, strlen(command)+1);//Plus null
   BOOL res = CreateProcessW(NULL, wcmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
   if (!res)
      return GetLastError();
   CloseHandle(childStdoutWr);
   // read output
   CHAR aBuf[1024 + 1];
   DWORD dwRead;
   while (true) 
   {
      BOOL bStatus = ReadFile(childStdoutRd, aBuf, sizeof(aBuf), &dwRead, NULL);
      if (!bStatus || dwRead == 0) 
         break;
      aBuf[dwRead] = '\0';
      out += aBuf;
   }
   CloseHandle(childStdoutRd);
   // wait for the process to exit
   ULONG rc;
   WaitForSingleObject(pi.hProcess, INFINITE);
   if(!GetExitCodeProcess(pi.hProcess, &rc))
        rc = 0;
   CloseHandle(pi.hThread);
   CloseHandle(pi.hProcess);
   return (int)rc;
#else
   //return system(command);
   char buffer[128];
   FILE* pipe = popen(command, "r");
   if (!pipe) 
      return 1;
   while (!feof(pipe)) 
   {
      if (fgets(buffer, 128, pipe) != NULL)
         out += buffer;
   }
   int res = pclose(pipe);
   if (res == -1)
      return errno;
   return res;
#endif
}*/

bool IsValidStr(const char *str)
{
   if (str == NULL || (str[0] == '\0'))
      return false;

   return true;
}

std::string GetEnvVar(const char* name)
{
   if (!IsValidStr(name))
      return std::string();

   char value[4096] = "";

#ifdef _WIN32
   HKEY hKey;
   LSTATUS result;

   result = RegOpenKeyExW(HKEY_CURRENT_USER, L"Environment", 0, KEY_READ, &hKey);
   if (result == ERROR_SUCCESS)
   {
      WCHAR szBuffer[2048];
      DWORD dwBufferSize = sizeof(szBuffer);
      wchar_t wName[512];
      std::mbstowcs(wName, name, strlen(name) + 1);
      result = RegQueryValueExW(hKey, wName, 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
      if (result == ERROR_SUCCESS)
      {
         std::wstring wStr(szBuffer);
         std::wcstombs(value, wStr.c_str(), 4096);
      }
      RegCloseKey(hKey);
   }

#elif defined _DARWIN
   // read the current value from the plist file
   std::string filename = std::string("setenv.") + std::string(name) + std::string(".plist");
   const char *homeDir = getenv("HOME");
   MString plistFn = (homeDir) ? homeDir : "";
   plistFn += "/Library/LaunchAgents/";
   plistFn += filename.c_str();

   std::ifstream file(plistFn.asChar());
   if (file.good() && file.is_open())
   {
      for (std::string line; std::getline(file, line);)
      {
         std::string pattern = std::string("launchctl setenv ") + std::string(name) + std::string(" ");
         std::string::size_type startIndex = line.find(pattern);
         if (startIndex != std::string::npos)
         {
            startIndex += pattern.length();
            std::string::size_type endIndex = line.find("</", startIndex);
            std::string::size_type count = endIndex == std::string::npos ? std::string::npos : endIndex - startIndex;
            std::string currentValue = line.substr(startIndex, count);
            strcpy(value, currentValue.c_str());
         }
      }
      file.close();
   }
#endif

   return std::string(value);
}

bool RemoveEnvVar(const char* name)
{
   if (!IsValidStr(name))
      return true;

//   #ifdef DEBUG
//   debug("remove environment variable %s", name);
//   #endif

#ifdef _WIN32
   HKEY hKey;
   LSTATUS result;

   result = RegOpenKeyExW(HKEY_CURRENT_USER, L"Environment", 0, KEY_ALL_ACCESS, &hKey);
   if (result != ERROR_SUCCESS)
      return false;

   wchar_t wName[512];
   std::mbstowcs(wName, name, strlen(name) + 1);
   result = RegDeleteValueW(hKey, wName);
   RegCloseKey(hKey);
   if (result != ERROR_SUCCESS)
      return false;

   // activate the settings
   SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0,
      (LPARAM) "Environment", SMTO_ABORTIFHUNG, 1000, NULL);

   // update the local env
   _putenv_s(name, "");

#elif defined _DARWIN
   // on OSX we use a launch agent to setup the environment variables
   // placed to the ~/Library/LaunchAgents folder

   // backup the original file
   std::string filename = std::string("setenv.") + std::string(name) + std::string(".plist");
   const char *homeDir = getenv("HOME");
   MString plistFn = (homeDir) ? homeDir : "";
   plistFn += "/Library/LaunchAgents/";
   plistFn += filename.c_str();

   std::ifstream ifile(plistFn.asChar());

   if (ifile.good())
      if (remove(plistFn.asChar()) != 0)
         return false;

   // update the local env
   setenv(name, "", 1);
#endif
   return true;
}


bool SetEnvVar(const char* name, const char* value, bool append = false)
{

   if (!IsValidStr(name))
      return true;

   // remove the environment variable when no value is set
   if (!IsValidStr(value))
   {
      if (append)
         return true;
      else
         return RemoveEnvVar(name);
   }

   std::string keyValue(value);

   // read the current value when append is set
   std::string currentValue;
   if (append)
   {
      std::string currentValue = GetEnvVar(name);
      //keyValue = AppendValue(currentValue.c_str(), ';', value);
      currentValue += ";";
      currentValue += value;
   }

#ifdef _WIN32
   HKEY hKey;
   LSTATUS result;

   wchar_t wName[512];
   std::mbstowcs(wName, name, strlen(name) + 1);

   wchar_t wValue[3000];
   std::mbstowcs(wValue, keyValue.c_str(), keyValue.length() + 1);

   // update the registry
   result = RegOpenKeyExW(HKEY_CURRENT_USER, L"Environment", 0, KEY_WRITE, &hKey);
   if (result != ERROR_SUCCESS)
      return false;
   
   result = RegSetValueExW(hKey, wName, 0, REG_SZ, (LPBYTE)wValue, (DWORD)(wcslen(wValue)+1) * sizeof(WCHAR));
   RegCloseKey(hKey);

   if (result != ERROR_SUCCESS)
      return false;

   // activate the settings
   SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0,
      (LPARAM) "Environment", SMTO_ABORTIFHUNG, 1000, NULL);

   // update the local env
   _putenv_s(name, value);

#elif defined _DARWIN
   // on OSX we use a launch agent to setup the environment variables
   // placed to the ~/Library/LaunchAgents folder

   // backup the original file
   std::string filename = std::string("setenv.") + std::string(name) + std::string(".plist");
   const char *homeDir = getenv("HOME");
   MString plistFn = (homeDir) ? homeDir : "";
   plistFn += "/Library/LaunchAgents/";
   MString plistFolder = plistFn;
   plistFn += filename.c_str();

   MString plistBackupFn = plistFn + MString(".bak");
   MFileObject ifile;
   ifile.setRawFullName(plistFn);
   if (ifile.exists())
   {
      if (rename(plistFn.asChar(), plistBackupFn.asChar()) != 0)
         return false;
   } else
   {
      MFileObject dirObject;
      dirObject.setRawFullName(plistFolder);
      if (!dirObject.exists())
         mkdir(plistFolder.asChar(), 0775);
   
   }   

   bool success = true;
   // update the file
   std::ofstream out(plistFn.asChar());
   if (out.is_open())
   {
      out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
      out << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
      out << "<plist version=\"1.0\">\n";
      out << "<dict>\n";
      out << "  <key>Label</key>\n";
      out << "  <string>setenv." << name << "</string>\n";
      out << "  <key>ProgramArguments</key>\n";
      out << "  <array>\n";
      out << "    <string>sh</string>\n";
      out << "    <string>-c</string>\n";
      out << "    <string>launchctl setenv " << name << " " << keyValue << "</string>\n";
      out << "  </array>\n";
      out << "  <key>RunAtLoad</key>\n";
      out << "  <true/>\n";
      out << "  <key>KeepAlive</key>\n";
      out << "  <true/>\n";
      out << "</dict>\n";
      out << "</plist>\n";

      out.close();
   }
   else
      success = false;

   // remove the backup file on success
   if (success)
   {      
      MFileObject plistBackupObj;
      plistBackupObj.setRawFullName(plistBackupFn);
      if (plistBackupObj.exists())
         remove(plistBackupFn.asChar());

      // update the local env
      setenv(name, value, 1);

      // reload the file
      // TODO does not reload the env var globally
      //std::string output;
      //std::string command = std::string("launchctl load -F ") + std::string(plistPath);
      //RunProgram(command.c_str(), output);
   }
   // restore the file on error
   else
   {
      MFileObject plistObj;
      plistObj.setRawFullName(plistFn);
      if (plistObj.exists())
         remove(plistFn.asChar());

      plistObj.setRawFullName(plistBackupFn);
      if (plistObj.exists())
         rename(plistBackupFn.asChar(), plistFn.asChar());
   }
#else
   // update the local env
   setenv(name, value, 1);
#endif

   return true;
}

void copyToClipboard(const MString &str)
{
   std::string s(str.asChar());

#ifdef _WIN32
   HWND hwnd = GetDesktopWindow();

   OpenClipboard(hwnd);
   EmptyClipboard();
   HGLOBAL hg=GlobalAlloc(GMEM_MOVEABLE,s.size()+1);
   if (!hg)
   {
      CloseClipboard();
      return;
   }
   memcpy(GlobalLock(hg),s.c_str(),s.size()+1);
   GlobalUnlock(hg);
   SetClipboardData(CF_TEXT,hg);
   CloseClipboard();
   GlobalFree(hg);
#endif

}

static MCallbackId s_idle_cb = 0;

struct ArnoldLicenseInfo
{
   ArnoldLicenseInfo() : needDetails(false), 
                         rlmStatus(AI_LIC_ERROR_INIT),
                         rlmLicenseCount(0),
                         rlmLicenseInUse(0),
                         nlmStatus(AI_LIC_ERROR_INIT),
                         nlmLicenseCount(0),
                         nlmLicenseInUse(0),
                         valid(true) {}


   bool needDetails;
   int rlmStatus;
   int rlmLicenseCount;
   int rlmLicenseInUse;
   int nlmStatus;
   int nlmLicenseCount;
   int nlmLicenseInUse;
   bool valid;
   MString details;
   MMutexLock mutex;
};
static ArnoldLicenseInfo *s_license_info = NULL;


void FinishedGetStatus(void *data)
{
   std::string *message = (std::string *)data;
   MMessage::removeCallback(s_idle_cb);
   s_idle_cb = 0;

   if (message == NULL || message->empty())
      return;

   MString cmd(message->c_str());
   delete message;
   
   MGlobal::executePythonCommand(cmd);

}

static void StrTrim(std::string &line)
{
   while((!line.empty()) && line[0] == ' ')
      line.erase(line.begin());


   while((!line.empty()) && line[line.length() - 1] == ' ')
      line.erase(line.begin() + line.length() - 1);

}

static bool StrStartsWith(const std::string &line, std::string startLine)
{
   if (startLine.length() > line.length())
      return false;

   for (size_t i = 0; i < startLine.length(); ++i)
   {
      if (startLine[i] != line[i])
         return false;
   }

   return true;
}

unsigned int ArnoldLicenseInfoThread(void* data)
{
   ArnoldLicenseInfo* info = (ArnoldLicenseInfo*)data;
   if (info == NULL)
      return 0;

   // get the RLM license info
   AtLicenseInfo* licenses;
   unsigned int numLicenses;
   info->rlmStatus = AiLicenseGetInfo(licenses, numLicenses);


   bool hasArnoldLicense = false;
   for (unsigned int i = 0; i < numLicenses; i++)
   {
      AtLicenseInfo& license = licenses[i];
      if (strcmp(license.name, "arnold"))
         continue;

      hasArnoldLicense = true;

      info->rlmLicenseCount = license.count;
      info->rlmLicenseInUse = license.current_inuse;
      break;
   }

   if (!hasArnoldLicense)
      info->rlmStatus = AI_LIC_ERROR_NOTFOUND;

   // get the NLM license info
   // run lmutil lmstat and parse the output
   //if (info->status != AI_LIC_SUCCESS && info->status != AI_LIC_ERROR_NOTAVAILABLE)
   {
      std::string output;

      MString res;
      MGlobal::executePythonCommand("import mtoa.licensing;mtoa.licensing.nlmStatus()", res);
      output = res.asChar();

      bool connected = false;
      bool inFeatureSection = false;

      std::istringstream sstream(output);

      for (std::string line; std::getline(sstream, line);)
      {
         StrTrim(line);
         if (line.empty())
            continue;

         // we could connect to a server which has the adskflex vendor
         if (StrStartsWith(line.c_str(), "adskflex:"))
            connected = true;

         if (StrStartsWith(line.c_str(), "Feature usage info:"))
            inFeatureSection = true;
         if (StrStartsWith(line.c_str(), "License server status:"))
            inFeatureSection = false;

         // check for the Arnold feature
         // the line in the feature info looks like this:
         // Users of XXXXXARNOL_[YEAR]_0F:  (Total of 50 licenses issued;  Total of 0 licenses in use)
         if (inFeatureSection)
         {
            if (line.length() > 14 && StrStartsWith(line.c_str()+14, "ARNOL_"))
            {
               // read the number of licenses issued
               std::string::size_type n = line.find("Total of ");
               if (n != std::string::npos)
               {
                  MString licCount(line.substr(n+9).c_str());
                  info->nlmLicenseCount = licCount.asInt();

                  // read the number of licenses in use
                  n = line.find("Total of ", n+1);
                  if (n != std::string::npos)
                  {
                     MString licInUse(line.substr(n + 9).c_str());
                     info->nlmLicenseInUse = licInUse.asInt();
                  }
               }
            }
         }
      }

      info->nlmStatus = AI_LIC_ERROR_CANTCONNECT;
      if (connected)
      {
         if (info->nlmLicenseCount == 0)
            info->nlmStatus = AI_LIC_ERROR_NOTFOUND;
         else
            info->nlmStatus = AI_LIC_SUCCESS;
      }
   }

   info->mutex.lock();
   if (!info->valid)
   {
      info->mutex.unlock();
      delete info;
      return 0;
   }
   s_license_info = NULL;
   MString cmd = "import mtoa.licensing;mtoa.licensing.returnServerStatus(";
   cmd += info->rlmStatus;
   cmd += ",";
   cmd += info->rlmLicenseCount;
   cmd += ",";
   cmd += info->rlmLicenseInUse;
   cmd += ",";
   cmd += info->nlmStatus;
   cmd += ",";
   cmd += info->nlmLicenseCount;
   cmd += ",";
   cmd += info->nlmLicenseInUse;
   cmd += ")";

   std::string *message = new std::string(cmd.asChar());

   MStatus status;
   s_idle_cb = MEventMessage::addEventCallback("idle",
                                                  FinishedGetStatus,
                                                  message,
                                                  &status);

   info->mutex.unlock();
   delete info;

   return 0;
}

static void GetServerStatus()
{
   if (s_license_info)
   {
      s_license_info->mutex.lock();
      s_license_info->valid = false;
      s_license_info->mutex.unlock();
   }
   
   s_license_info = new ArnoldLicenseInfo();

   void* thread = AiThreadCreate(ArnoldLicenseInfoThread, s_license_info, AI_PRIORITY_NORMAL);
   AiThreadClose(thread);
}


MSyntax CArnoldLicenseCmd::newSyntax()
{
   MSyntax syntax;
   
   syntax.addFlag("gr", "getRlmServer");
   syntax.addFlag("gn", "getNlmServer");
   syntax.addFlag("sr", "setRlmServer", MSyntax::kString);
   syntax.addFlag("sn", "setNlmServer", MSyntax::kString);
   syntax.addFlag("cc", "copyToClipboard", MSyntax::kString); // FIXME is there another way to do that natively in Maya API ?   
   syntax.addFlag("rs", "runServerStatus");
   return syntax;
}

MStatus CArnoldLicenseCmd::doIt(const MArgList& argList)
{
   MStatus status;
   MArgDatabase args(syntax(), argList);
   

   if (args.isFlagSet("copyToClipboard"))
   {
      copyToClipboard(args.flagArgumentString("copyToClipboard", 0) );
      return MS::kSuccess;
   }

   if (args.isFlagSet("getRlmServer"))
   {      
      std::string rlmServer = GetEnvVar("solidangle_LICENSE");
      setResult(MString(rlmServer.c_str()));
      return MS::kSuccess;   
   }
   if (args.isFlagSet("getNlmServer"))
   {      
      std::string nlmServer = GetEnvVar("ADSKFLEX_LICENSE_FILE");
      setResult(MString(nlmServer.c_str()));
      return MS::kSuccess;   
   }
   if (args.isFlagSet("setRlmServer"))
   {      
      MString rlmServer = args.flagArgumentString("setRlmServer", 0) ;
      return (SetEnvVar("solidangle_LICENSE", rlmServer.asChar())) ? MS::kSuccess : MS::kFailure;
   }

   if (args.isFlagSet("setNlmServer"))
   {
      MString nlmServer = args.flagArgumentString("setNlmServer", 0) ;
      return (SetEnvVar("ADSKFLEX_LICENSE_FILE", nlmServer.asChar())) ? MS::kSuccess : MS::kFailure;      
   }
   if (args.isFlagSet("runServerStatus"))
   {
      GetServerStatus();
      return MS::kSuccess;
   }
   return MS::kSuccess;
}
