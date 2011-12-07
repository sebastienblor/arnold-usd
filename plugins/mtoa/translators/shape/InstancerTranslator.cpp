#include "InstancerTranslator.h"

#include "scene/MayaScene.h"

#ifdef _WIN32
   #include <platform/win32/dirent.h>
   #define PATHSEP ';'
   #define DIRSEP "\\"
   #define LIBEXT MString(".dll")
#else
   #include <sys/types.h>
   #include <dirent.h>
   #include <dlfcn.h>

   #define PATHSEP ':'
   #define DIRSEP "/"

#endif



void addVelocityToMatrix(AtMatrix& outMatrix, AtMatrix& matrix,
                         const MVector& velocityVector)
{
   AiM4Copy(outMatrix, matrix);

   outMatrix[3][0] = matrix[3][0] + velocityVector.x;
   outMatrix[3][1] = matrix[3][1] + velocityVector.y;
   outMatrix[3][2] = matrix[3][2] + velocityVector.z;

}

void CInstancerTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya);

   // node attributes
   CShapeTranslator::MakeCommonAttributes(helper);

   CAttrData data;

   data.defaultValue.BOOL = false;
   data.name = "export_particleId";
   data.shortName = "exppartid";
   helper.MakeInputBoolean(data);

   data.defaultValue.STR = "";
   data.name = "export_attributes";
   data.shortName = "expartattr";
   helper.MakeInputString(data);
}

AtNode* CInstancerTranslator::CreateArnoldNodes()
{
   m_isMasterDag = IsMasterInstance(m_masterDag);
   // FIXME  this master dag return needs to be null because it doesen't contain anything, the instancer
   // produces individual ginstances for each particle instance
   if (m_isMasterDag)
      return  AddArnoldNode("ginstance");

   else
      return  AddArnoldNode("ginstance");
}

void CInstancerTranslator::Update(AtNode *anode)
{
   ExportInstancer(anode, true);
}

void CInstancerTranslator::ExportMotion(AtNode* anode, AtUInt step)
{
   if (m_isMasterDag)
   {
      ExportMatrix(anode, step);
      if (m_motionDeform)
      {
         ExportInstances(anode, step);
      }
   }
   else
   {
      ExportMatrix(anode, step);
   }
}

void CInstancerTranslator::UpdateMotion(AtNode* anode, AtUInt step)
{
   ExportMatrix(anode, step);
}

void CInstancerTranslator::ExportInstancer(AtNode* instancer, bool update)
{
   ExportMatrix(instancer,0);
   if(!update)
   {
      ExportInstances(instancer, 0);
   }

}

void CInstancerTranslator::ExportInstances(AtNode* instancer, AtUInt step)
{

   MTime oneSec(1.0, MTime::kSeconds);
   int fps =  (float)oneSec.asUnits(MTime::uiUnit());
   uint totalSteps = GetNumMotionSteps();
   //uint middleStep = ((totalSteps/2)+1)-1;

   MFnInstancer maya_instancer(m_dagPath);
   m_particleCount = maya_instancer.particleCount();


   MPlugArray conn;
   // the particleShape attached
   MFnDependencyNode depNodeInstancer(m_dagPath.node());
   depNodeInstancer.findPlug("inputPoints").connectedTo(conn, true, false);

   // inputPoints is not an array, so position [0] is the particleShape node
   m_fnParticleSystem.setObject(conn[0].node());
   MIntArray  partIds;
   MVectorArray velocities;
   m_fnParticleSystem.particleIds(partIds);
   m_fnParticleSystem.velocity(velocities);

   MDagPathArray paths;
   MMatrixArray mayaMatrices;
   MIntArray particlePathStartIndices;
   MIntArray pathIndices;

   MStatus  status;

   status = maya_instancer.allInstances(paths, mayaMatrices, particlePathStartIndices, pathIndices);
   CHECK_MSTATUS(status);

   bool exportID = maya_instancer.findPlug("export_particleId").asBool();

   /// STORE the custom attrs for use later
   m_customAttrs = maya_instancer.findPlug("export_attributes").asString();

   std::cout << "CUSTOM ATTRS: " <<  m_customAttrs << std::endl;
   MStringArray attrs;
   status = m_customAttrs.split(';', attrs);


   m_instant_customDoubleAttrArrays.clear();
   m_instant_customVectorAttrArrays.clear();
   m_instant_customIntAttrArrays.clear();

   // we have to do this no matter if we have particles or not..
   if (m_customAttrs.length() != 0)
   {
      MStringArray attrs;
      status = m_customAttrs.split(';', attrs);
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
         if (exportID)
         {
            m_instant_customIntAttrArrays["particleId"]= partIds;
         }
      }
   }

   if (step == 0)
   {
      m_instantVeloArray.clear();
      m_vec_matrixArrays.clear();
      m_startIndicesArray.clear();
      m_pathIndicesArray.clear();
      m_out_customVectorAttrArrays.clear();
      m_out_customDoubleAttrArrays.clear();
      m_out_customIntAttrArrays.clear();

      if (m_fnParticleSystem.count() > 0)
      {
         for (uint j = 0; j < particlePathStartIndices.length(); j++)
         {
            AtArray* outMatrix = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
            AtMatrix matrix;
            ConvertMatrix(matrix, mayaMatrices[j]);
            AiArraySetMtx(outMatrix, step, matrix);

            m_vec_matrixArrays.push_back(outMatrix);

            int id = partIds[j];
            m_particleIDMap[id] = j;
            m_startIndicesArray.append(particlePathStartIndices[j]);
            m_pathIndicesArray.append(pathIndices[j]);
            m_instantVeloArray.append(velocities[j]);
            m_instanceTags.append("originalGangster");
         }
      }

      if (m_customAttrs.length() != 0)
      {
         m_out_customVectorAttrArrays = m_instant_customVectorAttrArrays;
         m_out_customDoubleAttrArrays = m_instant_customDoubleAttrArrays;
         m_out_customIntAttrArrays    = m_instant_customIntAttrArrays;
      }

      /// export instance object masters
      int numObjects = paths.length();

      for (int i = 0; i < numObjects; ++i)
      {
         MDagPath dagPathMaster = paths[i];
         // Check if the node is in the scene already.
         AtNode* masterNode = AiNodeLookUpByName(dagPathMaster.partialPathName().asChar());

         // if not, we export it
         if (!masterNode)
         {
            // FIXME: check if the object will not be exported a second time later !
            ExportDagPath(dagPathMaster);
         }
         m_objectNames.append(dagPathMaster.partialPathName().asChar());
      }
      // Done export object masters
   }
   else // step > 0
   {
      std::map <int, int> tempMap = m_particleIDMap;
      std::map <int, int>::iterator it;
      if (m_fnParticleSystem.count() > 0)
      {
         for (uint j = 0; j < partIds.length(); j++)
         {
            it = tempMap.find(partIds[j]);
            if (it != tempMap.end())   // found the particle in the scene already
            {
               AtMatrix matrix;
               ConvertMatrix(matrix, mayaMatrices[j]);
               AiArraySetMtx(m_vec_matrixArrays[it->second], step, matrix);
               // update instant velocity
               m_instantVeloArray[it->second] = velocities[j];
               tempMap.erase(it);
            }
            else  // found a new particle in this substep
            {
               std::cout << "Adding new particle in this substep" << std::endl;
               AtArray* outMatrix = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
               AtMatrix matrix;
               ConvertMatrix(matrix, mayaMatrices[j]);
               AiArraySetMtx(outMatrix, step, matrix);
               // now compute the previous steps velocity matrices
               for (uint i = 0; i<step; i++)
               {
                  int k = (i-(int)(totalSteps/2));
                  MVector velocitySubstep = (((velocities[j]/fps)*GetMotionByFrame())/(GetNumMotionSteps()-1))*k;
                  AtMatrix substepMatrix;
                  addVelocityToMatrix (substepMatrix, matrix, velocitySubstep);
                  AiArraySetMtx(outMatrix, i, substepMatrix);
               }
               if (m_customAttrs.length() != 0)
               {
                  if (attrs.length() > 0)
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
                     if (exportID)
                     {
                        m_out_customIntAttrArrays["particleId"].append(m_instant_customIntAttrArrays["particleId"][j]);
                     }
                  }
               }
               m_vec_matrixArrays.push_back(outMatrix);
               m_particleIDMap[partIds[j]] = m_vec_matrixArrays.size()-1;
               m_startIndicesArray.append(particlePathStartIndices[j]);
               m_pathIndicesArray.append(pathIndices[j]);
               m_instanceTags.append("newToTheParty");
               m_instantVeloArray.append(velocities[j]);

            }
         }
      }
      if (tempMap.size() > 0)
      {
         std::cout << "found " << tempMap.size() << " particles that died, computing their new velocity" << std::endl;
         for (it = tempMap.begin(); it != tempMap.end(); it++)
         {
            // get last steps  matrix
            int k = (step-(int)(totalSteps/2));
            MVector velocitySubstep = (((m_instantVeloArray[it->second]/fps)*GetMotionByFrame())/(GetNumMotionSteps()-1))*k;
            AtMatrix substepMatrix;
            AiArrayGetMtx(m_vec_matrixArrays[it->second], step-1, substepMatrix);
            addVelocityToMatrix (substepMatrix, substepMatrix, velocitySubstep);
            AiArraySetMtx(m_vec_matrixArrays[it->second], step, substepMatrix);
            m_instanceTags[it->second] = ("LostAtSea");
         }
      }
   }

   /// NOW write out the final list
   if (step == (GetNumMotionSteps()-1))
   {
      // we check to see if there are any instances to export here instead of at the top(before first step)
      // because at each step we might be adding particles/instances,
      // and in the case of first particle born, that first step would not contain anything
      // and we'd end up with an extra blank frame
      if(m_particleIDMap.size() == 0)
      {
         return;
      }

         for(uint  j = 0; j< m_particleIDMap.size(); j++)
         {
            AtNode *instance;
            instance = AiNode("ginstance");

            int idx = m_pathIndicesArray[j];
            AtNode* obj = AiNodeLookUpByName(m_objectNames[idx].asChar());
            AiNodeSetPtr(instance, "node", obj);
            AiNodeSetBool(instance, "inherit_xform", false);
            AiNodeSetArray(instance, "matrix", m_vec_matrixArrays[j]);

            //AiNodeDeclare(instance, "instanceTag", "constant STRING");
            //AiNodeSetStr(instance, "instanceTag", m_instanceTags[j].asChar()); // for debug purposes

            // need this in case instance sources are hidden
            AiNodeSetInt(instance, "visibility", 1);

            // add the custom user selected attributes to export
            std::map<std::string, MVectorArray>::iterator custVect;
            for (custVect = m_out_customVectorAttrArrays.begin(); custVect != m_out_customVectorAttrArrays.end(); custVect++)
            {

               MVector vecAttrValue = custVect->second[j];
               if (MString(custVect->first.c_str()) == "rgbPP")
               {
                  AiNodeDeclare(instance, custVect->first.c_str(), "constant RGB");
                  AiNodeSetRGB(instance, custVect->first.c_str(),(AtFloat)vecAttrValue.x,(AtFloat)vecAttrValue.y, (AtFloat)vecAttrValue.z );
                                              }
               else
               {
                  AiNodeDeclare(instance, custVect->first.c_str(), "constant VECTOR");
                  AiNodeSetVec(instance, custVect->first.c_str(),(AtFloat)vecAttrValue.x,(AtFloat)vecAttrValue.y, (AtFloat)vecAttrValue.z );
               }
            }
            std::map<std::string, MDoubleArray>::iterator custDouble;
            for (custDouble = m_out_customDoubleAttrArrays.begin(); custDouble != m_out_customDoubleAttrArrays.end(); custDouble++)
            {
               AtFloat doubleAttrValue = custDouble->second[j];

                  AiNodeDeclare(instance, custDouble->first.c_str(), "constant FLOAT");
                  AiNodeSetFlt(instance, custDouble->first.c_str(),doubleAttrValue );

            }
            std::map<std::string, MIntArray>::iterator custInt;
            for (custInt = m_out_customIntAttrArrays.begin(); custInt != m_out_customIntAttrArrays.end(); custInt++)
            {
               AtInt intAttrValue = custInt->second[j];

                  AiNodeDeclare(instance, custInt->first.c_str(), "constant INT");
                  AiNodeSetInt(instance, custInt->first.c_str(), intAttrValue );

            }

         }

   }

}

void CInstancerTranslator::Export(AtNode* anode)
{
   ExportInstancer(anode, false);
}
