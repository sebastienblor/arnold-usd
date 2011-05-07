#include "PxMayaNode.h"
#if MAYA_API_VERSION >= 201200
   #include <maya/MNodeClass.h>
#endif


// A Maya node class proxy
CPxMayaNode::CPxMayaNode(const MString &typeName,
                         const MTypeId &typeId,
                         const MString &providerString,
                         MCreatorFunction creatorFunction,
                         MInitializeFunction initFunction,
                         MPxNode::Type typeNode,
                         const MString* classif)
{
   provider = providerString;
   name = typeName;
#if MAYA_API_VERSION < 201200
   id = typeId;
#else
   if (typeId.id() != 0)
      id = typeId;
   else
      id = MNodeClass(typeName).typeId();
#endif
   creator = creatorFunction;
   initialize = initFunction;
   type = typeNode;
   if (NULL != classif)
   {
      m_classification = MString(classif->asChar());
   }
   classification = m_classification.numChars() ? &m_classification : NULL;
}

