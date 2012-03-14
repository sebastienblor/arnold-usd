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
   MAYA_COLOR_BALANCE_ENUM
};

};

struct TokenData
{
   TokenModes mode;
   unsigned int position;
   void* extra;
};

typedef struct AtImageData
{
   char* path;
   TokenData* tokens;
   unsigned int ntokens;
   AtTextureHandle* texture_handle;
} AtImageData;

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
      const std::string token = fname.substr(lastPos, pos - lastPos);
      std::string sub = fname.substr(lastPos, pos - lastPos);
      //AiMsgInfo( "(%d, %d) found token '%s'", lastPos, pos, sub.c_str());
      if (opened)
      {
         // a non-token file path part
         newfname += sub;
      }
      else
      {
         // calculate the value of the map
         if (sub == "<shapeName>")
         {
            TokenData data;
            data.mode = SHAPE_NAME;
            data.position = (unsigned int)lastPos;
            data.extra = NULL;
            tokens.push_back(data);
         }
         else if (sub == "<shapePath>")
         {
            TokenData data;
            data.mode = SHAPE_PATH;
            data.position = (unsigned int)lastPos;
            data.extra = NULL;
            tokens.push_back(data);
         }
         else if (sub.substr(0, 6) == "<attr:")
         {
            std::string attr = sub.substr(6, sub.length()-7);
            TokenData data;
            data.mode = USER_PARAM;
            data.position = (unsigned int)lastPos;
            data.extra = AiMalloc((unsigned long)attr.size());
            strcpy((char*)data.extra, attr.c_str());
            tokens.push_back(data);
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
            data.position = (unsigned int)lastPos;
            //data.extra = NULL;
            data.extra = AiMalloc(sizeof(int));
            *((int*)data.extra) = dim;
            tokens.push_back(data);
         }
         else if (sub == "<tile>" )
         {
            TokenData data;
            data.mode = TILE;
            data.position = (unsigned int)lastPos;
            data.extra = NULL;
            tokens.push_back(data);
         }
         else
         {
            AiMsgError("unknown token %s", sub.c_str());
            newfname += sub;
         }
      }
   }

   AtImageData *idata = (AtImageData*) AiMalloc(sizeof(AtImageData));
   idata->ntokens = (unsigned int)tokens.size();
   if (tokens.size())
   {
      idata->tokens = (TokenData*) AiMalloc((unsigned long) (sizeof(TokenData) * tokens.size()));
      std::copy(tokens.begin(), tokens.end(), idata->tokens);

      idata->path = (char*) AiMalloc(newfname.size() + 1);
      strcpy(idata->path, newfname.c_str());
      idata->texture_handle = NULL;
   }
   else
      idata->texture_handle = AiTextureHandleCreate(AiNodeGetStr(node, "filename"));
   AiNodeSetLocalData(node, idata);
}

node_finish
{
   AtImageData *idata = (AtImageData*) AiNodeGetLocalData(node);
   if (idata != NULL)
   {
      // Freeing ShaderData
      if (idata->ntokens)
      {
         TokenData* token = idata->tokens;
         for (unsigned int i=0; i < idata->ntokens; i++, token++)
         {
            if (token->extra != NULL)
               AiFree(token->extra);
         }
         AiFree(idata->tokens);
         AiFree(idata->path);
      }
      else
         AiTextureHandleDestroy(idata->texture_handle);
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
      AtPoint uv;
      uv.x = inU * 16;
      uv.y = inV * 16;
      uv.z = 0.0f;
      outU += noise.x * AiPerlin3(uv);
   }

   if (noise.y > 0.0f)
   {
      AtPoint uv;
      uv.x = (1 - inU) * 16;
      uv.y = (1 - inV) * 16;
      uv.z = 0.0f;
      outV += noise.y * AiPerlin3(uv);
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
      // setup filter?
      bool success = true;
      if (idata->ntokens > 0)
      {
         std::string newfname = "";
         newfname.reserve(MAX_FILENAME);
         std::string fname = idata->path;
         //AiMsgInfo("FILE: # tokens: %d", idata->ntokens);
         //AiMsgInfo("FILE: filename: %s", idata->path);
         TokenData* token = idata->tokens;
         unsigned int pos = 0;
         for (unsigned int i=0; i < idata->ntokens; i++, token++)
         {
            newfname += fname.substr(pos, token->position - pos);
            pos = token->position;
            //AiMsgInfo("FILE: %d", (int)token->mode);
            switch(token->mode)
            {
               // calculate the value of the map
               case SHAPE_NAME:
               {
                  // short shape name
                  AtNode* shape = sg->Op;
                  std::string shapeName = AiNodeGetName(shape);
                  size_t sep = shapeName.rfind('|');
                  if (sep != std::string::npos)
                     newfname += shapeName.substr(sep + 1);
                  else
                     newfname += shapeName;
                  break;
               }
               case SHAPE_PATH:
               {
                  // full path with underscores for illegal characters
                  AtNode* shape = sg->Op;
                  std::string shapeName = AiNodeGetName(shape);
                  std::string::size_type found=shapeName.find_first_of("|:");
                  while (found != std::string::npos)
                  {
                     shapeName[found]='_';
                     found=shapeName.find("|:", found + 1);
                  }
                  newfname += shapeName;
                  break;
               }
               case USER_PARAM:
               {
                  // user attributes
                  const char* value;
                  if (AiUDataGetStr((const char*)token->extra, &value))
                  {
                     newfname += value;
                  }
                  else
                  {
                     // TODO: only warn once
                     // AiMsgWarning("could not find user attribute %s for token %s", attr.c_str(), sub.c_str());
                     success = false;
                  }
                  break;
               }
               case UDIM:
               {
                  // default dimension
                  int* ptr = (int*)token->extra;
                  int dim = *ptr;
                  int row = static_cast<int>(floorf(inV));
                  int col = static_cast<int>(floorf(inU));
                  if ((col+1) > dim)
                     AiMsgWarning("Invalid udim: U value %.04f is greater than maximum U dimension %d", inU, dim);
                  else if (row < 0)
                     AiMsgWarning("Invalid udim: U value %.04f is less than 0", inU);
                  if (col < 0)
                     AiMsgWarning("Invalid udim: V value %.04f is less than 0", inV);
                  char buf[4];
                  sprintf(buf, "%04d", ((row * dim) + col) + 1001);
                  newfname += buf;
                  break;
               }
               case TILE:
               {
                  /*
                  mudbox-style uv tiles: u1_v1
                  Mudbox default format is to use _uU_vV in the file name where U = u + 1 and V = v + 1 are the tile coordinates plus one.
                  UV tile [0,0] x (1,1) is marked with _u1_v1
                  UV tile [0,1]x(1,2) is marked with _u1_v2 etc.
                  */
                  int row = static_cast<int>(floorf(inV)) + 1;
                  int col = static_cast<int>(floorf(inU)) + 1;
                  char buf[7];
                  sprintf(buf, "%d_%d", row, col);
                  newfname += buf;
                  break;
               }
            }// switch
         }

         if (success)
         {
            newfname += fname.substr(pos, fname.length() - pos);
            sg->out.RGBA = AiTextureAccess(sg, newfname.c_str(), &texparams, &success);
         }
         //AiMsgInfo("FILE: new name: %s", newfname.c_str());
      }
      else
      {
         sg->out.RGBA = AiTextureHandleAccess(sg, idata->texture_handle, &texparams, &success);
      }
      if (success)
         MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA);
      else
         MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA);

      // restore shader globals
      sg->u = inU;
      sg->v = inV;
      sg->dudx = inDuDx;
      sg->dudy = inDuDy;
      sg->dvdx = inDvDx;
      sg->dvdy = inDvDy;
   }
}
