#pragma once

#include "ai.h"

inline AtRGB max(const AtRGB& c1, const AtRGB& c2)
{
	AtRGB c;
	c.r = std::max(c1.r, c2.r);
	c.g = std::max(c1.g, c2.g);
	c.b = std::max(c1.b, c2.b);
	return c;
}

inline AtRGB min(const AtRGB& c1, const AtRGB& c2)
{
	AtRGB c;
	c.r = std::min(c1.r, c2.r);
	c.g = std::min(c1.g, c2.g);
	c.b = std::min(c1.b, c2.b);
	return c;
}

inline AtVector max(const AtVector& c1, const AtVector& c2)
{
	AtVector c;
	c.x = std::max(c1.x, c2.x);
	c.y = std::max(c1.y, c2.y);
	c.z = std::max(c1.z, c2.z);
	return c;
}

inline AtVector min(const AtVector& c1, const AtVector& c2)
{
	AtVector c;
	c.x = std::min(c1.x, c2.x);
	c.y = std::min(c1.y, c2.y);
	c.z = std::min(c1.z, c2.z);
	return c;
}

inline int clamp(int a, int mn, int mx)
{
	return std::min(std::max(a, mn), mx);
}

inline float clamp(float a, float mn, float mx)
{
	return std::min(std::max(a, mn), mx);
}

inline AtRGB clamp(const AtRGB& a, const AtRGB& mn, const AtRGB& mx)
{
	return min( max(a, mn), mx );
}

inline AtVector clamp(const AtVector& a, const AtVector& mn, const AtVector& mx)
{
	return min( max(a, mn), mx );
}

inline float maxh(const AtRGB& c)
{
	return std::max(std::max(c.r, c.g), c.b);
}

inline float minh(const AtRGB& c)
{
	return std::min(std::min(c.r, c.g ), c.b);
}

inline void flipNormals( AtShaderGlobals* sg )
{
	sg->Nf = -sg->Nf;
	sg->Ngf = -sg->Ngf;
}

inline bool AiIsFinite(const AtRGB& c)
{
	return AiIsFinite(c.r) && AiIsFinite(c.g) && AiIsFinite(c.b);
}

inline bool AiIsFinite(const AtVector& v)
{
	return AiIsFinite(v.x) && AiIsFinite(v.y) && AiIsFinite(v.z);
}

/// simultaneous sin and cos
#ifndef __linux__
inline void sincosf(float x, float* sx, float* cx)
{
	*sx = sinf(x);
	*cx = sqrtf(1.0f - SQR(*sx));
}
#endif
