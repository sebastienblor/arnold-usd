#include "tbb.h"
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

class ForAll{
private:
    std::function<void (size_t)> function;
public:
    ForAll(const std::function<void (size_t)>& function) : function(function) {}
    void operator()(const tbb::blocked_range<int>& r) const{
        for(int i = r.begin(); i != r.end(); i++)
            function(i);
    }
};

class ForAllInBlock{
private:
    std::function<void (size_t, size_t)> function;
public:
    ForAllInBlock(const std::function<void (size_t, size_t)>& function) : function(function) {}
    void operator()(const tbb::blocked_range<int>& r) const{
        function(r.begin(), r.end());
    }
};

void TBB_FOR_ALL(size_t start, size_t end, size_t block, std::function<void(size_t)> function){
    tbb::parallel_for(tbb::blocked_range<int>(start, end, block), ForAll(function));
}

void TBB_FOR_ALL(size_t start, size_t end, size_t block, std::function<void(size_t, size_t)> function){
    tbb::parallel_for(tbb::blocked_range<int>(start, end, block), ForAllInBlock(function));
}
