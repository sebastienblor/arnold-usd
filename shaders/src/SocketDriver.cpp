#include <ai.h>

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h> 

AI_DRIVER_NODE_EXPORT_METHODS(SocketDriverMtd);

node_parameters
{
   
}

struct SocketDriverData{
   int socketFd;
};

node_initialize
{
   SocketDriverData* data = (SocketDriverData*)AiMalloc(sizeof(SocketDriverData));
   AiDriverInitialize(node, false, data);
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
      std::cerr << "getaddrinfo error " << gai_strerror(status) << std::endl;
      return;
   }
   
   int socketFd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
   
   if (socketFd == -1)
   {
      std::cerr << "socket creation error" << std::endl;
      return;
   }
   
   status = connect(socketFd, host_info_list->ai_addr, host_info_list->ai_addrlen);
   if (status == -1)
   {
      std::cerr << "error connecting to the server" << std::endl;
      return;
   }
   
   SocketDriverData* data = (SocketDriverData*)AiDriverGetLocalData(node);
   data->socketFd = socketFd;
   
   int ws[2];
   ws[0] = display_window.maxx - display_window.minx + 1;
   ws[1] = display_window.maxy - display_window.miny + 1;
   
   send(socketFd, (char*)ws, sizeof(int) * 2, 0);
}

driver_prepare_bucket
{
   
}

driver_write_bucket
{
   
}

driver_close
{
   
}

node_finish
{
   
}
