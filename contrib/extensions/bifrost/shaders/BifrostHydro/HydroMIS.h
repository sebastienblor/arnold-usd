#pragma once

#include <cassert>

#include <ai.h>

struct BrdfDataWrap
{
	BrdfDataWrap() : brdfData(NULL), fr(NULL), krInt(AI_RGB_BLACK), ns(0.0f)
	{
	}

	void* brdfData;
	AtShaderGlobals* sg;
	Fresnel* fr;
	float eta;
	AtVector V;
	AtVector N;
	mutable AtRGB kr;
	mutable AtRGB krInt;
	mutable AtRGB brdf;
	mutable float pdf;
	mutable float ns;
	mutable bool ibs;
};

AtRGB AiMicrofacetMISBRDFWrap(const void* brdfData, const AtVector* indir)
{
	const BrdfDataWrap* brdfw = reinterpret_cast<const BrdfDataWrap*>(brdfData);
	AtRGB result = AI_RGB_BLACK;

	if (brdfw->ibs) {
		result = brdfw->kr * brdfw->brdf;
	} else {
		AtVector H;
		AiV3Normalize(H, (*indir) + brdfw->V);
		result = brdfw->fr->kr(std::max(0.0f, AiV3Dot(H, *indir))) * AiMicrofacetMISBRDF(brdfw->brdfData, indir);
	}

	return result;
}

float AiMicrofacetMISPDFWrap(const void* brdfData, const AtVector* indir)
{
	const BrdfDataWrap* brdfw = reinterpret_cast<const BrdfDataWrap*>(brdfData);
	if (brdfw->ibs) {
		return brdfw->pdf;
	} else {
		return AiMicrofacetMISPDF(brdfw->brdfData, indir);
	}
}

AtVector AiMicrofacetMISSampleWrap(const void* brdfData, float randx,
								   float randy)
{
	const BrdfDataWrap* brdfw = reinterpret_cast<const BrdfDataWrap*>(brdfData);
	brdfw->ibs = true;
	AtVector indir = AiMicrofacetMISSample(brdfw->brdfData, randx, randy);
	if (!AiV3IsZero(indir)) {
		AtVector H;
		AiV3Normalize(H, (indir) + brdfw->V);
		brdfw->kr = brdfw->fr->kr(std::max(0.0f, AiV3Dot(H, indir)));
		brdfw->brdf = AiMicrofacetMISBRDF(brdfw->brdfData, &indir);
		brdfw->pdf = AiMicrofacetMISPDF(brdfw->brdfData, &indir);
		if (brdfw->pdf > 0.0f) {
			AtRGB w = brdfw->brdf / brdfw->pdf;
			brdfw->krInt += brdfw->kr * w;
			assert(AiIsFinite(brdfw->krInt));
			brdfw->ns++;
		}
	}
	return indir;
}

AtRGB AiCookTorranceMISBRDFWrap(const void* brdfData, const AtVector* indir)
{
	const BrdfDataWrap* brdfw = reinterpret_cast<const BrdfDataWrap*>(brdfData);
	AtRGB result = AI_RGB_BLACK;
	if (brdfw->ibs) {
		result = brdfw->kr * brdfw->brdf;
	} else {
		AtVector H;
		AiV3Normalize(H, (*indir) + brdfw->V);
		result = brdfw->fr->kr(std::max(0.0f, AiV3Dot(H, *indir))) * AiCookTorranceMISBRDF(brdfw->brdfData, indir);
	}

	return result;
}

float AiCookTorranceMISPDFWrap(const void* brdfData, const AtVector* indir)
{
	const BrdfDataWrap* brdfw = reinterpret_cast<const BrdfDataWrap*>(brdfData);

	if (brdfw->ibs) {
		return brdfw->pdf;
	} else {
		return AiCookTorranceMISPDF(brdfw->brdfData, indir);
	}
}

AtVector AiCookTorranceMISSampleWrap(const void* brdfData, float randx,
									 float randy)
{
	const BrdfDataWrap* brdfw = reinterpret_cast<const BrdfDataWrap*>(brdfData);
	brdfw->ibs = true;
	AtVector indir = AiCookTorranceMISSample(brdfw->brdfData, randx, randy);
	if (!AiV3IsZero(indir)) {
		AtVector H;
		AiV3Normalize(H, (indir) + brdfw->V);
		brdfw->kr = brdfw->fr->kr(std::max(0.0f, AiV3Dot(H, indir)));
		brdfw->brdf = AiCookTorranceMISBRDF(brdfw->brdfData, &indir);
		brdfw->pdf = AiCookTorranceMISPDF(brdfw->brdfData, &indir);
		if (brdfw->pdf > 0.0f) {
			AtRGB w = brdfw->brdf / brdfw->pdf;
			brdfw->krInt += brdfw->kr * w;
			assert(AiIsFinite(brdfw->krInt));
			brdfw->ns++;
		}
	}
	return indir;
}
