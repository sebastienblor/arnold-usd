#ifndef OUTPUT_DRIVER_H
#define OUTPUT_DRIVER_H

#include <maya/MComputation.h>

void InitializeDisplayUpdateQueue();
void ProcessDisplayUpdateQueue();
void ProcessDisplayUpdateQueueWithInterupt(MComputation & comp);
bool ProcessSomeOfDisplayUpdateQueue(const bool refresh=true);
void ClearDisplayUpdateQueue();
void DisplayUpdateQueueRenderFinished();

#endif  // OUTPUT_DRIVER_H
