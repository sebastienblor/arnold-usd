#include "BifrostTranslator.h"
#include "BifrostFoamMaterialTranslator.h"
#include "BifrostAeroMaterialTranslator.h"

#include "extension/Extension.h"
#include <maya/MTypes.h> 

#include <maya/MDGMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MFileIO.h>
#include <maya/MGlobal.h>

extern "C"
{
    namespace
    {
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
                // connection to shading engine made => replace shader
                int renderType = MFnDependencyNode(plug.node()).findPlug("bifrostRenderType").asInt();
                bool isVolume = renderType==0 || renderType==3; // Aero or Foam
                MString shaderType = isVolume? "aiStandardVolume" : "aiStandardSurface";

                MFnDependencyNode shadingGroup(otherPlug.node());

                MPlug surfaceShaderPlug = shadingGroup.findPlug("surfaceShader");

                MPlugArray plugArray;              
                surfaceShaderPlug.connectedTo(plugArray,  true, false);
                
                if (plugArray.length() > 0)
                {
                    MString oldShader = MFnDependencyNode(plugArray[0].node()).name();// oddly, even aero has a surfaceShader

                    MString command = "undoInfo -openChunk; $sel = `selectedNodes`;"; // next line doesn't work with createNode -skipSelection...
                    command += "string $oldShader = \""+oldShader+"\";string $newShader = `createNode "+shaderType+"`;replaceNode $oldShader $newShader;delete $oldShader;";
                    if(isVolume)
                    {
                        command += "string $srcPlug = `connectionInfo -sfd \""+shadingGroup.name()+".surfaceShader\"`;disconnectAttr $srcPlug \""+shadingGroup.name()+".surfaceShader\"; connectAttr $srcPlug \""+shadingGroup.name()+".volumeShader\";";
                    }
                    command += "select $sel;undoInfo -closeChunk;";
                    MGlobal::executeCommandOnIdle(command);
                }
                removeCallback(connectionCbId);
            }
        }
        void bifrostShapeAdded(MObject& obj, void*)
        {
            removeCallback(connectionCbId);
            if(!MFileIO::isReadingFile() && !MGlobal::isUndoing())
            { // && !MGlobal::isRedoing() => Temporary: Redoing bifrostShape creation is clearing redo stack anyway (which is wrong), so replace shader again...
                // must wait until shaging engine is connected to shape, otherwise shader assignment will be overridden by the old bifrost material
                // => registering temporary attribute change callback and removing it after material assignment
                connectionCbId = MNodeMessage::addAttributeChangedCallback(obj, bifrostShapeAttributeChanged);
            }
        }
    }

#ifdef ENABLE_BIFROST
    
    DLLEXPORT void initializeExtension ( CExtension& extension )
    {
        MStatus status;

#if MAYA_API_VERSION < 201650
        extension.Requires ( "BifrostMain" );
#else
        extension.Requires ( "bifrostvisplugin" );
#endif
        extension.LoadArnoldPlugin("bifrost_shaders");
        
        status = extension.RegisterTranslator ( "bifrostShape",
                                                "",
                                                CBfDescriptionTranslator::creator,
                                                CBfDescriptionTranslator::NodeInitializer );


        status = extension.RegisterTranslator ( "bifrostFoamMaterial",
                                                "",
                                                CBfFoamMaterialTranslator::creator);

        status = extension.RegisterTranslator ( "bifrostAeroMaterial",
                                                "",
                                                CBfAeroMaterialTranslator::creator,
                                                CBfAeroMaterialTranslator::NodeInitializer );

        addedCbId = MDGMessage::addNodeAddedCallback(bifrostShapeAdded, "bifrostShape");
       

    }

    DLLEXPORT void deinitializeExtension ( CExtension& extension )
    {
        removeCallback(addedCbId);
    }
#endif
}

