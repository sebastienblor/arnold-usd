#pragma once

#include <ai.h>

#include <algorithm>
#include <limits>
#include <vector>


//
// Intervals for collecting up intersections
//

struct Interval
{
   float t_start, t_end;
};

// Data structure to gather intersections from the RayIntersector.hits()
struct IntervalList
{
    std::vector<Interval> intervals_vec;

    IntervalList() : intervals_vec() { }

    void clear()
    {
        intervals_vec.clear();
    }

    size_t size() const
    {
        return intervals_vec.size();
    }

    bool empty() const
    {
        return intervals_vec.empty();
    }

    void reserve(size_t num)
    {
        intervals_vec.reserve(num);
    }

    void resize(size_t num)
    {
        intervals_vec.resize(num);
    }

    Interval& operator [](size_t i) { return intervals_vec[i]; }

    Interval& back() { return intervals_vec.back(); }

    void push_back(const Interval& i)
    {
        intervals_vec.push_back(i);
    }

    struct DistanceOrdering
    {
        bool operator ()(const Interval& a, const Interval& b) const
        {
            return a.t_start < b.t_start;
        }
    };

    void merge(size_t start_index = 0)
    {
        if (size() < 2 || start_index >= size() - 1)
            return;

        std::sort(intervals_vec.begin() + start_index, intervals_vec.begin() + (size() - start_index), DistanceOrdering());

        // merge overlapping steps
        size_t merged_i = start_index;
        for (size_t i = start_index; i < size(); merged_i++)
        {
            intervals_vec[merged_i] = intervals_vec[i];
            for (i++; i < size() && intervals_vec[i].t_start <= intervals_vec[merged_i].t_end; i++)
                intervals_vec[merged_i].t_end = std::max(intervals_vec[merged_i].t_end, intervals_vec[i].t_end);
        }
        resize(merged_i);
    }
};


// Detailed intersection methods for your primitive
struct PrimitiveIntersectorBase
{
    virtual ~PrimitiveIntersectorBase() { }

    virtual bool intersect(unsigned int prim, const AtVector& origin, const AtVector& direction, const AtVector& invDirection, float& t_start, float& t_end) = 0;
    virtual bool intersect(unsigned int prim, const AtVector& p) = 0;
};


struct BVHNode;


// Generic BVH, knows nothing about your primitives
class BVH
{
public:
    BVH(const std::vector<AtBBox>& primBBoxes);
    ~BVH();

    // Find all primitives that overlap/contain the point in question
    bool findOverlappingPrims(const AtVector& p,
                              std::vector<unsigned int>& prims,
                              PrimitiveIntersectorBase& intersector) const;

    // Find all intervals along the ray that hit primitives (for volumes)
    bool rayIntersections(const AtVector& origin,
                          const AtVector& direction,
                          float t_enter,
                          float t_exit,
                          IntervalList& intervals,
                          PrimitiveIntersectorBase& intersector) const;

    int64_t memUsage() const;

private:
    BVHNode *m_nodes;
    unsigned int m_num_nodes;
    unsigned int m_max_depth;

    void partitionNode(unsigned int begin,
                       unsigned int end,
                       std::vector<unsigned int>& prims,
                       const std::vector<AtBBox>& primBBoxes,
                       BVHNode& node,
                       unsigned int depth = 1);
};

