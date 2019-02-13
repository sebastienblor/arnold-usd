#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(MayaSamplerInfo2DMtd);

namespace
{

enum MaySamplerInfo2DParams
{
   p_mode
};

enum Mode
{
   UV_COORD = 0,
   PIXEL_CENTER
};

const char* mode_enum[] =
{
   "uvCoord",
   "pixelCenter",
   NULL
};

}


node_parameters
{
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
   AiParameterEnum("mode", 0, mode_enum);
}

node_initialize
{
}

node_update
{
}

node_finish
{
}

shader_evaluate
{
   switch (AiShaderEvalParamEnum(p_mode))
   {
   case UV_COORD:
      sg->out.VEC2().x = sg->u;
      sg->out.VEC2().y = sg->v;
      break;
   case PIXEL_CENTER:
      sg->out.VEC2().x = (float)sg->x;
      sg->out.VEC2().y = (float)sg->y;
      break;
   }
}
