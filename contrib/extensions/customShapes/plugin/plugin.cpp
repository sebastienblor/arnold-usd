#include "nodes/ArnoldNodeFactory.h"
#include "CustomShapes.h"

void initializePlugin(CExtension& plugin)
{
   CCustomShapeTranslator::RegisterCustomShapes(plugin);
}

void uninitializePlugin(CExtension& plugin)
{
   // TODO
}
