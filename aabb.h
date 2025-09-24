#ifndef AABBH
#define AABBH
#include "ray.h"
#include "hitable.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class aabb {
    public:
        aabb() {}
        aabb(const vec3& a, const vec3& b) { _min = a; _max = b;}  
        aabb(const aabb& box0, const aabb& box1) { 
            for (int a = 0; a < 3; a++) {
                _min[a] = ffmin(box0._min[a], box1._min[a]);
                _max[a] = ffmax(box0._max[a], box1._max[a]);
            }
        }  

        vec3 min() const {return _min; }
        vec3 max() const {return _max; }

        bool hit(const ray& r, float tmin, float tmax) const {
            for (int a = 0; a < 3; a++) {
                float t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a],  
                                (_max[a] - r.origin()[a]) / r.direction()[a]);
                float t1 = ffmax((_min[a] - r.origin()[a]) / r.direction()[a],  
                                (_max[a] - r.origin()[a]) / r.direction()[a]);
                tmin = ffmax(t0, tmin);
                tmax = ffmin(t1, tmax);
                if (tmax <= tmin)
                    return false;
            }
            return true;
        }

        int longest_axis() const {
            float x_size = abs(_max.x() - _min.x());
            float y_size = abs(_max.y() - _min.y());
            float z_size = abs(_max.z() - _min.z());
            if (x_size > y_size)
                return x_size > z_size ? 0 : 2;
            else
                return y_size > z_size ? 1 : 2;
        }

        void print() const {
            std::cerr << "\tBBox: [" 
                      << _min.x() << ", " << _min.y() << ", " << _min.z()
                      << "] to ["
                      << _max.x() << ", " << _max.y() << ", " << _max.z()
                      << "]" << std::endl;
        }

        vec3 _min;
        vec3 _max;
};

#endif
