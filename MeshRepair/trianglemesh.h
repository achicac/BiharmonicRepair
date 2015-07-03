#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <fstream>
#include <vector>
#include <iostream>
#include <map>
#include <utility>
#include "glm/glm.hpp"
#include "bbox.h"
#include "grid3D.h"


class TriangleMesh
{

public:
	TriangleMesh();
    ~TriangleMesh();

    bool loadPLY(const std::string& filename);
    bool savePLY(const std::string& filename) const;
    void clear();

    void setVertices(const std::vector<vec3f>& verts, const std::vector<int>& vtable);
    void setHasBorder(bool border);

    void computeNormalsPerFace();
    void computeBoundingBox();
    void computeConnectivity();

    bool  hasBorder() const;
    BBox  getBoundingBox() const;
    int   getNumTriangles() const;
    int   getNumVertices() const;
    void  getTriangle(int idTriangle, int& v0, int& v1, int& v2) const;
    void  getTriangleVerts(int idTriangle, vec3f& v0, vec3f& v1, vec3f& v2) const;
    vec3f getVertex(int idVtx) const;
    void  setVertex(int idVtx, const vec3f& coords);
    int   getVertexIndex(int idxVtable) const;
    vec3f getFaceNormal(int idTriangle) const;
    float getFaceArea(int idTriangle) const;
    vec3f getFaceCenter(int idTriangle) const;
    int   getRightTriangle(int hedgeV1, int hedgeV2) const;
    std::vector<int> getVertexNeighbors(int v) const;

    int   detectHoles();
    int   getNumHoles() const;
    const std::vector<int>& getHole(int idHole) const;
    void  getHole    (int idHole, std::vector<int>& vtxIds) const;
    void  getBorder(std::vector<int>& vtxIds) const;

    int  addVertex(const vec3f& v);
    int  addTriangle(int v0, int v1, int v2);
    void removeTriangles(const std::vector<int>& trisToRemove);

private:

    bool loadHeader(std::ifstream &fin, int& nVerts, int& nFaces);
    void loadVertices(std::ifstream &fin, int numVertices);
    void loadFaces(std::ifstream &fin, int numTriangles);

private:

    std::vector<vec3f>  m_vertices;
    std::vector<vec3f>  m_normals;
    std::vector<int>    m_vTable;

    BBox m_boundingBox;
    bool m_hasBorder;

    std::vector<std::vector<int> >     m_holes;
    std::map<std::pair<int, int>, int> m_hedgeRightFace;
    std::vector<std::vector<int> >     m_vertNeighbors;
};


inline void TriangleMesh::setHasBorder(bool border) {
    m_hasBorder = border;
}

inline bool TriangleMesh::hasBorder() const {
    return m_hasBorder;
}

inline BBox TriangleMesh::getBoundingBox() const {
    return m_boundingBox;
}

inline int TriangleMesh::getNumTriangles() const {
    return m_vTable.size()/3;
}

inline int TriangleMesh::getNumVertices() const {
    return m_vertices.size();
}

inline void TriangleMesh::getTriangle(int idTriangle, int &v0, int &v1, int &v2) const {
    v0 = m_vTable[3*idTriangle    ];
    v1 = m_vTable[3*idTriangle + 1];
    v2 = m_vTable[3*idTriangle + 2];
}

inline void TriangleMesh::getTriangleVerts(int idTriangle, vec3f &v0, vec3f &v1, vec3f &v2) const {
    v0 = m_vertices[m_vTable[3*idTriangle    ]];
    v1 = m_vertices[m_vTable[3*idTriangle + 1]];
    v2 = m_vertices[m_vTable[3*idTriangle + 2]];
}

inline vec3f TriangleMesh::getVertex(int idVtx) const {
    return m_vertices[idVtx];
}

inline void TriangleMesh::setVertex(int idVtx, const vec3f& coords) {
    m_vertices[idVtx] = coords;
}

inline int TriangleMesh::getVertexIndex(int idxVtable) const {
    return m_vTable[idxVtable];
}

inline vec3f TriangleMesh::getFaceNormal(int idTriangle) const {
    return m_normals[idTriangle];
}

inline float TriangleMesh::getFaceArea(int idTriangle) const {
    vec3f v0 = m_vertices[m_vTable[3*idTriangle    ]];
    vec3f v1 = m_vertices[m_vTable[3*idTriangle + 1]];
    vec3f v2 = m_vertices[m_vTable[3*idTriangle + 2]];
    return 0.5f*glm::length(glm::cross(v1 - v0, v2 - v0));
}

inline vec3f TriangleMesh::getFaceCenter(int idTriangle) const {
    vec3f v0 = m_vertices[m_vTable[3*idTriangle    ]];
    vec3f v1 = m_vertices[m_vTable[3*idTriangle + 1]];
    vec3f v2 = m_vertices[m_vTable[3*idTriangle + 2]];
    return (v0 + v1 + v2)/3.0f;
}

inline int TriangleMesh::getRightTriangle(int hedgeV1, int hedgeV2) const {
    std::pair<int, int> hedge(hedgeV1, hedgeV2);
    std::map<std::pair<int, int>, int>::const_iterator it = m_hedgeRightFace.find(hedge);
    if (it == m_hedgeRightFace.end())
        return -1;
    return it->second;
}

inline std::vector<int> TriangleMesh::getVertexNeighbors(int v) const {
    return m_vertNeighbors[v];
}

inline int TriangleMesh::getNumHoles() const {
    return m_holes.size() - (m_hasBorder ? 1 : 0);
}

inline void TriangleMesh::getBorder(std::vector<int> &vtxIds) const {
    if (m_hasBorder) {
        vtxIds = m_holes.back();
    }
}

inline const std::vector<int>& TriangleMesh::getHole(int idHole) const {
    return m_holes[idHole];
}

inline void TriangleMesh::getHole(int idHole, std::vector<int> &vtxIds) const {
    vtxIds = m_holes[idHole];
}


inline int TriangleMesh::addVertex(const vec3f &v) {
    m_vertices.push_back(v);
    return m_vertices.size()-1;
}

inline int TriangleMesh::addTriangle(int v0, int v1, int v2) {
    int tid = getNumTriangles();
    m_vTable.push_back(v0);
    m_vTable.push_back(v1);
    m_vTable.push_back(v2);
    return tid;
}

#endif // TRIANGLEMESH_H
