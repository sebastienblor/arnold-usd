#include "compatibility.h"
#include <maya/MDGModifier.h>
#include <maya/MFileIO.h>
#include <maya/MDGMessage.h>
#include <maya/MGlobal.h>
#include "helpers.h"
#include <iostream>

//*****************************************************************************
// This file contains all the dirty compatibility and callback stuff
//
//*****************************************************************************

#define FOAM_VOLUME_ONLY  0
#define FOAM_SURFACE_ONLY 1
#define FOAM_BOTH         2

#define FOAM_DEFAULT FOAM_VOLUME_ONLY

namespace ArnoldBifrost{
namespace Compatibility{

// Workaround to replace old auto-assigned bifrost material with standard (surface/volume) arnold shaders
MCallbackId addedCbId = 0, connectionCbId = 0;

void removeCallback(MCallbackId& id)
{
   if(id != 0) MMessage::removeCallback(id);
   id = 0;
}
void bifrostShapeAttributeChanged(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void*)
{
   if(msg & MNodeMessage::kConnectionMade && MFnAttribute(plug.attribute()).name()=="instObjGroups" &&
         MFnAttribute(otherPlug.attribute()).name()=="dagSetMembers")
   {
      removeCallback(connectionCbId);
      // connection to shading engine made => replace shader
      int renderType = MFnDependencyNode(plug.node()).findPlug("bifrostRenderType", true).asInt();

      MPlug render_as = MFnDependencyNode(plug.node()).findPlug("render_as", true);
      std::cerr << "RENDER TYPE: " << renderType << std::endl;
      switch(renderType){
      case 0: render_as.setInt(2); break; // Aero => Volume
      case 3: render_as.setInt(1); break; // Foam => Points
      default: render_as.setInt(0); // Surface
      }

      MString command = "";
      if(renderType == 0)
      {
         // Aero
         MFnDependencyNode shadingGroup(otherPlug.node());
         MString oldShader = MFnDependencyNode(shadingGroup.findPlug("surfaceShader", true).source().node()).name();// oddly, even aero has a surfaceShader

         command = "undoInfo -openChunk; $sel = `selectedNodes`;"; // next line doesn't work with createNode -skipSelection...
         command += "string $oldShader = \""+oldShader+"\";string $newShader = `createNode aiStandardVolume`;replaceNode $oldShader $newShader;delete $oldShader;";
         // Aero => set density channel to smoke
         command += "string $densityChannelPlg = $newShader+\".densityChannel\"; setAttr $densityChannelPlg -type \"string\" \"smoke\"; ";
         // Reconnect shader and set presets
         command += "string $srcPlug = `connectionInfo -sfd \""+shadingGroup.name()+".surfaceShader\"`;disconnectAttr $srcPlug \""+shadingGroup.name()+".surfaceShader\"; connectAttr $srcPlug \""+shadingGroup.name()+".volumeShader\";";
         command += "string $presetPath = `getenv(\"MTOA_PATH\")`; $presetPath += \"presets/attrPresets/aiStandardVolume/Smoke.mel\"; applyPresetToNode $newShader \"\" \"\" $presetPath 1;";
         command += "select $sel;undoInfo -closeChunk;";
      }
      else if(renderType == 3)
      {
         // Foam
         MFnDependencyNode shadingGroup(otherPlug.node());
         
#if FOAM_DEFAULT == FOAM_SURFACE_ONLY || FOAM_DEFAULT == FOAM_BOTH
         MString oldSurfaceShader = MFnDependencyNode(shadingGroup.findPlug("surfaceShader", true).source().node()).name();

         command += "undoInfo -openChunk; $sel = `selectedNodes`;"; // next line doesn't work with createNode -skipSelection...
         command += "string $oldSurfShader = \""+oldSurfaceShader+"\";string $newSurfShader = `createNode aiStandardSurface`;replaceNode $oldSurfShader $newSurfShader;delete $oldSurfShader;";
         // Reconnect shader and set presets
         command += "string $srcPlug = `connectionInfo -sfd \""+shadingGroup.name()+".surfaceShader\"`;disconnectAttr $srcPlug \""+shadingGroup.name()+".surfaceShader\"; connectAttr $srcPlug \""+shadingGroup.name()+".surfaceShader\";";
         command += "string $presetPath = `getenv(\"MTOA_PATH\")`; $presetPath += \"presets/attrPresets/aiStandardSurface/Foam.mel\"; applyPresetToNode $newSurfShader \"\" \"\" $presetPath 1;";
         command += "select $sel;undoInfo -closeChunk;";
#endif

         // If we want both a surface shader and volume shader (for the interior), the procedural supports it and will take both
#if FOAM_DEFAULT == FOAM_VOLUME_ONLY || FOAM_DEFAULT == FOAM_BOTH
         MString oldVolumeShader = MFnDependencyNode(shadingGroup.findPlug("volumeShader", true).source().node()).name();
         command += "undoInfo -openChunk; $sel = `selectedNodes`;"; // next line doesn't work with createNode -skipSelection...
         command += "string $oldVolShader = \""+oldVolumeShader+"\";string $newVolShader = `createNode aiStandardVolume`;replaceNode $oldVolShader $newVolShader;delete $oldVolShader;";
         // Reconnect shader and set presets
         command += "string $srcPlug = `connectionInfo -sfd \""+shadingGroup.name()+".surfaceShader\"`;disconnectAttr $srcPlug \""+shadingGroup.name()+".surfaceShader\"; connectAttr $srcPlug \""+shadingGroup.name()+".volumeShader\";";
         command += "string $presetPath = `getenv(\"MTOA_PATH\")`; $presetPath += \"presets/attrPresets/aiStandardVolume/Foam.mel\"; applyPresetToNode $newVolShader \"\" \"\" $presetPath 1;";
         command += "select $sel;undoInfo -closeChunk;";
#endif
      }
      else
      {
          MFnDependencyNode shadingGroup(otherPlug.node());
          MString oldShader = MFnDependencyNode(shadingGroup.findPlug("surfaceShader", true).source().node()).name();// oddly, even aero has a surfaceShader

          command = "undoInfo -openChunk; $sel = `selectedNodes`;"; // next line doesn't work with createNode -skipSelection...
          command += "string $oldShader = \""+oldShader+"\";string $newShader = `createNode aiStandardSurface`;replaceNode $oldShader $newShader;delete $oldShader;";
          command += "string $presetPath = `getenv(\"MTOA_PATH\")`; $presetPath += \"presets/attrPresets/aiStandardSurface/Deep_Water.mel\"; applyPresetToNode $newShader \"\" \"\" $presetPath 1;";
          command += "select $sel;undoInfo -closeChunk;";
      }

      MGlobal::executeCommandOnIdle(command);
   }
}
void bifrostShapeAdded(MObject& obj, void*)
{
   removeCallback(connectionCbId);
   if(!MFileIO::isReadingFile() && !MGlobal::isUndoing())
   {  // && !MGlobal::isRedoing() => Temporary: Redoing bifrostShape creation is clearing redo stack anyway (which is wrong), so replace shader again...
      // must wait until shaging engine is connected to shape, otherwise shader assignment will be overridden by the old bifrost material
      // => registering temporary attribute change callback and removing it after material assignment
      connectionCbId = MNodeMessage::addAttributeChangedCallback(obj, bifrostShapeAttributeChanged);
   }
}

void NodeInitializer(CAbTranslator &context){
   if(MGlobal::apiVersion() <= 20180100){
      // in Maya 2018 Update 2+ these attributes are added to the shape by bifrostvisplugin
      // (since they aren't renderer specific)

      MAttrData extData(context.maya);

      extData.setup("renderAs", "render_as", std::initializer_list<const char*>{"Surface", "Points", "Volume"}).makeInput();
      extData.setup("renderComponent", "render_component", std::initializer_list<const char*>{"Voxels", "Particles"}).makeInput();

      extData.setup("velocityScale", "velocity_scale", 1.f).setSoft(0.f,1.f).makeInput();
      extData.setup("spaceScale",    "space_scale", 1.f).setSoft(0.f,1.f).makeInput();

      extData.setup("channels", "channels", MString("vorticity")).makeInput();

      // surface
      extData.setup("distanceChannel", "distance_channel", MString("distance")).makeInput();

      extData.setup("levelsetDropletRevealFactor",     "levelset_droplet_reveal_factor",        3.0f).setSoft(0.f,10.f).setMin(0.f).makeInput();
      extData.setup("levelsetSurfaceRadius",           "levelset_surface_radius",               1.4f).setSoft(0.f,10.f).setMin(0.f).makeInput();
      extData.setup("levelsetDropletRadius",           "levelset_droplet_radius",               1.2f).setSoft(0.f,10.f).setMin(0.f).makeInput();
      extData.setup("levelsetResolutionFactor",        "levelset_resolution_factor",            1.0f).setSoft(0.f,10.f).setMin(0.f).makeInput();
      extData.setup("levelsetMaxVolumeOfHolesToClose", "levelset_max_volume_of_holes_to_close", 0.0f).setSoft(0.f,10.f).setMin(0.f).makeInput();

      extData.setup("dilate",           "dilate",            0.0f).setSoft(0.f,1.f).makeInput();
      extData.setup("smooth",           "smooth",            0.0f).setSoft(0.f,1.f).setMin(0.f).makeInput();
      extData.setup("smoothMode",       "smooth_mode", std::initializer_list<const char*>{"LaplacianFlow", "CurvatureFlow"}).makeInput();
      extData.setup("smoothIterations", "smooth_iterations", 1   ).setSoft(1,10).setMin(1).makeInput();
      extData.setup("erode",            "erode", 0.0f).setSoft(0.f,1.f).makeInput();

      extData.setup("exportLaplacian", "export_laplacian", false).makeInput();
      extData.setup("exportCurvature", "export_curvature", false).makeInput();

      extData.setup("clip", "clip", false).makeInput();
      extData.makeInputMesh("clipBox", "clip_box");

      extData.setup("enableOceanBlending", "enable_ocean_blending", false).makeInput();
      extData.makeInputMesh("oceanPlane", "ocean_plane");
      extData.setup("oceanBlendingRadius",     "ocean_blending_radius",      0.f).setSoft(0.f,1.f).setMin(0.f).makeInput();
      extData.setup("oceanBlendingOffsets",    "ocean_blending_offsets",     AtVector(0,0,0)).makeInput();
      extData.setup("oceanBlendingOutChannel", "ocean_blending_out_channel", MString("ocean")).makeInput();

      // volume
      extData.setup("densityChannel",         "density_channel",          MString("smoke")).makeInput();
      extData.setup("volumeSmooth",           "volume_smooth",            0.0f).setSoft(0.f,1.f).setMin(0.f).makeInput();
      extData.setup("volumeSmoothIterations", "volume_smooth_iterations", 1   ).setSoft(1,10).setMin(1).makeInput();

      // points
      extData.setup("radius",              "radius", 0.01f).setSoft(0.f,1.f).setMin(0.f).makeInput();
      extData.setup("enableRadiusChannel", "enable_radius_channel", false).makeInput();
      extData.setup("radiusChannel",       "radius_channel", MString("density")).makeInput();
   }

   // add callback when a bifrost shape is created, so that we can assign the right material
   addedCbId = MDGMessage::addNodeAddedCallback(bifrostShapeAdded, "bifrostShape");
}

void Uninitialize(){
   removeCallback(addedCbId);
}

} // Compatibility
} // ArnoldBifrost
