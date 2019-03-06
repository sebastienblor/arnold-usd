#pragma once

#include "platform/Platform.h"
#include "render/RenderSession.h"

#include <maya/MSwatchRenderBase.h> 
#include <maya/MString.h>
#include <maya/MPlugArray.h>
#include <extension/AbMayaNode.h>

/** Render Swatches!
 * 
 * This class with generate swatches for any arnold node.
 * 
 * It will create a simple Arnold scene then export and attach
 * the shader in question.
 * It will look in $MTOA_SWATCH_ASS_PATH for a ASS file matching
 * the name of the shader classification. A shader with the
 * classification shader/surface will cause it to look for
 * $MTOA_SWATCH_ASS_PATH/shader_surface.ass. There must be a
 * node called "swatch_geo" in that scene for it to apply the
 * shader too.
 */

typedef enum
{
   SWATCH_NONE,
   SWATCH_SHADER,
   SWATCH_ENVIRONMENT,
   SWATCH_ATMOSPHERE,
   SWATCH_LIGHT,
   SWATCH_LIGHTFILTER,
   SWATCH_NOGPU
} CRenderSwatchClass;


class CNodeTranslator;

class DLLEXPORT CRenderSwatchGenerator
   :  public MSwatchRenderBase
{

public:
   CRenderSwatchGenerator(MObject dependNode,
                          MObject renderNode,
                          int imageResolution);
   ~CRenderSwatchGenerator();

   static MSwatchRenderBase* creator(MObject dependNode,
                                     MObject renderNode,
                                     int imageResolution);

   virtual bool doIteration();
   static CAbMayaNode s_abstract;

private:
   /// Extracts the swatch class from the previwed
   /// node classification
   void SetSwatchClass(const MObject & swatchNode);
   /// Build the Arnold scene.
   /// \see LoadAssForNode
   /// \see DefaultArnoldScene
   /// \see ExportNode
   /// \see AssignNode
   /// \see ApplyOverrides
   MStatus BuildArnoldScene();
   /// Load an ASS file for the swatch.
   /// \see BuildArnoldScene
   MStatus LoadAssForNode();
   /// Creates a simple Arnold scene (sphere by default)
   /// \see BuildArnoldScene
   MStatus DefaultArnoldScene();
   /// Export previewed node and apply specific overrides
   /// on the built Arnold Scene.
   /// \see BuildArnoldScene
   MStatus ExportNode(AtNode* & arnoldNode, CNodeTranslator* & translator);
   /// Assign the exported node to the Arnold scene.
   /// \see BuildArnoldScene
   /// \see ExportSwatchNode
   MStatus AssignNode(AtNode* const arnoldNode, CNodeTranslator* const translator);
   /// Apply specified overrides on the Arnold scene.
   /// \see BuildArnoldScene
   MStatus ApplyOverrides(CNodeTranslator* const translator);

   /// Print an error and clear the swatch.
   /// \param msg the error message.
   void ErrorSwatch(const MString msg);
   /// Clear the swatch.
   void ClearSwatch();
   /// Creates a polygon sphere.
   AtNode* PolySphere();

   // MStatus GetArnoldNode (MObject node, AtNode* & arnoldNode);
   bool DoSwatchRender();
   
   bool DoNoGPUImage();

   bool DoStaticImage() { return true ; } // Is a dummy for now


private:
   int m_iteration;
   MString m_nodeClass;
   CRenderSwatchClass m_swatchClass;


};
