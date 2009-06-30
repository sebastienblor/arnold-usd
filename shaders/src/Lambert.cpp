
#include <ai_shaders.h>
#include <ai_shaderglobals.h>

AI_SHADER_NODE_EXPORT_METHODS( MayaLambertShaderMtd );

node_parameters
{

   AiParameterFLT( "diffuseCoef", 1 );
   AiParameterRGB( "color", 1, 1, 1 );
   AiParameterRGB( "transparency", 0, 0, 0 );
   AiParameterRGB( "ambientColor", 0, 0, 0 );
   AiParameterRGB( "incandescense", 0, 0, 0 );
   AiParameterFLT( "translucenceCoef", 1 );
   AiParameterFLT( "glowIntensity", 1 );
   AiParameterBOOL( "hideSource", FALSE );

}  // node_parameters


node_initialize
{
}  // node_initialize


node_finish
{
}  // node_finish


shader_evaluate
{

   AiColorCreate( sg->out.RGB, 1, 0, 0 );

}  // shader_evaluate
