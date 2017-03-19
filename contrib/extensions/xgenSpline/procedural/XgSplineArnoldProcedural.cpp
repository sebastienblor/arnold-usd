#include <ai.h>

#include <XGen/SgCurve.h>
#include <XGen/XgSplineAPI.h>

#include <cstring>
#include <string>
#include <sstream>


namespace XgArnoldInternal
{

using namespace XGenSplineAPI;

// ============================================================================

// Procedural to create "curves" node from input parameters
class XgSplineProcedural
{
public:
    XgSplineProcedural()
        : _curves(NULL)
        , _numPoints(NULL)
        , _points(NULL)
        , _radius(NULL)
        , _uCoord(NULL)
        , _vCoord(NULL)
        , _wCoord(NULL)
        , _shader(NULL)
        , _aiMinPixelWidth(0.0f)
        , _aiMode(0)
    {}

    ~XgSplineProcedural()
    {}

    bool init(AtNode* procedural)
    {
        // Load the spline data from the procedural into our workspace
        if (!loadSplineData(procedural))
        {
            AiMsgError("Failed to load spline data from the procedural parameter.");
            return false;
        }

        // Execute render-time script such as density multiplier and density mask
        _splines.executeScript();

        // Load other parameters from the procedural
        loadParameters(procedural);

        // Create the "curves" node as our hair object
        createCurves(procedural);

        // Fill the "curves" node with array data
        fillCurves(procedural);

        return true;
    }

    int numNodes()
    {
        return _curves ? 1 : 0;
    }

    AtNode* getNode(int i)
    {
        return _curves;
    }

private:
    bool loadSplineData(AtNode* procedural)
    {
        // Get the sample times
        AtArray* sampleTimes = AiNodeGetArray(procedural, "sampleTimes");
        if (!sampleTimes || AiArrayGetNumElements(sampleTimes) == 0)
        {
            AiMsgError("Invalid number of samples.");
            return false;
        }

        // Get frame-per-second
        const float fps = AiNodeGetFlt(procedural, "fps");
        _splines.setFps(fps);

        // Load samples
        // No motion blur : one sample at current frame
        // Motion blur    : one sample for each motion step
        unsigned nelements = AiArrayGetNumElements(sampleTimes);
        for (unsigned int i = 0; i < nelements; i++)
        {
            // Get the time of the sample
            const float sampleTime = AiArrayGetFlt(sampleTimes, i);

            // Get the name of the sample parameters
            std::stringstream ss;
            ss << "sampleData_" << i;
            const std::string dataParam = ss.str();

            ss.str("");
            ss << "samplePadding_" << i;
            const std::string paddingParam = ss.str();

            // Get the spline data for i-th motion step
            AtArray* sampleData = AiNodeGetArray(procedural, dataParam.c_str());
            if (!sampleData || AiArrayGetType(sampleData) != AI_TYPE_UINT) continue;

            // Get the number of padding bytes
            const unsigned int padding = AiNodeGetUInt(procedural, paddingParam.c_str());
            const size_t sampleSize = size_t(AiArrayGetNumElements(sampleData)) * sizeof(unsigned int) - padding;

            // Wrap the spline data as an input stream
            std::stringstream opaqueStrm;
            
            void *arrayData = AiArrayMap(sampleData);
            if (arrayData)
            {
                opaqueStrm.write(reinterpret_cast<const char*>(arrayData), sampleSize);
                opaqueStrm.flush();
                opaqueStrm.seekp(0);
            }
            AiArrayUnmap(sampleData);

            // Load the sample for i-th motion step
            if (!_splines.load(opaqueStrm, sampleSize, sampleTime))
            {
                AiMsgError("Failed to load sample %d.", i);
                return false;
            }
        }

        return true;
    }

    void loadParameters(AtNode* procedural)
    {
        _shader             = (AtNode*)AiNodeGetPtr(procedural, "shader");
        _aiMinPixelWidth    = AiNodeGetFlt(procedural, "ai_min_pixel_width");
        _aiMode             = AiNodeGetInt(procedural, "ai_mode");
    }

    void createCurves(AtNode* procedural)
    {
        // Create the Arnold "curves" node for all the splines
        _curves = AiNode("curves");

        // Inherit the name from the procedural
        const std::string proceduralName = AiNodeGetName(procedural);
        const std::string curvesName     = proceduralName + "_curves";
        AiNodeSetStr(_curves, "name", curvesName.c_str());
        
        // XGen is using uniform bsplines
        AiNodeSetStr(_curves, "basis", "b-spline");

        // Passthrough the shader assignment
        AiNodeSetPtr(_curves, "shader", _shader);

        // Min pixel width
        AiNodeSetFlt(_curves, "min_pixel_width", _aiMinPixelWidth);

        // Curves mode ("thick", "ribbon", or "oriented")
        AiNodeSetStr(_curves, "mode", (_aiMode == 1) ? "thick" : "ribbon");

        // (u, v) on patch
        AiNodeDeclare(_curves, "uparamcoord", "uniform FLOAT");
        AiNodeDeclare(_curves, "vparamcoord", "uniform FLOAT");

        // (w) from root to tip
        AiNodeDeclare(_curves, "wparamcoord", "varying FLOAT");
    }

    void fillCurves(AtNode* procedural)
    {
        // Count the number of curves and the number of points
        // Arnold's b-spline requires two phantom points.
        unsigned int curveCount        = 0;
        unsigned int pointCount        = 0;
        unsigned int pointInterpoCount = 0;
        for (XgItSpline splineIt = _splines.iterator(); !splineIt.isDone(); splineIt.next())
        {
            curveCount        += splineIt.primitiveCount();
            pointCount        += splineIt.vertexCount();
            pointInterpoCount += splineIt.vertexCount() + splineIt.primitiveCount() * 2;
        }

        // Get the number of motion samples
        const unsigned int steps = _splines.sampleCount();

        // Allocate buffers for the curves
        _numPoints  = AiArrayAllocate(curveCount, 1, AI_TYPE_UINT);
        _points     = AiArrayAllocate(pointInterpoCount, steps, AI_TYPE_VECTOR);
        _radius     = AiArrayAllocate(pointCount, 1, AI_TYPE_FLOAT);
        _uCoord     = AiArrayAllocate(curveCount, 1, AI_TYPE_FLOAT);
        _vCoord     = AiArrayAllocate(curveCount, 1, AI_TYPE_FLOAT);
        _wCoord     = AiArrayAllocate(pointInterpoCount, 1, AI_TYPE_FLOAT);

        // FIXME Arnold 5
        // here we're doing AiNodeSetArray below, so we won't need to unMap the arrays
        unsigned int*   numPoints   = reinterpret_cast<unsigned int*>(AiArrayMap(_numPoints));
        SgVec3f*        points      = reinterpret_cast<SgVec3f*>(AiArrayMap(_points));
        float*          radius      = reinterpret_cast<float*>(AiArrayMap(_radius));
        float*          uCoord      = reinterpret_cast<float*>(AiArrayMap(_uCoord));
        float*          vCoord      = reinterpret_cast<float*>(AiArrayMap(_vCoord));
        float*          wCoord      = reinterpret_cast<float*>(AiArrayMap(_wCoord));

        // Fill the array buffers for motion step 0
        for (XgItSpline splineIt = _splines.iterator(); !splineIt.isDone(); splineIt.next())
        {
            const unsigned int  stride         = splineIt.primitiveInfoStride();
            const unsigned int  primitiveCount = splineIt.primitiveCount();
            const unsigned int* primitiveInfos = splineIt.primitiveInfos();
            const SgVec3f*      positions      = splineIt.positions(0);
            const float*        width          = splineIt.width();
            const SgVec2f*      texcoords      = splineIt.texcoords();
            const SgVec2f*      patchUVs       = splineIt.patchUVs();

            for (unsigned int p = 0; p < primitiveCount; p++)
            {
                const unsigned int offset = primitiveInfos[p * stride];
                const unsigned int length = primitiveInfos[p * stride + 1];

                // Number of points
                *numPoints++ = length + 2;

                // Texcoord using the patch UV from the root point
                *uCoord++ = patchUVs[offset][0];
                *vCoord++ = patchUVs[offset][1];

                // Add phantom points at the beginning
                *points++ = phantomFirst(&positions[offset], length);
                *wCoord++ = phantomFirst(&texcoords[offset], length)[1];

                // Copy varying data
                for (unsigned int i = 0; i < length; i++)
                {
                    *points++ = positions[offset + i];
                    *radius++ = width[offset + i] * 0.5f;
                    *wCoord++ = texcoords[offset + i][1];
                }

                // Add phantom points at the end
                *points++ = phantomLast(&positions[offset], length);
                *wCoord++ = phantomLast(&texcoords[offset], length)[1];

            } // for each primitive
        } // for each primitive batch

        // Fill the array buffers for motion step > 0
        for (unsigned int key = 1; key < steps; key++)
        {
            for (XgItSpline splineIt = _splines.iterator(); !splineIt.isDone(); splineIt.next())
            {
                const unsigned int  stride         = splineIt.primitiveInfoStride();
                const unsigned int  primitiveCount = splineIt.primitiveCount();
                const unsigned int* primitiveInfos = splineIt.primitiveInfos();
                const SgVec3f*      positions      = splineIt.positions(key);

                for (unsigned int p = 0; p < primitiveCount; p++)
                {
                    const unsigned int offset = primitiveInfos[p * stride];
                    const unsigned int length = primitiveInfos[p * stride + 1];

                    // Add phantom points at the beginning
                    *points++ = phantomFirst(&positions[offset], length);

                    // Copy varying data
                    for (unsigned int i = 0; i < length; i++)
                    {
                        *points++ = positions[offset + i];
                    }

                    // Add phantom points at the end
                    *points++ = phantomLast(&positions[offset], length);

                } // for each primitive
            } // for each primitive batch
        } // for each motion step

        // Set the buffers to the curves node
        AiNodeSetArray(_curves, "num_points", _numPoints);
        AiNodeSetArray(_curves, "points", _points);
        AiNodeSetArray(_curves, "radius", _radius);
        AiNodeSetArray(_curves, "uparamcoord", _uCoord);
        AiNodeSetArray(_curves, "vparamcoord", _vCoord);
        AiNodeSetArray(_curves, "wparamcoord", _wCoord);
    }

    template<typename T>
    inline static T phantomFirst(const T* cpt, unsigned int)
    {
        return cpt[0] + (cpt[0] - cpt[1]);
    }

    template<typename T>
    inline static T phantomLast(const T* cpt, unsigned int nump)
    {
        const unsigned int nump1 = nump - 1;
        return cpt[nump1] + (cpt[nump1] - cpt[nump1-1]);
    }

private:
    // Input spline data
    XgFnSpline  _splines;

    // Output curves node
    AtNode*     _curves;

    // Curve arrays
    AtArray*    _numPoints;
    AtArray*    _points;
    AtArray*    _radius;
    AtArray*    _uCoord;
    AtArray*    _vCoord;
    AtArray*    _wCoord;

    // Material
    AtNode*     _shader;

    // Parameters
    float       _aiMinPixelWidth;
    int         _aiMode;
};
// ============================================================================


AI_PROCEDURAL_NODE_EXPORT_METHODS(XgSplineProceduralMtd);


node_parameters
{
}

procedural_init
{
    XgSplineProcedural* proc = new XgSplineProcedural();
    if (!proc->init(node))
    {
        delete proc;
        proc = NULL;
    }
    *user_ptr = reinterpret_cast<void*>(proc);
    return proc ? 1 : 0;
}

procedural_cleanup
{
    delete reinterpret_cast<XgSplineProcedural*>(user_ptr);
    return 1;
}
procedural_num_nodes
{
    XgSplineProcedural* proc = reinterpret_cast<XgSplineProcedural*>(user_ptr);
    return proc ? proc->numNodes() : 0;
}

procedural_get_node
{
    XgSplineProcedural* proc = reinterpret_cast<XgSplineProcedural*>(user_ptr);
    return proc ? proc->getNode(i) : NULL;
}


node_loader
{
   if (i>0)
      return false;

   node->methods      = XgSplineProceduralMtd;
   node->output_type  = AI_TYPE_NONE;
   node->name         = "xgenProcedural";
   node->node_type    = AI_NODE_SHAPE_PROCEDURAL;
   strcpy(node->version, AI_VERSION);
   return true;
}

};