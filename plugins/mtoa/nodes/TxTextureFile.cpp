#include "TxTextureFile.h"
#include <ai_texture.h>
#include <ai.h>
#include <stdio.h>
#include <math.h>

#include <maya/MGlobal.h>

const char* CTxTextureFile::fileName = "TxtextureFile";

CTxTextureFile::CTxTextureFile():
   fWidth(0),
   fHeight(0),
   fChannels(4),
   fMiplevel(0),
   fPathName("")
{
}

CTxTextureFile::~CTxTextureFile()
{
}

void* CTxTextureFile::creator()
{
   return new CTxTextureFile();
}

MStatus CTxTextureFile::open( MString pathname, MImageFileInfo* info)
{
   unsigned int res_x, res_y, channels;
   if(!AiTextureGetResolution(pathname.asChar(), &res_x, &res_y))
      return MS::kFailure;
   
   if(!AiTextureGetNumChannels(pathname.asChar(), &channels))
      return MS::kFailure;

   fWidth = res_x;
   fHeight = res_y;
   fChannels = channels;
   fPathName = pathname;
   
   int targetRes = 1024;
   
   int clampTexture = 0;
   int exists = 0;
   MGlobal::executeCommand("objExists hardwareRenderingGlobals.enableTextureMaxRes", exists);
   if (exists == 1)
   {
      MGlobal::executeCommand("getAttr \"hardwareRenderingGlobals.enableTextureMaxRes\"", clampTexture);
   }
      
   if (clampTexture == 1)
   {
      MGlobal::executeCommand("objExists hardwareRenderingGlobals.textureMaxResolution", exists);
      if (exists == 1)
      {
         MGlobal::executeCommand("getAttr \"hardwareRenderingGlobals.textureMaxResolution\"", targetRes);
      }
   }      
   
   
   if (res_x > targetRes)
   {
      fMiplevel = log(res_x / targetRes) / log(2);
      fWidth = res_x >> fMiplevel;
      fHeight = res_y >> fMiplevel;
   }
   
   if(info)
   {
      info->width( fWidth);
      info->height( fHeight);
      info->channels( fChannels);
      info->pixelType( MImage::kFloat);
   }
   
   return MS::kSuccess;
}

MStatus CTxTextureFile::load( MImage& image, unsigned int idx)
{
   image.create( fWidth, fHeight, fChannels, MImage::kFloat);

   float* dst = image.floatPixels();
   
   if(!AiTextureLoad(AtString(fPathName.asChar()), true, fMiplevel, dst))
      return MS::kFailure;
      
   return MS::kSuccess;
}
