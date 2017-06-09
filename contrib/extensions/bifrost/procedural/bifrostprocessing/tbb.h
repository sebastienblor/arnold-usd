#pragma once

#include <functional>
#include <tbb/parallel_for.h>

template<typename T>
inline void TBB_FOR_ALL(T start, T end, T block, const std::function<void(T i)>& function){
    tbb::parallel_for(tbb::blocked_range<T>(start, end, block),[&function](const tbb::blocked_range<T>& r){
        for(T i = r.begin(); i != r.end(); ++i)
            function(i);
    });
}
template<typename T>
inline void TBB_FOR_ALL(T start, T end, const std::function<void(T i)>& function){
    tbb::parallel_for(tbb::blocked_range<T>(start, end),[&function](const tbb::blocked_range<T>& r){
        for(T i = r.begin(); i != r.end(); ++i)
            function(i);
    });
}
template<typename T>
inline void TBB_FOR_ALL(T start, T end, T block, const std::function<void(T start, T end)>& function){
    tbb::parallel_for(tbb::blocked_range<T>(start, end, block),[&function](const tbb::blocked_range<T>& r){
        function(r.begin(), r.end());
    });
}
template<typename T>
inline void TBB_FOR_ALL(T start, T end, const std::function<void(T start, T end)>& function){
    tbb::parallel_for(tbb::blocked_range<T>(start, end),[&function](const tbb::blocked_range<T>& r){
        function(r.begin(), r.end());
    });
}
