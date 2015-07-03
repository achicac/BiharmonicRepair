#include "bbox.h"

BBox::BBox()
{
}

BBox::BBox(const glm::vec3 &minPoint, const glm::vec3 &maxPoint)
{
	limits[0] = minPoint;
	limits[1] = maxPoint;
}

BBox::BBox(const BBox &box)
{
    limits[0] = box.limits[0];
    limits[1] = box.limits[1];
}

void BBox::set(const glm::vec3 &minPoint, const glm::vec3 &maxPoint)
{
	limits[0] = minPoint;
	limits[1] = maxPoint;
}

glm::vec3 BBox::getCenter() const
{
    glm::vec3 center = glm::vec3( (limits[0].x + limits[1].x) / 2.f,
                                  (limits[0].y + limits[1].y) / 2.f,
                                  (limits[0].z + limits[1].z) / 2.f);

	return center;
}

float BBox::getCircumRadius() const
{
	glm::vec3 size = limits[1] - limits[0];
	return glm::length(size) / 2.f;
}

glm::vec3 BBox::getDimensions() const
{
    return limits[1] - limits[0];
}

void BBox::getPoints(glm::vec3 p[8]) const
{
    for (int i = 0; i < 8; i++) {
        p[i] = glm::vec3(limits[i%2].x, limits[(i/2)%2].y, limits[i/4].z);
    }
}

bool BBox::intersectsPlane(glm::vec3 n, float d) const
{
    float side = glm::dot(limits[0], n) + d;
    for (int i = 1; i < 8; i++) {
        glm::vec3 p(limits[i%2].x, limits[(i/2)%2].y, limits[i/4].z);
        if ((glm::dot(p, n) + d)*side <= 0) return true;
    }
    return false;
}

bool BBox::contains(glm::vec3 p) const
{
    return p.x >= limits[0].x && p.y >= limits[0].y && p.z >= limits[0].z
        && p.x <= limits[1].x && p.y <= limits[1].y && p.z <= limits[1].z;
}

