#pragma once

#include <maya/MPxImageFile.h>
#include <maya/MImageFileInfo.h>
#include <maya/MImage.h>

class CTxTextureFile : public MPxImageFile
{
public:
   CTxTextureFile();
   virtual ~CTxTextureFile();
   static void* creator();
   virtual MStatus open( MString pathname, MImageFileInfo* info);
   virtual MStatus load( MImage& image, unsigned int idx);
   
   static const char* fileName;
   
   void populateTestImage(unsigned char* pixels, unsigned int w, unsigned int h);

protected:
   int fWidth;
   int fHeight;
   int fChannels;
   MString fPathName;
   
}; // class CTxTextureFile
