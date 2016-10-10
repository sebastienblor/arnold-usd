//#include "extension/Extension.h"
//#include "utils/time.h"

#include <maya/MFileObject.h>
#include <maya/MTime.h>
#include <maya/MGlobal.h>

#include <maya/MTypes.h>
#ifdef WIN32
#undef min
#undef max
#endif

#include "BifrostTranslator.h"
#include "../common/bifrostObjectUserData.h"
#include "../common/bifrostHelpers.h"

#include <bifrostapi/bifrost_component.h>
#include <bifrostapi/bifrost_pointchannel.h>

#include <bifrostapi/bifrost_visitor.h>
#include <bifrostapi/bifrost_tiledataaccessor.h>

#include <string>
#include <vector>
#include <iostream>


AtNode* CBfDescriptionTranslator::CreateArnoldNodes()
{
   m_render_type = CBIFROST_FOAM; // FIXME get the appropriate value from Maya
   MFnDagNode  bifrostDesc;
   MDagPath bifrostPath = m_dagPath;
   //palDagPath.pop();

   bifrostDesc.setObject(bifrostPath.node());

   m_render_type = (RenderType)bifrostDesc.findPlug("bifrostRenderType").asInt();

   MPlug objectPlug = bifrostDesc.findPlug("object");
   MString obj_str = objectPlug.asString();
   
   m_object = obj_str.asChar();

#if MAYA_API_VERSION >= 201650
   
   MFnDependencyNode bfContainer(objectPlug.source().node());
   switch (m_render_type)
   {
      {
      case CBIFROST_AERO:
         
         MPlug containerPlug = bfContainer.findPlug("aeroCacheProperties");
         MFnDependencyNode bfAeroProps(containerPlug.source().node());
      
         MString cache_dir = bfAeroProps.findPlug("aeroCachePath").asString();
         MString cache_name = bfAeroProps.findPlug("aeroCacheFileName").asString();

         MString fullBifPath = cache_dir + cache_name + "/AeroObject//AeroObject_volume";
         m_file = fullBifPath.asChar();
         break;
      }
      {
      case CBIFROST_FOAM:
         MPlug containerPlug = bfContainer.findPlug("foamCacheProperties");
         MFnDependencyNode bfAeroProps(containerPlug.source().node());
      
         MString cache_dir = bfAeroProps.findPlug("foamCachePath").asString();
         MString cache_name = bfAeroProps.findPlug("foamCacheFileName").asString();

         MString fullBifPath = cache_dir + cache_name + "/Foam//Foam_particle";
         m_file = fullBifPath.asChar();
         break;
      }

      default:
         break;
   }

#else
   MString cache_dir = bifrostDesc.findPlug("cacheDir").asString();
   MString cache_name = bifrostDesc.findPlug("cacheName").asString();
    
   m_file = cache_dir.asChar();
   m_file += "/";
   m_file += cache_name.asChar();

#endif


   switch (m_render_type)
   {
      default:
      case CBIFROST_AERO:
         return AddArnoldNode("volume");

      case CBIFROST_LIQUID:
         AiMsgError("[bifrost]: liquid not implemented yet : %s", m_object.c_str());
      // not implemented for now
      break;

      case CBIFROST_FOAM:
      // export geometry as points
         return AddArnoldNode("points");
   }
   // for non-implemented Render Types
   return AddArnoldNode("procedural");
}



void CBfDescriptionTranslator::UpdateFoam(AtNode *node)
{
   std::vector<AtPoint> points;
   std::vector<AtVector> velocities;
   //std::vector<float> radius;

   bool motion =  IsMotionBlurEnabled();


   // Check if we have hot data in the current state server
   BifrostObjectUserData objectRef(m_object, m_file);

   if (!objectRef.objectExists())
   {
      // The specified object doesn't exist in the current state server.
      // Try to load the object from the cache file.

      const float frame = (float)MAnimControl::currentTime().value();
      const bool loaded = objectRef.loadFromFile(frame);
      
      if (!loaded || !objectRef.objectExists())
      {
         AiMsgError("[bifrost]: foam data %s  not found", m_object.c_str());
         return;
      }
   }

   // Get the particle channels
   Bifrost::API::PointChannel positionChannel = objectRef.findPointChannel("position");
   Bifrost::API::PointChannel velocityChannel = objectRef.findPointChannel("velocity");
   Bifrost::API::RefArray     extraChannels;

 // The position and velocity channels are mandatory
   if (!positionChannel || !velocityChannel) return;
   
   // gather all per-particle components
   const Bifrost::API::Component component = positionChannel.component();
   extraChannels += component.findChannelsByType(Bifrost::API::FloatType);
   extraChannels += component.findChannelsByType(Bifrost::API::FloatV2Type);
   extraChannels += component.findChannelsByType(Bifrost::API::FloatV3Type);
   extraChannels.remove(positionChannel);
   extraChannels.remove(velocityChannel);



   Bifrost::API::Layout       layout      = positionChannel.layout();
   const float            voxelScale  = layout.voxelScale();
   Bifrost::Math::Similarity  similarity  = component.world();

   // Tile data accessors
   Bifrost::API::TileDataAccessor   positionAccessor  = positionChannel.tileDataAccessor();
   Bifrost::API::TileDataAccessor   velocityAccessor  = velocityChannel.tileDataAccessor();


   std::vector<Bifrost::API::DataType> chType;
   std::vector<const void*>         chBase;
   std::vector<size_t>              chSize;
   std::vector<std::string>         chName;


   AiNodeDeclare(node, "velocity", "uniform VECTOR");

   // Get other tile data
   for (size_t ch = 0; ch < extraChannels.count(); ch++)
   {
      const Bifrost::API::PointChannel channel = extraChannels[ch];
      std::string ch_name = channel.name().c_str();
      size_t last_separator = ch_name.find_last_of("/");
      if (last_separator != std::string::npos) ch_name = ch_name.substr(last_separator + 1, ch_name.length() - 1 - last_separator);
      last_separator = ch_name.find_last_of("|");
      if (last_separator != std::string::npos) ch_name = ch_name.substr(last_separator + 1, ch_name.length() - 1 - last_separator);
      last_separator = ch_name.find_last_of("\\");
      if (last_separator != std::string::npos) ch_name = ch_name.substr(last_separator + 1, ch_name.length() - 1 - last_separator);

      chName.push_back(ch_name);
      chType.push_back(channel.dataType());
      chBase.push_back(NULL);
      chSize.push_back(0);

      std::string user_data_declaration;
      switch(channel.dataType())
      {
         default:
         case Bifrost::API::FloatType:
            user_data_declaration = "uniform FLOAT";
         break;
         case Bifrost::API::FloatV2Type:
            user_data_declaration = "uniform POINT2";
         break;
         case Bifrost::API::FloatV3Type:
            user_data_declaration = "uniform VECTOR";
         break;
      }

      AiNodeDeclare(node, ch_name.c_str(), user_data_declaration.c_str());
   }

   // get fps
   static const MTime sec(1.0, MTime::kSeconds);
   double fps = sec.as(MTime::uiUnit());
   float inv_fps = 1.f / (float)fps;

   // can we go directly to maxDepth instead of looping ?
   for (Bifrost::API::TreeIndex::Depth d = 0; d < (int)layout.depthCount(); d++)
   {
      for (Bifrost::API::TreeIndex::Tile t = 0; t < (int)layout.tileCount(d); t++)
      {
         Bifrost::API::TreeIndex tindex(t,d);

         // get number of elements at tindex.
         size_t elementCount = component.elementCount( tindex );
         if ( elementCount == 0 ) continue;

         // Get tile data
         const Bifrost::API::TileData<amino::Math::vec3f> positionTile =
            positionAccessor.tileData<amino::Math::vec3f>(tindex);
         const Bifrost::API::TileData<amino::Math::vec3f> velocityTile =
            velocityAccessor.tileData<amino::Math::vec3f>(tindex);


         // Loop over particles and emit
         for (size_t i = 0; i < positionTile.count(); i++)
         {
            // Convert position to world space
            const amino::Math::vec3f position = similarity.apply(voxelScale * positionTile[i]);
            const amino::Math::vec3f velocity = (i < velocityTile.count()) ? velocityTile[i] : amino::Math::vec3f(0.f, 0.f, 0.f);

            AtPoint part_pos;
            part_pos.x = position[0];
            part_pos.y = position[1];
            part_pos.z = position[2];

            AtVector part_vel;
            part_vel.x = velocity[0];
            part_vel.y = velocity[1];
            part_vel.z = velocity[2];

            // velocity is in seconds, needs to be expressed in frames
            part_vel *= inv_fps;

            points.push_back(part_pos);
            velocities.push_back(part_vel);
         }   
      }
   }
 
   AtArray *points_array = AiArrayAllocate(points.size(), (motion)? 2 : 1, AI_TYPE_POINT);
   AtArray *velocities_array = AiArrayAllocate(points.size(), 1, AI_TYPE_VECTOR);
   
   for (unsigned int i = 0; i < points.size(); ++i)
   {
      AiArraySetPnt(points_array, i, points[i]);
      if (motion)
      {
        AtPoint motion_pnt = points[i] + velocities[i];        
        AiArraySetPnt(points_array, i + points.size(), motion_pnt);
      }
      AiArraySetVec(velocities_array, i, velocities[i]);
   }


   AiNodeSetArray(node, "points", points_array);  
   MFnDagNode  bifrostDesc;
   bifrostDesc.setObject(m_dagPath.node());
   float global_radius = bifrostDesc.findPlug("renderParticleSize").asFloat();
   AiNodeSetFlt(node, "radius", global_radius );  
   AiNodeSetArray(node, "velocity", velocities_array);  


   AiNodeSetStr(node, "mode", "sphere");
   AiNodeSetFlt(node, "min_pixel_width", 0.f);
   AiNodeSetFlt(node, "step_size", 0.f);
   AiNodeSetBool(node, "opaque", false);


   std::vector<AtArray *> channels_array;
   channels_array.resize(extraChannels.count());
   for (size_t ch = 0; ch < extraChannels.count(); ch++)
   {
      AtByte user_data_type = (chType[ch] == Bifrost::API::FloatType) ? AI_TYPE_FLOAT : 
            ((chType[ch] == Bifrost::API::FloatV2Type) ? AI_TYPE_POINT2 : AI_TYPE_VECTOR );
      channels_array[ch] = AiArrayAllocate(points.size(), 1, user_data_type);
   }


   // Now need to loop again for the extra-attributes, all this because I didn't know the amount of particles in the first place
   // Isn't there a way to get it ?
   // can we go directly to maxDepth instead of looping ?
   int pindex = 0;
   for (Bifrost::API::TreeIndex::Depth d = 0; d < (int)layout.depthCount(); d++)
   {
      for (Bifrost::API::TreeIndex::Tile t = 0; t < (int)layout.tileCount(d); t++)
      {
         Bifrost::API::TreeIndex tindex(t,d);

         // get number of elements at tindex.
         size_t elementCount = component.elementCount( tindex );
         if ( elementCount == 0 ) continue;

         for (size_t i = 0; i < elementCount; ++i, ++pindex)
         {
            if (pindex >= (int)points.size()) continue; // this shouldn't be happening but it does. frame mismatch ?
            
         // Get Tile starting point for each of the extra-channels
            for (size_t ch = 0; ch < extraChannels.count(); ch++)
            {
               const Bifrost::API::PointChannel channel = extraChannels[ch];
               chBase[ch] = channel.tileDataPtr(tindex, chSize[ch]);

               switch (chType[ch])
               {
                  default:
                  {
                  case Bifrost::API::FloatType:
                     const float *val = reinterpret_cast<const float*>(chBase[ch]) + i;
                     AiArraySetFlt(channels_array[ch], pindex, *val);
                     break;
                  }
                  {
                  case Bifrost::API::FloatV2Type:
                     const amino::Math::vec2f *val = reinterpret_cast<const amino::Math::vec2f*>(chBase[ch]) + i;
                     AtPoint2 pnt2;
                     pnt2.x = (*val)[0];
                     pnt2.y = (*val)[1];
                     AiArraySetPnt2(channels_array[ch], pindex, pnt2);
                     break;
                  }
                  {
                  case Bifrost::API::FloatV3Type:
                     const amino::Math::vec3f *val = reinterpret_cast<const amino::Math::vec3f*>(chBase[ch]) + i;
                     AtVector vec;
                     vec.x = (*val)[0];
                     vec.y = (*val)[1];
                     vec.z = (*val)[2];
                     AiArraySetVec(channels_array[ch], pindex, vec);
                     break;
                  }               
               }           
            }         
         }
      }
   }

   for (size_t ch = 0; ch < extraChannels.count(); ch++)
   {
      AiNodeSetArray(node, chName[ch].c_str(), channels_array[ch]);  
   }


   ExportMatrix(node);   
   
   if (RequiresShaderExport())
      ExportBifrostShader();
   ExportLightLinking(node);

}
void CBfDescriptionTranslator::UpdateAero(AtNode *shape)
{

// Check if we have hot data in the current state server
   BifrostObjectUserData objectRef(m_object, m_file);
   
   if (!objectRef.objectExists())
   {
      // The specified object doesn't exist in the current state server.
      // Try to load the object from the cache file.

      const float frame = (float)MAnimControl::currentTime().value();
      const bool loaded = objectRef.loadFromFile(frame);

      if (!loaded || !objectRef.objectExists())
      {
         AiMsgError("[bifrost]: Aero data %s  not found", m_object.c_str());
         return;
      }
   }
   static std::string strDSO = std::string(getenv("MTOA_PATH")) + std::string("/procedurals/bifrost_procedural.so");
   AiNodeSetStr(shape, "dso", strDSO.c_str());
   AiNodeSetBool( shape, "load_at_init", true );

   AiNodeDeclare(shape, "object_name", "constant STRING");
   AiNodeDeclare(shape, "file_name", "constant STRING");
   AiNodeDeclare(shape, "inv_fps", "constant FLOAT");

   AiNodeSetStr(shape, "object_name", m_object.c_str());
   AiNodeSetStr(shape, "file_name", m_file.c_str());


   static const MTime sec(1.0, MTime::kSeconds);
   double fps = sec.as(MTime::uiUnit());
   float inv_fps = 1.f / (float)fps;

   AiNodeSetFlt(shape, "inv_fps", inv_fps);

   double bboxMin[3] = {0.0, 0.0, 0.0}, bboxMax[3] = {0.0, 0.0, 0.0} ;

   Bifrost::API::Channel chnl = objectRef.findVoxelChannel("smoke");
   ComputeVolumeVoxelBounds(objectRef, chnl, bboxMin, bboxMax);

   // Check if the bounds is valid
   if (bboxMin[0] >= bboxMax[0] || bboxMin[1] >= bboxMax[1] || bboxMin[2] >= bboxMax[2])
   {
      AiMsgError("[bifrost]: bounds for %s  are not valid", m_object.c_str());
      return;
   }
 
   AiNodeSetPnt(shape, "min", (float)bboxMin[0], (float)bboxMin[1], (float)bboxMin[2] );
   AiNodeSetPnt(shape, "max", (float)bboxMax[0], (float)bboxMax[1], (float)bboxMax[2] );

   AiNodeSetByte(shape, "visibility", 243);

   ExportMatrix(shape);   
   
   if (RequiresShaderExport())
   {
      ExportBifrostShader();
      // we need to hack this because a volume shader doesn't work with a 
      // MayaShadingGroup node in the middle, so I'm bypassing it
      AtNode *sgNode = (AtNode*)AiNodeGetPtr(shape, "shader");
      if (sgNode && AiNodeIsLinked(sgNode, "beauty"))
      {
         AtNode *shad = AiNodeGetLink(sgNode, "beauty");
         AiNodeSetPtr(shape, "shader", shad);
      }
   }
   ExportLightLinking(shape);
}
void CBfDescriptionTranslator::UpdateLiquid(AtNode *shape)
{
   // not implemented
}


void CBfDescriptionTranslator::Export(AtNode* node)
{   
   switch (m_render_type)
   {
      default:
      case CBIFROST_AERO:
         UpdateAero(node);
      break;
      case CBIFROST_LIQUID:
         UpdateLiquid(node);
      break;
      case CBIFROST_FOAM:
         UpdateFoam(node);
      break;
   }
}

void CBfDescriptionTranslator::ExportMotion(AtNode* shape)
{
   // Check if motionblur is enabled and early out if it's not.
   if (!IsMotionBlurEnabled()) return;

   // Set transform matrix
   ExportMatrix(shape);
}

void CBfDescriptionTranslator::NodeInitializer(CAbTranslator context)
{
   
}

void CBfDescriptionTranslator::ExportBifrostShader()
{
   AtNode *node = GetArnoldNode();

   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), 0);
   if (!shadingGroupPlug.isNull())
   {
      AtNode *rootShader = ExportConnectedNode(shadingGroupPlug);
      if (rootShader != NULL)
      { 
         // Push the shader in the vector to be assigned later to mtoa_shading_groups
         AiNodeSetPtr(node, "shader", rootShader);

      }
   }

}

void CBfDescriptionTranslator::RequestUpdate()
{
   SetUpdateMode(AI_RECREATE_NODE);
   CShapeTranslator::RequestUpdate();
}
