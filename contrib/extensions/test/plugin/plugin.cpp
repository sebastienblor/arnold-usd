#include "test.h"
#include "nodes/ArnoldNodeFactory.h"

extern "C"
{

#ifdef _WIN32
#if defined(TEST_EXPORTS)
#define LIBSPEC __declspec(dllexport)
#elif defined(__cplusplus)
#define LIBSPEC extern "C" _declspec (dllexport)
#else
#define LIBSPEC _declspec (dllimport)
#endif
#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif // _WIN32
#endif // _WIN32

LIBSPEC void initializePlugin(CExtension &plugin)
{
   plugin.RegisterDependTranslator("lambert", 0x524c414d, CTestTranslatorCmd::creator);
}

LIBSPEC void uninitializePlugin(CExtension &plugin)
{
}

}
