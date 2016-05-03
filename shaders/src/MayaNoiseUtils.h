#pragma once

class MayaNoiseUtils
{
public:
    static void DNoise2(double x, double y, float val[3]);
    static float noise2(float x, float y);

    static double erand48_r(register unsigned short *in);

private:
    static void nextRE(unsigned int *xx);
};