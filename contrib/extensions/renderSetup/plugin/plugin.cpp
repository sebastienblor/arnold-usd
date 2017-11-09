#include "extension/Extension.h"
#include <maya/MTypes.h> 
#include "translators.h"

extern "C"
{

DLLEXPORT void initializeExtension(CExtension& extension)
{
    extension.Requires("renderSetup");
    extension.RegisterTranslator("applyAbsFloatOverride",   "", CApplyAbsOverrideTranslator<float>::creator);
    extension.RegisterTranslator("applyAbs2FloatsOverride", "", CApplyAbsOverrideTranslator<float2>::creator);
    extension.RegisterTranslator("applyAbs3FloatsOverride", "", CApplyAbsOverrideTranslator<float3>::creator);
    extension.RegisterTranslator("applyAbsIntOverride",     "", CApplyAbsOverrideTranslator<int>::creator);

    extension.RegisterTranslator("applyRelFloatOverride",   "", CApplyRelOverrideTranslator<float>::creator);
    extension.RegisterTranslator("applyRel2FloatsOverride", "", CApplyRelOverrideTranslator<float2>::creator);
    extension.RegisterTranslator("applyRel3FloatsOverride", "", CApplyRelOverrideTranslator<float3>::creator);
    extension.RegisterTranslator("applyRelIntOverride",     "", CApplyRelOverrideTranslator<int>::creator);
}

DLLEXPORT void deinitializeExtension(CExtension& extension)
{
}

}
