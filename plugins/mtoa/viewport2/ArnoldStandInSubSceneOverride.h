#include <maya/MDrawRegistry.h>
#include <maya/MPxSubSceneOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MShaderManager.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MPointArray.h>
#include <maya/MMessage.h>

#if MAYA_API_VERSION >= 201650

#include <ai_vector.h>
#include <vector>
#include <map>

class CArnoldStandInShape;
class CArnoldStandInGeom;
class CArnoldStandInGeometry;

class CArnoldStandInSubSceneOverride : public MHWRender::MPxSubSceneOverride
{
public:
	static MHWRender::MPxSubSceneOverride* Creator(const MObject& obj)
	{
		return new CArnoldStandInSubSceneOverride(obj);
	}

	virtual ~CArnoldStandInSubSceneOverride();

    virtual MHWRender::DrawAPI supportedDrawAPIs() const;
    virtual bool requiresUpdate(const MHWRender::MSubSceneContainer& container,
                                const MHWRender::MFrameContext& frameContext) const;
    virtual void update(MHWRender::MSubSceneContainer& container,
                        const MHWRender::MFrameContext& frameContext);
    virtual bool furtherUpdateRequired(const MHWRender::MFrameContext& frameContext) { return false; }
    
    virtual bool areUIDrawablesDirty() const { return false; }
    virtual bool hasUIDrawables() const { return false; }
    virtual void addUIDrawables(MHWRender::MUIDrawManager& drawManager,
                                const MHWRender::MFrameContext& frameContext) {}

    virtual bool getInstancedSelectionPath(const MHWRender::MRenderItem& renderItem, 
                                           const MHWRender::MIntersection& intersection, 
                                           MDagPath& dagPath) const;

    void invalidate(bool tryToReuse) 
    { 
        mBBChanged = true; 
        mReuseBuffers &= tryToReuse;
    }

    void releaseShadedMaterial();

private:

    static void globalOptionsAdded(MObject& node, void* clientData);
    void globalOptionsAdded(MObject& node);

	CArnoldStandInSubSceneOverride(const MObject& obj);

    MHWRender::MRenderItem* getItem(MHWRender::MSubSceneContainer& container, const MString& name, 
                                    MHWRender::MGeometry::Primitive primitiveType, 
                                    unsigned int depthPriority = MHWRender::MRenderItem::sActiveWireDepthPriority,
                                    MHWRender::MGeometry::DrawMode drawMode = MHWRender::MGeometry::kAll);

    void updateWireframeCubeItem(CArnoldStandInShape* standIn, MHWRender::MRenderItem* wireframecube, 
                                 const MHWRender::MShaderInstance* shader, bool scaled);
    void updateRenderItem(MHWRender::MSubSceneContainer& container, CArnoldStandInGeom* geom, 
                          MHWRender::MRenderItem* item, size_t totalCount, const MHWRender::MShaderInstance* shader, 
                          bool wantNormals = false, bool boxMode = false);

    void fillBuffers(const CArnoldStandInGeometry& standIn, unsigned int* indices, float* vertices, float* normals, 
                     size_t& startIndex, size_t& pointOffset, const MHWRender::MGeometry::Primitive& primitive, 
                     bool wantNormals, bool boxMode);
    size_t getIndexing(const CArnoldStandInGeometry& standIn, unsigned int* indices, unsigned int pointOffset, 
                       const MHWRender::MGeometry::Primitive& primitive, bool wantNormals);
    size_t getVertexStreams(const CArnoldStandInGeometry& standIn, float* vertices, float* normals);

    int getDrawOverride();
    void updateShaderFromNode();

    bool updateInstanceData(const MDagPathArray& instances);
    void getInstanceTransforms(MMatrixArray& instanceMatrixArray, 
        MMatrixArray& selectedInstanceMatrixArray, 
        MMatrixArray& unselectedInstanceMatrixArray,
        int& leadIndex);

    bool anyChanges(const MHWRender::MSubSceneContainer& container);

	MHWRender::MShaderInstance* mSolidUIShader;
    MHWRender::MShaderInstance* mSelectedSolidUIShader;
    MHWRender::MShaderInstance* mLeadSolidUIShader;
    MHWRender::MShaderInstance* mSolidShader;
    MHWRender::MShaderInstance* mBlinnShader;
    MHWRender::MShaderInstance* mShaderFromNode;
	MObject mLocatorNode;
   
    unsigned int fLeadIndex;
    unsigned int fNumInstances;
    bool mBBChanged, mOneTimeUpdate, mReuseBuffers;
    MCallbackId mAttribChangedID, mGlobalOptionsChangedID, mGlobalOptionsCreatedID;

    struct InstanceInfo
    {
        MMatrix fTransform;
        bool fSelected;
        bool fLead;
        MDagPath fInstance;

        InstanceInfo() : fSelected(false), fLead(false) {}
        InstanceInfo(const MDagPath& instance, const MMatrix& matrix, bool selected, bool lead) : 
            fTransform(matrix), fSelected(selected), fLead(lead), fInstance(instance) {}
    };
    typedef AtMap<unsigned int, InstanceInfo> InstanceInfoMap;
    InstanceInfoMap fInstanceInfoCache;
	bool fLastTimeInvisible;
};

#endif
