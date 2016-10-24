#include <ai.h>
#include <cmath>

#ifndef SHADER_PREFIX
#define SHADER_PREFIX ""
#endif

AI_SHADER_NODE_EXPORT_METHODS(CurvatureMethods);

namespace {

inline float bias(float x, float inv_bias)
{
    return x / ((inv_bias - 2.0f) * (1.0f - x) + 1.0f);
}

static const char* enum_output[] =
{
    "convex",
    "concave",
    "both",
    NULL
};

enum Output
{
    OUTPUT_CONVEX,
    OUTPUT_CONCAVE,
    OUTPUT_BOTH,
};

enum Params
{
    p_output,
    p_samples,
    p_radius,
    p_spread,
    p_threshold,
    p_bias,
    p_multiply,
    p_trace_set,
    p_inclusive,
    p_self_only,
};

struct ShaderData
{
    Output output;
    AtUInt32 samples;
    AtSampler* sampler;
    AtString trace_set;
    bool inclusive;
    bool self_only;
};

} // namespace

node_parameters
{
    AiParameterEnum("output", OUTPUT_CONVEX, enum_output);
    AiParameterUInt("samples", 3);
    AiParameterFlt("radius", 0.1f);
    AiParameterFlt("spread", 1.0f);
    AiParameterFlt("threshold", 0.0f);
    AiParameterFlt("bias", 0.5f);
    AiParameterFlt("multiply", 1.0f);
    AiParameterStr("trace_set", "");
    AiParameterBool("inclusive", true);
    AiParameterBool("self_only", false);

    AiMetaDataSetBool(mds, "output", "linkable", false);
    AiMetaDataSetBool(mds, "samples", "linkable", false);
    AiMetaDataSetBool(mds, "trace_set", "linkable", false);
    AiMetaDataSetBool(mds, "inclusive", "linkable", false);
    AiMetaDataSetBool(mds, "self_only", "linkable", false);

}

node_initialize
{
    ShaderData* data = new ShaderData();
    data->sampler = NULL;
    AiNodeSetLocalData(node, data);
}

node_update
{
    ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));

    if (data->sampler) AiSamplerDestroy(data->sampler);
    data->sampler = AiSampler(AiNodeGetUInt(node, "samples"), 2);

    static const AtString output("output");
    data->output = static_cast<Output>(AiNodeGetInt(node, output));

    static const AtString samples("samples");
    data->samples = AiNodeGetUInt(node, samples);

    static const AtString trace_set("trace_set");
    data->trace_set = AiNodeGetStr(node, trace_set);

    static const AtString inclusive("inclusive");
    data->inclusive = AiNodeGetBool(node, inclusive);

    static const AtString self_only("self_only");
    data->self_only = AiNodeGetBool(node, self_only);
}

node_finish
{
    ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));
    AiSamplerDestroy(data->sampler);
    delete data;
}

shader_evaluate
{
    // early out for shadow rays and really transparent hits
    if ((sg->Rt & AI_RAY_SHADOW) || AiColorIsZero(sg->out_opacity))
       return;

    // local frame
    AtVector u, v;
    AiBuildLocalFrameShirley(&u, &v, &sg->Nf);

    // radius, bias, facing
    const float radius = MAX(AiShaderEvalParamFlt(p_radius), AI_EPSILON);
    const float radius_inv = 1.0f / radius;
    const float inv_bias = 1.0f / CLAMP(AiShaderEvalParamFlt(p_bias), AI_EPSILON, 1.0f - AI_EPSILON);
    const float front_facing = (float)SGN(AiV3Dot(sg->N, sg->Nf));
    const float dot_threshold = 2.0f * AiShaderEvalParamFlt(p_threshold) - 1.0f;

    // spread
    const float spread_sin = sinf((1.0f - AiShaderEvalParamFlt(p_spread)) * AI_PIOVER2);
    const float spread_threshold = 0.5f - spread_sin;
    const float spread_range = 2.0f * spread_threshold;
    const float spread_gap = 2.0f * spread_sin;

    // ray
    AtRay ray;
    AiMakeRay(&ray, AI_RAY_DIFFUSE, &sg->P, NULL, radius, sg);
    ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));
    ray.traceset = data->trace_set;
    ray.inclusive_traceset = data->inclusive;

    // probe over the sphere
    float sample[2];
    int hit_count = 0;
    float curvature = 0.0f;
    AtSamplerIterator* iterator = AiSamplerIterator(data->sampler, sg);

    while (AiSamplerGetSample(iterator, sample))
    {
        const float phi = sample[0] * AI_PITIMES2;
        const float sin_phi = sinf(phi);
        const float cos_phi = cosf(phi);

        // skip the spread zone
        float r = sample[1] * spread_range;
        if (r > spread_threshold) r += spread_gap;

        const float tmp = 2.0f * sqrtf(r * (1.0f - r));
        ray.dir.x = cos_phi * tmp;
        ray.dir.y = sin_phi * tmp;
        ray.dir.z = 1.0f - 2.0f * r;

        AiV3RotateToFrame(ray.dir, u, v, sg->Nf);

        AtShaderGlobals hit_sg;
        if (AiTraceProbe(&ray, &hit_sg))
        {
            if (data->self_only && hit_sg.Op != sg->Op) continue;

            const float dot_N = AiV3Dot(sg->N, hit_sg.N);

            // threshold, useful to prune "bubbles" behind the surface for convex curvature
            if (dot_N < dot_threshold) continue;

            const float angle = atan2f(AiV3Length(AiV3Cross(sg->N, hit_sg.N)), dot_N);
            const float sign = SGN(AiV3Dot(ray.dir, hit_sg.Ng)) * front_facing;
            const float w = bias(float(1.0f - hit_sg.Rl * radius_inv), (float)inv_bias);
            curvature += sign * angle * w;
            ++hit_count;
        }
    }

    // Even though the maximum curvature is theoretically PI, we are observing
    // a maximum curvature closer to PI/2 on mechanical models. This can be
    // adjusted with the multiply parameter.
    if (hit_count > 0)
        curvature /= float(hit_count) * AI_PIOVER2;

    curvature *= AiShaderEvalParamFlt(p_multiply);

    switch (data->output)
    {
    case OUTPUT_CONVEX:
        sg->out.RGB = curvature > 0.0f ? AiColor(curvature) : AI_RGB_BLACK;
        break;

    case OUTPUT_CONCAVE:
        sg->out.RGB = curvature < 0.0f ? AiColor(-curvature) : AI_RGB_BLACK;
        break;

    case OUTPUT_BOTH:
        if (curvature > 0.0f)
        {
            sg->out.RGB.r = curvature;
            sg->out.RGB.g = 0.0f;
        }
        else
        {
            sg->out.RGB.r = 0.0f;
            sg->out.RGB.g = -curvature;
        }

        sg->out.RGB.b = 0.0f;
        break;
    }
}
