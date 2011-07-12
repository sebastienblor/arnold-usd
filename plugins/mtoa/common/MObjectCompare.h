#ifndef MOBJECTCOMPARE_H
#define MOBJECTCOMPARE_H

#include <maya/MObjectHandle.h>

struct MObjectCompare
{
   bool operator()(MObjectHandle h1, MObjectHandle h2) const
   {
      return h1.hashCode() < h2.hashCode();
   }
};

#endif // MOBJECTCOMPARE_H
