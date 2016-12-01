// $Id$

#include <ai.h>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

AI_DRIVER_NODE_EXPORT_METHODS(MPlayDriverMtd);

// Array size. Only those 3 pixel size values are supported by Houdini.
enum ArraySize
{
    ARRAY_SIZE_SCALAR = 1,
    ARRAY_SIZE_RGB    = 3,
    ARRAY_SIZE_RGBA   = 4
};

/// Output (AOV) info
struct Output
{
    std::string name;          ///< AOV name
    ArraySize array_size; ///< AOV pixel size
};

/// Driver local data
struct DriverData
{
    std::vector<Output> outputs;
#ifdef _WIN32
    int fp; // string if a process is valid, for compatibility reasons
    PROCESS_INFORMATION process_information;
    HANDLE write_pipe;
    HANDLE read_pipe;
#else
    FILE* fp;               ///< Pipe file descriptor
#endif
};

/// Pixel size from Arnold type
ArraySize arraySize(int arnold_type)
{
    switch (arnold_type)
    {
    case AI_TYPE_RGBA:
        return ARRAY_SIZE_RGBA;

    case AI_TYPE_FLOAT:
        return ARRAY_SIZE_SCALAR;

    case AI_TYPE_RGB:
    case AI_TYPE_VECTOR:
    default:
        return ARRAY_SIZE_RGB;
    }
}

/// Open pipe process
void openPipeCommand(DriverData* ctx)
{
    char* HB = getenv("__HOUDINI_BINARY_FOLDER");
    if (HB == NULL)
    {
        AiMsgWarning("[mplay_driver] This driver can only be used in the Houdini environment");
        ctx->fp = 0;
        return;
    }
    
#ifndef _WIN32
    std::stringstream cmd;
    #ifdef _LINUX
        cmd << "unset LD_LIBRARY_PATH;\"" << HB << "/imdisplay\" -f -n Arnold -k -p";
    #else // DARWIN
        cmd << "unset DYLD_LIBRARY_PATH;\"" << HB << "/imdisplay\" -f -n Arnold -k -p"; // is this really neccessary?
    #endif
#endif
    

#ifdef _WIN32
    AiMsgDebug("[mplay_driver] Spawning process : %s from directory : %s", "imdisplay -f -n Arnold -k -p", HB);
    ctx->fp = 1;

    SECURITY_ATTRIBUTES sa_attrs;
    sa_attrs.nLength = sizeof(SECURITY_ATTRIBUTES); 
    sa_attrs.bInheritHandle = true; 
    sa_attrs.lpSecurityDescriptor = NULL;
    ctx->read_pipe = NULL;
    ctx->write_pipe = NULL;

    if (!CreatePipe(&ctx->read_pipe, &ctx->write_pipe, &sa_attrs, 0))
    {
        ctx->fp = 0;
        AiMsgWarning("[mplay_driver] Error creating the pipes for the process, error code : %i", GetLastError());
    }

    if ((ctx->fp != 0) && !SetHandleInformation(ctx->write_pipe, HANDLE_FLAG_INHERIT, 0))
    {
        ctx->fp = 0;
        AiMsgWarning("[mplay_driver] Error setting the write pipe's handle information, error code : %i", GetLastError());
    }
    
    if (ctx->fp == 1)
    {
        STARTUPINFO start_info;
        ZeroMemory(&ctx->process_information, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&start_info, sizeof(STARTUPINFO));

        start_info.hStdInput = ctx->read_pipe;
        start_info.dwFlags |= STARTF_USESTDHANDLES;
        start_info.cb = sizeof(STARTUPINFO);
        start_info.wShowWindow = SW_HIDE;

        BOOL success = CreateProcess(
                NULL,
                "imdisplay.exe -f -n Arnold -k -p",
                NULL,
                NULL,
                true,
                CREATE_NO_WINDOW,
                NULL,
                HB,
                &start_info,
                &ctx->process_information
            );

        if (success != TRUE)
        {
            ctx->fp = 0;
            DWORD lastError = GetLastError();
            AiMsgWarning("[mplay_driver] Error spawning the windows process, code : %i", lastError);
        }
    }
   
#else
    AiMsgDebug("[mplay_driver] Launching pipe command: %s", cmd.str().c_str());

    ctx->fp = popen(cmd.str().c_str(), "w");
#endif

    if (ctx->fp)
        AiMsgDebug("[mplay_driver] Houdini pipe opened");
    else
        AiMsgWarning("[mplay_driver] Could not open Houdini pipe command");
}

#ifdef _WIN32

void writeCompletionCallback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransferred, LPOVERLAPPED lpOverlapped)
{
    free(lpOverlapped);
}

#endif

bool writeData(const void* data, size_t elem_size, size_t elem_count, DriverData* ctx)
{
#ifdef _WIN32
    DWORD bytesToWrite = static_cast<DWORD>(elem_size * elem_count);
    LPOVERLAPPED pOverlapped = reinterpret_cast<LPOVERLAPPED>(malloc(sizeof(OVERLAPPED)));
    ZeroMemory(pOverlapped, sizeof(OVERLAPPED));
    if (WriteFileEx(ctx->write_pipe, data, bytesToWrite, pOverlapped, writeCompletionCallback) == TRUE)
        return true;
    else
    {
        AiMsgWarning("[mplay_driver] Error writing to pipe, error code : %i", GetLastError());
        return false;
    }
#else
    return fwrite(data, elem_size, elem_count, ctx->fp) == elem_count;
#endif
}

void flushData(DriverData* ctx)
{
#ifdef _WIN32
    //if (FlushFileBuffers(ctx->write_pipe) != true)
    //    AiMsgWarning("Error flushing the write pipe, error code : %i", GetLastError());
#else
    fflush(ctx->fp);
#endif
}

/// Write bucket
void writeBucket(DriverData* ctx, int index, int x0, int x1, int y0, int y1,
                 int pixel_size, const void* pixels)
{
    int bucket_header[4];

    // First, tell the reader what the plane index is for the data being
    // sent.  This is done by sending a special tile header.  The x0
    // coordinate is set to -1 to indicate the special header.  The x1
    // coordinate is set to the plane index.  The Y coordinates must be
    // zero.
    bucket_header[0] = -1;
    bucket_header[1] = index;
    bucket_header[2] = 0;
    bucket_header[3] = 0;

    // write plane index
    if (!writeData(bucket_header, sizeof(int), 4, ctx))
        AiMsgWarning("[mplay_driver] Unable to write plane definition");

    // fill out the tile header
    bucket_header[0] = x0;
    bucket_header[1] = x1;
    bucket_header[2] = y0;
    bucket_header[3] = y1;
    size_t npixels = (x1 - x0 + 1) * (y1 - y0 + 1);

    // write header
    if (!writeData(bucket_header, sizeof(int), 4, ctx))
        AiMsgWarning("[mplay_driver] Error writing bucket: %d %d %d %d\n", x0, x1, y0, y1);

    // write data
    if (!writeData(pixels, pixel_size * sizeof(float), npixels, ctx))
        AiMsgWarning("[mplay_driver] Error writing data: %d %d %d %d\n", x0, x1, y0, y1);

    flushData(ctx);
}

/// Write image header
void writeImageHeader(DriverData* ctx)
{
    AtNode* options = AiUniverseGetOptions();
    int xres = AiNodeGetInt(options, "xres");
    int yres = AiNodeGetInt(options, "yres");

    // write header
    int header[8];
    memset(header, 0, sizeof(header));
    header[0] = ('h' << 24) + ('M' << 16) + ('P' << 8) + ('0');
    header[1] = xres;
    header[2] = yres;
    header[3] = 0;  // For simple images, this would be the data format
    header[4] = 0;  // For simple images, this would be the array size
    header[5] = static_cast<int>(ctx->outputs.size());

    AiMsgDebug("[mplay_driver] Header: %d x %d x %lu", xres, yres, ctx->outputs.size());

    if (!writeData(header, sizeof(int), 8, ctx))
        AiMsgWarning("[mplay_driver] Unable to write header");

    // write plane definitions
    for (size_t i = 0; i < ctx->outputs.size(); ++i)
    {
        int plane_def[8];
        size_t name_length = ctx->outputs[i].name.size();
        memset(plane_def, 0, sizeof(plane_def));
        plane_def[0] = static_cast<int>(i);                         // Sequentially increasing integer
        plane_def[1] =  static_cast<int>(name_length);              // The length of the plane name
        plane_def[2] = 0;                                           // always float for Arnold
        plane_def[3] = ctx->outputs[i].array_size;                  // Array size of the data

        if (!writeData(plane_def, sizeof(int), 8, ctx))
            AiMsgWarning("[mplay_driver] Unable to write plane definition");

        if (!writeData(ctx->outputs[i].name.c_str(), sizeof(char), name_length, ctx))
            AiMsgWarning("[mplay_driver] Unable to write plane name");
    }

    // blank planes
    float* pixels = (float*) calloc(xres * yres, ARRAY_SIZE_RGBA * sizeof(float)); // largest possible buffer
    for (size_t i = 0; i < ctx->outputs.size(); ++i)
        writeBucket(ctx, static_cast<int>(i), 0, xres - 1, 0, yres - 1,
                    ctx->outputs[i].array_size, pixels);

    free(pixels);
    flushData(ctx);
}

/// Write end of image.
/// @note No update is possible after calling this function.
void writeEndOfImage(DriverData* ctx)
{
    int bucket_header[4];
    bucket_header[0] = -2; // image is finished
    bucket_header[1] = 0;
    bucket_header[2] = 0;
    bucket_header[3] = 0;

    // write plane index
    if (!writeData(bucket_header, sizeof(int), 4, ctx))
        AiMsgWarning("[mplay_driver] Unable to write end of image");
    else
        AiMsgDebug("[mplay_driver] Write end of image");

    flushData(ctx);
#ifdef _WIN32
    CloseHandle(ctx->write_pipe);
    CloseHandle(ctx->read_pipe);
    TerminateProcess(ctx->process_information.hProcess, 0);
    CloseHandle(ctx->process_information.hProcess);
    CloseHandle(ctx->process_information.hThread);
#else
    pclose(ctx->fp);
#endif
    ctx->fp = 0;
}

/// Driver parameters
node_parameters
{
    AiParameterPtr("swatch", NULL);
    AiMetaDataSetBool(nentry, "swatch", "maya.hide", true);
    AiMetaDataSetStr(nentry, NULL, "maya.translator", "mplay");
    AiMetaDataSetBool(nentry, NULL, "single_layer_driver", false);
    AiMetaDataSetBool(nentry, NULL, "display_driver", true);   
}

/// Instance initialization
node_initialize
{
    AiMsgDebug("[mplay_driver] node_initialize");
    DriverData* ctx = new DriverData;
    ctx->fp = 0;
    AiNodeSetLocalData(node, ctx);
    AiDriverInitialize(node, true);
}

node_update
{
    AiMsgDebug("[mplay_driver] node_update");
}

/// Query driver pixel-type capability
driver_supports_pixel_type
{
    switch (pixel_type)
    {
    case AI_TYPE_RGB:
    case AI_TYPE_RGBA:
    case AI_TYPE_VECTOR:
    case AI_TYPE_FLOAT:
        return true;

    default:
        return false;
    }
}

/// Register filename extensions associated with this driver
driver_extension
{
    return NULL;
}

/// Called before rendering starts
driver_open
{
    AiMsgDebug("[mplay_driver] driver_open");

    DriverData* ctx = reinterpret_cast<DriverData*>(AiNodeGetLocalData(node));

    if (ctx->fp)
        return;

    openPipeCommand(ctx);

    if (ctx->fp == 0)
        return;

    // gather output definitions
    const char *output_name;
    int pixel_type;

    while (AiOutputIteratorGetNext(iterator, &output_name, &pixel_type, NULL))
    {
        Output output_def;
        output_def.name = output_name;
        output_def.array_size = arraySize(pixel_type);
        ctx->outputs.push_back(output_def);
    }

    writeImageHeader(ctx);
}

/// This function is called to determine if a bucket will be rendered
driver_needs_bucket
{
    return true;
}

/// Called before a bucket is rendered
///
/// @note the renderer locks around this function
driver_prepare_bucket
{
    // ensure the bucket is big enough to contain the dog ears
    if (bucket_size_x < 6 || bucket_size_y < 6)
        return;

    DriverData* ctx = reinterpret_cast<DriverData*>(AiNodeGetLocalData(node));

    if (ctx->fp == 0)
        return;

    float white[16];
    for (int i = 0; i < 16; ++i)
        white[i] = 1;

    int x0 = bucket_xo + 1;
    int x1 = bucket_xo + bucket_size_x - 2;
    int y0 = bucket_yo + 1;
    int y1 = bucket_yo + bucket_size_y - 2;

    for (int i = 0; i < static_cast<int>(ctx->outputs.size()); ++i)
    {
        writeBucket(ctx, i, x0, x0 + 3, y0, y0, ctx->outputs[i].array_size, white);
        writeBucket(ctx, i, x0, x0, y0 + 1, y0 + 3, ctx->outputs[i].array_size, white);
        writeBucket(ctx, i, x1 - 3, x1, y0, y0, ctx->outputs[i].array_size, white);
        writeBucket(ctx, i, x1, x1, y0 + 1, y0 + 3, ctx->outputs[i].array_size, white);
        writeBucket(ctx, i, x1 - 3, x1, y1, y1, ctx->outputs[i].array_size, white);
        writeBucket(ctx, i, x1, x1, y1 - 3, y1 - 1, ctx->outputs[i].array_size, white);
        writeBucket(ctx, i, x0, x0 + 3, y1, y1, ctx->outputs[i].array_size, white);
        writeBucket(ctx, i, x0, x0, y1 - 3, y1 - 1, ctx->outputs[i].array_size, white);
    }
}

/// Called after a bucket has been rendered, but before it is written out.
driver_process_bucket
{
    // empty
}

/// Called after a bucket has been rendered
///
/// @note the renderer locks around this function
driver_write_bucket
{
    DriverData* ctx = reinterpret_cast<DriverData*>(AiNodeGetLocalData(node));

    if (ctx->fp == 0)
        return;

    int pixel_type;
    const void* pixel_data;
    int output_index = 0;

    while (AiOutputIteratorGetNext(iterator, NULL, &pixel_type, &pixel_data))
    {
        writeBucket(ctx, output_index++,
                    bucket_xo, bucket_xo + bucket_size_x - 1,
                    bucket_yo, bucket_yo + bucket_size_y - 1,
                    arraySize(pixel_type), pixel_data);
    }
}

// Called after rendering is finished
driver_close
{
    AiMsgDebug("[mplay_driver] driver_close");
}

// Called when driver is destroyed
node_finish
{
    AiMsgDebug("[mplay_driver] node_finish");
    DriverData* ctx = reinterpret_cast<DriverData*>(AiNodeGetLocalData(node));
    if (ctx->fp)
        writeEndOfImage(ctx);
    delete ctx;
}
