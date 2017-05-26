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
    Mean, Gaussian, Median, Laplacian, Curvature
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
    void error(const char* format, ...);
    void warn(const char* format, ...);

private:
    Bifrost::API::StringArray _warnings;
    Bifrost::API::String _error;
};

struct ShapeParameters{
    Bifrost::API::String cache_file;
    Bifrost::API::String object;
    RenderComponent render_component = RenderComponent::Volume;
    Bifrost::API::StringArray channels;
    Bifrost::API::StringArray velocity_channels;
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

    virtual amino::Math::bboxf bbox() const;
    inline const Bifrost::API::Component& component() const{ return _component; }

    const Status& status() const{ return _status; }

protected:
    Status _status;
    Bifrost::API::Component _component;

private:
    Bifrost::API::String tmp_folder;
};

}} // Bifrost::Processing

