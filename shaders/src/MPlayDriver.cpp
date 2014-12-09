// $Id$

#include <ai.h>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std; // TODO : REMOVE IT, THIS IS FUGLY!

AI_DRIVER_NODE_EXPORT_METHODS(MPlayDriverMtd);

/// Cross platform popen() and pclose()
#ifdef _WIN32
#define POPEN(cmd) _popen(cmd, "wb")
#define PCLOSE(fp) _pclose(fp)
#else
#define POPEN(cmd) popen(cmd, "w")
#define PCLOSE(fp) pclose(fp)
#endif // _WIN32

/// Array size. Only those 3 pixel size values are supported by Houdini.
enum ArraySize
{
    ARRAY_SIZE_SCALAR = 1,
    ARRAY_SIZE_RGB    = 3,
    ARRAY_SIZE_RGBA   = 4
};

/// Output (AOV) info
struct Output
{
    string name;          ///< AOV name
    ArraySize array_size; ///< AOV pixel size
};

/// Driver local data
struct DriverData
{
    vector<Output> outputs;
    FILE* fp;               ///< Pipe file descriptor
    unsigned int port;      ///< Socket port
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
    case AI_TYPE_POINT:
    default:
        return ARRAY_SIZE_RGB;
    }
}

/// Open pipe process
void openPipeCommand(DriverData* ctx)
{
    char* HB = getenv("HB");
    if (HB == NULL)
    {
        AiMsgWarning("[mplay_driver] This driver can only be used in the Houdini environment");
        ctx->fp = 0;
        return;
    }

    stringstream cmd;
#ifdef WIN32
    cmd << "\"" << HB << "/imdisplay\" -f -n Arnold -k -p";
#else
    cmd << "unset LD_LIBRARY_PATH;\"" << HB << "/imdisplay\" -f -n Arnold -k -p";
    std::cerr << cmd.str() << std::endl;
#endif
    if (ctx->port > 0)
        cmd << " -s " << ctx->port;

    AiMsgDebug("[mplay_driver] Launching pipe command: %s", cmd.str().c_str());

    ctx->fp = POPEN(cmd.str().c_str());

    if (ctx->fp)
        AiMsgDebug("[mplay_driver] Houdini pipe opened");
    else
        AiMsgWarning("[mplay_driver] Could not open Houdini pipe command");
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
    if (fwrite(bucket_header, sizeof(int), 4, ctx->fp) != 4)
        AiMsgWarning("[mplay_driver] Unable to write plane definition");

    // fill out the tile header
    bucket_header[0] = x0;
    bucket_header[1] = x1;
    bucket_header[2] = y0;
    bucket_header[3] = y1;
    size_t npixels = (x1 - x0 + 1) * (y1 - y0 + 1);

    // write header
    if (fwrite(bucket_header, sizeof(int), 4, ctx->fp) != 4)
        AiMsgWarning("[mplay_driver] Error writing bucket: %d %d %d %d\n", x0, x1, y0, y1);

    // write data
    if (fwrite(pixels, pixel_size * sizeof(float), npixels, ctx->fp) != npixels)
        AiMsgWarning("[mplay_driver] Error writing data: %d %d %d %d\n", x0, x1, y0, y1);

    fflush(ctx->fp);
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

    if (fwrite(header, sizeof(int), 8, ctx->fp) != 8)
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

        if (fwrite(plane_def, sizeof(int), 8, ctx->fp) != 8)
            AiMsgWarning("[mplay_driver] Unable to write plane definition");

        if (fwrite(ctx->outputs[i].name.c_str(), sizeof(char), name_length, ctx->fp) != name_length)
            AiMsgWarning("[mplay_driver] Unable to write plane name");
    }

    // blank planes
    float* pixels = (float*) calloc(xres * yres, ARRAY_SIZE_RGBA * sizeof(float)); // largest possible buffer
    for (size_t i = 0; i < ctx->outputs.size(); ++i)
        writeBucket(ctx, static_cast<int>(i), 0, xres - 1, 0, yres - 1,
                    ctx->outputs[i].array_size, pixels);

    free(pixels);
    fflush(ctx->fp);
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
    if (fwrite(bucket_header, sizeof(int), 4, ctx->fp) != 4)
        AiMsgWarning("[mplay_driver] Unable to write end of image");
    else
        AiMsgDebug("[mplay_driver] Write end of image");

    fflush(ctx->fp);
    PCLOSE(ctx->fp);
    ctx->fp = NULL;
}

/// Driver parameters
node_parameters
{
    AiParameterUINT("port", 0); // IPR socket port, a value of 0 means send to MPlay

    AiParameterPTR("swatch", NULL);
    AiMetaDataSetBool(mds, "swatch", "maya.hide", true);
    AiMetaDataSetStr(mds, NULL, "maya.translator", "mplay");
    AiMetaDataSetBool(mds, NULL, "single_layer_driver", false);
    AiMetaDataSetBool(mds, NULL, "display_driver", true);   
}

/// Instance initialization
node_initialize
{
    AiMsgDebug("[mplay_driver] node_initialize");
    DriverData* ctx = new DriverData;
    ctx->fp = NULL;
    AiDriverInitialize(node, true, ctx);
}

node_update
{
    AiMsgDebug("[mplay_driver] node_update");
    DriverData* ctx = reinterpret_cast<DriverData*>(AiDriverGetLocalData(node));
    ctx->port = AiNodeGetUInt(node, "port");
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
    case AI_TYPE_POINT:
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

    DriverData* ctx = reinterpret_cast<DriverData*>(AiDriverGetLocalData(node));

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

    DriverData* ctx = reinterpret_cast<DriverData*>(AiDriverGetLocalData(node));

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
    DriverData* ctx = reinterpret_cast<DriverData*>(AiDriverGetLocalData(node));

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

/// Called after rendering is finished
driver_close
{
    AiMsgDebug("[mplay_driver] driver_close");
}

/// Called when driver is destroyed
node_finish
{
    AiMsgDebug("[mplay_driver] node_finish");
    DriverData* ctx = reinterpret_cast<DriverData*>(AiDriverGetLocalData(node));
    if (ctx->fp)
        writeEndOfImage(ctx);
    delete ctx;
    AiDriverDestroy(node);
}
