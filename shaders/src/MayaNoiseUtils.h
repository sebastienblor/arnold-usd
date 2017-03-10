#pragma once

#include <ai.h>

class MayaNoiseUtils
{
public:
    static void DNoise2(double x, double y, float val[3]);
    static float noise2(float x, float y);

    static double erand48_r(register unsigned short *in);
    static float recurrenceGetValue3d(float size, float shaker, AtVector* coord);
    static AtVector recurrenceGetVector3d(float size, float shaker, AtVector* coord);

    static void cell_initialize();
    static float cellGetValue3d(float cellSize, float density, float spottyness,
        float randomness, float edgeWidth, bool creases, AtVector *coord, AtVector *levelCount);

private:
    static void nextRE(unsigned int *xx);
};