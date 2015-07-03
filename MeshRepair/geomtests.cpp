#include "geomtests.h"
#include <algorithm>

using namespace glm;

void getBarycentricCoordinates(const vec3f &p, const vec3f &v0, const vec3f &v1, const vec3f &v2, vec3f &bc)
{
    vec3f e01 = v1 - v0;
    vec3f e02 = v2 - v0;
    vec3f e0p = p - v0;
    float dot00 = dot(e01, e01);
    float dot01 = dot(e01, e02);
    float dot02 = dot(e01, e0p);
    float dot11 = dot(e02, e02);
    float dot12 = dot(e02, e0p);
    float invDenom = 1.0f/(dot00*dot11 - dot01*dot01);
    float u = (dot11*dot02 - dot01*dot12)*invDenom;
    float v = (dot00*dot12 - dot01*dot02)*invDenom;
    bc = vec3f(u, v, 1 - u - v);
}


float pointToTriangleUnsignedDist(const vec3f& p, const vec3f& v0,  const vec3f& v1,  const vec3f& v2)
{
    vec3f nplane = normalize(cross(v1 - v0, v2 - v0));
    float dplane = -dot(nplane, v0);

    float distToPlane = dot(nplane, p) + dplane;
    vec3f pProj = p - distToPlane*nplane;

    // check if projection inside triangle, in that case return dist to plane
    vec3f baryCoords;
    getBarycentricCoordinates(pProj, v0, v1, v2, baryCoords);
    float u = baryCoords.x;
    float v = baryCoords.y;
    if ((u >= 0) && (v >= 0) && (u + v < 1)) {
        return std::abs(distToPlane);
    }

    // check distance against edges and vertices
    float distv0 = length(v0 - p);
    float distv1 = length(v1 - p);
    float distv2 = length(v2 - p);

    vec3f u01 = normalize(v1 - v0);
    vec3f u12 = normalize(v2 - v1);
    vec3f u20 = normalize(v0 - v2);
    float disp01 = dot(p - v0, u01);
    float disp12 = dot(p - v1, u12);
    float disp20 = dot(p - v2, u20);
    vec3f proj01 = disp01*u01 + v0;
    vec3f proj12 = disp12*u12 + v1;
    vec3f proj20 = disp20*u20 + v2;
    float diste0 = length(p - proj01);
    float diste1 = length(p - proj12);
    float diste2 = length(p - proj20);
    if (disp01 < 0 || disp01 > length(v1 - v0)) diste0 = 1e32f;
    if (disp12 < 0 || disp12 > length(v2 - v1)) diste1 = 1e32f;
    if (disp20 < 0 || disp20 > length(v0 - v2)) diste2 = 1e32f;

    float distv = std::min(std::min(distv0, distv1), distv2);
    float diste = std::min(std::min(diste0, diste1), diste2);
    return std::min(diste, distv);
}


float pointToSegmentUnsignedDist(const vec3f& p, const vec3f& s1, const vec3f& s2)
{
    if (length(s2 - s1) < 1e-6) return 1e32f;
    vec3f  u     = normalize(s2 - s1);
    float pdist = dot(p - s1, u);
    if (pdist < 0 || pdist > length(s2 - s1)) return 1e32f;
    vec3f  pproj = pdist*u + s1;
    float dist  = length(pproj - p);
    return dist;
}


bool segmentTriangleIntersection(const vec3f& s0, const vec3f& s1, const vec3f& v0, const vec3f& v1, const vec3f& v2)
{
    vec3f  nplane = normalize(cross(v1 - v0, v2 - v0));
    float dplane = -dot(nplane, v0);

    // check stick on oposite sides
    float d0 = dot(nplane, s0) + dplane;
    float d1 = dot(nplane, s1) + dplane;
    if (d0*d1 > 0) return false;

    // check if projection inside triangle
    vec3f  s = normalize(s1 - s0);
    float t = -(dot(nplane, s0) + dplane)/dot(nplane, s);
    vec3f pInter = s0 + t*s;
    vec3f baryCoords;
    getBarycentricCoordinates(pInter, v0, v1, v2, baryCoords);
    float u = baryCoords.x;
    float v = baryCoords.y;
    float eps = 0.0;
    return (u >= -eps) && (v >= -eps) && (u + v <= 1 + eps);
}


bool pointInPositiveHalfSpace(const vec3f& p, const vec3f& v0, const vec3f& v1, const vec3f& v2)
{
    vec3f  nplane = normalize(cross(v1 - v0, v2 - v0));
    float dplane = -dot(nplane, v0);
    return dot(nplane, p) + dplane >= 0;
}
