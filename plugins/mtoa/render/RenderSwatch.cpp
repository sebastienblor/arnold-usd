#include "render/RenderSession.h"
#include "RenderSwatch.h"
#include <maya/MImage.h>
#include <ai_msg.h>
#include <string.h> // for memset.
#include <string>
#include <algorithm>

MSwatchRenderBase* CRenderSwatchGenerator::creator(MObject dependNode,
                                                  MObject renderNode,
                                                  int imageResolution)
{
   return new CRenderSwatchGenerator(dependNode, renderNode, imageResolution);
}

CRenderSwatchGenerator::CRenderSwatchGenerator(MObject dependNode,
                                             MObject renderNode,
                                             int imageResolution)
                                                : MSwatchRenderBase( dependNode,
                                                                    renderNode,
                                                                    imageResolution)
                                                , m_renderSession(0x0)
                                                , m_iteration(0)
{
}

CRenderSwatchGenerator::~CRenderSwatchGenerator()
{
}

void CRenderSwatchGenerator::BuildAss()
{
   m_renderSession = CRenderSession::GetInstance();
   m_renderSession->Init();
   m_renderSession->SetBatch(false);
   m_renderSession->SetProgressive(false);
   m_renderSession->GetMayaScene()->SetExportMode(MTOA_EXPORT_SWATCH);

   AiBegin();
   m_renderSession->LoadPlugins();
   MObject node = swatchNode();
   if ( node.isNull() ) ErrorSwatch( "Invalid swatch node.");
   LoadSwatchAssForNode( node );

   // Modify the loaded ass.

   MFnDependencyNode depfn( node );
   MString classification = MFnDependencyNode::classification(depfn.typeName());
   classification = classification.substringW(0, classification.indexW(':')-1 );

   // Export the shader.
   AtNode* shader = m_renderSession->GetMayaScene()->ExportShader( node, "" );
   if ( shader == NULL ) ErrorSwatch( "Could not export shader.");

   if ( classification == "light" )
   {
      AtNode* swatch_light = AiNodeLookUpByName( "swatch_light" );
      if ( swatch_light == NULL ) ErrorSwatch( "'swatch_light' node in Arnold not found.");
      AiNodeSetPtr(swatch_light, "shader", shader);
   }
   else
   {
      AtNode* swatch_hook = AiNodeLookUpByName( "swatch_hook" );
      if ( swatch_hook == NULL ) ErrorSwatch( "'swatch_hook' node in Arnold not found.");
      AiNodeSetPtr(swatch_hook, "shader", shader);
   }
}

void CRenderSwatchGenerator::ClearSwatch()
{
   const int num_pixels( resolution() * resolution() );
   image().create(resolution(), resolution(), 4, MImage::kByte);
   unsigned char * pixels = image().pixels();
   memset( pixels, 0, num_pixels * 4 * sizeof(char));
}

void CRenderSwatchGenerator::ErrorSwatch( const MString msg )
{
   const MString error_message( "[mtoa - swatch render] "+msg );
   MGlobal::displayError( error_message);
   AiMsgWarning( error_message.asChar() );
   ClearSwatch();
}

bool CRenderSwatchGenerator::doIteration()
{
   
   // Arnold is rendering, so bail out.
   // Return false to be called again.
   // This is how we manage to render many
   // swatches "at the same time".
   if (AiRendering()) return false;

   if ( m_iteration == 0)
   {
      // Arnold can only render one thing at a time.
      // It may be an option to block/wait here, but only
      // if it's another swatch render taking place.
      if (CRenderSession::GetInstance()->IsActive()) return false;

      BuildAss();
   }
   // Scene/ass is built, so start the render.
   else if ( m_iteration == 1)
   {
      if (!AiUniverseIsActive())
      {
         ErrorSwatch("Arnold universe not active.");
         return true; // Stop iterating/rendering.
      }

      m_renderSession->DoSwatchRender(resolution());
   }
   // We must be done rendering.
   else
   {
      if ( m_renderSession->GetSwatchImage( image() ) )
      {
         image().convertPixelFormat(MImage::kByte, 1.0f/255);
         m_renderSession->Finish();
         // Stop being called/iterated.
         return true;
      }
      else
      {
         // Start again as we were interupted.
         m_iteration = 0;
         return false;
      }
   }

   // Up the iteration count and return false so we're
   // called again.
   m_iteration++;
   return false;
}

void CRenderSwatchGenerator::SimpleSurfaceAss()
{
   // create a sphere geometric primitive
   AtNode *sph = AiNode("sphere");
   AiNodeSetStr(sph, "name", "swatch_hook");
   AiNodeSetBool( sph, "opaque", false);

   AtNode* light = AiNode("point_light");
   AiNodeSetStr(light, "name", "swatch_light");
   AiNodeSetPnt(light, "position", -5.f, 5.f, 5.f);

   AtNode* camera = AiNode("persp_camera");
   AiNodeSetStr(camera, "name", "swatch_camera");
   AiNodeSetPnt(camera, "position", 0.f, 0.f, 1.14f);

   // Add a sky shader to get solid alpha
   // TODO would be nice to expose the option to use an image for sky
   AtNode* sky = AiNode("sky");
   AiNodeSetStr(sky, "name", "swatch_sky");
   AiNodeSetRGB (sky, "color", 0.0f, 0.0f, 0.0f);
}

// This will try and find an ass file based on the classification
// of the shader. e.g. shader_surface.ass. It will try and find these
// ass files in a path pointed to by $MTOA_SWATCH_ASS_PATH.
void CRenderSwatchGenerator::LoadSwatchAssForNode( const MObject & node )
{
   MFnDependencyNode depfn( node );
   MString classification = MFnDependencyNode::classification(depfn.typeName());
   classification = classification.substringW(0, classification.indexW(':')-1 );

   const MString ass = MString("$MTOA_SWATCH_ASS_PATH").expandFilePath();
   std::string c_ass( classification.asChar() );
   std::replace( c_ass.begin(), c_ass.end(), '/', '_');
   c_ass = ass.asChar() + c_ass + ".ass";

   if ( m_renderSession->LoadAss(c_ass.c_str()) != AI_SUCCESS )
   {
      // We failed, so create a simple sphere + light.
      SimpleSurfaceAss();
   }
}


