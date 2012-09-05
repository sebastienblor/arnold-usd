#include <ai.h>

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h> 
#include <unistd.h>
#include <map>
#include <string>

AI_DRIVER_NODE_EXPORT_METHODS(SocketDriverMtd);

node_parameters
{
   
}

struct SocketDriverData{
   int socketFd;
   std::map<std::string, int> aovMap;
   
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
   AiDriverInitialize(node, true, data);
}

node_update
{
   
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
   int status;
   addrinfo host_info;
   addrinfo *host_info_list;
   
   memset(&host_info, 0, sizeof host_info);

   host_info.ai_family = AF_INET;
   host_info.ai_socktype = SOCK_STREAM;

   status = getaddrinfo("localhost", "4242", &host_info, &host_info_list);
   if (status != 0)
   {
      AiMsgError("Getaddrinfo error %s", gai_strerror(status));
      return;
   }
   
   int socketFd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
   
   if (socketFd == -1)
   {
      AiMsgError("socket creation error");
      return;
   }
   
   status = connect(socketFd, host_info_list->ai_addr, host_info_list->ai_addrlen);
   if (status == -1)
   {
      AiMsgError("error connecting to the server");
      return;
   }
   
   SocketDriverData* data = (SocketDriverData*)AiDriverGetLocalData(node);
   data->socketFd = socketFd;
   
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
}

driver_close
{
   SocketDriverData* data = (SocketDriverData*)AiDriverGetLocalData(node);
   if (data->socketFd != -1)
      close(data->socketFd);
}

node_finish
{
   delete (SocketDriverData*)AiDriverGetLocalData(node);
}
