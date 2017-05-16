#include <bifrostprocessing/bifrostprocessing_decl.h>
#include <string>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl

#define FOR_IJK(i,j,k,w) for(int i = 0; i < w; ++i) for(int j = 0; j < w; ++j) for(int k = 0; k < w; ++k)

#define INSTANCIATE(classname)\
    template class BIFROSTPROCESSING_API_DECL classname<float>;\
    template class BIFROSTPROCESSING_API_DECL classname<int32_t>;\
    template class BIFROSTPROCESSING_API_DECL classname<int64_t>;\
    template class BIFROSTPROCESSING_API_DECL classname<uint32_t>;\
    template class BIFROSTPROCESSING_API_DECL classname<uint64_t>;\
    template class BIFROSTPROCESSING_API_DECL classname<amino::Math::vec2f>;\
    template class BIFROSTPROCESSING_API_DECL classname<amino::Math::vec3f>;\
    template class BIFROSTPROCESSING_API_DECL classname<amino::Math::vec2i>;\
    template class BIFROSTPROCESSING_API_DECL classname<amino::Math::vec3i>

#define PROCESSING_PROFILING
#ifdef PROCESSING_PROFILING
#include <iostream>
#include <chrono>

#define PROFILE Profiler p(__PRETTY_FUNCTION__)
#define PROFILER(name) Profiler p(name)

namespace{

class Profiler{
private:
    std::string name;
    std::chrono::high_resolution_clock::time_point start;
public:
    Profiler(const std::string& name)
        :name(name), start(std::chrono::high_resolution_clock::now()){}
    ~Profiler(){
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        std::cerr << "[BIFROST PROFILER] " << name << ": " << (microseconds/(double)1000000.) << "s" << std::endl;
    }
};

}

#else

#define PROFILE
#define PROFILER(name)

#endif

#include <aminomath/vec.h>

namespace amino{
namespace Math{
    template<unsigned int N, typename T>
    amino::Math::vec<N,T> operator/(const amino::Math::vec<N,T>& v1, const amino::Math::vec<N,T>& v2){
        vec<N,T> componentwise_division;
        for(unsigned int i=0; i<N; ++i) componentwise_division[i]=v1.v[i]/v2.v[i];
        return componentwise_division;
    }
}
}


