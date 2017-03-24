#ifndef _IMPLICIT_H_
#define _IMPLICIT_H_

#include <bifrostrendercore/bifrostrender_types.h>
#include <bifrostrendercore/bifrostrender_defs.h>
#include <bifrostrendercore/bifrostrender_filters.h>
#include <ai.h>

using namespace Bifrost::RenderCore;

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl
#define INIT_ASSERT(condition) if(!condition){ AiMsgWarning("%s:%d condition failed: %s", __FILENAME__, __LINE__, #condition); printEndOutput( "[BIFROST POLYMESH] END OUTPUT", inData->diagnostics ); return false; }
#define ERROR_ASSERT(condition) if(!condition){ AiMsgError("%s:%d condition failed: %s", __FILENAME__, __LINE__, #condition); }

void ImplicitNodeDeclareParameters(AtList* params, AtNodeEntry* nentry);

bool getNodeParameters( ImplicitsInputData *inData, const AtNode *node );

CoreObjectUserData *createCoreObjectUserData(Bifrost::API::String& json, Bifrost::API::String& filename, Bifrost::API::String& writeToFolder);

bool InitializeImplicit(ImplicitsInputData* inData, FrameData* frameData, AtBBox* bounds);

#endif
