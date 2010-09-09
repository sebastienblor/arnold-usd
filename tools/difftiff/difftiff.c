/*
 * difftiff
 *
 * Computes the (absolute and relative) difference of two TIFF image files
 *
 * Future work:
 *    - support for 16-bit images
 *    x support for 3-channel images (done!)
 *    - support for single-channel images (grayscale)
 *    - display number of different pixels?
 *    - flag to ignore alpha channel?
 *    - other difference metrics?
 *
 * Marcos Fajardo
 * 23/03/2005
 */



#include <tiffio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/******************************************************************************
   Data types
******************************************************************************/

typedef struct PixelStruct {
   short int r, g, b, a;
} Pixel;

typedef unsigned char Byte;



/******************************************************************************
   Useful macros
******************************************************************************/

#define ABS(_a) ( ((_a)<0) ? -(_a) : (_a) )
#define MAX(_a, _b) (((_a)>(_b))?(_a):(_b))
#define MAX3(_a, _b, _c) ( MAX(MAX(_a,_b),_c) )
#define MAX4(_a, _b, _c, _d) ( MAX(MAX3(_a,_b,_c),_d) )
#define PERCTO8BIT(t) (int)(t*2.55f)
#define DEFAULT_MAX_THR 25.0f
#define DEFAULT_AVG_THR 0.1f
#ifndef TRUE
#define TRUE 1
#define FALSE !TRUE
#endif



/******************************************************************************
   Global vars
******************************************************************************/

int width, height, nchannels, bpc;



/******************************************************************************
   Local functions
******************************************************************************/

void TIFFMyErrorHandler(const char *module, const char *fmt, va_list ap)
{
   char tifferror[1024];

   if (module)
		sprintf(tifferror, "%s: ", module);
	vsprintf(tifferror+strlen(tifferror), fmt, ap);
   printf("%s\n", tifferror);
}

int TIFFGetWidth(TIFF *file)
{
   int w;

   TIFFGetField(file, TIFFTAG_IMAGEWIDTH, &w);
   return w;
}

int TIFFGetHeight(TIFF *file)
{
   int h;

   TIFFGetField(file, TIFFTAG_IMAGELENGTH, &h);
   return h;
}

int TIFFGetNumChannels(TIFF *file)
{
   short int nc;

   TIFFGetField(file, TIFFTAG_SAMPLESPERPIXEL, &nc);
   return nc;
}

int TIFFGetBitsPerChannel(TIFF *file)
{
   short int b;

   TIFFGetField(file, TIFFTAG_BITSPERSAMPLE, &b);
   return b;
}



Pixel BufferGetPixel(Byte *buffer, int x, int y)
{
   Pixel pix;
   Byte *ptr;

   ptr = buffer + (y*width+x)*nchannels;
   pix.r = *(ptr++);
   pix.g = *(ptr++);
   pix.b = *(ptr++);
   if (nchannels == 4)
      pix.a = *(ptr  );
   else
      pix.a = 0;
   return pix;
}



/* return the maximum absolute pixel difference of all channels */
int PixelDiff(Pixel p1, Pixel p2)
{
   int diff, rdiff, gdiff, bdiff, adiff;

   rdiff = ABS(p1.r - p2.r);
   gdiff = ABS(p1.g - p2.g);
   bdiff = ABS(p1.b - p2.b);
   adiff = ABS(p1.a - p2.a);

   diff = MAX4(rdiff, gdiff, bdiff, adiff);

   return diff;
}



void GenerateDiffImage(Byte *buf1, Byte *buf2, char *filename, int compress, int forcesolid)
{
   TIFF *file;
   int x, y;
   tdata_t buf;
   Byte *pp;
   Pixel p1, p2;

   file = TIFFOpen(filename, "w");
   if (!file)
      return;

   if (compress)
   {
      TIFFSetField(file, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
      TIFFSetField(file, TIFFTAG_PREDICTOR, 2);
   }
   else
   {
      TIFFSetField(file, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
   }
   TIFFSetField(file, TIFFTAG_IMAGEWIDTH, width);
   TIFFSetField(file, TIFFTAG_IMAGELENGTH, height);
   TIFFSetField(file, TIFFTAG_BITSPERSAMPLE, bpc);
   TIFFSetField(file, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
   TIFFSetField(file, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
   if (nchannels > 3 || (nchannels == 3 && forcesolid))
   {
      uint16 v[1];
      v[0] = EXTRASAMPLE_UNASSALPHA;
      TIFFSetField(file, TIFFTAG_EXTRASAMPLES, 1, v);
      TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, nchannels+
                                                 (nchannels == 3 ? 1 : 0));
   }
   else
      TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, nchannels);
   TIFFSetField(file, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
   TIFFSetField(file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
   TIFFSetField(file, TIFFTAG_ROWSPERSTRIP, height);

   buf = _TIFFmalloc(TIFFScanlineSize(file));

   for (y=0; y<height; y++)
   {
      pp = (Byte *)buf;
      for (x=0; x<width; x++)
      {
         p1 = BufferGetPixel(buf1, x, y);
         p2 = BufferGetPixel(buf2, x, y);

         *(pp++) = ABS(p1.r - p2.r);
         *(pp++) = ABS(p1.g - p2.g);
         *(pp++) = ABS(p1.b - p2.b);
         *(pp++) = forcesolid ? 255 : ABS(p1.a - p2.a);
      }
      TIFFWriteScanline(file, buf, y, 0);
   }

   _TIFFfree(buf);
   TIFFClose((void*)(file));
}



void PrintHelp(void)
{
   printf("difftiff 1.1");
   printf("\nCopyright (c) 2005-2007 Marcos Fajardo");
   printf("\n\nusing %s", TIFFGetVersion());

   printf("\n\nusage: difftiff [options] file1 file2");
   printf("\nwhere options are:");
   printf("\n -m %%           threshold for maximum difference (default %.3g%%)", DEFAULT_MAX_THR);
   printf("\n -a %%           threshold for average difference (default %.3g%%)", DEFAULT_AVG_THR);
   printf("\n -f filename    save diff image to file");
   printf("\n -n             do not compress diff image");
   printf("\n -s             save diff image with solid alpha");

   printf("\n\nlimitations:");
   printf("\n + does not support 16-bit images");
   printf("\n + supports 4-channel images only (RGBA, but not RGB)\n");
}



/******************************************************************************
   Main 
******************************************************************************/

int main(int argc, char **argv)
{
   TIFF *file1, *file2;
   Byte *buf1, *buf2, *scanline;
   int param, compression, forcesolid, return_code, are_different;
   int scansize, pixels, x, y, diff, maxdiff, totaldiff;
   long size;
   float max_threshold, avg_threshold, avgdiff;
   char *diff_filename;
   Pixel pix1, pix2;

   /* default values */
   max_threshold = DEFAULT_MAX_THR;
   avg_threshold = DEFAULT_AVG_THR;
   diff_filename = NULL;
   compression = TRUE;
   forcesolid = FALSE;

   /* initialize some variables */
   scanline = NULL;
   file1 = NULL;
   file2 = NULL;
   buf1 = NULL;
   buf2 = NULL;
   are_different = FALSE;
   return_code = -1;

   /* need at least the two filenames */
   if (argc < 3)
   {
      PrintHelp();
      goto finish;
   }

   /* parse command line options */
   param = 1;
   while (param<argc)
   {
      if (strcmp(argv[param], "-m") == 0)
      {
         if (param+1 < argc)
            max_threshold = (float)atof(argv[++param]);
      }
      else if (strcmp(argv[param], "-a") == 0)
      {
         if (param+1 < argc)
            avg_threshold = (float)atof(argv[++param]);
      }
      else if (strcmp(argv[param], "-f") == 0)
      {
         if (param+1 < argc)
            diff_filename = argv[++param];
      }
      else if (strcmp(argv[param], "-n") == 0)
      {
         compression = FALSE;
      }
      else if (strcmp(argv[param], "-s") == 0)
      {
         forcesolid = TRUE;
      }
      else
      {
         if (argv[param][0] == '-')
         {
            PrintHelp();
            goto finish;
         }

         /* this argument is not recognized, so it must be the filenames next */
         break;
      }

      param++;
   }

   /* extra options are not allowed after the two filenames */
   if (param < argc-2)
   {
      PrintHelp();
      goto finish;
   }

   TIFFSetWarningHandler(TIFFMyErrorHandler);
   TIFFSetErrorHandler(TIFFMyErrorHandler);
   
   file1 = TIFFOpen(argv[param++], "ru");
   if (!file1) 
      goto finish;

   file2 = TIFFOpen(argv[param], "ru");
   if (!file2) 
      goto finish;

   width = TIFFGetWidth(file1);
   height = TIFFGetHeight(file1);
   bpc = TIFFGetBitsPerChannel(file1);
   nchannels = TIFFGetNumChannels(file1);

   if ( (width!=TIFFGetWidth(file2)) || (height!=TIFFGetHeight(file2)) || (bpc!=TIFFGetBitsPerChannel(file2)) )
   {
      printf("Both images must have the same resolution/depth");
      printf("\n%s: %dx%d, %d-bit, %d channels", argv[param-1], width, height, bpc, nchannels);
      printf("\n%s: %dx%d, %d-bit, %d channels\n", argv[param], 
         TIFFGetWidth(file2), TIFFGetHeight(file2), TIFFGetBitsPerChannel(file2), TIFFGetNumChannels(file2));      
      goto finish;
   }

   /* 16-bit TIFF images not supported yet */
   if (bpc != 8)
   {
      printf("Support for other than 8-bit is not implemented (you are trying %d-bit)\n", bpc);
      goto finish;
   }

   /* RGB images not supported yet (todo: RGB, grayscale, and maybe arbitrary channels) */
   if ( nchannels != TIFFGetNumChannels(file2) )
   {
      printf("Attempting to compare non-matching image types (you are comparing %d- and %d-channel images)\n",
         nchannels, TIFFGetNumChannels(file2));
      goto finish;
   }
   else if (  nchannels!=3 &&  nchannels!=4 )
   {
      printf("Support for other than RGB and RGBA channels is not implemented (you are trying %d:%d channels)\n", 
         nchannels, TIFFGetNumChannels(file2));
      goto finish;
   }
   if (nchannels == 3)
      forcesolid = TRUE;
   pixels = width * height;
   scansize = width * nchannels * (bpc/8);
   size = scansize * height;
   buf1 = (Byte *)malloc(size);
   buf2 = (Byte *)malloc(size);
   if (!buf1 || !buf2)
   {
      printf("Cannot allocate image buffers (%d bytes)\n", (int)size);
      goto finish;
   }

   scanline = (Byte *)_TIFFmalloc(scansize);
   if (!scanline)
   {
      printf("Cannot allocate scanline buffer (%d bytes)\n", scansize);
      goto finish;
   }
   _TIFFmemset(scanline, 0, scansize);
   
   /* load both images in memory */
   for (y=0; y<height; y++)
   {
      TIFFReadScanline(file1, buf1 + y * scansize, y, 0);
      TIFFReadScanline(file2, buf2 + y * scansize, y, 0);
   }

   /* compute {max,avg} pixel difference */
   maxdiff = 0;
   totaldiff = 0;
   for (y=0; y<height; y++)
   {
      for (x=0; x<width; x++)
      {
         pix1 = BufferGetPixel(buf1, x, y);
         pix2 = BufferGetPixel(buf2, x, y);
         diff = PixelDiff(pix1, pix2);
         
         if (diff > maxdiff)
            maxdiff = diff;

         totaldiff += diff;
      }
   }
   avgdiff = (float)totaldiff / pixels;

   /* is the pixel difference above user thresholds? */
   if (avgdiff > avg_threshold*2.55f)
      are_different = TRUE;
   if (maxdiff > PERCTO8BIT(max_threshold))
      are_different = TRUE;

   if (are_different)
   {
      /* images are different, so return failure */
      return_code = -1;
      printf("Max diff: %d (%.3f%%)", maxdiff, maxdiff/2.55f);
      printf("\nAvg diff: %.3f (%.3f%%)", avgdiff, avgdiff/2.55f);
      if (diff_filename)
      {
         printf("\nWriting difference image to %s\n", diff_filename);
         GenerateDiffImage(buf1, buf2, diff_filename, compression, forcesolid);
      }
      else
      {
         printf("\n");
      }
   }
   else
   {
      /* images are equal (or similar enough), return success */
      return_code = 0;
   }

   /* clean up */
finish:
   if (scanline) _TIFFfree((void *)scanline);
   if (file1) TIFFClose(file1);
   if (file2) TIFFClose(file2);
   if (buf1) free(buf1);
   if (buf2) free(buf2);

   return (return_code);
}


