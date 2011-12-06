
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

   data.defaultValue.BOOL = true;
   data.name = "export_particleId";
   data.shortName = "exppartid";
   helper.MakeInputBoolean(data);

   data.defaultValue.STR = "";
   data.name = "export_attributes";
   data.shortName = "expartattr";
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

   data.defaultValue.FLT = 1000000;
   data.name = "max_particle_radius";
   data.shortName = "maxParticleRadius";
   helper.MakeInputFloat(data);

   data.defaultValue.FLT = 0;
   data.name = "min_pixel_width";
   data.shortName = "minPixelWidth";
   helper.MakeInputFloat(data);

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

MObject CParticleTranslator::GetNodeShadingGroup(MObject dagNode, int instanceNum)
{
   MPlugArray        connections;
   MFnDependencyNode fnDGNode(dagNode);

   MPlug plug(dagNode, fnDGNode.attribute("instObjGroups"));

   plug.elementByLogicalIndex(instanceNum).connectedTo(connections, false, true);

   for (unsigned int k=0; k<connections.length(); ++k)
   {
      MObject shadingGroup(connections[k].node());
      if (shadingGroup.apiType() == MFn::kShadingEngine)
      {
         return shadingGroup;
      }
   }
   return MObject::kNullObj;
}


void CParticleTranslator::ExportCustomParticleData(AtNode* particle, AtUInt step)
{
   //first, parsing the m_customAttrs

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
            MDoubleArray attributes;
            uint size = m_fnParticleSystem.getPerParticleAttribute(currentAttr, attributes);

            AtArray* a_attributes = AiArrayAllocate(size, 1, AI_TYPE_FLOAT);
            for (uint j = 0; j < size; j++)
            {
               AiArraySetFlt(a_attributes, j + (step * size), attributes[j]);
            }
            AiNodeDeclare(particle, currentAttr.asChar(), "uniform FLOAT");
            AiNodeSetArray(particle, currentAttr.asChar(), a_attributes);
            continue;
         }
         else if (m_fnParticleSystem.isPerParticleVectorAttribute(currentAttr))
         {
            MVectorArray attributes;
            uint size = m_fnParticleSystem.getPerParticleAttribute(currentAttr, attributes);
            AtArray* a_attributes = AiArrayAllocate(size, 1, AI_TYPE_VECTOR);
            AtVector a_attr;
            for (uint j = 0; j < size; j++)
            {
               a_attr.x = (AtFloat)attributes[j].x;
               a_attr.y = (AtFloat)attributes[j].y;
               a_attr.z = (AtFloat)attributes[j].z;

               AiArraySetVec(a_attributes, j + (step * size), a_attr);
            }
            AiNodeDeclare(particle, currentAttr.asChar(), "uniform VECTOR");
            AiNodeSetArray(particle, currentAttr.asChar(), a_attributes);
            continue;

         }
         else if (m_fnParticleSystem.isPerParticleIntAttribute(currentAttr))
         {
            MIntArray attributes;
            uint size = m_fnParticleSystem.getPerParticleAttribute(currentAttr, attributes);
            AtArray* a_attributes = AiArrayAllocate(size, 1, AI_TYPE_INT);
            for (uint j = 0; j < size; j++)
            {
                  AiArraySetInt(a_attributes, j + (step * size), attributes[j]);
            }
            AiNodeDeclare(particle, currentAttr.asChar(), "uniform INT");
            AiNodeSetArray(particle, currentAttr.asChar(), a_attributes);
            continue;

         }
      }
   }
}


// Export particle datas with basic attributes rgbPP, OpacityPP, radiusPP and particleId

void CParticleTranslator::ExportParticleData(AtNode* particle, AtUInt step)
{

   //FIXME : need cleaner code, concatenate all the conditions.

   int numParticles;
   int multiCount = 1;
   float multiRadius = 0;
   if (step == 0)
   {
      numParticles = m_fnParticleSystem.count();
      m_particleCount = numParticles;
   }
   else
   {
      numParticles = m_particleCount;
   }

   AiMsgInfo("[mtoa] Particle count : %i",m_particleCount);

   int renderType = m_fnParticleSystem.renderType();
   // RENDERTYPE list:
   // 3 = multiPoint
   // 4 = multiStreak
   // 5 = numeric
   // 6 = points
   // 7 = spheres
   // 8 = Sprites
   // 9 = streaks
   // 2 = blobbySurface
   // 0 = cloud
   // 1 = tube

   AiMsgInfo("ParticleType %i", renderType);

   float minPixelWidth = m_fnParticleSystem.findPlug("minPixelWidth").asFloat();
   AiNodeSetFlt(particle, "min_pixel_width", minPixelWidth);

   int  pointsAs     = m_fnParticleSystem.findPlug("renderPointsAs").asInt();
   float minRadius = m_fnParticleSystem.findPlug("minParticleRadius").asFloat();
   float maxRadius = m_fnParticleSystem.findPlug("maxParticleRadius").asFloat();


   switch (renderType)
   {
      case 7: // sphere
         AiNodeSetStr(particle, "mode", "sphere");
         break;
      case 8: // sprite
         AiNodeSetStr(particle, "mode", "quad");
         break;
      case 2: // blobby surface
         AiNodeSetStr(particle, "mode", "sphere");
         break;
      case 0: // cloud
         AiNodeSetStr(particle, "mode", "sphere");
         break;
      case 1: // tube
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

   AtArray* a_positionArray(NULL);
   AtArray* a_ParticleIdArray(NULL);
   AtArray* a_radiusArray(NULL);
   AtArray* a_aspectArray(NULL);
   AtArray* a_rgbPPArray(NULL);
   AtArray* a_opacityPPArray(NULL);

   MVectorArray positionArray;
   MIntArray particleId;
   MDoubleArray radiusArray;
   MDoubleArray spriteScaleXPP;
   MDoubleArray spriteScaleYPP;
   MVectorArray rgbArray;
   MDoubleArray opacityArray;

   MVector m_rgb;
   AtRGB a_rgb;

   // FIXME: optimize step > 0.  many calculations are occuring for all steps that should only occur on step == 0
   //ask if the particle has the basic PP attributes we want to export
   bool exportId     = m_fnParticleSystem.findPlug("export_particleId").asBool();
   bool hasRadiusPP  = m_fnParticleSystem.isPerParticleDoubleAttribute(MString("radiusPP"));
   bool hasRgb       = m_fnParticleSystem.hasRgb();

   bool isSpritePP   = false;
   bool doMultiPoint = false;

   // opaqueness
   bool hasOpacity = false;
   bool isOpaque  = m_fnParticleSystem.findPlug("opaque").asBool();
   if (!isOpaque)
      hasOpacity = m_fnParticleSystem.hasOpacity();
   AiNodeSetBool(particle, "opaque", isOpaque);

   // get the arrays of standard attributes

   m_fnParticleSystem.position(positionArray);


   // RadiusPP overrides everything
   if (hasRadiusPP && renderType != 8)
   {
      m_fnParticleSystem.getPerParticleAttribute(MString("radiusPP"), radiusArray);
   }
   else
   {
      MStatus status;
      float particleSize= 1;
      float radius = .5;
      float pointSize = 1;
      float lineWidth = 1;
      float spriteScaleX = 1;
      float spriteScaleY = 1;
      MPlug radiusPlug( m_fnParticleSystem.findPlug("radius", &status));
      if ( MS::kSuccess == status )
      {
         radiusPlug.getValue( radius );
      }
      MPlug pointPlug( m_fnParticleSystem.findPlug("pointSize", &status));
      if ( MS::kSuccess == status )
      {
         pointPlug.getValue( pointSize );
      }
      MPlug linePlug( m_fnParticleSystem.findPlug("lineWidth", &status));
      if ( MS::kSuccess == status )
      {
         linePlug.getValue( lineWidth );
      }
      MPlug ssXPlug( m_fnParticleSystem.findPlug("spriteScaleX", &status));
      if ( MS::kSuccess == status )
      {
         ssXPlug.getValue( spriteScaleX );
      }
      MPlug ssYPlug( m_fnParticleSystem.findPlug("spriteScaleY", &status));
      if ( MS::kSuccess == status )
      {
         ssYPlug.getValue( spriteScaleY );
      }

      switch (renderType)
      {
         case 3: // multiPoint
            {
               particleSize = (pointSize)*0.01;
               doMultiPoint = true;
               MPlug mcPlug( m_fnParticleSystem.findPlug("multiCount", &status));
               if ( MS::kSuccess == status )
               {
                  mcPlug.getValue( multiCount );
               }
               MPlug mrPlug( m_fnParticleSystem.findPlug("multiRadius", &status));
               if ( MS::kSuccess == status )
               {
                  mrPlug.getValue( multiRadius );
               }
               multiRadius *=2;
            }
            break;
         case 7: // sphere
            particleSize = radius;
            break;
         case 8: // sprite
            {
               if (m_fnParticleSystem.isPerParticleDoubleAttribute(MString("spriteScaleXPP")) &&
                   m_fnParticleSystem.isPerParticleDoubleAttribute(MString("spriteScaleYPP")))
               {
                  isSpritePP = true;
                  AiMsgInfo("SPRITESCALE and Y PP values found");
                  m_fnParticleSystem.getPerParticleAttribute(MString("spriteScaleXPP"), spriteScaleXPP);
                  m_fnParticleSystem.getPerParticleAttribute(MString("spriteScaleYPP"), spriteScaleYPP);
               }

               //float aspect = spriteScaleX/spriteScaleY;
               particleSize = spriteScaleX/2;

            }
            break;
         case 9: // streak
            {
               MGlobal::displayWarning("[mtoa]: Streak particle type is not yet supported");
               AiMsgInfo("[mtoa] Streak particle type is not yet supported");
               particleSize = (lineWidth)*0.01;
            }
            break;
         case 2:// blobby
            {
               MGlobal::displayWarning("[mtoa]: Blobby particle type is not yet supported");
               AiMsgInfo("[mtoa] Blobby particle type is not yet supported");
               particleSize = radius;
            }
            break;
         case 0:// cloud
            {
               MGlobal::displayWarning("[mtoa]: Cloud particle type is not yet supported");
               AiMsgInfo("[mtoa] Cloud particle type is not yet supported");
               particleSize = radius;
            }
            break;
         case 1:// tube
            {
               MGlobal::displayWarning("[mtoa]: Tube particle type is not yet supported");
               AiMsgInfo("[mtoa] Tube particle type is not yet supported");
               particleSize = radius;
            }
            break;
         default:
            particleSize = (pointSize)*0.01;
      }

      // populate the radius array with the value
      for (int i=0; i<numParticles; i++)
      {
         for (int j=0; j<multiCount; j++)
         {
            if (renderType == 8 && !isSpritePP)
            {
               spriteScaleXPP.append(spriteScaleX);
               spriteScaleYPP.append(spriteScaleY);
            }
            else
            {
               radiusArray.append(particleSize);
            }
         }
      }
   } // End No RadiusPP

   AiMsgInfo("MAYA ARRAYS DONE");
   if (hasRgb)
      a_rgbPPArray = AiArrayAllocate(numParticles*multiCount, 1, AI_TYPE_RGB);

   if (hasOpacity)
      a_opacityPPArray = AiArrayAllocate(numParticles*multiCount, 1, AI_TYPE_FLOAT);

   if (exportId)
   {
      a_ParticleIdArray = AiArrayAllocate(numParticles*multiCount, 1, AI_TYPE_INT);
      m_fnParticleSystem.particleIds(particleId);
   }

   /// Allocation of Arnold Arrays
   if (step == 0)
   {
      // allocate
       if (m_motionDeform || m_motion)
       {
          a_positionArray = AiArrayAllocate(numParticles*multiCount, GetNumMotionSteps(), AI_TYPE_POINT);
          a_radiusArray = AiArrayAllocate(numParticles*multiCount, GetNumMotionSteps(), AI_TYPE_FLOAT) ;
          if (renderType == 8)
          {
             a_aspectArray = AiArrayAllocate(numParticles*multiCount, GetNumMotionSteps(), AI_TYPE_FLOAT);
          }
       }
       else
       {
          a_positionArray = AiArrayAllocate(numParticles*multiCount, 1, AI_TYPE_POINT);
          a_radiusArray = AiArrayAllocate(numParticles*multiCount, 1, AI_TYPE_FLOAT) ;
          if (renderType == 8)
          {
             a_aspectArray = AiArrayAllocate(numParticles*multiCount, 1, AI_TYPE_FLOAT);
          }
       }

      //write the points
      AiNodeSetArray(particle, "points", a_positionArray);

      //write the radius
      AiNodeSetArray(particle, "radius", a_radiusArray);

      if (renderType == 8)
      {
         AiNodeSetArray(particle, "aspect", a_aspectArray);
      }

      // get the array of rgbPPs
      if (hasRgb)
         m_fnParticleSystem.rgb(rgbArray);

      // get the array of opacities
      if (hasOpacity)
         m_fnParticleSystem.opacity(opacityArray);

      if (hasRgb || hasOpacity || exportId)
      {
         for (int i=0; i<numParticles; i++)
         {
            for (int j=0; j<multiCount; j++)
            {
               if (hasRgb)
               {
                  m_rgb = rgbArray[i];
                  a_rgb.r = (AtFloat)m_rgb.x;
                  a_rgb.g = (AtFloat)m_rgb.y;
                  a_rgb.b = (AtFloat)m_rgb.z;
                  AiArraySetRGB(a_rgbPPArray, ((i*multiCount)+j), a_rgb);
               }

               if (hasOpacity)
               {
                  AiArraySetFlt(a_opacityPPArray, ((i*multiCount)+j), opacityArray[i]);
               }

               if (exportId)
               {
                  AiArraySetInt(a_ParticleIdArray, ((i*multiCount)+j), (int)particleId[i]);
               }
            }
         }
      }

      // add rgb per point attribute
      if (hasRgb)
      {
         AiNodeDeclare(particle, "rgbPP", "uniform RGB");
         AiNodeSetArray(particle, "rgbPP", a_rgbPPArray);
      }

      // add opacity per point attribute
      // no need to export opacityPP if the particle is opaque !
      if (hasOpacity)
      {
         AiNodeDeclare(particle, "opacityPP", "uniform FLOAT");
         AiNodeSetArray(particle, "opacityPP", a_opacityPPArray);
      }

      // add particle id
      if (exportId)
      {
         AiNodeDeclare(particle, "particleId", "uniform INT");
         AiNodeSetArray(particle, "particleId", a_ParticleIdArray);
      }

   }
   else // step > 0
   {
      // get curves node and arrays
      // we may at this point need to get the velocity and  add it to position somehow
      // FIXME: don't use lookupByName, use CTranslator::GetArnoldRootNode
      particle = AiNodeLookUpByName(m_dagPath.partialPathName().asChar());

      a_positionArray = AiNodeGetArray(particle, "points");
      a_radiusArray = AiNodeGetArray(particle, "radius");
      a_aspectArray = AiNodeGetArray(particle, "aspect");

   }
   AiMsgInfo("Anold Motionblur stuff done");
   /// Finally set the  position and  radius values with their cache values minus Transform position
   AtPoint a_v;
   MVector m_v;
   AtFloat a_r;
   AtFloat a_a;
   //getting the matrix of the point particle object

   MMatrix mpm = m_dagPath.inclusiveMatrix();

   // convert it to AtMatrix
   AtMatrix apm;
   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         apm[I][J] = (float) mpm[I][J];
      }
   }
   // inverting the matrix
   AiM4Invert (apm, apm);

   for (int i = 0; i < numParticles; i++)
   {
      for (int j = 0; j<multiCount; j++)
      {
         // ADD IN MULTIPOINT NOISE HERE
         m_v = positionArray[i];
         AtPoint noisePoint;
         noisePoint.x = m_v.x+i+j+.1454329;
         noisePoint.y = m_v.y-i+j+.3234548;
         noisePoint.z = m_v.z+i+j+.09210;

         a_v.x = (AtFloat)m_v.x+(AiPerlin4(noisePoint,i+j+.234)*multiRadius);
         a_v.y = (AtFloat)m_v.y+(AiPerlin4(noisePoint,i+j+23)*multiRadius);
         a_v.z = (AtFloat)m_v.z+(AiPerlin4(noisePoint,i+j-.487)*multiRadius);

         //remove matrix for the point
         AiM4PointByMatrixMult (&a_v, apm, &a_v);

         AiArraySetPnt(a_positionArray, (((i*multiCount)+j) + (step * numParticles)), a_v);

         if (renderType == 8)
         {
            a_a = spriteScaleXPP[i]/spriteScaleYPP[i];
            AiArraySetFlt (a_aspectArray, (((i*multiCount)+j) + (step * numParticles)), (AtFloat)a_a);
            a_r = CLAMP(((AtFloat)spriteScaleXPP[i]/2), minRadius, maxRadius);
         }
         else
         {
            a_r = CLAMP(((AtFloat)radiusArray[i]), minRadius, maxRadius);
         }

         AiArraySetFlt(a_radiusArray, (((i*multiCount)+j) + (step * numParticles)), a_r);
      }

   }
   AiMsgInfo("Done EXPORT INSTANCE");

}

// FIXME: how does this differ from CGeometryTranslator::ExportInstance? why not use that?
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

   AiNodeSetStr(particle, "name", m_dagPath.partialPathName().asChar());

   //MFnParticleSystem::RenderType renderType = m_fnParticleSystem.renderType();

   //set particle type
   //if (renderType == 7)
   //   AiNodeSetStr(particle, "mode", "sphere");
   //else
   //   AiNodeSetStr(particle, "mode", "disk");

   ExportMatrix(particle, 0);
   ExportParticleShaders(particle);
   if (!update)
       ExportParticleData(particle, 0);

       m_customAttrs = m_fnParticleSystem.findPlug("export_attributes").asString();

       if (m_customAttrs.length() != 0)
       ExportCustomParticleData(particle, 0);


   return particle;
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

   // FIXME: dont' use string comparison here, use master DAG check from CreateArnoldNodes
   const char* nodeType = AiNodeEntryGetName(anode->base_node);

   if (strcmp(nodeType, "ginstance") == 0)
      ExportInstance(anode, m_masterDag);
   else if (strcmp(nodeType, "points") == 0)
      ExportParticle(anode, false);

}
