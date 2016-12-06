#include "simplex.h"
#include "utils.h"

namespace simplex
{
   struct float2
   {
      float2() {};
      float2(const float s) : x(s), y(s) {}
      float2(const float x, const float y) : x(x), y(y) {}
      float x, y;

      inline float2 operator*(const float s) const { return float2(x*s, y*s); }
      inline float2 operator*=(const float s) { x *= s; y *= s; return *this; }
      inline float2 operator*=(const float2& a) { x *= a.x; y *= a.y; return *this; }
      inline float2 operator+(const float2& a) const { return float2(x+a.x, y+a.y); }
      inline float2 operator-(const float2& a) const { return float2(x-a.x, y-a.y); }
   };

   inline float dot(const float2& a, const float2& b)
   {
      return a.x * b.x + a.y * b.y;
   }

   inline float2 floor(const float2& in)
   {
      return float2(floorf(in.x), floorf(in.y));
   }

   inline float2 min(const float2& a, const float2& b)
   {
      return float2(AiMin(a.x, b.x), AiMin(a.y, b.y));
   }


   struct float3
   {
      float3() {};
      float3(const float s) : x(s), y(s), z(s) {}
      float3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
      float x, y, z;

      inline float3 operator*(const float s) const { return float3(x*s, y*s, z*s); }
      inline float3 operator*(const float3& a) const { return float3(x*a.x, y*a.y, z*a.z); }
      inline float3 operator+(const float3& a) const { return float3(x+a.x, y+a.y, z+a.z); }
      inline float3 operator-(const float3& a) const { return float3(x-a.x, y-a.y, z-a.z); }
   };

   inline float dot(const float3& a, const float3& b)
   {
      return a.x * b.x + a.y * b.y + a.z * b.z;
   }

   inline float3 max(const float3& a, const float b)
   {
      return float3(AiMax(a.x, b), AiMax(a.y, b), AiMax(a.z, b));
   }

   inline float3 inversesqrt(const float3& in)
   {
      return float3(1.0f/sqrt(in.x), 1.0f/sqrt(in.y), 1.0f/sqrt(in.z));
   }


   struct float4
   {
      float4() {};
      float4(const float s) : x(s), y(s), z(s), w(s) {}
      float4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
      float x, y, z, w;

      inline float4 operator*(const float s) const { return float4(x*s, y*s, z*s, w*s); }
      inline float4 operator*(const float4& a) const { return float4(x*a.x, y*a.y, z*a.z, w*a.w); }
      inline float4 operator*=(const float s) { x*=s; y*=s; z*=s; w*=s; return *this; }
      inline float4 operator*=(const float4& a) { x*=a.x; y*=a.y; z*=a.z; w*=a.w; return *this; }
      inline float4 operator+(const float4& a) const { return float4(x+a.x, y+a.y, z+a.z, w+a.w); }
      inline float4 operator+=(const float4& a) { x+=a.x; y+=a.y; z+=a.z; w+=a.w; return *this; }
      inline float4 operator-(const float4& a) const { return float4(x-a.x, y-a.y, z-a.z, w-a.w); }
      inline float4 operator-=(const float4& a) { x-=a.x; y-=a.y; z-=a.z; w-=a.w; return *this; }
   };

   inline float dot(const float4& a, const float4& b)
   {
      return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
   }

   inline float4 floor(const float4& in)
   {
      return float4(floorf(in.x), floorf(in.y), floorf(in.z), floorf(in.w));
   }

   inline float4 fract(const float4& in)
   {
      return float4(in.x - floorf(in.x), in.y - floorf(in.y), in.z - floorf(in.z), in.w - floorf(in.w));
   }

   inline float4 sign(const float4& in)
   {
      return float4(float(SGN(in.x)), float(SGN(in.y)), float(SGN(in.z)), float(SGN(in.w)));
   }


   //
   // source: https://briansharpe.wordpress.com/2012/01/13/simplex-noise, https://github.com/BrianSharpe
   // modified for cpp and compatibility with library code.
   //
   inline void Fast32Hash2D(float2 gridcell, float4 *hash_0, float4 *hash_1)
   {
      const float2 OFFSET = float2( 26.0f, 161.0f );
      const float XDOMAIN = 71.0f;
      const float2 SOMELARGEFLOATS = float2( 951.135664f, 642.949883f );
      float4 P = float4( gridcell.x, gridcell.y, gridcell.x + 1.0f, gridcell.y + 1.0f );
      P = P - floor(P * ( 1.0f / XDOMAIN )) * XDOMAIN;
      P += float4(OFFSET.x, OFFSET.y, OFFSET.x, OFFSET.y); // OFFSET.xyxy
      P *= P;
      P = float4(P.x, P.z, P.x, P.z) * float4(P.y, P.y, P.w, P.w); // P.xzxz * P.yyww;
      *hash_0 = fract( P * ( 1.0f / SOMELARGEFLOATS.x ) );
      *hash_1 = fract( P * ( 1.0f / SOMELARGEFLOATS.y ) );
   }

   inline float4 CellularWeightSamples( float4 samples )
   {
      samples = samples * 2.0f - 1.0f;
      return (samples * samples * samples) - sign(samples);
   }

   //
   // source: https://briansharpe.wordpress.com/2012/01/13/simplex-noise, https://github.com/BrianSharpe
   // modified for cpp and compatibility with library code.
   //
   float SimplexPerlin2D(float2 P)
   {
      const float SKEWFACTOR = 0.36602540378443864676372317075294f;
      const float UNSKEWFACTOR = 0.21132486540518711774542560974902f;
      const float SIMPLEX_TRI_HEIGHT = 0.70710678118654752440084436210485f;
      const float3 SIMPLEX_POINTS = float3( 1.0f-UNSKEWFACTOR, -UNSKEWFACTOR, 1.0f-2.0f*UNSKEWFACTOR );

      P *= SIMPLEX_TRI_HEIGHT;
      float2 Pi = floor( P + dot( P, float2(SKEWFACTOR) ) );

      float4 hash_x, hash_y;
      Fast32Hash2D( Pi, &hash_x, &hash_y );

      float2 v0 = Pi - dot( Pi, float2(UNSKEWFACTOR, UNSKEWFACTOR) ) - P;
      float4 v1pos_v1hash = (v0.x < v0.y) ? float4(SIMPLEX_POINTS.x, SIMPLEX_POINTS.y, hash_x.y, hash_y.y) : float4(SIMPLEX_POINTS.y, SIMPLEX_POINTS.x, hash_x.z, hash_y.z);
      float4 v12 = float4(v1pos_v1hash.x, v1pos_v1hash.y, SIMPLEX_POINTS.z, SIMPLEX_POINTS.z) + float4(v0.x, v0.y, v0.x, v0.y);

      float3 grad_x = float3( hash_x.x, v1pos_v1hash.z, hash_x.w ) - 0.49999f;
      float3 grad_y = float3( hash_y.x, v1pos_v1hash.w, hash_y.w ) - 0.49999f;
      float3 grad_results = inversesqrt( grad_x * grad_x + grad_y * grad_y ) * ( grad_x * float3( v0.x, v12.x, v12.z ) + grad_y * float3( v0.y, v12.y, v12.w ) );

      const float FINAL_NORMALIZATION = 99.204334582718712976990005025589f;

      float3 m = float3( v0.x, v12.x, v12.z ) * float3( v0.x, v12.x, v12.z ) + float3( v0.y, v12.y, v12.w ) * float3( v0.y, v12.y, v12.w );

      m = max(float3(0.5f) - m, 0.0f);
      m = m*m;

      return dot(m*m, grad_results) * FINAL_NORMALIZATION;
   }

   float SimplexCellular2D(float2 P)
   {
      const float SKEWFACTOR = 0.36602540378443864676372317075294f;
      const float UNSKEWFACTOR = 0.21132486540518711774542560974902f;
      const float SIMPLEX_TRI_HEIGHT = 0.70710678118654752440084436210485f;
      const float INV_SIMPLEX_TRI_HEIGHT = 1.4142135623730950488016887242097f;
      const float3 SIMPLEX_POINTS = float3( 1.0f-UNSKEWFACTOR, -UNSKEWFACTOR, 1.0f-2.0f*UNSKEWFACTOR ) * INV_SIMPLEX_TRI_HEIGHT;

      P *= SIMPLEX_TRI_HEIGHT;
      float2 Pi = floor( P + dot( P, float2( SKEWFACTOR ) ) );

      float4 hash_x, hash_y;
      Fast32Hash2D( Pi, &hash_x, &hash_y );

      const float JITTER_WINDOW = ( 0.10566243270259355887271280487451f * INV_SIMPLEX_TRI_HEIGHT );
      hash_x = CellularWeightSamples( hash_x ) * JITTER_WINDOW;
      hash_y = CellularWeightSamples( hash_y ) * JITTER_WINDOW;

      float2 p0 = ( ( Pi - dot( Pi, float2( UNSKEWFACTOR ) ) ) - P ) * INV_SIMPLEX_TRI_HEIGHT;
      hash_x += float4(p0.x, p0.x, p0.x, p0.x);
      hash_y += float4(p0.y, p0.y, p0.y, p0.y);
      hash_x.y += SIMPLEX_POINTS.x;
      hash_x.z += SIMPLEX_POINTS.y;
      hash_x.w += SIMPLEX_POINTS.z;
      hash_y.y += SIMPLEX_POINTS.y;
      hash_y.z += SIMPLEX_POINTS.x;
      hash_y.w += SIMPLEX_POINTS.z;
      float4 distsq = hash_x*hash_x + hash_y*hash_y;
      float2 tmp = min( float2(distsq.x, distsq.y), float2(distsq.z, distsq.w) );
      return AiMin( tmp.x, tmp.y );
   }

   float FractalNoise(
      float u, float v,
      float amplitude,
      float ratio,
      int octaves,
      float frequency,
      float frequencyRatio,
      float distortionU, float distortionV,
      float distortionRatio)
   {
      float2 uv(u,v);
      float2 distortion(distortionU, distortionV);

      float noise = 0.0f;
      float simplex = 0.0f;

      distortion *= distortionRatio;

      for (int i=0; i < octaves+1; i++) {
            noise = SimplexPerlin2D(uv+distortion) * pow(frequency, -amplitude*i);
            simplex += amplitude * noise;

            float tmp = uv.x;
            uv.x = uv.y * frequency - 0.02f;
            uv.y = tmp * frequency + 0.3f;

            amplitude *= ratio;
            frequency *= frequencyRatio;
            distortion *= distortionRatio;
      }

      simplex = 0.5f * simplex + 0.5f;

      return simplex;
   }

   float CellNoise(
      float u, float v,
      float amplitude,
      float frequency)
   {
      float2 uv(u,v);
      float noise = SimplexCellular2D(uv*frequency);
      float simplex = amplitude * noise;

      return simplex;
   }

   float RidgedNoise(
      float u, float v,
      float amplitude,
      float ratio,
      int octaves,
      float frequency,
      float frequencyRatio,
      float distortionU, float distortionV,
      float distortionRatio)
   {
      float2 uv(u,v);
      float2 distortion(distortionU, distortionV);

      float simplex, sig, weight, offset;

      simplex = 1.0f;
      offset = 1.0f;

      sig = offset - fabs(SimplexPerlin2D(uv));
      sig *= sig;
      simplex = sig;
      weight = 1.0f;

      distortion *= distortionRatio;

      for(int i=1; i<octaves; ++i) {
            uv *= frequency;

            weight = Clamp((sig*frequencyRatio), 1.0f, 0.0f);
            sig = offset - fabs(SimplexPerlin2D(uv+distortion));

            sig *= sig*weight;
            simplex += sig * pow(frequency, (ratio * -1.0f)) * amplitude;
            frequency *= frequencyRatio;
            distortion *= distortionRatio;
      }

      simplex = 0.5f * simplex + 0.5f;

      return simplex;
   }

} // end simplex namespace
