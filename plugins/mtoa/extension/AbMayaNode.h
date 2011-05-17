#ifndef ABMAYANODE_H
#define ABMAYANODE_H

#include "extension/PxMayaNode.h"

#include <maya/MPxNode.h>

#include <ai_nodes.h>
#include <ai.h>

// A Maya node class abstract
// Hold the information necessary to customize base Maya node classes
// for different Arnold nodes

// Extend as it becomes necessary

class CAbMayaNode
{
   friend class CPxMayaNode;
public:
   CAbMayaNode(const MString &mayaClassName = "",
               const MString &arnoldClassName = "",
               const MString &classif = "",
               const MString &providerName = "")
   : name(mayaClassName),
     arnold(arnoldClassName),
     classification(classif),
     provider(providerName)
   {}
   CAbMayaNode(const CPxMayaNode &mayaClass)
   : name(mayaClass.name),
     arnold(mayaClass.arnold),
     classification(mayaClass.classification),
     provider(mayaClass.provider)
   {}

   MString name;
   MString arnold;
   MString classification;
   MString provider;
};


#endif // ABMAYANODE_H
