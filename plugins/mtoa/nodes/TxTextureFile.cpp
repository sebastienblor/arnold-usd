#include "TxTextureFile.h"
#include <ai_texture.h>
#include <ai.h>
#include <stdio.h>

const char* CTxTextureFile::fileName = "TxtextureFile";

CTxTextureFile::CTxTextureFile():
   fWidth(0),
   fHeight(0),
   fChannels(4),
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
   if(info)
   {
      info->width( res_x);
      info->height( res_y);
      info->channels( channels);
      info->pixelType( MImage::kFloat);
   }

   return MS::kSuccess;
}

MStatus CTxTextureFile::load( MImage& image, unsigned int idx)
{
   image.create( fWidth, fHeight, fChannels, MImage::kFloat);

   float* dst = image.floatPixels();
   
   if(!AiTextureLoad(AtString(fPathName.asChar()), true, dst))
      return MS::kFailure;

   return MS::kSuccess;
}
