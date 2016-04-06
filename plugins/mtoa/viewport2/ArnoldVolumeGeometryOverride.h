#pragma once

#include "utils/DrawUtils.h"

#include <maya/MPxGeometryOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MFloatVectorArray.h>

#if MAYA_API_VERSION >= 201650

class CArnoldVolumeGeometryOverride : public MHWRender::MPxGeometryOverride{
public:
    static MHWRender::MPxGeometryOverride* Creator(const MObject& obj);

    ~CArnoldVolumeGeometryOverride();

    virtual MHWRender::DrawAPI supportedDrawAPIs() const;

	virtual void updateDG() {};
	virtual bool isIndexingDirty(const MHWRender::MRenderItem &item);
	virtual bool isStreamDirty(const MHWRender::MVertexBufferDescriptor &desc);
	virtual void updateRenderItems(const MDagPath &path, MHWRender::MRenderItemList& list);
	virtual void populateGeometry(const MHWRender::MGeometryRequirements &requirements, 
								const MHWRender::MRenderItemList &renderItems, 
								MHWRender::MGeometry &data);
	virtual void cleanUp();

private:
    CArnoldVolumeGeometryOverride(const MObject& obj);

   	MHWRender::MShaderInstance* m_wireframeShader;
	MHWRender::MShaderInstance* m_activeWireframeShader; 

	// Cached data
	MFloatVectorArray m_wirePositions;
	MUintArray m_wireIndexing;
	bool m_geometryDirty;
	float m_scale[3];
    float m_offset[3];
    float m_wireframeColor[4];

	// Render item names
	static MString s_wireframeItemName;
	static MString s_activeWireframeItemName;

	// Associated object
	MObject m_Node;
};
#endif
