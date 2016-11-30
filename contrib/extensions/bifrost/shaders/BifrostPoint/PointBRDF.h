#pragma once

#include <float.h>

#include "PointUtils.h"

// this implements a volume brdf for foam rendering

#define INV_FOURPI	0.0795774715459476f
#define INV_TWOPI	0.1591549430918953f
#define TWOPI		6.2831853071795862f

struct AnisotropyPreCalc {
	bool isotropic;
	float g;
	float twog;
	float g2;
	float onePlusg2;
	float oneMinusg2;
	float oneOver2g;
};

struct VolumeBRDFData {
	AtVector N;
	AtVector Ng;
	AtVector rayDirection;
	AtVector eye;

	AtVector U;
	AtVector V;

	bool dual;
	float blend;
	float blendInvert;
	AnisotropyPreCalc firstG;
	AnisotropyPreCalc secondG;
	AnisotropyPreCalc finalG;
};

void PreCalcAnisotropy( AnisotropyPreCalc &G, float g )
{
	// never go over -1 and 1 for g
	G.g = clamp( g, -0.9999f, 0.9999f );

	// set isotropy
	if ( fabs( G.g ) < 0.0001f ) {
		G.isotropic = true;
	} else {
		G.isotropic = false;
	}

	// set values
	if ( ! G.isotropic ) {
		G.g2 = G.g * G.g;
		G.twog = 2.0f * G.g;
		G.onePlusg2 = 1.0f + G.g2;
		G.oneMinusg2 = 1.0f - G.g2;
		G.oneOver2g = 0.5f / G.g;
	}
}

void CopyAnisotropyPreCalc( AnisotropyPreCalc from, AnisotropyPreCalc &to )
{
	to.isotropic = from.isotropic;
	to.g = from.g;
	to.twog = from.twog;
	to.g2 = from.g2;
	to.onePlusg2 = from.onePlusg2;
	to.oneMinusg2 = from.oneMinusg2;
	to.oneOver2g = from.oneOver2g;
}

void *VolumeBRDFSetupData ( const AtShaderGlobals *sg, const float firstG, const float secondG, const bool dual, const float blend )
{
	VolumeBRDFData *data = (VolumeBRDFData*) AiShaderGlobalsQuickAlloc(sg, sizeof(VolumeBRDFData));

	data->N = AiV3Normalize( sg->Nf );
	data->Ng = AiV3Normalize( sg->Ngf );
	data->eye = AiV3Normalize( -sg->Rd );
	data->rayDirection = sg->Rd;

	// do this outside of the BRDF calcs therefore only doing once per sample
	if ( !AiV3isZero( sg->dPdu ) && AiV3Exists( sg->dPdu ) ) {
		// we have valid a valid dPdu derivative, construct V
		AtVector Utmp = AiV3Normalize( sg->dPdu );
		data->V = AiV3Normalize( AiV3Cross( sg->N, Utmp ) );
		data->U = AiV3Cross( data->V, sg->N );
	} else {
		AiBuildLocalFramePolar( &(data->U), &(data->V), &(data->N) );
	}

	// precalc values
	PreCalcAnisotropy( data->firstG, firstG );
	PreCalcAnisotropy( data->secondG, secondG );

	if ( dual ) {
		// check blend and decide on dual or not
		if ( blend < FLT_EPSILON || blend > 0.9999999f ) {
			// we only need one phase function
			// set that
			data->dual = false;

			if ( blend < FLT_EPSILON ) {
				// we are using only the first phase function
				CopyAnisotropyPreCalc( data->firstG, data->finalG );
			}

			if ( blend > 0.9999999f ) {
				// we are using only the second phase function
				CopyAnisotropyPreCalc( data->secondG, data->finalG );
			}
		} else {
			data->dual = true;
		}
	} else {
		data->dual = false;
		CopyAnisotropyPreCalc( data->firstG, data->finalG );
	}

	data->blend = blend;
	data->blendInvert = 1.0f - blend;

	return data;
}

//
//
// HENYEY-GREENSTEIN PHASE FUNCTION

static AtVector HGSample( const AnisotropyPreCalc *G, const AtVector &U, const AtVector &V, const AtVector &N, float &rx, float &ry )
{
	// define phi
	const float phi = rx * AI_PITIMES2;
	float sinPhi, cosPhi;
	sincosf( phi, &sinPhi, &cosPhi );

	// define theta
	float sinTheta, cosTheta;
	AtVector scatterDir;
	if ( G->isotropic ) {
		// do uniform distribution using archimedes cyclinder hat theorem
		cosTheta = ry;
		//cosTheta = 2.0f * ry - 1.0f;  // this is for using cdf
		sinTheta = std::sqrt( std::max(0.0f, 1.0f - cosTheta * cosTheta) );
		scatterDir = AiVector( sinTheta * cosPhi, sinTheta * sinPhi, cosTheta );
	} else {
		// apply henyey
		float tmp = G->oneMinusg2 / ( 1.0f - G->g + G->twog * ry);
		cosTheta = G->oneOver2g * ( G->onePlusg2 - tmp * tmp ); // invertcdf
		sinTheta = std::sqrt( std::max(0.0f, 1.0f - cosTheta * cosTheta) );
		scatterDir = AiVector( sinTheta * cosPhi, sinTheta * sinPhi, cosTheta );
	}

	AiV3RotateToFrame( scatterDir, U, V, N );

	return scatterDir;
}

static AtVector HGVolumeBRDFEvalSample (const void *brdf_data, float rx, float ry)
{
	const VolumeBRDFData *data = (const VolumeBRDFData*)brdf_data;

	if ( data->dual ) {
		// eval both functions and interpolate
		AtVector vector1 = HGSample( &( data->firstG ), data->U, data->V, data->N, rx, ry );
		AtVector vector2 = HGSample( &( data->secondG ), data->U, data->V, data->N, rx, ry );

		return vector1 * data->blendInvert + vector2 * data->blend;
	} else {
		return HGSample( &( data->finalG ), data->U, data->V, data->N, rx, ry );
	}
}

static float HGPDF( const AnisotropyPreCalc *G, const AtVector &N, AtVector *inDirection )
{
	if ( G->isotropic ) {
		return INV_FOURPI;
	} else {
		AtVector direction = *inDirection;
		direction = AiV3Normalize( direction );

		float cosTheta = AiV3Dot( N, direction );

		return INV_FOURPI * ( G->oneMinusg2 ) / powf( G->onePlusg2 - G->twog * cosTheta, 1.5f );
	}
}

static float HGVolumeBRDFEvalPDF( const void *brdf_data, AtVector *inDirection )
{
	const VolumeBRDFData *data = (const VolumeBRDFData*) brdf_data;

	if ( data->dual ) {
		// eval both functions and interpolate
		float pdf1 = HGPDF( &( data->firstG ), data->N, inDirection );
		float pdf2 = HGPDF( &( data->secondG ), data->N, inDirection );

		return pdf1 * data->blendInvert + pdf2 * data->blend;
	} else {
		return HGPDF( &( data->finalG ), data->N, inDirection );
	}
}

static AtColor HGVolumeBRDFEvalBRDF( const void *brdf_data, AtVector *inDirection )
{
	const VolumeBRDFData *data = (const VolumeBRDFData*)brdf_data;

	float pdf = HGVolumeBRDFEvalPDF ( brdf_data, inDirection );

	return pdf * AI_RGB_WHITE;
};


//
//
// SCHLICK PHASE FUNCTION

static AtVector SchlickSample( const AnisotropyPreCalc *G, const AtVector &U, const AtVector &V, const AtVector &N, float &rx, float &ry )
{
	// define phi
	const float phi = rx * AI_PITIMES2;
	float sinPhi, cosPhi;
	sincosf( phi, &sinPhi, &cosPhi );

	// define theta
	float sinTheta, cosTheta;
	AtVector scatterDir;
	if ( G->isotropic ) {
		cosTheta = ry; // do uniform distribution using archimedes cyclinder hat theorem
		//cosTheta = 2.0f * ry - 1.0f;  // this is for using cdf
		sinTheta = std::sqrt( std::max(0.0f, 1.0f - cosTheta * cosTheta) );
		scatterDir = AiVector( sinTheta * cosPhi, sinTheta * sinPhi, cosTheta );
	} else {
		// apply schlick
		float tmp = 2.0f * ry - 1.0f;
		cosTheta = ( tmp + G->g ) / ( G->g * tmp + 1.0f ); // invertcdf
		sinTheta = std::sqrt( std::max(0.0f, 1.0f - cosTheta * cosTheta) );
		scatterDir = AiVector( sinTheta * cosPhi, sinTheta * sinPhi, cosTheta );
	}

	AiV3RotateToFrame( scatterDir, U, V, N );

	return scatterDir;
}

static AtVector SchlickVolumeBRDFEvalSample (const void *brdf_data, float rx, float ry)
{
	const VolumeBRDFData *data = (const VolumeBRDFData*)brdf_data;

	if ( data->dual ) {
		// eval both functions and interpolate
		AtVector vector1 = SchlickSample( &( data->firstG ), data->U, data->V, data->N, rx, ry );
		AtVector vector2 = SchlickSample( &( data->secondG ), data->U, data->V, data->N, rx, ry );

		return vector1 * data->blendInvert + vector2 * data->blend;
	} else {
		return SchlickSample( &( data->finalG ), data->U, data->V, data->N, rx, ry );
	}
}

static float SchlickPDF( const AnisotropyPreCalc *G, const AtVector &N, AtVector *inDirection )
{
	if ( G->isotropic ) {
		return INV_FOURPI;
	} else {
		AtVector direction = *inDirection;
		direction = AiV3Normalize( direction );

		float cosTheta = AiV3Dot( N, direction );
		float tmp = 1.0f - G->g * cosTheta;

		return INV_FOURPI * ( 1.0f - G->g2 ) / ( tmp * tmp );
	}
}

static float SchlickVolumeBRDFEvalPDF( const void *brdf_data, AtVector *inDirection )
{
	const VolumeBRDFData *data = (const VolumeBRDFData*) brdf_data;

	if ( data->dual ) {
		// eval both functions and interpolate
		float pdf1 = SchlickPDF( &( data->firstG ), data->N, inDirection );
		float pdf2 = SchlickPDF( &( data->secondG ), data->N, inDirection );

		return pdf1 * data->blendInvert + pdf2 * data->blend;
	} else {
		return SchlickPDF( &( data->finalG ), data->N, inDirection );
	}
}

static AtColor SchlickVolumeBRDFEvalBRDF (const void *brdf_data, AtVector *inDirection)
{
	const VolumeBRDFData *data = (const VolumeBRDFData*)brdf_data;

	float pdf = SchlickVolumeBRDFEvalPDF ( brdf_data, inDirection );

	return pdf * AI_RGB_WHITE;
};
