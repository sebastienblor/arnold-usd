#include "XgSplineArnoldExpand.h"
#include <ai.h>
#include <SgCurve.h>
#include <XgSplineAPI.h>
#include <cstring>
#include <string>
#include <sstream>

namespace XgArnoldInternal
{

using namespace XGenSplineAPI;
// ============================================================================

bool XgSplineProcedural::Init(AtNode* procedural, bool procParent)
{
   // Load the spline data from the procedural into our workspace
   if (!LoadSplineData(procedural))
   {
      AiMsgError("Failed to load spline data from the procedural parameter.");
      return false;
   }

   // Execute render-time script such as density multiplier and density mask
   _splines.executeScript();

   // Load other parameters from the procedural
   LoadParameters(procedural);

   // Create the "curves" node as our hair object
   CreateCurves(procedural, procParent);

   // Fill the "curves" node with array data
   FillCurves(procedural);

   return true;
}

bool XgSplineProcedural::LoadSplineData(AtNode* procedural)
{
   // Get the sample times
   AtArray* sampleTimes = AiNodeGetArray(procedural, "sampleTimes");
   if (!sampleTimes || AiArrayGetNumElements(sampleTimes) == 0)
   {
      AiMsgError("Invalid number of samples.");
      return false;
   }

   // Get frame-per-second
   const float fps = AiNodeGetFlt(procedural, "fps");
   _splines.setFps(fps);

   // Load samples
   // No motion blur : one sample at current frame
   // Motion blur    : one sample for each motion step
   unsigned nelements = AiArrayGetNumElements(sampleTimes);
   for (unsigned int i = 0; i < nelements; i++)
   {
      // Get the time of the sample
      const float sampleTime = AiArrayGetFlt(sampleTimes, i);

      // Get the name of the sample parameters
      std::stringstream ss;
      ss << "sampleData_" << i;
      const std::string dataParam = ss.str();

      ss.str("");
      ss << "samplePadding_" << i;
      const std::string paddingParam = ss.str();

      // Get the spline data for i-th motion step
      AtArray* sampleData = AiNodeGetArray(procedural, dataParam.c_str());
      if (!sampleData || AiArrayGetType(sampleData) != AI_TYPE_UINT) continue;

      // Get the number of padding bytes
      const unsigned int padding = AiNodeGetUInt(procedural, paddingParam.c_str());
      const size_t sampleSize = size_t(AiArrayGetNumElements(sampleData)) * sizeof(unsigned int) - padding;

      // Wrap the spline data as an input stream
      std::stringstream opaqueStrm;
         
      void *arrayData = AiArrayMap(sampleData);
      if (arrayData)
      {
         opaqueStrm.write(reinterpret_cast<const char*>(arrayData), sampleSize);
         opaqueStrm.flush();
         opaqueStrm.seekp(0);
      }
      AiArrayUnmap(sampleData);

      // Load the sample for i-th motion step
      if (!_splines.load(opaqueStrm, sampleSize, sampleTime))
      {
         AiMsgError("Failed to load sample %d.", i);
         return false;
      }
   }
   return true;
}

void XgSplineProcedural::LoadParameters(AtNode* procedural)
{
   _shader             = (AtNode*)AiNodeGetPtr(procedural, "shader");
   _aiMinPixelWidth    = AiNodeGetFlt(procedural, "ai_min_pixel_width");
   _aiMode             = AiNodeGetInt(procedural, "ai_mode");
}

void XgSplineProcedural::CreateCurves(AtNode* procedural, bool procParent)
{
   // Create the Arnold "curves" node for all the splines

   // Inherit the name from the procedural
   const std::string proceduralName = AiNodeGetName(procedural);
   const std::string curvesName     = proceduralName + "_curves";
   _curves = AiNode("curves", curvesName.c_str(), procParent ? procedural : NULL);

   // XGen is using uniform bsplines
   AiNodeSetStr(_curves, "basis", "b-spline");

   // Passthrough the shader assignment
   AiNodeSetPtr(_curves, "shader", _shader);

   // Min pixel width
   AiNodeSetFlt(_curves, "min_pixel_width", _aiMinPixelWidth);

   // Curves mode ("thick", "ribbon", or "oriented")
   AiNodeSetInt(_curves, "mode", _aiMode);
   AiNodeSetFlt(_curves, "motion_start", AiNodeGetFlt(procedural, "motion_start"));
   AiNodeSetFlt(_curves, "motion_end", AiNodeGetFlt(procedural, "motion_end"));

   if (!procParent)
   {
      // Transmitting parent node parameters to child nodes (#2752)
      // ... but only do it when there's no parent procedural (#3606)
      AiNodeSetBool(_curves, "opaque", AiNodeGetBool(procedural, "opaque"));
      AiNodeSetByte(_curves, "visibility", AiNodeGetByte(procedural, "visibility"));
      AiNodeSetBool(_curves, "self_shadows", AiNodeGetBool(procedural, "self_shadows"));
      AiNodeSetBool(_curves, "receive_shadows", AiNodeGetBool(procedural, "receive_shadows"));
      AiNodeSetBool(_curves, "matte", AiNodeGetBool(procedural, "matte"));
      AiNodeSetArray(_curves, "matrix", AiArrayCopy(AiNodeGetArray(procedural, "matrix")));
      bool use_light_group = AiNodeGetBool(procedural, "use_light_group");
      if (use_light_group)
      {
         AiNodeSetBool(_curves, "use_light_group", use_light_group);
         AiNodeSetArray(_curves, "light_group",  AiArrayCopy(AiNodeGetArray(procedural, "light_group")));
      }
      bool use_shadow_group = AiNodeGetBool(procedural, "use_shadow_group");
      if (use_shadow_group)
      {
         AiNodeSetBool(_curves, "use_shadow_group", use_shadow_group);
         AiNodeSetArray(_curves, "shadow_group",  AiArrayCopy(AiNodeGetArray(procedural, "shadow_group")));
      }
   }

}


SgVec3f XgSplineProcedural::Orientation(const SgVec3f& direction, const SgVec3f* positions, unsigned int i, unsigned int length, const unsigned int offset)
{
   SgVec3f splineCVDirection(1.0f, 1.0f, 1.0f);
   if (length <= 1)
   {
      return splineCVDirection;
   }

   if (i < length - 1)
   {
      splineCVDirection = positions[offset + i + 1] - positions[offset + i];
   }
   else
   {
      splineCVDirection = positions[offset + i] - positions[offset + i - 1];
   }
    
   return direction * splineCVDirection;
}

void XgSplineProcedural::FillCurves(AtNode* procedural)
{
   // Count the number of curves and the number of points
   // Arnold's b-spline requires two phantom points.
   unsigned int curveCount        = 0;
   unsigned int pointCount        = 0;
   unsigned int pointInterpoCount = 0;
   for (XgItSpline splineIt = _splines.iterator(); !splineIt.isDone(); splineIt.next())
   {
      curveCount        += splineIt.primitiveCount();
      pointCount        += splineIt.vertexCount();
      pointInterpoCount += splineIt.vertexCount() + splineIt.primitiveCount() * 2;
   }

#ifdef XGEN_ARNOLD_ORIENTATIONS
   bool orient = (AiNodeGetInt(procedural, "ai_mode") == 2);
#endif        
   // Get the number of motion samples
   const unsigned int steps = _splines.sampleCount();

   // Allocate buffers for the curves
   _numPoints  = AiArrayAllocate(curveCount, 1, AI_TYPE_UINT);
   _points     = AiArrayAllocate(pointInterpoCount, steps, AI_TYPE_VECTOR);
   _radius     = AiArrayAllocate(pointCount, 1, AI_TYPE_FLOAT);
   _uvCoord     = AiArrayAllocate(curveCount, 1, AI_TYPE_VECTOR2);

   // FIXME Arnold 5
   // here we're doing AiNodeSetArray below, so we won't need to unMap the arrays
   unsigned int*   numPoints   = reinterpret_cast<unsigned int*>(AiArrayMap(_numPoints));
   SgVec3f*        points      = reinterpret_cast<SgVec3f*>(AiArrayMap(_points));
   float*          radius      = reinterpret_cast<float*>(AiArrayMap(_radius));
   SgVec2f*        uvCoord      = reinterpret_cast<SgVec2f*>(AiArrayMap(_uvCoord));

   std::vector<SgVec3f> orientations;
   int orientIndex = 0;

#ifdef XGEN_ARNOLD_ORIENTATIONS
   if (orient)
      orientations.resize(pointInterpoCount, SgVec3f(0.f, 0.f, 0.f));
#endif


   // Fill the array buffers for motion step 0
   for (XgItSpline splineIt = _splines.iterator(); !splineIt.isDone(); splineIt.next())
   {
      const unsigned int  stride         = splineIt.primitiveInfoStride();
      const unsigned int  primitiveCount = splineIt.primitiveCount();
      const unsigned int* primitiveInfos = splineIt.primitiveInfos();
      const SgVec3f*      positions      = splineIt.positions(0);
   
      const SgVec3f*      directions     = NULL;
#ifdef XGEN_ARNOLD_ORIENTATIONS
      if (orient)
         directions     = splineIt.widthDirection(0);
#endif

      const float*        width          = splineIt.width();
      //const SgVec2f*      texcoords      = splineIt.texcoords();
      const SgVec2f*      patchUVs       = splineIt.patchUVs();

      for (unsigned int p = 0; p < primitiveCount; p++)
      {
         const unsigned int offset = primitiveInfos[p * stride];
         const unsigned int length = primitiveInfos[p * stride + 1];

         // Number of points
         *numPoints++ = length + 2;
         // Texcoord using the patch UV from the root point
         *uvCoord++ = patchUVs[offset];
         // Add phantom points at the beginning
         *points++ = PhantomFirst(&positions[offset], length);
         //*wCoord++ = phantomFirst(&texcoords[offset], length)[1];
         SgVec3f direction;
         if (directions)
         {
            direction = PhantomFirst(&directions[offset], length);
            orientations[orientIndex++] = Orientation(direction, positions, 0, length, offset);
         }
         // Copy varying data
         for (unsigned int i = 0; i < length; i++)
         {
            *points++ = positions[offset + i];
            *radius++ = width[offset + i] * 0.5f;
            //*wCoord++ = texcoords[offset + i][1];

            if (directions)
            {
               direction = directions[offset + i];
               orientations[orientIndex++] = Orientation(direction, positions, i, length, offset);
            }
         }

         // Add phantom points at the end
         *points++ = PhantomLast(&positions[offset], length);
         //*wCoord++ = phantomLast(&texcoords[offset], length)[1];

         if (directions)
         {
            direction = PhantomLast(&directions[offset], length);
            orientations[orientIndex++] = Orientation(direction, positions, length - 1, length, offset);
         }

      } // for each primitive
   } // for each primitive batch

   // Fill the array buffers for motion step > 0
   for (unsigned int key = 1; key < steps; key++)
   {
      for (XgItSpline splineIt = _splines.iterator(); !splineIt.isDone(); splineIt.next())
      {
         const unsigned int  stride         = splineIt.primitiveInfoStride();
         const unsigned int  primitiveCount = splineIt.primitiveCount();
         const unsigned int* primitiveInfos = splineIt.primitiveInfos();
         const SgVec3f*      positions      = splineIt.positions(key);

         for (unsigned int p = 0; p < primitiveCount; p++)
         {
            const unsigned int offset = primitiveInfos[p * stride];
            const unsigned int length = primitiveInfos[p * stride + 1];

            // Add phantom points at the beginning
            *points++ = PhantomFirst(&positions[offset], length);

            // Copy varying data
            for (unsigned int i = 0; i < length; i++)
            {
               *points++ = positions[offset + i];
            }

            // Add phantom points at the end
            *points++ = PhantomLast(&positions[offset], length);
           

         } // for each primitive
      } // for each primitive batch
   } // for each motion step

   // Set the buffers to the curves node
   AiNodeSetArray(_curves, "num_points", _numPoints);
   AiNodeSetArray(_curves, "points", _points);
   AiNodeSetArray(_curves, "radius", _radius);
   AiNodeSetArray(_curves, "uvs", _uvCoord);
   if (!orientations.empty())
      AiNodeSetArray(_curves, "orientations", AiArrayConvert((unsigned int)orientations.size(), 1, AI_TYPE_VECTOR, &orientations[0]));


}

};