#pragma once

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl

#define PROFILING
#ifdef PROFILING
#define PROFILE Profiler p(__PRETTY_FUNCTION__)
#define PROFILER(name) Profiler p(name)

#include <iostream>
#include <chrono>

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
