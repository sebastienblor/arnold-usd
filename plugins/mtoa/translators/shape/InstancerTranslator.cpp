#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#endif

#include "InstancerTranslator.h"
#include <maya/MFnDagNode.h>
#include "utils/time.h"
#include "utils/MtoaLog.h"
#include "utils/ConstantStrings.h"

void ConvertMatrixWithoutOffset(AtMatrix& matrix, const MMatrix& mayaMatrix) 
{
   MTransformationMatrix trMat = mayaMatrix;
   MMatrix copyMayaMatrix = trMat.asMatrix();

   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) copyMayaMatrix[I][J];
      }
   }
}

void addVelocityToMatrix(AtMatrix& outMatrix, AtMatrix& matrix,
                         const MVector& velocityVector)
{
   outMatrix = matrix;

   outMatrix[3][0] = matrix[3][0] + (float)velocityVector.x;
   outMatrix[3][1] = matrix[3][1] + (float)velocityVector.y;
   outMatrix[3][2] = matrix[3][2] + (float)velocityVector.z;

}

void CInstancerTranslator::NodeInitializer(CAbTranslator context)
{
}

AtNode* CInstancerTranslator::CreateArnoldNodes()
{
   // We need to invoke IsMasterInstance first so that the m_isMasterDag value is initialized
   IsMasterInstance();
   return AddArnoldNode("ginstance");
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
   
   MPlug plug = fnNode.findPlug("castsShadows", true);
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_SHADOW;
   }

   plug = fnNode.findPlug("primaryVisibility", true);
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_CAMERA;
   }
   plug = fnNode.findPlug("aiVisibleInDiffuseReflection", true);
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~(AI_RAY_DIFFUSE_REFLECT);
   }
   plug = fnNode.findPlug("aiVisibleInSpecularReflection", true);
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_SPECULAR_REFLECT;
   }
   plug = fnNode.findPlug("aiVisibleInDiffuseTransmission", true);
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~(AI_RAY_DIFFUSE_TRANSMIT);
   }

   plug = fnNode.findPlug("aiVisibleInSpecularTransmission", true);
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_SPECULAR_TRANSMIT;
   }

   plug = fnNode.findPlug("aiVisibleInVolume", true);
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_VOLUME;
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

   MPlug inputPointsPlug = depNodeInstancer.findPlug("inputPoints", true);
   inputPointsPlug.connectedTo(conn, true, false);
   
   MObject inputPointsData = inputPointsPlug.asMObject();

   if (conn.length() == 0)
	   return;

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
      if (MtoaTranslationInfo())
      {
         MString log = "[mtoa] Instancer "+m_fnMayaInstancer.partialPathName()+" for particle instancer "+m_fnParticleSystem.partialPathName()+" exporting instances for step ";
         log += step;
         MtoaDebugLog(log);
      }
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
      exportID = m_fnParticleSystem.findPlug("aiExportParticleIDs", true).asBool();

      /// STORE the custom attrs for use later
      m_customAttrs = m_fnParticleSystem.findPlug("aiExportAttributes", true).asString();

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

               m_fnParticleSystem.findPlug(currentAttr, true, &status);
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
      MPlug plug = fnParticle.findPlug("aiExportParticleIDs", true);
      exportID = (!plug.isNull() && plug.asBool());

      plug = fnParticle.findPlug("aiExportAttributes", true);
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
      m_cloneInstances.clear();
      m_objectDagPaths.clear();

      m_sceneTransforms = (GetSessionOptions().GetOrigin().length() > AI_EPSILON ||
         std::abs(GetSessionOptions().GetScaleFactor() - 1.f) > AI_EPSILON);

      /// export instance object masters
      int numObjects = paths.length();
      m_cloneInstances.reserve(numObjects);
      MFnDagNode fnDag;
      for (int i = 0; i < numObjects; ++i)
      {
         MDagPath dagPathMaster = paths[i];
         fnDag.setObject(dagPathMaster);

         AtNode *masterNode = NULL;
         // MFnInstancer.allInstances returns intermediate objects. D'oh!
         if (!fnDag.isIntermediateObject())
         {
            // Check if the node is in the scene already.
            MFnDependencyNode masterDepNode(dagPathMaster.node());
            MPlug dummyPlug = masterDepNode.findPlug("matrix", true);
            // in case master instance wasn't exported (#648)
            // and also to create the reference between both translators
            if (!dummyPlug.isNull())
               masterNode = ExportConnectedNode(dummyPlug);

         }
         if (masterNode != NULL && AiNodeEntryGetType(AiNodeGetNodeEntry(masterNode)) == AI_NODE_LIGHT)
         {
            m_cloneInstances.push_back(true); // marking the path object as needing to be cloned
         }
         else
         {
            m_cloneInstances.push_back(false); // standard gInstance of node will be created
         }
         m_objectNames.append(GetSessionOptions().GetArnoldNaming(dagPathMaster).asChar());
         m_objectDagPaths.append(dagPathMaster);
         m_objectMatrices.append(dagPathMaster.inclusiveMatrix());
      }
      // Done export object masters
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

            // because there could be multiple paths for each particle(grouped objects) we need to get the number of paths for each particleId
            int numPathsPP = particlePathStartIndices[j + 1] - particlePathStartIndices[j];
            MIntArray pathsIdx;
            for (int i = 0; i < numPathsPP; i++)
            {
               pathsIdx.append(pathIndices[particlePathStartIndices[j]+i]);
            }
            m_particlePathsMap[id] = pathsIdx;
            
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
                  if (i == (int)step || m_exportedSteps[i] == false)
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
                           m_fnParticleSystem.findPlug(currentAttr, true, &status);
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
               MIntArray pathIdx;
               for (int i = 0; i < pathNumber; i++)
               {
                  pathIdx.append(pathIndices[particlePathStartIndices[j]+i]);
               }
               m_particlePathsMap[partIds[j]] = pathIdx;

               //m_instanceTags.append("newParticle");
               if (velocities.length() > 0)
               {
                  m_instantVeloArray.append(velocities[j]);
               }
            }
         }
         if (MtoaTranslationInfo())
         {
            MString log = "[mtoa] Instancer "+m_fnMayaInstancer.partialPathName()+" export for particle system "+m_fnParticleSystem.partialPathName()+" found a ";
            log += newParticleCount;
            log += " new particles for step ";
            log += step;
            MtoaDebugLog(log);
         }
      }
      if ((!m_vec_matrixArrays.empty()) && tempMap.size() > 0)
      {
         if (MtoaTranslationInfo())
         {
            MString log = "[mtoa] Instancer "+m_fnMayaInstancer.partialPathName()+" export for particle system "+m_fnParticleSystem.partialPathName()+" found ";
            log += (int)tempMap.size();
            log += " particles that died for step ";
            log += step;
            log += ", computing velocity...";
            MtoaDebugLog(log);
         }

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
               substepMatrix = AiArrayGetMtx(m_vec_matrixArrays[it->second], previousStep);
               addVelocityToMatrix (substepMatrix, substepMatrix, velocitySubstep);
               AiArraySetMtx(m_vec_matrixArrays[it->second], step, substepMatrix);
            }
            //m_instanceTags[it->second] = ("deadParticle");
         }
      }
   }
}

void CInstancerTranslator::PostExport(AtNode *node)
{

   /// NOW write out the final list
   // if deform blur is disabled, this function will only be called for current frame (#2386)
   // so we need to create the instancer now   
   
   // we check to see if there are any instances to export here instead of at the top(before first step)
   // because at each step we might be adding particles/instances,
   // and in the case of first particle born, that first step would not contain anything
   // and we'd end up with an extra blank frame
   if(m_particleIDMap.size() == 0)
      return;
   
   MString baseName = GetSessionOptions().GetArnoldNaming(GetMayaDagPath());
   
   int globalIndex = 0;
   
   for (unordered_map<int,int>::iterator it = m_particleIDMap.begin();
        it !=  m_particleIDMap.end(); ++it)
   {
      int partID = it->first;
      int j = it->second;
      if (j >= (int)m_vec_matrixArrays.size()) continue;

      for (unsigned int  k = 0; k < m_particlePathsMap[partID].length(); k++, globalIndex++)
      {
         int idx = m_particlePathsMap[partID][k];
         if (idx >= (int)m_objectNames.length()) continue;

         AtNode *instance = NULL;
         AtNode* obj = AiNodeLookUpByName(GetUniverse(), AtString(m_objectNames[idx].asChar()));

         // if offset : get obj matrix array !

         MString instanceKey = "inst";
         instanceKey += globalIndex;

         if (m_cloneInstances[idx])
         {
            MString instName = baseName + MString("/") + instanceKey;

            // Clone the master node (lights can't be instanced in arnold)
            instance  = AiNodeClone(obj, AtString(instName.asChar()));
   
            AddExistingArnoldNode(instance, instanceKey.asChar());
            AiNodeSetDisabled(instance, false);
            // no motion blur on lighs
            AtMatrix origM = AiNodeGetMatrix(instance, str::matrix);
            AtMatrix particleMatrix = AiArrayGetMtx(m_vec_matrixArrays[j], 0);
            AtMatrix total_matrix = AiM4Mult(particleMatrix, origM);
            AiNodeSetMatrix(instance, str::matrix, total_matrix);
         } else
         { 
            // Regular instances
            instance = (globalIndex == 0) ? node : GetArnoldNode(instanceKey.asChar());
            if (instance == NULL)
            {
               // Create and register this ginstance node, so that it is properly cleared later
               instance = AddArnoldNode("ginstance", instanceKey.asChar());
               // We no longer set the name of these instances since AddArnoldNode already does,
               // and takes the prefix into account (#2684)
            }
            AiNodeSetPtr(instance, str::node, obj);
            AtArray *inheritMtx = AiArrayCopy(m_vec_matrixArrays[j]);

            if (m_sceneTransforms)
            {
               // There is an origin offset, or a scene scale factor. 
               // We need to manually apply the object transformation 
               // instead of enabling inherit_transform in the ginstance MTOA-1216
               AtMatrix objAtMtx;
               ConvertMatrixWithoutOffset(objAtMtx, m_objectMatrices[idx]);
               unsigned int keyCount = AiArrayGetNumKeys(inheritMtx);
               for (unsigned int k = 0; k < keyCount; ++k)
               {
                  AtMatrix tmpMtx = AiArrayGetMtx(inheritMtx, k);
                  AiArraySetMtx(inheritMtx, k, AiM4Mult(tmpMtx, objAtMtx));
               }
               AiNodeSetBool(instance, str::inherit_xform, false); 
            } else
            {
               // regular use case, just rely on inherit_transform
               AiNodeSetBool(instance, str::inherit_xform, true);   
            }

            AiNodeSetArray(instance, str::matrix, inheritMtx); 
         }
         //AiNodeDeclare(instance, "instanceTag", "constant STRING");
         //AiNodeSetStr(instance, "instanceTag", m_instanceTags[j].asChar()); // for debug purposes
        
         // need this in case instance sources are hidden
         AtByte visibility = ComputeMasterVisibility(m_objectDagPaths[idx]);
         AiNodeSetByte(instance, str::visibility, visibility);

         // add the custom user selected attributes to export
         unordered_map<std::string, MVectorArray>::iterator custVect;
         for (custVect = m_out_customVectorAttrArrays.begin(); custVect != m_out_customVectorAttrArrays.end(); custVect++)
         {

            MVector vecAttrValue = custVect->second[j];
            AtString attrName(custVect->first.c_str());
            if (MString(custVect->first.c_str()) == "rgbPP")
            {
               if (m_cloneInstances[idx]) // override the light color
                  AiNodeSetRGB(instance, str::color, (float)vecAttrValue.x, (float)vecAttrValue.y, (float)vecAttrValue.z); 
               
               if (!AiNodeLookUpUserParameter(instance, attrName))
                  AiNodeDeclare(instance, attrName, str::constant_RGB);
               AiNodeSetRGB(instance, attrName,(float)vecAttrValue.x,(float)vecAttrValue.y, (float)vecAttrValue.z );
            }
            else
            {

               if (!AiNodeLookUpUserParameter(instance, attrName))
                  AiNodeDeclare(instance, attrName, str::constant_VECTOR);
               AiNodeSetVec(instance, attrName,(float)vecAttrValue.x,(float)vecAttrValue.y, (float)vecAttrValue.z );
            }
         }
         unordered_map<std::string, MDoubleArray>::iterator custDouble;
         for (custDouble = m_out_customDoubleAttrArrays.begin(); custDouble != m_out_customDoubleAttrArrays.end(); custDouble++)
         {
            float doubleAttrValue = (float)custDouble->second[j];
            AtString attrName(custDouble->first.c_str());
            if (!AiNodeLookUpUserParameter(instance, attrName))
               AiNodeDeclare(instance, attrName, str::constant_FLOAT);
            AiNodeSetFlt(instance, attrName, doubleAttrValue );

         }
         unordered_map<std::string, MIntArray>::iterator custInt;
         for (custInt = m_out_customIntAttrArrays.begin(); custInt != m_out_customIntAttrArrays.end(); custInt++)
         {
            int intAttrValue = custInt->second[j];
            AtString attrName(custInt->first.c_str());
            if (!AiNodeLookUpUserParameter(instance, attrName))
               AiNodeDeclare(instance, attrName, str::constant_INT);
            AiNodeSetInt(instance, attrName, intAttrValue );

         }
      }
   }
   for (size_t i = 0; i < m_vec_matrixArrays.size(); ++i)
   {
      AiArrayDestroy(m_vec_matrixArrays[i]);
   }
   m_vec_matrixArrays.clear();

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

