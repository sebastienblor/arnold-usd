#include "TxTextureFile.h"
#include <ai_texture.h>

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
   unsigned res_x, res_y;
   if(!AiTextureGetResolution(pathname.asChar(), &res_x, &res_y))
      return MS::kFailure;
   
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
   
   
   if(!AiEntireTextureAccess(AtString(fPathName.asChar()), fWidth, fHeight, &data[0]))
      return MS::kFailure;

   for (int y = 0; y < fHeight; ++y)
   {
      for (int x = 0; x < fWidth; ++x)
      {
         *dst++ = (unsigned char)CLAMP(data[y*fWidth+x].r*255.0f+0.5f, 0.0f, 255.0f);
         *dst++ = (unsigned char)CLAMP(data[y*fWidth+x].g*255.0f+0.5f, 0.0f, 255.0f);
         *dst++ = (unsigned char)CLAMP(data[y*fWidth+x].b*255.0f+0.5f, 0.0f, 255.0f);
         *dst++ = (unsigned char)CLAMP(data[y*fWidth+x].a*255.0f+0.5f, 0.0f, 255.0f);
      }
   }
   
   delete [] data;
   
   return MS::kSuccess;
}
