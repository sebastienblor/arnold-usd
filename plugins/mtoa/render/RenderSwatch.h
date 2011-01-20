#ifndef RENDERSWATCH_H
#define RENDERSWATCH_H

#include "render/RenderSession.h"

#include <maya/MSwatchRenderBase.h> 
#include <maya/MString.h>

class CRenderSwatchGenerator
   :  public MSwatchRenderBase
{

public:
   CRenderSwatchGenerator(MObject dependNode, MObject renderNode, int imageResolution);
   ~CRenderSwatchGenerator();

   static MSwatchRenderBase* creator(MObject dependNode, MObject renderNode, int imageResolution);

   virtual bool doIteration();

private:
   void LoadSwatchAssForNode( const MObject & node );
   void SimpleSurfaceAss();
   void BuildAss();
   void ErrorSwatch( const MString msg);
   void ClearSwatch();

private:
   CRenderSession * m_renderSession;
   CMayaScene * m_maya_scene;
   bool m_error;

   int m_iteration;

};
      


#endif // RENDERSWATCH_H
