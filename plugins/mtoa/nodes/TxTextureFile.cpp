#include "TxTextureFile.h"
#include <ai_texture.h>

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
   unsigned res_x, res_y;
   AiTextureGetResolution(pathname.asChar(), &res_x, &res_y);
   
   fWidth = res_x;
   fHeight = res_y;
   fPathName = pathname;
   if(info)
   {
      info->width( res_x);
      info->height( res_y);
      info->channels( 4);
      info->pixelType( MImage::kByte);
   }
   return MS::kSuccess;
}

MStatus CTxTextureFile::load( MImage& image, unsigned int idx)
{
   AtRGBA *data = new AtRGBA[fHeight*fWidth];
   
   image.create( fWidth, fHeight, 4, MImage::kByte);

   unsigned char* dst = image.pixels();
   
   
   AiEntireTextureAccess(AtString(fPathName.asChar()), fWidth, fHeight, &data[0]);
   for (int y = 0; y < fHeight; ++y)
   {
      for (unsigned x = 0; x < fWidth; ++x)
      {
         *dst++ = (char)(data[y*fWidth+x].r*255);
         *dst++ = (char)(data[y*fWidth+x].g*255);
         *dst++ = (char)(data[y*fWidth+x].b*255);
         *dst++ = (char)(data[y*fWidth+x].a*255);
      }
   }
   
   delete [] data;
   
   return MS::kSuccess;
}
