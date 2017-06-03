#pragma once

#include <aminomath/vec.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_tileaccessor.h>

namespace Bifrost{
namespace Processing{

struct Interval{
    float t0, t1;
    Interval() : t0(-1), t1(-2){}
    Interval(float t0, float t1) : t0(t0), t1(t1){}
    inline bool valid() const{ return t0 >= 0 && t1 > t0; }
};

class Intersector{
public:
    Intersector(const Bifrost::API::Layout& layout);
    Intersector(const Bifrost::Processing::Intersector& o);
    ~Intersector();
    void init(const amino::Math::vec3f& origin, const amino::Math::vec3f& direction, float t0, float t1, bool debug=false);
    Interval next();
    bool done() const;
private:
    void* impl;
};

}} // namespace Bifrost::Processing
