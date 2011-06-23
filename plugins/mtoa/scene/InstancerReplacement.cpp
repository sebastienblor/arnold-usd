
#include "MayaScene.h"

#include <maya/MFnInstancer.h>
#include <maya/MDagPathArray.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MMatrix.h>

void CMayaScene::ExportInstancerReplacement(const MDagPath& dagPath, AtUInt step)
{
   MFnInstancer instancer(dagPath);
   MFnDagNode fnDagNode(dagPath.node());
   int numParticles = instancer.particleCount();

   // Find the particleShape attached to this instancer
   MPlugArray conn;

   // Instancer replacement nodes do not have a motion blur enable/disable attribute. Use object .
   bool mb = IsMotionBlurEnabled(MTOA_MBLUR_OBJECT);

   // the particleShape attached
   MFnDependencyNode depNodeInstancer(dagPath.node());
   depNodeInstancer.findPlug("inputPoints").connectedTo(conn, true, false);
   // inputPoints is not an array, so position [0] is the particleShape node
   MFnParticleSystem particleSystem(conn[0].node());
   MIntArray arrayParticleIds;
   // Get particles ids for later ginstance naming
   particleSystem.particleIds(arrayParticleIds);

   // for each particle
   for(int i=0; i<numParticles; i++)
   {
      MDagPathArray dagPathArray;
      MMatrix       particleMatrix;

      instancer.instancesForParticle(i, dagPathArray, particleMatrix);
      for(AtUInt j = 0; (j < dagPathArray.length()); j++)
      {
         MDagPath dagPathMaster = dagPathArray[j];

         MMatrix mayaMatrix;
         AtMatrix matrix;
          
         mayaMatrix = dagPathArray[j].inclusiveMatrix() * particleMatrix;
         ConvertMatrix(matrix, mayaMatrix);

         // If theres an arnold node for the dagPath
         AtNode* masterNode = AiNodeLookUpByName(dagPathArray[j].fullPathName().asChar());
         if (masterNode)
         {
            AtNode* instanceNode;
            MString instanceName = dagPath.fullPathName();
            instanceName = instanceName + "_" + dagPathArray[j].fullPathName() + "_particle_" + arrayParticleIds[i];

            if (step == 0)
            {
               instanceNode = AiNode("ginstance");
               AiNodeSetStr(instanceNode, "name", instanceName.asChar());
               AiNodeSetPtr(instanceNode, "node", masterNode);

               if (mb)
               {
                  AtArray* matrices = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
                  AiArraySetMtx(matrices, step, matrix);
                  AiNodeSetArray(instanceNode, "matrix", matrices);
               }
               else
               {
                  AiNodeSetMatrix(instanceNode, "matrix", matrix);
               }

               AiNodeSetBool(instanceNode, "inherit_xform", false);

            }
            else
            {
               instanceNode = AiNodeLookUpByName(instanceName.asChar());

               if (mb && instanceNode)
               {
                  AtArray* matrices = AiNodeGetArray(instanceNode, "matrix");
                  AiArraySetMtx(matrices, step, matrix);
               }

            }

         }
         
      }

   }

   

}

