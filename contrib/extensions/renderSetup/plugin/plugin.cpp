#include "extension/Extension.h"
#include <maya/MTypes.h> 
#include "translators.h"

extern "C"
{

DLLEXPORT void initializeExtension(CExtension& extension)
{
    extension.Requires("renderSetup");
    extension.RegisterTranslator("applyAbsFloatOverride",   "", CApplyAbsFloatOverrideTranslator::creator);
    extension.RegisterTranslator("applyAbs2FloatsOverride", "", CApplyAbsFloat2OverrideTranslator::creator);
    extension.RegisterTranslator("applyAbs3FloatsOverride", "", CApplyAbsFloat3OverrideTranslator::creator);
    extension.RegisterTranslator("applyAbsIntOverride",     "", CApplyAbsIntOverrideTranslator::creator);

    extension.RegisterTranslator("applyRelFloatOverride",   "", CApplyRelFloatOverrideTranslator::creator);
    extension.RegisterTranslator("applyRel2FloatsOverride", "", CApplyRelFloat2OverrideTranslator::creator);
    extension.RegisterTranslator("applyRel3FloatsOverride", "", CApplyRelFloat3OverrideTranslator::creator);
    extension.RegisterTranslator("applyRelIntOverride",     "", CApplyRelIntOverrideTranslator::creator);
}

DLLEXPORT void deinitializeExtension(CExtension& extension)
{
}

}
