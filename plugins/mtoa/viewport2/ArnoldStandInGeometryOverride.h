//#include "utils/DrawUtils.h"

/*
#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MObject.h>
*/

#include <maya/MDrawRegistry.h>
#include <maya/MPxGeometryOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MShaderManager.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MPointArray.h>

class CArnoldStandInGeometryOverride : public MHWRender::MPxGeometryOverride
{
public:
	static MHWRender::MPxGeometryOverride* Creator(const MObject& obj)
	{
		return new CArnoldStandInGeometryOverride(obj);
	}

	virtual ~CArnoldStandInGeometryOverride();

	virtual MHWRender::DrawAPI supportedDrawAPIs() const;

	virtual bool hasUIDrawables() const { return false; }

	virtual void updateDG();
	virtual bool isIndexingDirty(const MHWRender::MRenderItem &item) { return false; }
	virtual bool isStreamDirty(const MHWRender::MVertexBufferDescriptor &desc) { return mBBChanged; }
	virtual void updateRenderItems(const MDagPath &path, MHWRender::MRenderItemList& list);
	virtual void populateGeometry(const MHWRender::MGeometryRequirements &requirements, const MHWRender::MRenderItemList &renderItems, MHWRender::MGeometry &data);
	virtual void cleanUp() {};


private:
	CArnoldStandInGeometryOverride(const MObject& obj);

	MHWRender::MShaderInstance* mSolidUIShader;
	MObject mLocatorNode;
   
   float minx, miny, minz;
   float maxx, maxy, maxz;
   bool mBBChanged;
};

