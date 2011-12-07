#include "InstancerTranslator.h"

#include "scene/MayaScene.h"

// TODO   support proper translation sampling  so that  velocity can be calculated to include  sampled rotation

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

void ConvertMatrix(AtMatrix& matrix, const MMatrix& mayaMatrix)
{
   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) mayaMatrix[I][J];
      }
   }
}

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
   if (m_isMasterDag)
      return  AddArnoldNode("procedural");
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
      if (GetNumMotionSteps() %2 != 1)
      {
         AiMsgError("Right now, Particle Instancer does not support even number of motion blur steps.");
         return;
      }
      ExportInstances(instancer, 0);
   }

}

void CInstancerTranslator::ExportInstances(AtNode* instancer, AtUInt step)
{

   MTime oneSec(1.0, MTime::kSeconds);
   int fps =  (float)oneSec.asUnits(MTime::uiUnit());
   uint totalSteps = GetNumMotionSteps();
   uint middleStep = ((totalSteps/2)+1)-1;

   std::cout << totalSteps << std::endl;

   if (step == middleStep)
   {

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
      MStringArray objectNames;

      MStatus status = maya_instancer.allInstances(paths, mayaMatrices, particlePathStartIndices, pathIndices);
      CHECK_MSTATUS(status);

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

         objectNames.append(dagPathMaster.partialPathName().asChar());

      }

      for (uint j = 0; j < particlePathStartIndices.length(); j++)
      {

         AtNode *instance;
         instance = AiNode("ginstance");
         int idx = pathIndices[j];
         AtNode* obj = AiNodeLookUpByName(objectNames[idx].asChar());
         AiNodeSetPtr(instance, "node", obj);
         AiNodeSetBool(instance, "inherit_xform", false);

         AtArray* outMatrix = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);

         AtMatrix matrix;
         ConvertMatrix(matrix, mayaMatrices[j]);

         for (int i = 0; i < outMatrix->nkeys; i++)
         {

            int k = (i-(int)(totalSteps/2));
            MVector velocitySubstep = (((velocities[j]/fps)*GetMotionByFrame())/(GetNumMotionSteps()-1))*k;
            AtMatrix substepMatrix;
            addVelocityToMatrix (substepMatrix, matrix, velocitySubstep);
            AiArraySetMtx(outMatrix, i, substepMatrix);
         }
         AiNodeSetArray(instance, "matrix", outMatrix);

         m_customAttrs = maya_instancer.findPlug("export_attributes").asString();
         std::cout << "CUSTOM ATTRS: " <<  m_customAttrs << std::endl;
         if (m_customAttrs.length() != 0)
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
                    MDoubleArray attributes;
                    m_fnParticleSystem.getPerParticleAttribute(currentAttr, attributes);

                    AiNodeDeclare(instance, currentAttr.asChar(), "constant FLOAT");
                    AiNodeSetFlt(instance, currentAttr.asChar(), attributes[j]);
                    continue;
                 }
                 else if (m_fnParticleSystem.isPerParticleVectorAttribute(currentAttr))
                 {
                    MVectorArray attributes;
                    m_fnParticleSystem.getPerParticleAttribute(currentAttr, attributes);

                    std::cout << attributes[j] << std::endl;

                    AtFloat x = attributes[j].x;
                    AtFloat y = attributes[j].y;
                    AtFloat z = attributes[j].z;

                    AiNodeDeclare(instance, currentAttr.asChar(), "constant RGB");
                    AiNodeSetRGB(instance, currentAttr.asChar(), x,y,z);
                    continue;

                 }
                 else if (m_fnParticleSystem.isPerParticleIntAttribute(currentAttr))
                 {
                    MIntArray attributes;
                    m_fnParticleSystem.getPerParticleAttribute(currentAttr, attributes);

                    AiNodeDeclare(instance, currentAttr.asChar(), "constant INT");
                    AiNodeSetInt(instance, currentAttr.asChar(), attributes[j]);
                    continue;

                 }
              }
           }

         }


      }
   }

}

void CInstancerTranslator::Export(AtNode* anode)
{
   ExportInstancer(anode, false);
}
