#include "utils/DrawUtils.h"
#include "nodes/light/ArnoldSkyDomeLightNode.h"

#include <maya/MPxGeometryOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MObject.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MUintArray.h>

class CArnoldSkyDomeLightGeometryOverride : public MHWRender::MPxGeometryOverride{
public:
    static MHWRender::MPxGeometryOverride* Creator(const MObject& obj);

    ~CArnoldSkyDomeLightGeometryOverride();

	virtual void updateDG();
	virtual bool isIndexingDirty(const MHWRender::MRenderItem &item);
	virtual bool isStreamDirty(const MHWRender::MVertexBufferDescriptor &desc);
	virtual void updateRenderItems(const MDagPath &path, MHWRender::MRenderItemList& list);
	virtual void populateGeometry(const MHWRender::MGeometryRequirements &requirements, 
								const MHWRender::MRenderItemList &renderItems, 
								MHWRender::MGeometry &data);
	virtual void cleanUp();
/*
    virtual bool isBounded(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const;

    virtual MBoundingBox boundingBox(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const;

    virtual bool disableInternalBoundingBoxDraw() const;

    virtual MUserData* prepareForDraw(
        const MDagPath& objPath,
        const MDagPath& cameraPath,
        const MHWRender::MFrameContext& frameContext,
        MUserData* oldData);
*/
    virtual MHWRender::DrawAPI supportedDrawAPIs() const;
/*
	virtual bool hasUIDrawables() const { return true; }
	virtual void addUIDrawables(
		const MDagPath& objPath,
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext,
		const MUserData* data);

    static void draw(const MHWRender::MDrawContext& context, const MUserData* data);
    static void clearGPUResources();
*/
private:
    CArnoldSkyDomeLightGeometryOverride(const MObject& obj);

	CArnoldSkyDomeLightNode* p_skydomeNode;

	// Shaders used for drawing 
	MHWRender::MShaderInstance* m_wireframeShader;
	MHWRender::MShaderInstance* m_texturedShader;

	// Texture for texture shader

	// Render item names
	static MString s_wireframeItemName;
	static MString s_texturedItemName;

	// Associated object
	MObject m_LocatorNode;

	// Node data
	float m_modelMatrix[4][4]; // cut out translation, that doesn't matter
    float m_wireframeColor[4];
    float m_radius;
    int m_format;    
    // 0 - MirroredBall
    // 1 - Angular
    // 2 - LatLong

	static void createSkyDomeGeometry(unsigned int divisions[2],
										float radius);

	// Cleared from main, like clear GPU resources....
	static unsigned int s_divisions[2];

	static MFloatVectorArray s_filledPositions;
	static MFloatArray  s_filledUvs[3]; // One set for each uv format
	static MUintArray s_filledIndexing;

	static MFloatVectorArray s_wirePositions;
	static MUintArray s_wireIndexing;
/*
    static void initializeGPUResources();

    static GLuint s_vertexShaderWireframe;
    static GLuint s_fragmentShaderWireframe;
    static GLuint s_programWireframe;

    //static GLuint s_vertexShaderTextured;
    //static GLuint s_fragmentShaderTextured;
    //static GLuint s_programTextured;

    static GLuint s_VBO;
    static GLuint s_IBOWireframe;
    static GLuint s_IBOTextured;

    static GLuint s_numWireframeIndices;
    static GLuint s_numTexturedIndices;

    static GLint s_modelLocWireframe;
    static GLint s_viewProjLocWireframe;
    static GLint s_scaleLocWireframe;
    static GLint s_shadeColorLocWireframe;

    static bool s_isValid;
    static bool s_isInitialized;
*/
};
