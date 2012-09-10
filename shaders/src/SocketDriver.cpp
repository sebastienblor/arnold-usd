#include <ai.h>

#include <iostream>
#include <cstring>

#include <map>
#include <string>

#ifdef WIN32
#include <WinSock.h>
#else
#include <sys/socket.h>
#include <netdb.h> 
#include <unistd.h>
#endif

AI_DRIVER_NODE_EXPORT_METHODS(SocketDriverMtd);

node_parameters
{
   AiParameterStr("host_name", "localhost");
   
   AiParameterPTR("swatch", NULL);
   AiMetaDataSetBool(mds, "swatch", "maya.hide", true);
   AiMetaDataSetStr(mds, NULL, "maya.translator", "socket");
   AiMetaDataSetStr(mds, NULL, "maya.attr_prefix", "");
   AiMetaDataSetBool(mds, NULL, "single_layer_driver", false);
   AiMetaDataSetBool(mds, NULL, "display_driver", true);   
}

struct SocketDriverData{
   int socketFd;
   std::map<std::string, int> aovMap;
   std::string hostName;
   AtCritSec critSec;
   
   static void* operator new(size_t size)
   {
      return AiMalloc(size);
   }
   
   static void operator delete(void* p)
   {
      AiFree(p);
   }
};

node_initialize
{
   SocketDriverData* data = new SocketDriverData;
   data->socketFd = -1;
   AiDriverInitialize(node, true, data);
}

node_update
{
   SocketDriverData* data = (SocketDriverData*)AiDriverGetLocalData(node);
   data->hostName = AiNodeGetStr(node, "host_name");
}

driver_supports_pixel_type
{
   switch (pixel_type)
   {
      case AI_TYPE_RGB:
      case AI_TYPE_RGBA:
         return true;
      default:
         return false;
   }
}

driver_extension
{
   return 0;
}

void SendSocket(int socketFd, const void* data, int dataSize)
{
   int rem = dataSize;
   const char* d = (const char*)data;
   while(rem)
   {
      int curr = send(socketFd, d, MIN(1024, rem), 0); // without this there might be some problems (especially on windows)
      if (curr == -1)
         return;
      rem = rem - curr;
      d += curr;
   }
}

template <typename T>
void SendSocket(int socketFd, const T& data)
{
   SendSocket(socketFd, &data, sizeof(T));
}

driver_open
{
   SocketDriverData* data = (SocketDriverData*)AiDriverGetLocalData(node);
   if (data->socketFd != -1)
      return;
   AiCritSecInit(&data->critSec);
   
   int status;
#ifdef WIN32
   WSADATA wsaData;
   WORD version;
   int error;
   version = MAKEWORD(2, 0);
   error = WSAStartup(version, &wsaData); // TODO : check for the error
#endif
   
   int socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   data->socketFd = socketFd;
   
   if (socketFd == -1)
   {
      AiMsgError("socket creation error");
      return;
   }
   
   hostent* hst;
   hst = gethostbyname(data->hostName.c_str());
   
   sockaddr_in sin;
   memset(&sin, 0, sizeof(sockaddr_in));
   
   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = ((in_addr*)(hst->h_addr))->s_addr;
   sin.sin_port = htons(4242);
   
   status = connect(socketFd, (sockaddr*)&sin, sizeof(sin));
   if (status == -1)
   {
      AiMsgError("error connecting to the server");
      return;
   }
   
   int ws[2];
   ws[0] = display_window.maxx - display_window.minx + 1;
   ws[1] = display_window.maxy - display_window.miny + 1;
   
   int evt = 1;
   
   SendSocket(socketFd, evt);
   SendSocket(socketFd, ws, sizeof(int) * 2);
   
   const char* outputName;
   int pixelType;
   
   while (AiOutputIteratorGetNext(iterator, &outputName, &pixelType, 0))
   {
      const int outputNameLength = strlen(outputName);
      if (outputNameLength < 1)
         continue;
      
      int aovID = data->aovMap.size();
      data->aovMap[outputName] = aovID;
      
      evt = 2;
      SendSocket(socketFd, evt);
      SendSocket(socketFd, aovID);
      SendSocket(socketFd, pixelType);
      SendSocket(socketFd, outputNameLength);
      SendSocket(socketFd, outputName, outputNameLength);
   }
}

driver_prepare_bucket
{
   
}

driver_write_bucket
{
   SocketDriverData* data = (SocketDriverData*)AiDriverGetLocalData(node);
   
   const char* outputName;
   int pixelType;
   const void* bucketData;
   
   AiCritSecEnter(&data->critSec);
   
   while (AiOutputIteratorGetNext(iterator, &outputName, &pixelType, &bucketData))
   {
      std::map<std::string, int>::iterator it = data->aovMap.find(outputName);
      if (it == data->aovMap.end())
         continue;
      int evt = 3; // aov data
      SendSocket(data->socketFd, evt);
      SendSocket(data->socketFd, it->second);
      SendSocket(data->socketFd, bucket_xo);
      SendSocket(data->socketFd, bucket_yo);
      SendSocket(data->socketFd, bucket_size_x);
      SendSocket(data->socketFd, bucket_size_y);
      int pixelSize = pixelType == AI_TYPE_RGB ? sizeof(AtRGB) : sizeof(AtRGBA);
      SendSocket(data->socketFd, bucketData, pixelSize * bucket_size_x * bucket_size_y);
   }
   
   AiCritSecLeave(&data->critSec);
}

driver_close
{
   
}

node_finish
{
   SocketDriverData* data = (SocketDriverData*)AiDriverGetLocalData(node);
   if (data->socketFd != -1)
   {
#ifdef WIN32
      closesocket(data->socketFd);
#else
      close(data->socketFd);
#endif
   }
   AiCritSecClose(&data->critSec);
   delete (SocketDriverData*)AiDriverGetLocalData(node);
}
