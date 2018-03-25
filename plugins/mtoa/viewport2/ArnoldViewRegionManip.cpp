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


#if MAYA_API_VERSION >= 201700

// Statics
MTypeId ArnoldViewRegionManipulator::id(0x81046); // TODO: Need a new ID for this
MString ArnoldViewRegionManipulator::registrantId("ArnoldViewRegionManipulatorPlugin");

//
// class implementation
//
ArnoldViewRegionManipulator::ArnoldViewRegionManipulator()
: MPxManipulatorNode()
, fInitialized(false)
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
, fNoneName(0)
, fMousePoint(MPoint())
, fDrawAsMouseOver(false)
{
    mViewRectangle = MFloatPoint(0.33f, 0.33f, 0.66f, 0.66f);
    // order here is left, top, right, bottom

    initializeInstance();
}

ArnoldViewRegionManipulator::~ArnoldViewRegionManipulator()
{
}

void ArnoldViewRegionManipulator::initializeInstance()
{
    if (!fInitialized)
    {

        // Eventually get the crop region value in the render session
        CRenderSession *renderSession = CMayaScene::GetRenderSession();
        CRenderOptions *renderOptions = (renderSession) ? renderSession->RenderOptions() : NULL;
        if (renderOptions && renderOptions->useRenderRegion())
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
            fInitialized = true;
        }
    }
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
    fNoneName = glPickableItem++;

    registerForMouseMove();
}

//virtual 
void ArnoldViewRegionManipulator::preDrawUI( const M3dView &view )
{
    initializeInstance();

	M3dView *viewPtr = const_cast<M3dView*>( &view );
	MDagPath dpath;
	viewPtr->getCamera(dpath);
     fDrawManip = shouldDraw(dpath)
        && view.renderOverrideName() == "arnoldViewOverride";
}

namespace {
    static const bool kDrawScreenSpace = false;

    void lerp(MPoint& pt1, const MPoint& pt2, float offset)
    {
        pt1 = pt1 * (1.0f - offset) + pt2 * offset;
    }

    void transformPoint(const MHWRender::MFrameContext& frameContext, float x, float y, MPoint& pt)
    {
        static const float kPointDepth = 0.5f;

        if (kDrawScreenSpace)
        {
            pt.x = x;
            pt.y = y;
            pt.z = kPointDepth;
        }
        else
        {
            MPoint tempPt;
            frameContext.viewportToWorld(x, y, pt, tempPt);
            lerp(pt, tempPt, kPointDepth);
        }
    }

    void drawLine(MHWRender::MUIDrawManager& drawManager, const MPoint& pt1, const MPoint& pt2)
    {
        if (kDrawScreenSpace)
            drawManager.line2d(pt1, pt2);
        else
            drawManager.line(pt1, pt2);
    }
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
    MPoint pt3, pt3_1, pt3_2;
    MPoint pt4, pt4_1, pt4_2;

    float xPos = mViewRectangle.x * width;
    float yPos = (1.f - mViewRectangle.y) * height;
    float xPos2 = mViewRectangle.z * width;
    float yPos2 = (1.f - mViewRectangle.w) * height;

    float endPointOffset = 0.1f;

    float lineWidth = 3.f;

    float midPointX;
    float midPointY;
    float ptDepth = 0.5f;

    transformPoint(frameContext, xPos, yPos, pt1);
    transformPoint(frameContext, xPos2, yPos2, pt2);
    transformPoint(frameContext, xPos, yPos2, pt3);
    transformPoint(frameContext, xPos2, yPos, pt4);

    const short defaultCol = mainColor();

    if (fDrawAsMouseOver)
    {
        { // upper left corner
            midPointX = xPos * (1.0f - endPointOffset) + xPos2 * endPointOffset;
            midPointY = yPos * (1.0f - endPointOffset) + yPos2 * endPointOffset;
            transformPoint(frameContext, midPointX, yPos, pt1_1);
            transformPoint(frameContext, xPos, midPointY, pt1_2);

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fTopLeftName);
            drawManager.setLineWidth(lineWidth);
            setHandleColor(drawManager, fTopLeftName, defaultCol);
            drawLine(drawManager, pt1, pt1_1);
            drawLine(drawManager, pt1, pt1_2);
            drawManager.endDrawable();
        }

        { // left
            midPointX = yPos * (0.5f - endPointOffset) + yPos2 * (0.5f + endPointOffset);
            midPointY = yPos * (0.5f + endPointOffset) + yPos2 * (0.5f - endPointOffset);

            transformPoint(frameContext, xPos, midPointX, pt4_1);
            transformPoint(frameContext, xPos, midPointY, pt4_2);

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fLeftName);
            drawManager.setLineWidth(lineWidth);
            setHandleColor(drawManager, fLeftName, defaultCol);
            drawLine(drawManager, pt4_1, pt4_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawLine(drawManager, pt1_2, pt4_2);
        drawManager.endDrawable();

        { // lower left corner
            midPointX = xPos * (1.0f - endPointOffset) + xPos2 * endPointOffset;
            midPointY = yPos2 * (1.0f - endPointOffset) + yPos * endPointOffset;
            transformPoint(frameContext, midPointX, yPos2, pt3_1);
            transformPoint(frameContext, xPos, midPointY, pt3_2);

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fBottomLeftName);
            drawManager.setLineWidth(lineWidth);
            setHandleColor(drawManager, fBottomLeftName, defaultCol);
            drawLine(drawManager, pt3, pt3_1);
            drawLine(drawManager, pt3, pt3_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawLine(drawManager, pt4_1, pt3_2);
        drawManager.endDrawable();

        { // bottom
            midPointX = xPos * (0.5f - endPointOffset) + xPos2 * (0.5f + endPointOffset);
            midPointY = xPos * (0.5f + endPointOffset) + xPos2 * (0.5f - endPointOffset);

            transformPoint(frameContext, midPointX, yPos2, pt4_1);
            transformPoint(frameContext, midPointY, yPos2, pt4_2);

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fBottomName);
            drawManager.setLineWidth(lineWidth);
            setHandleColor(drawManager, fBottomName, defaultCol);
            drawLine(drawManager, pt4_1, pt4_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawLine(drawManager, pt4_2, pt3_1);
        drawManager.endDrawable();

        { // lower right corner
            midPointX = xPos2 * (1.0f - endPointOffset) + xPos * endPointOffset;
            midPointY = yPos2 * (1.0f - endPointOffset) + yPos * endPointOffset;
            transformPoint(frameContext, midPointX, yPos2, pt2_1);
            transformPoint(frameContext, xPos2, midPointY, pt2_2);

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fBottomRightName);
            drawManager.setLineWidth(lineWidth);
            setHandleColor(drawManager, fBottomRightName, defaultCol);
            drawLine(drawManager, pt2, pt2_1);
            drawLine(drawManager, pt2, pt2_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawLine(drawManager, pt4_1, pt2_1);
        drawManager.endDrawable();

        { // right
            midPointX = yPos * (0.5f - endPointOffset) + yPos2 * (0.5f + endPointOffset);
            midPointY = yPos * (0.5f + endPointOffset) + yPos2 * (0.5f - endPointOffset);

            transformPoint(frameContext, xPos2, midPointX, pt3_1);
            transformPoint(frameContext, xPos2, midPointY, pt3_2);

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fRightName);
            drawManager.setLineWidth(lineWidth);
            setHandleColor(drawManager, fRightName, defaultCol);
            drawLine(drawManager, pt3_1, pt3_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawLine(drawManager, pt3_1, pt2_2);
        drawManager.endDrawable();

        { // upper right corner
            midPointX = xPos2 * (1.0f - endPointOffset) + xPos * endPointOffset;
            midPointY = yPos * (1.0f - endPointOffset) + yPos2 * endPointOffset;
            transformPoint(frameContext, midPointX, yPos, pt4_1);
            transformPoint(frameContext, xPos2, midPointY, pt4_2);

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fTopRightName);
            drawManager.setLineWidth(lineWidth);
            setHandleColor(drawManager, fTopRightName, defaultCol);
            drawLine(drawManager, pt4, pt4_1);
            drawLine(drawManager, pt4, pt4_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawLine(drawManager, pt3_2, pt4_2);
        drawManager.endDrawable();

        { // top
            midPointX = xPos * (0.5f - endPointOffset) + xPos2 * (0.5f + endPointOffset);
            midPointY = xPos * (0.5f + endPointOffset) + xPos2 * (0.5f - endPointOffset);

            transformPoint(frameContext, midPointX, yPos, pt3_1);
            transformPoint(frameContext, midPointY, yPos, pt3_2);

            drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fTopName);
            drawManager.setLineWidth(lineWidth);
            setHandleColor(drawManager, fTopName, defaultCol);
            drawLine(drawManager, pt3_1, pt3_2);
            drawManager.endDrawable();
        }

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawLine(drawManager, pt4_1, pt3_1);
        drawManager.endDrawable();

        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fAllName);
        setHandleColor(drawManager, fAllName, defaultCol);
        drawLine(drawManager, pt3_2, pt1_1);
        drawManager.endDrawable();
    }
    else
    {
        drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fNoneName);
        setHandleColor(drawManager, fNoneName, defaultCol);
        drawManager.setLineStyle(MHWRender::MUIDrawManager::kDashed);
        drawLine(drawManager, pt1, pt3);
        drawLine(drawManager, pt3, pt2);
        drawLine(drawManager, pt2, pt4);
        drawLine(drawManager, pt4, pt1);
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

    renderOptions->SetRegion(AiClamp(int(mViewRectangle.x * width), 0, width -1), AiClamp(int(mViewRectangle.z * width), 0, width -1),
            AiClamp(int((1.f - mViewRectangle.w ) * height), 0, height - 1), AiClamp(int((1.f - mViewRectangle.y) * height), 0, height - 1)); // expected order is left, right, bottom, top
    
    if (!renderSession->IsIPRPaused())
        CMayaScene::GetArnoldSession()->RequestUpdate();
	   //renderSession->SetRenderViewOption(MString("Refresh Render"), MString("1"));

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

#endif
