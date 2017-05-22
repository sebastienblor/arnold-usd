#pragma once
#include <bifrostapi/bifrost_string.h>
#include <bifrostapi/bifrost_component.h>

namespace Bifrost {
namespace Processing {

enum RenderComponent{
    Volume, Particle
};
enum SmoothMode{
    Mean, Gaussian, Median, Laplacian, Curvature
};
enum BlendType{
    Linear, Smooth, Smoother
};

class Status{
public:
    Status& operator=(const Status& o);
    inline bool operator!() const{ return !_error.empty(); }
    const Bifrost::API::String& error() const{ return _error; }
    const Bifrost::API::StringArray& warnings() const{ return _warnings; }
    void error(const Bifrost::API::String& str, ...);
    void warn(const Bifrost::API::String& str, ...);

private:
    Bifrost::API::StringArray _warnings;
    Bifrost::API::String _error;
};

class Shape{
public:
    Bifrost::API::String cache_file;
    Bifrost::API::String tmp_folder;
    Bifrost::API::String object;
    RenderComponent render_component = Volume;
    Bifrost::API::StringArray channels;
    Bifrost::API::StringArray velocity_channels;
    Bifrost::API::String uv_channel;

    float velocity_scale = 1;
    float space_scale = 1;
    unsigned int fps = 24;

    bool clip = false;
    amino::Math::bboxf clip_bbox = amino::Math::bboxf(amino::Math::vec3f(-1,-1,-1), amino::Math::vec3f(1,1,1));

    virtual Status initialize();
    virtual Bifrost::API::String str() const;

    inline const Bifrost::API::Component& component() const{ return _component; }

protected:
    Bifrost::API::Component _component;
};

}} // Bifrost::Processing
