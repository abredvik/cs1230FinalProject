#pragma once

#include "shapes/shape.h"
#include <vector>
#include <glm/glm.hpp>

class Sphere : public Shape
{
public:
    Sphere() { setVertexData(); num_vertices = m_vertexData.size() / 6; };
    void updateParams(int param1, int param2);
    const std::vector<float>& generateShape() const { return m_vertexData; }
    const int getNumVertices() const { return num_vertices; }

private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void setVertexData();
    void makeTile(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void makeWedge(float currTheta, float nextTheta);
    void makeSphere();

    std::vector<float> m_vertexData;
    int num_vertices;
    float m_radius = 0.5;
    int m_param1;
    int m_param2;
};
