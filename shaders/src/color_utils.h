// $Id: color_utils.h 184 2013-01-14 18:03:37Z kik $

#pragma once

#include <ai.h>

#define COLOR_SPACE_RGB 0
#define COLOR_SPACE_XYZ 1
#define COLOR_SPACE_XYY 2
#define COLOR_SPACE_HSL 3
#define COLOR_SPACE_HSV 4

inline AtColor XYZToxyY(const AtColor& xyz)
{
    AtColor result;
    float sum = xyz.r + xyz.g + xyz.b;
    if (sum > 0.00001f)
        result = AtRGB(xyz.r / sum, xyz.g / sum, xyz.g);
    else
        result = AI_RGB_BLACK;
    return result;
}

inline AtColor xyYToXYZ(const AtColor& xyY)
{
    AtColor result(
                  xyY.b * xyY.r / xyY.g,
                  xyY.b,
                  xyY.b * (1.0f - xyY.r - xyY.g) / xyY.g);
    return result;
}

AtColor convertToRGB(const AtColor& color, int from_space);
AtColor convertFromRGB(const AtColor& color, int to_space);
