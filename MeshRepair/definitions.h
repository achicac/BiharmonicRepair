#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "glm/glm.hpp"
#include <functional>

typedef glm::tvec3<float> vec3f;
typedef glm::tvec3<int>   vec3i;

namespace std
{
    template<>
    struct less<vec3i>
    {
    public:
        bool operator()(const vec3i &a, const vec3i &b) const
        {
            if (a.x != b.x) return a.x < b.x;
            if (a.y != b.y) return a.y < b.y;
            return a.z < b.z;
        }
    };
}




#endif // DEFINITIONS_H
