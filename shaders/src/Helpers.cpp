#include "Helpers.h"

// Set sg->P to Pref if Pref is existing on the object.
//
// How to use it in your shader :
// At the beginning of shader_evaluate, place these lines :
//
// 		AtPoint tmpPts;
//		bool usePref = setRefPts(sg, tmpPts);
//
// Use  restorePts() when you don't use Pref anymore :
//
//		if (usePref) restorePts(sg, tmpPts);
//
// @param sg		shading global.
// @param tmpTps	keep the original sg->P in this variable.
// @return    		True if Pref was found and sg->P replaced.
//
bool setRefPts(AtShaderGlobals *sg, AtPoint &tmpPts)
{
	AtPoint Pref;
	AtBoolean usePref = AiUDataGetPnt("Pref",&Pref);
	if (usePref)
	{
		tmpPts = sg->P;
		sg->P = Pref;
		return true;
	}
	return false;
}

// Set sg->N to Nref if Nref is existing on the object.
//
// How to use it in your shader :
// At the beginning of shader_evaluate, place these lines :
//
// 		AtVector tmpNmrs;
//		bool useNref = setRefNmrs(sg, tmpNmrs);
//
// Use  restoreNmrs() when you don't use Nref anymore :
//
//		if (useNref) restoreNmrs(sg, tmpNmrs);
//
// @param sg		shading global.
// @param tmpTps	keep the original sg->N in this variable.
// @return    		True if Nref was found and sg->N replaced.
//
bool setRefNmrs(AtShaderGlobals *sg, AtVector &tmpNmrs)
{
   AtVector Nref;
	AtBoolean useNref = AiUDataGetPnt("Nref",&Nref);
	if (useNref)
	{
		tmpNmrs = sg->N;
		sg->N = Nref;
		return true;
	}
	return false;
}

// Restore sg->P with original P. Use in combinaison with setRefPts.
//
// @param sg		shading global.
// @param tmpTps	The original P, given by setRefPts.
//
void restorePts(AtShaderGlobals *sg, AtPoint tmpPts)
{
   sg->P = tmpPts;
}

// Restore sg->N with original N. Use in combinaison with setRefNmrs.
// @param sg		shading global.
// @param tmpNmrs	The original P, given by tmpNmrsv.
//
void restoreNmrs(AtShaderGlobals *sg, AtVector tmpNmrs)
{
   sg->N = tmpNmrs;
}
