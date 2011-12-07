
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

            for (int i = 0; i <= (GetNumMotionSteps()-1); i++)
            {
               if (i == 0)
               {
                  out_customDoubleAttrArrays[currentAttr.asChar()].push_back(attributes);
               }
               else
               {
                  MDoubleArray* newAttributes = new MDoubleArray(*attributes);
                  out_customDoubleAttrArrays[currentAttr.asChar()].push_back(newAttributes);
               }
            }
            continue;
         }
         else if (m_fnParticleSystem.isPerParticleVectorAttribute(currentAttr))
         {
            MVectorArray*  attributes = new MVectorArray;
            m_fnParticleSystem.getPerParticleAttribute(currentAttr, *attributes);

            for (int i = 0; i <= (GetNumMotionSteps()-1); i++)
            {
               if (i == 0)
               {
                  out_customVectorAttrArrays[currentAttr.asChar()].push_back(attributes);
               }
               else
               {
                  MVectorArray* newAttributes = new MVectorArray(*attributes);
                  out_customVectorAttrArrays[currentAttr.asChar()].push_back(newAttributes);
               }
            }
            continue;

         }
         else if (m_fnParticleSystem.isPerParticleIntAttribute(currentAttr))
         {
            MIntArray*  attributes = new MIntArray;
            m_fnParticleSystem.getPerParticleAttribute(currentAttr, *attributes);
            for (int i = 0; i <= (GetNumMotionSteps()-1); i++)
            {
               if (i == 0)
               {
                  out_customIntAttrArrays[currentAttr.asChar()].push_back(attributes);
               }
               else
               {
                  MIntArray* newAttributes = new MIntArray(*attributes);
                  out_customIntAttrArrays[currentAttr.asChar()].push_back(newAttributes);
               }
            }

            continue;

         }
      }
   }

}


/// Export particle datas with basic attributes rgbPP, OpacityPP, radiusPP and particleId

void CParticleTranslator::ExportParticleData(AtNode* particle, AtUInt step)
{
   // quick synopsis:  we  loop on steps->particles->multipoint
   // first step, we push a copy of first steps particle data into all arrays
   // all other steps, we  loop on particle IDS  and find matching particles by ID  and modify  that steps value
   // if we find new particles, we compute  previous velocity positions and add those particles to the end of all the arrays
   // if particles die, we compute current values only,  based on last position and last velocity and leave all other attrs alone,
   // as they are already populated with their most current values


   // FIXED  Reordered stuff into  motion blur and non motion blur sections
   //FIXME : need cleaner code, concatenate all the conditions.
   MTime oneSec(1.0, MTime::kSeconds);
   int fps =  (float)oneSec.asUnits(MTime::uiUnit());
   uint totalSteps = GetNumMotionSteps();
   int numParticles;

   AtArray* a_positionArray(NULL);
   AtArray* a_ParticleIdArray(NULL);
   AtArray* a_ParticleMultiIndexArray(NULL);
   AtArray* a_radiusArray(NULL);
   AtArray* a_aspectArray(NULL);
   AtArray* a_rgbPPArray(NULL);
   AtArray* a_opacityPPArray(NULL);

   MVectorArray   velocityArray;
   MIntArray      particleId;

   MVectorArray*   positionArray = new MVectorArray;
   MDoubleArray*   radiusArray = new MDoubleArray;
   MDoubleArray*   spriteScaleXPP = new MDoubleArray;
   MDoubleArray*   spriteScaleYPP = new MDoubleArray;
   MVectorArray*   rgbArray = new MVectorArray;
   MDoubleArray*   opacityArray = new MDoubleArray;

   // these hold  the current frame steps values per map entry  for looping to update the  out_custom*  vectors
   std::map<std::string, MVectorArray* > instant_customVectorAttrArrays;
   std::map<std::string, MDoubleArray* > instant_customDoubleAttrArrays;
   std::map<std::string, MIntArray* > instant_customIntAttrArrays;

   MVector m_rgb;
   AtRGB a_rgb;
   MStatus status;

   // FIXED
   // FIXME: create an enum for the renderType
   int renderType = m_fnParticleSystem.renderType();

   AiMsgInfo("ParticleType %i", renderType);

   // Particle shape extra attributes
   int  pointsAs     = m_fnParticleSystem.findPlug("renderPointsAs").asInt();
   float minRadius   = m_fnParticleSystem.findPlug("minParticleRadius").asFloat();
   float maxRadius   = m_fnParticleSystem.findPlug("maxParticleRadius").asFloat();
   float radiusMult  = m_fnParticleSystem.findPlug("radiusMultiplier").asFloat();
   bool exportId     = m_fnParticleSystem.findPlug("export_particleId").asBool();
   bool m_isOpaque   = m_fnParticleSystem.findPlug("opaque").asBool();


   AiMsgInfo("[mtoa] Particle Exporting Step:: %i",step);

   numParticles = m_fnParticleSystem.count();

   std::map <std::string, std::vector< MVectorArray* > >::iterator vecIt;
   std::map <std::string, std::vector< MDoubleArray* > >::iterator doubleIt;
   std::map <std::string, std::vector< MIntArray* > >::iterator intIt;

   // STEP 0
   if (step == 0)
   {

      m_deleteDeadParticles = m_fnParticleSystem.findPlug("deleteDeadParticles").asBool();
      m_inheritCacheTxfm = m_fnParticleSystem.findPlug("inheritCacheTransform").asBool();

      m_particleCount = numParticles;

      float minPixelWidth = m_fnParticleSystem.findPlug("minPixelWidth").asFloat();
      AiNodeSetFlt(particle, "min_pixel_width", minPixelWidth);


      // TODO implement  streak / blobby / cloud / tube,  formats
      switch (renderType)
      {
         case m_renderTypeSphere:
            AiNodeSetStr(particle, "mode", "sphere");
            break;
         case m_renderTypeSprite:
            AiNodeSetStr(particle, "mode", "quad");
            m_isSprite = true;
            break;
         case m_renderTypeBlobbySurface:
            AiNodeSetStr(particle, "mode", "sphere");
            break;
         case m_renderTypeCloud:
            AiNodeSetStr(particle, "mode", "sphere");
            break;
         case m_renderTypeTube:
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
      }
      // now check for multi
      if (renderType == m_renderTypeMultiPoint || renderType == m_renderTypeMultiStreak) // multiPoint/multiStreak
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

      m_hasRadiusPP  = m_fnParticleSystem.isPerParticleDoubleAttribute(MString("radiusPP"));

      if (exportId)
      {
         AiNodeDeclare(particle, "particleId", "uniform INT");
      }

      // get the array of rgbPPs
      if (m_hasRGB)
      {
         m_fnParticleSystem.rgb(*rgbArray);
         AiNodeDeclare(particle, "rgbPP", "uniform RGB");
      }

      // get the array of opacities
      if (m_hasOpacity)
      {
         m_fnParticleSystem.opacity(*opacityArray);
         AiNodeDeclare(particle, "opacityPP", "uniform FLOAT");
      }

      if (m_hasRadiusPP && !m_isSprite)
      {
         m_fnParticleSystem.getPerParticleAttribute(MString("radiusPP"), *radiusArray);
      }
      else //
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
         case m_renderTypeMultiPoint:
            {
               m_particleSize = (m_pointSize)*0.01;
            }
            break;
         case m_renderTypeSphere:
            {
               m_particleSize = m_radius;
            }
            break;
         case m_renderTypeSprite: // sprite
            {
               if (m_fnParticleSystem.isPerParticleDoubleAttribute(MString("spriteScaleXPP")) &&
               m_fnParticleSystem.isPerParticleDoubleAttribute(MString("spriteScaleYPP")))
               {
                  m_isSpritePP = true;
                  AiMsgInfo("SPRITESCALE X and Y PP values found");
                  m_fnParticleSystem.getPerParticleAttribute(MString("spriteScaleXPP"), *spriteScaleXPP);
                  m_fnParticleSystem.getPerParticleAttribute(MString("spriteScaleYPP"), *spriteScaleYPP);
               }

               m_particleSize = m_spriteScaleX/2;
            }
            break;
         case m_renderTypeStreak: // streak
            {
               MGlobal::displayWarning("[mtoa]: Streak particle type is not yet supported");
               AiMsgInfo("[mtoa] Streak particle type is not yet supported");
               m_particleSize = (m_lineWidth)*0.01;
            }
            break;
         case m_renderTypeBlobbySurface:// blobby
            {
               MGlobal::displayWarning("[mtoa]: Blobby particle type is not yet supported");
               AiMsgInfo("[mtoa] Blobby particle type is not yet supported");
               m_particleSize = m_radius;
            }
            break;
         case m_renderTypeCloud:// cloud
            {
               MGlobal::displayWarning("[mtoa]: Cloud particle type is not yet supported");
               AiMsgInfo("[mtoa] Cloud particle type is not yet supported");
               m_particleSize = m_radius;
            }
            break;
         case m_renderTypeTube:// tube
            {
               MGlobal::displayWarning("[mtoa]: Tube particle type is not yet supported");
               AiMsgInfo("[mtoa] Tube particle type is not yet supported");
               m_particleSize = m_radius;
            }
            break;
         default:
            m_particleSize = (m_pointSize)*0.01;
         }

         // populate the radius array with the value
         if (m_isSprite )
         {
            if (!m_isSpritePP)
            {
               for (int i=0; i<numParticles; i++)
               {
                  (*spriteScaleXPP).append(m_spriteScaleX);
                  (*spriteScaleYPP).append(m_spriteScaleY);
               }
            }
         }
         else
         {
            for (int i=0; i<numParticles; i++)
            {
               (*radiusArray).append(m_particleSize);
            }
         }
      }

      // Always get these
      m_fnParticleSystem.position(*positionArray);
      m_fnParticleSystem.velocity(velocityArray);
      m_fnParticleSystem.particleIds(particleId);

      instantVeloArray = velocityArray;

      // push back a "starter" array for all the steps
      for (uint i = 0; i < GetNumMotionSteps(); i++)
      {
         if (i == 0)
         {
            out_positionArrays.push_back(positionArray);
         }
         else
         {
            MVectorArray   *newPositionArray = new MVectorArray(*positionArray);
            out_positionArrays.push_back(newPositionArray);
         }

         if (m_isSprite)
         {
            if(i == 0)
            {
               out_spriteScaleXArrays.push_back(spriteScaleXPP);
               out_spriteScaleYArrays.push_back(spriteScaleYPP);
            }
            else
            {
               MDoubleArray *newSSXArray = new MDoubleArray(*spriteScaleXPP);
               MDoubleArray *newSSYArray = new MDoubleArray(*spriteScaleYPP);
               out_spriteScaleXArrays.push_back(newSSXArray);
               out_spriteScaleYArrays.push_back(newSSYArray);
            }
         }
         else
         {
            if(i == 0)
            {
               out_radiusArrays.push_back(radiusArray);
            }
            else
            {
               MDoubleArray *newRadiusArray = new MDoubleArray(*radiusArray);
               out_radiusArrays.push_back(newRadiusArray);
            }

         }

         if (m_hasOpacity)
         {
            if(i == 0)
            {
               out_opacityArrays.push_back(opacityArray);
            }
            else
            {
               MDoubleArray *newOpacityArray = new MDoubleArray(*opacityArray);
               out_opacityArrays.push_back(newOpacityArray);
            }
         }

         if (m_hasRGB)
         {
            if(i == 0)
            {
               out_colorArrays.push_back(rgbArray);
            }
            else
            {
               MVectorArray   *newRGBArray = new MVectorArray(*rgbArray);
               out_colorArrays.push_back(newRGBArray);
            }
         }
      }

      //  create the map  from particleID to array index in the "out_*"  arrays
      for (int i=0; i<numParticles; i++)
      {
         int id = particleId[i];
         particleIDMap[id] = i;
         // FIXED
         // FIXME: at this point haven't we just copied particleId to out_idArray? can we get rid of particleId?
         // got rid of  out_idArray  since I'm using particle ID map now  to track it all

      }


      m_customAttrs = m_fnParticleSystem.findPlug("export_attributes").asString();

      if (m_customAttrs.length() != 0)
      {
         m_doExtraAttributes =true;
         ExportCustomParticleData(particle, step);
      }

   }// end step == 0


   // STEP  > 0
   else
   {

      numParticles = m_fnParticleSystem.count();

      particle = GetArnoldRootNode();

      m_fnParticleSystem.position(*positionArray);
      m_fnParticleSystem.velocity(velocityArray);
      m_fnParticleSystem.particleIds(particleId);

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
            for (int i=0; i<numParticles; i++)
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
         for (int i=0; i<numParticles; i++)
         {
            (*radiusArray).append(m_particleSize);
         }
      }

      if (m_doExtraAttributes)
      {

         // Extra export attrs
         for (doubleIt = out_customDoubleAttrArrays.begin(); doubleIt != out_customDoubleAttrArrays.end(); doubleIt++)
         {

            MDoubleArray *attributes = new MDoubleArray;
            m_fnParticleSystem.getPerParticleAttribute(MString(doubleIt->first.c_str()), *attributes);
            // FIXME: i'm pretty sure the assignment below is a copy: we should use 'new' to create pointers.
            // this applies to anywhere that we are putting M*Arrays into vectors or maps
            // however, we need to ensure that we call 'delete' for each, or we'll have a memory leak
            instant_customDoubleAttrArrays[doubleIt->first] = attributes;

         }
         for (vecIt = out_customVectorAttrArrays.begin(); vecIt != out_customVectorAttrArrays.end(); vecIt++)
         {
            MVectorArray *attributes = new MVectorArray;
            m_fnParticleSystem.getPerParticleAttribute(vecIt->first.c_str(), *attributes);
            instant_customVectorAttrArrays[vecIt->first] = attributes;
         }
         for (intIt = out_customIntAttrArrays.begin(); intIt != out_customIntAttrArrays.end(); intIt++)
         {
            MIntArray *attributes = new MIntArray;
            m_fnParticleSystem.getPerParticleAttribute(intIt->first.c_str(), *attributes);
            instant_customIntAttrArrays[intIt->first] = attributes;
         }
      }

      // need to copy here since we need to keep particle ID map sacred
      std::map <int, int> tempMap = particleIDMap;
      std::map <int, int>::iterator it;
      int newParticleCount = 0;
      for (int j = 0; j < numParticles; j++)
      {
         it = tempMap.find(particleId[j]);
         if (it != tempMap.end())   // found the particle in the scene already
         {
            instantVeloArray[it->second] = velocityArray[j];
            (*out_positionArrays[step])[it->second] = (*positionArray)[j];
            if (m_hasRGB)
            {
               (*out_colorArrays[step])[it->second] = (*rgbArray)[j];
            }
            if(m_hasOpacity)
            {
               (*out_opacityArrays[step])[it->second] = (*opacityArray)[j];
            }

            if (m_isSprite)
            {
               (*out_spriteScaleXArrays[step])[it->second] = (*spriteScaleXPP)[j];
               (*out_spriteScaleYArrays[step])[it->second] = (*spriteScaleYPP)[j];
            }
            else
            {
               (*out_radiusArrays[step])[it->second] = (*radiusArray)[j];
            }

            if (m_doExtraAttributes)
            {
               // Extra export attrs
               for (doubleIt = out_customDoubleAttrArrays.begin(); doubleIt != out_customDoubleAttrArrays.end(); doubleIt++)
               {
                  (*out_customDoubleAttrArrays[doubleIt->first][step])[it->second] = (*instant_customDoubleAttrArrays[doubleIt->first])[j];
               }
               for (vecIt = out_customVectorAttrArrays.begin(); vecIt != out_customVectorAttrArrays.end(); vecIt++)
               {
                  (*out_customVectorAttrArrays[vecIt->first][step])[it->second] = (*instant_customVectorAttrArrays[vecIt->first])[j];
               }
               for (intIt = out_customIntAttrArrays.begin(); intIt != out_customIntAttrArrays.end(); intIt++)
               {
                  (*out_customIntAttrArrays[intIt->first][step])[it->second] = (*instant_customIntAttrArrays[intIt->first])[j];
               }
            }
            tempMap.erase(it);
         }
         else   // new particle this step
         {
            newParticleCount ++;
            //add new particles to the  arrays

            particleIDMap[particleId[j]] = (*out_positionArrays[step]).length();
            for (uint k = 0; k < totalSteps; k++)
            {
               // add particle to this steps arrays
               int s = (k-step);
               if (s != 0)
               {
                  // this calculates the "Velocity substep" which is the delta position based on the particle's current velocity
                  // and  the step we're on.
                  MVector velocitySubstep = (((velocityArray[j]/fps)*GetMotionByFrame())/(GetNumMotionSteps()-1))*s;
                  MVector oldVeloPosition = (*positionArray)[j] + velocitySubstep;
                  (*out_positionArrays[k]).append(oldVeloPosition);
               }
               else
               {
                  (*out_positionArrays[k]).append((*positionArray)[j]);
               }

               instantVeloArray.append(velocityArray[j]);

               if (m_hasRGB)
               {
                  (*out_colorArrays[k]).append((*rgbArray)[j]);
               }

               if (m_hasOpacity)
               {
                  (*out_opacityArrays[k]).append((*opacityArray)[j]);
               }

               if (m_isSprite)
               {
                  if (m_isSpritePP)
                  {
                     (*out_spriteScaleXArrays[k]).append((*spriteScaleXPP)[j]);
                     (*out_spriteScaleYArrays[k]).append((*spriteScaleYPP)[j]);
                  }
                  else
                  {
                     (*out_spriteScaleXArrays[k]).append(m_spriteScaleX);
                     (*out_spriteScaleYArrays[k]).append(m_spriteScaleY);
                  }
               }
               else
               {
                     (*out_radiusArrays[k]).append((*radiusArray)[j]);
               }

               if (m_doExtraAttributes)
               {
                  // Extra export attrs
                  for (doubleIt = out_customDoubleAttrArrays.begin(); doubleIt != out_customDoubleAttrArrays.end(); doubleIt++)
                  {
                     (*out_customDoubleAttrArrays[doubleIt->first][k]).append((*instant_customDoubleAttrArrays[doubleIt->first])[j]);
                  }
                  for (vecIt = out_customVectorAttrArrays.begin(); vecIt != out_customVectorAttrArrays.end(); vecIt++)
                  {
                     (*out_customVectorAttrArrays[vecIt->first][k]).append((*instant_customVectorAttrArrays[vecIt->first])[j]);
                  }
                  for (intIt = out_customIntAttrArrays.begin(); intIt != out_customIntAttrArrays.end(); intIt++)
                  {
                     (*out_customIntAttrArrays[intIt->first][k]).append((*instant_customIntAttrArrays[intIt->first])[j]);
                  }
               }
            }
         }
      }

      // delete the  pointers to the instant array values
      for (doubleIt = out_customDoubleAttrArrays.begin(); doubleIt != out_customDoubleAttrArrays.end(); doubleIt++)
      {
         delete instant_customDoubleAttrArrays[doubleIt->first];
      }
      for (vecIt = out_customVectorAttrArrays.begin(); vecIt != out_customVectorAttrArrays.end(); vecIt++)
      {
         delete instant_customVectorAttrArrays[vecIt->first];
      }
      for (intIt = out_customIntAttrArrays.begin(); intIt != out_customIntAttrArrays.end(); intIt++)
      {
         delete instant_customIntAttrArrays[intIt->first];      }



      AiMsgInfo("[mtoa] Particle Export found %i new particles for this step",newParticleCount);

      if (tempMap.size() > 0)
      {
         AiMsgInfo("[mtoa] Particle Export found %i particles that died for this step, computing velocity...", (int)tempMap.size());

         for (it = tempMap.begin(); it != tempMap.end(); it++)
         {
            // here we Support removing of dead particles via looping and culling by particle map in the final output loop
            if (m_deleteDeadParticles)
            {
               particleIDMap[it->first] = -1;
            }
            else
            {
               // get last step's position
               MVector velocitySubstep = (((instantVeloArray[it->second]/fps)*GetMotionByFrame())/(GetNumMotionSteps()-1));
               MVector newVeloPosition = (*out_positionArrays[step-1])[it->second] + velocitySubstep;
               (*out_positionArrays[step])[it->second] = newVeloPosition;
            }
         }
      }
   }
   if (step == (GetNumMotionSteps()-1)) // Last step only
   {

      m_particleCount = (*out_positionArrays[0]).length();

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
         // FIXED
         // FIXME: there is a utility for this on CNodeTranslator

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

      if (exportId)
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
         for (it = particleIDMap.begin(); it != particleIDMap.end();  it++)
         {
            if (it->second >= 0)
            {
               for (int j = 0; j< m_multiCount; j++)
               {

                  m_v = (*out_positionArrays[s])[it->second];

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
                     a_a = (*out_spriteScaleXArrays[s])[it->second]/(*out_spriteScaleYArrays[s])[it->second];
                     AiArraySetFlt (a_aspectArray, index, (AtFloat)a_a);

                     a_r = CLAMP((((AtFloat)(*out_spriteScaleXArrays[s])[it->second])/2), minRadius, maxRadius);
                     a_r *= radiusMult;

                  }
                  else
                  {
                     a_r = CLAMP(((AtFloat)(*out_radiusArrays[s])[it->second]), minRadius, maxRadius);
                     a_r *= radiusMult;
                  }

                  AiArraySetFlt(a_radiusArray, index, a_r);

                  if (exportId)
                  {
                     AiArraySetInt(a_ParticleIdArray, index, (int)it->first);
                  }
                  if (m_doMultiPoint)
                  {
                     AiArraySetInt(a_ParticleMultiIndexArray, index, (int)j);
                  }
                  if (m_hasRGB)
                  {
                     m_rgb = (*out_colorArrays[s])[it->second];
                     a_rgb.r = (AtFloat)m_rgb.x;
                     a_rgb.g = (AtFloat)m_rgb.y;
                     a_rgb.b = (AtFloat)m_rgb.z;
                     AiArraySetRGB(a_rgbPPArray, index, a_rgb);
                  }
                  if (m_hasOpacity)
                  {
                     AiArraySetFlt(a_opacityPPArray, index,  (*out_opacityArrays[s])[it->second]);
                  }

               }// end  multicount

            }// if particle is valid (did not die within motion blur steps)

            i++;  // tracking the  iteration thru the map

         }// end particleIDMap  iteration

         // CLEAN UP MEMORY
         std::cout << "cleaning up memory " << std::endl;
         delete out_positionArrays[s];

         if (m_hasRGB)
         {
            delete out_colorArrays[s];
         }
         if (m_hasOpacity)
         {
            delete out_opacityArrays[s];
         }
         if (m_isSprite)
         {
            delete out_spriteScaleXArrays[s];
            delete out_spriteScaleYArrays[s];
         }
         else
         {
            delete out_radiusArrays[s];
         }
      }// end  numMotionSteps


      //write the points
      AiNodeSetArray(particle, "points", a_positionArray);

      //write the radius
      AiNodeSetArray(particle, "radius", a_radiusArray);

      if (renderType == m_renderTypeSprite)
      {
         AiNodeSetArray(particle, "aspect", a_aspectArray);
      }

      if (exportId)
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
         std::cout << "doing extra attributes " << std::endl;
         // Extra export attrs
         for (doubleIt = out_customDoubleAttrArrays.begin(); doubleIt != out_customDoubleAttrArrays.end(); doubleIt++)
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
                     AiArraySetFlt(a_attributes, index, (*out_customDoubleAttrArrays[doubleIt->first][s])[i]);
                  }
               }
               // memory cleanup
               std::cout << "cleaning up extra Double attr memory " << out_customDoubleAttrArrays[doubleIt->first][s] << std::endl;
               delete out_customDoubleAttrArrays[doubleIt->first][s];
            }
            AiNodeDeclare(particle, doubleIt->first.c_str(), "uniform FLOAT");
            AiNodeSetArray(particle, doubleIt->first.c_str(), a_attributes);

         }
         for (vecIt = out_customVectorAttrArrays.begin(); vecIt != out_customVectorAttrArrays.end(); vecIt++)
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

                     a_attr.x = (AtFloat)(*out_customVectorAttrArrays[vecIt->first][s])[i].x;
                     a_attr.y = (AtFloat)(*out_customVectorAttrArrays[vecIt->first][s])[i].y;
                     a_attr.z = (AtFloat)(*out_customVectorAttrArrays[vecIt->first][s])[i].z;

                     AiArraySetVec(a_attributes, index, a_attr);
                  }
               }
               // memory cleanup
               std::cout << "cleaning up extra Vector attr memory " << out_customVectorAttrArrays[vecIt->first][s] << std::endl;
               delete out_customVectorAttrArrays[vecIt->first][s];
            }
            AiNodeDeclare(particle, vecIt->first.c_str(), "uniform VECTOR");
            AiNodeSetArray(particle, vecIt->first.c_str(), a_attributes);

         }
         for(intIt = out_customIntAttrArrays.begin(); intIt != out_customIntAttrArrays.end(); intIt++)
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
                    AiArraySetInt(a_attributes, index, (*out_customIntAttrArrays[intIt->first][s])[i]);
                 }
              }
              // memory cleanup
              std::cout << "cleaning up extra Int attr memory " << out_customIntAttrArrays[vecIt->first][s] << std::endl;
              delete out_customIntAttrArrays[vecIt->first][s];
           }
           AiNodeDeclare(particle, intIt->first.c_str(), "uniform INT");
           AiNodeSetArray(particle, intIt->first.c_str(), a_attributes);
         }

      }

   }// end  last step export
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

AtNode* CParticleTranslator::ExportParticle(AtNode* particle, bool update)
{
   MTimer exportParticleTimer;
   exportParticleTimer.beginTimer();
   AiNodeSetStr(particle, "name", m_dagPath.partialPathName().asChar());

   ExportMatrix(particle, 0);
   ExportParticleShaders(particle);
   if (!update)
   {
     ExportParticleData(particle, 0);
   }
   exportParticleTimer.endTimer();
   double elapsed = exportParticleTimer.elapsedTime();
   AiMsgInfo("[mtoa] Particle Export took : %f seconds",elapsed);
   return particle;
}

void CParticleTranslator::Update(AtNode *anode)
{
   ExportParticle(anode, true);
}

void CParticleTranslator::ExportMotion(AtNode* anode, AtUInt step)
{
   if (m_isMasterDag)
   {
      ExportMatrix(anode, step);
      if (m_motionDeform)
      {
         ExportParticleData(anode, step);
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
      ExportParticle(anode, false);
   else
      ExportInstance(anode, m_masterDag);


}
