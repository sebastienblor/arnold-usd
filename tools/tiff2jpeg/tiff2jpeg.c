/*
 * This program reads a tiff image, processes it and outputs to a jpeg image.
 */

#include <stdio.h>
#include <stdlib.h>
#include "tiffio.h"
#include "jpeglib.h"

unsigned char **cpicalloc(int row, int col) 
{
   int i;
   unsigned char **p;
   
   p = (unsigned char **) calloc(row,sizeof(unsigned char *));
   if(!p)
   {
      fprintf(stderr,"memory allocation error\n");
      exit(-1);
   }
   
   for(i=0;i<row;i++)
   {
      p[i] = (unsigned char *) calloc(col,sizeof(unsigned char));
      if(!p[i])
      {
         fprintf(stderr,"memory allocation error\n");
         exit(-1);
      }
   }
   return p;
}

int main(int argc, char **argv)
{
   TIFF *tiffin;
   FILE *jpegout;
   char filename[1024];
   unsigned char **pic;
   unsigned int i, j, width, height, row_stride;
   struct jpeg_compress_struct cinfo;
   struct jpeg_error_mgr jerr;
   JSAMPROW row_pointer[1];
   
   if (argc != 3) 
   {
      fprintf(stderr, "usage: tiff2jpeg <filein> <fileout>\n");
      exit(-1);
   }
   
   sprintf(filename, "%s", argv[1]);
   if((tiffin = TIFFOpen(filename,"r")) == NULL)
   {
      fprintf(stderr, "%s: file open error\n", filename);
      exit(-1);
   }
   
   if(tiffin)
   {
      uint32 w, h;
      // size_t npixels;
      tsize_t npixels;
      uint32* raster;
      TIFFGetField(tiffin, TIFFTAG_IMAGEWIDTH, &w);
      TIFFGetField(tiffin, TIFFTAG_IMAGELENGTH, &h);
      npixels = w*h;
      raster = (uint32*) _TIFFmalloc(npixels*sizeof(uint32));
      width = w;
      height = h;
      pic = cpicalloc(height, width*3);
      
      if(raster != NULL)
      {
         if(TIFFReadRGBAImage(tiffin, w, h, raster, 0))
         {
            for(i=0; i<h; i++)
            {
               for(j=0; j<w; j++)
               {
                  pic[(h-1)-i][j*3+0] = TIFFGetR(raster[i*w+j]);
                  pic[(h-1)-i][j*3+1] = TIFFGetG(raster[i*w+j]);
                  pic[(h-1)-i][j*3+2] = TIFFGetB(raster[i*w+j]);
               }
            }
         }
      }
   }
   TIFFClose(tiffin);
   
   /*
    * Simple Processing (negate the picture) 
    *
    for(i=0; i<height; i++){
       for(j=0; j<width*3; j++){
          pic[i][j] = -pic[i][j];
       }
    }
    */
   
   sprintf(filename, "%s", argv[2]);
   if((jpegout = fopen(filename, "wb")) == NULL)
   {
      fprintf(stderr, "cannot open %s\n", filename);
      exit(-1);
   }
   
   cinfo.err = jpeg_std_error(&jerr);
   jpeg_create_compress(&cinfo);
   jpeg_stdio_dest(&cinfo, jpegout);
   
   cinfo.image_width = width;
   cinfo.image_height = height;
   cinfo.input_components = 3;
   cinfo.in_color_space = JCS_RGB;
   jpeg_set_defaults(&cinfo);
   
   jpeg_set_quality(&cinfo, 100, TRUE);
   jpeg_start_compress(&cinfo, TRUE);
   row_stride = width * 3;
   
   while(cinfo.next_scanline < cinfo.image_height)
   {
      row_pointer[0] = &pic[cinfo.next_scanline][0];
      (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
   }
   
   jpeg_finish_compress(&cinfo);
   fclose(jpegout);
   jpeg_destroy_compress(&cinfo);
   
   return 0;
}

