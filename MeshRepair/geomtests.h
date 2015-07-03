#ifndef GEOM_TESTS_H
#define GEOM_TESTS_H

#include "definitions.h"
#include "bbox.h"


float pointToTriangleUnsignedDist(const vec3f& p, const vec3f& v0, const vec3f& v1, const vec3f& v2);

float pointToSegmentUnsignedDist(const vec3f& p, const vec3f& s1, const vec3f& s2);

bool segmentTriangleIntersection(const vec3f& s0, const vec3f& s1, const vec3f& v0, const vec3f& v1, const vec3f& v2);

bool pointInPositiveHalfSpace(const vec3f& p, const vec3f& v0, const vec3f& v1, const vec3f& v2);


#endif // GEOM_TESTS_H
