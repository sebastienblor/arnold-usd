#include <algorithm>
#include <iostream>

#include "mini_bvh.h"


// Debugging for BVH build
//#define PRINT_BVH 1

// Use strictly balanced BVH build (alternate strategy, usually is slower)
//#define BVH_BALANCED 1


// BVH node (internal)
struct BVHNode
{
    AtBBox m_bbox;
    unsigned int m_prim;
    unsigned int m_left;

    BVHNode() : m_prim(0), m_left(0)
    {
        m_bbox.init(std::numeric_limits<float>::max());
    }

    bool interiorNode()  const   { return m_prim == 0xffffffff; }
    void setInteriorNode()       { m_prim = 0xffffffff; }

    unsigned int prim() const            { return m_prim; }
    void         setLeaf(unsigned int p) { m_prim = p; }

    unsigned int left()  const { return m_left; }
    unsigned int right() const { return m_left + 1; }

    void setLeft(unsigned int index) { m_left = index; }
};


// Create the BVH, and build immediately
BVH::BVH(const std::vector<AtBBox>& primBBoxes)
    : m_nodes(NULL),
      m_num_nodes(0),
      m_max_depth(0)
{
    if (primBBoxes.empty()) return;

    // Up to 2N+1 possible total nodes (leaf + interior)
    m_nodes = new BVHNode[primBBoxes.size() * 2 + 1];

    // Start setup with the root node
    std::vector<unsigned int> prims(primBBoxes.size());
    for (unsigned int i = 0; i < primBBoxes.size(); ++i)
    {
        prims[i] = i;
        m_nodes[0].m_bbox = AiBBoxUnion(m_nodes[0].m_bbox, primBBoxes[i]);
    }
    m_num_nodes = 1;

    // Process the rest of the tree
    partitionNode(0, (unsigned int)prims.size(), prims, primBBoxes, m_nodes[0]);

#if PRINT_BVH
    std::cout << "Max depth: " << m_max_depth << std::endl;
    std::cout << "Num nodes: " << m_num_nodes << " of " << (primBBoxes.size() * 2 + 1) << std::endl;
#endif
}

BVH::~BVH()
{
    delete[] m_nodes;
}

int64_t BVH::memUsage() const
{
    return (int64_t)(sizeof(BVHNode) * m_num_nodes + sizeof(BVH));
}


// Find all primitives that overlap the given point
bool BVH::findOverlappingPrims(const AtVector& p,
                               std::vector<unsigned int>& prims,
                               PrimitiveIntersectorBase& intersector) const
{
    if (m_nodes == NULL)
        return false;

    unsigned int *stack = reinterpret_cast<unsigned int*>(::alloca(sizeof(unsigned int) * m_max_depth));
    stack[0] = 0;

    unsigned int si = 1;
    while (si > 0)
    {
        si--;
        unsigned int ni = stack[si];
        const BVHNode& node = m_nodes[ni];
        if (!node.m_bbox.inside(p))
            continue;
        if (!node.interiorNode())
        {
            if (intersector.intersect(node.prim(), p))
                prims.push_back(node.prim());
            continue;
        }
        stack[si] = node.left();
        stack[si + 1] = node.right();
        si += 2;
    }
    return !prims.empty();
}


// Support code and structures for ray intersection
namespace
{

// Tracking for a future node to examine during BVH traversal
struct NodeVisit
{
    unsigned int m_node;
    float m_tmin, m_tmax;
};

// Test a ray against a bounding box, outputting the intersection interval if it intersects
bool intersects(const AtVector& origin, const AtVector& invDirection, const AtBBox& bbox, float& t0, float& t1)
{
    AtVector vt0 = (bbox.min - origin) * invDirection;
    AtVector vt1 = (bbox.max - origin) * invDirection;
    AtVector vtNear = AiV3Min(vt0, vt1);
    AtVector vtFar = AiV3Max(vt0, vt1);
    float btMin = std::max(std::max(vtNear.x, vtNear.y), vtNear.z);
    float btMax = std::min(std::min(vtFar.x, vtFar.y), vtFar.z);
    t0 = std::max(btMin, t0);
    t1 = std::min(btMax, t1);
    return t1 > t0;
}

} // namespace


// Ray intersection, accumulating spans along the ray that overlap primitives
bool BVH::rayIntersections(const AtVector& origin,
                           const AtVector& direction,
                           float t_enter,
                           float t_exit,
                           IntervalList& intervals,
                           PrimitiveIntersectorBase& intersector) const
{
    if (!m_nodes)
        return false;

    AtVector invDirection = AI_V3_ONE / direction;

    if (!intersects(origin, invDirection, m_nodes[0].m_bbox, t_enter, t_exit))
        return false;

    NodeVisit *stack = reinterpret_cast<NodeVisit*>(::alloca(sizeof(NodeVisit) * m_max_depth));
    stack[0].m_node = 0;
    stack[0].m_tmin = t_enter;
    stack[0].m_tmax = t_exit;

    unsigned int si = 1;
    while (si > 0)
    {
        si--;
        NodeVisit& nv = stack[si];
        const BVHNode& node = m_nodes[nv.m_node];
        if (!node.interiorNode())
        {
            float prim_t_start = nv.m_tmin;
            float prim_t_end = nv.m_tmax;
            if (intersector.intersect(node.prim(), origin, direction, invDirection, prim_t_start, prim_t_end))
            {
                // We don't do any fancy attempt at merging with previous intervals;
                // a final merge will be done all at once at the end.  Note that
                // since the intervals passed in are per-thread and will only very
                // rarely trigger an allocation, this should generally be efficient
                // to just collect up all the intervals individually.
                Interval interval = { prim_t_start, prim_t_end };
                intervals.push_back(interval);
            }
            continue;
        }

        float box1_t_start = nv.m_tmin;
        float box1_t_end = nv.m_tmax;
        float box2_t_start = nv.m_tmin;
        float box2_t_end = nv.m_tmax;
        bool box1_intersects = intersects(origin, invDirection, m_nodes[node.left()].m_bbox,  box1_t_start, box1_t_end);
        bool box2_intersects = intersects(origin, invDirection, m_nodes[node.right()].m_bbox, box2_t_start, box2_t_end);
        if (box1_intersects && box2_intersects && box2_t_start < box1_t_start)
        {
            stack[si].m_node = node.left();
            stack[si].m_tmin = box1_t_start;
            stack[si].m_tmax = box1_t_end;
            si++;
            stack[si].m_node = node.right();
            stack[si].m_tmin = box2_t_start;
            stack[si].m_tmax = box2_t_end;
            si++;
        }
        else
        {
            if (box2_intersects)
            {
                stack[si].m_node = node.right();
                stack[si].m_tmin = box2_t_start;
                stack[si].m_tmax = box2_t_end;
                si++;
            }
            if (box1_intersects)
            {
                stack[si].m_node = node.left();
                stack[si].m_tmin = box1_t_start;
                stack[si].m_tmax = box1_t_end;
                si++;
            }
        }
    }

    return !intervals.empty();
}

// Support code for BVH build
namespace
{

const unsigned int X_AXIS = 0;
const unsigned int Y_AXIS = 1;
const unsigned int Z_AXIS = 2;

template<int Axis>
struct Compare
{
    const std::vector<AtBBox>& bboxes;

    Compare(const std::vector<AtBBox>& primBBoxes) : bboxes(primBBoxes) { }

    bool operator ()(unsigned int p0, unsigned int p1)
    {
        if (Axis == 0)
            return bboxes[p0].min.x < bboxes[p1].min.x;
        else if (Axis == 1)
            return bboxes[p0].min.y < bboxes[p1].min.y;
        else //if (Axis == 2)
            return bboxes[p0].min.z < bboxes[p1].min.z;
    }
};

} // namespace

#if BVH_BALANCED

// BVH build (currently a simple spatial split scheme)
void BVH::partitionNode(unsigned int begin,
                        unsigned int end,
                        std::vector<unsigned int>& prims,
                        const std::vector<AtBBox>& primBBoxes,
                        BVHNode& node,
                        unsigned int depth)
{
    if (depth > m_max_depth)
        m_max_depth = depth;

#if PRINT_BVH
static unsigned int indent = 0;
#endif
    if (end - begin == 1)
    {
#if PRINT_BVH
        for (unsigned int i = 0; i < indent; ++i)
            std::cout << '.';
        std::cout << "Prim: " << prims[begin] << " (" << node.m_bbox.min.x << ' ' << node.m_bbox.min.y << ' ' << node.m_bbox.min.z << ") -> ("
              << node.m_bbox.max.x << ' ' << node.m_bbox.max.y << ' ' << node.m_bbox.max.z << ")" << std::endl;
#endif
        node.setLeaf(prims[begin]);
        return;
    }

    // Decide split dimension and location
    AtVector extents = node.m_bbox.max - node.m_bbox.min;
    unsigned int splitAxis = extents.x >= extents.y ? (extents.x >= extents.z ? X_AXIS : Z_AXIS) : (extents.y >= extents.z ? Y_AXIS : Z_AXIS);

    if (splitAxis == X_AXIS)
    {
        Compare<X_AXIS> comparator(primBBoxes);
        std::sort(prims.begin() + begin, prims.begin() + end, comparator);
    }
    else if (splitAxis == Y_AXIS)
    {
        Compare<Y_AXIS> comparator(primBBoxes);
        std::sort(prims.begin() + begin, prims.begin() + end, comparator);
    }
    else
    {
        Compare<Z_AXIS> comparator(primBBoxes);
        std::sort(prims.begin() + begin, prims.begin() + end, comparator);
    }

#if PRINT_BVH
    for (unsigned int i = 0; i < indent; ++i)
        std::cout << '.';
    std::cout << "I: (" << node.m_bbox.min.x << ' ' << node.m_bbox.min.y << ' ' << node.m_bbox.min.z << ") -> ("
              << node.m_bbox.max.x << ' ' << node.m_bbox.max.y << ' ' << node.m_bbox.max.z << ")"
              << " split: " << (char)(splitAxis + 'X') << ' ' << split << " range: (" << begin << ',' << examineIndex << ',' << end << ")" << std::endl;
    indent++;
#endif

    unsigned int split = begin + (end - begin) / 2;

    // Set the left (the right is the one just after), and allocate them
    node.setInteriorNode();
    node.setLeft(m_num_nodes);
    m_num_nodes += 2;

    // Build the left child bbox and continue the tree below it
    AtBBox& leftBBox = m_nodes[node.left()].m_bbox;
    for (unsigned int i = begin; i < split; ++i)
    {
        AiBBoxUnion(leftBBox, leftBBox, primBBoxes[prims[i]]);
    }
    partitionNode(begin, split, prims, primBBoxes, m_nodes[node.left()], depth + 1);

    // Build the right child bbox and continue the tree below it
    AtBBox& rightBBox = m_nodes[node.right()].m_bbox;
    for (unsigned int i = split; i < end; ++i)
    {
        AiBBoxUnion(rightBBox, rightBBox, primBBoxes[prims[i]]);
    }
    partitionNode(split, end, prims, primBBoxes, m_nodes[node.right()], depth + 1);
#if PRINT_BVH
    indent--;
#endif
}

#else // BVH_BALANCED

// BVH build (currently a simple spatial split scheme)
void BVH::partitionNode(unsigned int begin,
                        unsigned int end,
                        std::vector<unsigned int>& prims,
                        const std::vector<AtBBox>& primBBoxes,
                        BVHNode& node,
                        unsigned int depth)
{
    if (depth > m_max_depth)
        m_max_depth = depth;

#if PRINT_BVH
static unsigned int indent = 0;
#endif
    if (end - begin == 1)
    {
#if PRINT_BVH
        for (unsigned int i = 0; i < indent; ++i)
            std::cout << '.';
        std::cout << "Prim: " << prims[begin] << " (" << node.m_bbox.min.x << ' ' << node.m_bbox.min.y << ' ' << node.m_bbox.min.z << ") -> ("
              << node.m_bbox.max.x << ' ' << node.m_bbox.max.y << ' ' << node.m_bbox.max.z << ")" << std::endl;
#endif
        node.setLeaf(prims[begin]);
        return;
    }

    // Decide split dimension and location
    AtVector extents = node.m_bbox.max - node.m_bbox.min;
    unsigned int splitAxis = extents.x >= extents.y ? (extents.x >= extents.z ? X_AXIS : Z_AXIS) : (extents.y >= extents.z ? Y_AXIS : Z_AXIS);
    float split;
    if (splitAxis == X_AXIS)
        split = (node.m_bbox.min.x + node.m_bbox.max.x) * 0.5f;
    else if (splitAxis == Y_AXIS)
        split = (node.m_bbox.min.y + node.m_bbox.max.y) * 0.5f;
    else
        split = (node.m_bbox.min.z + node.m_bbox.max.z) * 0.5f;

    unsigned int examineIndex = begin;
    unsigned int swapIndex = end - 1;
    unsigned int leftMost = begin;
    unsigned int rightMost = begin;
    float leftMostCoord = splitAxis == X_AXIS ? primBBoxes[prims[begin]].min.x :
                         (splitAxis == Y_AXIS ? primBBoxes[prims[begin]].min.y :
                                                primBBoxes[prims[begin]].min.z);
    float rightMostCoord = splitAxis == X_AXIS ? primBBoxes[prims[begin]].max.x :
                          (splitAxis == Y_AXIS ? primBBoxes[prims[begin]].max.y :
                                                 primBBoxes[prims[begin]].max.z);
    while (examineIndex <= swapIndex)
    {
        float median;
        if (splitAxis == X_AXIS)
            median = (primBBoxes[prims[examineIndex]].min.x + primBBoxes[prims[examineIndex]].max.x) * 0.5f;
        else if (splitAxis == Y_AXIS)
            median = (primBBoxes[prims[examineIndex]].min.y + primBBoxes[prims[examineIndex]].max.y) * 0.5f;
        else // if (splitAxis == Z_AXIS)
            median = (primBBoxes[prims[examineIndex]].min.z + primBBoxes[prims[examineIndex]].max.z) * 0.5f;

        if (median > split)
        {
            float rightCoord = splitAxis == X_AXIS ? primBBoxes[prims[examineIndex]].max.x :
                              (splitAxis == Y_AXIS ? primBBoxes[prims[examineIndex]].max.y :
                                                     primBBoxes[prims[examineIndex]].max.z);
            if (rightCoord > rightMostCoord)
            {
                rightMostCoord = rightCoord;
                rightMost = examineIndex;
            }

            unsigned int tempPrim = prims[swapIndex];
            prims[swapIndex] = prims[examineIndex];
            prims[examineIndex] = tempPrim;

            --swapIndex;
        }
        else
        {
            float leftCoord = splitAxis == X_AXIS ? primBBoxes[prims[examineIndex]].min.x :
                             (splitAxis == Y_AXIS ? primBBoxes[prims[examineIndex]].min.y :
                                                    primBBoxes[prims[examineIndex]].min.z);
            if (leftCoord < leftMostCoord)
            {
                leftMostCoord = leftCoord;
                leftMost = examineIndex;
            }

            ++examineIndex;
        }
    }
    // If the tree is extremely unbalanced at this point, sort it and split with equal number of prims in each branch
    const unsigned int unbalancedThreshold = 1;
    if (examineIndex <= begin + unbalancedThreshold || examineIndex >= end - unbalancedThreshold)
    {
        if (splitAxis == X_AXIS)
        {
            Compare<X_AXIS> comparator(primBBoxes);
            std::sort(prims.begin() + begin, prims.begin() + end, comparator);
        }
        else if (splitAxis == Y_AXIS)
        {
            Compare<Y_AXIS> comparator(primBBoxes);
            std::sort(prims.begin() + begin, prims.begin() + end, comparator);
        }
        else
        {
            Compare<Z_AXIS> comparator(primBBoxes);
            std::sort(prims.begin() + begin, prims.begin() + end, comparator);
        }
        examineIndex = begin + (end - begin) / 2;
        swapIndex = examineIndex - 1;
    }

#if PRINT_BVH
    for (unsigned int i = 0; i < indent; ++i)
        std::cout << '.';
    std::cout << "I: (" << node.m_bbox.min.x << ' ' << node.m_bbox.min.y << ' ' << node.m_bbox.min.z << ") -> ("
              << node.m_bbox.max.x << ' ' << node.m_bbox.max.y << ' ' << node.m_bbox.max.z << ")"
              << " split: " << (char)(splitAxis + 'X') << ' ' << split << " range: (" << begin << ',' << examineIndex << ',' << end << ")" << std::endl;
    indent++;
#endif

    // Set the left (the right is the one just after), and allocate them
    node.setInteriorNode();
    node.setLeft(m_num_nodes);
    m_num_nodes += 2;

    // Build the left child bbox and continue the tree below it
    AtBBox& leftBBox = m_nodes[node.left()].m_bbox;
    for (unsigned int i = begin; i < examineIndex; ++i)
    {
        leftBBox = AiBBoxUnion(leftBBox, primBBoxes[prims[i]]);
    }
    partitionNode(begin, examineIndex, prims, primBBoxes, m_nodes[node.left()], depth + 1);

    // Build the right child bbox and continue the tree below it
    AtBBox& rightBBox = m_nodes[node.right()].m_bbox;
    for (unsigned int i = swapIndex + 1; i < end; ++i)
    {
        rightBBox = AiBBoxUnion(rightBBox, primBBoxes[prims[i]]);
    }
    partitionNode(swapIndex + 1, end, prims, primBBoxes, m_nodes[node.right()], depth + 1);

#if PRINT_BVH
    indent--;
#endif
}

#endif // BVH_BALANCED
