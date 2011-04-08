#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaLayeredTextureMtd);

namespace
{

enum MayaLayeredTextureParams
{
   p_color,
	p_alpha,
	p_blendMode,
	p_visible,
	p_colorConnectedToAlpha
};

};

node_parameters
{
	AiParameterARRAY("color", AiArrayAllocate(1, 1, AI_TYPE_NODE));
	AiParameterARRAY("alpha", AiArrayAllocate(1, 1, AI_TYPE_FLOAT));
	AiParameterARRAY("blendMode", AiArrayAllocate(1, 1, AI_TYPE_INT));
	AiParameterARRAY("visible", AiArrayAllocate(1, 1, AI_TYPE_BOOLEAN));
	AiParameterARRAY("colorConnectedToAlpha", AiArrayAllocate(1, 1, AI_TYPE_BOOLEAN));

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
}

node_initialize
{
}

node_update
{
}

node_finish
{
}

shader_evaluate
{
	AtArray *colorArray = AiNodeGetArray(node, "color");
	AtArray *alphaArray = AiNodeGetArray(node, "alpha");
	AtArray *blendModeArray = AiNodeGetArray(node, "blendMode");
	AtArray *visibleArray = AiNodeGetArray(node, "visible");
	AtArray *alphaConnectedArray = AiNodeGetArray(node, "colorConnectedToAlpha");

	AtRGBA outColor = AI_RGBA_BLACK;

	for (int i = colorArray->nelements-1; i >= 0; i-=1)
	{
		if (AiArrayGetBool(visibleArray, i) == 0)	// Disabled, skip
			continue;

		AtRGBA fc = AI_RGBA_BLACK;
		AtNode *inputNode = (AtNode *)AiArrayGetPtr(colorArray, i);
		AiShaderEvaluate(inputNode, sg);
		fc = sg->out.RGBA;

      float fa = AiArrayGetFlt(alphaArray, i);
		int blendMode = AiArrayGetInt(blendModeArray, i);

		// Multiply input alpha with the color's alpha value to support alpha texturing, until Arnold has a proper connectable array attribute
		if (AiArrayGetBool(alphaConnectedArray, i))
			fa *= fc.a;

		if ( blendMode == 0 )						 // None
			outColor = fc;
		else if ( blendMode == 1 )					 // Over
		{
			outColor.r = fc.r * fa + (outColor.r * (1.0f - fa));
			outColor.g = fc.g * fa + (outColor.g * (1.0f - fa));
			outColor.b = fc.b * fa + (outColor.b * (1.0f - fa));
         outColor.a = 1.0f - ((1.0f - outColor.a) * (1.0f - fa));
		}
		else if ( blendMode == 2 )					 // In
		{
			outColor.r *= fa;
			outColor.g *= fa;
			outColor.b *= fa;
         outColor.a *= fa;
		}
		else if ( blendMode == 3 )					 // Out
		{
			outColor.r *= (1.0f - fa);
			outColor.g *= (1.0f - fa);
			outColor.b *= (1.0f - fa);
         outColor.a *= (1.0f - fa);
		}
		else if ( blendMode == 4 )					 // Add
		{
			outColor.r += fc.r * fa;
			outColor.g += fc.g * fa;
			outColor.b += fc.b * fa;
		}
		else if ( blendMode == 5 )					 // Substract
		{
			outColor.r -= fc.r * fa;
			outColor.g -= fc.g * fa;
			outColor.b -= fc.b * fa;
		}
		else if ( blendMode == 6 )					 // Multiply
		{
			outColor.r *= (fc.r * fa + 1.0f - fa);
			outColor.g *= (fc.g * fa + 1.0f - fa);
			outColor.b *= (fc.b * fa + 1.0f - fa);
		}
      else if ( blendMode == 7 )					 // Difference
		{
         outColor.r = (fabs((fc.r * fa) - outColor.r)) * fa + outColor.r * (1.0f - fa);
			outColor.g = (fabs((fc.g * fa) - outColor.g)) * fa + outColor.g * (1.0f - fa);
			outColor.b = (fabs((fc.b * fa) - outColor.b)) * fa + outColor.b * (1.0f - fa);
      }
      else if ( blendMode == 8 )					 // Lighten
		{
         outColor.r = (MAX((fc.r * fa), outColor.r)) * fa + outColor.r * (1.0f - fa);
			outColor.g = (MAX((fc.g * fa), outColor.g)) * fa + outColor.g * (1.0f - fa);
			outColor.b = (MAX((fc.b * fa), outColor.b)) * fa + outColor.b * (1.0f - fa);
      }
      else if ( blendMode == 9 )					 // Darken
		{
         outColor.r = (MIN((fc.r * fa), outColor.r)) * fa + outColor.r * (1.0f - fa);
			outColor.g = (MIN((fc.g * fa), outColor.g)) * fa + outColor.g * (1.0f - fa);
			outColor.b = (MIN((fc.b * fa), outColor.b)) * fa + outColor.b * (1.0f - fa);
      }
		else if ( blendMode == 10 )				 // Saturate
		{
			outColor.r *= (1.0f + (fc.r * fa));
			outColor.g *= (1.0f + (fc.g * fa));
			outColor.b *= (1.0f + (fc.b * fa));
		}
		else if ( blendMode == 11 )				 // Desaturate
		{
			outColor.r *= (1.0f - (fc.r * fa));
			outColor.g *= (1.0f - (fc.g * fa));
			outColor.b *= (1.0f - (fc.b * fa));
		}
		else if ( blendMode == 12 )				 // Illuminate
		{
			outColor.r *= (2.0f * fc.r * fa + 1.0f - fa);
			outColor.g *= (2.0f * fc.g * fa + 1.0f - fa);
			outColor.b *= (2.0f * fc.b * fa + 1.0f - fa);
		}
	}
	sg->out.RGBA = outColor;
}
