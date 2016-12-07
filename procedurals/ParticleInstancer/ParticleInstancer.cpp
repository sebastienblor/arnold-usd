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

         if (AiNodeLookUpUserParameter(mynode, "objects") != NULL)
         {
            objArray = AiArrayCopy(AiNodeGetArray(mynode, "objects"));
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

// All done, deallocate stuff
static int Cleanup(const AtNode *node, void *user_ptr)
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
static int NumNodes(const AtNode *node, void *user_ptr)
{
   return nbInsts;
}

// Get the i_th node


static AtNode *GetNode(const AtNode *node, void *user_ptr, int i)
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

AI_EXPORT_LIB int ProcLoader(AtProceduralNodeMethods *vtable)
{
   vtable->Init = Init;
   vtable->Cleanup = Cleanup;
   vtable->NumNodes = NumNodes;
   vtable->GetNode = GetNode;

   // FIXME Arnold5
   //sprintf(vtable->version, AI_VERSION);
   return 1;
}

#ifdef __cplusplus
}
#endif
