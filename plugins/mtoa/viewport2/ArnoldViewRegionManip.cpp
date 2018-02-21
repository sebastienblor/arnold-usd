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

//
//	squareScaleManip.cpp
//		Creates manipulator node squareScaleManipulator
//		Creates command squareManipCmd
//
//	This example demonstrates how to use the MPxManipulatorNode
//	class along with a command to create a user defined
//	manipulator.  The manipulator created is a simple square
//	with the 4 sides as OpenGL pickable components.  As you
//	move the pickable component, selected transforms have
//	their scale attribute modified.
//	A corresponding command is used to create and delete
//	the manipulator node and to support undo/redo etc.

/*

// To show this example using MEL, run the following:
loadPlugin squareScaleManip.so;
squareManipCmd -create;

// To delete the manipulator using MEL:
squareManipCmd -delete;

*/

#include "ArnoldViewRegionManip.h"

#include "scene/MayaScene.h"

#include <maya/MIOStream.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnTransform.h>
#include <maya/MGlobal.h>
#include <maya/MFnCamera.h>
#include <maya/MDrawContext.h>
#include <maya/MUserData.h>
#include <maya/MStateManager.h>

// Statics
MTypeId ArnoldViewRegionManipulator::id(0x81046);
MString ArnoldViewRegionManipulator::registrantId("SquareScaleManipPlugin");

//
// class implementation
//
ArnoldViewRegionManipulator::ArnoldViewRegionManipulator()
: MPxManipulatorNode()
, fActiveName(0)
, fTopName(0)
, fRightName(0)
, fBottomName(0)
, fLeftName(0)
, fTopLeftName(0)
, fTopRightName(0)
, fBottomLeftName(0)
, fBottomRightName(0)
, fAllName(0)
, fMousePoint(MPoint())
, fDrawAsMouseOver(false)
{

    mViewRectangle = MFloatPoint(0.33f, 0.33f, 0.66f, 0.66f);
    // order here is left, top, right, bottom

    // Eventually get the crop region value in the render session
    CRenderSession *renderSession = CMayaScene::GetRenderSession();
    CRenderOptions *renderOptions = (renderSession) ? renderSession->RenderOptions() : NULL;
	
    if (renderOptions && renderOptions->useRenderRegion() && renderSession->IsRegionCropped())
    {
        int width = renderOptions->width();
        int height = renderOptions->height();
        int minx = renderOptions->minX();
        int miny = renderOptions->minY();
        int maxx = renderOptions->maxX();
        int maxy = renderOptions->maxY();

        mViewRectangle.x = (float)minx / (float)width;
        mViewRectangle.y = 1.f - ((float)maxy / (float)height);
        mViewRectangle.z = (float)maxx / (float)width;
        mViewRectangle.w = 1.f - ((float)miny / (float)height);
    }
}

ArnoldViewRegionManipulator::~ArnoldViewRegionManipulator()
{
}

void ArnoldViewRegionManipulator::postConstructor()
{
	// Get the starting value of the pickable items
	MGLuint glPickableItem;
	glFirstHandle(glPickableItem);

	fTopName = glPickableItem++;
	fBottomName = glPickableItem++;
	fLeftName = glPickableItem++;
	fRightName = glPickableItem++;

    fTopLeftName = glPickableItem++;
    fTopRightName = glPickableItem++;
    fBottomLeftName = glPickableItem++;
    fBottomRightName = glPickableItem++;
    fAllName = glPickableItem++;

    registerForMouseMove();
}

//virtual 
void ArnoldViewRegionManipulator::preDrawUI( const M3dView &view )
{
	M3dView *viewPtr = const_cast<M3dView*>( &view );
	MDagPath dpath;
	viewPtr->getCamera(dpath);
	fDrawManip = shouldDraw(dpath);
}

//virtual 
void ArnoldViewRegionManipulator::drawUI(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext) const
{
	if(!fDrawManip)
		return;

    int originX = 0;
    int originY = 0;
    int width = 0;
    int height = 0;
    frameContext.getViewportDimensions(originX, originY, width, height);

    MPoint pt1, pt1_1, pt1_2;
    MPoint pt2, pt2_1, pt2_2;
    MPoint pt3, pt3_1, pt3_2, pt4, pt4_1, pt4_2;
    MPoint tempPt;

    float xPos = mViewRectangle.x * width;
    float yPos = (1.f - mViewRectangle.y) * height;
    float xPos2 = mViewRectangle.z * width;
    float yPos2 = (1.f - mViewRectangle.w) * height;

    float endPointOffset = 0.1f;

    float midPointX;
    float midPointY;

    frameContext.viewportToWorld(xPos, yPos, pt1, tempPt);
    pt1 = (pt1 + tempPt) * 0.5f;

    frameContext.viewportToWorld(xPos2, yPos2, pt2, tempPt);
    pt2 = (pt2 + tempPt) * 0.5f;

    frameContext.viewportToWorld(xPos, yPos2, pt3, tempPt);
    pt3 = (pt3 + tempPt) * 0.5f;

    frameContext.viewportToWorld(xPos2, yPos, pt4, tempPt);
    pt4 = (pt4 + tempPt) * 0.5f;
    const short defaultCol = mainColor();

    if (fDrawAsMouseOver)
    {
        { // upper left corner
            midPointX = xPos * (1.0f - endPointOffset) + xPos2 * endPointOffset;
            midPointY = yPos * (1.0f - endPointOffset) + yPos2 * endPointOffset;
            frameContext.viewportToWorld(midPointX, yPos, pt1_1, tempPt);
            pt1_1 = (pt1_1 + tempPt) * 0.5f;
            frameContext.viewportToWorld(xPos, midPointY, pt1_2, tempPt);
            pt1_2 = (pt1_2 + tempPt) * 0.5f;

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fTopLeftName);
            drawManager.setLineWidth(3);
            setHandleColor(drawManager, fTopLeftName, defaultCol);
            drawManager.line(pt1, pt1_1);
            drawManager.line(pt1, pt1_2);
            drawManager.endDrawable();
        }

        { // left
            midPointX = yPos * (0.5f - endPointOffset) + yPos2 * (0.5f + endPointOffset);
            midPointY = yPos * (0.5f + endPointOffset) + yPos2 * (0.5f - endPointOffset);

            frameContext.viewportToWorld(xPos, midPointX, pt4_1, tempPt);
            pt4_1 = (pt4_1 + tempPt) * 0.5f;
            frameContext.viewportToWorld(xPos, midPointY, pt4_2, tempPt);
            pt4_2 = (pt4_2 + tempPt) * 0.5f;

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fLeftName);
            drawManager.setLineWidth(3);
            setHandleColor(drawManager, fLeftName, defaultCol);
            drawManager.line(pt4_1, pt4_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawManager.line(pt1_2, pt4_1);
        drawManager.endDrawable();

        { // lower left corner
            midPointX = xPos * (1.0f - endPointOffset) + xPos2 * endPointOffset;
            midPointY = yPos2 * (1.0f - endPointOffset) + yPos * endPointOffset;
            frameContext.viewportToWorld(midPointX, yPos2, pt3_1, tempPt);
            pt3_1 = (pt3_1 + tempPt) * 0.5f;
            frameContext.viewportToWorld(xPos, midPointY, pt3_2, tempPt);
            pt3_2 = (pt3_2 + tempPt) * 0.5f;

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fBottomLeftName);
            drawManager.setLineWidth(3);
            setHandleColor(drawManager, fBottomLeftName, defaultCol);
            drawManager.line(pt3, pt3_1);
            drawManager.line(pt3, pt3_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawManager.line(pt4_2, pt3_2);
        drawManager.endDrawable();

        { // bottom
            midPointX = xPos * (0.5f - endPointOffset) + xPos2 * (0.5f + endPointOffset);
            midPointY = xPos * (0.5f + endPointOffset) + xPos2 * (0.5f - endPointOffset);

            frameContext.viewportToWorld(midPointX, yPos2, pt4_1, tempPt);
            pt4_1 = (pt4_1 + tempPt) * 0.5f;
            frameContext.viewportToWorld(midPointY, yPos2, pt4_2, tempPt);
            pt4_2 = (pt4_2 + tempPt) * 0.5f;

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fBottomName);
            drawManager.setLineWidth(3);
            setHandleColor(drawManager, fBottomName, defaultCol);
            drawManager.line(pt4_1, pt4_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawManager.line(pt4_2, pt3_1);
        drawManager.endDrawable();

        { // lower right corner
            midPointX = xPos2 * (1.0f - endPointOffset) + xPos * endPointOffset;
            midPointY = yPos2 * (1.0f - endPointOffset) + yPos * endPointOffset;
            frameContext.viewportToWorld(midPointX, yPos2, pt2_1, tempPt);
            pt2_1 = (pt2_1 + tempPt) * 0.5f;
            frameContext.viewportToWorld(xPos2, midPointY, pt2_2, tempPt);
            pt2_2 = (pt2_2 + tempPt) * 0.5f;

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fBottomRightName);
            drawManager.setLineWidth(3);
            setHandleColor(drawManager, fBottomRightName, defaultCol);
            drawManager.line(pt2, pt2_1);
            drawManager.line(pt2, pt2_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawManager.line(pt4_1, pt2_1);
        drawManager.endDrawable();

        { // right
            midPointX = yPos * (0.5f - endPointOffset) + yPos2 * (0.5f + endPointOffset);
            midPointY = yPos * (0.5f + endPointOffset) + yPos2 * (0.5f - endPointOffset);

            frameContext.viewportToWorld(xPos2, midPointX, pt3_1, tempPt);
            pt3_1 = (pt3_1 + tempPt) * 0.5f;
            frameContext.viewportToWorld(xPos2, midPointY, pt3_2, tempPt);
            pt3_2 = (pt3_2 + tempPt) * 0.5f;

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fRightName);
            drawManager.setLineWidth(3);
            setHandleColor(drawManager, fRightName, defaultCol);
            drawManager.line(pt3_1, pt3_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawManager.line(pt3_1, pt2_2);
        drawManager.endDrawable();

        { // upper right corner
            midPointX = xPos2 * (1.0f - endPointOffset) + xPos * endPointOffset;
            midPointY = yPos * (1.0f - endPointOffset) + yPos2 * endPointOffset;
            frameContext.viewportToWorld(midPointX, yPos, pt4_1, tempPt);
            pt4_1 = (pt4_1 + tempPt) * 0.5f;
            frameContext.viewportToWorld(xPos2, midPointY, pt4_2, tempPt);
            pt4_2 = (pt4_2 + tempPt) * 0.5f;

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fTopRightName);
            drawManager.setLineWidth(3);
            setHandleColor(drawManager, fTopRightName, defaultCol);
            drawManager.line(pt4, pt4_1);
            drawManager.line(pt4, pt4_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawManager.line(pt3_2, pt4_2);
        drawManager.endDrawable();

        { // top
            midPointX = xPos * (0.5f - endPointOffset) + xPos2 * (0.5f + endPointOffset);
            midPointY = xPos * (0.5f + endPointOffset) + xPos2 * (0.5f - endPointOffset);

            frameContext.viewportToWorld(midPointX, yPos, pt3_1, tempPt);
            pt3_1 = (pt3_1 + tempPt) * 0.5f;
            frameContext.viewportToWorld(midPointY, yPos, pt3_2, tempPt);
            pt3_2 = (pt3_2 + tempPt) * 0.5f;

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fTopName);
            drawManager.setLineWidth(3);
            setHandleColor(drawManager, fTopName, defaultCol);
            drawManager.line(pt3_1, pt3_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawManager.line(pt4_1, pt3_1);
        drawManager.endDrawable();

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawManager.line(pt3_2, pt1_1);
        drawManager.endDrawable();
    }

    else
    {
        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawManager.setLineStyle(MHWRender::MUIDrawManager::kDashed);
        drawManager.line(pt1, pt3);
        drawManager.line(pt3, pt2);
        drawManager.line(pt2, pt4);
        drawManager.line(pt4, pt1);
        drawManager.endDrawable();
    }

}

MStatus ArnoldViewRegionManipulator::doMove(M3dView& view, bool& refresh)
{
    short xPos;
    short yPos;
    mousePosition(xPos, yPos);

    int width = view.portWidth();
    int height = view.portHeight();
    float x = (float)xPos / (float)width;
    float y = 1.0f - ((float)yPos / (float)height);
    static const float delta = 0.02f;

    fDrawAsMouseOver = true;
    if (x < mViewRectangle.x-delta || x > mViewRectangle.z+delta)
        fDrawAsMouseOver = false;
    else if (y < mViewRectangle.y-delta || y > mViewRectangle.w+delta)
        fDrawAsMouseOver = false;

    return MS::kSuccess;
}

// virtual
MStatus	ArnoldViewRegionManipulator::doPress(M3dView& view)
{
	// Reset the mousePoint information on a new press

    // Find the mouse point in local space
    MPoint localMousePoint;
    MVector localMouseDirection;
    if (MS::kFailure == mouseRay(localMousePoint, localMouseDirection))
        return MS::kFailure;

	fMousePoint = localMousePoint;
    mDragRectangle = mViewRectangle;
	//updateDragInformation();
	return MS::kSuccess;
}

// virtual
MStatus	ArnoldViewRegionManipulator::doDrag(M3dView& view)
{
    if (!CMayaScene::GetRenderSession())
        return MS::kFailure;

    MPoint localMousePoint;
    MVector localMouseDirection;
    if (MS::kFailure == mouseRay(localMousePoint, localMouseDirection))
        return MS::kFailure;

    short startX, startY;
    short currentX, currentY;
    view.worldToView(fMousePoint, startX, startY);
    view.worldToView(localMousePoint, currentX, currentY);

    float xDelta = float(currentX - startX) / (float)view.portWidth();
    float yDelta = float(currentY - startY) / (float)view.portHeight();
    const float minSize = 0.01f;

    if (glActiveName(fActiveName))
    {
        if (fActiveName == fAllName)
        {
            mViewRectangle.x = mDragRectangle.x + xDelta;
            mViewRectangle.y = mDragRectangle.y - yDelta;
            mViewRectangle.z = mDragRectangle.z + xDelta;
            mViewRectangle.w = mDragRectangle.w - yDelta;
        }
        if (fActiveName == fTopLeftName)
        {
            mViewRectangle.x = mDragRectangle.x + xDelta;
            mViewRectangle.y = mDragRectangle.y - yDelta;
            if (mViewRectangle.x > mViewRectangle.z - minSize)
                mViewRectangle.x = mViewRectangle.z - minSize;
            if (mViewRectangle.y > mViewRectangle.w - minSize)
                mViewRectangle.y = mViewRectangle.w - minSize;
        }
        if (fActiveName == fBottomRightName)
        {
            mViewRectangle.z = mDragRectangle.z + xDelta;
            mViewRectangle.w = mDragRectangle.w - yDelta;
            if (mViewRectangle.w < mViewRectangle.y + minSize)
                mViewRectangle.w = mViewRectangle.y + minSize;
            if (mViewRectangle.z < mViewRectangle.x + minSize)
                mViewRectangle.z = mViewRectangle.x + minSize;
        }
        if (fActiveName == fBottomLeftName)
        {
            mViewRectangle.x = mDragRectangle.x + xDelta;
            mViewRectangle.w = mDragRectangle.w - yDelta;
            if (mViewRectangle.w < mViewRectangle.y + minSize)
                mViewRectangle.w = mViewRectangle.y + minSize;
            if (mViewRectangle.x > mViewRectangle.z - minSize)
                mViewRectangle.x = mViewRectangle.z - minSize;
        }
        if (fActiveName == fTopRightName)
        {
            mViewRectangle.z = mDragRectangle.z + xDelta;
            mViewRectangle.y = mDragRectangle.y - yDelta;
            if (mViewRectangle.z < mViewRectangle.x + minSize)
                mViewRectangle.z = mViewRectangle.x + minSize;
            if (mViewRectangle.y > mViewRectangle.w - minSize)
                mViewRectangle.y = mViewRectangle.w - minSize;
        }
        if (fActiveName == fLeftName)
        {
            mViewRectangle.x = mDragRectangle.x + xDelta;
            if (mViewRectangle.x > mViewRectangle.z - minSize)
                mViewRectangle.x = mViewRectangle.z - minSize;
        }
        if (fActiveName == fRightName)
        {
            mViewRectangle.z = mDragRectangle.z + xDelta;
            if (mViewRectangle.z < mViewRectangle.x + minSize)
                mViewRectangle.z = mViewRectangle.x + minSize;
        }
        if (fActiveName == fTopName)
        {
            mViewRectangle.y = mDragRectangle.y - yDelta;
            if (mViewRectangle.y > mViewRectangle.w - minSize)
                mViewRectangle.y = mViewRectangle.w - minSize;
        }
        if (fActiveName == fBottomName)
        {
            mViewRectangle.w = mDragRectangle.w - yDelta;
            if (mViewRectangle.w < mViewRectangle.y + minSize)
                mViewRectangle.w = mViewRectangle.y + minSize;
        }
    }

	return MS::kSuccess;
}

// virtual
MStatus ArnoldViewRegionManipulator::doRelease(M3dView& view)
{    
    CRenderSession *renderSession = CMayaScene::GetRenderSession();
    CRenderOptions *renderOptions = (renderSession) ? renderSession->RenderOptions() : NULL;
	if (!renderSession->IsRegionCropped()) return MS::kSuccess;
    if (renderOptions == NULL) return MS::kFailure;

    renderSession->InterruptRender(true);
    int width = renderOptions->width();
    int height = renderOptions->height();

    renderOptions->SetRegion(int(mViewRectangle.x * width), int(mViewRectangle.z * width),
            int((1.f - mViewRectangle.w ) * height), int((1.f - mViewRectangle.y) * height)); // expected order is left, right, bottom, top
    
	renderSession->SetRenderViewOption(MString("Refresh Render"), MString("1"));
	return MS::kSuccess;
}

bool ArnoldViewRegionManipulator::shouldDraw(const MDagPath& cameraPath) const
{
	MStatus status;
	MFnCamera camera(cameraPath, &status);
	if (!status)
	{
		return false;
	}

    CRenderSession *renderSession = CMayaScene::GetRenderSession();
    CRenderOptions *renderOptions = (renderSession) ? renderSession->RenderOptions() : NULL;
    return (renderOptions) ? renderOptions->useRenderRegion() : false;
}

//
// Static methods
//
void* ArnoldViewRegionManipulator::creator()
{
	return new ArnoldViewRegionManipulator();
}

MStatus ArnoldViewRegionManipulator::initialize()
{
	// No-op
	return MS::kSuccess;
}

