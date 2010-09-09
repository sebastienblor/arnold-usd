#ifndef OUTPUT_DRIVER_H
#define OUTPUT_DRIVER_H

#include <maya/MComputation.h>

void InitializeDisplayUpdateQueue();
void ProcessDisplayUpdateQueue(MComputation& comp);

#endif  // OUTPUT_DRIVER_H
