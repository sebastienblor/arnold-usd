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

static int Init(AtNode *mynode, void **user_ptr)
{

   *user_ptr = mynode;
   particleSystem = AiNodeLookUpByName(AiNodeGetStr(mynode, "data"));

   if (AiNodeLookUpUserParameter(mynode, "pathIndices") == NULL)
   {
      AiMsgError("[INSTANCER] No path Indices");
      AiRenderAbort();
   }

   AtArray* pathIndicesArray = AiArrayCopy(AiNodeGetArray(mynode, "pathIndices"));

   if (AiNodeLookUpUserParameter(mynode, "pathStartIndices") == NULL)
   {
      AiMsgError("[INSTANCER] No path Start Indices");
      AiRenderAbort();
   }

   AtArray* pathStartIndices = AiArrayCopy(AiNodeGetArray(mynode, "pathStartIndices"));

   AtArray* pathMatrixArray = AiArrayCopy(AiNodeGetArray(mynode, "instanceMatrix"));


   if (particleSystem)
   {
      if (AiNodeIs(particleSystem, "points"))

      {
         AiNodeSetInt(particleSystem, "visibility", 0);

         if (pathMatrixArray->nelements == 0)
         {
            AiMsgError("[INSTANCER] No matrix datas.");
            AiMsgError("[INSTANCER] Check preserve_scene_data option.");
            AiRenderAbort();
         }

         nbInsts = pathIndicesArray->nelements;

         AiMsgInfo("[INSTANCER] instanciating %i objects", nbInsts);

         indxArray = AiArrayAllocate(nbInsts, 1, AI_TYPE_INT);

         partArray = AiArrayAllocate(nbInsts, pathMatrixArray->nkeys, AI_TYPE_INT);


         matrixArray = AiArrayAllocate(nbInsts, pathMatrixArray->nkeys, AI_TYPE_MATRIX);

         // Preparing array for user attributes
         AtUserParamIterator *iter = AiNodeGetUserParamIterator(particleSystem);
         while (!AiUserParamIteratorFinished(iter))
         {
            const AtUserParamEntry *upentry = AiUserParamIteratorGetNext(iter);
            const char* attrName = AiUserParamGetName(upentry);


            AtArray* paramArray = AiNodeGetArray(particleSystem, attrName);

            int type = paramArray->type;

            params.push_back(AiArrayAllocate(nbInsts, paramArray->nkeys, type));

            paramsName.push_back(attrName);

            AiMsgInfo("[INSTANCER] Exporting attribute : %s", attrName);


         }
         AiUserParamIteratorDestroy(iter);

         int index = 0;

         for (size_t i = 0; i < pathMatrixArray->nelements; ++i)
         {



            int pathEnd = AiArrayGetInt(pathStartIndices,(AtULong)i+1);
            int pathStart = AiArrayGetInt(pathStartIndices,(AtULong)i);
            int nbPath = pathEnd - pathStart;

            if (nbPath != 0)
            {
               for (int j = pathStart; j <= pathEnd - 1; ++j)
               {

                  AiArraySetInt(indxArray, index, AiArrayGetInt(pathIndicesArray,j ));

                  for (int step = 0; step < pathMatrixArray->nkeys; step++)
                  {

                     AtMatrix curPathMatrix;
                     AiArrayGetMtx(pathMatrixArray, j * pathMatrixArray->nkeys + step, curPathMatrix);
                     AiArraySetMtx(matrixArray, index * pathMatrixArray->nkeys + step, curPathMatrix);
                  }

                  for (int step = 0; step < pathMatrixArray->nkeys; step++)
                  {
                     int offset = nbInsts * step;
                     AiArraySetInt(partArray, index + offset, AtULong(i + pathMatrixArray->nelements*step));



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

                        int type = paramArray->type;

                        for (int step = 0; step < paramArray->nkeys; step++)
                        {
                           int offset = nbInsts * step;
                           switch (type)
                           {
                           case 1:
                              AiArraySetInt(params[k], index + offset, AiArrayGetInt(paramArray,j + paramArray->nelements*step));
                              break;
                           case 4:
                              AiArraySetFlt(params[k], index + offset, AiArrayGetFlt(paramArray,j + paramArray->nelements*step));
                              break;
                           case 5:
                              AiArraySetRGB(params[k], index + offset, AiArrayGetRGB(paramArray,j + paramArray->nelements*step));
                              break;
                           case 7:
                              AiArraySetVec(params[k], index + offset, AiArrayGetVec(paramArray,j + paramArray->nelements*step));
                              break;
                           case 8:
                              AiArraySetPnt(params[k], index + offset, AiArrayGetPnt(paramArray,j + paramArray->nelements*step));
                              break;
                           case 10:
                              AiArraySetStr(params[k], index + offset, AiArrayGetStr(paramArray,j + paramArray->nelements*step));
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

         if (AiNodeLookUpUserParameter(mynode, "objects") != NULL)
         {
            objArray = AiArrayCopy(AiNodeGetArray(mynode, "objects"));
            nbObjects = objArray->nelements;
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

// All done, deallocate stuff
static int Cleanup(void *user_ptr)
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
static int NumNodes(void *user_ptr)
{
   return nbInsts;
}

// Get the i_th node


static AtNode *GetNode(void *user_ptr, int i)
{
   AtNode *procnode = (AtNode*) user_ptr;
   AtNode *instance;
   instance = AiNode("ginstance");
   char nodeName[MAX_NAME_SIZE];
   AiNodeSetStr(instance, "name", NodeUniqueName(instance, nodeName));
   
   //AiNodeSetInt(instance, "visibility", AI_RAY_ALL);
   int idx = AiArrayGetInt(indxArray, i);
   AtNode* obj = AiNodeLookUpByName(AiArrayGetStr(objArray,idx));
   AiNodeSetPtr(instance, "node", obj);
   AiNodeSetBool(instance, "inherit_xform", false);
   int steps = partArray->nkeys;

   AtMatrix instancerMatrix;
   AiNodeGetMatrix(procnode, "matrix", instancerMatrix);





   AtArray* matrices = AiArrayAllocate(1, steps, AI_TYPE_MATRIX);

   for (int step = 0; step < steps; step++)
   {

      AtMatrix matrix;

      int index = AiArrayGetInt(partArray, i * matrixArray->nkeys + step);

      AiArrayGetMtx(matrixArray,index, matrix);


      AiArraySetMtx(matrices, step, matrix);

   }

   AiNodeSetArray(instance, "matrix", matrices);

   //user attributes
   for (size_t j = 0; j < params.size(); ++j)
   {

      int type = params[j]->type;
      int steps = params[j]->nkeys;

      AtVector value;
      for (int step = 0; step < steps; ++step)
      {
         switch (type)
         {
         case 1:
            AiNodeDeclare(instance, paramsName[j], "constant INT");
            AiNodeSetInt(instance, paramsName[j],
                  AiArrayGetInt(params[j], i + (params[j]->nelements*step)));
            break;
         case 4:
            AiNodeDeclare(instance, paramsName[j], "constant FLOAT");
            AiNodeSetFlt(instance, paramsName[j],
                  AiArrayGetFlt(params[j], i + (params[j]->nelements*step)));
            break;
         case 5:
            AiNodeDeclare(instance, paramsName[j], "constant RGB");
            value.x = AiArrayGetRGB(params[j], i + (params[j]->nelements*step)).r;
            value.y = AiArrayGetRGB(params[j], i + (params[j]->nelements*step)).g;
            value.z = AiArrayGetRGB(params[j], i + (params[j]->nelements*step)).b;
            AiNodeSetRGB(instance, paramsName[j], value.x, value.y, value.z);
            break;
         case 7:
            AiNodeDeclare(instance, paramsName[j], "constant VECTOR");
            value = AiArrayGetVec(params[j], i + (params[j]->nelements*step));
            AiNodeSetVec(instance, paramsName[j], value.x, value.y, value.z);
            break;
         case 8:
            AiNodeDeclare(instance, paramsName[j], "constant POINT");
            value.x = AiArrayGetPnt(params[j], i + (params[j]->nelements*step)).x;
            value.y = AiArrayGetPnt(params[j], i + (params[j]->nelements*step)).y;
            value.z = AiArrayGetPnt(params[j], i + (params[j]->nelements*step)).z;
            AiNodeSetPnt(instance, paramsName[j], value.x, value.y, value.z);
            break;
         case 10:
            AiNodeDeclare(instance, paramsName[j], "constant STRING");
            AiNodeSetStr(instance, paramsName[j],
                  AiArrayGetStr(params[j], i + (params[j]->nelements*step)));
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

AI_EXPORT_LIB int ProcLoader(AtProcVtable *vtable)
{
   vtable->Init = Init;
   vtable->Cleanup = Cleanup;
   vtable->NumNodes = NumNodes;
   vtable->GetNode = GetNode;

   sprintf(vtable->version, AI_VERSION);
   return 1;
}

#ifdef __cplusplus
}
#endif
