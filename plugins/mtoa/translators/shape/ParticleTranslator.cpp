
#include "ParticleTranslator.h"
#include "render/RenderSession.h"
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

#include <ai_msg.h>
#include <ai_nodes.h>

AtNode* CParticleTranslator::CreateArnoldNodes()
{
   m_isMasterDag = IsMasterInstance(m_masterDag);
   if (m_isMasterDag)
      return  AddArnoldNode("points");
   else
      return  AddArnoldNode("ginstance");
}

void CParticleTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "points");

   // node attributes
   CShapeTranslator::MakeCommonAttributes(helper);

   CAttrData data;

   data.defaultValue.BOOL = false;
   data.name = "export_particleId";
   data.shortName = "aiexppartid";
   helper.MakeInputBoolean(data);

   data.defaultValue.STR = "";
   data.name = "export_attributes";
   data.shortName = "aiexpartattr";
   helper.MakeInputString(data);

   MStringArray  enumNames;
   enumNames.append("points");
   enumNames.append("spheres");
   enumNames.append("quads");
   //data.defaultValue.ENUM = 0; /// how do you do a default in an enum?
   data.name = "render_points_as";
   data.shortName = "renderPointsAs";
   data.enums= enumNames;
   helper.MakeInputEnum(data);

   data.defaultValue.FLT = 0;
   data.name = "min_particle_radius";
   data.shortName = "minParticleRadius";
   helper.MakeInputFloat(data);

   data.defaultValue.FLT = 1.0;
   data.name = "radiusMultiplier";
   data.shortName = "radiusMult";
   helper.MakeInputFloat(data);

   data.defaultValue.FLT = 1000000;
   data.name = "max_particle_radius";
   data.shortName = "maxParticleRadius";
   helper.MakeInputFloat(data);

   data.defaultValue.FLT = 0;
   data.name = "min_pixel_width";
   data.shortName = "minPixelWidth";
   helper.MakeInputFloat(data);

   data.defaultValue.BOOL = false;
   data.name = "deleteDeadParticles";
   data.shortName = "delDead";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = false;
   data.name = "inheritCacheTransform";
   data.shortName = "inhCTx";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = false;
   data.name = "computeInstantBlurSteps";
   data.shortName = "compInstBlrSteps";
   helper.MakeInputBoolean(data);

}

void CParticleTranslator::UpdateMotion(AtNode* anode, AtUInt step)
{
   ExportMatrix(anode, step);
}

void CParticleTranslator::ExportParticleShaders(AtNode* particle)
{
   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   std::vector<AtNode*> meshShaders;

   MObject shadingGroup = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroup.isNull())
   {
      MPlugArray        connections;
      MFnDependencyNode fnDGNode(shadingGroup);
      MPlug shaderPlug = fnDGNode.findPlug("surfaceShader");
      shaderPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         // shader assigned to node
         AtNode* shader = ExportNode(connections[0]);

         AiNodeSetPtr(particle, "shader", shader);
         meshShaders.push_back(shader);
      }
      else
         AiMsgWarning("[mtoa] ShadingGroup %s has no surfaceShader input", fnDGNode.name().asChar());
   }

}

/// parse the m_customAttrs and populate the step vectors for each type of custom export attrs.
/// called from within ExportParticleData
void CParticleTranslator::ExportCustomParticleData(AtNode* particle, AtUInt step)
{
   MStringArray attrs;

   MStatus status = m_customAttrs.split(';', attrs);
   if (status ==  MS::kSuccess)
   {
      for (unsigned int i=0; i < attrs.length(); i++)
      {

         MString currentAttr = attrs[i];
         m_fnParticleSystem.findPlug(currentAttr, &status);
         if (status != MS::kSuccess)
            continue;

         //check the type of the plug
         if (m_fnParticleSystem.isPerParticleDoubleAttribute(currentAttr))
         {
            MDoubleArray*  attributes = new MDoubleArray;
            m_fnParticleSystem.getPerParticleAttribute(currentAttr, *attributes);
            m_out_customDoubleAttrArrays[currentAttr.asChar()].push_back(attributes);

            continue;
         }
         else if (m_fnParticleSystem.isPerParticleVectorAttribute(currentAttr))
         {
            MVectorArray*  attributes = new MVectorArray;
            m_fnParticleSystem.getPerParticleAttribute(currentAttr, *attributes);
            m_out_customVectorAttrArrays[currentAttr.asChar()].push_back(attributes);

            continue;
         }
         else if (m_fnParticleSystem.isPerParticleIntAttribute(currentAttr))
         {
            MIntArray*  attributes = new MIntArray;
            m_fnParticleSystem.getPerParticleAttribute(currentAttr, *attributes);
            m_out_customIntAttrArrays[currentAttr.asChar()].push_back(attributes);

            continue;
         }
      }
   }

}

/// ExportPreambleData: sets up most of the class variables for any/all motion steps, and writes out some of the default data
///  to the settings AiStrings
void CParticleTranslator::ExportPreambleData(AtNode* particle)
{
   m_particleCount = m_fnParticleSystem.count();

   int renderType = m_fnParticleSystem.renderType();

   AiMsgInfo("ParticleType %i", renderType);
   MStatus status;

   // Particle shape extra attributes
   int pointsAs            = m_fnParticleSystem.findPlug("renderPointsAs").asInt();
   m_exportId              = m_fnParticleSystem.findPlug("export_particleId").asBool();
   m_isOpaque              = m_fnParticleSystem.findPlug("opaque").asBool();
   m_deleteDeadParticles   = m_fnParticleSystem.findPlug("deleteDeadParticles").asBool();
   m_inheritCacheTxfm      = m_fnParticleSystem.findPlug("inheritCacheTransform").asBool();

   float minPixelWidth = m_fnParticleSystem.findPlug("minPixelWidth").asFloat();
   AiNodeSetFlt(particle, "min_pixel_width", minPixelWidth);

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
            {
               m_particleSize = (m_pointSize)*0.01;
            }
            break;
         case PARTICLE_TYPE_SPHERE:
            {
               m_particleSize = m_radius;
            }
            break;
         case PARTICLE_TYPE_SPRITE: // sprite
            {
               if (m_fnParticleSystem.isPerParticleDoubleAttribute(MString("spriteScaleXPP")) &&
               m_fnParticleSystem.isPerParticleDoubleAttribute(MString("spriteScaleYPP")))
               {
                  m_isSpritePP = true;
               }

               m_particleSize = m_spriteScaleX/2;
            }
            break;
         case PARTICLE_TYPE_STREAK: // streak
            {
               MGlobal::displayWarning("[mtoa]: Streak particle type is not yet supported");
               AiMsgInfo("[mtoa] Streak particle type is not yet supported");
               m_particleSize = (m_lineWidth)*0.01;
            }
            break;
         case PARTICLE_TYPE_BLOBBYSURFACE:// blobby
            {
               MGlobal::displayWarning("[mtoa]: Blobby particle type is not yet supported");
               AiMsgInfo("[mtoa] Blobby particle type is not yet supported");
               m_particleSize = m_radius;
            }
            break;
         case PARTICLE_TYPE_CLOUD:// cloud
            {
               MGlobal::displayWarning("[mtoa]: Cloud particle type is not yet supported");
               AiMsgInfo("[mtoa] Cloud particle type is not yet supported");
               m_particleSize = m_radius;
            }
            break;
         case PARTICLE_TYPE_TUBE:// tube
            {
               MGlobal::displayWarning("[mtoa]: Tube particle type is not yet supported");
               AiMsgInfo("[mtoa] Tube particle type is not yet supported");
               m_particleSize = m_radius;
            }
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
   MIntArray     particleId;

   MVectorArray*   positionArray = new MVectorArray;
   MDoubleArray*   radiusArray = new MDoubleArray;
   MDoubleArray*   spriteScaleXPP = new MDoubleArray;
   MDoubleArray*   spriteScaleYPP = new MDoubleArray;
   MVectorArray*   rgbArray = new MVectorArray;
   MDoubleArray*   opacityArray = new MDoubleArray;

   MStatus status;

   AiMsgInfo("[mtoa] Particle Exporting Step:: 0");

   GatherStandardPPData( positionArray ,
                         radiusArray ,
                         spriteScaleXPP ,
                         spriteScaleYPP ,
                         rgbArray ,
                         opacityArray,
                         velocityArray,
                         particleId);

   m_instantVeloArray = velocityArray;

   // push back a "starter" array for this step

   m_out_positionArrays.push_back(positionArray);

   if (m_isSprite)
   {
      m_out_spriteScaleXArrays.push_back(spriteScaleXPP);
      m_out_spriteScaleYArrays.push_back(spriteScaleYPP);
   }
   else
   {
      m_out_radiusArrays.push_back(radiusArray);
   }

   if (m_hasOpacity)
   {
      m_out_opacityArrays.push_back(opacityArray);
   }

   if (m_hasRGB)
   {
      m_out_colorArrays.push_back(rgbArray);
   }

   //  create the map  from particleID to array index in the "out_*"  arrays
   for (int i=0; i < m_particleCount; i++)
   {
      int id = particleId[i];
      m_particleIDMap[id] = i;
   }

   m_customAttrs = m_fnParticleSystem.findPlug("export_attributes").asString();

   if (m_customAttrs.length() != 0)
   {
      m_doExtraAttributes = true;
      ExportCustomParticleData(particle, 0);
   }

}// end step == 0

///

void CParticleTranslator::GatherBlurSteps(AtNode* particle, AtUInt step)
{
   AiMsgInfo("[mtoa] Particle Exporting Step:: %i",step);

   int numParticles = m_fnParticleSystem.count();

   MVectorArray   velocityArray;
   MIntArray      particleId;

   MVectorArray*   positionArray = new MVectorArray;
   MDoubleArray*   radiusArray = new MDoubleArray;
   MDoubleArray*   spriteScaleXPP = new MDoubleArray;
   MDoubleArray*   spriteScaleYPP = new MDoubleArray;
   MVectorArray*   rgbArray = new MVectorArray;
   MDoubleArray*   opacityArray = new MDoubleArray;

   GatherStandardPPData( positionArray ,
                         radiusArray ,
                         spriteScaleXPP ,
                         spriteScaleYPP ,
                         rgbArray ,
                         opacityArray,
                         velocityArray,
                         particleId);

   // add in a new vector entry for this step to all the  maps/vectors
   // this is just making a copy of the last step's data and placing it onto the stack of the out*Arrays
   // these will be modified with the data from this step.
   // We want to do this here because  it gives us the simplest way to get the new steps arrays set up
   // and later if particles don't exist in this step, we already have a value in this index and we don't have to go thru
   // another set of loops to fill in the gaps  for particles that have died as well.


   MVectorArray   *newPositionArray = new MVectorArray((*m_out_positionArrays[step-1]));
   m_out_positionArrays.push_back(newPositionArray);

   MDoubleArray *newSSXArray = NULL;
   MDoubleArray *newSSYArray = NULL;
   MDoubleArray *newRadiusArray =NULL;
   MDoubleArray *newOpacityArray = NULL;
   MVectorArray *newRGBArray= NULL;


   if (m_isSprite)
   {
      newSSXArray = new MDoubleArray((*m_out_spriteScaleXArrays[step-1]));
      newSSYArray = new MDoubleArray((*m_out_spriteScaleYArrays[step-1]));
      m_out_spriteScaleXArrays.push_back(newSSXArray);
      m_out_spriteScaleYArrays.push_back(newSSYArray);
   }
   else
   {
      newRadiusArray = new MDoubleArray((*m_out_radiusArrays[step-1]));
      m_out_radiusArrays.push_back(newRadiusArray);
   }

   if (m_hasOpacity)
   {
      newOpacityArray = new MDoubleArray((*m_out_opacityArrays[step-1]));
      m_out_opacityArrays.push_back(newOpacityArray);
   }

   if (m_hasRGB)
   {
      newRGBArray = new MVectorArray((*m_out_colorArrays[step-1]));
      m_out_colorArrays.push_back(newRGBArray);
   }

   std::map <std::string, std::vector< MVectorArray* > >::iterator vecIt;
   std::map <std::string, std::vector< MDoubleArray* > >::iterator doubleIt;
   std::map <std::string, std::vector< MIntArray* > >::iterator intIt;

   // using pointers to maya arrays to avoid copies
   std::map<std::string, MVectorArray* > instant_customVectorAttrArrays;
   std::map<std::string, MDoubleArray* > instant_customDoubleAttrArrays;
   std::map<std::string, MIntArray* > instant_customIntAttrArrays;

   if (m_doExtraAttributes)
   {
      // Extra export attrs
      for (doubleIt = m_out_customDoubleAttrArrays.begin(); doubleIt != m_out_customDoubleAttrArrays.end(); doubleIt++)
      {
         MDoubleArray *attributes = new MDoubleArray;
         m_fnParticleSystem.getPerParticleAttribute(MString(doubleIt->first.c_str()), *attributes);
         instant_customDoubleAttrArrays[doubleIt->first] = attributes;

         // add in a copy of the last step
         MDoubleArray* newAttributes = new MDoubleArray((*m_out_customDoubleAttrArrays[doubleIt->first][step-1]));
         m_out_customDoubleAttrArrays[doubleIt->first].push_back(newAttributes);

      }
      for (vecIt = m_out_customVectorAttrArrays.begin(); vecIt != m_out_customVectorAttrArrays.end(); vecIt++)
      {
         MVectorArray *attributes = new MVectorArray;
         m_fnParticleSystem.getPerParticleAttribute(vecIt->first.c_str(), *attributes);
         instant_customVectorAttrArrays[vecIt->first] = attributes;

         // add in a copy of the last step
         MVectorArray* newAttributes = new MVectorArray((*m_out_customVectorAttrArrays[vecIt->first][step-1]));
         m_out_customVectorAttrArrays[vecIt->first].push_back(newAttributes);
      }
      for (intIt = m_out_customIntAttrArrays.begin(); intIt != m_out_customIntAttrArrays.end(); intIt++)
      {
         MIntArray *attributes = new MIntArray;
         m_fnParticleSystem.getPerParticleAttribute(intIt->first.c_str(), *attributes);
         instant_customIntAttrArrays[intIt->first] = attributes;

         // add in a copy of the last step
         MIntArray* newAttributes = new MIntArray((*m_out_customIntAttrArrays[intIt->first][step-1]));
         m_out_customIntAttrArrays[intIt->first].push_back(newAttributes);
      }
   }

   particle = GetArnoldRootNode();

   MTime oneSec(1.0, MTime::kSeconds);
   int fps =  (float)oneSec.asUnits(MTime::uiUnit());

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

         if (m_hasRGB)
         {
            (*newRGBArray)[pindex] = (*rgbArray)[j];
         }
         if(m_hasOpacity)
         {
            (*newOpacityArray)[pindex] = (*opacityArray)[j];
         }

         if (m_isSprite)
         {
            // FIXME: keep in mind there is no point in using "new" if we're indexing
            // and copying its contents over one by one.  if we can use spriteScaleXPP
            // as a starting point, it should be much more efficient
            // NOTE: this needs more thinking on the subject first, as it may introduce its own  overhead

            (*newSSXArray)[pindex] = (*spriteScaleXPP)[j];
            (*newSSYArray)[pindex] = (*spriteScaleYPP)[j];
         }
         else
         {
            (*m_out_radiusArrays[step])[pindex] = (*radiusArray)[j];
         }

         if (m_doExtraAttributes)
         {
            // Extra export attrs
            for (doubleIt = m_out_customDoubleAttrArrays.begin(); doubleIt != m_out_customDoubleAttrArrays.end(); doubleIt++)
            {
               (*doubleIt->second[step])[pindex] = (*instant_customDoubleAttrArrays[doubleIt->first])[j];
            }
            for (vecIt = m_out_customVectorAttrArrays.begin(); vecIt != m_out_customVectorAttrArrays.end(); vecIt++)
            {
               (*vecIt->second[step])[pindex] = (*instant_customVectorAttrArrays[vecIt->first])[j];
            }
            for (intIt = m_out_customIntAttrArrays.begin(); intIt != m_out_customIntAttrArrays.end(); intIt++)
            {
               (*intIt->second[step])[pindex] = (*instant_customIntAttrArrays[intIt->first])[j];
            }
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
         for (uint k = 0; k <= step; k++)
         {
            // add particle to this steps arrays
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

            if (m_hasRGB)
            {
               (*m_out_colorArrays[k]).append((*rgbArray)[j]);

            }

            if (m_hasOpacity)
            {
               (*m_out_opacityArrays[k]).append((*opacityArray)[j]);
            }

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
            else
            {
                  (*m_out_radiusArrays[k]).append((*radiusArray)[j]);
            }

            if (m_doExtraAttributes)
            {
               // Extra export attrs
               for (doubleIt = m_out_customDoubleAttrArrays.begin(); doubleIt != m_out_customDoubleAttrArrays.end(); doubleIt++)
               {
                  (*doubleIt->second[k]).append((*instant_customDoubleAttrArrays[doubleIt->first])[j]);
               }
               for (vecIt = m_out_customVectorAttrArrays.begin(); vecIt != m_out_customVectorAttrArrays.end(); vecIt++)
               {
                  (*vecIt->second[k]).append((*instant_customVectorAttrArrays[vecIt->first])[j]);
               }
               for (intIt = m_out_customIntAttrArrays.begin(); intIt != m_out_customIntAttrArrays.end(); intIt++)
               {
                  (*intIt->second[k]).append((*instant_customIntAttrArrays[intIt->first])[j]);
               }
            }
         }
      }
   }

   // delete the  pointers to the instant array values for the extra custom attributes
   for (doubleIt = m_out_customDoubleAttrArrays.begin(); doubleIt != m_out_customDoubleAttrArrays.end(); doubleIt++)
   {
      delete instant_customDoubleAttrArrays[doubleIt->first];
   }
   for (vecIt = m_out_customVectorAttrArrays.begin(); vecIt != m_out_customVectorAttrArrays.end(); vecIt++)
   {
      delete instant_customVectorAttrArrays[vecIt->first];
   }
   for (intIt = m_out_customIntAttrArrays.begin(); intIt != m_out_customIntAttrArrays.end(); intIt++)
   {
      delete instant_customIntAttrArrays[intIt->first];
   }

   AiMsgInfo("[mtoa] Particle Export found %i new particles for this step",newParticleCount);

   // if we still have entries in tempMap, that means the particle died in this frameStep and we need to update
   // the value for position based on velocity only.. all other attributes, since they were copied over at the beginning
   // can just stay the same. This  seems faster than iterating thru every  particle attribute  again here...
   if (tempMap.size() > 0)
   {
      AiMsgInfo("[mtoa] Particle Export found %i particles that died for this step, computing velocity...", (int)tempMap.size());

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
            MVector velocitySubstep = (((m_instantVeloArray[it->second]/fps)*GetMotionByFrame())/(GetNumMotionSteps()-1));
            MVector newVeloPosition = (*m_out_positionArrays[step-1])[it->second] + velocitySubstep;
            (*newPositionArray)[it->second] = newVeloPosition;
         }
      }
   }
}

/// OR... a slightly speedier approach.
/// instead of actually gathering the values from maya per step, we just use the first steps  values
// and last steps velocity and compute  the new position.
// we just copy the rest of the data via pointers as it doesn't need to change per frameStep.
// There is an extra attribute  called  "computeInstantBlurSteps"  that defines whether to use this or the above
// function to populate the motion blur steps

void CParticleTranslator::ComputeBlurSteps(AtNode* particle, AtUInt step)
{
   AiMsgInfo("[mtoa] Particle Computing Step:: %i",step);

   MTime oneSec(1.0, MTime::kSeconds);
   int fps =  (float)oneSec.asUnits(MTime::uiUnit());
   //uint totalSteps = GetNumMotionSteps(); // not needed

   MVectorArray   velocityArray;
   MIntArray      particleId;

   std::map <std::string, std::vector< MVectorArray* > >::iterator vecIt;
   std::map <std::string, std::vector< MDoubleArray* > >::iterator doubleIt;
   std::map <std::string, std::vector< MIntArray* > >::iterator intIt;

    particle = GetArnoldRootNode();


   // add in a new vector entry for this step to all the  maps/vectors
   // this is just making a copy of the last steps data and placing it onto the stack of the out*Arrays
   // these will be modified with the data from this step

   MVectorArray   *newPositionArray = new MVectorArray((*m_out_positionArrays[step-1]));


   if (m_isSprite)
   {
      MDoubleArray *newSSXArray = new MDoubleArray((*m_out_spriteScaleXArrays[step-1]));
      MDoubleArray *newSSYArray = new MDoubleArray((*m_out_spriteScaleYArrays[step-1]));
      m_out_spriteScaleXArrays.push_back(newSSXArray);
      m_out_spriteScaleYArrays.push_back(newSSYArray);
   }
   else
   {
      MDoubleArray *newRadiusArray = new MDoubleArray((*m_out_radiusArrays[step-1]));
      m_out_radiusArrays.push_back(newRadiusArray);
   }

  if (m_hasOpacity)
   {
     MDoubleArray *newOpacityArray = new MDoubleArray((*m_out_opacityArrays[step-1]));
     m_out_opacityArrays.push_back(newOpacityArray);
   }

   if (m_hasRGB)
   {
      MVectorArray   *newRGBArray = new MVectorArray((*m_out_colorArrays[step-1]));
      m_out_colorArrays.push_back(newRGBArray);
   }


   if (m_doExtraAttributes)
   {

      // Extra export attrs
      for (doubleIt = m_out_customDoubleAttrArrays.begin(); doubleIt != m_out_customDoubleAttrArrays.end(); doubleIt++)
      {
         // add in a copy of the last step
         MDoubleArray* newAttributes = new MDoubleArray((*m_out_customDoubleAttrArrays[doubleIt->first][step-1]));
         doubleIt->second.push_back(newAttributes);

      }
      for (vecIt = m_out_customVectorAttrArrays.begin(); vecIt != m_out_customVectorAttrArrays.end(); vecIt++)
      {
         // add in a copy of the last step
         MVectorArray* newAttributes = new MVectorArray((*m_out_customVectorAttrArrays[vecIt->first][step-1]));
         vecIt->second.push_back(newAttributes);
      }
      for (intIt = m_out_customIntAttrArrays.begin(); intIt != m_out_customIntAttrArrays.end(); intIt++)
      {
         // add in a copy of the last step
         MIntArray* newAttributes = new MIntArray((*m_out_customIntAttrArrays[intIt->first][step-1]));
         intIt->second.push_back(newAttributes);
      }
   }

   for (int j = 0; j < m_particleCount; j++)
   {
      MVector velocitySubstep = (((m_instantVeloArray[j]/fps)*GetMotionByFrame())/(GetNumMotionSteps()-1));
      MVector newVeloPosition = (*m_out_positionArrays[step-1])[j] + velocitySubstep;
      (*newPositionArray)[j] = newVeloPosition;
   }

   m_out_positionArrays.push_back(newPositionArray);
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

   float minRadius   = m_fnParticleSystem.findPlug("minParticleRadius").asFloat();
   float maxRadius   = m_fnParticleSystem.findPlug("maxParticleRadius").asFloat();
   float radiusMult  = m_fnParticleSystem.findPlug("radiusMultiplier").asFloat();

   std::map <std::string, std::vector< MVectorArray* > >::iterator vecIt;
   std::map <std::string, std::vector< MDoubleArray* > >::iterator doubleIt;
   std::map <std::string, std::vector< MIntArray* > >::iterator intIt;

   m_particleCount = (*m_out_positionArrays[0]).length();

   AiMsgInfo("[mtoa] Particle count : %i",m_particleCount);

   /// Finally set the  position and  radius/aspect values with their cache values minus Transform position
   AtPoint a_v;
   MVector m_v;
   AtFloat a_r;
   AtFloat a_a;
   //getting the matrix of the point particle object

   AtMatrix apm;
   MMatrix mpm;
   if (!m_inheritCacheTxfm)  //  if we  want to inherit the transform we need to not remove it from the particles when we export
   {
      mpm = m_dagPath.inclusiveMatrix();

      // convert it to AtMatrix
      ConvertMatrix (apm, mpm); // util From CNodeTranslator

      // inverting the matrix
      AiM4Invert (apm, apm);
   }

   // declare the arrays  now that we have gathered all the particle info from each step
   a_positionArray = AiArrayAllocate(m_particleCount*m_multiCount, GetNumMotionSteps(), AI_TYPE_POINT);
   a_radiusArray = AiArrayAllocate(m_particleCount*m_multiCount, GetNumMotionSteps(), AI_TYPE_FLOAT) ;

   if (m_isSprite)
   {
      a_aspectArray = AiArrayAllocate(m_particleCount*m_multiCount, GetNumMotionSteps(), AI_TYPE_FLOAT);
   }

   if (m_exportId)
   {
      a_ParticleIdArray = AiArrayAllocate(m_particleCount*m_multiCount, GetNumMotionSteps(), AI_TYPE_INT);
   }

   if (m_doMultiPoint) // multiPoint index
   {
      a_ParticleMultiIndexArray = AiArrayAllocate(m_particleCount*m_multiCount, GetNumMotionSteps(), AI_TYPE_INT);
   }
   if (m_hasRGB)
   {
      a_rgbPPArray = AiArrayAllocate(m_particleCount*m_multiCount, GetNumMotionSteps(),  AI_TYPE_RGB);
   }
   if(m_hasOpacity)
   {
      a_opacityPPArray = AiArrayAllocate(m_particleCount*m_multiCount, GetNumMotionSteps(), AI_TYPE_FLOAT);
   }

   std::map <int, int>::iterator it;
   for (uint s = 0; s < GetNumMotionSteps(); s++)
   {
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
               noisePoint.x = i+j+.1454329;
               noisePoint.y = i+j+.3234548;
               noisePoint.z = i+j+.0921081;

               a_v.x = (AtFloat)m_v.x+(AiPerlin4(noisePoint,i+j+.2340970)*m_multiRadius);
               a_v.y = (AtFloat)m_v.y+(AiPerlin4(noisePoint,i+j+23.1203093)*m_multiRadius);
               a_v.z = (AtFloat)m_v.z+(AiPerlin4(noisePoint,i+j-.4874771)*m_multiRadius);

               //remove matrix for the point if inherit from transform is not  set
               if (!m_inheritCacheTxfm)
                  AiM4PointByMatrixMult (&a_v, apm, &a_v);


               // Calculated offset index
               int  index =  s*(m_particleCount*m_multiCount) +i*m_multiCount+j;

               AiArraySetPnt(a_positionArray, index, a_v);

               if (m_isSprite)
               {
                  a_a = (*m_out_spriteScaleXArrays[s])[pindex]/(*m_out_spriteScaleYArrays[s])[pindex];
                  AiArraySetFlt (a_aspectArray, index, (AtFloat)a_a);

                  a_r = CLAMP((((AtFloat)(*m_out_spriteScaleXArrays[s])[pindex])/2), minRadius, maxRadius);
                  a_r *= radiusMult;
               }
               else
               {
                  a_r = CLAMP(((AtFloat)(*m_out_radiusArrays[s])[pindex]), minRadius, maxRadius);
                  a_r *= radiusMult;
               }

               AiArraySetFlt(a_radiusArray, index, a_r);

               if (m_exportId)
               {
                  AiArraySetInt(a_ParticleIdArray, index, (int)it->first);
               }
               if (m_doMultiPoint)
               {
                  AiArraySetInt(a_ParticleMultiIndexArray, index, (int)j);
               }
               if (m_hasRGB)
               {
                  // Make m_rgb local and remove m_
                  m_rgb = (*m_out_colorArrays[s])[pindex];
                  a_rgb.r = (AtFloat)m_rgb.x;
                  a_rgb.g = (AtFloat)m_rgb.y;
                  a_rgb.b = (AtFloat)m_rgb.z;
                  AiArraySetRGB(a_rgbPPArray, index, a_rgb);
               }
               if (m_hasOpacity)
               {
                  AiArraySetFlt(a_opacityPPArray, index,  (*m_out_opacityArrays[s])[pindex]);
               }

            }// end  multicount

         }// if particle is valid (did not die within motion blur steps)

         i++;  // tracking the  iteration thru the map

      }// end m_particleIDMap  iteration

      // CLEAN UP MEMORY
      AiMsgInfo("[particleExport] cleaning up memory ");
      delete m_out_positionArrays[s];
      if (m_hasRGB)
      {
         delete m_out_colorArrays[s];
      }
      if (m_hasOpacity)
      {
         delete m_out_opacityArrays[s];
      }
      if (m_isSprite)
      {
         delete m_out_spriteScaleXArrays[s];
         delete m_out_spriteScaleYArrays[s];
      }
      else
      {
         delete m_out_radiusArrays[s];
      }
   }// end  numMotionSteps


   //write the points
   AiNodeSetArray(particle, "points", a_positionArray);

   //write the radius
   AiNodeSetArray(particle, "radius", a_radiusArray);

   if (m_isSprite)
   {
      AiNodeSetArray(particle, "aspect", a_aspectArray);
   }

   if (m_exportId)
   {
      AiNodeSetArray(particle, "particleId", a_ParticleIdArray);
   }

   if (m_doMultiPoint)
   {
      AiNodeSetArray(particle, "particleMultiIndex", a_ParticleMultiIndexArray);
   }

   if(m_hasRGB)
   {
      AiNodeSetArray(particle, "rgbPP", a_rgbPPArray);
   }
   if (m_hasOpacity)
   {
      AiNodeSetArray(particle, "opacityPP", a_opacityPPArray);
   }

   if (m_doExtraAttributes)
   {

      // Extra export attrs
      for (doubleIt = m_out_customDoubleAttrArrays.begin(); doubleIt != m_out_customDoubleAttrArrays.end(); doubleIt++)
      {
         AtArray* a_attributes = AiArrayAllocate(m_particleCount*m_multiCount, GetNumMotionSteps(), AI_TYPE_FLOAT);
         for (uint s = 0; s < GetNumMotionSteps(); s++)
         {
            for (int i = 0; i < m_particleCount; i++)
            {
               for (int j = 0; j< m_multiCount; j++)
               {
                  // Calculated offset index
                  int  index =  s*(m_particleCount*m_multiCount) +i*m_multiCount+j;
                  AiArraySetFlt(a_attributes, index, (*doubleIt->second[s])[i]);
               }
            }
            // memory cleanup
            //std::cout << "cleaning up extra Double attr memory " << doubleIt->second[s] << std::endl;
            delete doubleIt->second[s];
         }
         AiNodeDeclare(particle, doubleIt->first.c_str(), "uniform FLOAT");
         AiNodeSetArray(particle, doubleIt->first.c_str(), a_attributes);

      }
      for (vecIt = m_out_customVectorAttrArrays.begin(); vecIt != m_out_customVectorAttrArrays.end(); vecIt++)
      {
         AtArray* a_attributes = AiArrayAllocate(m_particleCount*m_multiCount, GetNumMotionSteps(), AI_TYPE_VECTOR);
         for (uint s = 0; s < GetNumMotionSteps(); s++)
         {
            for (int i = 0; i < m_particleCount; i++)
            {
               for (int j = 0; j< m_multiCount; j++)
               {
                  // Calculated offset index
                  int  index =  s*(m_particleCount*m_multiCount) +i*m_multiCount+j;
                  AtVector a_attr;

                  a_attr.x = (AtFloat)(*vecIt->second[s])[i].x;
                  a_attr.y = (AtFloat)(*vecIt->second[s])[i].y;
                  a_attr.z = (AtFloat)(*vecIt->second[s])[i].z;

                  AiArraySetVec(a_attributes, index, a_attr);
               }
            }
            // memory cleanup
            //std::cout << "cleaning up extra Vector attr memory " << vecIt->second[s] << std::endl;
            delete vecIt->second[s];
         }
         AiNodeDeclare(particle, vecIt->first.c_str(), "uniform VECTOR");
         AiNodeSetArray(particle, vecIt->first.c_str(), a_attributes);

      }
      for(intIt = m_out_customIntAttrArrays.begin(); intIt != m_out_customIntAttrArrays.end(); intIt++)
      {
         AtArray* a_attributes = AiArrayAllocate(m_particleCount*m_multiCount, GetNumMotionSteps(), AI_TYPE_INT);
        for (uint s = 0; s < GetNumMotionSteps(); s++)
        {
           for (int i = 0; i < m_particleCount; i++)
           {
              for (int j = 0; j< m_multiCount; j++)
              {
                 // Calculated offset index
                 int  index =  s*(m_particleCount*m_multiCount) +i*m_multiCount+j;
                 AiArraySetInt(a_attributes, index, (*intIt->second[s])[i]);
              }
           }
           // memory cleanup
           //std::cout << "cleaning up extra Int attr memory " << vecIt->second[s] << std::endl;
           delete vecIt->second[s];
        }
        AiNodeDeclare(particle, intIt->first.c_str(), "uniform INT");
        AiNodeSetArray(particle, intIt->first.c_str(), a_attributes);
      }

   }

   // end  last step export
}

void CParticleTranslator::GatherStandardPPData( MVectorArray*   positionArray ,
                                                MDoubleArray*   radiusArray ,
                                                MDoubleArray*   spriteScaleXPP ,
                                                MDoubleArray*   spriteScaleYPP ,
                                                MVectorArray*   rgbArray ,
                                                MDoubleArray*   opacityArray,
                                                MVectorArray    &velocityArray,
                                                MIntArray       &particleId)
{

   uint numParticles = m_fnParticleSystem.count();

   if (m_hasRGB)
   {
      m_fnParticleSystem.rgb(*rgbArray);
   }
   if(m_hasOpacity)
   {
      m_fnParticleSystem.opacity(*opacityArray);
   }

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

   if (m_hasRadiusPP )
   {
      m_fnParticleSystem.getPerParticleAttribute(MString("radiusPP"), *radiusArray);
   }
   else
   {
      for (uint i=0; i < numParticles; i++)
      {
         (*radiusArray).append(m_particleSize);
      }
   }

   m_fnParticleSystem.position(*positionArray);
   m_fnParticleSystem.velocity(velocityArray);
   m_fnParticleSystem.particleIds(particleId);

}


AtNode* CParticleTranslator::ExportInstance(AtNode *instance, const MDagPath& masterInstance)
{

   AtNode* masterNode = AiNodeLookUpByName(masterInstance.partialPathName().asChar());

   int instanceNum =  m_dagPath.instanceNumber();

   AiNodeSetStr(instance, "name", m_dagPath.partialPathName().asChar());

   ExportMatrix(instance, 0);

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
      sgNode = iogConnections[0].node();
      dsgNode.setObject(sgNode);
      MPlug             shaderPlug(sgNode, dsgNode.attribute("surfaceShader"));
      MPlugArray        connections;

      shaderPlug.connectedTo(connections, true, false);

      AtNode* shader = ExportNode(connections[0]);
      AiNodeSetPtr(instance, "shader", shader);
   }

   return instance;
}


AtNode* CParticleTranslator::ExportParticleNode(AtNode* particle, AtUInt step)
{
   if (step == 0)
   {
      ExportParticleShaders(particle);
      ExportPreambleData(particle);
      GatherFirstStep(particle);
   }
   else
   {
      if (!(m_fnParticleSystem.findPlug("computeInstantBlurSteps").asBool()))
      {
         GatherBlurSteps(particle, step); // gather the data from each step
      }
      else
      {
         ComputeBlurSteps(particle, step); // compute all the data from  the first steps  population
      }
   }

   /// write out final data
   if (step == (GetNumMotionSteps()-1))
   {
      WriteOutParticle(particle);
   }
   return particle;
}


void CParticleTranslator::Update(AtNode *anode)
{
   ExportMatrix(anode, 0);
}

void CParticleTranslator::ExportMotion(AtNode* anode, AtUInt step)
{
   if (m_isMasterDag)
   {
      //ExportMatrix(anode, step);
      if (m_motionDeform)
      {
         ExportParticleNode(anode, step);
      }
   }
   else
   {
      ExportMatrix(anode, step);
   }
}

void CParticleTranslator::Export(AtNode* anode)
{
   // check if the particle system is linked to an instancer
   m_fnParticleSystem.setObject(m_dagPath.node());
   if (m_fnParticleSystem.isValid() == false)
   {
      AiMsgError("[mtoa]: Particle system not exported. It has 0 particles");
      return;
   }


   if (m_isMasterDag)
   {
      ///MTimer exportParticleTimer;
      //exportParticleTimer.beginTimer();
      ExportParticleNode(anode, 0);
      //exportParticleTimer.endTimer();
      //double elapsed = exportParticleTimer.elapsedTime();
      //AiMsgInfo("[mtoa] Particle Export took : %f seconds",elapsed);
   }

   else
      ExportInstance(anode, m_masterDag);


}
