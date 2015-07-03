#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <utility>
#include <stack>
#include <queue>
#include <map>
#include <cstring>
#include "trianglemesh.h"


TriangleMesh::TriangleMesh()
{
}

TriangleMesh::~TriangleMesh()
{
}



bool TriangleMesh::loadPLY(const std::string& fname)
{
    std::ifstream fin;

    fin.open(fname.c_str(), std::ios_base::in | std::ios_base::binary);
    if(!fin.is_open())
        return false;
    int nVerts, nFaces;
    if(!loadHeader(fin, nVerts, nFaces))
    {
        fin.close();
        return false;
    }

    loadVertices(fin, nVerts);
    loadFaces(fin, nFaces);
    fin.close();

    computeNormalsPerFace();
    computeBoundingBox();

    return true;
}



bool TriangleMesh::loadHeader(std::ifstream &fin, int& nVertices, int& nFaces)
{
    char line[100];

    fin.getline(line, 100);
    if (strncmp(line, "ply", 3) != 0)
        return false;

    nVertices = 0;
    nFaces = 0;

    fin.getline(line, 100);
    while (strncmp(line, "end_header", 10) != 0)
    {
        if(strncmp(line, "element vertex", 14) == 0)
            nVertices = atoi(&line[15]);
        fin.getline(line, 100);
        if(strncmp(line, "element face", 12) == 0)
            nFaces = atoi(&line[13]);
    }
    if(nVertices <= 0)
        return false;

    std::cout << "Loading triangle mesh..." << std::endl;
    std::cout << "    verts: " << nVertices << std::endl;
    std::cout << "    faces: " << nFaces << std::endl;

    return true;
}

void TriangleMesh::loadVertices(std::ifstream &fin, int nVertices)
{
    m_vertices.resize(nVertices);
    fin.read(reinterpret_cast<char*>(&m_vertices[0].x), sizeof(vec3f)*nVertices);
}

void TriangleMesh::loadFaces(std::ifstream &fin, int nFaces)
{
    int tri[3];
    unsigned char nVrtxPerFace;
    for(int i = 0; i < nFaces; i++)
    {
        fin.read(reinterpret_cast<char*>(&nVrtxPerFace), sizeof(unsigned char));
        fin.read(reinterpret_cast<char*>(&tri[0]), 3*sizeof(int));
        m_vTable.push_back(tri[0]);
        m_vTable.push_back(tri[1]);
        m_vTable.push_back(tri[2]);
        for(; nVrtxPerFace>3; nVrtxPerFace--)
        {
            tri[1] = tri[2];
            fin.read(reinterpret_cast<char*>(&tri[2]), sizeof(int));
            m_vTable.push_back(tri[0]);
            m_vTable.push_back(tri[1]);
            m_vTable.push_back(tri[2]);
        }
    }
}



bool TriangleMesh::savePLY(const std::string &filename) const
{
    std::fstream fout;
    fout.open(filename.c_str(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    if (!fout.good())
        return false;

    fout << "ply" << std::endl;
    fout << "format binary_little_endian 1.0" << std::endl;
    fout << "element vertex " << getNumVertices() << std::endl;
    fout << "property float x" << std::endl;
    fout << "property float y" << std::endl;
    fout << "property float z" << std::endl;
    fout << "element face " << getNumTriangles() << std::endl;
    fout << "property list uchar int vertex_indices" << std::endl;
    fout << "end_header" << std::endl;

    fout.write(reinterpret_cast<const char*>(&m_vertices[0].x), sizeof(vec3f)*m_vertices.size());
    for (unsigned int i = 0; i < m_vTable.size(); i += 3) {
        unsigned char aux = 3;
        fout.write(reinterpret_cast<const char*>(&aux), sizeof(unsigned char));
        fout.write(reinterpret_cast<const char*>(&m_vTable[i]), sizeof(int));
        fout.write(reinterpret_cast<const char*>(&m_vTable[i+1]), sizeof(int));
        fout.write(reinterpret_cast<const char*>(&m_vTable[i+2]), sizeof(int));
    }

    fout.close();

    return true;
}


void TriangleMesh::clear()
{
    m_vertices.clear();
    m_normals.clear();
    m_vTable.clear();
}



void TriangleMesh::setVertices(const std::vector<glm::vec3>& verts, const std::vector<int>& vtable)
{
    m_vertices = verts;
    m_vTable = vtable;
}

void TriangleMesh::computeNormalsPerFace()
{
    m_normals.clear();
    m_normals.reserve(getNumTriangles());
    for (unsigned int i = 0; i < m_vTable.size(); i += 3)
    {
        glm::vec3 normal = glm::cross(m_vertices[m_vTable[i+1]] - m_vertices[m_vTable[i]],
                                      m_vertices[m_vTable[i+2]] - m_vertices[m_vTable[i]]);
        if (glm::length(normal) < 1e-5)
            normal = glm::vec3(0, 0, 0);
        else
            normal = glm::normalize(normal);
        m_normals.push_back(normal);
    }
}

void TriangleMesh::computeBoundingBox()
{
    if (m_vertices.size() > 0) {
        glm::vec3 bbmin = m_vertices[0];
        glm::vec3 bbmax = m_vertices[0];
        for (unsigned int i = 1; i < m_vertices.size(); i++) {
            bbmin.x = std::min(bbmin.x, m_vertices[i].x);
            bbmin.y = std::min(bbmin.y, m_vertices[i].y);
            bbmin.z = std::min(bbmin.z, m_vertices[i].z);
            bbmax.x = std::max(bbmax.x, m_vertices[i].x);
            bbmax.y = std::max(bbmax.y, m_vertices[i].y);
            bbmax.z = std::max(bbmax.z, m_vertices[i].z);
        }
        m_boundingBox.limits[0] = bbmin;
        m_boundingBox.limits[1] = bbmax;
    }
}


void TriangleMesh::computeConnectivity()
{
	std::vector<std::set<int> > auxNeighbors(getNumVertices());

    // construct a structure in which each halfedge points to its right triangle
    int numTriangles = getNumTriangles();
    for (int t = 0; t < numTriangles; t++) {
        int v[3];
        getTriangle(t, v[0], v[1], v[2]);
        for (int i = 0; i < 3; i++) {
            // current triangle edge v1 -> v2
            int v1 = v[i];
            int v2 = v[(i+1)%3];
            // the reverse half-edge will have triangle t as right face
            m_hedgeRightFace[std::make_pair(v2, v1)] = t;

            // also store the vertex neighborhoods
            auxNeighbors[v1].insert(v2);
            auxNeighbors[v2].insert(v1);
        }
    }

	// more comfortable to work with vectors from the outter api
    m_vertNeighbors = std::vector<std::vector<int> >(getNumVertices());
	for (unsigned int i = 0; i < auxNeighbors.size(); i++) {
		for (std::set<int>::iterator it = auxNeighbors[i].begin(); it != auxNeighbors[i].end(); it++) {
			m_vertNeighbors[i].push_back(*it);
		}
	}
}


int TriangleMesh::detectHoles()
{
    std::cout << "Detecting holes..." << std::endl;
    m_holes.clear();

    // get half edges without an opposite
    int numTriangles = getNumTriangles();

    std::set<std::pair<int, int> > hedges;
    for (int t = 0; t < numTriangles; t++) {
        for (int vi = 0; vi < 3; vi++) {
            int v0 = m_vTable[3*t +  vi];
            int v1 = m_vTable[3*t + (vi + 1)%3];
            std::pair<int, int> hedge(v1, v0);
            std::pair<int, int> oppos(v0, v1);
            std::set<std::pair<int, int> >::iterator iter = hedges.find(oppos);
            if (iter != hedges.end()) {
                hedges.erase(iter);
            } else {
                hedges.insert(hedge);
            }
        }
    }

    // construct these edges table
    std::vector<std::vector<int> > next(getNumVertices());
    for (std::set<std::pair<int,int> >::const_iterator it = hedges.begin(); it != hedges.end(); it++) {
        next[it->first].push_back(it->second);
    }

    // construct loops
    while (!hedges.empty()) {
        std::pair<int, int> initEdge = *hedges.begin();
        std::vector<int> loop;
        int v = initEdge.first;
        do {
            loop.push_back(v);
            int vnext = next[v][0];
            next[v].erase(next[v].begin());
            hedges.erase(std::pair<int, int>(v, vnext));
            v = vnext;
        } while (v != initEdge.first);

        m_holes.push_back(loop);
    }

    if (m_hasBorder) {
        if (m_holes.size() > 1) {
            int maxhole = 0;
            for (unsigned int i = 1; i < m_holes.size(); i++) {
                if (m_holes[i].size() > m_holes[maxhole].size()) {
                    maxhole = i;
                }
            }
            swap(m_holes[m_holes.size()-1], m_holes[maxhole]);
        }
    }

    return getNumHoles();
}



void TriangleMesh::removeTriangles(const std::vector<int>& trisToRemove)
{
	std::vector<bool> removedTriangle(getNumTriangles(), false);
    for (unsigned int i = 0; i < trisToRemove.size(); i++) {
        int t = trisToRemove[i];
		removedTriangle[t] = true;
        m_vTable[3*t    ] = -1;
        m_vTable[3*t + 1] = -1;
        m_vTable[3*t + 2] = -1;
    }

    std::vector<int> vertexReferences(m_vertices.size(), 0);
    for (int i = m_vTable.size()-1; i >= 0; i--) {
        if (m_vTable[i] >= 0) {
            vertexReferences[m_vTable[i]]++;
        }
        else {
            m_vTable.erase(m_vTable.begin() + i);
        }
    }

    std::vector<int> newVertexId(m_vertices.size(), -1);
    std::vector<bool> removeVtx(m_vertices.size(), false);
    int lastVertexId = 0;
    for (unsigned int i = 0; i < m_vertices.size(); i++) {
        if (vertexReferences[i] > 0) {
            newVertexId[i] = lastVertexId++;
        }
        else {
            removeVtx[i] = true;
        }
    }

    for (unsigned int i = 0; i < m_vTable.size(); i++) {
        m_vTable[i] = newVertexId[m_vTable[i]];
    }
	for (int i = removedTriangle.size()-1; i >= 0; i--) {
		if (removedTriangle[i]) {
			m_normals.erase(m_normals.begin() + i);
		}
	}
    for (int i = m_vertices.size()-1; i >= 0; i--) {
        if (removeVtx[i]) {
            m_vertices.erase(m_vertices.begin() + i);
        }
    }
}

