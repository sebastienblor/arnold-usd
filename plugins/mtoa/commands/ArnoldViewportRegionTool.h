
#include <maya/MPxContextCommand.h>
#include <maya/MPxContext.h>
#include <maya/MEvent.h>
#include <maya/MGlobal.h>
#include <maya/M3dView.h>


class ArnoldViewportRegionContext : public MPxContext
{
public:
	ArnoldViewportRegionContext();
	void	toolOnSetup( MEvent & event ) override;

	// Default viewport or hardware viewport methods override, will not be triggered in viewport 2.0.
	MStatus	doPress( MEvent & event ) override;
	MStatus	doDrag( MEvent & event ) override;
	MStatus	doRelease( MEvent & event ) override;
	MStatus	doEnterRegion( MEvent & event ) override;

	// Viewport 2.0 methods, will only be triggered in viewport 2.0.
	MStatus		doPress ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;
	MStatus		doRelease( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;
	MStatus		doDrag ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;

private:
	// Marquee draw method in default viewport or hardware viewport with immediate OpenGL call
	void					drawMarqueeGL();
	// Common operation to handle when pressed
	void					doPressCommon( MEvent & event );
	// Common operation to handle when released
	void					doReleaseCommon( MEvent & event );

	short					start_x, start_y;
	short					last_x, last_y;

	bool					fsDrawn;

	MGlobal::ListAdjustment	listAdjustment;
	M3dView					view;
};


//////////////////////////////////////////////
// Command to create contexts
//////////////////////////////////////////////

class ArnoldViewportRegionContextCmd : public MPxContextCommand
{
public:	
						ArnoldViewportRegionContextCmd();
	MPxContext*	makeObj() override;
	static	void*		creator();
};
