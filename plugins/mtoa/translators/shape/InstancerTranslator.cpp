#include "InstancerTranslator.h"
#include <maya/MFnDagNode.h>
#include <scene/MayaScene.h>
#include "utils/time.h"



void addVelocityToMatrix(AtMatrix& outMatrix, AtMatrix& matrix,
                         const MVector& velocityVector)
{
   AiM4Copy(outMatrix, matrix);

   outMatrix[3][0] = matrix[3][0] + (float)velocityVector.x;
   outMatrix[3][1] = matrix[3][1] + (float)velocityVector.y;
   outMatrix[3][2] = matrix[3][2] + (float)velocityVector.z;

}

void CInstancerTranslator::NodeInitializer(CAbTranslator context)
{
}

AtNode* CInstancerTranslator::CreateArnoldNodes()
{
   // FIXME  this master dag return needs to be null because it doesen't contain anything, the instancer
   // produces individual ginstances for each particle instance
   if (IsMasterInstance())
      return  AddArnoldNode("ginstance");
   else
      return  AddArnoldNode("ginstance");

   // the code above is interesting....
}


void CInstancerTranslator::Export(AtNode* anode)
{
   ExportMatrix(anode);
   ExportInstances(anode);
}

void CInstancerTranslator::ExportMotion(AtNode* anode)
{
   ExportMatrix(anode);

   if (IsMasterInstance() && m_motionDeform)
   {
      ExportInstances(anode);
   }
}

AtByte CInstancerTranslator::ComputeMasterVisibility(const MDagPath& masterDagPath) const{

   MObject node = masterDagPath.node();
   MFnDagNode fnNode;
   fnNode.setObject(node);
   
   AtByte visibility = AI_RAY_ALL;
   
   MPlug plug = fnNode.findPlug("castsShadows");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_SHADOW;
   }

   plug = fnNode.findPlug("primaryVisibility");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_CAMERA;
   }

   plug = fnNode.findPlug("visibleInReflections");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_REFLECTED;
   }

   plug = fnNode.findPlug("visibleInRefractions");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_REFRACTED;
   }

   plug = fnNode.findPlug("aiVisibleInDiffuse");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_DIFFUSE;
   }

   plug = fnNode.findPlug("aiVisibleInGlossy");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_GLOSSY;
   }
   
   return visibility;
}


void CInstancerTranslator::ExportInstances(AtNode* instancer)
{
   unsigned int step = GetMotionStep();
   MTime oneSec(1.0, MTime::kSeconds);
   // FIXME: was it intended to be rounded to int ?
   float fps =  (float)oneSec.asUnits(MTime::uiUnit());
   //unsigned int middleStep = ((totalSteps/2)+1)-1;

   MFnInstancer m_fnMayaInstancer(m_dagPath);
   m_particleCount = m_fnMayaInstancer.particleCount();


   MPlugArray conn;
   // the particleShape attached
   MFnDependencyNode depNodeInstancer(m_dagPath.node());

   MPlug inputPointsPlug = depNodeInstancer.findPlug("inputPoints");
   inputPointsPlug.connectedTo(conn, true, false);

   MObject inputPointsData = inputPointsPlug.asMObject();

   // inputPoints is not an array, so position [0] is the particleShape node
   MObject particleShape = conn[0].node();

   MString particleName;
   MFnDependencyNode fnParticle;
   MString idAttrName;


   MIntArray  partIds;
   MVectorArray velocities;
   MFnArrayAttrsData::Type vectorType(MFnArrayAttrsData::kVectorArray);
   MFnArrayAttrsData::Type doubleType(MFnArrayAttrsData::kDoubleArray);
   MFnArrayAttrsData::Type intType(MFnArrayAttrsData::kIntArray);
   MFnArrayAttrsData::Type arrayType;

   bool usingParticleSource = true;
   MFnArrayAttrsData instancerData;

   MDagPathArray paths;
   MMatrixArray mayaMatrices;
   MIntArray particlePathStartIndices;
   MIntArray pathIndices;

   MStatus  status;


   status = m_fnMayaInstancer.allInstances(paths, mayaMatrices, particlePathStartIndices, pathIndices);
   CHECK_MSTATUS(status);

   if (particleShape.apiType() == MFn::kParticle || particleShape.apiType() == MFn::kNParticle )
   {
      m_fnParticleSystem.setObject(particleShape);
      AiMsgDebug("[mtoa] Instancer %s for particle instancer %s exporting instances for step %i",
            m_fnMayaInstancer.partialPathName().asChar(), m_fnParticleSystem.partialPathName().asChar(), step);
      m_fnParticleSystem.particleIds(partIds);
      m_fnParticleSystem.velocity(velocities);
      
      particleName = m_fnParticleSystem.partialPathName();
   }
   // if particles are not connected to the instancer it must be being fed by something else
   // like a plugin  node of some kind.
   // we have to just use whats given to us on the input array
   else
   {

      usingParticleSource = false;
      MString arrayName;
      fnParticle.setObject(particleShape);
      instancerData.setObject(inputPointsData);
      // check 'particleId' first and 'id'
      arrayName = "particleId";
      if (!instancerData.checkArrayExist(arrayName, arrayType) || (arrayType != intType && arrayType != doubleType))
      {
         arrayName = "id";
         if (!instancerData.checkArrayExist(arrayName, arrayType) || (arrayType != intType && arrayType != doubleType))
         {
            arrayName = "";
         }
      }
      if (arrayName.length() > 0)
      {
         idAttrName = arrayName;
         if (arrayType == intType)
         {
            partIds = instancerData.getIntData(arrayName);
         }
         else
         {
            MDoubleArray idArray = instancerData.getDoubleData(arrayName);
            partIds.setLength(idArray.length());
            for (unsigned int i = 0; i< idArray.length(); i++)
            {
               partIds[i] = (int) idArray[i];
            }
         }
      }
      else
      {
         partIds.setLength(mayaMatrices.length());
         for (unsigned int i = 0; i< mayaMatrices.length(); i++)
         {
            partIds[i] = (int)i;
         }
      }

      arrayName = "velocity";
      if (instancerData.checkArrayExist(arrayName, arrayType) && arrayType == vectorType)
      {
         velocities = instancerData.getVectorData(arrayName);
      }
      particleName = fnParticle.name();
   }

   // if deform blur is disabled, this function will only be called for current frame
   bool hasMotion = (RequiresMotionData() && m_motionDeform);

   bool exportID = false;
   MStringArray attrs;
   if( usingParticleSource )
   {
      exportID = m_fnParticleSystem.findPlug("aiExportParticleIDs").asBool();

      /// STORE the custom attrs for use later
      m_customAttrs = m_fnParticleSystem.findPlug("aiExportAttributes").asString();

      status = m_customAttrs.split(' ', attrs);

      m_instant_customDoubleAttrArrays.clear();
      m_instant_customVectorAttrArrays.clear();
      m_instant_customIntAttrArrays.clear();

      // we have to do this no matter if we have particles or not..
      if (m_customAttrs.length() != 0)
      {
         if (status ==  MS::kSuccess)
         {
            for (unsigned int i=0; i < attrs.length(); i++)
            {
               MString currentAttr = attrs[i];

               if (currentAttr == "particleId")
               {
                  exportID = true;
                  continue;
               }

               m_fnParticleSystem.findPlug(currentAttr, &status);
               if (status != MS::kSuccess)
                  continue;

               //check the type of the plug
               if (m_fnParticleSystem.isPerParticleDoubleAttribute(currentAttr))
               {
                  MDoubleArray doubleAttribute;
                  m_fnParticleSystem.getPerParticleAttribute(currentAttr, doubleAttribute);
                  m_instant_customDoubleAttrArrays[currentAttr.asChar()]= doubleAttribute;
                  continue;
               }
               else if (m_fnParticleSystem.isPerParticleVectorAttribute(currentAttr))
               {
                  MVectorArray vectorAttribute;
                  m_fnParticleSystem.getPerParticleAttribute(currentAttr, vectorAttribute);
                  m_instant_customVectorAttrArrays[currentAttr.asChar()] = vectorAttribute;
                  continue;
               }
               else if (m_fnParticleSystem.isPerParticleIntAttribute(currentAttr))
               {
                  MIntArray intAttribute;
                  m_fnParticleSystem.getPerParticleAttribute(currentAttr, intAttribute);
                  m_instant_customIntAttrArrays[currentAttr.asChar()] = intAttribute;
                  continue;
               }
               else
               {
                  continue;
               }
            }
         }
      }
      
      if (exportID)
      {
         m_instant_customIntAttrArrays["particleId"]= partIds;
      }
   } else
   {
      MPlug plug = fnParticle.findPlug("aiExportParticleIDs");
      exportID = (!plug.isNull() && plug.asBool());

      plug = fnParticle.findPlug("aiExportAttributes");
      m_customAttrs = (plug.isNull() ? "" : plug.asString());

      status = m_customAttrs.split(' ', attrs);

      m_instant_customDoubleAttrArrays.clear();
      m_instant_customVectorAttrArrays.clear();
      m_instant_customIntAttrArrays.clear();

      if (status ==  MS::kSuccess && m_customAttrs.length() != 0)
      {
         for (unsigned int i=0; i < attrs.length(); i++)
         {
            MString currentAttr = attrs[i];

            if (currentAttr == idAttrName)
            {
               exportID = true;
               continue;
            }

            if (instancerData.checkArrayExist(currentAttr, arrayType))
            {
               if (arrayType == intType)
               {
                  m_instant_customIntAttrArrays[currentAttr.asChar()] = instancerData.getIntData(currentAttr);
               }
               else if (arrayType == doubleType)
               {
                  m_instant_customDoubleAttrArrays[currentAttr.asChar()]= instancerData.getDoubleData(currentAttr);
               }
               else if (arrayType == vectorType)
               {
                  m_instant_customVectorAttrArrays[currentAttr.asChar()] = instancerData.getVectorData(currentAttr);
               }
            }
         }
      }

      if (exportID)
      {
         m_instant_customIntAttrArrays["particleId"] = partIds;
      }
   }


   if (!IsExportingMotion())
   {
      m_exportedSteps.clear();
      m_exportedSteps.assign(GetNumMotionSteps(), false);
      m_exportedSteps[step] = true;
      m_instantVeloArray.clear();
      m_vec_matrixArrays.clear();
      m_out_customVectorAttrArrays.clear();
      m_out_customDoubleAttrArrays.clear();
      m_out_customIntAttrArrays.clear();
      m_particlePathsMap.clear();
      m_particleIDMap.clear();
      m_objectNames.clear();
      m_objectDagPaths.clear();

      if (mayaMatrices.length() > 0)
      {
         unsigned int nmtx = ((hasMotion) ? GetNumMotionSteps() : 1);
         
         for (unsigned int j = 0; j < mayaMatrices.length(); j++)
         {
            AtArray* outMatrix = AiArrayAllocate(1, nmtx, AI_TYPE_MATRIX);
            AtMatrix matrix;
            ConvertMatrix(matrix, mayaMatrices[j]);
            AiArraySetMtx(outMatrix, step, matrix);

            m_vec_matrixArrays.push_back(outMatrix);

            int id = partIds[j];
            m_particleIDMap[id] = j;
            int pathNumber = particlePathStartIndices[j + 1] - particlePathStartIndices[j];
            MIntArray paths;
            for (int i = 0; i < pathNumber; i++)
            {
               paths.append(pathIndices[particlePathStartIndices[j]+i]);
            }
            m_particlePathsMap[id] = paths;
            //m_instanceTags.append("originalParticle");
            if(velocities.length() > 0)
            {
               m_instantVeloArray.append(velocities[j]);
            }
         }
      }

      if (m_customAttrs.length() != 0 || exportID)
      {
         m_out_customVectorAttrArrays = m_instant_customVectorAttrArrays;
         m_out_customDoubleAttrArrays = m_instant_customDoubleAttrArrays;
         m_out_customIntAttrArrays    = m_instant_customIntAttrArrays;
      }

      /// export instance object masters
      int numObjects = paths.length();
      MFnDagNode fnDag;
      for (int i = 0; i < numObjects; ++i)
      {
         MDagPath dagPathMaster = paths[i];
         fnDag.setObject(dagPathMaster);
         // MFnInstancer.allInstances returns intermediate objects. D'oh!
         if (!fnDag.isIntermediateObject())
         {
            // Check if the node is in the scene already.
            AtNode* masterNode = AiNodeLookUpByName(CDagTranslator::GetArnoldNaming(dagPathMaster).asChar());
            // if not, we export it
            if (!masterNode)
            {
               // FIXME: check if the object will not be exported a second time later !
               ExportDagPath(dagPathMaster);
            }
         }
         m_objectNames.append(CDagTranslator::GetArnoldNaming(dagPathMaster).asChar());
         m_objectDagPaths.append(dagPathMaster);
      }
      // Done export object masters

   }
   else // motion Steps
   {
      int numMotionSteps = GetNumMotionSteps();
      m_exportedSteps[step] = true;
      unordered_map <int, int> tempMap = m_particleIDMap;
      unordered_map <int, int>::iterator it;
      if (mayaMatrices.length() > 0)
      {
         int newParticleCount = 0;
         for (unsigned int j = 0; j < mayaMatrices.length(); j++)
         {
            it = tempMap.find(partIds[j]);
            if (it != tempMap.end())   // found the particle in the scene already
            {
               AtMatrix matrix;
               ConvertMatrix(matrix, mayaMatrices[j]);
               // setting the matrix with the index corresponding to the original index
               if (it->second < (int)m_vec_matrixArrays.size())
                  AiArraySetMtx(m_vec_matrixArrays[it->second], step, matrix);
               

               if (velocities.length() > 0)
               {
                  // update instant velocity
                  if (it->second < (int)m_instantVeloArray.length())
                     m_instantVeloArray[it->second] = velocities[j];
               }
               tempMap.erase(it);
            }
            else  // found a new particle in this substep
            {
               newParticleCount++;
               AtArray* outMatrix = AiArrayAllocate(1, numMotionSteps, AI_TYPE_MATRIX);
               AtMatrix matrix;
               ConvertMatrix(matrix, mayaMatrices[j]);
               AiArraySetMtx(outMatrix, step, matrix);
               // now compute the previous steps velocity matrices
               for (int i = 0; i < numMotionSteps; i++)
               {
                  // if the motion step hasn't been computed yet, there's no
                  // need to fill it as we'll do it soon
                  if (i == step || m_exportedSteps[i] == false)
                     continue;

                  int k = (i-step);
                  MVector velocitySubstep(0, 0, 0);
                  if (velocities.length() > 0)
                     velocitySubstep = (((velocities[j]/fps)*GetMotionByFrame())/(numMotionSteps-1))*k;
                  AtMatrix substepMatrix;
                  addVelocityToMatrix (substepMatrix, matrix, velocitySubstep);
                  AiArraySetMtx(outMatrix, i, substepMatrix);
               }
               if (m_customAttrs.length() != 0)
               {
                  if (attrs.length() > 0)
                  {
                     if (usingParticleSource)
                     {
                        for (unsigned int i=0; i < attrs.length(); i++)
                        {

                           MString currentAttr = attrs[i];
                           if (currentAttr == "particleId")
                           {
                              exportID = true;
                              continue;
                           }
                           m_fnParticleSystem.findPlug(currentAttr, &status);
                           if (status != MS::kSuccess)
                              continue;

                           //check the type of the plug
                           if (m_fnParticleSystem.isPerParticleDoubleAttribute(currentAttr))
                           {
                            m_out_customDoubleAttrArrays[currentAttr.asChar()].append(m_instant_customDoubleAttrArrays[currentAttr.asChar()][j]);
                            continue;
                           }
                           else if (m_fnParticleSystem.isPerParticleVectorAttribute(currentAttr))
                           {
                            m_out_customVectorAttrArrays[currentAttr.asChar()].append(m_instant_customVectorAttrArrays[currentAttr.asChar()][j]);
                            continue;
                           }
                           else if (m_fnParticleSystem.isPerParticleIntAttribute(currentAttr))
                           {
                            m_out_customIntAttrArrays[currentAttr.asChar()].append(m_instant_customIntAttrArrays[currentAttr.asChar()][j]);
                            continue;
                           }
                        }
                     } else
                     {
                        for (unsigned int i=0; i < attrs.length(); i++)
                        {
                           MString currentAttr = attrs[i];
                           if (currentAttr == "particleId")
                           {
                              exportID = true;
                              continue;
                           }

                           if (instancerData.checkArrayExist(currentAttr, arrayType))
                           {
                              if (arrayType == intType)
                              {
                                 m_out_customIntAttrArrays[currentAttr.asChar()].append(m_instant_customIntAttrArrays[currentAttr.asChar()][j]);
                              }
                              else if (arrayType == doubleType)
                              {
                                 m_out_customDoubleAttrArrays[currentAttr.asChar()].append(m_instant_customDoubleAttrArrays[currentAttr.asChar()][j]);
                              }
                              else if (arrayType == vectorType)
                              {
                                 m_out_customVectorAttrArrays[currentAttr.asChar()].append(m_instant_customVectorAttrArrays[currentAttr.asChar()][j]);
                              }
                           }
                        }
                     }
                  }
               }
               
               if (exportID)
               {
                  m_out_customIntAttrArrays["particleId"].append(m_instant_customIntAttrArrays["particleId"][j]);
               }

               m_vec_matrixArrays.push_back(outMatrix);
               m_particleIDMap[partIds[j]] = m_vec_matrixArrays.size()-1;

               int pathNumber = particlePathStartIndices[j + 1] - particlePathStartIndices[j];
               MIntArray paths;
               for (int i = 0; i < pathNumber; i++)
               {
                  paths.append(pathIndices[particlePathStartIndices[j]+i]);
               }
               m_particlePathsMap[partIds[j]] = paths;

               //m_instanceTags.append("newParticle");
               if (velocities.length() > 0)
               {
                  m_instantVeloArray.append(velocities[j]);
               }
            }
         }
         AiMsgDebug("[mtoa] Instancer %s export for particle system %s found a %i new particles for step %i",
                  m_fnMayaInstancer.partialPathName().asChar(), m_fnParticleSystem.partialPathName().asChar(), newParticleCount, step);
      }
      if ((!m_vec_matrixArrays.empty()) && tempMap.size() > 0)
      {
         AiMsgDebug("[mtoa] Instancer %s export for particle system %s found %i particles that died for step %i, computing velocity...",
            m_fnMayaInstancer.partialPathName().asChar(), m_fnParticleSystem.partialPathName().asChar(), (int)tempMap.size(), step);

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

         for (it = tempMap.begin(); it != tempMap.end(); it++)
         {

            // get last steps  matrix
            MVector velocitySubstep(0, 0, 0);
            if (velocities.length() > 0)
               velocitySubstep = (((m_instantVeloArray[it->second]/fps)*GetMotionByFrame())/(numMotionSteps-1)) * (step - previousStep);
            AtMatrix substepMatrix;

            if (it->second < (int)m_vec_matrixArrays.size())
            {
               AiArrayGetMtx(m_vec_matrixArrays[it->second], previousStep, substepMatrix);
               addVelocityToMatrix (substepMatrix, substepMatrix, velocitySubstep);
               AiArraySetMtx(m_vec_matrixArrays[it->second], step, substepMatrix);
            }
            //m_instanceTags[it->second] = ("deadParticle");
         }
      }
   }

   // check if all the steps have been exported
   for (size_t i = 0; i < m_exportedSteps.size(); ++i)
   {
      if (m_exportedSteps[i] == false)
         return;
   }

   /// NOW write out the final list
   // if deform blur is disabled, this function will only be called for current frame (#2386)
   // so we need to create the instancer now   
   
   // we check to see if there are any instances to export here instead of at the top(before first step)
   // because at each step we might be adding particles/instances,
   // and in the case of first particle born, that first step would not contain anything
   // and we'd end up with an extra blank frame
   if(m_particleIDMap.size() == 0)
      return;
   
   int globalIndex = 0;
   const char *arnoldBaseName = GetArnoldNodeName();

   for (unordered_map<int,int>::iterator it = m_particleIDMap.begin();
        it !=  m_particleIDMap.end(); ++it)
   {
      int partID = it->first;
      int j = it->second;
      if (j >= (int)m_vec_matrixArrays.size()) continue;

      for (unsigned int  k = 0; k < m_particlePathsMap[partID].length(); k++, globalIndex++)
      {
         MString instanceKey = "inst";
         instanceKey += globalIndex;

         // check if the instance for this index was already found
         AtNode *instance = GetArnoldNode(instanceKey.asChar());
         if (instance == NULL)
         {
            // Create and register this ginstance node, so that it is properly cleared later
            instance = AddArnoldNode("ginstance", instanceKey.asChar());

            MString instanceName(arnoldBaseName);
            instanceName += globalIndex;
            AiNodeSetStr(instance, "name", instanceName.asChar());
         }
         int idx = m_particlePathsMap[partID][k];
         if (idx >= (int)m_objectNames.length()) continue;

         AtNode* obj = AiNodeLookUpByName(m_objectNames[idx].asChar());
         AiNodeSetPtr(instance, "node", obj);
         AiNodeSetBool(instance, "inherit_xform", true);
         // Do not assign same array to more than one node
         if (k == 0)
            AiNodeSetArray(instance, "matrix", m_vec_matrixArrays[j]);
         else
            AiNodeSetArray(instance, "matrix", AiArrayCopy(m_vec_matrixArrays[j]));

         //AiNodeDeclare(instance, "instanceTag", "constant STRING");
         //AiNodeSetStr(instance, "instanceTag", m_instanceTags[j].asChar()); // for debug purposes
        
         // need this in case instance sources are hidden
         AtByte visibility = ComputeMasterVisibility(m_objectDagPaths[idx]);
         AiNodeSetByte(instance, "visibility", visibility);

         // add the custom user selected attributes to export
         unordered_map<std::string, MVectorArray>::iterator custVect;
         for (custVect = m_out_customVectorAttrArrays.begin(); custVect != m_out_customVectorAttrArrays.end(); custVect++)
         {

            MVector vecAttrValue = custVect->second[j];
            if (MString(custVect->first.c_str()) == "rgbPP")
            {
               AiNodeDeclare(instance, custVect->first.c_str(), "constant RGB");
               AiNodeSetRGB(instance, custVect->first.c_str(),(float)vecAttrValue.x,(float)vecAttrValue.y, (float)vecAttrValue.z );
                                             }
            else
            {
               AiNodeDeclare(instance, custVect->first.c_str(), "constant VECTOR");
               AiNodeSetVec(instance, custVect->first.c_str(),(float)vecAttrValue.x,(float)vecAttrValue.y, (float)vecAttrValue.z );
            }
         }
         unordered_map<std::string, MDoubleArray>::iterator custDouble;
         for (custDouble = m_out_customDoubleAttrArrays.begin(); custDouble != m_out_customDoubleAttrArrays.end(); custDouble++)
         {
            float doubleAttrValue = (float)custDouble->second[j];

               AiNodeDeclare(instance, custDouble->first.c_str(), "constant FLOAT");
               AiNodeSetFlt(instance, custDouble->first.c_str(),doubleAttrValue );

         }
         unordered_map<std::string, MIntArray>::iterator custInt;
         for (custInt = m_out_customIntAttrArrays.begin(); custInt != m_out_customIntAttrArrays.end(); custInt++)
         {
            int intAttrValue = custInt->second[j];

               AiNodeDeclare(instance, custInt->first.c_str(), "constant INT");
               AiNodeSetInt(instance, custInt->first.c_str(), intAttrValue );

         }
      }
   }

}
void CInstancerTranslator::RequestUpdate()
{
   /*
   FIXME : ideally we'd like to avoid re-generating instancer all the time, 
   and do something more optimized based on the plug name in NodeChanged.
   But from what I understand any parameter could require a full re-export
   - inputPoints : particles/instances have changed
   - translate/rotate/scale : ginstance matrices will have to be recomputed
   - level of detail params : recompute
   - inputHierarchy : would have been nice to stay independent of it, as ginstance references the original shape
       however, if you change the scale/rotation of the shape, it will affect the matrices we get from maya.
      -> TO BE VERIFIED

   }*/

   SetUpdateMode(AI_RECREATE_NODE);
   CShapeTranslator::RequestUpdate();
}
