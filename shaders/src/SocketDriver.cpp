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
   AiParameterInt("port", 4242);
   AiParameterInt("bucket_threshold", -3);   
   
   AiParameterPTR("swatch", NULL);
   AiMetaDataSetBool(mds, "swatch", "maya.hide", true);
   AiMetaDataSetStr(mds, NULL, "maya.translator", "socket");
   AiMetaDataSetStr(mds, NULL, "maya.attr_prefix", "");
   AiMetaDataSetBool(mds, NULL, "single_layer_driver", false);
   AiMetaDataSetBool(mds, NULL, "display_driver", true);   
}

struct AOVData{
   std::string name;
   AtInt type;
   float* data;   
};

struct SocketDriverData{
   int socketFd;
   int port;
   std::string hostName;
   AtCritSec critSec;
   bool fullUpdate;
   std::vector<AOVData> aovData;
   int width, height;
   
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
   data->port = AiNodeGetInt(node, "port");
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
      if (curr <= 0)
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
   data->width = display_window.maxx - display_window.minx + 1;
   data->height = display_window.maxy - display_window.miny + 1;
   AtNode* options = AiUniverseGetOptions();
   int AASamples = AiNodeGetInt(options, "AA_samples");
   if (AASamples <= AiNodeGetInt(node, "bucket_threshold"))
      data->fullUpdate = true;
   else
      data->fullUpdate = false;
   if (data->fullUpdate && data->aovData.size() == 0)
   {
      const char* outputName;
      int pixelType;
      
      int pixelCount = (data->width) * (data->height);

      while (AiOutputIteratorGetNext(iterator, &outputName, &pixelType, 0))
      {
         const int outputNameLength = strlen(outputName);
         if (outputNameLength < 1)
            continue;
         AOVData aovData;
         aovData.name = outputName;
         aovData.type = pixelType;
         aovData.data = (float*)AiMalloc(pixelCount * (pixelType == AI_TYPE_RGB ? sizeof(AtRGB) : sizeof(AtRGBA)));
         data->aovData.push_back(aovData);
      }
      if (data->socketFd == -1)
         AiOutputIteratorReset(iterator);
   }
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
   int OPT = 1;
   setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, (char*)&OPT, sizeof(int));
   
   hostent* hst;
   hst = gethostbyname(data->hostName.c_str());
   
   sockaddr_in sin;
   memset(&sin, 0, sizeof(sockaddr_in));
   
   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = ((in_addr*)(hst->h_addr))->s_addr;
   sin.sin_port = htons(data->port);
   
   status = connect(socketFd, (sockaddr*)&sin, sizeof(sin));
   if (status == -1)
   {
      AiMsgError("error connecting to the server");
      return;
   }
   
   int ws[2];
   ws[0] = data->width;
   ws[1] = data->height;
   
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
      
      evt = 2;
      SendSocket(socketFd, evt);
      int dataToSend[2] = {pixelType, outputNameLength};
      SendSocket(socketFd, dataToSend, sizeof(int) * 2);
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
   
   if (data->fullUpdate)
   {
      while (AiOutputIteratorGetNext(iterator, &outputName, &pixelType, &bucketData))
      {
         std::string aovName = outputName;
         for (std::vector<AOVData>::iterator it = data->aovData.begin(); it != data->aovData.end(); ++it)
         {
            AOVData& aovData = *it;
            if (aovData.name != aovName)
               continue;
            if (pixelType == AI_TYPE_RGB) // TODO : CLEAN UP THIS LATER!
            {
               AtRGB* trg = (AtRGB*)aovData.data;
               AtRGB* src = (AtRGB*)bucketData;
               for (int x = 0; x < bucket_size_x; ++x)
               {
                  for (int y = 0; y < bucket_size_y; ++y)
                  {
                     trg[bucket_xo + x + (bucket_yo + y) * data->width] = src[x + y * bucket_size_x];
                  }
               }
            }
            else
            {
               AtRGBA* trg = (AtRGBA*)aovData.data;
               AtRGBA* src = (AtRGBA*)bucketData;
               for (int x = 0; x < bucket_size_x; ++x)
               {
                  for (int y = 0; y < bucket_size_y; ++y)
                  {
                     trg[bucket_xo + x + (bucket_yo + y) * data->width] = src[x + y * bucket_size_x];
                  }
               }               
            }            
         }
      }
   }
   else
   {   
      AiCritSecEnter(&data->critSec);

      while (AiOutputIteratorGetNext(iterator, &outputName, &pixelType, &bucketData))
      {
         int evt = 3; // aov data
         SendSocket(data->socketFd, evt);
         int nameLen = strlen(outputName);
         SendSocket(data->socketFd, nameLen);
         SendSocket(data->socketFd, outputName, nameLen);
         int dataToSend[4] = {bucket_xo, bucket_yo, bucket_size_x, bucket_size_y};
         SendSocket(data->socketFd, dataToSend, sizeof(int) * 4);
         int pixelSize = pixelType == AI_TYPE_RGB ? sizeof(AtRGB) : sizeof(AtRGBA);
         pixelSize = pixelSize * bucket_size_x * bucket_size_y;
         SendSocket(data->socketFd, pixelSize);
         SendSocket(data->socketFd, bucketData, pixelSize);
      }

      AiCritSecLeave(&data->critSec);
   }
}

driver_close
{
   SocketDriverData* data = (SocketDriverData*)AiDriverGetLocalData(node);
   if (data->fullUpdate)
   {
      for (std::vector<AOVData>::iterator it = data->aovData.begin(); it != data->aovData.end(); ++it)
      {         
         AOVData& aovData = *it;
         int nameLen = aovData.name.length();
         int evt = 3; // aov data
         SendSocket(data->socketFd, evt);
         SendSocket(data->socketFd, nameLen);
         SendSocket(data->socketFd, aovData.name.c_str(), nameLen);
         int dataToSend[4] = {0, 0, data->width, data->height};
         SendSocket(data->socketFd, dataToSend, sizeof(int) * 4);
         int pixelSize = aovData.type == AI_TYPE_RGB ? sizeof(AtRGB) : sizeof(AtRGBA);
         pixelSize = pixelSize * data->width * data->height;
         SendSocket(data->socketFd, pixelSize);
         SendSocket(data->socketFd, aovData.data, pixelSize);
      }
   }
}

node_finish
{
   SocketDriverData* data = (SocketDriverData*)AiDriverGetLocalData(node);
   if (data->socketFd != -1)
   {
#ifdef WIN32
      closesocket(data->socketFd);
#else
      shutdown(data->socketFd, SHUT_RDWR);
#endif
   }
   AiCritSecClose(&data->critSec);
   for (std::vector<AOVData>::iterator it = data->aovData.begin(); it != data->aovData.end(); ++it)
      AiFree((*it).data);
   delete (SocketDriverData*)AiDriverGetLocalData(node);
}
