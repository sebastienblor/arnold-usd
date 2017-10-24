#include "extension/Extension.h"
#include "LookDevKitTranslator.h"

extern "C"
{

DLLEXPORT void initializeExtension(CExtension& extension)
{
   extension.RegisterPluginNodesAndTranslators("lookdevkit");

   const char* lookDevKitNodes[17] = 
               {
                  "floatConstant", 
                  "floatComposite", 
                  "floatCondition", 
                  "floatCorrect", 
                  "floatLogic", 
                  "floatMask", 
                  "floatMath",
                  "colorComposite", 
                  "colorCondition", 
                  "colorConstant", 
                  "colorCorrect", 
                  "colorLogic", 
                  "colorMask", 
                  "colorMath",
                  "channels", 
                  "premultiply", 
                  "simplexNoise"
                };

   for (int i = 0; i < 17; ++i)
      extension.RegisterTranslator ( lookDevKitNodes[i],
                                       "",
                                       CLookDevKitTranslator::creator );
   

}

DLLEXPORT void deinitializeExtension(CExtension& extension)
{
}

}
