#ifndef BVHH
#define BVHH

#include "hitable.h"
#include <algorithm>

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * drand48();
}

inline int random_int(int min, int max) {
    // Returns a random integer in [min,max].
    return int(random_double(min, max + 1));
}

class bvh_node : public hitable  {
    public:
        bvh_node() {}
        bvh_node(hitable **l, size_t n): bvh_node(l, 0, n) {}
        bvh_node(hitable **l, size_t start, size_t end);
        virtual ~bvh_node();
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
        hitable *left;
        hitable *right; 
        aabb bbox;
        aabb bounding_box() const { return bbox; }

        void print() const override {
            std::cerr << "BVHNode {" << std::endl;
                
            bbox.print();

            std::cerr << "\t";
            if (left) left->print();
            std::cerr << "\t";
            if (right) right->print();
            
            std::cerr << "}" << std::endl;
        }
};

bool bvh_node::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    if (!bbox.hit(r, t_min, t_max))
        return false;

    // check intersections for the left and right children
    // make sure they are not nullptr first
    bool hit_left = left && left->hit(r, t_min, t_max, rec);
    bool hit_right = right && right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}


static bool box_compare(hitable* a, hitable* b, int axis_index) {
    return a->bounding_box().min()[axis_index] < b->bounding_box().min()[axis_index];
}

static bool box_x_compare (hitable* a, hitable* b) {
    return box_compare(a, b, 0);
}
static bool box_y_compare (hitable* a, hitable* b) {
    return box_compare(a, b, 1);
}
static bool box_z_compare (hitable* a, hitable* b) {
    return box_compare(a, b, 2);
}

bvh_node::bvh_node(hitable **l, size_t start, size_t end) {
    size_t object_span = end - start;
    if (object_span < 1)
        // left = nullptr;
        // right = nullptr;
        return;
        
    // use the longest axis heuristic
    bbox = l[start]->bounding_box();
    for (size_t i=start; i < end; i++)
        bbox = aabb(bbox, l[i]->bounding_box());
    int axis = bbox.longest_axis();

    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                    : box_z_compare;

    if (object_span == 1) {
        left = l[start];
        // right = nullptr;
    } else if (object_span == 2) {
        left = l[start];
        right = l[start+1];
    } else {
        std::sort(l + start, l + end, comparator);
        auto mid = start + object_span/2;
        left = new bvh_node(l, start, mid);
        right = new bvh_node(l, mid, end);
    }
}

bvh_node::~bvh_node() {
    delete left;
    delete right;
}

#endif