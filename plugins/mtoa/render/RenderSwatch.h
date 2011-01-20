#ifndef RENDERSWATCH_H
#define RENDERSWATCH_H

#include "render/RenderSession.h"

#include <maya/MSwatchRenderBase.h> 
#include <maya/MString.h>

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
 * node called "swatch_hook" in that scene for it to apply the
 * shader too.
 */
 
class CRenderSwatchGenerator
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

private:
   /// Load an ASS file for the swatch.
   /// \param node the node to load the ass for.
   void LoadSwatchAssForNode( const MObject & node );
   /// Creates a simple sphere, light and camera.
   void SimpleSurfaceAss();
   /// Build the Arnold scene.
   /// \see LoadSwatchAssForNode
   /// \see SimpleSurfaceAss
   void BuildAss();
   /// Print an error and clear the swatch.
   /// \param msg the error message.
   void ErrorSwatch( const MString msg);
   /// Clear the swatch.
   void ClearSwatch();

private:
   CRenderSession * m_renderSession;
   int m_iteration;

};
      


#endif // RENDERSWATCH_H
