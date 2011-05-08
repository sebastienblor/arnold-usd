#ifndef ABMAYANODE_H
#define ABMAYANODE_H

#include <maya/MPxNode.h>

#include <ai_nodes.h>
#include <ai.h>

// A Maya node class abstract
// Hold the information necessary to customize base Maya node classes
// for different Arnold nodes

// Extend as it becomes necessary

struct CAbMayaNode
{
   MString arnold;
   MString classification;
   CAbMayaNode(const MString &arnoldNode = "",
               const MString &classif = "")
   : arnold(arnoldNode), classification(classif)
   {}
};


#endif // ABMAYANODE_H
