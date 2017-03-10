/*
 * ParticleInstancer.cpp
 *
 *  Created on: Feb 3, 2011
 *      Author: nozon
 */

#include <ai.h>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>

#include "../../plugins/mtoa/utils/time.h"

//FIXME Remove global variables

int nbInsts = 0;
int nbObjects = 0;

std::vector<AtArray*> params;
std::vector<const char*> paramsName;


AtArray* matrixArray = AiArrayAllocate(1, 1, AI_TYPE_MATRIX);
AtArray* indxArray = AiArrayAllocate(1, 1, AI_TYPE_INT);
AtArray* objArray = AiArrayAllocate(1, 1, AI_TYPE_STRING);


AtArray* partArray = AiArrayAllocate(1, 1, AI_TYPE_INT);

AtNode* particleSystem;

AI_PROCEDURAL_NODE_EXPORT_METHODS(ParticleInstancerMtd);

node_parameters
{
}

procedural_init
{

   *user_ptr = node;
   particleSystem = AiNodeLookUpByName(AiNodeGetStr(node, "data"));

   if (AiNodeLookUpUserParameter(node, "pathIndices") == NULL)
   {
      AiMsgError("[INSTANCER] No path Indices");
      AiRenderAbort();
   }

   AtArray* pathIndicesArray = AiArrayCopy(AiNodeGetArray(node, "pathIndices"));

   if (AiNodeLookUpUserParameter(node, "pathStartIndices") == NULL)
   {
      AiMsgError("[INSTANCER] No path Start Indices");
      AiRenderAbort();
   }

   AtArray* pathStartIndices = AiArrayCopy(AiNodeGetArray(node, "pathStartIndices"));

   AtArray* pathMatrixArray = AiArrayCopy(AiNodeGetArray(node, "instanceMatrix"));

   static const AtString points_str("points");
   if (particleSystem)
   {
      if (AiNodeIs(particleSystem, points_str))

      {
         AiNodeSetInt(particleSystem, "visibility", 0);

         if (AiArrayGetNumElements(pathMatrixArray) == 0)
         {
            AiMsgError("[INSTANCER] No matrix datas.");
            AiMsgError("[INSTANCER] Check preserve_scene_data option.");
            AiRenderAbort();
         }

         nbInsts = AiArrayGetNumElements(pathIndicesArray);

         AiMsgInfo("[INSTANCER] instanciating %i objects", nbInsts);

         indxArray = AiArrayAllocate(nbInsts, 1, AI_TYPE_INT);

         partArray = AiArrayAllocate(nbInsts, AiArrayGetNumKeys(pathMatrixArray), AI_TYPE_INT);


         matrixArray = AiArrayAllocate(nbInsts, AiArrayGetNumKeys(pathMatrixArray), AI_TYPE_MATRIX);

         // Preparing array for user attributes
         AtUserParamIterator *iter = AiNodeGetUserParamIterator(particleSystem);
         while (!AiUserParamIteratorFinished(iter))
         {
            const AtUserParamEntry *upentry = AiUserParamIteratorGetNext(iter);
            const char* attrName = AiUserParamGetName(upentry);


            AtArray* paramArray = AiNodeGetArray(particleSystem, attrName);

            int type = AiArrayGetType(paramArray);

            params.push_back(AiArrayAllocate(nbInsts, AiArrayGetNumKeys(paramArray), type));

            paramsName.push_back(attrName);

            AiMsgInfo("[INSTANCER] Exporting attribute : %s", attrName);


         }
         AiUserParamIteratorDestroy(iter);

         int index = 0;

         unsigned arrayNumElements = AiArrayGetNumElements(pathMatrixArray);

         for (size_t i = 0; i < arrayNumElements; ++i)
         {



            int pathEnd = AiArrayGetInt(pathStartIndices,(unsigned long)i+1);
            int pathStart = AiArrayGetInt(pathStartIndices,(unsigned long)i);
            int nbPath = pathEnd - pathStart;

            if (nbPath != 0)
            {
               for (int j = pathStart; j <= pathEnd - 1; ++j)
               {

                  AiArraySetInt(indxArray, index, AiArrayGetInt(pathIndicesArray,j ));
                  int arrayNumKeys  = AiArrayGetNumKeys(pathMatrixArray);

                  for (int step = 0; step < arrayNumKeys; step++)
                  {

                     AtMatrix curPathMatrix = AiArrayGetMtx(pathMatrixArray, j * arrayNumKeys + step);
                     AiArraySetMtx(matrixArray, index * arrayNumKeys + step, curPathMatrix);
                  }

                  for (int step = 0; step < arrayNumKeys; step++)
                  {
                     int offset = nbInsts * step;
                     AiArraySetInt(partArray, index + offset, (unsigned long)(i + arrayNumElements*step));



                  }

                  //rearranging arrays for user attributes

                  AtUserParamIterator *iter = AiNodeGetUserParamIterator(particleSystem);
                  int k = 0;
                  while (!AiUserParamIteratorFinished(iter))
                  {

                     const AtUserParamEntry *upentry = AiUserParamIteratorGetNext(iter);
                     const char* attrName = AiUserParamGetName(upentry);
//
//                     if (strcmp(attrName, "instanceRotation") != 0 && strcmp(attrName, "instanceScale") != 0)
//                     {
                        AtArray* paramArray = AiNodeGetArray(particleSystem, attrName);

                        int type = AiArrayGetType(paramArray);
                        int nkeys = AiArrayGetNumKeys(paramArray);
                        unsigned nelements = AiArrayGetNumElements(paramArray);

                        for (int step = 0; step < nkeys; step++)
                        {
                           int offset = nbInsts * step;
                           switch (type)
                           {
                           case AI_TYPE_INT:
                              AiArraySetInt(params[k], index + offset, AiArrayGetInt(paramArray,j + nelements*step));
                              break;
                           case AI_TYPE_FLOAT:
                              AiArraySetFlt(params[k], index + offset, AiArrayGetFlt(paramArray,j + nelements*step));
                              break;
                           case AI_TYPE_RGB:
                              AiArraySetRGB(params[k], index + offset, AiArrayGetRGB(paramArray,j + nelements*step));
                              break;
                           case AI_TYPE_VECTOR:
                              AiArraySetVec(params[k], index + offset, AiArrayGetVec(paramArray,j + nelements*step));
                              break;
                           case AI_TYPE_STRING:
                              AiArraySetStr(params[k], index + offset, AiArrayGetStr(paramArray,j + nelements*step));
                              break;
                           }

                        }

//                     }
                     k++;
                  }
                  AiUserParamIteratorDestroy(iter);

                  index++;
               }

            }

         }

         // OBJS

         if (AiNodeLookUpUserParameter(node, "objects") != NULL)
         {
            objArray = AiArrayCopy(AiNodeGetArray(node, "objects"));
            nbObjects = AiArrayGetNumElements(objArray);
         }

         if (nbObjects == 0)
         {
            AiMsgError("[INSTANCER] No object to instance");
            AiRenderAbort();
         }

      }
      else
      {
         AiMsgError("[INSTANCER] data is not a particle system");
         AiRenderAbort();
      }

   }
   else
   {
      AiMsgError("[INSTANCER] data is not a valid node");
      AiRenderAbort();
   }

   return true;
}

procedural_cleanup
{
   AiMsgInfo("[INSTANCER] Cleaning datas");

   for (size_t i = 0; i < params.size(); ++i)
      AiArrayDestroy(params[i]);

   params.resize(0);
   params.clear();
   paramsName.resize(0);
   paramsName.clear();

   AiArrayDestroy(matrixArray);
   AiArrayDestroy(indxArray);
   AiArrayDestroy(objArray);
   AiArrayDestroy(partArray);

   AiMsgInfo("[INSTANCER] Done cleaning datas");
   return true;
}

// Get number of nodes
procedural_num_nodes
{
   return nbInsts;
}

// Get the i_th node
procedural_init_bounds
{
   // FIXME Arnold5 return true or false? 
   return false;
}

procedural_get_node
{
   AtNode *instance;
   instance = AiNode("ginstance");
   char nodeName[MAX_NAME_SIZE];
   AiNodeSetStr(instance, "name", NodeUniqueName(instance, nodeName));
   
   //AiNodeSetInt(instance, "visibility", AI_RAY_ALL);
   int idx = AiArrayGetInt(indxArray, i);
   AtNode* obj = AiNodeLookUpByName(AiArrayGetStr(objArray,idx));
   AiNodeSetPtr(instance, "node", obj);
   AiNodeSetBool(instance, "inherit_xform", false);
   int steps = AiArrayGetNumKeys(partArray);

   AtMatrix instancerMatrix = AiNodeGetMatrix(node, "matrix");

   AtArray* matrices = AiArrayAllocate(1, steps, AI_TYPE_MATRIX);

   for (int step = 0; step < steps; step++)
   {

      AtMatrix matrix;

      int index = AiArrayGetInt(partArray, i * AiArrayGetNumKeys(matrixArray) + step);

      matrix = AiArrayGetMtx(matrixArray,index);


      AiArraySetMtx(matrices, step, matrix);

   }

   AiNodeSetArray(instance, "matrix", matrices);

   //user attributes
   for (size_t j = 0; j < params.size(); ++j)
   {

      int type = AiArrayGetType(params[j]);
      int steps = AiArrayGetNumKeys(params[j]);

      AtVector value;
      for (int step = 0; step < steps; ++step)
      {
         unsigned nelements = AiArrayGetNumElements(params[j]);
         switch (type)
         {
         case AI_TYPE_INT:
            AiNodeDeclare(instance, paramsName[j], "constant INT");
            AiNodeSetInt(instance, paramsName[j],
                  AiArrayGetInt(params[j], i + (nelements*step)));
            break;
         case AI_TYPE_FLOAT:
            AiNodeDeclare(instance, paramsName[j], "constant FLOAT");
            AiNodeSetFlt(instance, paramsName[j],
                  AiArrayGetFlt(params[j], i + (nelements*step)));
            break;
         case AI_TYPE_RGB:
            AiNodeDeclare(instance, paramsName[j], "constant RGB");
            value.x = AiArrayGetRGB(params[j], i + (nelements*step)).r;
            value.y = AiArrayGetRGB(params[j], i + (nelements*step)).g;
            value.z = AiArrayGetRGB(params[j], i + (nelements*step)).b;
            AiNodeSetRGB(instance, paramsName[j], value.x, value.y, value.z);
            break;
         case AI_TYPE_VECTOR:
            AiNodeDeclare(instance, paramsName[j], "constant VECTOR");
            value = AiArrayGetVec(params[j], i + (nelements*step));
            AiNodeSetVec(instance, paramsName[j], value.x, value.y, value.z);
            break;
         case AI_TYPE_STRING:
            AiNodeDeclare(instance, paramsName[j], "constant STRING");
            AiNodeSetStr(instance, paramsName[j],
                  AiArrayGetStr(params[j], i + (nelements*step)));
            break;
         }

      }
   }

   return instance;
}

// DSO hook
#ifdef __cplusplus
extern "C"
{
#endif

node_loader
{
   if (i>0)
      return false;

   node->methods      = ParticleInstancerMtd;
   node->output_type  = AI_TYPE_NONE;
   node->name         = "particleInstancer"; // FIXME Arnold5
   node->node_type    = AI_NODE_SHAPE_PROCEDURAL;
   strcpy(node->version, AI_VERSION);

   return true;
}



#ifdef __cplusplus
}
#endif
