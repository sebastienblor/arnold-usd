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
// The user Context
//////////////////////////////////////////////
const char helpString[] =
			"Click and drag with left button to create a new Arnold render region";


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
}

void ArnoldViewportRegionContext::doPressCommon( MEvent & event )
{
	// TODO multiple regions with modifiers

	// if (event.isModifierShift() || event.isModifierControl() ) {
	// 	if ( event.isModifierShift() ) {
	// 		if ( event.isModifierControl() ) {
	// 			// both shift and control pressed, merge new selections
	// 			listAdjustment = MGlobal::kAddToList;
	// 		} else {
	// 			// shift only, xor new selections with previous ones
	// 			listAdjustment = MGlobal::kXORWithList;
	// 		}
	// 	} else if ( event.isModifierControl() ) {
	// 		// control only, remove new selections from the previous list
	// 		listAdjustment = MGlobal::kRemoveFromList; 
	// 	}
	// } else {
	// 	listAdjustment = MGlobal::kReplaceList;
	// }

	// If click just delete the region
	MGlobal::executeCommandOnIdle("aiViewRegionCmd -delete;");

	// Extract the event information for the start position
	//
	event.getPosition( start_x, start_y );
}

void ArnoldViewportRegionContext::doReleaseCommon( MEvent & event )
{
	// Get the end position of the ArnoldViewportRegion
	event.getPosition( last_x, last_y );

	// Only do a region if it is larger than 2x2	//
	if ( abs(start_x - last_x) > 2 && abs(start_y - last_y) > 2 ) {
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

				// clamp the values to the screen res -1
				left = std::max(left, 1);
				right = std::min(right, width-1);
				bottom = std::max(bottom, 1);
				top = std::min(top, height-1);

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
	view.beginXorDrawing();

	if (fsDrawn) {
		// Redraw the ArnoldViewportRegion at its old position to erase it.
		drawMarqueeGL();
	}

	fsDrawn = true;

	//	Get the ArnoldViewportRegion's new end position.
	event.getPosition( last_x, last_y );

	// Draw the ArnoldViewportRegion at its new position.
	drawMarqueeGL();

	view.endXorDrawing();

	return MS::kSuccess;		
}

MStatus ArnoldViewportRegionContext::doRelease( MEvent & event )
//
// Selects objects within the ArnoldViewportRegion box.
{
	if (fsDrawn) {
		view.beginXorDrawing();
		// Redraw the ArnoldViewportRegion at its old position to erase it.
		drawMarqueeGL();

		view.endXorDrawing();
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
	drawMgr.setColor( MColor(0.8f, 0.8f, 0.2f) );
    drawMgr.setLineStyle(MHWRender::MUIDrawManager::kDashed);
	drawMgr.line2d( MPoint( start_x, start_y), MPoint(last_x, start_y) );
	drawMgr.line2d( MPoint( last_x, start_y), MPoint(last_x, last_y) );
	drawMgr.line2d( MPoint( last_x, last_y), MPoint(start_x, last_y) );
	drawMgr.line2d( MPoint( start_x, last_y), MPoint(start_x, start_y) );
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