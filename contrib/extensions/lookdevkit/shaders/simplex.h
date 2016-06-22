#pragma once

namespace simplex
{
   float CellNoise(float u, float v, float amplitude, float frequency);
   float FractalNoise(float u, float v, float amplitude, float ratio, int octaves, float frequency, float frequencyRatio, float distortionU, float distortionV, float distortionRatio);
   float RidgedNoise( float u, float v, float amplitude, float ratio, int octaves, float frequency, float frequencyRatio, float distortionU, float distortionV, float distortionRatio);
}
