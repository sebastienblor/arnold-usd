#include "Helpers.h"


bool setRefPts(AtShaderGlobals *sg, AtPoint &tmpPts)
{
	AtPoint Pref;
	bool usePref = AiUDataGetPnt("Pref",&Pref);
	if (usePref)
	{
		tmpPts = sg->P;
		sg->P = Pref;
		return 1;
	}
	return 0;
}

bool setRefNmrs(AtShaderGlobals *sg, AtVector &tmpNmrs)
{
	AtVector Nref;
	bool useNref = AiUDataGetPnt("Nref",&Nref);
	if (useNref)
	{
		tmpNmrs = sg->N;
		sg->N = Nref;
		return 1;
	}
	return 0;
}

void restorePts(AtShaderGlobals *sg, AtPoint tmpPts)
{
		sg->P = tmpPts;
}

void restoreNmrs(AtShaderGlobals *sg, AtVector tmpNmrs)
{
		sg->N = tmpNmrs;
}
