//
// SynColor implementation for the color_manager interface
//

#include <maya/MTypes.h>
#include <ai.h>

#include <synColor/config.h>
#include <synColor/synColorInit.h>
#include <synColor/synColorPrefs.h>
#include <synColor/synColor.h>
#include <synColor/template.h>
#include <synColor/synColorFactory.h>

#ifdef _LINUX
#include <tr1/unordered_map>
typedef std::tr1::unordered_map<size_t, SYNCOLOR::TransformPtr> ProcessorMap;
#else
#include <unordered_map>
typedef std::unordered_map<size_t, SYNCOLOR::TransformPtr> ProcessorMap;
#endif
#include <string>
#include <fstream>


size_t computeKey(size_t csHash, unsigned int sFormat, unsigned int dFormat, unsigned int dir)
{
   size_t data[4];
   data[0] = csHash;
   data[1] = sFormat;
   data[2] = dFormat;
   data[3] = dir;

   size_t size = sizeof(data);
   unsigned char *input = (unsigned char*)data;
   size_t hash = 5381;
   int c;
   while (size--)
   {
      c = *input++;
      hash = ((hash << 5) + hash) + c; 
   }
   return hash;
}


AI_COLOR_MANAGER_NODE_EXPORT_METHODS(synColor_color_manager_Methods);


// All the strings used by the ass file
namespace DataStr
{ 
   const AtString native_catalog_path("native_catalog_path");
   const AtString custom_catalog_path("custom_catalog_path");
   const AtString ocioconfig_path("ocioconfig_path");
   const AtString rendering_color_space("rendering_color_space");
   const AtString output_color_conversion("output_color_conversion");
};

// The type used for the color transformation cache


// Helper class for critical section gaurd
class ThreadGuard
{
public:
   ThreadGuard(AtCritSec& section)
      : m_section(section)
   {
      AiCritSecEnter(&m_section);
   }
   ~ThreadGuard()
   {
      AiCritSecLeave(&m_section);
   }

private:
   AtCritSec& m_section;

private:
   ThreadGuard(const ThreadGuard&);
   ThreadGuard& operator=(ThreadGuard const&);
};

// this will be retrieved through Get and SetLocalData
// useful for storing precomputed or cached values to be used
// during render time
class ColorManagerData
{
public:
   ColorManagerData()
   {
      m_initialization_done = false;

      AiCritSecInit(&m_output_guard);
      AiCritSecInit(&m_input_guard);
   }

   ~ColorManagerData()
   {
      AiCritSecClose(&m_output_guard);
      AiCritSecClose(&m_input_guard);
   }

   // Enforce to initialize the template only once and only when needed
   bool m_initialization_done;

   // Enforce to initialize the synColor library only once and only when needed
   static bool m_initialization_library_done;

   // The synColor current configuration
   SYNCOLOR::Config::Ptr m_config;

   // The way to correctly initialize the synColor engine
   AtString m_native_catalog_path;
   AtString m_custom_catalog_path;

   AtString m_ocioconfig_path;        // The ocio config file to use if ocio mode enabled
   AtString m_rendering_color_space;  // The rendering color space
   
   // Keep a cache of all output transforms to only compute them once
   SYNCOLOR::TemplatePtr m_output_template;
   AtCritSec             m_output_guard;
   ProcessorMap          m_output_transforms;

   // Keep a cache of all input transforms to only compute them once
   SYNCOLOR::TemplatePtr m_input_template;
   AtCritSec             m_input_guard;
   ProcessorMap          m_input_transforms;
};

bool ColorManagerData::m_initialization_library_done = false;

namespace
{
   const SYNCOLOR::OptimizerFlags optimizerFlags(SYNCOLOR::OPTIMIZER_LOSSLESS);
   const SYNCOLOR::ResolveFlags resolveFlag(SYNCOLOR::RESOLVE_GRAPHICS_MONITOR);

   // Compute the pixel format from the channel layout 
   SYNCOLOR::PixelFormat computePixelFormat(const AtChannelLayout& layout)
   {
      SYNCOLOR::PixelFormat pxlFormat(SYNCOLOR::PF_UNKNOWN);

      switch(layout.channel_type)
      {
         case AI_TYPE_FLOAT:
         {
            switch(layout.type)
            {
               case AI_TYPE_RGB:
               {
                  pxlFormat = SYNCOLOR::PF_RGB_32f;
                  break;
               }
               case AI_TYPE_RGBA:
               {
                  pxlFormat = SYNCOLOR::PF_RGBA_32f;
                  break;
               }
            }
            break;
         }
         case AI_TYPE_USHORT:
         {
            switch(layout.type)
            {
               case AI_TYPE_RGB:
               {
                  pxlFormat = SYNCOLOR::PF_RGB_16i;
                  break;
               }
               case AI_TYPE_RGBA:
               {
                  pxlFormat = SYNCOLOR::PF_RGBA_16i;
                  break;
               }
            }
            break;
         }
         case AI_TYPE_BYTE:
         {
            switch(layout.type)
            {
               case AI_TYPE_RGB:
               {
                  pxlFormat = SYNCOLOR::PF_RGB_8i;
                  break;
               }
               case AI_TYPE_RGBA:
               {
                  pxlFormat = SYNCOLOR::PF_RGBA_8i;
                  break;
               }
            }
            break;
         }
      }

      return pxlFormat;
   }

   SYNCOLOR::PixelFormat all_pixel_formats[] = {
      SYNCOLOR::PF_RGBA_32f,
      SYNCOLOR::PF_RGB_32f,
      SYNCOLOR::PF_RGBA_16i,
      SYNCOLOR::PF_RGB_16i,
      SYNCOLOR::PF_RGBA_8i,
      SYNCOLOR::PF_RGB_8i
   };
/*
   const char* pixelFormatStr(const SYNCOLOR::PixelFormat& pixel_format)
   {
      switch(pixel_format)
      {
         case SYNCOLOR::PF_RGBA_32f:return "RGBA(32f)";
         case SYNCOLOR::PF_RGB_32f: return "RGB(32f)";
         case SYNCOLOR::PF_RGBA_16i:return "RGBA(16i)";
         case SYNCOLOR::PF_RGB_16i: return "RGB(16i)";
         case SYNCOLOR::PF_RGBA_8i: return "RGBA(8i)";
         case SYNCOLOR::PF_RGB_8i:  return "RGB(8i)";
         default: break;
      }
      return "";
   }
*/
   
/*   void logger(SYNCOLOR::LogLevel, const char*)
   {
   }
*/
   // The method initializes the SynColor library only once.
   void initializeSynColor(ColorManagerData* colorData)
   {
      SYNCOLOR::SynStatus status;

      const char* envVariableValue = getenv("SYNCOLOR");
      const bool useEnvVariable = envVariableValue!=0x0;

      if(!ColorManagerData::m_initialization_library_done)
      {
         std::string filename;

         if(colorData->m_native_catalog_path.empty())
         {
            ColorManagerData::m_initialization_library_done = false;
            colorData->m_initialization_done = false;

            AiMsgError("[color_manager_syncolor] Initialization failed: native_catalog_path is missing");

            return;
         }

         const AtString nativePath(colorData->m_native_catalog_path);

         // Provide a dummy path to avoid error on empty path: synColor 2017 limitation.
         const AtString sharedPath(
            colorData->m_custom_catalog_path.empty() ? "/tmp/shared" : colorData->m_custom_catalog_path);

         if(useEnvVariable)
         {
            status 
               = SYNCOLOR::Config::get(
                  envVariableValue, 
                  colorData->m_ocioconfig_path.c_str(), 
                  colorData->m_config);

            filename = envVariableValue;
         }
         else
         {
            char tmpFilename[L_tmpnam];
            tmpnam(tmpFilename);

            status 
               = SYNCOLOR::Config::get(
                  nativePath.c_str(),
                  sharedPath.c_str(),
                  colorData->m_ocioconfig_path.c_str(), 
                  colorData->m_config);

            filename = tmpFilename;
         }

         if(!status)
         {
            // Try to survive to unexpected issue by creating the preferences from scratch.
            // It should never happen within Maya; however it could happen if used 
            // with kick (a tool without its own synColor catalog installation).
            status 
               = SYNCOLOR::Config::create(
                  filename.c_str(),
                  nativePath.c_str(),
                  sharedPath.c_str(),
                  colorData->m_ocioconfig_path.c_str(), 
                  colorData->m_config);
         }

         if(!useEnvVariable)
         {
            std::remove(filename.c_str());
         }

         if(status)
         {
            AiMsgInfo("[color_manager_syncolor] synColor version %s", SYNCOLOR::getVersionString());
            if(useEnvVariable)
            {
               AiMsgInfo("[color_manager_syncolor] with SYNCOLOR env. variable: %s", envVariableValue);
            }
            else
            {
               AiMsgInfo("[color_manager_syncolor] native catolog directory: %s", nativePath.c_str());
            }

            if(!colorData->m_ocioconfig_path.empty())
            {
               AiMsgInfo("[color_manager_syncolor] OCIO config file: %s", colorData->m_ocioconfig_path.c_str());
            }

            if(!colorData->m_custom_catalog_path.empty())
            {
               const char* pSharedDirectory = 0x0;
               SYNCOLOR::getSharedColorTransformPath(pSharedDirectory);                  
               AiMsgInfo("[color_manager_syncolor] optional custom catalog directory: %s", 
                  pSharedDirectory ? pSharedDirectory : "");
            }
         }

         ColorManagerData::m_initialization_library_done = (bool)status;
         colorData->m_initialization_done = false;
      }

      if(!colorData->m_initialization_done && ColorManagerData::m_initialization_library_done)
      {

         if(status)
         {
            colorData->m_config->getTemplate(SYNCOLOR::InputTemplate, colorData->m_input_template);
            if(status)
            {
               status = colorData->m_config->getTemplate(SYNCOLOR::ViewingTemplate, colorData->m_output_template);
            }

         }

         colorData->m_initialization_done = (bool)status;
      }

      if(!status)
      {
         if(useEnvVariable)
         {
            AiMsgError("[color_manager_syncolor] Initialization failed: %s from the preference file %s", 
               status.getErrorMessage(), envVariableValue);
         }
         else
         {
            AiMsgError("[color_manager_syncolor] Initialization failed: %s", status.getErrorMessage());
         }
      }

      if(colorData->m_initialization_done && ColorManagerData::m_initialization_library_done)
      {
         SYNCOLOR::TemplateParameterPtr in_param;
         status 
            = status ? colorData->m_input_template->getParameter(SYNCOLOR::InputSpace, in_param)
                     : status;
         if(status)
         {
            const unsigned mum_input_color_spaces = in_param->getNumValues();
            for(unsigned idx=0; idx<mum_input_color_spaces && status; ++idx)
            {
               const AtString cs(in_param->getValue(idx));
               static const unsigned max_pixel_formats
                  = sizeof(all_pixel_formats) / sizeof(SYNCOLOR::PixelFormat);

               for(unsigned i=0; i<max_pixel_formats && status; ++i)
               {
                  for(unsigned j=0; j<max_pixel_formats && status; ++j)
                  {
                     {
                        const size_t key = computeKey(
                           cs.hash(), 
                           (unsigned int)all_pixel_formats[i], 
                           (unsigned int)all_pixel_formats[j],
                           (unsigned int)SYNCOLOR::TransformForward);

                        colorData->m_input_transforms[key] = SYNCOLOR::TransformPtr();
                     }
                     {
                        const size_t key = computeKey(
                           cs.hash(), 
                           (unsigned int)all_pixel_formats[i], 
                           (unsigned int)all_pixel_formats[j],
                           (unsigned int)SYNCOLOR::TransformReverse);

                        colorData->m_input_transforms[key] = SYNCOLOR::TransformPtr();
                     }
                  }
               }
            }
         }

         SYNCOLOR::TemplateParameterPtr vt_param;
         status 
            = status ? colorData->m_output_template->getParameter(SYNCOLOR::ViewTransform, vt_param)
                     : status;
         if(status)
         {
            const unsigned mum_view_transforms = vt_param->getNumValues();
            for(unsigned idx=0; idx<mum_view_transforms && status; ++idx)
            {
               const AtString cs(vt_param->getValue(idx));
               static const unsigned max_pixel_formats
                  = sizeof(all_pixel_formats) / sizeof(SYNCOLOR::PixelFormat);

               for(unsigned i=0; i<max_pixel_formats && status; ++i)
               {
                  for(unsigned j=0; j<max_pixel_formats && status; ++j)
                  {
                     {
                        const size_t key = computeKey(
                           cs.hash(), 
                           (unsigned int)all_pixel_formats[i], 
                           (unsigned int)all_pixel_formats[j],
                           (unsigned int)SYNCOLOR::TransformForward);

                        colorData->m_output_transforms[key] = SYNCOLOR::TransformPtr();
                     }
                     {
                        const size_t key = computeKey(
                           cs.hash(), 
                           (unsigned int)all_pixel_formats[i], 
                           (unsigned int)all_pixel_formats[j],
                           (unsigned int)SYNCOLOR::TransformReverse);

                        colorData->m_output_transforms[key] = SYNCOLOR::TransformPtr();
                     }
                  }
               }
            }
         }

         colorData->m_initialization_done = (bool)status;
      }
   }

   SYNCOLOR::SynStatus computeInputColorTransformation(ColorManagerData* colorData,
                                                       const AtString& color_space, 
                                                       const SYNCOLOR::PixelFormat& src_pixel_format, 
                                                       const SYNCOLOR::PixelFormat& dst_pixel_format,
                                                       const SYNCOLOR::TransformDirection& direction,
                                                       SYNCOLOR::TransformPtr& transform)
   {
      SYNCOLOR::SynStatus status;

      SYNCOLOR::TemplateParameterPtr in_param;
      status = colorData->m_input_template->getParameter(SYNCOLOR::InputSpace, in_param);
      if(status)
      {
         status = in_param->select(color_space.c_str());
         if(status)
         {
            SYNCOLOR::TemplateParameterPtr ws_param;
            status = colorData->m_input_template->getParameter(SYNCOLOR::WorkingSpace, ws_param);
            if(status)
            {
               status = ws_param->select(colorData->m_rendering_color_space);
               if(status)
               {
                  status = colorData->m_input_template->createTransform(transform, direction);
               }
            }
         }
      }

      return status;
   }

   // The method finalizes the input color transformation which is the conversion from
   // the input color space to the rendering color space.
   SYNCOLOR::SynStatus finalizeInputColorTransformation(ColorManagerData* colorData,
                                                        const AtString& color_space, 
                                                        const SYNCOLOR::PixelFormat& src_pixel_format, 
                                                        const SYNCOLOR::PixelFormat& dst_pixel_format,
                                                        const SYNCOLOR::TransformDirection& direction,
                                                        SYNCOLOR::TransformPtr& transform)
   {
      const size_t key = computeKey(
         color_space.hash(), 
         (unsigned int)src_pixel_format, 
         (unsigned int)dst_pixel_format,
         (unsigned int)direction);

      SYNCOLOR::SynStatus status;

      ProcessorMap::const_iterator it1 = colorData->m_input_transforms.find(key);
      if(it1==colorData->m_output_transforms.end())
      {
         // Use a wrong error id because synColor does not have a 
         // generic error id for now.
         status.setErrorCode(SYNCOLOR::ERROR_RENDERER_GPU_ADAPTOR_ERROR);
         std::string err("The color space is unknown: ");
         err += color_space.c_str();
         status.setErrorMessage(err.c_str());
         return status;
      }

      if(it1->second.get()!=0x0)
      {
         transform = it1->second;
      }
      else
      {
         ThreadGuard guard(colorData->m_input_guard);

         ProcessorMap::iterator it2 = colorData->m_input_transforms.find(key);
         if(it2->second.get()!=0x0)
         {
            transform = it2->second;
         }
         else
         {
            status = computeInputColorTransformation(
               colorData, color_space, src_pixel_format, dst_pixel_format, direction, transform);
            if(status)
            {
               status = SYNCOLOR::finalize(
                  transform, src_pixel_format, dst_pixel_format, optimizerFlags, resolveFlag, transform);
               if(status)
               {
                  it2->second = transform;
/*
                  AiMsgInfo("[color_manager_syncolor] Computation of %s input color transformation from '%s' to '%s'"\
                            " for a buffer from '%s' to '%s'",
                     direction==SYNCOLOR::TransformForward ? "forward" : "reverse",
                     color_space.c_str(), colorData->m_rendering_color_space.c_str(),
                     pixelFormatStr(src_pixel_format), pixelFormatStr(dst_pixel_format));
*/
               }
            }
         }
      }

      return status;
   }

   SYNCOLOR::SynStatus computeOutputColorTransformation(ColorManagerData* colorData,
                                                        const AtString& color_space, 
                                                        const SYNCOLOR::PixelFormat& src_pixel_format, 
                                                        const SYNCOLOR::PixelFormat& dst_pixel_format,
                                                        const SYNCOLOR::TransformDirection& direction,
                                                        SYNCOLOR::TransformPtr& transform)
   {
      SYNCOLOR::SynStatus status;

      SYNCOLOR::TemplateParameterPtr ws_param;
      status = colorData->m_output_template->getParameter(SYNCOLOR::WorkingSpace, ws_param);
      if(status)
      {
         status = ws_param->select(colorData->m_rendering_color_space);
         if(status)
         {
            SYNCOLOR::TemplateParameterPtr vt_param;
            status = colorData->m_output_template->getParameter(SYNCOLOR::ViewTransform, vt_param);
            if(status)
            {
               status = vt_param->select(color_space.c_str());
               if(status)
               {
                  status = colorData->m_output_template->createTransform(transform, direction);
               }
            }
         }
      }

      return status;
   }

   // The method finalizes the output color transformation which could be either the 'rendering to view transform'
   // color transformation (which includes Exposure/Contrast and the monitor lut)
   // or the reverse of the 'input to rendering' color transformation (to access to all linear color spaces). 
   //
   SYNCOLOR::SynStatus finalizeOutputColorTransformation(ColorManagerData* colorData,
                                                         const AtString& color_space, 
                                                         const SYNCOLOR::PixelFormat& src_pixel_format, 
                                                         const SYNCOLOR::PixelFormat& dst_pixel_format,
                                                         const SYNCOLOR::TransformDirection& direction,
                                                         SYNCOLOR::TransformPtr& transform)
   {
      const size_t key = computeKey(
         color_space.hash(), 
         (unsigned int)src_pixel_format, 
         (unsigned int)dst_pixel_format,
         (unsigned int)direction);

      SYNCOLOR::SynStatus status;

      ProcessorMap::const_iterator it1 = colorData->m_output_transforms.find(key);
      if(it1==colorData->m_output_transforms.end())
      {
         // Use a wrong error id because synColor does not have a 
         // generic error id for now.
         status.setErrorCode(SYNCOLOR::ERROR_RENDERER_GPU_ADAPTOR_ERROR);
         std::string err("The color space is unknown: ");
         err += color_space.c_str();
         status.setErrorMessage(err.c_str());
         return status;
      }

      if(it1->second.get()!=0x0)
      {
         transform = it1->second;
      }
      else
      {
         ThreadGuard guard(colorData->m_output_guard);

         ProcessorMap::iterator it2 = colorData->m_output_transforms.find(key);
         if(it2->second.get()!=0x0)
         {
            transform = it2->second;
         }
         else
         {
            status = computeOutputColorTransformation(
               colorData, color_space, src_pixel_format, dst_pixel_format, direction, transform);
            if(status)
            {
               status = SYNCOLOR::finalize(
                  transform, src_pixel_format, dst_pixel_format, optimizerFlags, resolveFlag, transform);
               if(status)
               {
                  it2->second = transform;
/*
                  AiMsgInfo("[color_manager_syncolor] Computation of %s output color transformation from '%s' to '%s'"\
                            " for a buffer from '%s' to '%s'",
                     direction==SYNCOLOR::TransformForward ? "forward" : "reverse",
                     colorData->m_rendering_color_space.c_str(), color_space.c_str(),
                     pixelFormatStr(src_pixel_format), pixelFormatStr(dst_pixel_format));
*/
               }
            }
         }
      }

      return status;
   }

   bool isInputColorSpace(ColorManagerData* colorData, const AtString& color_space)
   {
      const unsigned mum_color_spaces
         = colorData->m_config->getNumColorSpaces(SYNCOLOR::Config::InputColorSpaces);

      for(unsigned idx=0; idx<mum_color_spaces; ++idx)
      {
         if(0==strcmp(color_space.c_str(),
                      colorData->m_config->getColorSpaceName(SYNCOLOR::Config::AllColorSpaces, idx)))
         {
            return true;
         }
      }

      return false;
   }

};

node_parameters
{
   AiParameterStr (DataStr::native_catalog_path,     NULL);
   AiParameterStr (DataStr::custom_catalog_path,     NULL);
   AiParameterStr (DataStr::ocioconfig_path,         NULL);
   AiParameterStr (DataStr::rendering_color_space,   NULL);
   AiParameterBool(DataStr::output_color_conversion, false);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "defaultColorMgtGlobals");
}

node_initialize
{
   ColorManagerData* colorData = new ColorManagerData();
   AiNodeSetLocalData(node, colorData);
}

node_update
{
   ColorManagerData* colorData = (ColorManagerData*)AiNodeGetLocalData(node);

   colorData->m_native_catalog_path     = AiNodeGetStr (node, DataStr::native_catalog_path);
   colorData->m_custom_catalog_path     = AiNodeGetStr (node, DataStr::custom_catalog_path);

   colorData->m_ocioconfig_path         = AiNodeGetStr (node, DataStr::ocioconfig_path);
   colorData->m_rendering_color_space   = AiNodeGetStr (node, DataStr::rendering_color_space);

   colorData->m_input_transforms.clear();
   colorData->m_output_transforms.clear();

   initializeSynColor(colorData);
}

// That methods returns true if a color transformation exists even if the
// region of interest is empty.
// 
color_manager_transform
{  
   ColorManagerData* colorData = (ColorManagerData*)AiNodeGetLocalData(node);

   if(!colorData->m_initialization_done) return false;

   // 'auto' should have been resolved 
   static AtString autoStr("auto");
   if (color_space == autoStr) return false;

   // Find all the information to finalize the color transformation

   static const AtBBox2 default_roi(0, 0, 0, 0);
   const AtBBox2 &actual_roi = roi ? *roi : default_roi;
   const size_t width = actual_roi.maxx - actual_roi.minx + 1;
   const size_t height = actual_roi.maxy - actual_roi.miny + 1;
   uint8_t *actual_src = (uint8_t*)src;
   uint8_t *actual_dst = dst ? (uint8_t*)dst : actual_src;

   static const AtChannelLayout default_layout(AI_TYPE_FLOAT, AI_TYPE_RGB, 0, 0);
   const AtChannelLayout &actual_src_layout = src_layout ? *src_layout : default_layout;
   const AtChannelLayout &actual_dst_layout = dst_layout ? *dst_layout : actual_src_layout;

   const SYNCOLOR::PixelFormat src_pixel_format = computePixelFormat(actual_src_layout);
   const SYNCOLOR::PixelFormat dst_pixel_format = computePixelFormat(actual_dst_layout);
   if(src_pixel_format==SYNCOLOR::PF_UNKNOWN || dst_pixel_format==SYNCOLOR::PF_UNKNOWN)
   {
      return false;
   }

   // Compute the color transformation only if not already existing

   const bool is_input_color_space = isInputColorSpace(colorData, color_space);
   // Note: is_output name is misleading as it means from or to rendering color space
   const bool from_rendering_color_space = is_output;

   SYNCOLOR::SynStatus status;
   SYNCOLOR::TransformPtr transform;
   if(is_input_color_space)
   {
      status = finalizeInputColorTransformation(
         colorData, 
         color_space,
         src_pixel_format, dst_pixel_format,
         // The default behavior of the method is to compute the color processing based 
         //   on input to rendering color space explaining why the logic related to forward/reverse 
         //   is the inverse of the expected one. 
         from_rendering_color_space ? SYNCOLOR::TransformReverse : SYNCOLOR::TransformForward, transform);
   }
   else
   {
      status = finalizeOutputColorTransformation(
         colorData, 
         color_space, 
         src_pixel_format, dst_pixel_format,
         from_rendering_color_space ? SYNCOLOR::TransformForward : SYNCOLOR::TransformReverse, transform);
   }

   // No color processing returns true if a color transformation exists.

   if (!actual_src || width == 0 || height == 0 || !status)
   {
      if (!status)
      {
         AtString direction(from_rendering_color_space ? "Forward" : "Reverse");
         if(is_input_color_space)
         {
            direction = from_rendering_color_space ? AtString("Reverse") : AtString("Forward");
         }

         const AtString type(is_input_color_space ? "input" : "output");

         AiMsgDebug("[color_manager_syncolor] %s %s color transformation computation failed: %s", 
            direction.c_str(), type.c_str(), status.getErrorMessage());
         colorData->m_initialization_done = false;
      }

      return status;
   }

   // Apply the color transformation

   const size_t src_y_increment = actual_src_layout.y_stride - actual_src_layout.x_stride * width;
   const size_t dst_y_increment = actual_dst_layout.y_stride - actual_dst_layout.x_stride * width;
   for (int y = actual_roi.miny; y <= actual_roi.maxy; y++)
   {
      for (int x = actual_roi.minx; x <= actual_roi.maxx; x++)
      {
         transform->applyCPUSinglePixel(actual_src, actual_dst, x, y);

         actual_src += actual_src_layout.x_stride;
         actual_dst += actual_dst_layout.x_stride;
      }
      actual_src += src_y_increment;
      actual_dst += dst_y_increment;
   }

   return true;
}

color_manager_get_defaults
{
   static AtString sRgb_str("sRGB gamma");

   // This is creating problems with arnold color management (see #2893)
   AtString ocioPath = AiNodeGetStr(node, DataStr::ocioconfig_path);
   if (ocioPath.empty())
      sRGB = sRgb_str;

   linear = AiNodeGetStr (node, DataStr::rendering_color_space);
}

color_manager_get_chromaticities
{
   ColorManagerData* colorData = (ColorManagerData*)AiNodeGetLocalData(node);

   if(!colorData->m_initialization_done) return false;

   SYNCOLOR::ColorSpace::Ptr pColorSpace;
   SYNCOLOR::SynStatus status = colorData->m_config->getColorSpace(space.c_str(), pColorSpace);
   if(status)
   {
      SYNCOLOR::Chromaticities c;
      status = pColorSpace->getChromaticities(c);
      if(status)
      {
         chromaticities[0] = c.red.x;
         chromaticities[1] = c.red.y;
         chromaticities[2] = c.green.x;
         chromaticities[3] = c.green.y;
         chromaticities[4] = c.blue.x;
         chromaticities[5] = c.blue.y;
         chromaticities[6] = c.white.x;
         chromaticities[7] = c.white.y;

         return true;
      }
   }

   return false;
}

color_manager_get_custom_attributes
{
   num = 0;
}

color_manager_get_num_color_spaces
{
   ColorManagerData* colorData = (ColorManagerData*)AiNodeGetLocalData(node);

   if(!colorData->m_initialization_done) return 0;

   return colorData->m_config->getNumColorSpaces(SYNCOLOR::Config::AllColorSpaces);
}

color_manager_get_color_space_name_by_index
{
   ColorManagerData* colorData = (ColorManagerData*)AiNodeGetLocalData(node);

   if(!colorData->m_initialization_done)
   {
      return AtString("");
   }

   return AtString(colorData->m_config->getColorSpaceName(SYNCOLOR::Config::AllColorSpaces, i));
}

node_finish
{
   ColorManagerData* colorData =  (ColorManagerData*)AiNodeGetLocalData(node);
   delete colorData;
}

node_loader
{
   if (i > 0)
      return false;

   node->methods      = synColor_color_manager_Methods;
   node->name         = "color_manager_syncolor";
   node->node_type    = AI_NODE_COLOR_MANAGER;
   strcpy(node->version, AI_VERSION);
   return true;
}
