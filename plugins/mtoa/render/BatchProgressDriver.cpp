#include <ai.h>

#include <sstream>
#include <string>
#include <cstring> // for memset

#ifdef WIN32
#include <WinSock.h>
#else
#include <sys/socket.h>
#include <netdb.h> 
#include <unistd.h>
#endif

#ifndef WIN32
typedef int SOCKET;
#endif

AI_DRIVER_NODE_EXPORT_METHODS(batch_progress_driver_mtd);

node_parameters
{
   AiParameterInt("port", 4700);
}

static SOCKET g_socketFd;

node_initialize
{
   AiDriverInitialize(node, false, NULL);
   g_socketFd = -1;
   
   int status;
#ifdef WIN32
   WSADATA wsaData;
   WORD version;
   int error;
   version = MAKEWORD(2, 0);
   error = WSAStartup(version, &wsaData); // TODO : check for the error
#endif
   
   SOCKET socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   g_socketFd = socketFd;
   
   if (socketFd == -1)
   {
      return;
   }
   
   hostent* hst;
   hst = gethostbyname("127.0.0.1");
   
   sockaddr_in sin;
   memset(&sin, 0, sizeof(sockaddr_in));
   
   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = ((in_addr*)(hst->h_addr))->s_addr;
   sin.sin_port = htons(AiNodeGetInt(node, "port"));
   
   status = connect(socketFd, (sockaddr*)&sin, sizeof(sin));
   if (status == -1)
   {
      g_socketFd = -1;
      return;
   }
}

void SendSocket(SOCKET socketFd, const void* data, int dataSize)
{
   int rem = dataSize;
   const char* d = (const char*)data;
   while(rem)
   {
      int curr = send(socketFd, d, MIN(1024, rem), 0); // without this there might be some problems (especially on windows)
      if (curr <= 0)
         return;
      rem = rem - curr;
      d += curr;
   }
}

node_update
{
   
}

driver_supports_pixel_type
{
   return true;
}

driver_extension
{
   return NULL;
}

static int g_totalPixels;
static int g_calculatedPixels;
static int g_lastReportedPercent;
static int g_frameNumber = 0;

driver_open
{
   AtNode* options = AiUniverseGetOptions();
   const int regionMinX = AiNodeGetInt(options, "region_min_x");
   if (regionMinX == -1)
   {
      const int xRes = AiNodeGetInt(options, "xres");
      const int yRes = AiNodeGetInt(options, "yres");
      
      g_totalPixels = xRes * yRes;
   }
   else
   {
      const int regionMinY = AiNodeGetInt(options, "region_min_y");
      const int regionMaxX = AiNodeGetInt(options, "region_max_x");
      const int regionMaxY = AiNodeGetInt(options, "region_max_y");
      
      g_totalPixels = (regionMaxX - regionMinX + 1) * (regionMaxY - regionMinY + 1);
   }
   
   g_calculatedPixels = 0;
   g_lastReportedPercent = -1;
   if (g_socketFd != -1)
   {
      std::stringstream ss;
      if (AiNodeLookUpUserParameter(options, "frame") != 0)
      {
         const float frame = AiNodeGetFlt(options, "frame");
         if (AiNodeLookUpUserParameter(options, "render_layer") != 0)
         {
            ss << "print \"Started Render of Layer " << AiNodeGetStr(options, "render_layer")
               << " of Frame " << frame << "\\n\";";
         }
         else ss << "print \"Started Render of Frame " << frame << "\\n\";";
      }
      else ss << "print \"Started Render of Frame " << ++g_frameNumber << "\\n\";";
      SendSocket(g_socketFd, ss.str().c_str(), (int)ss.str().length() + 1);
   }
}

#if AI_VERSION_ARCH_NUM == 4 && AI_VERSION_MAJOR_NUM == 1

driver_needs_bucket
{
   return true;
}

driver_process_bucket
{

}

#endif

driver_prepare_bucket
{
   
}

driver_write_bucket
{   
   if (g_socketFd != -1)
   {
      g_calculatedPixels += bucket_size_x * bucket_size_y;      
      const int currentPercent = (int)(100.f * ((float)g_calculatedPixels / (float)g_totalPixels)) / 5;
      if (currentPercent != g_lastReportedPercent)
      {
         std::stringstream ss;
         g_lastReportedPercent = currentPercent;
         ss << "print \"Render progress : " << currentPercent * 5 << "%\\n\";";
         SendSocket(g_socketFd, ss.str().c_str(), (int)ss.str().length() + 1);
      }      
   }
}

driver_close
{
    if (g_socketFd != -1)
   {
#ifdef WIN32
      closesocket(g_socketFd);
#else
      shutdown(g_socketFd, SHUT_WR);
#endif
   }
}

node_finish
{
   
}
