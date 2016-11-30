#pragma once

#include "float.h"

enum OutputStandard
{
	OUT_ACESCG = 0,
	OUT_REC709,
	OUT_SRGB
};

static float XYZtoRGB[][9] = {
    // ACES cg
    {   1.64102338f, -0.32480329f, -0.2364247f, 
       -0.66366286f,  1.61533159f,  0.01675635f,
        0.01172189f, -0.00828444f,  0.98839486f
    },
    // Rec.709 D65
    {   3.24100326f, -1.53739899f, -0.49861587f,
       -0.96922426f,  1.87592999f,  0.04155422f,
        0.05563942f, -0.2040112f,   1.05714897f
    },
	// sRGB D65
    {   3.2404542f, -1.5371385f, -0.4985314f,
       -0.9692660f,  1.8760108f,  0.0415560f,
        0.0556434f, -0.2040259f,  1.0572252f
    }
};

AtRGB calcBlackBodyColor( 	const float colorTemp,
							const float physicalMix,
							const float exposure,
                            const OutputStandard outStd )
{

	// Planck Law samples using 5nm-spaced CIE color-matching (32 total).
	static const AtRGB planck[] = {
		AiColor(0.85597f, 0.14403f, 0.00000f),	// 0K
		AiColor(0.72457f, 0.27539f, 0.00004f),	// 500K
		AiColor(0.65283f, 0.34439f, 0.00278f),	// 1000K
		AiColor(0.58574f, 0.39311f, 0.02115f),	// 1500K
		AiColor(0.52669f, 0.41330f, 0.06002f),	// 2000K
		AiColor(0.47700f, 0.41368f, 0.10932f),	// 2500K
		AiColor(0.43694f, 0.40408f, 0.15898f),	// 3000K
		AiColor(0.40531f, 0.39073f, 0.20396f),	// 3500K
		AiColor(0.38045f, 0.37677f, 0.24278f),	// 4000K
		AiColor(0.36080f, 0.36357f, 0.27563f),	// 4500K
		AiColor(0.34512f, 0.35164f, 0.30325f),	// 5000K
		AiColor(0.33245f, 0.34107f, 0.32648f),	// 5500K
		AiColor(0.32210f, 0.33179f, 0.34611f),	// 6000K
		AiColor(0.31355f, 0.32367f, 0.36278f),	// 6500K
		AiColor(0.30639f, 0.31656f, 0.37705f),	// 7000K
		AiColor(0.30036f, 0.31032f, 0.38933f),	// 7500K
		AiColor(0.29521f, 0.30482f, 0.39997f),	// 8000K
		AiColor(0.29078f, 0.29996f, 0.40926f),	// 8500K
		AiColor(0.28695f, 0.29564f, 0.41741f),	// 9000K
		AiColor(0.28360f, 0.29179f, 0.42461f),	// 9500K
		AiColor(0.28066f, 0.28835f, 0.43099f),	// 10000K
		AiColor(0.27805f, 0.28525f, 0.43669f),	// 10500K
		AiColor(0.27574f, 0.28246f, 0.44180f),	// 11000K
		AiColor(0.27367f, 0.27993f, 0.44640f),	// 11500K
		AiColor(0.27181f, 0.27763f, 0.45056f),	// 12000K
		AiColor(0.27013f, 0.27554f, 0.45434f),	// 12500K
		AiColor(0.26861f, 0.27362f, 0.45778f),	// 13000K
		AiColor(0.26722f, 0.27186f, 0.46092f),	// 13500K
		AiColor(0.26596f, 0.27024f, 0.46381f),	// 14000K
		AiColor(0.26480f, 0.26874f, 0.46646f),	// 14500K
		AiColor(0.26373f, 0.26736f, 0.46891f),	// 15000K
		AiColor(0.26275f, 0.26607f, 0.47118f),	// 15500K
	};

	// Number of LUT entries
	static const int numKnots = sizeof( planck ) / sizeof( planck[0] );

	// linearly interpolate within the planck array
	static const float minTemp = 0.0f;
	static const float scaleFactor = 1.0f / ( 15000.0f - minTemp );
	float t = (colorTemp - minTemp) * scaleFactor;
	t = std::min<float>( std::max<float> ( t, 0.0f ), 1.0f );

	int startIndex = int( floor( t * ( numKnots - 1 ) ) );
	int endIndex = startIndex + 1;
	float interp = t * ( numKnots - 1 ) - startIndex;
	float mixA = 1.0f - interp;
	AtVector blendXYZ;
	blendXYZ[ 0 ] = planck[ startIndex ][ 0 ] * mixA + planck[ endIndex ][ 0 ] * interp;
	blendXYZ[ 1 ] = planck[ startIndex ][ 1 ] * mixA + planck[ endIndex ][ 1 ] * interp;
	blendXYZ[ 2 ] = planck[ startIndex ][ 2 ] * mixA + planck[ endIndex ][ 2 ] * interp;

	// normalize chromaticity
	// blendXYZ = AiV3Normalize( blendXYZ );

	// Convert to RGB
	AtRGB result;
	result.r = XYZtoRGB[ outStd ][ 0 ] * blendXYZ.x + XYZtoRGB[ outStd ][ 1 ] * blendXYZ.y + XYZtoRGB[ outStd ][ 2 ] * blendXYZ.z;
	result.g = XYZtoRGB[ outStd ][ 3 ] * blendXYZ.x + XYZtoRGB[ outStd ][ 4 ] * blendXYZ.y + XYZtoRGB[ outStd ][ 5 ] * blendXYZ.z;
	result.b = XYZtoRGB[ outStd ][ 6 ] * blendXYZ.x + XYZtoRGB[ outStd ][ 7 ] * blendXYZ.y + XYZtoRGB[ outStd ][ 8 ] * blendXYZ.z;

	// Clamp negative values
	result.r = std::max<float>( 0.0f, result.r );
	result.g = std::max<float>( 0.0f, result.g );
	result.b = std::max<float>( 0.0f, result.b );

	if ( physicalMix > 0.0f ) {
		float norm = result.r + result.g + result.b;
		if ( norm > 0.0f ) {
			// Normalise the color
			result /= norm;

			// calc emission intesity by Boltzman law
			static const double sb = 5.6703E-8;
			double intensity = std::pow( (double)colorTemp, 4.0) * sb;

			double intensityScale;
			if ( physicalMix <= 1.0f ) {
				intensityScale = ( 1.0f - physicalMix ) + (float) intensity * physicalMix;
			} else {
				intensityScale = intensity * physicalMix;
			}

			intensityScale *= std::pow( 2.0f, exposure );

			// check final values
			if ( intensityScale > FLT_MAX ) {
				intensityScale = FLT_MAX ;
			}

			result *= (float) intensityScale;
		}
	} else {
		result *= std::pow( 2.0f, exposure );
	}

	return result;
}
