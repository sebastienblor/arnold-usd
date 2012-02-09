#ifndef OUTPUTDRIVER_H
#define OUTPUTDRIVER_H

#include <maya/MComputation.h>
#include <maya/MImage.h>

/// Initialize the display queue for a new render.
void InitializeDisplayUpdateQueue(const MString camera="", const MString panel="");

/// Process a message on the queue from Arnold.
/// \param refresh the render view is slow to refresh, so pass false if possible.
bool ProcessUpdateMessage(const bool refresh=true);

/// Process the whole queue until empty.
void ProcessDisplayUpdateQueue();

/// Process the queue until either the user interupts or and ender of render message processed.
/// \param comp a MComputation that should already have had begin() called.
void ProcessDisplayUpdateQueueWithInterupt(MComputation & comp);

void RefreshRenderViewBBox();

/// Clear the queue.
void ClearDisplayUpdateQueue();

/// Add a finished render message to the queue.
void DisplayUpdateQueueRenderFinished();

/// Convert the image in the queue to an MImage.
/// \param image the image from the queue is placed this MImage.
/// \return returns false if the queue doesn't contain a complete image.
bool DisplayUpdateQueueToMImage(MImage & image);

#endif  // OUTPUTDRIVER_H
