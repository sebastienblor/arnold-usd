#pragma once

#include <functional>
#include <tbb/parallel_for.h>

inline void TBB_FOR_ALL(size_t start, size_t end, size_t block, const std::function<void(size_t i)>& function){
    tbb::parallel_for(tbb::blocked_range<size_t>(start, end, block),[&function](const tbb::blocked_range<size_t>& r){
        for(size_t i = r.begin(); i != r.end(); ++i)
            function(i);
    });
}
inline void TBB_FOR_ALL(size_t start, size_t end, const std::function<void(size_t i)>& function){
    tbb::parallel_for(tbb::blocked_range<size_t>(start, end),[&function](const tbb::blocked_range<size_t>& r){
        for(size_t i = r.begin(); i != r.end(); ++i)
            function(i);
    });
}
inline void TBB_FOR_ALL(size_t start, size_t end, size_t block, const std::function<void(size_t start, size_t end)>& function){
    tbb::parallel_for(tbb::blocked_range<size_t>(start, end, block),[&function](const tbb::blocked_range<size_t>& r){
        function(r.begin(), r.end());
    });
}

inline void TBB_FOR_ALL(size_t start, size_t end, const std::function<void(size_t start, size_t end)>& function){
    tbb::parallel_for(tbb::blocked_range<size_t>(start, end),[&function](const tbb::blocked_range<size_t>& r){
        function(r.begin(), r.end());
    });
}
