#include "nodes/ArnoldNodeFactory.h"
#include "ShaveAndHaircut.h"

extern "C"
{

void initializePlugin(CExtension& plugin)
{
   plugin.RegisterDagTranslator("shaveHair",
                                0x1029b7,
                                CShaveTranslator::creator,
                                CShaveTranslator::NodeInitializer,
                                "shaveNode");
}

void uninitializePlugin(CExtension& plugin)
{
}

}

