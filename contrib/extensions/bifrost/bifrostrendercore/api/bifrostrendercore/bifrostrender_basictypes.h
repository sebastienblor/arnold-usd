//*****************************************************************************
// Copyright 2016 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//*****************************************************************************

// Code written by Cave (www.cavevfx.com) for Autodesk in 2015
// Written by Erdem Taylan

#pragma once

#include <vector>
#include <stdint.h>
#include <cfloat>

#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_object.h>
#include <bifrostapi/bifrost_stateserver.h>

#include "bifrostrender_decl.h"

namespace Bifrost{
namespace RenderCore{

//
// TYPEDEFS
//
typedef short CvBool;
typedef float CvFloat;
typedef int CvInt;
typedef int CvError;
typedef char *CvString;
typedef char *CvToken;
typedef void *CvPointer;
typedef CvFloat CvColor[3];
typedef CvFloat CvPoint[3];
typedef CvFloat CvVector[3];
typedef CvFloat CvNormal[3];
typedef CvFloat CvHpoint[4];
typedef CvFloat CvMatrix[4][4];
typedef CvFloat CvBasis[4][4];
typedef CvFloat CvBound[6];
typedef CvFloat CvInterval[2];

class CvColorRGB;

class CvFloat3 
{
  public:
    // allow direct member access
    CvFloat x, y, z; 
    
    inline CvFloat3() {}
    inline CvFloat3(CvFloat xx, CvFloat yy, CvFloat zz)
    {
        assert(sizeof(*this) == sizeof(CvFloat[3])); 
        // ensure compat with legacy c-layout
        x = xx; y = yy; z = zz;
    }
    explicit inline CvFloat3(CvFloat v)
    {
	x = y = z = v;
    }
    // construct from a float array
    // nb: CvFloat3(0) is ambiguous (float and NULL)
    explicit inline CvFloat3(const CvFloat *d)
    {
        x = d[0];
        y = d[1];
        z = d[2];
    }
    explicit inline CvFloat3(CvColorRGB const &rgb);

    // offer access as array (original RtPoint was CvFloat[3])
    inline CvFloat& operator[] (int i) 
    {
        assert(i >= 0 && i<3);
        return (&x)[i];
    }
    inline const CvFloat& operator[] (int i) const
    {
        assert(i >= 0 && i<3);
        return (&x)[i];
    }

    // keep if we want to be able to compare point/vector/normal for equality
    // otherwise move to derived
    inline int operator==(const CvFloat3 &rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
    inline int operator!=(const CvFloat3 &rhs) const
    {
        return x != rhs.x || y != rhs.y || z != rhs.z;
    }

    // type cast  Too dangerous as the compiler does them automatically
    // and we can't make them explicit (not supported before C++ 11)
    // use &x[0]
    //inline operator const CvFloat *() const { return (CvFloat *) &x; }
    //inline operator CvFloat *() { return (CvFloat *) &x; }
 
    // serialize
    friend std::ostream& operator<<(std::ostream& o, const CvFloat3& v)
    {
        o << v.x << " " << v.y << " " << v.z;
        return o;
    }

    // addition
    inline CvFloat3 operator+(const CvFloat3 &rhs) const
    {
        return CvFloat3(x + rhs.x,
                        y + rhs.y,
                        z + rhs.z);
    }
    inline CvFloat3 &operator+=(const CvFloat3 &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    // multiplication
    inline CvFloat3 operator*(const CvFloat3 &rhs) const
    {
        return CvFloat3(x * rhs.x,
                        y * rhs.y,
                        z * rhs.z);
    }
    inline CvFloat3 &operator*=(const CvFloat3 &rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }

    // subtraction
    inline CvFloat3 operator-(const CvFloat3 &rhs) const
    {
        return CvFloat3(x - rhs.x,
                        y - rhs.y,
                        z - rhs.z);
    }
    inline CvFloat3 &operator-=(const CvFloat3 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    // division
    inline CvFloat3 operator/(const CvFloat3 &rhs) const
    {
        return CvFloat3(x / rhs.x,
                        y / rhs.y,
                        z / rhs.z);
    }
    inline CvFloat3 &operator/=(const CvFloat3 &rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }

    // unary minus
    inline CvFloat3 operator-() const { return CvFloat3(-x, -y, -z); }

    // scalar addition
    inline CvFloat3 operator+(CvFloat rhs) const
    {
        return CvFloat3(x + rhs, y + rhs, z + rhs);
    }
    inline friend  CvFloat3 operator+(CvFloat lhs, const CvFloat3 &rhs) {
        return rhs + lhs;
    }
    inline CvFloat3 &operator+=(CvFloat rhs)
    {
        x += rhs;
        y += rhs;
        z += rhs;
        return *this;
    }

    // scalar subtraction
    inline CvFloat3 operator-(CvFloat rhs) const
    {
        return CvFloat3(x - rhs, y - rhs, z - rhs);
    }
    inline friend  CvFloat3 operator-(CvFloat lhs, const CvFloat3 &rhs) {
        return rhs - lhs;
    }
    inline CvFloat3 &operator-=(CvFloat rhs)
    {
        x -= rhs;
        y -= rhs;
        z -= rhs;
        return *this;
    }

    // scalar multiplication
    inline CvFloat3 operator*(CvFloat rhs) const
    {
        return CvFloat3(x * rhs, y * rhs, z * rhs);
    }
    inline friend  CvFloat3 operator*(CvFloat lhs, const CvFloat3 &rhs) {
        return rhs * lhs;
    }
    inline CvFloat3 &operator*=(CvFloat rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    // scalar division
    inline CvFloat3 operator/(CvFloat rhs) const
    {
        CvFloat inv = 1.0f / rhs;
        return operator*(inv);
    }

    inline CvFloat3 & operator/=(CvFloat rhs)
    {
        CvFloat inv = 1.0f / rhs;
        return operator*=(inv);
    }

    // Dot product
    inline CvFloat Dot(const CvFloat3 &v2) const
    {
        return x * v2.x + y * v2.y + z * v2.z;
    }

    inline friend CvFloat Dot(const CvFloat3 &v1, const CvFloat3 &v2)
    {
        return v1.Dot(v2);
    }

    // Absolute value of dot product
    inline CvFloat AbsDot(const CvFloat3 &v2) const
    {
        float result =  x * v2.x + y * v2.y + z * v2.z;
        if(result < 0.f) 
            result = -result;
        return result;
    }

    inline friend CvFloat AbsDot(const CvFloat3 &v1, const CvFloat3 &v2)
    {
        return v1.AbsDot(v2);
    }

    // Length
    inline CvFloat LengthSq() const
    { 
        return (x*x + y*y + z*z);
    }

    inline CvFloat Length() const
    { 
        return std::sqrt(LengthSq());
    }

    inline bool IsUnitLength(float eps=.005f) const 
    {
        CvFloat len = Length();
        if(len > 1.f+eps || len < 1.f-eps) return false;
        else return true;
    }

    // normalize vector in place, return length
    inline CvFloat Normalize()
    {
        CvFloat len = Dot(*this);
        if (len > FLT_MIN)
        {
            len = std::sqrt(len);
            *this /= len;
        }
        else 
            x = y = z = 0.f;
        return len;
    }
    inline CvFloat NormalizeAndNegate()
    {
        CvFloat len = Dot(*this);

        if (len > FLT_MIN) 
        {
            len = std::sqrt(len);
            *this /= len;
            x = -x;
            y = -y;
            z = -z;
        } 
        else 
        {
            x = 0.f;
            y = 0.f;
            z = 0.f;
        }
        return len;
    }

    inline friend CvFloat3 NormalizeCopy(const CvFloat3 &v) 
    {
        CvFloat3 copy = v;
        copy.Normalize();
        return copy;
    }

    inline friend void Normalize(CvFloat3 &v) 
    {
        v.Normalize();
    }

    // cross product
    inline CvFloat3 Cross(const CvFloat3 &v2) const
    {
        return CvFloat3(y * v2.z - z * v2.y,
                        z * v2.x - x * v2.z,
                        x * v2.y - y * v2.x);
    }
    inline friend CvFloat3 Cross(const CvFloat3 &v1, const CvFloat3 &v2) 
    {
        return v1.Cross(v2);
    }

    // create orthonormal basis from a single vector.  The vector has to be
    // non-zero.
    inline void
    CreateOrthonormalBasis(CvFloat3 &v1, CvFloat3 &v2) const
    {
        CvFloat3 copy = *this;
        copy.Normalize();

        // Pick v1
        if (fabsf(copy.x) > 1e-3f || fabsf(copy.y) > 1e-3f)
            v1 = CvFloat3(copy.y, -copy.x, 0.0); // v1 = copy x Z
        else
            v1 = CvFloat3(0.0, copy.z, -copy.y); // v1 = copy x X

        v1.Normalize();

        // Compute v2
        v2 = copy.Cross(v1);   // v2 = copy x v1
    }

    inline CvFloat ChannelAvg() const
    {
        return (x + y + z) * (1.0f / 3.0f);
    }

    inline CvFloat ChannelMin() const
    {
        CvFloat min = x;
        if(y < min) min = y;
        if(z < min) min = z;
        return min;
    }

    inline CvFloat ChannelMax() const
    {
        CvFloat max = x;
        if(y > max) max = y;
        if(z > max) max = z;
        return max;
    }
};

typedef CvFloat3 CvPoint3;
typedef CvFloat3 CvVector3;
typedef CvFloat3 CvNormal3;

// -------------------------------------------------------------------------
// CvColorRGB is a Float3 with special type to trigger alternate
// transform semantics (color transformation)
class CvColorRGB 
{
  public:
    CvFloat r,g,b;
    
    CvColorRGB() {}
    CvColorRGB(CvFloat rr, CvFloat gg, CvFloat bb ) : r(rr), g(gg), b(bb) { 
        assert(sizeof(*this) == sizeof(CvFloat[3]));
    } 
    explicit inline CvColorRGB(CvFloat v)
    {
	r = g = b = v;
    }
    // construct from a float array
    // nb: v = CvColorRGB(0) is ambiguous (float and NULL)
    explicit inline CvColorRGB(const CvFloat *d)
    {
        r = d[0];
        g = d[1];
        b = d[2];
    }

    explicit inline CvColorRGB(CvFloat3 const &rhs, bool asNormalVector = false)
    {
        if(asNormalVector)
        {
            r = rhs.x*.5f + .5f;
            g = rhs.y*.5f + .5f;
            b = rhs.z*.5f + .5f;
        }
        else
        {
            r = rhs.x;
            g = rhs.y;
            b = rhs.z;
        }
    }

    inline void Zero()
    {
        r = g = b = 0.f;
    } 

    inline void One() 
    {
        r = g = b = 1.f;
    }

    // IsBlack returns true if all components are small or negative
    inline bool IsBlack(float epsilon=1e-6f) const
    {
        return (r <= epsilon && g <= epsilon && b <= epsilon);
    }

    // IsZero returns true if all components are near zero.
    inline bool IsZero(CvFloat epsilon=1e-6f) const
    {
        return (fabsf(r)<=epsilon && fabsf(g)<=epsilon && fabsf(b)<=epsilon);
    }

    // IsOne returns true if all components are >= 1.0f
    inline bool IsOne(CvFloat epsilon=1e-6f) const
    {
        return (r+epsilon >= 1.f && g+epsilon >= 1.f && b+epsilon >= 1.f);
    }

    inline bool IsMonochrome(CvFloat epsilon=1e-6f) const
    {
        return ( fabsf(r-g)<epsilon && fabsf(g-b)<epsilon );
    }

    // offer access as array (original RtPoint was CvFloat[3])
    inline CvFloat& operator[] (int i) 
    {
        assert(i >= 0 && i<3);
        return (&r)[i];
    }

    inline const CvFloat& operator[] (int i) const
    {
        assert(i >= 0 && i<3);
        return (&r)[i];
    }

    // keep if we want to be able to compare point/vector/normal for equality
    // otherwise move to derived
    inline int operator==(const CvColorRGB &rhs) const
    {
        return r == rhs.r && g == rhs.g && b == rhs.b;
    }

    inline int operator!=(const CvColorRGB &rhs) const
    {
        return r != rhs.r || g != rhs.g || b != rhs.b;
    }

    // type cast too dangerous as the compiler does them automatically
    // and we can't make them explicit (not supported before C++ 11)
    // use &r[0]
    //inline operator const CvFloat *() const { return (CvFloat *) &r; }
    //inline operator CvFloat *() { return (CvFloat *) &r; }
 
    // serialize
    friend std::ostream& operator<<(std::ostream& o, const CvColorRGB& c)
    {
        o << c.r << " " << c.g << " " << c.b;
        return o;
    }

    // addition
    inline CvColorRGB operator+(const CvColorRGB &rhs) const
    {
        return CvColorRGB(r + rhs.r,
                          g + rhs.g,
                          b + rhs.b);
    }

    inline CvColorRGB &operator+=(const CvColorRGB &rhs)
    {
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        return *this;
    }

    // subtraction
    inline CvColorRGB operator-(const CvColorRGB &rhs) const
    {
        return CvColorRGB(r - rhs.r,
                          g - rhs.g,
                          b - rhs.b);
    }

    inline CvColorRGB &operator-=(const CvColorRGB &rhs)
    {
        r -= rhs.r;
        g -= rhs.g;
        b -= rhs.b;
        return *this;
    }

    // unary minus
    inline CvColorRGB operator-() const { return CvColorRGB(-r, -g, -b); }

    // scalar multiplication, division
    inline CvColorRGB operator*(CvFloat rhs) const // scale this by scalar
    {
        return CvColorRGB(r * rhs, g * rhs, b * rhs);
    }

    inline friend CvColorRGB operator*(CvFloat lhs, const CvColorRGB &rhs) {
        return rhs * lhs;
    }

    inline CvColorRGB &operator*=(CvFloat rhs)
    {
        r *= rhs; g *= rhs; b *= rhs;
        return *this;
    }

    inline CvColorRGB operator/(CvFloat rhs) const
    {
        CvFloat inv = rhs != 0.f ? 1.0f / rhs : 1e-8f;
        return CvColorRGB(r * inv, g * inv, b * inv);
    }

    inline friend CvColorRGB operator/(CvFloat lhs, const CvColorRGB &rhs) 
    {
        return CvColorRGB(lhs/rhs.r, lhs/rhs.g, lhs/rhs.b);
    }

    inline CvColorRGB &operator/=(CvFloat rhs)
    {
        CvFloat inv = rhs != 0.f ? 1.0f / rhs : 1e-8f;
        r *= inv; g *= inv; b *= inv;
        return *this;
    }

    // scalar addition, subtraction
    inline CvColorRGB operator+(CvFloat rhs) const
    {
        return CvColorRGB(r + rhs, g + rhs, b + rhs);
    }

    inline friend  CvColorRGB operator+(CvFloat lhs, const CvColorRGB &rhs) 
    {
        return rhs + lhs;
    }

    inline CvColorRGB operator-(CvFloat rhs) const
    {
        return CvColorRGB(r - rhs, g - rhs, b - rhs);
    }

    inline friend  CvColorRGB operator-(CvFloat lhs, const CvColorRGB &rhs) 
    {
        return CvColorRGB(lhs - rhs.r, lhs - rhs.g, lhs - rhs.b);
    }

    // component multiplication, division
    inline CvColorRGB operator*(const CvColorRGB &rhs) const 
    {
        return CvColorRGB(r * rhs.r, g * rhs.g, b * rhs.b);
    }

    inline CvColorRGB &operator*=(const CvColorRGB &rhs)
    {
        r *= rhs.r; g *= rhs.g; b *= rhs.b;
        return *this;
    }

    inline CvColorRGB operator/(const CvColorRGB &rhs) const 
    {
        CvFloat rr = r / (rhs.r == 0.0f ? 1e-8f : rhs.r);
        CvFloat gg = g / (rhs.g == 0.0f ? 1e-8f : rhs.g);
        CvFloat bb = b / (rhs.b == 0.0f ? 1e-8f : rhs.b);
        return CvColorRGB(rr, gg, bb);
    }

    inline CvColorRGB &operator/=(const CvColorRGB &rhs)
    {
        r /= rhs.r; g /= rhs.g; b /= rhs.b;
        return *this;
    }

    // misc cross-channel computations
    inline CvFloat Luminance() const
    {
        return 0.2126f * r + 0.7152f * g + 0.0722f * b;
    }

    inline CvFloat ChannelAvg() const
    {
        return (r + g + b) * .33333f;
    }

    inline CvFloat ChannelMin() const
    {
        CvFloat min = r;
        if(g < min) min = g;
        if(b < min) min = b;
        return min;
    }

    inline CvFloat ChannelMax() const
    {
        CvFloat max = r;
        if(g > max) max = g;
        if(b > max) max = b;
        return max;
    }

    void ConvertToRGB(CvFloat *spectrum, int nsamps, CvFloat *cvt)
    {
        CvFloat *p = cvt;
        r = g = b = 0.0f;
        for(int i=0;i<nsamps;i++)
        {
            r += *p++ * *spectrum;
            g += *p++ * *spectrum;
            b += *p++ * *spectrum++;
        }

    }

    CvColorRGB Exp(CvFloat val) const
    {
        CvColorRGB result;
        result.r = std::exp(r*val);
        result.g = std::exp(g*val);
        result.b = std::exp(b*val);
        return result;
    }

    void InvertAlbedo()
    {
        r = 1.0f - r;
        g = 1.0f - g;
        b = 1.0f - b;
    }

    void ClampAlbedo()
    {
        if(r > 1.f) r = 1.f;
        else if(r < 0.f) r = 0.f;
        if(g > 1.f) g = 1.f;
        else if(g < 0.f) g = 0.f;
        if(b > 1.f) b = 1.f;
        else if(b < 0.f) b = 0.f;
    }

};


//
// CLASSES
//
class array3f
{
public:
	int ni;
	int nj;
	int nk;
	float defaultValue;
	float *data;

	array3f() {		
		ni = 0;
		nj = 0;
		nk = 0;
		defaultValue = 0.0;
		data = 0;
	}

	array3f (int xsize, int ysize, int zsize, float def) {
		ni = xsize;
		nj = ysize;
		nk = zsize;
		defaultValue = def;

		data = (float *) malloc( sizeof(float) * ni * nj * nk);
	}

	float& operator() (int i, int j, int k) {
		return *(data + k * nj * ni + j * ni + i);
	}

	const float& operator() (int i, int j, int k) const {
		return *(data + k * nj * ni + j * ni + i);
	}

	~array3f() {
		free (data);
	}
};

}}
