#pragma once

#include <bifrostapi/bifrost_string.h>
#include <bifrostapi/bifrost_component.h>

namespace Bifrost {
namespace Processing {

enum class RenderComponent{
    Volume, Particle
};
std::ostream& operator <<(std::ostream& out, RenderComponent rc);

enum class SmoothMode{
    Laplacian, Curvature
};
std::ostream& operator <<(std::ostream& out, SmoothMode sm);

enum class BlendType{
    Linear, Smooth, Smoother
};
std::ostream& operator <<(std::ostream& out, BlendType bt);

class Status{
public:
    Status& operator=(const Status& o);
    inline bool operator!() const{ return !_error.empty(); }
    const Bifrost::API::String& error() const{ return _error; }
    const Bifrost::API::StringArray& warnings() const{ return _warnings; }
    void error(const Bifrost::API::String& format, ...);
    void warn(const Bifrost::API::String& format, ...);

private:
    Bifrost::API::StringArray _warnings;
    Bifrost::API::String _error;
};

struct ShapeParameters{
    Bifrost::API::String cache_folder;
    Bifrost::API::String activeGraph;
    Bifrost::API::String resourceId;
    Bifrost::API::String object;
    Bifrost::API::String point_component;
    Bifrost::API::String voxel_component;
    int frame;

    Bifrost::API::StringArray channels;
    Bifrost::API::String velocity_channel = "velocity";
    Bifrost::API::String uv_channel = "uv";

    float velocity_scale = 1;
    float space_scale = 1;
    unsigned int fps = 24;

    bool clip = false;
    amino::Math::bboxf clip_bbox = amino::Math::bboxf(amino::Math::vec3f(-1,-1,-1), amino::Math::vec3f(1,1,1));

    virtual ~ShapeParameters(){}
    virtual Bifrost::API::String str() const;
};

class Shape{
public:
    Shape(const ShapeParameters& params);
    virtual ~Shape();

    void scale(float space);

    virtual amino::Math::bboxf bbox() const;
    inline const Bifrost::API::Component& points() const{ return _points; }
    inline const Bifrost::API::Component& voxels() const{ return _voxels; }

    const Status& status() const{ return _status; }

protected:
    Status _status;
    Bifrost::API::Component _points, _voxels;

private:
    Bifrost::API::String activeGraph;
    Bifrost::API::String tmp_folder;
};

}} // Bifrost::Processing

