//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

//////////////////////////////////////////////////////
//DESCRIPTION:
//
// Produces the MEL command "ArnoldViewportRegionToolContext".
//
// This plug-in demonstrates "ArnoldViewportRegion selection" in a user defined context. It is supported in Viewport 2.0 and OpenGL.
// Selection is done through the API (MGlobal).
//
// This is another context example, except that this example
// does not have an associated command. 
//
// To use it, you must execute the command "source ArnoldViewportRegionTool" in the command window.
// This will create a new entry in the "Shelf1" tab of the tool shelf called "Marquee Tool".
// When this tool is active, you can select objects in the 3D windows in the same way that
// you do with the selection tool (it can be used for either click selection or drag selection).
// Both will also work with the shift key held down in the same manner as the selection tool. 
//
//////////////////////////////////////////////////////

#include "ArnoldViewportRegionTool.h"

#include "session/ArnoldRenderViewSession.h"
#include "session/SessionManager.h"

#include <maya/MIOStream.h>
#include <math.h>
#include <stdlib.h>

#include <maya/MString.h>
#include <maya/MDagPath.h>
 
#include <maya/MItSelectionList.h>
#include <maya/MSelectionList.h>

#include <maya/MUIDrawManager.h>
#include <maya/MFrameContext.h>
#include <maya/MPoint.h>
#include <maya/MColor.h>
#include <maya/MCursor.h>


//////////////////////////////////////////////
// Custom XOR Draw Class
//////////////////////////////////////////////

//	Set this to 1 if you want to use the 'xorDraw' class for
//	customized XOR drawing rather than M3dView's methods.
#define CUSTOM_XOR_DRAW 0

#if CUSTOM_XOR_DRAW
// Example class code which basically does the same operations
// as M3dView's beingXorDrawing() and endXorDrawing() methods.
// This class could be used in place these methods if desired.
//
class xorDraw
{
public:
	xorDraw(M3dView	*view) { fView = view; };
	~xorDraw() {};

	void beginXorDrawing();
	void endXorDrawing();

protected:
	xorDraw() { fView = NULL; }

	// Setup for XOR drawing
	GLboolean depthTest[1];
	GLboolean colorLogicOp[1];
	GLboolean lineStipple[1];

	M3dView *fView;


};

void xorDraw::beginXorDrawing()
{
	// Save the state of these 3 attribtes and restore them later.
	glGetBooleanv (GL_DEPTH_TEST, depthTest);
	glGetBooleanv (GL_COLOR_LOGIC_OP, colorLogicOp);
	glGetBooleanv (GL_LINE_STIPPLE, lineStipple);

	glDrawBuffer( GL_FRONT );

	// Turn Line stippling on.
	glLineStipple( 1, 0x5555 );
	glLineWidth( 1.0 );
	glEnable( GL_LINE_STIPPLE );

	// Save the state of the matrix on stack
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix();

	// Setup the Orthographic projection Matrix.
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
    gluOrtho2D(
    			0.0, (GLdouble) fView->portWidth(),
    			0.0, (GLdouble) fView->portHeight()
    );
    glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
    glTranslatef(0.375, 0.375, 0.0);

	// Set the draw color
	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw the ArnoldViewportRegion in XOR mode
	
	glDisable (GL_DEPTH_TEST);

	// Enable XOR mode.
	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp (GL_XOR);
}

void xorDraw::endXorDrawing()
{
	glFlush();
	glDrawBuffer( GL_BACK );

	// Restore the state of the matrix from stack
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	// Restore the previous state of these attributes
	if (colorLogicOp[0])
		glEnable (GL_COLOR_LOGIC_OP);
	else
		glDisable (GL_COLOR_LOGIC_OP);

	if (depthTest[0])
		glEnable (GL_DEPTH_TEST);
	else
		glDisable (GL_DEPTH_TEST);

	if (lineStipple[0])
		glEnable( GL_LINE_STIPPLE );
	else
		glDisable( GL_LINE_STIPPLE );
}
#endif


//////////////////////////////////////////////
// The user Context
//////////////////////////////////////////////
const char helpString[] =
			"Click with left button or drag with middle button to select";


static std::string s_arnoldViewportSession("arnoldViewport");

ArnoldViewportRegionContext::ArnoldViewportRegionContext()
{
	setTitleString ( "Arnold Viewport Region Tool" );
	setCursor( MCursor::crossHairCursor );
	// Tell the context which XPM to use so the tool can properly
	// be a candidate for the 6th position on the toolbar.
	setImage("ArnoldViewportRegionTool.png", MPxContext::kImage1 );

}

void ArnoldViewportRegionContext::toolOnSetup ( MEvent & )
{
	setHelpString( helpString );
    // MGlobal::executeCommandOnIdle("aiViewRegionCmd -delete;aiViewRegionCmd -create;arnoldViewOverrideOptionBox -opt \"Crop Region\" \"1\"");
}

void ArnoldViewportRegionContext::doPressCommon( MEvent & event )
{
	// Figure out which modifier keys were pressed, and set up the
	// listAdjustment parameter to reflect what to do with the selected points.
	if (event.isModifierShift() || event.isModifierControl() ) {
		if ( event.isModifierShift() ) {
			if ( event.isModifierControl() ) {
				// both shift and control pressed, merge new selections
				listAdjustment = MGlobal::kAddToList;
			} else {
				// shift only, xor new selections with previous ones
				listAdjustment = MGlobal::kXORWithList;
			}
		} else if ( event.isModifierControl() ) {
			// control only, remove new selections from the previous list
			listAdjustment = MGlobal::kRemoveFromList; 
		}
	} else {
		listAdjustment = MGlobal::kReplaceList;
	}
	MGlobal::executeCommandOnIdle("aiViewRegionCmd -delete;");

	// Extract the event information
	//
	event.getPosition( start_x, start_y );
}

void ArnoldViewportRegionContext::doReleaseCommon( MEvent & event )
{
	MSelectionList			incomingList, ArnoldViewportRegionList;

	// Get the end position of the ArnoldViewportRegion
	event.getPosition( last_x, last_y );

	// Save the state of the current selections.  The "selectFromSceen"
	// below will alter the active list, and we have to be able to put
	// it back.
	MGlobal::getActiveSelectionList(incomingList);

	// If we have a zero dimension box, just do a fixed width height region
	//
	if ( abs(start_x - last_x) < 2 && abs(start_y - last_y) < 2 ) {
		// This will check to see if the active view is in wireframe or not.
		// MGlobal::SelectionMethod selectionMethod = MGlobal::selectionMethod();

		// MGlobal::selectFromScreen( start_x, start_y, MGlobal::kReplaceList, selectionMethod );
	} else {
		// The Maya select tool goes to wireframe select when doing a ArnoldViewportRegion, so
		// we will copy that behaviour.
		// Select all the objects or components within the ArnoldViewportRegion.
		// MGlobal::selectFromScreen( start_x, start_y, last_x, last_y,
		// 	MGlobal::kReplaceList, 
		// 	MGlobal::kWireframeSelectMethod );
		CArnoldRenderViewSession *session = (CArnoldRenderViewSession *)CSessionManager::FindActiveSession(s_arnoldViewportSession);
        if (session)
        {  
			CSessionOptions &sessionOptions = session->GetOptions();
			session->GetRenderView().InterruptRender(true);
			int width, height;
			if (sessionOptions.GetResolution(width, height))
			{
				int left, right, top, bottom;

				left = (last_x > start_x) ? start_x : last_x;
				right = (last_x > start_x) ? last_x : start_x;
				bottom = (last_y > start_y) ? start_y : last_y;
				top = (last_y > start_y) ? last_y : start_y;

				MFnDependencyNode fnArnoldRenderOptions(sessionOptions.GetArnoldRenderOptions());
				MPlug avpRegionLeft = fnArnoldRenderOptions.findPlug("avpRegionLeft", true);
				MPlug avpRegionRight = fnArnoldRenderOptions.findPlug("avpRegionRight", true);
				MPlug avpRegionTop = fnArnoldRenderOptions.findPlug("avpRegionTop", true);
				MPlug avpRegionBottom = fnArnoldRenderOptions.findPlug("avpRegionBottom", true);

				avpRegionLeft.setInt(left);
				avpRegionRight.setInt(right);
				avpRegionTop.setInt(top);
				avpRegionBottom.setInt(bottom);
				
				sessionOptions.SetRegion(left, right, bottom, top); // expected order is left, right, bottom, top
         		MGlobal::executeCommandOnIdle("aiViewRegionCmd -create;arnoldViewOverrideOptionBox -opt \"Crop Region\" \"1\"");
				MString arvRunIpr = session->GetRenderViewOption(MString("Run IPR"));
				if (arvRunIpr != MString("0"))
				{
					session->RequestUpdate();
				}
				else
				{
					session->SetRenderViewOption("Run IPR", "1");
				}

			}
			    
		}
	}

}

MStatus ArnoldViewportRegionContext::doPress( MEvent & event )
//
// Begin ArnoldViewportRegion drawing (using OpenGL)
// Get the start position of the ArnoldViewportRegion 
//
{
	doPressCommon( event );

	view = M3dView::active3dView();
	fsDrawn = false;

	return MS::kSuccess;
}

MStatus ArnoldViewportRegionContext::doDrag( MEvent & event )
//
// Drag out the ArnoldViewportRegion (using OpenGL)
//
{
#if CUSTOM_XOR_DRAW
	xorDraw XORdraw(&view);
	XORdraw.beginXorDrawing();
#else
	view.beginXorDrawing();
#endif

	if (fsDrawn) {
		// Redraw the ArnoldViewportRegion at its old position to erase it.
		drawMarqueeGL();
	}

	fsDrawn = true;

	//	Get the ArnoldViewportRegion's new end position.
	event.getPosition( last_x, last_y );

	// Draw the ArnoldViewportRegion at its new position.
	drawMarqueeGL();

#if CUSTOM_XOR_DRAW
	XORdraw.endXorDrawing();
#else
	view.endXorDrawing();
#endif

	return MS::kSuccess;		
}

MStatus ArnoldViewportRegionContext::doRelease( MEvent & event )
//
// Selects objects within the ArnoldViewportRegion box.
{
	if (fsDrawn) {
#if CUSTOM_XOR_DRAW
		xorDraw XORdraw(&view);
		XORdraw.beginXorDrawing();
#else
		view.beginXorDrawing();
#endif
		// Redraw the ArnoldViewportRegion at its old position to erase it.
		drawMarqueeGL();

#if CUSTOM_XOR_DRAW
		XORdraw.endXorDrawing();
#else
		view.endXorDrawing();
#endif
	}

	doReleaseCommon( event );

	return MS::kSuccess;
}

MStatus	ArnoldViewportRegionContext::doPress (
	MEvent & event,
	MHWRender::MUIDrawManager& drawMgr,
	const MHWRender::MFrameContext& context)
{
	doPressCommon( event );
	return MS::kSuccess;
}

MStatus	ArnoldViewportRegionContext::doRelease(
	MEvent & event,
	MHWRender::MUIDrawManager& drawMgr,
	const MHWRender::MFrameContext& context)
{
	doReleaseCommon( event );
	return MS::kSuccess;
}

MStatus	ArnoldViewportRegionContext::doDrag (
	MEvent & event,
	MHWRender::MUIDrawManager& drawMgr,
	const MHWRender::MFrameContext& context)
{
	//	Get the ArnoldViewportRegion's new end position.
	event.getPosition( last_x, last_y );

	// Draw the ArnoldViewportRegion at its new position.
	drawMgr.beginDrawable();
	drawMgr.setColor( MColor(1.0f, 0.2f, 0.0f) );
	drawMgr.line2d( MPoint( start_x, start_y), MPoint(last_x, start_y) );
	drawMgr.line2d( MPoint( last_x, start_y), MPoint(last_x, last_y) );
	drawMgr.line2d( MPoint( last_x, last_y), MPoint(start_x, last_y) );
	drawMgr.line2d( MPoint( start_x, last_y), MPoint(start_x, start_y) );

	// double len = (last_y - start_y) * (last_y - start_y) + 
	// 	(last_x - start_x) * (last_x - start_x) * 0.01;
	// drawMgr.line(MPoint(0,0,0), MPoint(len, len, len));
	drawMgr.endDrawable();

	return MS::kSuccess;
}

MStatus ArnoldViewportRegionContext::doEnterRegion( MEvent & )
{
	return setHelpString( helpString );
}

void ArnoldViewportRegionContext::drawMarqueeGL()
{
	glBegin( GL_LINE_LOOP );
		glVertex2i( start_x, start_y );
		glVertex2i( last_x, start_y );
		glVertex2i( last_x, last_y );
		glVertex2i( start_x, last_y );
	glEnd();
}

// comand to create the context

ArnoldViewportRegionContextCmd::ArnoldViewportRegionContextCmd() {}

MPxContext* ArnoldViewportRegionContextCmd::makeObj()
{
	return new ArnoldViewportRegionContext();
}

void* ArnoldViewportRegionContextCmd::creator()
{
	return new ArnoldViewportRegionContextCmd;
}