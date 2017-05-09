#include <bifrostprocessing/bifrostprocessing_filters.h>
#include <bifrostprocessing/bifrostprocessing_visitors.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_component.h>
#include <bifrostapi/bifrost_stateserver.h>
#include <bifrostapi/bifrost_om.h>
#include "defs.h"

#define SWAP(ch1, ch2) { API::VoxelChannel _tmp = ch1; ch1 = ch2; ch2 = _tmp; }

namespace Bifrost{
namespace Processing{

template<typename T>
OffsetFilter<T>::OffsetFilter(const T& amount) : _amount(amount) {}

template<typename T>
void OffsetFilter<T>::filter(const API::VoxelChannel &in, API::VoxelChannel &out){
    assert(in.layout() == out.layout());
    OffsetVisitor<T> visitor(in, out, _amount);
    API::Layout(in.layout()).traverse(visitor, API::TraversalMode::ParallelBreadthFirst);
}

template<typename T>
DilateFilter<T>::DilateFilter(const T& amount) : OffsetFilter<T>(-amount){}

template<typename T>
ErodeFilter<T>::ErodeFilter(const T& amount) : OffsetFilter<T>(amount){}

template<typename T>
SmoothFilter<T>::SmoothFilter(Mode mode, unsigned int iterations, float blend)
    : _mode(mode), _iterations(iterations), blend(blend) {}

namespace{

template<typename T>
Bifrost::API::Channel copy(const Bifrost::API::VoxelChannel& in, const Bifrost::API::String& name){
    Bifrost::API::Layout layout(in.layout());
    Bifrost::API::StateServer ss(Bifrost::API::ObjectModel().stateServer(in.stateID()));

    Bifrost::API::VoxelChannel out = ss.createChannel(Bifrost::API::Component(in.component()), in.dataType(), name);
    out.setOffsetType (in.offsetType());
    out.setBackgroundValue<T>(in.backgroundValue<T>());

    CopyVisitor<T> visitor(in,out);
    layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst);

    return out;
}

}

template<typename T>
void SmoothFilter<T>::filter(const API::VoxelChannel &_in, API::VoxelChannel &out){
    Bifrost::API::VoxelChannel in(_in); // create new reference for swapping
    assert(in.layout() == out.layout());

    Bifrost::API::Layout layout(in.layout());
    Bifrost::API::StateServer ss(Bifrost::API::ObjectModel().stateServer(in.stateID()));

    unsigned int n = _iterations;
    if(_mode == kGaussian) n *= 4; // gaussian approximation handled as 4*mean

    Bifrost::API::VoxelChannel tmp = copy<T>(in, "tmp");
    bool need_copy = (blend != 1 && in == out);
    if(need_copy) out = copy<T>(in, "tmp2");

    if(n%2==1) SWAP(tmp,out); // => result in 'out' at the end of the loop
    for(unsigned int i = 0; i < n; ++i){
        SWAP(tmp,out);
        switch(_mode){
        case kMeanValue:
        case kGaussian: {// gaussian approximation handled as 4*mean
            MeanVisitor<T> visitor(tmp, out);
            layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst);
            break;
        }case kMedianValue:{
            MedianVisitor<T> visitor(tmp, out);
            layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst);
            break;
        }case kLaplacianFlow:{
            LaplacianFlowVisitor<T> visitor(tmp, out);
            layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst);
            break;
        }case kCurvatureFlow:{
            CurvatureFlowVisitor<T> visitor(tmp, out);
            layout.traverse(visitor, Bifrost::API::TraversalMode::ParallelBreadthFirst);
            break;
        }
        }
    }
    float blend = need_copy? this->blend : 1-this->blend;
    if(need_copy) SWAP(in,out);

    if(blend != 1){
        BlendVisitor<T> visitor(in, out, blend, out);
        layout.traverse(visitor, API::TraversalMode::ParallelBreadthFirst);
    }
    // clear temporary channels
    tmp.reset();
    in.reset();
    ss.removeChannel("tmp");
    if(need_copy) ss.removeChannel("tmp2");
}

INSTANCIATE(OffsetFilter);
INSTANCIATE(DilateFilter);
INSTANCIATE(ErodeFilter);
INSTANCIATE(SmoothFilter);

}} // namespace Bifrost::Processing
