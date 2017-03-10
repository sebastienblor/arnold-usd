// $Id: color_utils.cpp 420 2013-06-18 14:01:26Z kik $

#include "color_utils.h"
#include <ai.h>
#include <algorithm>

AtRGB convertToRGB(const AtRGB& color, int from_space)
{
    if (from_space == COLOR_SPACE_HSV || from_space == COLOR_SPACE_HSL)
    {
        float hue6 = fmod(color.r, 1.0f) * 6.0f;
        float hue2 = hue6;
        if (hue6 > 4.0f) hue2 -= 4.0f;
        else if (hue6 > 2.0f) hue2 -= 2.0f;

        float chroma;
        float sat = AiClamp(color.g, 0.0f, 1.0f);
        if (from_space == COLOR_SPACE_HSV)
            chroma = sat * color.b;
        else // HSL
            chroma = (1.0f - fabsf(2.0f * color.b - 1.0f)) * sat;
      
        float component = chroma * (1.0f - fabsf(hue2 - 1.0f));

        AtRGB rgb;
        if      (hue6 < 1) rgb = AtRGB(chroma, component, 0.0f);
        else if (hue6 < 2) rgb = AtRGB(component, chroma, 0.0f);
        else if (hue6 < 3) rgb = AtRGB(0.0f, chroma, component);
        else if (hue6 < 4) rgb = AtRGB(0.0f, component, chroma);
        else if (hue6 < 5) rgb = AtRGB(component, 0.0f, chroma);
        else               rgb = AtRGB(chroma, 0.0f, component);
      
        float cmin;
        if (from_space == COLOR_SPACE_HSV)
            cmin = color.b - chroma;
        else // HSL
            cmin = color.b - chroma * 0.5f;

        rgb += cmin;
        return rgb;
    }
    else if (from_space == COLOR_SPACE_XYZ || from_space == COLOR_SPACE_XYY)
    {
        // for documentation purposes, CIE->RGB needs color system data, and here
        // are some typical bits that are needed:
        //
        // Name               xRed     yRed     xGreen   yGreen   xBlue    yBlue    White point                Gamma
        // -----------------------------------------------------------------------------------------------------------------------
        // "NTSC",            0.67f,   0.33f,   0.21f,   0.71f,   0.14f,   0.08f,   WhitePoint::IlluminantC,   GAMMA_REC601
        // "EBU (PAL/SECAM)", 0.64f,   0.33f,   0.29f,   0.60f,   0.15f,   0.06f,   WhitePoint::IlluminantD65, GAMMA_REC709
        // "SMPTE",           0.630f,  0.340f,  0.310f,  0.595f,  0.155f,  0.070f,  WhitePoint::IlluminantD65, GAMMA_REC709
        // "HDTV",            0.670f,  0.330f,  0.210f,  0.710f,  0.150f,  0.060f,  WhitePoint::IlluminantD65, GAMMA_REC709 (2.35)
        // "sRGB",            0.670f,  0.330f,  0.210f,  0.710f,  0.150f,  0.060f,  WhitePoint::IlluminantD65, 2.2
        // "CIE",             0.7355f, 0.2645f, 0.2658f, 0.7243f, 0.1669f, 0.0085f, WhitePoint::IlluminantE,   GAMMA_REC709
        // "CIE REC 709",     0.64f,   0.33f,   0.30f,   0.60f,   0.15f,   0.06f,   WhitePoint::IlluminantD65, GAMMA_REC709
        //
        // typical white points are as follows:
        //
        // Name          x            y              Description
        // -----------------------------------------------------------------------------
        // IlluminantA   0.44757f,    0.40745f    Incandescent tungsten
        // IlluminantB   0.34842f,    0.35161f    Obsolete, direct sunlight at noon
        // IlluminantC   0.31006f,    0.31616f    Obsolete, north sky daylight
        // IlluminantD50 0.34567f,    0.35850f    Some print, cameras
        // IlluminantD55 0.33242f,    0.34743f    Some print, cameras
        // IlluminantD65 0.31271f,    0.32902f    For EBU and SMPTE, HDTV, sRGB
        // IlluminantD75 0.29902f,    0.31485f    ???
        // IlluminantE   0.33333333f, 0.33333333f CIE equal-energy illuminant
        // Illuminant93K 0.28480f,    0.29320f    High-efficiency blue phosphor monitors
        // IlluminantF2  0.37207f,    0.37512f    Cool white flourescent (CWF)
        // IlluminantF7  0.31285f,    0.32918f    Broad-band daylight flourescent
        // IlluminantF11 0.38054f,    0.37691f    Narrow-band white flourescent

        // we use the CIE equal-energy color space, as it is the most generic

        float xr, yr, zr, xg, yg, zg, xb, yb, zb;
        float xw, yw, zw;
        float rx, ry, rz, gx, gy, gz, bx, by, bz;
        float rw, gw, bw;

        float xc, yc, zc;
        if (from_space == COLOR_SPACE_XYZ)
        {
            xc = color.r;
            yc = color.g;
            zc = color.b;
        }
        else
        {
            AtRGB xyz = xyYToXYZ(color);
            xc = xyz.r;
            yc = xyz.g;
            zc = xyz.b;
        }

        // these come from the tables above, using CIE equal-energy color space

        xr = 0.7355f; yr = 0.2654f; zr = 1.0f - (xr + yr);
        xg = 0.2658f; yg = 0.7243f; zg = 1.0f - (xg + yg);
        xb = 0.1669f; yb = 0.0085f; zb = 1.0f - (xb + yb);

        xw = 1.0f / 3.0f; yw = 1.0f / 3.0f; zw = 1.0f - (xw + yw);

        // xyz -> rgb matrix, before scaling to white

        rx = (yg * zb) - (yb * zg);  ry = (xb * zg) - (xg * zb);  rz = (xg * yb) - (xb * yg);
        gx = (yb * zr) - (yr * zb);  gy = (xr * zb) - (xb * zr);  gz = (xb * yr) - (xr * yb);
        bx = (yr * zg) - (yg * zr);  by = (xg * zr) - (xr * zg);  bz = (xr * yg) - (xg * yr);

        // white scaling factors; dividing by yw scales the white luminance to unity, as conventional

        rw = ((rx * xw) + (ry * yw) + (rz * zw)) / yw;
        gw = ((gx * xw) + (gy * yw) + (gz * zw)) / yw;
        bw = ((bx * xw) + (by * yw) + (bz * zw)) / yw;

        // xyz -> rgb matrix, correctly scaled to white

        rx = rx / rw;  ry = ry / rw;  rz = rz / rw;
        gx = gx / gw;  gy = gy / gw;  gz = gz / gw;
        bx = bx / bw;  by = by / bw;  bz = bz / bw;

        // rgb of the desired point

        float r = (rx * xc) + (ry * yc) + (rz * zc);
        float g = (gx * xc) + (gy * yc) + (gz * zc);
        float b = (bx * xc) + (by * yc) + (bz * zc);

        AtRGB result( r, g, b );
        return result;
    }
   
    // was RGB already (or unknown color space)
    return color;
}

AtRGB convertFromRGB(const AtRGB& color, int to_space)
{
    if (to_space == COLOR_SPACE_HSL || to_space == COLOR_SPACE_HSV)
    {
        float cmax = AiColorMaxRGB(color);
        float cmin = std::min(std::min(color.r, color.g), color.b);
        float chroma = cmax - cmin;

        float hue;
        if (chroma == 0.0f)
            hue = 0.0f;
        else if (cmax == color.r)
            hue = (color.g - color.b) / chroma;
        else if (cmax == color.g)
            hue = (color.b - color.r) / chroma + 2.0f;
        else
            hue = (color.r - color.g) / chroma + 4.0f;
        hue *= 1.0f / 6.0f;
        if (hue < 0.0f)
           hue += 1.0f;

        if (to_space == COLOR_SPACE_HSL)
        {
            float lightness = (cmax + cmin) * 0.5f;
            float saturation;
            if (chroma == 0.0f)
                saturation = 0.0f;
            else
                saturation = chroma / (1.0f - fabsf(2.0f * lightness - 1.0f));
            AtRGB result( hue, saturation, lightness );
            return result;
        }
        else
        {
            // HSV
            float value = cmax;
            float saturation = chroma == 0.0f ? 0.0f : chroma / value;
            AtRGB result( hue, saturation, value );
            return result;
        }
    }
    else if (to_space == COLOR_SPACE_XYZ || to_space == COLOR_SPACE_XYY)
    {
        float X = (0.49f * color.r + 0.31f * color.g + 0.2f * color.b) / 0.17697f;
        float Y = (0.17697f * color.r + 0.81240f * color.g + 0.01063f * color.b) / 0.17697f;
        float Z = (0.0f * color.r + 0.01f * color.g + 0.99f * color.b) / 0.17697f;
        AtRGB result;
        if (to_space == COLOR_SPACE_XYZ)
            result = AtRGB(X, Y, Z);
        else // xyY
        {
            float sum = X + Y + Z;
            if (sum > 0.00001f)
                result = AtRGB(X / sum, Y / sum, Y);
            else
                result = AI_RGB_BLACK;
        }
        return result;
    }

    // was RGB already (or unknown color space)
    return color;
}
