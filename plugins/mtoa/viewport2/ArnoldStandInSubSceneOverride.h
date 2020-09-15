#include <maya/MDrawRegistry.h>
#include <maya/MPxSubSceneOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MShaderManager.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MPointArray.h>
#include <maya/MMessage.h>

#include "common/UnorderedContainer.h"
#include <ai_vector.h>
#include <vector>
#include <map>
#include "ArnoldProceduralSubSceneOverride.h"

class CArnoldStandInSubSceneOverride : public CArnoldProceduralSubSceneOverride
{
public:
	static MHWRender::MPxSubSceneOverride* Creator(const MObject& obj);
	virtual ~CArnoldStandInSubSceneOverride();
private:
    CArnoldStandInSubSceneOverride(const MObject& obj);

};

