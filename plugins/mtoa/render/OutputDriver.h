#ifndef OUTPUT_DRIVER_H
#define OUTPUT_DRIVER_H

#include <maya/MComputation.h>
#include <maya/MImage.h>

void InitializeDisplayUpdateQueue();
bool ProcessUpdateMessage(const bool refresh=true);
void ProcessDisplayUpdateQueue();
void ProcessDisplayUpdateQueueWithInterupt(MComputation & comp);
bool ProcessSomeOfDisplayUpdateQueue(const bool refresh=true);
void ClearDisplayUpdateQueue();
void DisplayUpdateQueueRenderFinished();
bool DisplayUpdateQueueToMImage( MImage & image );

#endif  // OUTPUT_DRIVER_H
