#ifndef BBOX_H
#define BBOX_H

#include "definitions.h"

struct BBox
{
    vec3f limits[2];

    BBox();
    BBox(const vec3f &minPoint, const vec3f &maxPoint);
    BBox(const BBox& box);

    void set(const vec3f &minPoint, const vec3f &maxPoint);

    vec3f getCenter() const;
	float getCircumRadius() const;
    vec3f getDimensions() const;

    void getPoints(vec3f p[8]) const;

    bool intersectsPlane(vec3f n, float d) const;
    bool contains(vec3f p) const;
};


#endif // BBOX_H
