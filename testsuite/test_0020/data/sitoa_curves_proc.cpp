
#include <ai.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

char* datafile;

// Reads the parameter value from data file and assign it to node
AtVoid ReadParameterValue(AtNode* curve_node, FILE* fp, const AtChar* param_name);

// Startup -- on error return false.
static int Init(AtNode *mynode, void **user_ptr)
{
   datafile = (char*) AiNodeGetStr(mynode, "data");
   return TRUE;
}

// All done, deallocate stuff
static int Cleanup(void *user_ptr)
{
   return TRUE;
}


// Get number of nodes
static int NumNodes(void *user_ptr)
{
   return 1;
}


// Get the i_th node
static AtNode *GetNode(void *user_ptr, int i)
{
   AtNode *curveNode;
   curveNode = AiNode("curves");

   if(curveNode!=NULL)
   {
      // Hardcode for catmull-rom
      AiNodeSetStr(curveNode, "basis", "catmull-rom");

      FILE *fp = fopen(datafile, "rb");

      if(fp != NULL)
      {
         AtChar signature[12];

         // Reading the header
         size_t readed = fread(&signature, sizeof(AtChar), 12, fp);

         // Check if header is correctly read
         if(readed < 1)
         {
            AiMsgError("Can't Read Signature from DataFile");
            AiRenderAbort();
            return NULL;
         }

         // Check if this is a sitoa curves binary file
         if(strncmp(signature, "SITOA_CURVES", 12) != 0)
         {
            AiMsgError("File is not a valid DataFile for sitoa_curves_proc Procedural");
            AiRenderAbort();
            return NULL;
         }

         ////////////////////////////////////////////////////////

         // Reading Node Name
         AtUInt32 len;
         fread(&len, sizeof(AtUInt32), 1, fp);

         AtChar *nodename = (AtChar*) AiMalloc(sizeof(AtChar)*(len+1));
         fread(nodename, sizeof(AtChar), len+1, fp);
         AiNodeSetStr(curveNode, "name", nodename);
         AiFree(nodename);

         ////////////////////////////////////////////////////////

         // Reading Object ID
         AtUInt32 objectID;
         fread(&objectID, sizeof(AtUInt32), 1, fp);
         AiNodeSetInt(curveNode, "id", objectID);

         ////////////////////////////////////////////////////////

         // Reading shader Name assigned
         fread(&len, sizeof(AtUInt32), 1, fp);

         if(len>0)
         {
            AtChar *shadername = (AtChar*) AiMalloc(sizeof(AtChar)*(len+1));
            fread(shadername, sizeof(AtChar), len+1, fp);

            AtNode* shader_node = AiNodeLookUpByName(shadername);
            if(shader_node!=NULL)
               AiNodeSetArray(curveNode, "shader", AiArray(1, 1, AI_TYPE_NODE, shader_node));

            AiFree(shadername);
         }

         ////////////////////////////////////////////////////////

         // Visibility & Sidedness
         AtUInt32 visibility;
         AtUInt32 sidedness;

         fread(&visibility, sizeof(AtUInt32), 1, fp);
         AiNodeSetInt(curveNode, "visibility", visibility);

         fread(&sidedness, sizeof(AtUInt32), 1, fp);
         AiNodeSetInt(curveNode, "sidedness", sidedness);

         ////////////////////////////////////////////////////////

         // Arnold Specific Parameters
         AtUInt32 num_parameters;
         fread(&num_parameters, sizeof(AtUInt32), 1, fp);

         for(AtUInt32 iparam=0; iparam<num_parameters; iparam++)
         {
            AtUInt32 len;
            fread(&len, sizeof(AtUInt32), 1, fp);

            AtChar *param_name = (AtChar*) AiMalloc(sizeof(AtChar)*(len+1));
            fread(param_name, sizeof(AtChar), len+1, fp);

            ReadParameterValue(curveNode, fp, param_name);

            AiFree(param_name);
         }

         ////////////////////////////////////////////////////////

         // Light Groups
         bool use_light_group;
         fread(&use_light_group, sizeof(bool), 1, fp);
         AiNodeSetBool(curveNode, "use_light_group", use_light_group);

         // Reading Lights Names
         if(use_light_group)
         {
            AtUInt32 nlights;
            fread(&nlights, sizeof(AtUInt32), 1, fp);

            AtArray* light_group = AiArrayAllocate(nlights, 1, AI_TYPE_NODE);

            for(AtUInt32 ilight=0; ilight<nlights; ilight++)
            {
               AtUInt32 len;
               fread(&len, sizeof(AtUInt32), 1, fp);

               AtChar *light_name = (AtChar*) AiMalloc(sizeof(AtChar)*(len+1));
               fread(light_name, sizeof(AtChar), len+1, fp);

               AtNode* lightNode = AiNodeLookUpByName(light_name);
               AiArraySetPtr(light_group, ilight, lightNode);
               AiFree(light_name);
            }

            AiNodeSetArray(curveNode, "light_group", light_group);
         }

         ////////////////////////////////////////////////////////

         // UV Projections
         AtUInt32 nuvs;
         fread(&nuvs, sizeof(AtUInt32), 1, fp);

         for(AtUInt32 iuv=0; iuv<nuvs; iuv++)
         {
            // Reading Projection Name
            AtUInt32 len;
            fread(&len, sizeof(AtUInt32), 1, fp);

            if(len>0)
            {
               AtChar *projectionname = (AtChar*) AiMalloc(sizeof(AtChar)*(len+1));
               fread(projectionname, sizeof(AtChar), len+1, fp);

               // Declaring userdef
               if(AiNodeDeclare(curveNode, projectionname, "uniform POINT2"))
               {
                  // Reading UV Array
                  AtUInt32 nuvsfloats;
                  fread(&nuvsfloats, sizeof(AtUInt32), 1, fp);

                   // Allocating memory
                  AtFloat *uvs_array = (AtFloat*) AiMalloc(sizeof(AtFloat)*nuvsfloats);
                  fread(uvs_array, sizeof(AtFloat), nuvsfloats, fp);

                  // Setting data into Arnold Array
                  AtArray* uvs = AiArrayAllocate(nuvsfloats/2, 1, AI_TYPE_POINT2);
                  AtPoint2 uv;

                  AtUInt counter = 0;
                  for(AtUInt32 j=0; j<nuvsfloats; j+=2)
                  {
                     uv.x = (AtFloat) uvs_array[j];
                     uv.y = (AtFloat) uvs_array[j+1];
                     AiArraySetPnt2(uvs, counter++, uv);
                  }

                  // Assigning Array to Node
                  AiNodeSetArray(curveNode, projectionname, uvs);

                  AiFree(uvs_array);
               }

               AiFree(projectionname);
            }
         }



         ////////////////////////////////////////////////////////

         //Reading matrix samples (for mblur)
         AtUInt32 num_matrix;
         fread(&num_matrix, sizeof(AtUInt32), 1, fp);

         // Allocating Array
         AtArray* matrixs = AiArrayAllocate(1, num_matrix, AI_TYPE_MATRIX);

         // Reading each matrix
         for(AtUInt i=0; i<num_matrix; i++)
         {
            AtMatrix matrix;

            fread(&matrix[0][0], sizeof(AtFloat), 1, fp);
            fread(&matrix[0][1], sizeof(AtFloat), 1, fp);
            fread(&matrix[0][2], sizeof(AtFloat), 1, fp);
            fread(&matrix[0][3], sizeof(AtFloat), 1, fp);

            fread(&matrix[1][0], sizeof(AtFloat), 1, fp);
            fread(&matrix[1][1], sizeof(AtFloat), 1, fp);
            fread(&matrix[1][2], sizeof(AtFloat), 1, fp);
            fread(&matrix[1][3], sizeof(AtFloat), 1, fp);

            fread(&matrix[2][0], sizeof(AtFloat), 1, fp);
            fread(&matrix[2][1], sizeof(AtFloat), 1, fp);
            fread(&matrix[2][2], sizeof(AtFloat), 1, fp);
            fread(&matrix[2][3], sizeof(AtFloat), 1, fp);

            fread(&matrix[3][0], sizeof(AtFloat), 1, fp);
            fread(&matrix[3][1], sizeof(AtFloat), 1, fp);
            fread(&matrix[3][2], sizeof(AtFloat), 1, fp);
            fread(&matrix[3][3], sizeof(AtFloat), 1, fp);

            AiArraySetMtx(matrixs, i, matrix);
         }

         AiNodeSetArray(curveNode, "matrix", matrixs);

         ////////////////////////////////////////////////////////

         // Reading num_points from file
         AtUInt32 num_points;
         fread(&num_points, sizeof(AtUInt32), 1, fp);

         // Setting Array of num_points (hardcoded 1 value)
         AiNodeSetArray(curveNode, "num_points", AiArray(1, 1, AI_TYPE_UINT, num_points));

         ////////////////////////////////////////////////////////

         // Reading Radius Array
         AtUInt32 num_radius_array;
         fread(&num_radius_array, sizeof(AtUInt32), 1, fp);

         // Allocating memory
         AtFloat *radius_array = (AtFloat*) AiMalloc(sizeof(AtFloat)*num_radius_array);
         fread(radius_array, sizeof(AtFloat), num_radius_array, fp);

         //Settings Radius Array
         AtArray *radius = AiArrayAllocate(num_radius_array, 1, AI_TYPE_FLOAT);

         for(AtUInt32 i=0;i <num_radius_array; i++)
            ((AtFloat*)(radius->data))[i] = (AtFloat)radius_array[i];

         AiNodeSetArray(curveNode, "radius", radius);
         AiFree(radius_array);

         ////////////////////////////////////////////////////////

         // Reading Samples number for deform motion blur
         AtUInt32 num_samples;
         fread(&num_samples, sizeof(AtUInt32), 1, fp);

         // Reading Points from file for each sample
         AtUInt32 num_points_array;
         fread(&num_points_array, sizeof(AtUInt32), 1, fp);

         AtLong total_points = num_samples*num_points_array;

         // Allocating memory
         AtFloat *points_array = (AtFloat*) AiMalloc(sizeof(AtFloat)*total_points);
         fread(points_array, sizeof(AtFloat), total_points, fp);

         // Allocating points Array
         AtArray *points = AiArrayAllocate(num_points_array, num_samples, AI_TYPE_FLOAT);

         // Filling arnold array
         for(AtUInt32 i=0; i<(AtUInt32)total_points; i++)
            ((AtFloat*)(points->data))[i] = (AtFloat) points_array[i];

         // Setting parameter value
         AiNodeSetArray(curveNode, "points", points);
         AiFree(points_array);

         ////////////////////////////////////////////////////////

         // Reading curve ID array
         if(AiNodeDeclare(curveNode, "curve_id", "uniform UINT"))
         {
            AtUInt32 nids;
            fread(&nids, sizeof(AtUInt32), 1, fp);

            AtUInt32 *ids_array = (AtUInt32*) AiMalloc(sizeof(AtUInt32)*nids);
            fread(ids_array, sizeof(AtUInt32), nids, fp);

            AtArray *curve_id = AiArrayAllocate(nids, 1, AI_TYPE_UINT);

            // Filling arnold array
            for(AtUInt32 i=0; i<nids; i++)
               ((AtUInt32*)(curve_id->data))[i] = (AtUInt32) ids_array[i];

            AiNodeSetArray(curveNode, "curve_id", curve_id);
            AiFree(ids_array);
         }
      }
      else
      {
         AiMsgError("Can't Read DataFile %s", datafile);
         AiRenderAbort();
      }
   }

   return curveNode;
}

// Reads the parameter value from data file and assign it to node
AtVoid ReadParameterValue(AtNode* curve_node, FILE* fp, const AtChar* param_name)
{
   // Search param
   const AtParamEntry* param_entry = AiNodeEntryLookUpParameter(curve_node->base_node, param_name);
   AtInt param_type = AiParamGetType(param_entry);

   if(param_type != AI_TYPE_UNDEFINED)
   {
      // Reading parameter value
      switch(param_type)
      {
         case AI_TYPE_BYTE:
         {
            AtByte value;
            fread(&value, sizeof(AtByte), 1, fp);
            AiNodeSetByte(curve_node, param_name, value);
            break;
         }
         case AI_TYPE_INT:
         {
            AtInt value;
            fread(&value, sizeof(AtInt), 1, fp);
            AiNodeSetInt(curve_node, param_name, value);
            break;
         }
         case AI_TYPE_UINT:
         {
            AtLong value;
            fread(&value, sizeof(AtUInt), 1, fp);
            AiNodeSetUInt(curve_node, param_name, value);
            break;
         }
         case AI_TYPE_BOOLEAN:
         {
            bool value;
            fread(&value, sizeof(bool), 1, fp);
            AiNodeSetBool(curve_node, param_name, value);
            break;
         }
         case AI_TYPE_FLOAT:
         {
            AtFloat value;
            fread(&value, sizeof(AtFloat), 1, fp);
            AiNodeSetFlt(curve_node, param_name, value);
            break;
         }
         case AI_TYPE_ENUM:
         case AI_TYPE_STRING:
         {
            AtUInt len;
            fread(&len, sizeof(AtUInt), 1, fp);

            AtChar *value = (AtChar*) AiMalloc(sizeof(AtChar)*(len+1));
            fread(value, sizeof(AtChar), len+1, fp);

            AiNodeSetStr(curve_node, param_name, value);
            AiFree(value);
            break;
         }

         // If other type found, we will abort the render
         default:
            AiMsgError("Not valid parameter type found on Data File");
            AiRenderAbort();
      }
   }
   else
   {
      AtInt value;
      // Read Null parameters, which are defined as AtInt (see ticket #1064)
      fread(&value, sizeof(AtInt), 1, fp);
   }
}




// DSO hook
#ifdef __cplusplus
extern "C" {
#endif

AI_EXPORT_LIB int ProcLoader(AtProcVtable *vtable)
{
   vtable->Init     = Init;
   vtable->Cleanup  = Cleanup;
   vtable->NumNodes = NumNodes;
   vtable->GetNode  = GetNode;

   sprintf(vtable->version, AI_VERSION);
   return 1;
}

#ifdef __cplusplus
}
#endif
