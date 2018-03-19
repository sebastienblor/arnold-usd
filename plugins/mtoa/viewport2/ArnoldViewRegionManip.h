//-
// ==========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

#include <maya/MTypeId.h>
#include <maya/MTypes.h>
#include <maya/MPxManipulatorNode.h>
#include <maya/MObjectHandle.h>
#include <maya/MDagModifier.h>
#include <maya/MPxDrawOverride.h>
#include <maya/M3dView.h>
#include <maya/MPoint.h>
#include <maya/MGLFunctionTable.h>

#if MAYA_API_VERSION >= 201700
//
// Custom manipulator class
//
class ArnoldViewRegionManipulator : public MPxManipulatorNode
{
public:
	ArnoldViewRegionManipulator();
	~ArnoldViewRegionManipulator() override;
	void postConstructor() override;

	// The important virtuals to implement
	void preDrawUI( const M3dView &view ) override;
	void drawUI(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext) const override;

	MStatus	doPress(M3dView& view) override;
	MStatus	doDrag(M3dView& view) override;
	MStatus	doRelease(M3dView& view) override;
    MStatus doMove(M3dView& view, bool& refresh) override;

	// Standard API required methods
	static void* creator();
	static MStatus initialize();

	// Node id
	static MTypeId id;
	static MString classification;
	static MString registrantId;

private:
	bool shouldDraw(const MDagPath& camera) const;

	// GL component names for drawing and picking
	MGLuint fActiveName, fTopName, fRightName, fBottomName, fLeftName, fNoneName;
    MGLuint fTopLeftName, fTopRightName, fBottomLeftName, fBottomRightName, fAllName;

	// Modified mouse position used for updating manipulator
	MPoint fMousePoint;

	bool fDrawManip;
    bool fDrawAsMouseOver;
    MFloatPoint mViewRectangle;
    MFloatPoint mDragRectangle;
};

#endif
