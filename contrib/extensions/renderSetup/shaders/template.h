#ifndef TEMPLATE_H
#define TEMPLATE_H

#define IMPLEMENT_APPLY_ABS_OVERRIDE_SHADER(OUT, EvalParam)\
namespace\
{\
enum Params\
{\
   p_original,\
   p_value,\
   p_enabled,\
};\
}\
node_initialize{}\
node_update{}\
node_finish{}\
\
shader_evaluate\
{\
    sg->out.OUT = AiShaderEvalParamBool(p_enabled)?\
                EvalParam(p_value) :\
                EvalParam(p_original);\
}

#define IMPLEMENT_APPLY_REL_OVERRIDE_SHADER(OUT, EvalParam)\
namespace\
{\
enum Params\
{\
   p_original,\
   p_multiply,\
   p_offset,\
   p_enabled,\
};\
}\
node_initialize{}\
node_update{}\
node_finish{}\
\
shader_evaluate\
{\
    sg->out.OUT = AiShaderEvalParamBool(p_enabled)?\
                EvalParam(p_original) * EvalParam(p_multiply) + EvalParam(p_offset) :\
                EvalParam(p_original);\
}


#endif // TEMPLATE_H
