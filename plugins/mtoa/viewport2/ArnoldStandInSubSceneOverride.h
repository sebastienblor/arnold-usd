#include <maya/MDrawRegistry.h>
#include <maya/MPxSubSceneOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MShaderManager.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MPointArray.h>
#include <maya/MMessage.h>

#include <ai_vector.h>
#include <vector>

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

    void invalidate() { mBBChanged = true; }
    void releaseShadedMaterial();

private:
	CArnoldStandInSubSceneOverride(const MObject& obj);

    MHWRender::MRenderItem* getItem(MHWRender::MSubSceneContainer& container, const MString& name, 
                                    MHWRender::MGeometry::Primitive primitiveType, 
                                    unsigned int depthPriority = MHWRender::MRenderItem::sActiveWireDepthPriority,
                                    MHWRender::MGeometry::DrawMode drawMode = MHWRender::MGeometry::kAll);

    void updateWireframeCubeItem(CArnoldStandInShape* standIn, MHWRender::MRenderItem* wireframecube, bool scaled);
    void updateRenderItem(MHWRender::MSubSceneContainer& container, CArnoldStandInGeom* geom, 
                          MHWRender::MRenderItem* item, size_t totalCount, MHWRender::MShaderInstance* shader, 
                          bool wantNormals = false, bool boxMode = false);

    void fillBuffers(const CArnoldStandInGeometry& standIn, unsigned int* indices, float* vertices, float* normals, 
                     size_t& startIndex, size_t& pointOffset, const MHWRender::MGeometry::Primitive& primitive, 
                     bool wantNormals, bool boxMode);
    size_t getIndexing(const CArnoldStandInGeometry& standIn, unsigned int* indices, unsigned int pointOffset, 
                       const MHWRender::MGeometry::Primitive& primitive, bool wantNormals);
    size_t getVertexStreams(const CArnoldStandInGeometry& standIn, float* vertices, float* normals);

    int getDrawOverride();
    void updateShaderFromNode();

	MHWRender::MShaderInstance* mSolidUIShader;
    MHWRender::MShaderInstance* mSolidShader;
    MHWRender::MShaderInstance* mBlinnShader;
    MHWRender::MShaderInstance* mShaderFromNode;
	MObject mLocatorNode;
   
   bool mBBChanged, mOneTimeUpdate;
   MCallbackId mAttribChangedID, mGlobalOptionsChangedID;
};

