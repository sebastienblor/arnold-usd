#include "ParticleTranslator.h"
#include "attributes/AttrHelper.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MString.h>
#include <maya/MFnMesh.h>
#include <maya/MVectorArray.h>
#include <maya/MStringArray.h>
#include <maya/MMatrix.h>
#include <maya/MSelectionList.h>

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_noise.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

AtNode* CParticleTranslator::CreateArnoldNodes()
{
   if (IsMasterInstance())
      return  AddArnoldNode("points");
   else
      return  AddArnoldNode("ginstance");
}

void CParticleTranslator::NodeInitializer(CAbTranslator context)
{
   MString mayaNodeType = context.maya;
   CExtensionAttrHelper helper(mayaNodeType, "points");

   // node attributes
   CShapeTranslator::MakeCommonAttributes(helper);

   CAttrData data;

   data.defaultValue.BOOL = false;
   data.name = "aiExportParticleIDs";
   data.shortName = "ai_export_particle_ids";
   helper.MakeInputBoolean(data);

   data.defaultValue.STR = "";
   data.name = "aiExportAttributes";
   data.shortName = "ai_export_attributes";
   helper.MakeInputString(data);

   MStringArray  enumNames;
   enumNames.append("points");
   enumNames.append("spheres");
   enumNames.append("quads");
   data.defaultValue.INT = 0;
   data.name = "aiRenderPointsAs";
   data.shortName = "ai_render_points_as";
   data.enums= enumNames;
   helper.MakeInputEnum(data);

   data.defaultValue.FLT = 0;
   data.name = "aiMinParticleRadius";
   data.shortName = "ai_min_particle_radius";
   helper.MakeInputFloat(data);

   data.defaultValue.FLT = 1.0;
   data.name = "aiRadiusMultiplier";
   data.shortName = "ai_radius_multiplier";
   helper.MakeInputFloat(data);

   data.defaultValue.FLT = 1000000;
   data.name = "aiMaxParticleRadius";
   data.shortName = "ai_max_particle_radius";
   helper.MakeInputFloat(data);

   data.defaultValue.FLT = 0;
   data.name = "aiMinPixelWidth";
   data.shortName = "ai_min_pixel_width";
   helper.MakeInputFloat(data);

   data.defaultValue.BOOL = false;
   data.name = "aiDeleteDeadParticles";
   data.shortName = "ai_delete_dead_particles";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "aiInterpolateBlur";
   data.shortName = "ai_interpolate_blur";
   helper.MakeInputBoolean(data);

   data.defaultValue.FLT = 0.f;
   data.name = "aiStepSize";
   data.shortName = "ai_step_size";
   data.hasMin = true;
   data.min.FLT = 0.f;
   data.hasSoftMax = true;
   data.softMax.FLT = 2.f;
   helper.MakeInputFloat(data);

   data.defaultValue.FLT = 1.f;
   data.name = "aiEvaluateEvery";
   data.shortName = "ai_evaluate_every";
   data.hasMin = true;
   data.hasSoftMin = true;
   data.min.FLT = 0.0001f;
   data.softMin.FLT = 0.1f;
   data.hasSoftMax = true;
   data.softMax.FLT = 2.f;
   helper.MakeInputFloat(data);
}

void CParticleTranslator::ExportParticleShaders(AtNode* particle)
{
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroupPlug.isNull())
   {
      AtNode *rootShader = ExportConnectedNode(shadingGroupPlug);
      if (rootShader != NULL)
      {
         AiNodeSetPtr(particle, "shader", rootShader);
      }
   }
}

/// parse the m_customAttrs and populate the step vectors for each type of custom export attrs.
/// called from within ExportParticleData
void CParticleTranslator::ExportCustomParticleData(AtNode* particle)
{
   MStringArray attrs;

   MStatus status = m_customAttrs.split(' ', attrs);
   if (status ==  MS::kSuccess)
   {
      for (unsigned int i=0; i < attrs.length(); i++)
      {

         MString currentAttr = attrs[i];
         if (currentAttr == "rgbPP" ||
             currentAttr == "opacityPP" ||
             currentAttr == "radiusPP")
            continue;
         m_fnParticleSystem.findPlug(currentAttr, &status);
         if (status != MS::kSuccess)
            continue;

         //check the type of the plug
         if (m_fnParticleSystem.isPerParticleDoubleAttribute(currentAttr))
         {
            MDoubleArray*  attributes = new MDoubleArray;
            m_fnParticleSystem.getPerParticleAttribute(currentAttr, *attributes);
            if (attributes->length() == 0)
               delete attributes;
            else
               m_out_customDoubleAttrArrays[currentAttr.asChar()] = attributes;
         }
         else if (m_fnParticleSystem.isPerParticleVectorAttribute(currentAttr))
         {
            MVectorArray*  attributes = new MVectorArray;
            m_fnParticleSystem.getPerParticleAttribute(currentAttr, *attributes);
            if (attributes->length() == 0)
               delete attributes;
            else
               m_out_customVectorAttrArrays[currentAttr.asChar()] = attributes;
         }
         else if (m_fnParticleSystem.isPerParticleIntAttribute(currentAttr))
         {
            MIntArray*  attributes = new MIntArray;
            m_fnParticleSystem.getPerParticleAttribute(currentAttr, *attributes);
            if (attributes->length() == 0)
               delete attributes;
            else
               m_out_customIntAttrArrays[currentAttr.asChar()] = attributes;
         }
      }
   }
}

/// ExportPreambleData: sets up most of the class variables for any/all motion steps, and writes out some of the default data
///  to the settings AiStrings
void CParticleTranslator::ExportPreambleData(AtNode* particle)
{
   int renderType = m_fnParticleSystem.renderType();

   AiMsgDebug("[mtoa] Exporting particle system %s with particleType %i", m_fnParticleSystem.partialPathName().asChar(), renderType);
   MStatus status;

   // Particle shape extra attributes
   int pointsAs            = m_fnParticleSystem.findPlug("aiRenderPointsAs").asInt();
   m_exportId              = m_fnParticleSystem.findPlug("aiExportParticleIDs").asBool();
   m_isOpaque              = m_fnParticleSystem.findPlug("aiOpaque").asBool();
   m_deleteDeadParticles   = m_fnParticleSystem.findPlug("aiDeleteDeadParticles").asBool();
   m_inheritCacheTxfm      = m_fnParticleSystem.findPlug("aiInheritCacheTransform").asBool();

   m_minPixelWidth = m_fnParticleSystem.findPlug("aiMinPixelWidth").asFloat();
   AiNodeSetFlt(particle, "min_pixel_width", m_minPixelWidth);

   // TODO implement  streak / blobby / cloud / tube,  formats
   switch (renderType)
   {
      case PARTICLE_TYPE_SPHERE:
         AiNodeSetStr(particle, "mode", "sphere");
         break;
      case PARTICLE_TYPE_SPRITE:
         AiNodeSetStr(particle, "mode", "quad");
         m_isSprite = true;
         break;
      case PARTICLE_TYPE_BLOBBYSURFACE:
         AiNodeSetStr(particle, "mode", "sphere");
         break;
      case PARTICLE_TYPE_CLOUD:
         AiNodeSetStr(particle, "mode", "sphere");
         break;
      case PARTICLE_TYPE_TUBE:
         AiNodeSetStr(particle, "mode", "sphere");
         break;
      default: // points
         {
            if (pointsAs == 1)
               AiNodeSetStr(particle, "mode", "sphere");
            else if (pointsAs == 2)
               AiNodeSetStr(particle, "mode", "quad");
            else
               AiNodeSetStr(particle, "mode", "disk");
         }
         break;
   }

   // now check for multi
   if (renderType == PARTICLE_TYPE_MULTIPOINT || renderType == PARTICLE_TYPE_MULTISTREAK) // multiPoint/multiStreak
   {
      m_doMultiPoint = true;
      AiNodeDeclare(particle, "particleMultiIndex", "uniform INT");
      MPlug mcPlug( m_fnParticleSystem.findPlug("multiCount", &status));
      if ( MS::kSuccess == status )
      {
         mcPlug.getValue( m_multiCount );
      }
      MPlug mrPlug( m_fnParticleSystem.findPlug("multiRadius", &status));
      if ( MS::kSuccess == status )
      {
         mrPlug.getValue( m_multiRadius );
      }
      m_multiRadius *=2;
   }

   // ask if the particle has the basic PP attributes we want to export
   m_hasRGB = m_fnParticleSystem.hasRgb();

   // opaqueness
   m_hasOpacity = false;
   if (!m_isOpaque)
      m_hasOpacity = m_fnParticleSystem.hasOpacity();  // if all opacity is  1, this will return 0

   AiNodeSetBool(particle, "opaque", m_isOpaque);

   // get the array of rgbPPs
   if (m_hasRGB)
   {
      AiNodeDeclare(particle, "rgbPP", "uniform RGB");
   }

   // get the array of opacities
   if (m_hasOpacity)
   {
      AiNodeDeclare(particle, "opacityPP", "uniform FLOAT");
   }

   // radius is a bit more complicated.   radiusPP overrides everything except spritePP widths
   m_hasRadiusPP  = m_fnParticleSystem.isPerParticleDoubleAttribute(MString("radiusPP"));

   if (m_exportId)
   {
      AiNodeDeclare(particle, "particleId", "uniform INT");
   }

   if (!m_hasRadiusPP || m_isSprite)
   {
      MPlug radiusPlug( m_fnParticleSystem.findPlug("radius", &status));
      if ( MS::kSuccess == status )
      {
         radiusPlug.getValue( m_radius );
      }
      MPlug pointPlug( m_fnParticleSystem.findPlug("pointSize", &status));
      if ( MS::kSuccess == status )
      {
         pointPlug.getValue( m_pointSize );
      }
      MPlug linePlug( m_fnParticleSystem.findPlug("lineWidth", &status));
      if ( MS::kSuccess == status )
      {
         linePlug.getValue( m_lineWidth );
      }
      MPlug ssXPlug( m_fnParticleSystem.findPlug("spriteScaleX", &status));
      if ( MS::kSuccess == status )
      {
         ssXPlug.getValue( m_spriteScaleX );
      }
      MPlug ssYPlug( m_fnParticleSystem.findPlug("spriteScaleY", &status));
      if ( MS::kSuccess == status )
      {
         ssYPlug.getValue( m_spriteScaleY );
      }

      switch (renderType)
      {
      case PARTICLE_TYPE_MULTIPOINT:
         m_particleSize = (m_pointSize)*0.01;
         break;
      case PARTICLE_TYPE_SPHERE:
         m_particleSize = m_radius;
         break;
      case PARTICLE_TYPE_SPRITE: // sprite
         if (m_fnParticleSystem.isPerParticleDoubleAttribute(MString("spriteScaleXPP")) &&
             m_fnParticleSystem.isPerParticleDoubleAttribute(MString("spriteScaleYPP")))
         {
            m_isSpritePP = true;
         }

         m_particleSize = m_spriteScaleX/2.0;
         break;
      case PARTICLE_TYPE_STREAK: // streak
         MGlobal::displayWarning("[mtoa]: Streak particle type is not yet supported");
         AiMsgWarning("[mtoa] Streak particle type is not yet supported");
         m_particleSize = (m_lineWidth)*0.01;
         break;
      case PARTICLE_TYPE_BLOBBYSURFACE:// blobby
         MGlobal::displayWarning("[mtoa]: Blobby particle type is not yet supported");
         AiMsgWarning("[mtoa] Blobby particle type is not yet supported");
         m_particleSize = m_radius;
         break;
      case PARTICLE_TYPE_CLOUD:// cloud
         MGlobal::displayWarning("[mtoa]: Cloud particle type is not yet supported");
         AiMsgWarning("[mtoa] Cloud particle type is not yet supported");
         m_particleSize = m_radius;
         break;
      case PARTICLE_TYPE_TUBE:// tube
         MGlobal::displayWarning("[mtoa]: Tube particle type is not yet supported");
         AiMsgWarning("[mtoa] Tube particle type is not yet supported");
         m_particleSize = m_radius;
         break;
      default:
         m_particleSize = (m_pointSize)*0.01;
         break;
      }
   }
}

/// Export particle data with basic attributes rgbPP, OpacityPP, radiusPP and particleId

void CParticleTranslator::GatherFirstStep(AtNode* particle)
{
   // quick synopsis:  we  loop on steps->particles->multipoint
   // first step, we push a copy of first steps particle data into output arrays
   // each subsequent step we copy the values from the  previous step, and then
   // we  loop on particle IDS  finding matching particles by ID  and modify  that steps value
   // if we find new particles, we compute  previous velocity positions and add those particles to the end of all the arrays
   // if particles die, we compute current values only,  based on last position and last velocity
   // because we already copied  the last existing steps value at the beginning of the current step
   // we don't have to compute anything else for any other attribute
   // as they are already populated with their most current values

   MVectorArray   velocityArray;
   MVectorArray   accelerationArray;
   MIntArray      particleId;

   MVectorArray*   positionArray = new MVectorArray;
   MDoubleArray*   radiusArray = new MDoubleArray;
   MDoubleArray*   spriteScaleXPP = new MDoubleArray;
   MDoubleArray*   spriteScaleYPP = new MDoubleArray;

   MStatus status;
   
   GatherStandardPPData( MAnimControl::currentTime(),
                         positionArray,
                         radiusArray,
                         spriteScaleXPP,
                         spriteScaleYPP,
                         &m_out_colorArray,
                         &m_out_opacityArray,
                         velocityArray,
                         accelerationArray,
                         particleId);

   m_particleCount = positionArray->length();

   m_instantVeloArray = velocityArray;
   m_instantAcceArray = accelerationArray;

   int numStep = GetNumMotionSteps();
   int step = GetMotionStep();
   
   m_out_positionArrays.assign(numStep, NULL);
   m_out_positionArrays[step] = positionArray;

   if (m_isSprite)
   {
      m_out_spriteScaleXArrays.assign(numStep, NULL);
      m_out_spriteScaleXArrays[step] = spriteScaleXPP;
      m_out_spriteScaleYArrays.assign(numStep, NULL);
      m_out_spriteScaleYArrays[step] = spriteScaleYPP;
   }
   else
   {
      m_out_radiusArrays.assign(numStep, NULL);
      m_out_radiusArrays[step] = radiusArray;
   }

   //  create the map  from particleID to array index in the "out_*"  arrays
   for (int i=0; i < m_particleCount; i++)
      m_particleIDMap[particleId[i]] = i;
   

   m_customAttrs = m_fnParticleSystem.findPlug("aiExportAttributes").asString();

   if (m_customAttrs.length() != 0)
   {
      m_doExtraAttributes = true;
      ExportCustomParticleData(particle);
   }

   if ((m_fnParticleSystem.findPlug("aiInterpolateBlur").asBool()) && IsNParticle())
   {
      const float evaluateEvery = FindMayaPlug("aiEvaluateEvery").asFloat();

      MTime curTime = MAnimControl::currentTime();
      MTime finalTime;

      float fra1 = (float)curTime.as(MTime::uiUnit());
      float diff = 0.0;

      if(fmod(fra1, evaluateEvery) != 0.0f)
      {
         diff = fmod(fra1, evaluateEvery);
         MTime timeDiff(evaluateEvery - diff, MTime::uiUnit());
         diff /= evaluateEvery; //Normalize diff in the 0-1 interval
         finalTime = curTime + timeDiff;
      }
      else
         return;

      MTime oneSec(1.0, MTime::kSeconds);
      float fps =  (float)oneSec.asUnits(MTime::uiUnit());
      double dt = (1/fps);

      int numParticles = m_fnParticleSystem.count();

      //Interpolation using the first two points. That are calculated from first particle data.

      for (int j = 0; j < numParticles; j++)
      {
         MVector p0, p1;
         p0 = (*m_out_positionArrays[step])[j] - velocityArray[j]*dt*evaluateEvery;
         p1 = (*m_out_positionArrays[step])[j];
            
         MVector result = diff*((2-diff)*diff - 1)*p0;
         result += (diff*diff*(3*diff - 5) + 2)*p1;

         (*m_out_positionArrays[step])[j] = result;

      }

      MVectorArray   velocityArray2;
      MVectorArray   accelerationArray2;
      MIntArray      particleId2;

      MVectorArray*   positionArray2 = new MVectorArray;
      MDoubleArray*   radiusArray2 = new MDoubleArray;
      MDoubleArray*   spriteScaleXPP2 = new MDoubleArray;
      MDoubleArray*   spriteScaleYPP2 = new MDoubleArray;

      GatherStandardPPData( finalTime,
                            positionArray2,
                            radiusArray2,
                            spriteScaleXPP2,
                            spriteScaleYPP2,
                            0,
                            0,
                            velocityArray2,
                            accelerationArray2,
                            particleId2);

      numParticles = m_fnParticleSystem.count();

      // FIXME this system doesn't seem very optimized...
      std::map <int, int> tempMap = m_particleIDMap;
      std::map <int, int>::iterator it;

      std::map <int, int> pos2Map;
      std::map <int, int>::iterator it2;

      for (int j = 0; j < numParticles; j++)
      {
         int partId = particleId2[j];
         it = tempMap.find(partId);
         if (it != tempMap.end())   // found the particle in the scene already
         {
            int pindex = it->second;

            pos2Map[pindex] = j;

            MVector p2;
            p2 = (*positionArray2)[j];
            
            MVector result = diff*((4 - 3*diff)*diff + 1) * p2;

            (*m_out_positionArrays[step])[pindex] += result;

            // to speed up the  search, we remove the particles we've already found..
            tempMap.erase(it);
         }
      }

      // if we still have entries in tempMap, that means the particle died in this frameStep
      if (tempMap.size() > 0)
      {
         for (it = tempMap.begin(); it != tempMap.end(); it++)
         {
            // here we Support removing of dead particles via looping and culling by particle map in the final output loop
            int pindex = it->second;
            (*m_out_radiusArrays[step])[pindex] = 0.0;
         }
      }

      MTime cacheInterval(evaluateEvery, MTime::uiUnit());
      finalTime += cacheInterval;

      MVectorArray   velocityArray3;
      MVectorArray   accelerationArray3;
      MIntArray      particleId3;

      MVectorArray*   positionArray3 = new MVectorArray;
      MDoubleArray*   radiusArray3 = new MDoubleArray;
      MDoubleArray*   spriteScaleXPP3 = new MDoubleArray;
      MDoubleArray*   spriteScaleYPP3 = new MDoubleArray;

      GatherStandardPPData( finalTime,
                            positionArray3,
                            radiusArray3,
                            spriteScaleXPP3,
                            spriteScaleYPP3,
                            0,
                            0,
                            velocityArray3,
                            accelerationArray3,
                            particleId3);

      numParticles = m_fnParticleSystem.count();

      tempMap = m_particleIDMap;

      m_fnParticleSystem.evaluateDynamics(curTime, false);

      for (int j = 0; j < numParticles; j++)
      {
         int partId = particleId3[j];
         it = tempMap.find(partId);
         if (it != tempMap.end())   // found the particle in the scene already
         {
            int pindex = it->second;

            MVector p3;
            p3 = (*positionArray3)[j];
            
            MVector result = (diff - 1) * diff * diff * p3;

            (*m_out_positionArrays[step])[pindex] += result;
            (*m_out_positionArrays[step])[pindex] *= 0.5;

            // to speed up the  search, we remove the particles we've already found..
            tempMap.erase(it);
         }
      }

      // if we still have entries in tempMap, that means the particle died in this frameStep
      if (tempMap.size() > 0)
      {
         for (it = tempMap.begin(); it != tempMap.end(); it++)
         {
            // here we Support removing of dead particles via looping and culling by particle map in the final output loop
            int pindex = it->second;
            it2 = pos2Map.find(pindex);
            if (it2 == pos2Map.end())   // found the particle in the scene already
            {
               (*m_out_radiusArrays[step])[pindex] = 0.0;
            }
            else
            {
               int k = it2->second;
               MVector p3;
               // We need to get value from previous step. We do not know value of j in previous step
               p3 = (*positionArray2)[k];
            
               MVector result = (diff - 1) * diff * diff * p3;

               (*m_out_positionArrays[step])[pindex] += result;
               (*m_out_positionArrays[step])[pindex] *= 0.5;
            }

         }
      }

   }
}
///

void CParticleTranslator::GatherBlurSteps(AtNode* particle, unsigned int step)
{
   AiMsgDebug("[mtoa] Particle system %s exporting step %i", m_fnParticleSystem.partialPathName().asChar(), step);

   int numParticles = m_fnParticleSystem.count();

   MVectorArray   velocityArray;
   MVectorArray   accelerationArray;
   MIntArray      particleId;

   bool multipleRadiuses = true;//!(m_minPixelWidth > AI_EPSILON);

   MVectorArray*   positionArray = new MVectorArray;
   MDoubleArray*   radiusArray = multipleRadiuses ? new MDoubleArray : 0;
   MDoubleArray*   spriteScaleXPP = new MDoubleArray;
   MDoubleArray*   spriteScaleYPP = new MDoubleArray;

   GatherStandardPPData( MAnimControl::currentTime(),
                         positionArray,
                         radiusArray,
                         spriteScaleXPP,
                         spriteScaleYPP,
                         0,
                         0,
                         velocityArray,
                         accelerationArray,
                         particleId);

   // add in a new vector entry for this step to all the  maps/vectors
   // this is just making a copy of the last step's data and placing it onto the stack of the out*Arrays
   // these will be modified with the data from this step.
   // We want to do this here because  it gives us the simplest way to get the new steps arrays set up
   // and later if particles don't exist in this step, we already have a value in this index and we don't have to go thru
   // another set of loops to fill in the gaps  for particles that have died as well.

   int previousStep = step - 1;
   if (previousStep < 0)
   {
      for (size_t i = 0; i < m_exportedSteps.size(); ++i)
      {
         if (i == step) continue;
         if (m_exportedSteps[i])
         {
            previousStep = i;
            break;
         }
      }
   }
   if (previousStep < 0) return; // shouldn't happen

   MVectorArray   *newPositionArray = new MVectorArray((*m_out_positionArrays[previousStep]));
   m_out_positionArrays[step] = newPositionArray;

   MDoubleArray *newSSXArray = NULL;
   MDoubleArray *newSSYArray = NULL;
   MDoubleArray *newRadiusArray =NULL;

   if (m_isSprite)
   {
      newSSXArray = new MDoubleArray((*m_out_spriteScaleXArrays[previousStep]));
      newSSYArray = new MDoubleArray((*m_out_spriteScaleYArrays[previousStep]));
      m_out_spriteScaleXArrays[step] = newSSXArray;
      m_out_spriteScaleYArrays[step] = newSSYArray;
   }
   else if (multipleRadiuses)
   {
      newRadiusArray = new MDoubleArray((*m_out_radiusArrays[previousStep]));
      m_out_radiusArrays[step] = newRadiusArray;
   }

   particle = GetArnoldNode();

   MTime oneSec(1.0, MTime::kSeconds);
   // FIXME: was it intended to be rounded to int ?
   float fps =  (float)oneSec.asUnits(MTime::uiUnit());

   // now we iterate thru the particle list and change the data for this step.

   // Need to copy here since we need to keep particle ID map sacred.
   // tempMap is destroyed  little by little as we progress thru the particle list
   // to speed up the search for particles as we go.. then whatever is left over  in tempMap
   // gets iterated over  because they are "the particles that died" in this frameStep
   std::map <int, int> tempMap = m_particleIDMap;
   std::map <int, int>::iterator it;

   int newParticleCount = 0;
   for (int j = 0; j < numParticles; j++)
   {
      int partId = particleId[j];
      it = tempMap.find(partId);
      if (it != tempMap.end())   // found the particle in the scene already
      {
         int pindex = it->second;
         m_instantVeloArray[pindex] = velocityArray[j];

         (*newPositionArray)[pindex] = (*positionArray)[j];

         if (m_isSprite)
         {
            // FIXME: keep in mind there is no point in using "new" if we're indexing
            // and copying its contents over one by one.  if we can use spriteScaleXPP
            // as a starting point, it should be much more efficient
            // NOTE: this needs more thinking on the subject first, as it may introduce its own  overhead

            (*newSSXArray)[pindex] = (*spriteScaleXPP)[j];
            (*newSSYArray)[pindex] = (*spriteScaleYPP)[j];
         }
         else if(multipleRadiuses)
         {
            (*m_out_radiusArrays[step])[pindex] = (*radiusArray)[j];
         }
         // to speed up the  search, we remove the particles we've already found..
         tempMap.erase(it);
      }
      else   // new particle this step
      {
         newParticleCount ++;
         //add new particles to the  arrays

         m_particleIDMap[partId] = (*m_out_positionArrays[step]).length();

         // Because we are dealing with a new particle, we need to create all its past step data, so we loop thru steps
         // here  and fill in the gaps with the current frame's  data and only compute its  position from current velocity

         for (size_t k = 0; k < m_exportedSteps.size(); k++)
         {
            // the steps that haven't been processed yet
            // can be ignored as they'll be filled later
            if (!m_exportedSteps[k])
               continue;
            
            int s = (k-step);
            if (s != 0)
            {
               // this calculates the "Velocity substep" which is the delta position based on the particle's current velocity
               // and  the step we're on.
               MVector velocitySubstep = (((velocityArray[j]/fps)*GetMotionByFrame())/(GetNumMotionSteps()-1))*s;
               MVector oldVeloPosition = (*positionArray)[j] + velocitySubstep;
               (*m_out_positionArrays[k]).append(oldVeloPosition);
            }
            else
            {
               (*m_out_positionArrays[k]).append((*positionArray)[j]);
            }

            m_instantVeloArray.append(velocityArray[j]);

            if (m_isSprite)
            {
               if (m_isSpritePP)
               {
                  (*m_out_spriteScaleXArrays[k]).append((*spriteScaleXPP)[j]);
                  (*m_out_spriteScaleYArrays[k]).append((*spriteScaleYPP)[j]);
               }
               else
               {
                  (*m_out_spriteScaleXArrays[k]).append(m_spriteScaleX);
                  (*m_out_spriteScaleYArrays[k]).append(m_spriteScaleY);
               }
            }
            else if(multipleRadiuses)
            {
               (*m_out_radiusArrays[k]).append((*radiusArray)[j]);
            }
         }
      }
   }

   AiMsgDebug("[mtoa] Particle system %s export found %i new particles for step %i",
      m_fnParticleSystem.partialPathName().asChar(), newParticleCount, step);

   // if we still have entries in tempMap, that means the particle died in this frameStep and we need to update
   // the value for position based on velocity only.. all other attributes, since they were copied over at the beginning
   // can just stay the same. This  seems faster than iterating thru every  particle attribute  again here...
   if (tempMap.size() > 0)
   {
      AiMsgDebug("[mtoa] Particle system %s export found %i particles that died for step %i, computing velocity...",
         m_fnParticleSystem.partialPathName().asChar(), (int)tempMap.size(), step);

      for (it = tempMap.begin(); it != tempMap.end(); it++)
      {
         // here we Support removing of dead particles via looping and culling by particle map in the final output loop
         if (m_deleteDeadParticles)
         {
            m_particleIDMap[it->first] = -1;
         }
         else
         {
            // get last step's position
            MVector velocitySubstep = (((m_instantVeloArray[it->second]/fps)*GetMotionByFrame())/(GetNumMotionSteps()-1)) * (step - previousStep);
            MVector newVeloPosition = (*m_out_positionArrays[previousStep])[it->second] + velocitySubstep;
            (*newPositionArray)[it->second] = newVeloPosition;
         }
      }
   }
}

/// OR... a slightly speedier approach.
/// instead of actually gathering the values from maya per step, we just use the first steps  values
// and last steps velocity and compute  the new position.
// we just copy the rest of the data via pointers as it doesn't need to change per frameStep.
// There is an extra attribute  called  "aiInterpolateBlurSteps"  that defines whether to use this or the above
// function to populate the motion blur steps

void CParticleTranslator::InterpolateBlurSteps(AtNode* particle, unsigned int step)
{
   AiMsgDebug("[mtoa] Particle system %s interpolating step %i", m_fnParticleSystem.partialPathName().asChar(), step);

   MTime oneSec(1.0, MTime::kSeconds);
   // FIXME: was it intended to be rounded to int ?
   float fps =  (float)oneSec.asUnits(MTime::uiUnit());
   //uint totalSteps = GetNumMotionSteps(); // not needed

   particle = GetArnoldNode();

   int previousStep = step - 1;
   if (previousStep < 0)
   {
      for (size_t i = 0; i < m_exportedSteps.size(); ++i)
      {
         if (i == step) continue;
         if (m_exportedSteps[i])
         {
            previousStep = i;
            break;
         }
      }
   }
   if (previousStep < 0) return; // shouldn't happen

   // add in a new vector entry for this step to all the  maps/vectors
   // this is just making a copy of the last steps data and placing it onto the stack of the out*Arrays
   // these will be modified with the data from this step

   MVectorArray   *newPositionArray = new MVectorArray((*m_out_positionArrays[previousStep]));

   const float evaluateEvery = FindMayaPlug("aiEvaluateEvery").asFloat();


   if (m_isSprite)
   {
      MDoubleArray *newSSXArray = new MDoubleArray((*m_out_spriteScaleXArrays[previousStep]));
      MDoubleArray *newSSYArray = new MDoubleArray((*m_out_spriteScaleYArrays[previousStep]));
      m_out_spriteScaleXArrays[step] = newSSXArray;
      m_out_spriteScaleYArrays[step] = newSSYArray;
   }

   
   //////////////////////////////////////////

   MVectorArray   velocityArray1;
   MVectorArray   accelerationArray1;
   MIntArray     particleId1;

   MVectorArray*   positionArray1 = new MVectorArray;
   MDoubleArray*   radiusArray1 = new MDoubleArray;
   MDoubleArray*   spriteScaleXPP1 = new MDoubleArray;
   MDoubleArray*   spriteScaleYPP1 = new MDoubleArray;

   MStatus status;

   AiMsgDebug("[mtoa] Particle system %s exporting step 0", m_fnParticleSystem.partialPathName().asChar());

   MTime curTime = MAnimControl::currentTime();

   /*double fra1 = curTime.as(MTime::uiUnit());
   double val = curTime.value();

   if(floor(fra1) != fra1)
   {
      double diff = fra1 - floor(fra1);
      MTime timeDiff(diff, MTime::uiUnit());
      MTime finalTime = curTime - diff;
      double fra2 = finalTime.as(MTime::uiUnit());
   }*/

   GatherStandardPPData( curTime,
                           positionArray1,
                           radiusArray1,
                           spriteScaleXPP1,
                           spriteScaleYPP1,
                           0,
                           0,
                           velocityArray1,
                           accelerationArray1,
                           particleId1);

   //m_instantAcceArray = accelerationArray;

   //////////////////////////////////////////

   int numParticles = m_fnParticleSystem.count();

   std::map <int, int> tempMap = m_particleIDMap;
   std::map <int, int>::iterator it;

   MDoubleArray *newRadiusArray =NULL;
   newRadiusArray = new MDoubleArray((*m_out_radiusArrays[previousStep]));
   m_out_radiusArrays[step] = newRadiusArray;

   float fra1 = (float) curTime.as(MTime::uiUnit());
   float diff = 0.0;

   for (int j = 0; j < numParticles; j++)
   {
      int partId = particleId1[j];
      it = tempMap.find(partId);
      if (it != tempMap.end())   // found the particle in the scene already
      {
         int pindex = it->second;

         (*newPositionArray)[pindex] = (*positionArray1)[j];
         (*m_out_radiusArrays[step])[pindex] = (*radiusArray1)[j];

         // to speed up the  search, we remove the particles we've already found..
         tempMap.erase(it);
      }
      else
      {
          //add new particles to the  arrays
         m_particleIDMap[partId] = (*m_out_positionArrays[previousStep]).length();

         // Because we are dealing with a new particle, we need to create all its past step data, so we loop thru steps
         // here  and fill in the gaps with the current frame's  data and only compute its  position from current velocity
         for (size_t k = 0; k < m_exportedSteps.size(); k++)
         {
            if (!m_exportedSteps[k])
               continue;

            // add particle to this steps arrays
            if (k == step)
            {
               (*newPositionArray).append((*positionArray1)[j]);
            }
            else
            {
               (*m_out_positionArrays[k]).append((*positionArray1)[j]);
            }

            if ((k == step) || ((fmod(fra1, evaluateEvery) != 0.0f) &&(k == step - 1))) //Not if it is a cache frame
            {
               (*m_out_radiusArrays[k]).append((*radiusArray1)[j]);
            }
            else
            {
               (*m_out_radiusArrays[k]).append(0.0);
            }
         }
      }
   }

   // if we still have entries in tempMap, that means the particle died in this frameStep
   if (tempMap.size() > 0)
   {
      for (it = tempMap.begin(); it != tempMap.end(); it++)
      {
         int pindex = it->second;
         (*newPositionArray)[pindex] = (*m_out_positionArrays[previousStep])[pindex];
         (*m_out_radiusArrays[step])[pindex] = 0.0;
      }
   }

   MTime finalTime;

   tempMap = m_particleIDMap;

   // If this is not a cache sample. Interpolate
   if(fmod(fra1, evaluateEvery) != 0.0f)
   {
      MTime oneSec(1.0, MTime::kSeconds);
      double dt = (1/fps);

      diff = fmod(fra1, evaluateEvery);
      MTime timeDiff(evaluateEvery - diff, MTime::uiUnit());
      diff /= evaluateEvery; //Normalize diff in the 0-1 interval
      finalTime = curTime + timeDiff;

      for (int j = 0; j < numParticles; j++)
      {
         int partId = particleId1[j];
         it = tempMap.find(partId);
         if (it != tempMap.end())   // found the particle in the scene already
         {
            int pindex = it->second;

            MVector p0, p1;
            p0 = (*positionArray1)[j] - velocityArray1[j]*dt*evaluateEvery;
            p1 = (*positionArray1)[j];
            
            MVector result = diff*((2-diff)*diff - 1)*p0;
            result += (diff*diff*(3*diff - 5) + 2)*p1;

            (*newPositionArray)[pindex] = result;
         }
      }

      MVectorArray   velocityArray2;
      MVectorArray   accelerationArray2;
      MIntArray      particleId2;

      MVectorArray*   positionArray2 = new MVectorArray;
      MDoubleArray*   radiusArray2 = new MDoubleArray;
      MDoubleArray*   spriteScaleXPP2 = new MDoubleArray;
      MDoubleArray*   spriteScaleYPP2 = new MDoubleArray;

      GatherStandardPPData( finalTime,
                              positionArray2,
                              radiusArray2,
                              spriteScaleXPP2,
                              spriteScaleYPP2,
                              0,
                              0,
                              velocityArray2,
                              accelerationArray2,
                              particleId2);

      numParticles = m_fnParticleSystem.count();

      tempMap = m_particleIDMap;

      std::map <int, int> pos2Map;
      std::map <int, int>::iterator it2;

      for (int j = 0; j < numParticles; j++)
      {
         int partId = particleId2[j];
         it = tempMap.find(partId);
         if (it != tempMap.end())   // found the particle in the scene already
         {
            int pindex = it->second;
            // We need to keep p2 value in case wee need to reuse it in next step because particle dies
            pos2Map[pindex] = j;

            MVector p2;
            p2 = (*positionArray2)[j];
            
            MVector result = diff*((4 - 3*diff)*diff + 1) * p2;

            (*newPositionArray)[pindex] += result;

            // to speed up the  search, we remove the particles we've already found..
            tempMap.erase(it);
         }
      }
      // if we still have entries in tempMap, that means the particle died in this frameStep
      if (tempMap.size() > 0)
      {
         for (it = tempMap.begin(); it != tempMap.end(); it++)
         {
            int pindex = it->second;
            (*newPositionArray)[pindex] = (*m_out_positionArrays[previousStep])[pindex];
            (*m_out_radiusArrays[step])[pindex] = 0.0;
         }
      }

      MTime cacheInterval(evaluateEvery, MTime::uiUnit());
      finalTime += cacheInterval;

      MVectorArray   velocityArray3;
      MVectorArray   accelerationArray3;
      MIntArray      particleId3;

      MVectorArray*   positionArray3 = new MVectorArray;
      MDoubleArray*   radiusArray3 = new MDoubleArray;
      MDoubleArray*   spriteScaleXPP3 = new MDoubleArray;
      MDoubleArray*   spriteScaleYPP3 = new MDoubleArray;

      GatherStandardPPData( finalTime,
                              positionArray3,
                              radiusArray3,
                              spriteScaleXPP3,
                              spriteScaleYPP3,
                              0,
                              0,
                              velocityArray3,
                              accelerationArray3,
                              particleId3);

      numParticles = m_fnParticleSystem.count();

      m_fnParticleSystem.evaluateDynamics(curTime, false);

      tempMap = m_particleIDMap;

      for (int j = 0; j < numParticles; j++)
      {
         int partId = particleId3[j];
         it = tempMap.find(partId);
         if (it != tempMap.end())   // found the particle in the scene already
         {
            int pindex = it->second;

            MVector p3;
            p3 = (*positionArray3)[j];
            
            MVector result = (diff - 1) * diff * diff * p3;

            (*newPositionArray)[pindex] += result;
            (*newPositionArray)[pindex] *= 0.5;

            // to speed up the  search, we remove the particles we've already found..
            tempMap.erase(it);
         }
      }
      // if we still have entries in tempMap, that means the particle died in this frameStep
      if (tempMap.size() > 0)
      {
         // In this case, we only have to set radius to 0 if the particle died in the previous caches.
         // If the particle dies in this cache. Set position to previous one and calculate position as normal.
         for (it = tempMap.begin(); it != tempMap.end(); it++)
         {
            int pindex = it->second;
            it2 = pos2Map.find(pindex);
            if (it2 == pos2Map.end())   // found the particle in the scene already
            {
               (*newPositionArray)[pindex] = (*m_out_positionArrays[previousStep])[pindex];
               (*m_out_radiusArrays[step])[pindex] = 0.0;
            }
            else
            {
               int k = it2->second;
               MVector p3;
               // We need to get value from previous step. We do not know value of j in previous step
               p3 = (*positionArray2)[k];
            
               MVector result = (diff - 1) * diff * diff * p3;

               (*newPositionArray)[pindex] += result;
               (*newPositionArray)[pindex] *= 0.5;
            }
         }
      }
   }


   m_out_positionArrays[step] =newPositionArray;
}


///  writing out the particle data

void CParticleTranslator::WriteOutParticle(AtNode* particle)
{

   AtArray* a_positionArray(NULL);
   AtArray* a_ParticleIdArray(NULL);
   AtArray* a_ParticleMultiIndexArray(NULL);
   AtArray* a_radiusArray(NULL);
   AtArray* a_aspectArray(NULL);
   AtArray* a_rgbPPArray(NULL);
   AtArray* a_opacityPPArray(NULL);

   MVector m_rgb;
   AtRGB a_rgb;

   float minRadius   = m_fnParticleSystem.findPlug("aiMinParticleRadius").asFloat();
   float maxRadius   = m_fnParticleSystem.findPlug("aiMaxParticleRadius").asFloat();
   float radiusMult  = m_fnParticleSystem.findPlug("aiRadiusMultiplier").asFloat();

   m_particleCount = (*m_out_positionArrays[0]).length();

   AiMsgDebug("[mtoa] Particle system %s count: %i", m_fnParticleSystem.partialPathName().asChar(), m_particleCount);

   /// Finally set the  position and  radius/aspect values with their cache values minus Transform position
   AtPoint a_v;
   MVector m_v;
   float a_r;
   float a_a;
   //getting the matrix of the point particle object

   AtMatrix inclMatrix;
   if (m_inheritCacheTxfm)
   {
      MMatrix mpm;
      mpm = m_dagPath.inclusiveMatrix();
      // convert it to AtMatrix
      ConvertMatrix (inclMatrix, mpm); // util From CNodeTranslator
   }

   const unsigned int numMotionSteps = (m_motionDeform && RequiresMotionData()) ? GetNumMotionSteps() : 1;

   // declare the arrays  now that we have gathered all the particle info from each step
   a_positionArray = AiArrayAllocate(m_particleCount*m_multiCount, numMotionSteps, AI_TYPE_POINT);
   bool multipleRadiuses = /*(!(m_minPixelWidth > AI_EPSILON)) &&*/ (m_out_radiusArrays.size() > 1);
   if (multipleRadiuses)
      a_radiusArray = AiArrayAllocate(m_particleCount * m_multiCount, numMotionSteps, AI_TYPE_FLOAT);
   else
   {
      a_radiusArray = AiArrayAllocate(m_particleCount * m_multiCount, 1, AI_TYPE_FLOAT);
      if ((m_minPixelWidth > AI_EPSILON) && (m_out_radiusArrays.size() > 1))
         AiMsgWarning("[mtoa] Only one motion key is exported for particle radius because min pixel widht is bigger than 0 on %s", m_fnParticleSystem.name().asChar());
   }

   if (m_isSprite)
      a_aspectArray = AiArrayAllocate(m_particleCount*m_multiCount, numMotionSteps, AI_TYPE_FLOAT);

   if (m_exportId)
      a_ParticleIdArray = AiArrayAllocate(m_particleCount*m_multiCount, numMotionSteps, AI_TYPE_INT);

   if (m_doMultiPoint) // multiPoint index
      a_ParticleMultiIndexArray = AiArrayAllocate(m_particleCount*m_multiCount, numMotionSteps, AI_TYPE_INT);

   if (m_hasRGB)
      a_rgbPPArray = AiArrayAllocate(m_particleCount*m_multiCount, 1,  AI_TYPE_RGB);
   
   if(m_hasOpacity)
      a_opacityPPArray = AiArrayAllocate(m_particleCount*m_multiCount, 1, AI_TYPE_FLOAT);
   
   std::map <int, int>::iterator it;
   
   for (unsigned int s = 0; s < numMotionSteps; s++)
   {
      bool writeRadius = false;
      if ((s == 0) || (multipleRadiuses && (s > 0)))
         writeRadius = true;
      int i = 0;
      for (it = m_particleIDMap.begin(); it != m_particleIDMap.end();  it++)
      {
         int pindex = it->second;
         if (pindex >= 0)
         {
            for (int j = 0; j< m_multiCount; j++)
            {

               m_v = (*m_out_positionArrays[s])[pindex];

               AtPoint noisePoint;
               noisePoint.x = float(i+j+0.1454329);
               noisePoint.y = float(i+j+0.3234548);
               noisePoint.z = float(i+j+0.0921081);

               a_v.x = float(m_v.x+(AiPerlin4(noisePoint,i+j+0.2340970f)*m_multiRadius));
               a_v.y = float(m_v.y+(AiPerlin4(noisePoint,i+j+23.1203093f)*m_multiRadius));
               a_v.z = float(m_v.z+(AiPerlin4(noisePoint,i+j-0.4874771f)*m_multiRadius));

               // MFnParticleSystem::position returns values in different spaces depending on whether
               // the system is cached or not. As a result, we need to apply the inclusive matrix if the
               // particle system has been cached.
               if (m_inheritCacheTxfm)
                  AiM4PointByMatrixMult (&a_v, inclMatrix, &a_v);

               // Calculated offset index
               int  index =  s*(m_particleCount*m_multiCount) +i*m_multiCount+j;

               AiArraySetPnt(a_positionArray, index, a_v);

               if (m_isSprite)
               {
                  a_a = float((*m_out_spriteScaleXArrays[s])[pindex]/(*m_out_spriteScaleYArrays[s])[pindex]);
                  AiArraySetFlt (a_aspectArray, index, a_a);
                  if (writeRadius)
                  {
                     a_r = CLAMP((((float)(*m_out_spriteScaleXArrays[s])[pindex])/2), minRadius, maxRadius);
                     a_r *= radiusMult;
                     AiArraySetFlt(a_radiusArray, index, a_r);
                  }
               }
               else if (writeRadius)
               {
                  a_r = CLAMP(((float)(*m_out_radiusArrays[s])[pindex]), minRadius, maxRadius);
                  a_r *= radiusMult;
                  AiArraySetFlt(a_radiusArray, index, a_r);
               }                  

               if (m_exportId)
                  AiArraySetInt(a_ParticleIdArray, index, (int)it->first);
               if (m_doMultiPoint)
                  AiArraySetInt(a_ParticleMultiIndexArray, index, (int)j);
            }// end  multicount

         }// if particle is valid (did not die within motion blur steps)

         i++;  // tracking the  iteration thru the map

      }// end m_particleIDMap  iteration

      // CLEAN UP MEMORY
      AiMsgDebug("[mtoa] Particle system %s export cleaning up memory.", m_fnParticleSystem.partialPathName().asChar());
      delete m_out_positionArrays[s];
      if (m_isSprite)
      {
         delete m_out_spriteScaleXArrays[s];
         delete m_out_spriteScaleYArrays[s];
      }
      else if (writeRadius)
      {
         delete m_out_radiusArrays[s];
      }
   }// end  numMotionSteps

   int i = 0;
   int rgbLength = 0;
   if (m_hasRGB)
      rgbLength = (int)m_out_colorArray.length();
   int opacityLength = 0;
   if (m_hasOpacity)
      opacityLength = (int)m_out_opacityArray.length();
   for (it = m_particleIDMap.begin(); it != m_particleIDMap.end();  it++)
   {
      const int pindex = it->second;
      
      if (pindex >= 0)
      {
         a_rgb = AI_RGB_BLACK;
         if (m_hasRGB && (pindex < rgbLength))
         {
            m_rgb = m_out_colorArray[pindex];
            a_rgb.r = (float)m_rgb.x;
            a_rgb.g = (float)m_rgb.y;
            a_rgb.b = (float)m_rgb.z;
         }
         float opacity = 0.0f;
         if (m_hasOpacity && (pindex < opacityLength))
            opacity = (float)m_out_opacityArray[pindex];
         for (int j = 0; j< m_multiCount; j++)
         {
            const int index = i * m_multiCount + j;
            if (m_hasRGB)
               AiArraySetRGB(a_rgbPPArray, (AtUInt32)index, a_rgb);
            if (m_hasOpacity)
               AiArraySetFlt(a_opacityPPArray, (AtUInt32)index, opacity);
         }// end  multicount

      }// if particle is valid (did not die within motion blur steps)

      i++;  // tracking the  iteration thru the map
   }// end m_particleIDMap  iteration

   //write the points
   AiNodeSetArray(particle, "points", a_positionArray);

   //write the radius
   AiNodeSetArray(particle, "radius", a_radiusArray);

   if (m_isSprite)
      AiNodeSetArray(particle, "aspect", a_aspectArray);

   if (m_exportId)
      AiNodeSetArray(particle, "particleId", a_ParticleIdArray);

   if (m_doMultiPoint)
      AiNodeSetArray(particle, "particleMultiIndex", a_ParticleMultiIndexArray);

   if(m_hasRGB)
      AiNodeSetArray(particle, "rgbPP", a_rgbPPArray);
   if (m_hasOpacity)
      AiNodeSetArray(particle, "opacityPP", a_opacityPPArray);

   if (m_doExtraAttributes)
   {
      std::map <std::string, MVectorArray*>::iterator vecIt;
      std::map <std::string, MDoubleArray*>::iterator doubleIt;
      std::map <std::string, MIntArray*>::iterator intIt;

      // Extra export attrs
      for (doubleIt = m_out_customDoubleAttrArrays.begin(); doubleIt != m_out_customDoubleAttrArrays.end(); doubleIt++)
      {
         AtArray* a_attributes = AiArrayAllocate(m_particleCount*m_multiCount, 1, AI_TYPE_FLOAT);
         for (int i = 0; i < (int)a_attributes->nelements; ++i)
            AiArraySetFlt(a_attributes, i, 0.0f);
         int inputCount = (int)doubleIt->second->length();
         int i = 0;
         for (it = m_particleIDMap.begin(); it != m_particleIDMap.end();  it++)
         {
            const int pindex = it->second;
            if ((pindex >= 0) && (pindex < inputCount))
            {
               const float floatValue = (float)(doubleIt->second->operator[](pindex));
               for (int j = 0; j< m_multiCount; j++)
               {
                  // Calculated offset index
                  int index =  i * m_multiCount + j;
                  AiArraySetFlt(a_attributes, index, floatValue);
               }
            }
            i++;
         }
         // memory cleanup
         delete doubleIt->second;
         AiNodeDeclare(particle, doubleIt->first.c_str(), "uniform FLOAT");
         AiNodeSetArray(particle, doubleIt->first.c_str(), a_attributes);

      }
      for (vecIt = m_out_customVectorAttrArrays.begin(); vecIt != m_out_customVectorAttrArrays.end(); vecIt++)
      {
         AtArray* a_attributes = AiArrayAllocate(m_particleCount * m_multiCount, 1, AI_TYPE_VECTOR);
         for (int i = 0; i < (int)a_attributes->nelements; ++i)
            AiArraySetVec(a_attributes, i, AI_V3_ZERO);
         const int inputCount = (int)vecIt->second->length();
         int i = 0;
         for (it = m_particleIDMap.begin(); it != m_particleIDMap.end();  it++)
         {
            const int pindex = it->second;
            if ((pindex >= 0) && (pindex < inputCount))
            {
               const MVector vectorValue = vecIt->second->operator[](pindex);
               AtVector a_attr = {(float)vectorValue.x, (float)vectorValue.y, (float)vectorValue.z};
               for (int j = 0; j< m_multiCount; j++)
               {
                  // Calculated offset index
                  int  index = i * m_multiCount + j;
                  AiArraySetVec(a_attributes, index, a_attr);
               }
            }
            i++;
         }
         // memory cleanup
         //std::cout << "cleaning up extra Vector attr memory " << vecIt->second[s] << std::endl;
         delete vecIt->second;
         AiNodeDeclare(particle, vecIt->first.c_str(), "uniform VECTOR");
         AiNodeSetArray(particle, vecIt->first.c_str(), a_attributes);

      }
      for(intIt = m_out_customIntAttrArrays.begin(); intIt != m_out_customIntAttrArrays.end(); intIt++)
      {
         AtArray* a_attributes = AiArrayAllocate(m_particleCount * m_multiCount, 1, AI_TYPE_INT);
         for (int i = 0; i < (int)a_attributes->nelements; ++i)
            AiArraySetInt(a_attributes, i, 0);
         const int inputCount = (int)intIt->second->length();
         int i = 0;
         for (it = m_particleIDMap.begin(); it != m_particleIDMap.end();  it++)
         {
            const int pindex = it->second;
            if ((pindex >= 0) && (pindex < inputCount))
            {
               const int intValue = intIt->second->operator[](i);
               for (int j = 0; j < m_multiCount; j++)
               {
                  // Calculated offset index
                  int index = i * m_multiCount + j;
                  AiArraySetInt(a_attributes, index, intValue);
               }
            }
            i++;
         }
         // memory cleanup
         //std::cout << "cleaning up extra Int attr memory " << vecIt->second[s] << std::endl;
         delete vecIt->second;
         AiNodeDeclare(particle, intIt->first.c_str(), "uniform INT");
         AiNodeSetArray(particle, intIt->first.c_str(), a_attributes);
      }
   }
}

/// determine if the particles are cached
bool CParticleTranslator::IsCached()
{
   // right now this is kind of a hack, as it assumes that if the dynGlobals is set
   // that there must be a particle cache for this  object which may not always be the case.
   // however, there is no alternative for determining if a particle is cached.

   // FIXME: some of this may end up having to change once we have per node frame-step in place;
   MStatus stat;
   MSelectionList list;
   list.add("dynGlobals1");
   MObject dynGlobals;
   list.getDependNode(0, dynGlobals);
   MFnDependencyNode dynGlobalsNode(dynGlobals, &stat);
   return (stat == MS::kSuccess && dynGlobalsNode.findPlug("useParticleDiskCache").asBool());
}

bool CParticleTranslator::IsNParticle()
{
   return false;
}

void CParticleTranslator::GatherStandardPPData( MTime           curTime,
                                                MVectorArray*   positionArray ,
                                                MDoubleArray*   radiusArray ,
                                                MDoubleArray*   spriteScaleXPP ,
                                                MDoubleArray*   spriteScaleYPP ,
                                                MVectorArray*   rgbArray ,
                                                MDoubleArray*   opacityArray,
                                                MVectorArray    &velocityArray,
                                                MVectorArray    &accelerationArray,
                                                MIntArray       &particleId)
{
   // cached particles need special treatment
   if (IsCached())
   {
      // force an evaluation of particles (don't runupFromStart).
      // Good to do in all cases, but particularly important when motion blur
      // is enabled so that values are correct on other steps
      m_fnParticleSystem.evaluateDynamics(curTime, false);
      // We are geting particles World coordinates, so we do not need this
      //m_inheritCacheTxfm = true; // defaults to false
   }
   
   // It is needed to call setObject() again so that particles will return world position
   m_fnParticleSystem.setObject(m_dagPath.node());

   uint numParticles = m_fnParticleSystem.count();

   if (m_hasRGB && (rgbArray != 0))
      m_fnParticleSystem.rgb(*rgbArray);
   if(m_hasOpacity && (opacityArray != 0))
      m_fnParticleSystem.opacity(*opacityArray);

   if (m_isSprite)
   {
      if (m_isSpritePP)
      {
         m_fnParticleSystem.getPerParticleAttribute(MString("spriteScaleXPP"), *spriteScaleXPP);
         m_fnParticleSystem.getPerParticleAttribute(MString("spriteScaleYPP"), *spriteScaleYPP);
      }
      else
      {
         for (uint i=0; i < numParticles; i++)
         {
            (*spriteScaleXPP).append(m_spriteScaleX);
            (*spriteScaleYPP).append(m_spriteScaleY);
         }
      }
   }

   if (radiusArray)
   {
      if (m_hasRadiusPP )
         m_fnParticleSystem.getPerParticleAttribute(MString("radiusPP"), *radiusArray);
      else
      {
         for (uint i=0; i < numParticles; i++)
            radiusArray->append(m_particleSize);
      }
   }

   m_fnParticleSystem.position(*positionArray);
   //m_fnParticleSystem.getPerParticleAttribute(MString("worldPosition"),*positionArray);
   m_fnParticleSystem.velocity(velocityArray);
   m_fnParticleSystem.acceleration(accelerationArray);
   MDoubleArray tempDoubleParticleId;
   m_fnParticleSystem.getPerParticleAttribute(MString("particleId"), tempDoubleParticleId);
   unsigned int particleIdCount = tempDoubleParticleId.length();
   particleId.setLength(particleIdCount);
   for (unsigned int i = 0; i < particleIdCount; ++i)
      particleId[i] = static_cast<int>(tempDoubleParticleId[i]);
}


AtNode* CParticleTranslator::ExportInstance(AtNode *instance, const MDagPath& masterInstance)
{

   AtNode* masterNode = AiNodeLookUpByName(CDagTranslator::GetArnoldNaming(masterInstance).asChar());

   int instanceNum =  m_dagPath.instanceNumber();

   AiNodeSetStr(instance, "name", CDagTranslator::GetArnoldNaming(m_dagPath).asChar());

   ExportMatrix(instance);

   AiNodeSetPtr(instance, "node", masterNode);
   AiNodeSetBool(instance, "inherit_xform", false);

   //
   // SHADERS
   //

   MObject sgNode;
   MObject sgNodeMaster;
   MObject node = m_dagPath.node();
   MFnDependencyNode nodeFn;
   MFnDependencyNode dsgNode;
   MFnDependencyNode dsgNodeMaster;

   nodeFn.setObject(node);
   MObject iogAttr = nodeFn.attribute("instObjGroups");

   MPlug iogPlug(node, iogAttr);
   MPlug iogPlugMaster(node, iogAttr);
   MPlugArray iogConnections;
   MPlugArray iogConnectionsMaster;

   iogPlug.elementByLogicalIndex(instanceNum).connectedTo(iogConnections, false, true);

   // FIXME: it is incorrect to assume that instance 0 is the master as it may be hidden (chad)
   iogPlugMaster.elementByLogicalIndex(0).connectedTo(iogConnectionsMaster, false, true);

   if (iogConnectionsMaster[0].node() != iogConnections[0].node())
   {
      //FIXME : Is it ok to assume that the shader is the first Dag member ?
      AtNode *shader = ExportConnectedNode(iogConnections[0]);
      AiNodeSetPtr(instance, "shader", shader);
   }

   return instance;
}


AtNode* CParticleTranslator::ExportParticleNode(AtNode* particle, unsigned int step)
{
   if (!IsExportingMotion())
   {
      if (RequiresShaderExport())
         ExportParticleShaders(particle);
      ExportPreambleData(particle);
      GatherFirstStep(particle);
      ProcessParameter(particle, "step_size", AI_TYPE_FLOAT, "aiStepSize");
   }
   else
   {
      if ((m_fnParticleSystem.findPlug("aiInterpolateBlur").asBool()) && IsNParticle())
         InterpolateBlurSteps(particle, step); // compute all the data from  the first steps  population
      else
         GatherBlurSteps(particle, step); // gather the data from each step
   }

   /// write out final data
   bool missingStep = false;
   for (size_t i = 0; i < m_exportedSteps.size(); ++i)
   {
      if (!m_exportedSteps[i])
      {
         missingStep = true;
         break;
      }
   }
   if (!missingStep)
      WriteOutParticle(particle);
   /// visibility flags
   ProcessRenderFlags(particle);

   return particle;
}

void CParticleTranslator::Export(AtNode* anode)
{
   if (IsExported())
   {
      // During Updates we only re-exported the render flags
      // make sure it's really what we want !
      ProcessRenderFlags(anode);
      ExportMatrix(anode);

      // shouldn't we re-export the shaders ?
      return;
   }

   // check if the particle system is linked to an instancer
   m_fnParticleSystem.setObject(m_dagPath.node());
   if (m_fnParticleSystem.isValid() == false)
   {
      AiMsgError("[mtoa]: Particle system %s not exported. It has 0 particles", m_fnParticleSystem.partialPathName().asChar());
      return;
   }

   if (IsMasterInstance())
   {
      // reset the list of processed steps.
      // Once they're all processed, we can write out the particles
      m_exportedSteps.clear();
      m_exportedSteps.assign(GetNumMotionSteps(), false);
      int step = GetMotionStep();
      m_exportedSteps[step]=true;
      ExportMatrix(anode);

      ExportParticleNode(anode, step);

      //
      if (!RequiresMotionData() && (m_exportedSteps.size() > 1))
      {
         // Motion blur is enabled, but this particle node is not motion blurred
         // so we can write out the particle now
         m_exportedSteps.assign(m_exportedSteps.size(), true);

         WriteOutParticle(anode);
         ProcessRenderFlags(anode);
      }
   }

   else
      ExportInstance(anode, GetMasterInstance());

}

void CParticleTranslator::ExportMotion(AtNode* anode)
{
   if (IsExported())
   {
      // FIXME: This used to crash with IPR & motion blurred particles (#691)
      // so let's check if this is obsolete now...we do need to exported the motion matrix
      ExportMatrix(anode);
      return;
   }

   int step = GetMotionStep();
   if (IsMasterInstance())
   {
      m_exportedSteps[step] = true;
      // matrix export used to be commented. Check if it works fine now
      ExportMatrix(anode);
      if (m_motionDeform)
         ExportParticleNode(anode, step);
      else
      {
         for (size_t i = 0; i < m_exportedSteps.size(); ++i)
         {
            // one of the motion steps is still missing
            if (!m_exportedSteps[i])
            return;
         }

         // all steps have been processed, it's time to write out
         // the particles
         WriteOutParticle(anode);
         ProcessRenderFlags(anode);
      }
   }
   else
      ExportMatrix(anode);
}

