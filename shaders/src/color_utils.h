// $Id: color_utils.h 184 2013-01-14 18:03:37Z kik $

#pragma once

#include <ai.h>

#define COLOR_SPACE_RGB 0
#define COLOR_SPACE_XYZ 1
#define COLOR_SPACE_XYY 2
#define COLOR_SPACE_HSL 3
#define COLOR_SPACE_HSV 4

inline AtRGB XYZToxyY(const AtRGB& xyz)
{
    AtRGB result;
    float sum = xyz.r + xyz.g + xyz.b;
    if (sum > 0.00001f)
        result = AtRGB(xyz.r / sum, xyz.g / sum, xyz.g);
    else
        result = AI_RGB_BLACK;
    return result;
}

inline AtRGB xyYToXYZ(const AtRGB& xyY)
{
    AtRGB result(
                  xyY.b * xyY.r / xyY.g,
                  xyY.b,
                  xyY.b * (1.0f - xyY.r - xyY.g) / xyY.g);
    return result;
}

AtRGB convertToRGB(const AtRGB& color, int from_space);
AtRGB convertFromRGB(const AtRGB& color, int to_space);
