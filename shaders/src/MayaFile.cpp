#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <map>

#include <ai.h>

#include "MayaUtils.h"

#define MAX_FILENAME 500

AI_SHADER_NODE_EXPORT_METHODS(MayaFileMtd);

namespace
{

// Modes: SHAPE_NAME, SHAPE_PATH, TILE and USER_PARAM has a variable token size,
//  so they will require break the file path where they are found to be able to
//  insert the evaluated token value in the correct place.
// Mode UDIM has a fixed token size = 4. So This token will not break the file path
//  where it is found, but it will be replaced by a 4 character mask that will be
//  updated in the evaluation of the token to improve performance
enum TokenModes
{
   SHAPE_NAME = 0,
   SHAPE_PATH,
   UDIM,
   TILE,
   USER_PARAM
};

enum MayaFileParams
{
   p_coverage = 0,
   p_translate_frame,
   p_rotate_frame,
   p_mirror_u,
   p_mirror_v,
   p_wrap_u,
   p_wrap_v,
   p_stagger,
   p_repeat,
   p_offset,
   p_rotate,
   p_filename,
   p_noise,
   p_mip_bias,
   p_filter,
   p_use_default_color,
   MAYA_COLOR_BALANCE_ENUM
};

};

struct TokenData
{
   TokenModes mode;
   int position;    // Position on AtImageData.origName of next string chunk after token
                    //  For UDIM token, as it does not break the current string chunk,
                    //  it is the distance from the end of the current chunk to the begining
                    //  of the UDIM mask as a negative number.

   int nextSize;    // Size of next string chunk after token. For the UDIM token, it does
                    //  not have any meaning so it will be 0
   void* extra;
};

typedef struct AtImageData
{
   char* origPath;        // Original file path without tokens
   char** processPath;    // Current evaluated file path (one for each thread)
   unsigned int startPos; // First position in the "processPath" where the first token will be added
   TokenData* tokens;
   unsigned int ntokens;
   AtTextureHandle* texture_handle;
} AtImageData;

static const char* filterNames[] = {"closest", "bilinear", "bicubic", "smart_bicubic", 0};

node_parameters
{
   AiParameterPNT2("coverage", 1.0f, 1.0f);
   AiParameterPNT2("translateFrame", 0.0f, 0.0f);
   AiParameterFLT("rotateFrame", 0.0f);
   AiParameterBOOL("mirrorU", false);
   AiParameterBOOL("mirrorV", false);
   AiParameterBOOL("wrapU", true);
   AiParameterBOOL("wrapV", true);
   AiParameterBOOL("stagger", false);
   AiParameterPNT2("repeatUV", 1.0f, 1.0f);
   AiParameterPNT2("offsetUV", 0.0f, 0.0f);
   AiParameterFLT("rotateUV", 0.0f);
   AiParameterSTR("filename", "");
   AiParameterPNT2("noiseUV", 0.0f, 0.0f);
   AiParameterINT("mipBias", 0);
   AiParameterENUM("filter", 3, filterNames);
   AiParameterBOOL("useDefaultColor", true);
   AddMayaColorBalanceParams(params, mds);
   
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
}

node_initialize
{
}

node_update
{
   // cleanup
   // since node_update may be called multiple times and a static memory allocation in node_init
   // will not suffice, we need to free memory between updates
   Finish(node);

   AtImageData *idata = (AtImageData*) AiMalloc(sizeof(AtImageData));
   idata->origPath = NULL;
   idata->processPath = NULL;
   idata->startPos = 0;
   idata->tokens = NULL;      
   idata->ntokens = 0;
   idata->texture_handle = NULL;   
   
   // Check if the filename has a shading network. If it does, we will fallback to the name based
   // lookup and avoid the token computation
   if (!AiNodeGetLink(node, "filename"))
   {
      const char *filename = params[p_filename].STR;

      // calculate file tokens
      std::vector<TokenData> tokens;

      std::string fname(filename);
      std::string newfname("");
      std::map< std::string, std::string > processedTokens;
      std::map< std::string, std::string >::iterator iter;
      std::string::size_type lastPos = 0;
      // Find first "non-delimiter".
      std::string::size_type pos = 0;
      std::string::size_type end = fname.length();
      bool opened = false;
      int prevToken = -1;
      int firstBreak = 0;
      bool breakFound = false;
      while ( pos < end && pos != std::string::npos &&
            lastPos < end && lastPos != std::string::npos)
      {
         lastPos = pos;
         // Find next "non-delimiter"
         if (opened)
         {
            pos = fname.find(">", lastPos);
            opened = false;
            if (pos != std::string::npos)
               pos = pos + 1;
         }
         else
         {
            pos = fname.find("<", lastPos);
            opened = true;
         }

         // Found a token, add it to the vector.
         std::string sub = fname.substr(lastPos, pos - lastPos);
         if (opened)
         {
            // a non-token file path part. "sub" has a chunk of text without tokens.
            newfname += sub;
            // If we are at the end of the file path and there is a token that broke the file path chunk, update its "nextSize" attribute
            if (pos == std::string::npos && prevToken >= 0)
               tokens[prevToken].nextSize = (int)newfname.size() - tokens[prevToken].position;
            // If we have not found a token that break the first file path text, increase the size of the "firstBreak" value
            if (!breakFound)
               firstBreak = (int)newfname.size();
            // If last token was a UDIM
            if (tokens.size() > 0 && tokens.back().mode == UDIM)
               // Update it to be the position of the begining of the UDIM mask
               //  relative to the end of the string chunk. Example:
               //   text####text
               //       |
               //       \------- = "-8"
               tokens.back().position = tokens.back().position - (int) newfname.size();
         }
         else
         {
            // process the tokens
            if (sub == "<shapeName>")
            {
               TokenData data;
               data.mode = SHAPE_NAME;
               data.position = (int) newfname.size();
               data.extra = NULL;
               data.nextSize = 0;
               // If a previous token broke the file path chunk, update its "nextSize" attribute
               if (prevToken >= 0)
                  tokens[prevToken].nextSize = (int) newfname.size() - tokens[prevToken].position;
               tokens.push_back(data);
               // Set this token as the last that broke the text chunk
               prevToken = (int) tokens.size()-1;
               breakFound = true;
            }
            else if (sub == "<shapePath>")
            {
               TokenData data;
               data.mode = SHAPE_PATH;
               data.position = (int) newfname.size();
               data.extra = NULL;
               data.nextSize = 0;
               // If a previous token broke the file path chunk, update its "nextSize" attribute
               if (prevToken >= 0)
                  tokens[prevToken].nextSize = (int) newfname.size() - tokens[prevToken].position;
               tokens.push_back(data);
               // Set this token as the last that broke the text chunk
               prevToken = (int) tokens.size()-1;
               breakFound = true;
            }
            else if (sub.substr(0, 6) == "<attr:")
            {
               std::string attr = sub.substr(6, sub.length()-7);
               TokenData data;
               data.mode = USER_PARAM;
               data.position = (int) newfname.size();
               data.extra = AiMalloc((unsigned long)attr.size() + 1);
               strcpy((char*)data.extra, attr.c_str());
               ((char*)data.extra)[attr.size()] = 0;
               data.nextSize = 0;
               // If a previous token broke the file path chunk, update its "nextSize" attribute
               if (prevToken >= 0)
                  tokens[prevToken].nextSize = (int) newfname.size() - tokens[prevToken].position;
               tokens.push_back(data);
               // Set this token as the last that broke the text chunk
               prevToken = (int) tokens.size()-1;
               breakFound = true;
            }
            else if (sub.substr(0, 5) == "<udim")
            {
               /*
               mari/zbrush/cyslice-style
               Mari has a 4 digit number equal to 1000 + (u + 1 + v*10). UV tile [0,0] x (1,1) is assigned 1001
               UV tile [0,1]x(1,2) is assigned 1011 etc. 
               */

               std::string::size_type len = sub.length();
               // default dimension
               int dim = 10;
               // a specific dimension can be provided with <udim:5>
               if (len > 7)
               {
                  dim = atoi(sub.substr(6, len-7).c_str());
                  dim = dim <= 0 ? 1 : dim;
               }
               TokenData data;
               data.mode = UDIM;
               // Position will be updated when the end of the text chunk is reached
               data.position = (int) newfname.size();
               data.extra = AiMalloc(sizeof(int));
               *((int*)data.extra) = dim;

               tokens.push_back(data);
               // This token does not break the text chunk, we only add a 4 characters
               //  mask to be replaced at evaluation time
               newfname += "1001";
               // Just in case file name ends here
               if (!breakFound)
                  firstBreak += 4;
            }
            else if (sub == "<tile>" )
            {
               TokenData data;
               data.mode = TILE;
               data.position = (int) newfname.size();
               data.extra = NULL;
               data.nextSize = 0;
               // If a previous token broke the file path chunk, update its "nextSize" attribute
               if (prevToken >= 0)
                  tokens[prevToken].nextSize = (int) newfname.size() - tokens[prevToken].position;
               tokens.push_back(data);
               // Set this token as the last that broke the text chunk
               prevToken = (int) tokens.size()-1;
               breakFound = true;
            }
            else
            {
               AiMsgError("unknown token %s", sub.c_str());
               newfname += sub;
            }
         }
      }

      idata->ntokens = (unsigned int)tokens.size();
      if (tokens.size())
      {
         idata->tokens = (TokenData*) AiMalloc((unsigned long) (sizeof(TokenData) * tokens.size()));
         int k = 0;
         for (std::vector<TokenData>::const_iterator it = tokens.begin(); it != tokens.end(); ++it, ++k)
            idata->tokens[k] = *it;

         idata->origPath = (char*) AiMalloc((unsigned long)newfname.size() + 1);
         strcpy(idata->origPath, newfname.c_str());

         // For each thread, create a processPath with the first text chunk already copied to it.
         AtNode* nodeOpt = AiUniverseGetOptions();
         int threads = AiNodeGetInt(nodeOpt, "threads");
         idata->processPath = (char**) AiMalloc(sizeof(char*) * threads);
         for(int k = 0; k < threads; k++)
         {
            idata->processPath[k] = (char*) AiMalloc(sizeof(char) * MAX_FILENAME);
            memcpy(idata->processPath[k],idata->origPath,firstBreak);
            idata->startPos = firstBreak;
            idata->processPath[k][firstBreak] = 0;
         }
      }
      else
         idata->texture_handle = AiTextureHandleCreate(AiNodeGetStr(node, "filename"));      
   }
   AiNodeSetLocalData(node, idata);   
}

node_finish
{
   AtImageData *idata = (AtImageData*) AiNodeGetLocalData(node);
   if (idata != NULL)
   {
      // Freeing ShaderData
      if(idata->texture_handle != NULL)
         AiTextureHandleDestroy(idata->texture_handle);      
         
      if (idata->ntokens > 0)
      {
         TokenData* token = idata->tokens;
         for (unsigned int i=0; i < idata->ntokens; i++, token++)
         {
            if (token->extra != NULL)
               AiFree(token->extra);
         }
         AiFree(idata->tokens);
         AiFree(idata->origPath);

         AtNode* nodeOpt = AiUniverseGetOptions();
         int threads = AiNodeGetInt(nodeOpt, "threads");
         for(int k = 0; k < threads; k++)
         {
            if (idata->processPath[k] != NULL)
               AiFree(idata->processPath[k]);
         }
         AiFree(idata->processPath);
      }
         
      AiFree(idata);
   }
}

shader_evaluate
{
   AtImageData *idata = (AtImageData*) AiNodeGetLocalData(node);
   
   AtPoint2 coverage = AiShaderEvalParamPnt2(p_coverage);
   AtPoint2 translate = AiShaderEvalParamPnt2(p_translate_frame);
   float frotate = AiShaderEvalParamFlt(p_rotate_frame);
   bool mirrorU = (AiShaderEvalParamBool(p_mirror_u) == true);
   bool mirrorV = (AiShaderEvalParamBool(p_mirror_v) == true);
   bool wrapU = (AiShaderEvalParamBool(p_wrap_u) == true);
   bool wrapV = (AiShaderEvalParamBool(p_wrap_v) == true);
   bool stagger = (AiShaderEvalParamBool(p_stagger) == true);
   AtPoint2 repeat = AiShaderEvalParamPnt2(p_repeat);
   AtPoint2 offset = AiShaderEvalParamPnt2(p_offset);
   float rotate = AiShaderEvalParamFlt(p_rotate);
   AtPoint2 noise = AiShaderEvalParamPnt2(p_noise);

   float inU = sg->u;
   float inV = sg->v;
   float inDuDx = sg->dudx;
   float inDuDy = sg->dudy;
   float inDvDx = sg->dvdx;
   float inDvDy = sg->dvdy;

   float outU = inU;
   float outV = inV;
   float outDuDx = inDuDx;
   float outDuDy = inDuDy;
   float outDvDx = inDvDx;
   float outDvDy = inDvDy;

   if (noise.x > 0.0f)
   {
      AtVector2 uv = {inU * 16, inV * 16};
      outU += noise.x * AiPerlin2(uv);
   }

   if (noise.y > 0.0f)
   {
      AtVector2 uv = {(1 - inU) * 16, (1 - inV) * 16};
      outV += noise.y * AiPerlin2(uv);
   }

   // for frame, rotate first then translate
   if (frotate <= -AI_EPSILON || frotate >= AI_EPSILON)
   {
      float x, y;
      float ca = cos(frotate);
      float sa = sin(frotate);

      x = outU - 0.5f;
      y = outV - 0.5f;
      outU = 0.5f + ca * x - sa * y;
      outV = 0.5f + ca * y + sa * x;

      x = outDuDx;
      y = outDuDy;
      outDuDx = ca * x - sa * y;
      outDuDy = ca * y + sa * x;

      x = outDvDx;
      y = outDvDy;
      outDvDx = ca * x - sa * y;
      outDvDy = ca * y + sa * x;
   }

   outU -= translate.x;
   outV -= translate.y;

   // If coverage.x or coverage.y are <= 1.0f
   //   check of the wrapped u or v coordinades respectively wraps in a valid range
   // If wrap is off, check incoming coordinate is in the range [0, 1]
   if (Mod(outU, 1.0f) > coverage.x ||
       Mod(outV, 1.0f) > coverage.y ||
       (!wrapU && (outU < 0 || outU > 1)) ||
       (!wrapV && (outV < 0 || outV > 1)))
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA);
   }
   else
   {
      float icx = 1.0f / coverage.x;
      float icy = 1.0f / coverage.y;

      if (coverage.x < 1.0f)
      {
         outU = Mod(outU, 1.0f);
      }

      if (coverage.y < 1.0f)
      {
         outV = Mod(outV, 1.0f);
      }

      outU *= icx;
      outV *= icy;

      // apply repetition factor
      outU *= repeat.x;
      outV *= repeat.y;

      // for UVs, translate first, then rotate
      outU += offset.x;
      outV += offset.y;

      outDuDx *= icx * repeat.x;
      outDuDy *= icx * repeat.x;
      outDvDx *= icy * repeat.y;
      outDvDy *= icy * repeat.y;

      // do mirror, stagger before rotation
      if (Mod(outV, 2.0f) >= 1.0f)
      {
         if (stagger)
         {
            outU += 0.5f;
         }

         if (mirrorV)
         {
            float center = floor(outV) + 0.5f;
            outV = center - (outV - center);

            outDuDy = -outDuDy;
            outDvDy = -outDvDy;
         }
      }
      
      if (mirrorU && Mod(outU, 2.0f) >= 1.0f)
      {
         float center = floor(outU) + 0.5f;
         outU = center - (outU - center);

         outDuDx = -outDuDx;
         outDvDx = -outDvDx;
      }

      // finally rotate UV
      if (rotate <= -AI_EPSILON || rotate >= AI_EPSILON)
      {
         float x, y;
         float ca = cos(rotate);
         float sa = sin(rotate);

         x = outU - 0.5f;
         y = outV - 0.5f;
         outU = 0.5f + ca * x - sa * y;
         outV = 0.5f + ca * y + sa * x;

         x = outDuDx;
         y = outDuDy;
         outDuDx = ca * x - sa * y;
         outDuDy = ca * y + sa * x;

         x = outDvDx;
         y = outDvDy;
         outDvDx = ca * x - sa * y;
         outDvDy = ca * y + sa * x;
      }
      
      // replace shader globals
      sg->u = outU;
      sg->v = outV;
      sg->dudx = outDuDx;
      sg->dudy = outDuDy;
      sg->dvdx = outDvDx;
      sg->dvdy = outDvDy;

      // do texture lookup
      AtTextureParams texparams;
      AiTextureParamsSetDefaults(&texparams);
      texparams.mipmap_bias = AiShaderEvalParamInt(p_mip_bias);
      texparams.filter = AiShaderEvalParamInt(p_filter);
      if ((sg->Rt & AI_RAY_DIFFUSE) && (texparams.filter > AI_TEXTURE_BILINEAR))
         texparams.filter = AI_TEXTURE_BILINEAR;
      bool success = true;
      bool useDefaultColor = AiShaderEvalParamBool(p_use_default_color);
      bool* successP = useDefaultColor ? &success : 0;
      if (idata->ntokens > 0)
      {
         TokenData* token = idata->tokens;
         unsigned int pos = 0;
         pos = idata->startPos;
         for (unsigned int i=0; (i < idata->ntokens) && success; i++, token++)
         {
            switch(token->mode)
            {
               // calculate the value of the map
               case SHAPE_NAME:
               {
                  // short shape name
                  AtNode* shape = sg->Op;
                  
                  const char* shapeName = AiNodeGetName(shape);
                  int lastSep = -1;
                  int it = 0;
                  for(char c = '0';((c = (shapeName[it])) != '\0') && (c != '@');++it)
                  {
                     if (c == '|')
                        lastSep = it;
                  }
                  if (lastSep == -1)
                  {
                     memcpy(&(idata->processPath[sg->tid][pos]),shapeName,it);
                     pos += (unsigned int) it;
                  }
                  else
                  {
                     lastSep += 1;
                     const unsigned int memcpySize = (unsigned int)(it - lastSep);
                     memcpy(&(idata->processPath[sg->tid][pos]), shapeName + lastSep, memcpySize);
                     pos += memcpySize;
                  }
                  // Copy next text chunk to the "processPath"
                  memcpy(&(idata->processPath[sg->tid][pos]),&(idata->origPath[token->position]),token->nextSize);
                  pos += token->nextSize;
                  // Set the end of the string
                  idata->processPath[sg->tid][pos] = 0;
                  break;
               }
               case SHAPE_PATH:
               {
                  // full path with underscores for illegal characters
                  AtNode* shape = sg->Op;
                  const char* shapeName = AiNodeGetName(shape);
                  char c;
                  while(((c = *(shapeName++)) != '\0') && (c != '@'))
                  {
                     if ((c == '|') || (c == ':'))
                        c = '_';
                     idata->processPath[sg->tid][pos++] = c;
                  }
                  // Copy next text chunk to the "processPath"
                  memcpy(&(idata->processPath[sg->tid][pos]),&(idata->origPath[token->position]),token->nextSize);
                  pos += token->nextSize;
                  // Set the end of the string
                  idata->processPath[sg->tid][pos] = 0;
                  break;
               }
               case USER_PARAM:
               {
                  // user attributes
                  const char* value;
                  if (AiUDataGetStr((const char*)token->extra, &value))
                  {
                     int len = (int) strlen(value);
                     memcpy(&(idata->processPath[sg->tid][pos]),value,len);
                     pos += (unsigned int) len;
                     // Copy next text chunk to the "processPath"
                     memcpy(&(idata->processPath[sg->tid][pos]),&(idata->origPath[token->position]),token->nextSize);
                     pos += token->nextSize;
                     // Set the end of the string
                     idata->processPath[sg->tid][pos] = 0;
                  }
                  else
                  {
                     // TODO: only warn once
                     // AiMsgWarning("could not find user attribute %s for token %s", attr.c_str(), sub.c_str());
                     idata->processPath[sg->tid][pos] = 0;
                     success = false;
                     const char* shapeName = AiNodeGetName(sg->shader);
                     AiMsgWarning("[MayaFile] Could not find user attribute %s for file node %s, setting to default color", (const char*)token->extra, shapeName);
                  }
                  break;
               }
               case UDIM:
               {
                  // default dimension
                  int* ptr = (int*)token->extra;
                  int dim = *ptr;
                  
                  int row = static_cast<int>(outV <= 0 ? floorf(outV) : ceilf(outV) - 1);
         	      int col = static_cast<int>(outU <= 0 ? floorf(outU) : ceilf(outU) - 1);
                  
                  int mariCode = ((row * dim) + col) + 1001;

                  // Update the UDIM mask in the "processPath"
                  idata->processPath[sg->tid][pos + token->position+3] = (mariCode%10) + '0';
                  mariCode /= 10;
                  idata->processPath[sg->tid][pos + token->position+2] = (mariCode%10) + '0';
                  mariCode /= 10;
                  idata->processPath[sg->tid][pos + token->position+1] = (mariCode%10) + '0';
                  mariCode /= 10;
                  idata->processPath[sg->tid][pos + token->position+0] = (mariCode%10) + '0';
                  
                  sg->u = fmod(outU, 1.f);
                  sg->v = fmod(outV, 1.f);
                  
                  break;
               }
               case TILE:
               {
                  // mudbox-style uv tiles: u1_v1
                  // Mudbox default format is to use _uU_vV in the file name where U = u + 1 and V = v + 1 are the tile coordinates plus one.
                  // UV tile [0,0] x (1,1) is marked with _u1_v1
                  // UV tile [0,1]x(1,2) is marked with _u1_v2 etc.

                  int col = static_cast<int>(floorf(inU)) + 1;
                  int row = static_cast<int>(floorf(inV)) + 1;
                  char buf[7];
                  sprintf(buf, "u%d_v%d", col, row);
                  int len = (int) strlen(buf);
                  memcpy(&(idata->processPath[sg->tid][pos]),buf,len);
                  pos += len;
                  // Copy next text chunk to the "processPath"
                  memcpy(&(idata->processPath[sg->tid][pos]),&(idata->origPath[token->position]),token->nextSize);
                  pos += token->nextSize;
                  // Set the end of the string
                  idata->processPath[sg->tid][pos] = 0;
                  break;
               }
            }// switch
         }

         if (success)
         {
            sg->out.RGBA = AiTextureAccess(sg, idata->processPath[sg->tid], &texparams, successP);
         }
         //AiMsgInfo("FILE: new name: %s", newfname.c_str());
      }
      else if (idata->texture_handle != NULL)
      {
         sg->out.RGBA = AiTextureHandleAccess(sg, idata->texture_handle, &texparams, successP);
      }
      else
      {       
         sg->out.RGBA = AiTextureAccess(sg, AiShaderEvalParamStr(p_filename), &texparams, successP);
      }
      if (useDefaultColor && !success)
         MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA);
      else if (success)
         MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA);

      // restore shader globals
      sg->u = inU;
      sg->v = inV;
      sg->dudx = inDuDx;
      sg->dudy = inDuDy;
      sg->dvdx = inDvDx;
      sg->dvdy = inDvDy;
   }
}
