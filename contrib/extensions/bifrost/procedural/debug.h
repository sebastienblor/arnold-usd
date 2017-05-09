#pragma once

#include <iostream>
#include <string>
#include <chrono>

#define ASSERT_TRUE(v) { if(!(v)) AiMsgFatal("%d: failed: " #v, __LINE__); }
#define ASSERT_EQ(v1, v2) if(v1 != v2){ std::cerr << "line " << __LINE__ << " failed: " << #v1 " (" << v1 << ") != " << #v2 << " (" << v2 << ")" << std::endl; }
#define ASSERT_ASTR_EQ(a,b)
#define ASSERT_STRNE(a,b)
#define SCOPED_COMPARE(a,b)
#define ASSERT_FALSE(v) if(v) { std::cerr << "line " << __LINE__ << " failed: " << #v << " != false" << std::endl; }
#define ASSERT_FLOAT_EQ(a,b) ASSERT_EQ(a,b)
#define INFO(msg) std::cout << "[aminobif] Info: " << msg << std::endl
#define ERROR(msg) std::cerr << "[aminobif] Error: " << msg << std::endl; return false

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DL std::cerr << __FILENAME__ << ":" << __LINE__ << std::endl
#define DUMP(v) std::cerr << __FILENAME__ << ":" << __LINE__ << ": " << #v << " = " << (v) << std::endl

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
