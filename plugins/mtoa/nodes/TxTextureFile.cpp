#include "TxTextureFile.h"

const char* CTxTextureFile::fileName = "TxtextureFile";

CTxTextureFile::CTxTextureFile()
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
   if(info)
   {
      info->width( 512);
      info->height( 512);
      info->channels( 4);
      info->pixelType( MImage::kByte);
   }
   return MS::kSuccess;
}

MStatus CTxTextureFile::load( MImage& image, unsigned int idx)
{
   image.create( 512, 512, 4, MImage::kByte);

   unsigned char* dst = image.pixels();   

   // Create an opaque gray texture
   for (int y=0; y < 512*512; ++y)
   {
      *dst++ = 128;
      *dst++ = 128;
      *dst++ = 128;
      *dst++ = 255;
   }
   return MS::kSuccess;
}
