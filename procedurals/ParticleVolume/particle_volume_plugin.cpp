#include <cstring>
#include <algorithm>
#include <vector>
#include <map>

#include <ai.h>

#include "mini_bvh.h"


namespace
{

//
// Plugin data and computations
//

struct DataChannel
{
    AtParamValue value;
    int type;
    AtArray *data;
    AtString name;

    DataChannel(AtString n) : data(NULL), name(n) { }
};


struct AtStringComparator
{
    bool operator() (const AtString& s1, const AtString& s2) const
    {
        return s1.c_str() < s2.c_str();
    }
};


typedef std::map<AtString, DataChannel, AtStringComparator> DataChannelMap;


struct ParticleVolumePluginData
{
    BVH *bvh;
    AtArray *points;
    AtArray *radii;
    uint32_t points_nelements, radii_nelements;
    uint8_t  points_nkeys, radii_nkeys;
    AtString particle_volume_plugin_name, implicit_node_name;
    AtString density_channel, field_channel; // special channels automatically provided by the plugin
    float falloff_exponent;                  // falloff control (when not using smoothstep)
    bool implicit_mode;                      // if yes, we produce blobbies, otherwise particle densities
    bool smooth_step;                        // use smoothstep, and ignore falloff controls
    float gradient_eps, gradient_inv_eps;    // precomputed quantities for faster gradient calculations

    // Arbitrary constant or per-particle data (can be sampled via AiVolumeSample*Func() API functions)
    DataChannelMap data_channels;

    // Per-thread data, cached so that allocations are very infrequent
    // TODO: This data only needs to exist once for the entire plugin
    std::vector<IntervalList> per_thread_intervals;
    std::vector< std::vector<unsigned int> > per_thread_prims;

    ParticleVolumePluginData()
        : bvh(NULL),
          points(NULL),
          radii(NULL),
          points_nelements(0),
          radii_nelements(0),
          points_nkeys(0),
          radii_nkeys(0),
          particle_volume_plugin_name("particle volume plugin"),
          implicit_node_name("implicit_particle"),
          density_channel(),
          field_channel(),
          falloff_exponent(1.0f),
          implicit_mode(false),
          smooth_step(true),
          gradient_eps(1.0e-5f),
          gradient_inv_eps(1.0f / 1.0e-5f),
          data_channels(),
          per_thread_intervals(),
          per_thread_prims()
    {
        // Assume there could be up to a generous number of threads
        per_thread_intervals.resize(256);
        for (size_t i = 0; i < per_thread_intervals.size(); ++i)
        {
            // Reserve up to 128 particles hit along a ray (will resize infrequently per-thread as needed)
            per_thread_intervals[i].reserve(128);
        }
        per_thread_prims.resize(256);
        for (size_t i = 0; i < per_thread_prims.size(); ++i)
        {
            // Reserve up to 128 particles hit along a ray (will resize infrequently per-thread as needed)
            per_thread_prims[i].reserve(128);
        }
    }

    ~ParticleVolumePluginData() { delete bvh; }

    // Here for reference, this is how we compute motion-blurred particle positions
    AtVector position(unsigned int prim, float time) const
    {
        if (points_nkeys == 1 || time <= 0.0f)
            return AiArrayGetVec(points, prim);
        if (time >= 1.0f)
            return AiArrayGetVec(points, prim + points_nelements * (points_nkeys - 1));
        float fkey = time * (points_nkeys - 1);
        unsigned int key = (unsigned int)fkey;
        float t = fkey - key;
        AtVector p0 = AiArrayGetVec(points, prim + key * points_nelements);
        AtVector p1 = AiArrayGetVec(points, prim + (key + 1) * points_nelements);
        return p0 * (1.0f - t) + p1 * t;
    }

    // Here for reference, this is how we compute motion-blurred particle radii (constant or per-particle)
    float radius(unsigned int prim, float time) const
    {
        if (radii_nelements == points_nelements)
        {
            if (radii_nkeys == 1 || time <= 0.0f)
                return AiArrayGetFlt(radii, prim);
            if (time >= 1.0f)
                return AiArrayGetFlt(radii, prim + radii_nelements * (radii_nkeys - 1));
            float fkey = time * (radii_nkeys - 1);
            unsigned int key = (unsigned int)fkey;
            float t = fkey - key;
            float r0 = AiArrayGetFlt(radii, prim + key * radii_nelements);
            float r1 = AiArrayGetFlt(radii, prim + (key + 1) * radii_nelements);
            return r0 * (1.0f - t) + r1 * t;
        }
        else
        {
            if (radii_nkeys == 1 || time <= 0.0f)
                return AiArrayGetFlt(radii, 0);
            if (time >= 1.0f)
                return AiArrayGetFlt(radii, (radii_nkeys - 1) * radii_nelements);
            float fkey = time * (radii_nkeys - 1);
            unsigned int key = (unsigned int)fkey;
            float t = fkey - key;
            float r0 = AiArrayGetFlt(radii, key * radii_nelements);
            float r1 = AiArrayGetFlt(radii, (key + 1) * radii_nelements);
            return r0 * (1.0f - t) + r1 * t;
        }
    }

    // If multiple particles are going to be queried at the same time, use the
    // 'fast' versions to save time calculating motion-blurred positions and radii

    void precomputeFastRadiusTerms(float time,
                                   unsigned int& out_key,
                                   float& out_t) const
    {
        float fkey = std::max(0.0f, std::min((float)(radii_nkeys - 1), time * (radii_nkeys - 1)));
        out_key = (unsigned int)fkey;
        out_t = fkey - out_key;
    }

    unsigned int fastRadiusPrim(unsigned int prim) const
    {
        return prim >= radii_nelements ? 0 : prim;
    }

    float fastRadius(unsigned int prim, unsigned int key, float t) const
    {
        float r0 = AiArrayGetFlt(radii, prim + key * radii_nelements);
        float r1 = ((int)key >= radii_nkeys - 1) ? r0 : AiArrayGetFlt(radii, prim + (key + 1) * radii_nelements);
        return r0 * (1.0f - t) + r1 * t;
    }

    void precomputeFastPositionTerms(float time,
                                     unsigned int& out_key,
                                     float& out_t) const
    {
        float fkey = std::max(0.0f, std::min((float)(points_nkeys - 1), time * (points_nkeys - 1)));
        out_key = (unsigned int)fkey;
        out_t = fkey - out_key;
    }

    AtVector fastPosition(unsigned int prim, unsigned int key, float t) const
    {
        AtVector p0 = AiArrayGetVec(points, prim + key * points_nelements);
        AtVector p1 = ((int)key >= points_nkeys - 1) ? p0 : AiArrayGetVec(points, prim + (key + 1) * points_nelements);
        return p0 * (1.0f - t) + p1 * t;
    }

    // Per-thread vectors, so that they allocate very infrequently and don't need thread locking

    IntervalList& perThreadIntervalList(unsigned int tid)
    {
        if (tid >= per_thread_intervals.size())
        {
            AiMsgError("[volume_particle] catastrophic error: thread ID too large!");
            AiRenderAbort();
            return per_thread_intervals.back();
        }
        return per_thread_intervals[tid];
    }

    std::vector<unsigned int>& perThreadPrims(unsigned int tid)
    {
        if (tid >= per_thread_prims.size())
        {
            AiMsgError("[volume_particle] catastrophic error: thread ID too large!");
            AiRenderAbort();
            return per_thread_prims.back();
        }
        return per_thread_prims[tid];
    }
};


// Helper to compute ray/sphere and point/sphere intersections
struct ParticleIntersector : public PrimitiveIntersectorBase
{
    ParticleIntersector(ParticleVolumePluginData& data, float time)
        : m_data(data)
    {
        // All queries will use the same time, so precompute a few bits so we
        // can use the fast radius and position calculations
        m_data.precomputeFastPositionTerms(time, m_pkey, m_pt);
        m_data.precomputeFastRadiusTerms(time, m_rkey, m_rt);
    }

    virtual ~ParticleIntersector() { }

    virtual bool intersect(unsigned int prim, const AtVector& origin, const AtVector& direction, const AtVector& invDirection, float& t_start, float& t_end)
    {
        AtVector adjusted_origin = origin - m_data.fastPosition(prim, m_pkey, m_pt);
        float radius = m_data.fastRadius(m_data.fastRadiusPrim(prim), m_rkey, m_rt);
        if (m_data.implicit_mode)
            radius *= 2.0f;

        // Calculate quadratic coeffs
        float a = AiV3Dot(direction, direction);
        float b = 2.0f * AiV3Dot(direction, adjusted_origin);
        float c = AiV3Dot(adjusted_origin, adjusted_origin) - radius * radius;

        float t0, t1, discriminant;
        discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f)
            return false;
        discriminant = std::sqrt(discriminant);

        // Compute a more stable form of our param t (t0 = q/a, t1 = c/q)
        // q = -0.5 * (b - sqrt(b * b - 4.0 * a * c)) if b < 0, or
        // q = -0.5 * (b + sqrt(b * b - 4.0 * a * c)) if b >= 0
        float q;
        if (b < 0.0f)
            q = -0.5f * (b - discriminant);
        else
            q = -0.5f * (b + discriminant);

        // Get our final parametric values
        t0 = q / a;
        if (q != 0.0f)
            t1 = c / q;
        else
            t1 = t_start;

        // Swap them so they are ordered right
        if (t0 > t1)
        {
            float temp = t1;
            t1 = t0;
            t0 = temp;
        }

        if (t0 > t_start)
            t_start = t0;
        if (t1 < t_end)
            t_end = t1;
        return t1 > t0;
    }

    virtual bool intersect(unsigned int prim, const AtVector& p)
    {
        AtVector d = p - m_data.fastPosition(prim, m_pkey, m_pt);
        float radius = m_data.fastRadius(m_data.fastRadiusPrim(prim), m_rkey, m_rt);
        if (m_data.implicit_mode)
            radius *= 2.0f;
        return AiV3Dot(d, d) <= radius * radius;
    }

    ParticleVolumePluginData& m_data;
    unsigned int m_pkey, m_rkey;
    float m_pt, m_rt;
};


// No 0..1 clamp (for speed)
inline float unitSmoothstep(float t)
{
    return t * t * (3.0f - 2.0f * t);
}

// Arnold-friendly field value for implicits (in this case, blobbies)
float fieldValue(ParticleVolumePluginData *pluginData,
                 const AtVector& p,
                 float time,
                 ParticleIntersector& intersector,
                 std::vector<unsigned int>& prims)
{
    prims.clear();
    if (!pluginData->bvh->findOverlappingPrims(p, prims, intersector))
        return 0.5f;

    unsigned int pkey = intersector.m_pkey, rkey = intersector.m_rkey;
    float pt = intersector.m_pt, rt = intersector.m_rt;

    // Note: field values can ultimately be whatever we want them to be, but we
    // use the ranges here for convenience and numeric stability in Arnold's solver

    float meta_f = 0.0f;
    if (pluginData->smooth_step)
    {
        // Field values are 1-smoothstep(d/(2*r)), remapped to -0.5 on the innermost and 0.5 on the outermost extent
        for (size_t i = 0; i < prims.size(); ++i)
        {
            float r = pluginData->fastRadius(pluginData->fastRadiusPrim(prims[i]), rkey, rt);
            float t = AiV3Dist(p, pluginData->fastPosition(prims[i], pkey, pt)) / (2.0f * r);
            float f = 1.0f - unitSmoothstep(std::min(t, 1.0f));
            meta_f += f;
        }
    }
    else
    {
        // Field values are (1-d/(2*r))^falloff_exponent, remapped to -0.5 on the innermost and 0.5 on the outermost extent
        for (size_t i = 0; i < prims.size(); ++i)
        {
            float r = pluginData->fastRadius(pluginData->fastRadiusPrim(prims[i]), rkey, rt);
            float t = AiV3Dist(p, pluginData->fastPosition(prims[i], pkey, pt)) / (2.0f * r);
            float f = std::pow(1.0f - std::min(t, 0.99999f), pluginData->falloff_exponent);
            meta_f += f;
        }
    }
    return 0.5f - meta_f;
}


//
// Plugin methods
//


AI_VOLUME_NODE_EXPORT_METHODS(VolumeParticleMtd);



node_parameters
{
    AiParameterArray("points",           AiArrayAllocate(0, 0, AI_TYPE_VECTOR));
    AiParameterArray("radius",           AiArrayAllocate(0, 0, AI_TYPE_FLOAT));
    AiParameterBool ("smooth_step",      true);
    AiParameterFlt  ("falloff_exponent", 0.0f);
    AiParameterFlt  ("step_scale"      , 1.0f);
}


node_plugin_initialize
{
   return true;
}

node_plugin_cleanup
{

}

volume_update
{
    /*
     * Parameters:
     *
     * declare points constant ARRAY POINT (includes moblur if present)
     * declare radius constant ARRAY FLOAT  (includes moblur if present; can be size 1 (constant) or same number of entries as points)
     * declare radii  constant ARRAY FLOAT (synonym for radius)
     * declare smooth_step constant BOOL
     * declare falloff_exponent constant FLOAT (useful range 0.0 to 4.0)
     * declare step_scale constant FLOAT
     *
     * Any additional user data w/ same number of entries as points can be sampled
     *
     * It also respects implicit.field_channel
     */

    ParticleVolumePluginData *pluginData;
    bool regenerate = false;
    if (!data->private_info)
    {
        regenerate = true;
        pluginData = new ParticleVolumePluginData();
    }
    else
        pluginData = reinterpret_cast<ParticleVolumePluginData*>(data->private_info);

    // Note: points and radii can have motion blur, keys will be equally distributed
    // between 0.0 and 1.0.  Radii can also be constant (one value for all points).
    AtArray *pointsArray = AiNodeGetArray(node, "points");
    AtArray *radiiArray  = AiNodeGetArray(node, "radius");

    // If smoothstep is on, the exponent is ignored.  Otherwise:
    //   * For volumes use (1-d/r)^falloff_exponent where d is distance from the particle and r is radius
    //   * For implicits us (1-d/(2*r))^falloff_exponent instead
    bool  smoothStep      = AiNodeGetBool(node, "smooth_step");
    float falloffExponent = AiNodeGetFlt(node, "falloff_exponent");

    // This modifies the calculated step size
    float stepScale = AiNodeGetFlt(node, "step_scale");

    // Produce blobbies when used from an implicit node (otherwise spherical volume particles)
    pluginData->implicit_mode = AiNodeIs(node, pluginData->implicit_node_name);

    // This pads the bounds (for example to accomodate a shader that adds positional noise to a volume)
    float boundsSlack = pluginData->implicit_mode ? 0.0f : AiNodeGetFlt(node, "bounds_slack");

    // Add in user-declared data channels so they can be sampled by shaders
    AtUserParamIterator *piter = AiNodeGetUserParamIterator(node);
    pluginData->data_channels.clear();
    while (!AiUserParamIteratorFinished(piter))
    {
        const AtUserParamEntry *pentry = AiUserParamIteratorGetNext(piter);
        const char *pname = AiUserParamGetName(pentry);
        // Skip known params
        if (!strcmp(pname, "points") ||
            !strcmp(pname, "radii") ||
            !strcmp(pname, "radius") ||
            !strcmp(pname, "falloff_exponent") ||
            !strcmp(pname, "step_scale") ||
            !strcmp(pname, "smooth_step"))
        {
            continue;
        }
        // Skip non-floats
        int type = AiUserParamGetType(pentry);
        if (type == AI_TYPE_BYTE || type == AI_TYPE_INT || type == AI_TYPE_UINT ||
            type == AI_TYPE_BOOLEAN || type == AI_TYPE_STRING || type == AI_TYPE_POINTER ||
            type == AI_TYPE_NODE || type == AI_TYPE_MATRIX || type == AI_TYPE_ENUM)
        {
            continue;
        }
        AtString name(pname);
        if (type == AI_TYPE_ARRAY)
        {
            type = AiUserParamGetArrayType(pentry);
            // Skip non-float arrays
            if (type == AI_TYPE_BYTE || type == AI_TYPE_INT || type == AI_TYPE_UINT ||
                type == AI_TYPE_BOOLEAN || type == AI_TYPE_STRING || type == AI_TYPE_POINTER ||
                type == AI_TYPE_NODE || type == AI_TYPE_MATRIX || type == AI_TYPE_ENUM ||
                type == AI_TYPE_ARRAY)
            {
                continue;
            }
            // Skip arrays not of the right size
            AtArray *parray = AiNodeGetArray(node, name);
            if (AiArrayGetNumElements(parray) != AiArrayGetNumElements(pointsArray))
                continue;
            // We have a valid data channel the user can sample
            DataChannelMap::iterator iter = pluginData->data_channels.insert(std::pair<AtString, DataChannel>(name, DataChannel(name))).first;
            iter->second.data = parray;
            iter->second.type = type;
            AiMsgDebug("[volume_particle] %s: added varying data channel: %s",
                       AiNodeGetName(node), pname);
        }
        else
        {
            // We have a valid (constant) channel the user can sample
            DataChannelMap::iterator iter = pluginData->data_channels.insert(std::pair<AtString, DataChannel>(name, DataChannel(name))).first;
            iter->second.data = NULL;
            iter->second.type = type;
            iter->second.value.RGBA() = AI_RGBA_ZERO;
            if (type == AI_TYPE_FLOAT)
                iter->second.value.FLT() = AiNodeGetFlt(node, name);
            else if (type == AI_TYPE_RGB)
                iter->second.value.RGB() = AiNodeGetRGB(node, name);
            else if (type == AI_TYPE_RGBA)
                iter->second.value.RGBA() = AiNodeGetRGBA(node, name);
            else if (type == AI_TYPE_VECTOR)
                iter->second.value.VEC() = AiNodeGetVec(node, name);
            else if (type == AI_TYPE_VECTOR2)
                iter->second.value.VEC2() = AiNodeGetVec2(node, name);
            AiMsgDebug("[volume_particle] %s: added constant data channel: %s",
                       AiNodeGetName(node), pname);
        }
    }
    AiUserParamIteratorDestroy(piter);

    // Calculate overall bounds with motion blur, and individual particle bounds for the BVH along the way
    float radius_average = 0.0f;
    uint32_t pn = AiArrayGetNumElements(pointsArray);
    uint32_t rn = AiArrayGetNumElements(radiiArray);
    uint8_t pk = AiArrayGetNumKeys(pointsArray);
    uint8_t rk = AiArrayGetNumKeys(radiiArray);
    std::vector<AtBBox> prim_bboxes;
    prim_bboxes.resize(pn);
    AtBBox bbox;
    bbox.init(AI_INFINITE);
    for (unsigned int i = 0; i < pn; ++i)
    {
        float radius = (rn == pn) ? AiArrayGetFlt(radiiArray, i) : AiArrayGetFlt(radiiArray, 0);
        if (pluginData->implicit_mode)
            radius *= 2.0f;
        radius_average += radius;
        AtVector p = AiArrayGetVec(pointsArray, i);
        AtBBox pbbox(p - AI_V3_ONE * radius, p + AI_V3_ONE * radius);
        for (unsigned int key = 1; key < pk; ++key)
        {
            float key_radius = radius;
            if (rk == pk)
            {
                key_radius = (rn == pn) ? AiArrayGetFlt(radiiArray, i + key * rn) : AiArrayGetFlt(radiiArray, key);
                if (pluginData->implicit_mode)
                    key_radius *= 2.0f;
                radius_average += key_radius;
            }
            p = AiArrayGetVec(pointsArray, i + key * pn);
            AtBBox pbbox_key(p - AI_V3_ONE * key_radius, p + AI_V3_ONE * key_radius);
            pbbox = AiBBoxUnion(pbbox, pbbox_key);
        }
        prim_bboxes[i] = pbbox;
        bbox = AiBBoxUnion(bbox, pbbox);
    }
    radius_average /= (float)pn;
    if (rk > 1)
        radius_average /= (float)rk;

    bbox.min -= AI_V3_ONE * boundsSlack;
    bbox.max += AI_V3_ONE * boundsSlack;

    // Base step size on the average radius
    float calculated_step_size = radius_average;
    if (pluginData->implicit_mode)
        calculated_step_size *= 0.0001f; // Capture the sphere edges faithfully in implicit mode

    // Precalculate some constants for gradient computations
    pluginData->gradient_eps = std::max(1.0e-5f, calculated_step_size * stepScale);
    pluginData->gradient_inv_eps = 1.0f / pluginData->gradient_eps;

    // Finalize particles

    AiMsgDebug("[volume_particle] %s: %d particles", AiNodeGetName(node), pn);

    if (pluginData->points != pointsArray || pluginData->radii != radiiArray)
    {
        regenerate = true;
    }

    if (!regenerate)
    {
       // No changes nor is it the first time through
       return true;
    }

    pluginData->points = pointsArray;
    pluginData->points_nelements = AiArrayGetNumElements(pointsArray);
    pluginData->points_nkeys = AiArrayGetNumKeys(pointsArray);

    pluginData->radii = radiiArray;
    pluginData->radii_nelements = AiArrayGetNumElements(radiiArray);
    pluginData->radii_nkeys = AiArrayGetNumKeys(radiiArray);

    pluginData->falloff_exponent = falloffExponent;
    pluginData->smooth_step = smoothStep;
    pluginData->density_channel = AtString("density");
    if (pluginData->implicit_mode)
        pluginData->field_channel = AtString(AiNodeGetStr(node, "field_channel"));
    else
        pluginData->field_channel = AtString("field");

    // (Re)generate BVH
    if (pluginData->bvh)
    {
       AiAddMemUsage(-pluginData->bvh->memUsage(), pluginData->particle_volume_plugin_name);
       delete pluginData->bvh;
    }

    pluginData->bvh = new BVH(prim_bboxes);
    AiAddMemUsage(pluginData->bvh->memUsage(), pluginData->particle_volume_plugin_name);

    // We're ready to roll
    data->bbox = bbox;
    data->auto_step_size = calculated_step_size * stepScale;
    data->private_info = pluginData;

    return true;
}

volume_create
{
    return VolumeUpdate(node, data);
}

volume_cleanup
{
    if (!data->private_info) return false;

    ParticleVolumePluginData *pluginData = reinterpret_cast<ParticleVolumePluginData*>(data->private_info);
    AiAddMemUsage(-pluginData->bvh->memUsage(), pluginData->particle_volume_plugin_name);
    delete pluginData;
    data->private_info = NULL;
    return true;
}

volume_sample
{
    ParticleVolumePluginData *pluginData = reinterpret_cast<ParticleVolumePluginData*>(data->private_info);
    if (!pluginData)
        return false;

    // All sampling modes need to query for particles overlapping the shading location
    ParticleIntersector intersector(*pluginData, sg->time);
    std::vector<unsigned int>& prims = pluginData->perThreadPrims(sg->tid);

    if (channel == pluginData->density_channel)
    {
        // Density channel gets falloff treatment (unlike other volume data channels)
        prims.clear();
        pluginData->bvh->findOverlappingPrims(sg->Po, prims, intersector);
        if (pluginData->falloff_exponent > 0.0f)
        {
            float density = 0.0f;
            if (pluginData->smooth_step)
            {
                // Use smoothstep weighting
                for (size_t i = 0; i < prims.size(); ++i)
                {
                    AtVector d = sg->Po - pluginData->position(prims[i], sg->time);
                    float nd = AiV3Length(d) / pluginData->radius(prims[i], sg->time);
                    density += 1.0f - unitSmoothstep(std::min(1.0f, nd));
                }
            }
            else
            {
                // We use (1-d/r)^falloff_exponent weighting
                for (size_t i = 0; i < prims.size(); ++i)
                {
                    AtVector d = sg->Po - pluginData->position(prims[i], sg->time);
                    float nd = AiV3Length(d) / pluginData->radius(prims[i], sg->time);
                    density += std::pow(1.0f - std::min(0.99999f, nd), pluginData->falloff_exponent);
                }
            }
            value->RGB() = AI_RGB_WHITE * density;
        }
        else
        {
            value->RGB() = AI_RGB_WHITE * (float)prims.size();
        }
        *type = AI_TYPE_RGB;
        return !prims.empty();
    }
    else if (channel == pluginData->field_channel)
    {
        // Field channel uses blobby weighting (either smoothstep or (1-d/(2*r))^falloff_exponent)
        value->FLT() = fieldValue(pluginData, sg->Po, sg->time, intersector, prims);
        *type = AI_TYPE_FLOAT;
        return true;
    }

    DataChannelMap::iterator iter = pluginData->data_channels.find(channel);
    if (iter != pluginData->data_channels.end())
    {
        *type = iter->second.type;
        if (iter->second.data != NULL)
        {
            prims.clear();
            pluginData->bvh->findOverlappingPrims(sg->Po, prims, intersector);
            value->RGBA() = AI_RGBA_ZERO; // RGBA is big enough to hold all float types
            if (pluginData->implicit_mode)
            {
                unsigned int pkey = intersector.m_pkey, rkey = intersector.m_rkey;
                float pt = intersector.m_pt, rt = intersector.m_rt;

                // Blend channel values using blobby weights
                float weight_total = 0.0f;
                if (pluginData->smooth_step)
                {
                    // Use smoothstep falloff, ignore falloff exponent
                    if (iter->second.type == AI_TYPE_FLOAT)
                    {
                        for (size_t i = 0; i < prims.size(); ++i)
                        {
                            float t = AiV3Dist(sg->Po, pluginData->fastPosition(prims[i], pkey, pt)) /
                                      (2.0f * pluginData->fastRadius(pluginData->fastRadiusPrim(prims[i]), rkey, rt));
                            float w = 1.0f - unitSmoothstep(std::min(t, 1.0f));
                            value->FLT() += AiArrayGetFlt(iter->second.data, prims[i]) * w;
                            weight_total += w;
                        }
                    }
                    else if (iter->second.type == AI_TYPE_RGB)
                    {
                        for (size_t i = 0; i < prims.size(); ++i)
                        {
                            float t = AiV3Dist(sg->Po, pluginData->fastPosition(prims[i], pkey, pt)) /
                                      (2.0f * pluginData->fastRadius(pluginData->fastRadiusPrim(prims[i]), rkey, rt));
                            float w = 1.0f - unitSmoothstep(std::min(t, 1.0f));
                            value->RGB() += AiArrayGetRGB(iter->second.data, prims[i]) * w;
                            weight_total += w;
                        }
                    }
                    else if (iter->second.type == AI_TYPE_RGBA)
                    {
                        for (size_t i = 0; i < prims.size(); ++i)
                        {
                            float t = AiV3Dist(sg->Po, pluginData->fastPosition(prims[i], pkey, pt)) /
                                      (2.0f * pluginData->fastRadius(pluginData->fastRadiusPrim(prims[i]), rkey, rt));
                            float w = 1.0f - unitSmoothstep(std::min(t, 1.0f));
                            value->RGBA() += AiArrayGetRGBA(iter->second.data, prims[i]) * w;
                            weight_total += w;
                        }
                    }
                    else if (iter->second.type == AI_TYPE_VECTOR)
                    {
                        for (size_t i = 0; i < prims.size(); ++i)
                        {
                            float t = AiV3Dist(sg->Po, pluginData->fastPosition(prims[i], pkey, pt)) /
                                      (2.0f * pluginData->fastRadius(pluginData->fastRadiusPrim(prims[i]), rkey, rt));
                            float w = 1.0f - unitSmoothstep(std::min(t, 1.0f));
                            value->VEC() += AiArrayGetVec(iter->second.data, prims[i]) * w;
                            weight_total += w;
                        }
                    }
                    else if (iter->second.type == AI_TYPE_VECTOR2)
                    {
                        for (size_t i = 0; i < prims.size(); ++i)
                        {
                            float t = AiV3Dist(sg->Po, pluginData->fastPosition(prims[i], pkey, pt)) /
                                      (2.0f * pluginData->fastRadius(pluginData->fastRadiusPrim(prims[i]), rkey, rt));
                            float w = 1.0f - unitSmoothstep(std::min(t, 1.0f));
                            value->VEC2() += AiArrayGetVec2(iter->second.data, prims[i]) * w;
                            weight_total += w;
                        }
                    }
                }
                else
                {
                    // Use (1-d/(2*r))^falloff_exponent blobby weighting
                    if (iter->second.type == AI_TYPE_FLOAT)
                    {
                        for (size_t i = 0; i < prims.size(); ++i)
                        {
                            float t = AiV3Dist(sg->Po, pluginData->fastPosition(prims[i], pkey, pt)) /
                                      (2.0f * pluginData->fastRadius(pluginData->fastRadiusPrim(prims[i]), rkey, rt));
                            float w = std::pow(1.0f - std::min(t, 0.99999f), pluginData->falloff_exponent);
                            value->FLT() += AiArrayGetFlt(iter->second.data, prims[i]) * w;
                            weight_total += w;
                        }
                    }
                    else if (iter->second.type == AI_TYPE_RGB)
                    {
                        for (size_t i = 0; i < prims.size(); ++i)
                        {
                            float t = AiV3Dist(sg->Po, pluginData->fastPosition(prims[i], pkey, pt)) /
                                      (2.0f * pluginData->fastRadius(pluginData->fastRadiusPrim(prims[i]), rkey, rt));
                            float w = std::pow(1.0f - std::min(t, 0.99999f), pluginData->falloff_exponent);
                            value->RGB() += AiArrayGetRGB(iter->second.data, prims[i]) * w;
                            weight_total += w;
                        }
                    }
                    else if (iter->second.type == AI_TYPE_RGBA)
                    {
                        for (size_t i = 0; i < prims.size(); ++i)
                        {
                            float t = AiV3Dist(sg->Po, pluginData->fastPosition(prims[i], pkey, pt)) /
                                      (2.0f * pluginData->fastRadius(pluginData->fastRadiusPrim(prims[i]), rkey, rt));
                            float w = std::pow(1.0f - std::min(t, 0.99999f), pluginData->falloff_exponent);
                            value->RGBA() += AiArrayGetRGBA(iter->second.data, prims[i]) * w;
                            weight_total += w;
                        }
                    }
                    else if (iter->second.type == AI_TYPE_VECTOR)
                    {
                        for (size_t i = 0; i < prims.size(); ++i)
                        {
                            float t = AiV3Dist(sg->Po, pluginData->fastPosition(prims[i], pkey, pt)) /
                                      (2.0f * pluginData->fastRadius(pluginData->fastRadiusPrim(prims[i]), rkey, rt));
                            float w = std::pow(1.0f - std::min(t, 0.99999f), pluginData->falloff_exponent);
                            value->VEC() += AiArrayGetVec(iter->second.data, prims[i]) * w;
                            weight_total += w;
                        }
                    }
                    else if (iter->second.type == AI_TYPE_VECTOR2)
                    {
                        for (size_t i = 0; i < prims.size(); ++i)
                        {
                            float t = AiV3Dist(sg->Po, pluginData->fastPosition(prims[i], pkey, pt)) /
                                      (2.0f * pluginData->fastRadius(pluginData->fastRadiusPrim(prims[i]), rkey, rt));
                            float w = std::pow(1.0f - std::min(t, 0.99999f), pluginData->falloff_exponent);
                            value->VEC2() += AiArrayGetVec2(iter->second.data, prims[i]) * w;
                            weight_total += w;
                        }
                    }
                }
                // Normalize weighting
                if (weight_total > 0.0f)
                    value->RGBA() *= (1.0f / weight_total);
            }
            else
            {
                // Generally speaking, for volumetric particles we don't apply the
                // falloff to the other channels, and assume that the density's
                // falloff deals with that already.  So we just add it all up.
                if (iter->second.type == AI_TYPE_FLOAT)
                {
                    for (size_t i = 0; i < prims.size(); ++i)
                        value->FLT() += AiArrayGetFlt(iter->second.data, prims[i]);
                }
                else if (iter->second.type == AI_TYPE_RGB)
                {
                    for (size_t i = 0; i < prims.size(); ++i)
                        value->RGB() += AiArrayGetRGB(iter->second.data, prims[i]);
                }
                else if (iter->second.type == AI_TYPE_RGBA)
                {
                    for (size_t i = 0; i < prims.size(); ++i)
                        value->RGBA() += AiArrayGetRGBA(iter->second.data, prims[i]);
                }
                else if (iter->second.type == AI_TYPE_VECTOR)
                {
                    for (size_t i = 0; i < prims.size(); ++i)
                        value->VEC() += AiArrayGetVec(iter->second.data, prims[i]);
                }
                else if (iter->second.type == AI_TYPE_VECTOR2)
                {
                    for (size_t i = 0; i < prims.size(); ++i)
                        value->VEC2() += AiArrayGetVec2(iter->second.data, prims[i]);
                }
            }
            return !prims.empty();
        }
        else
        {
            // Constant value, return it uniformly (RGBA is big enough to hold all float types)
            value->RGBA() = iter->second.value.RGBA();
            return true;
        }
    }

    return false;
}

volume_gradient
{
    ParticleVolumePluginData *pluginData = reinterpret_cast<ParticleVolumePluginData*>(data->private_info);

    *gradient = AI_V3_ZERO;
    if (channel != pluginData->field_channel)
        return false;

    ParticleIntersector intersector(*pluginData, sg->time);
    std::vector<unsigned int>& prims = pluginData->perThreadPrims(sg->tid);

    float field = fieldValue(pluginData, sg->Po, sg->time, intersector, prims);

    AtVector p = sg->Po;
    float f;
    p.x += pluginData->gradient_eps;
    f = fieldValue(pluginData, p, sg->time, intersector, prims);
    gradient->x = (f - field) * pluginData->gradient_inv_eps;

    p = sg->Po;
    p.y += pluginData->gradient_eps;
    f = fieldValue(pluginData, p, sg->time, intersector, prims);
    gradient->y = (f - field) * pluginData->gradient_inv_eps;

    p = sg->Po;
    p.z += pluginData->gradient_eps;
    f = fieldValue(pluginData, p, sg->time, intersector, prims);
    gradient->z = (f - field) * pluginData->gradient_inv_eps;

    return true;
}

volume_ray_extents
{
    if (!data->private_info) return;

    ParticleVolumePluginData *pluginData = reinterpret_cast<ParticleVolumePluginData*>(data->private_info);
    IntervalList& intervals = pluginData->perThreadIntervalList(tid);
    intervals.clear();

    // Gather all ray extents (not generally sorted, and may overlap)
    ParticleIntersector intersector(*pluginData, time);
    if (!pluginData->bvh->rayIntersections(*origin, *direction, t0, t1, intervals, intersector))
        return;

    // Sort and merge overlapping extents
    intervals.merge();

    for (size_t i = 0; i < intervals.size(); ++i)
        AiVolumeAddIntersection(info, intervals[i].t_start, intervals[i].t_end);
}


} // namespace


node_loader
{
   if (i > 1)
      return false;

   if (i == 0)
   {
      node->methods      = VolumeParticleMtd;
      node->output_type  = AI_TYPE_NONE;
      node->name         = "volume_particle";
      node->node_type    = AI_NODE_SHAPE_VOLUME;
      strcpy(node->version, AI_VERSION);
   }
   else
   {
      node->methods      = VolumeParticleMtd;
      node->output_type  = AI_TYPE_NONE;
      node->name         = "implicit_particle";
      node->node_type    = AI_NODE_SHAPE_IMPLICIT;
      strcpy(node->version, AI_VERSION);
   }
   return true;
}

