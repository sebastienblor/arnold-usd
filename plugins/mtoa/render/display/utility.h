/*
 * Kick window data structure and functions called from outside driver
 */

#pragma once

#include <ai.h>




inline void AiBBox2Init(AtBBox2 &bbox, int bounds)
{
   bbox.minx =  bounds;
   bbox.miny =  bounds;
   bbox.maxx = -bounds;
   bbox.maxy = -bounds;
}

inline void AiBBox2Union(AtBBox2 &bbox_out, const AtBBox2 &bbox1, const AtBBox2 &bbox2)
{
   bbox_out.minx = MIN(bbox1.minx, bbox2.minx);
   bbox_out.miny = MIN(bbox1.miny, bbox2.miny);
   bbox_out.maxx = MAX(bbox1.maxx, bbox2.maxx);
   bbox_out.maxy = MAX(bbox1.maxy, bbox2.maxy);
}


