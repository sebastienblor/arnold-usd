//
// SynColor implementation for the color_manager interface
//

#include <ai.h>

#include <synColor/synColorInit.h>
#include <synColor/synColorPrefs.h>
#include <synColor/synColor.h>
#include <synColor/template.h>
#include <synColor/synColorFactory.h>

#ifdef _LINUX
#include <tr1/unordered_map>
typedef std::tr1::unordered_map<const AtString, SYNCOLOR::TransformPtr, AtStringHash> ProcessorMap;
#else
#include <unordered_map>
typedef std::unordered_map<const AtString, SYNCOLOR::TransformPtr, AtStringHash> ProcessorMap;
#endif
#include <string>
#include <fstream>


AI_COLOR_MANAGER_NODE_EXPORT_METHODS(synColor_color_manager_Methods);


// All the strings used by the ass file
namespace DataStr
{ 
   const AtString native_catalog_path("native_catalog_path");
   const AtString custom_catalog_path("custom_catalog_path");
   const AtString ocioconfig_path("ocioconfig_path");
   const AtString rendering_color_space("rendering_color_space");
   const AtString view_transform_space("view_transform_space");
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
      : m_output_color_conversion(false)
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

   // The way to correctly initialize the synColor engine
   AtString m_native_catalog_path;
   AtString m_custom_catalog_path;

   AtString m_ocioconfig_path;        // The ocio config file to use if ocio mode enabled
   AtString m_rendering_color_space;  // The rendering color space
   AtString m_view_transform_space;   // The view transform space (for kick display)
   bool     m_output_color_conversion;// Is it a color conversion only ?
   
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

   // The method initializes the SynColor library only once.
   void initializeSynColor(ColorManagerData* colorData)
   {
      SYNCOLOR::SynStatus status;

      const char* envVariableValue = getenv("SYNCOLOR");
      const bool useEnvVariable = envVariableValue!=0x0;

      if(!ColorManagerData::m_initialization_library_done)
      {
         std::string filename;

         status = SYNCOLOR::setUp(SYNCOLOR::Maya, SYNCOLOR::LANG_EN);
         if(status)
         {
            if(useEnvVariable)
            {
               filename = envVariableValue;
            }
            else
            {
               char tmpFilename[L_tmpnam];
               tmpnam(tmpFilename);

               std::ofstream ofs(tmpFilename, std::ofstream::out);

               ofs   << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                     << "<SynColorConfig version=\"2.0\">\n"
                     << "   <AutoConfigure graphicsMonitor=\"false\" />\n"
                     << "   <TransformsHome dir=\"" << (colorData->m_native_catalog_path ? colorData->m_native_catalog_path : AtString("")) << "\" />\n"
                     << "   <SharedHome dir=\"" << (colorData->m_custom_catalog_path ? colorData->m_custom_catalog_path: AtString("")) << "\" />\n"
                     << "   <ReferenceTable>\n"
                     << "   <Ref alias=\"OutputToSceneBridge\" path=\"misc/identity.ctf\" basePath=\"Autodesk\" />\n"
                     << "   <Ref alias=\"SceneToOutputBridge\" path=\"RRT+ODT/ACES_to_CIE-XYZ_v0.1.1.ctf\" basePath=\"Autodesk\" />\n"
                     << "   <Ref alias=\"broadcastMonitor\" path=\"display/broadcast/CIE-XYZ_to_HD-video.ctf\" basePath=\"Autodesk\" />\n"
                     << "   <Ref alias=\"defaultLook\" path=\"misc/identity.ctf\" basePath=\"Autodesk\" />\n"
                     << "   <Ref alias=\"graphicsMonitor\" path=\"interchange/sRGB/CIE-XYZ_to_sRGB.ctf\" basePath=\"Autodesk\" />\n"
                     << "   </ReferenceTable>\n"
                     << "</SynColorConfig>\n";

               ofs.close();

               filename = tmpFilename;
            }

            status = SYNCOLOR::configureAsStandalone(filename.c_str());
            if(status.getErrorCode()==SYNCOLOR::ERROR_SYN_COLOR_PREFS_ALREADY_LOADED)
            {
               // When using the Maya syncolor library, the initialization was already done.
               status = SYNCOLOR::SynStatus();
            }

            if(!status)
            {
               AiMsgWarning("[color_manager] Error: %s", status.getErrorMessage());

               // Try to survive to unexpected issue by creating the preferences from scratch.
               // It should never happen within Maya; however it could happen if used 
               // with kick (a tool without its own synColor catalog installation).
               status 
                  = SYNCOLOR::configurePaths(colorData->m_native_catalog_path, filename.c_str(), colorData->m_custom_catalog_path);
            }

            if(!useEnvVariable)
            {
               remove(filename.c_str());
            }

            if(status)
            {
               AiMsgInfo("[color_manager] Using syncolor_color_manager Version %s", SYNCOLOR::getVersionString());
               if(useEnvVariable)
               {
                  AiMsgInfo("                from the preference file %s", envVariableValue);
               }
               else
               {
                  AiMsgInfo("                with the native catolog from %s", colorData->m_native_catalog_path);
               }

               if(!colorData->m_ocioconfig_path.empty())
               {
                  AiMsgInfo("                using the OCIO config file %s", colorData->m_ocioconfig_path);
               }

               const char* pSharedDirectory = 0x0;
               SYNCOLOR::getSharedColorTransformPath(pSharedDirectory);                  
               AiMsgInfo("                and the optional custom catalog from %s", pSharedDirectory);
            }
         }
         ColorManagerData::m_initialization_library_done = (bool)status;
         colorData->m_initialization_done = false;
      }

      if(!colorData->m_initialization_done && ColorManagerData::m_initialization_library_done)
      {
         if(status)
         {
            if(!colorData->m_ocioconfig_path.empty())
            {
               status = SYNCOLOR::loadOCIOTemplate(
                  SYNCOLOR::InputTemplate, colorData->m_ocioconfig_path, colorData->m_input_template);
               if(status)
               {
                  status = SYNCOLOR::loadOCIOTemplate(
                     SYNCOLOR::ViewingTemplate, colorData->m_ocioconfig_path, colorData->m_output_template);
               }
            }
            else
            {
               status = SYNCOLOR::loadNativeTemplate(SYNCOLOR::InputTemplate, colorData->m_input_template);
               if(status)
               {
                  status = SYNCOLOR::loadNativeTemplate(SYNCOLOR::ViewingTemplate, colorData->m_output_template);
               }
            }
         }

         if(!status)
         {
            if(useEnvVariable)
            {
               AiMsgError("[color_manager] Initialization failed: %s from the preference file %s", 
                  status.getErrorMessage(), envVariableValue);
            }
            else
            {
               AiMsgError("[color_manager] Initialization failed: %s", status.getErrorMessage());
            }
         }

         colorData->m_initialization_done = (bool)status;
      }
   }

   // The method computes a identity color transformation useful for channel layout 
   // conversion between the source and destination buffers.
   SYNCOLOR::SynStatus computeRawColorTransformation(ColorManagerData* colorData, 
                                                     const SYNCOLOR::PixelFormat& src_pixel_format, 
                                                     const SYNCOLOR::PixelFormat& dst_pixel_format,
                                                     SYNCOLOR::TransformPtr& transform)
   {
      static const AtString tag("<Identity>");

      SYNCOLOR::SynStatus status;

      ProcessorMap::const_iterator it = colorData->m_output_transforms.find(tag);
      if(it != colorData->m_output_transforms.end())
      {
         transform = it->second;
      }
      else
      {
         ThreadGuard guard(colorData->m_output_guard);

         it = colorData->m_output_transforms.find(tag);
         if(it != colorData->m_output_transforms.end())
         {
            transform = it->second;
         }
         else
         {
            status = SYNCOLOR::createStockTransform(SYNCOLOR::STOCK_TRANSFORM_IDENTITY, transform);
            if(status)
            {
               status = SYNCOLOR::finalize(
                  transform, src_pixel_format, dst_pixel_format, optimizerFlags, resolveFlag, transform);
               if(status)
               {
                  colorData->m_output_transforms[tag] = transform;
               }
            }
         }
      }

      return status;
   }

   // The method computes the input color transformation which is the conversion from
   // the input color space to the rendering color space.
   SYNCOLOR::SynStatus computeInputColorTransformation(ColorManagerData* colorData,
                                                       const AtString& color_space, 
                                                       const SYNCOLOR::PixelFormat& src_pixel_format, 
                                                       const SYNCOLOR::PixelFormat& dst_pixel_format,
                                                       const SYNCOLOR::TransformDirection& direction,
                                                       SYNCOLOR::TransformPtr& transform)
   {
      // Having a human readable tag is always useful when debugging.
      const AtString key( std::string(
                              std::string(color_space.c_str())
                              + std::string(" to ")
                              + std::string(colorData->m_rendering_color_space.c_str())
                              + std::string(direction==SYNCOLOR::TransformReverse ? " reverse" : "" ) ).c_str() );

      SYNCOLOR::SynStatus status;

      ProcessorMap::const_iterator it = colorData->m_input_transforms.find(key);
      if(it != colorData->m_input_transforms.end())
      {
         transform = it->second;
      }
      else
      {
         ThreadGuard guard(colorData->m_input_guard);

         it = colorData->m_input_transforms.find(key);
         if(it != colorData->m_input_transforms.end())
         {
            transform = it->second;
         }
         else
         {
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
                        if(status)
                        {
                           if(status)
                           {
                              status = SYNCOLOR::finalize(
                                 transform, src_pixel_format, dst_pixel_format, optimizerFlags, resolveFlag, transform);
                              if(status)
                              {
                                 colorData->m_input_transforms[key] = transform;

                                 if(direction==SYNCOLOR::TransformReverse)
                                 {
                                    AiMsgInfo("[color_manager] Color transformation from '%s' to '%s'", 
                                       colorData->m_rendering_color_space.c_str(), color_space.c_str());
                                 }
                                 else
                                 {
                                    AiMsgInfo("[color_manager] Color transformation from '%s' to '%s'", 
                                       color_space.c_str(), colorData->m_rendering_color_space.c_str());
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }

      if(!status)
      {
         AiMsgError("[color_manager] Initialization failed: %s", status.getErrorMessage());
      }

      return status;
   }

   // The method computes the output color transformation which could be either the 'rendering to view transform'
   // color transformation (which includes Exposure/Contrast and the monitor lut)
   // or the reverse of the 'input to rendering' color transformation (to access to all linear color spaces). 
   //
   SYNCOLOR::SynStatus computeOutputColorTransformation(ColorManagerData* colorData,
                                                        const AtString& color_space, 
                                                        const SYNCOLOR::PixelFormat& src_pixel_format, 
                                                        const SYNCOLOR::PixelFormat& dst_pixel_format,
                                                        SYNCOLOR::TransformPtr& transform)
   {
      SYNCOLOR::SynStatus status;

      // FIXME is this the right place to test this flag ?
      if(colorData->m_output_color_conversion)
      {
         // The color transformation is the reverse of the input color transformation
         status = computeInputColorTransformation(
            colorData, color_space, src_pixel_format, dst_pixel_format, SYNCOLOR::TransformReverse, transform); 
      }
      else
      {
         // Having a human readable tag is always useful when debugging.
         const AtString key( std::string(
                                 std::string(colorData->m_rendering_color_space.c_str())
                                 + std::string(" to ")
                                 + std::string(color_space.c_str()) ).c_str() );

         ProcessorMap::const_iterator it = colorData->m_output_transforms.find(key);
         if(it != colorData->m_output_transforms.end())
         {
            transform = it->second;
         }
         else
         {
            ThreadGuard guard(colorData->m_output_guard);

            it = colorData->m_output_transforms.find(key);
            if(it != colorData->m_output_transforms.end())
            {
               transform = it->second;
            }
            else
            {
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
                           status = colorData->m_output_template->createTransform(transform, SYNCOLOR::TransformForward);
                           if(status)
                           {
                              status = SYNCOLOR::finalize(
                                 transform, src_pixel_format, dst_pixel_format, optimizerFlags, resolveFlag, transform);
                              if(status)
                              {
                                 colorData->m_output_transforms[key] = transform;
                                 AiMsgInfo("[color_manager] Color transformation from '%s' to '%s'",
                                    colorData->m_rendering_color_space.c_str(), color_space.c_str());
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }

      if(!status)
      {
         AiMsgError("[color_manager] Initialization failed: %s", status.getErrorMessage());
      }

      return status;
   }
};

node_parameters
{
   AiParameterStr (DataStr::native_catalog_path,     NULL);
   AiParameterStr (DataStr::custom_catalog_path,     NULL);
   AiParameterStr (DataStr::ocioconfig_path,         NULL);
   AiParameterStr (DataStr::rendering_color_space,   NULL);
   AiParameterStr (DataStr::view_transform_space,   NULL);
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
   colorData->m_view_transform_space   = AiNodeGetStr (node, DataStr::view_transform_space);
   colorData->m_output_color_conversion = AiNodeGetBool(node, DataStr::output_color_conversion);

   colorData->m_input_transforms.clear();
   colorData->m_output_transforms.clear();

   initializeSynColor(colorData);
}

color_manager_transform
{  
   ColorManagerData* colorData = (ColorManagerData*)AiNodeGetLocalData(node);

   if(!colorData->m_initialization_done) return false;

   // During kick rendering, this function is currently being called with "auto".
   // Let's use the view transform in that case.
   // FIXME : remove this if arnold core changes this behaviour
   static AtString autoStr("auto");
   if (color_space == autoStr)
      color_space = colorData->m_view_transform_space;

   // Find all the information to finalize the color transformation

   static const AtBBox2 default_roi(0, 0, 0, 0);
   const AtBBox2 &actual_roi = roi ? *roi : default_roi;
   const size_t width = actual_roi.maxx - actual_roi.minx + 1;
   const size_t height = actual_roi.maxy - actual_roi.miny + 1;
   uint8_t *actual_src = (uint8_t*)src;
   uint8_t *actual_dst = dst ? (uint8_t*)dst : actual_src;
   if (!actual_src || width == 0 || height == 0)
   {
      return true;
   }

   static const AtChannelLayout default_layout(AI_TYPE_FLOAT, AI_TYPE_RGB, 0, 0);
   const AtChannelLayout &actual_src_layout = src_layout ? *src_layout : default_layout;
   const AtChannelLayout &actual_dst_layout = dst_layout ? *dst_layout : actual_src_layout;

   SYNCOLOR::PixelFormat src_pixel_format = computePixelFormat(actual_src_layout);
   SYNCOLOR::PixelFormat dst_pixel_format = computePixelFormat(actual_dst_layout);
   if(src_pixel_format==SYNCOLOR::PF_UNKNOWN || dst_pixel_format==SYNCOLOR::PF_UNKNOWN)
   {
      return false;
   }

   // Compute the color transformation only if not already existing

   SYNCOLOR::SynStatus status;
   SYNCOLOR::TransformPtr transform;
   if(is_output)
   {
      status = computeOutputColorTransformation(
         colorData, color_space, src_pixel_format, dst_pixel_format, transform);
   }
   else
   {
      status = computeInputColorTransformation(
         colorData, color_space, src_pixel_format, dst_pixel_format, SYNCOLOR::TransformForward, transform);
   }

   // No color processing or failed to create the color transformation

   if(!status)
   {
      AiMsgError("[color_manager] %s color transformation computation failed: %s", 
         is_output ? "Output" : "Input", status.getErrorMessage());

      colorData->m_initialization_done = false;
      return false;
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

   return status;
}

color_manager_get_defaults
{
   static AtString sRgb_str = AtString("sRGB gamma");
   // This is creating problems with arnold color management (see #2893)
   AtString ocioPath = AiNodeGetStr(node, DataStr::ocioconfig_path);
   if (ocioPath.empty())
      sRGB = sRgb_str;
   
   linear = AiNodeGetStr (node, DataStr::rendering_color_space);
}

color_manager_get_chromaticities
{
   return false;
}

color_manager_get_custom_attributes
{
   num = 0;
}

color_manager_get_num_color_spaces
{
   return 0;
}

color_manager_get_color_space_name_by_index
{
   return AtString("");
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
